using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Org.Ortc;
using Org.Ortc.Log;
using System.Net.NetworkInformation;
using System.Diagnostics;

namespace DataChannel.Net.Signaling
{
    public class OrtcSignaler :
        OrtcSignalerEvents,
        IDisposable
    {
        RTCIceGatherer _gatherer;
        RTCIceTransport _ice;   // Ice transport for the currently selected peer.
        RTCDtlsTransport _dtls;
        RTCSctpTransport _sctp;
        RTCDataChannel _dataChannel;    // Data channel for the currently selected peer.
        bool _isInitiator = true;      // True for the client that started the connection.

        private static readonly string _localPeerRandom = new Func<string>(() =>
        {
            Random random = new Random();   // WARNING: NOT cryptographically strong!
            const string chars = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            const int length = 5;
            return new string(Enumerable.Repeat(chars, length).Select(s => s[random.Next(s.Length)]).ToArray());
        })();

        RTCDataChannelParameters _dataChannelParams = new RTCDataChannelParameters
        {
            Label = "channel1",
            Negotiated = false,
            Ordered = true,
            Protocol = "ship"
        };

        static readonly Peer _localPeer = new Func<Peer>(() =>
        {
            string hostname = IPGlobalProperties.GetIPGlobalProperties().HostName;

            // A random string is added to the peer name to easily filter
            // our local peer by name when the server re-announces the
            // local peer to itself. Thus two peers with the same hostname
            // will never be the same and running the application again
            // causes a slightly different peer name on the peer list to
            // distinguish a new peer from an old zombie peer still not
            // yet purged from the server.
            string peerName = (hostname != null ? hostname : "<unknown host>") + "-" + _localPeerRandom + "-data";

            return new Peer(-1, peerName);
        })();

        public static Peer LocalPeer
        {
            // return a clone of the static local peer contents to ensure original Peer values cannot be modified
            get { return new Peer(_localPeer); }
        }

        private Peer _remotePeer;
        public Peer RemotePeer
        {
            // return a clone of the remote peer contents to ensure original Peer values cannot be modified
            get { return new Peer(_remotePeer); }
        }

        public bool IsInitiator
        {
            get { return _isInitiator; }
        }

        private bool _notifiedConnected = false;
        private bool _iceConnected = false;
        private bool _dataChannelConnected = false;

        private void NotifyConnectionState()
        {
            bool shouldNotify = (_iceConnected && _dataChannelConnected);
            if (shouldNotify)
            {
                if (!_notifiedConnected) OnDataChannelConnected();
            }
            else
            {
                if (_notifiedConnected) OnDataChannelDisonnected();
            }

            _notifiedConnected = shouldNotify;
        }

        private void NotifyIceConnected(bool connected)
        {
            _iceConnected = connected;
            NotifyConnectionState();
        }

        private void NotifyDataChannelConnected(bool connected)
        {
            _dataChannelConnected = connected;
            NotifyConnectionState();
        }

        static OrtcSignaler()
        {
            OrtcLib.Setup();
            Settings.ApplyDefaults();
#if ENABLE_ORTCLIB_LOGGING
            Logger.InstallEventingListener("secret", 58888, TimeSpan.FromSeconds(60));
            Logger.SetLogLevel(Org.Ortc.Log.Component.All, Org.Ortc.Log.Level.Trace);
            Logger.SetLogLevel(Org.Ortc.Log.Component.Eventing, Org.Ortc.Log.Level.Basic);
            Logger.InstallDebuggerLogger();
#endif
        }

        public OrtcSignaler(Peer remotePeer, bool isInitiator)
        {
            // make a clone of the remote peer contents in cause original values get modified
            _remotePeer = new Peer(remotePeer);
            _isInitiator = isInitiator;
        }

        ~OrtcSignaler()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            // clean all unmanaged resources (if any)

            if (disposing)
            {
                // clean all managed resources
                if (_dataChannel != null)
                {
                    _dataChannel.Close();
                    _dataChannel = null;
                }
                if (_sctp != null)
                {
                    _sctp.Stop();
                    _sctp = null;
                }
                if (_dtls != null)
                {
                    _dtls.Stop();
                    _dtls = null;
                }
                if (_ice != null)
                {
                    _ice.Stop();
                    _ice = null;
                }
                if (_gatherer != null)
                {
                    _gatherer.Close();
                    _gatherer = null;
                }
            }
        }

        public async Task SetupAsync()
        {
            await InitializeORTC();
            OpenDataChannel();
        }

        public void HandleSendMessageViaDataChannel(string message)
        {
            if (_dataChannel == null)
            {
                Debug.WriteLine("Attempting to send data where no data channel exists");
                return;
            }
            _dataChannel.Send(message);
        }

        public async Task HandleMessageFromPeer(string message)
        {
            if (message.StartsWith("{\"candidate"))
            {
                Debug.WriteLine("contains IceCandidate (or IceCandidateComplete)");

                Json jsonMessage = new Json(message);
                RTCIceGathererCandidate remoteCandidate = RTCIceGathererCandidate.Create(jsonMessage);
                _ice.AddRemoteCandidate(remoteCandidate);
                return;
            }
            if (message.StartsWith("{\"RTCIceParameters\":"))
            {
                Debug.WriteLine("contains IceParameters");

                Json jsonMessage = new Json(message);
                RTCIceParameters iceParameters = new RTCIceParameters(jsonMessage);

                // Start the ice transport with the appropriate role based on whether this is the initiator of the call.
                var role = _isInitiator ? RTCIceRole.Controlling : RTCIceRole.Controlled;
                _ice.Start(_gatherer, iceParameters, role);
                return;
            }
            if (message.StartsWith("{\"RTCDtlsParameters\":"))
            {
                Debug.WriteLine("contains DtlsParameters");

                Json jsonMessage = new Json(message);
                RTCDtlsParameters dtlsParameters = new RTCDtlsParameters(jsonMessage);

                _dtls.Start(dtlsParameters);
                Debug.WriteLine("Dtls start called.");
                return;
            }
            // this order guarantees:
            if (message.StartsWith("{\"RTCSctpCapabilities\":"))
            {
                Debug.WriteLine("contains SctpCapabilities");
                Json jsonMessage = new Json(message);
                // Message ordering: alice -> bob; bob.start(); bob -> alice; alice.start(); alice -> datachannel -> bob
                RTCSctpCapabilities sctpCaps = new RTCSctpCapabilities(jsonMessage);

                if (!_isInitiator)
                {
                    Debug.WriteLine("Receiver: Waiting for OnDataChannel event and starting sctp.");

                    // The remote side will receive notification when the data channel is opened.
                    _sctp.OnDataChannel += Sctp_OnDataChannel;
                    _sctp.OnStateChange += Sctp_OnStateChange;

                    _sctp.Start(sctpCaps);

                    RTCSctpCapabilities caps = RTCSctpTransport.GetCapabilities();

                    OnSendMessageToPeer(caps.ToJson().ToString());
                }
                else
                {
                    // The initiator has received sctp caps back from the remote peer, 
                    // which means the remote peer has already called sctp.start(). 
                    // It's now safe to open a data channel, which will fire the 
                    // Sctp_OnDataChannel event on the remote side.
                    Debug.WriteLine("Initiator: Creating the data channel and starting sctp.");

                    _sctp.OnStateChange += Sctp_OnStateChange;
                    _sctp.Start(sctpCaps);

                    RTCDataTransport data = RTCDataTransport.Cast(_sctp);

                    _dataChannel = new RTCDataChannel(data, _dataChannelParams);
                    _dataChannel.OnMessage += DataChannel_OnMessage;
                    _dataChannel.OnError += DataChannel_OnError;
                    _dataChannel.OnOpen += DataChannel_OnOpen;
                    _dataChannel.OnClose += DataChannel_OnClose;
                }
            }
        }

        private async Task InitializeORTC()
        {
            var gatherOptions = new RTCIceGatherOptions()
            {
                IceServers = new List<RTCIceServer>()
                {
                    new RTCIceServer { Urls = new string[] { "stun.l.google.com:19302" } },
                    new RTCIceServer { Username = "redmond", Credential = "redmond123",
                        CredentialType = RTCIceCredentialType.Password,
                        Urls = new string[] { "turn:turn-testdrive.cloudapp.net:3478?transport=udp" } }
                }
            };

            _gatherer = new RTCIceGatherer(gatherOptions);

            _gatherer.OnStateChange += IceGatherer_OnStateChange;

            _gatherer.OnLocalCandidate += (@event) =>
            {
                OnSendMessageToPeer(@event.Candidate.ToJson().ToString());
            };
            _gatherer.OnLocalCandidateComplete += (@event) =>
            {
                OnSendMessageToPeer(@event.Candidate.ToJson().ToString());
            };

            var cert = await RTCCertificate.GenerateCertificate();

            _ice = new RTCIceTransport(_gatherer);
            _ice.OnStateChange += IceTransport_OnStateChange;

            _dtls = new RTCDtlsTransport(_ice, new RTCCertificate[] { cert });
            _dtls.OnStateChange += Dtls_OnStateChange;

            _sctp = new RTCSctpTransport(_dtls);

            _gatherer.Gather(null);
        }

        /// <summary>
        /// Establishes a DataChannel with the parameter peer.
        /// </summary>
        private void OpenDataChannel()
        {
            Debug.WriteLine($"Opening data channel to peer id: {RemotePeer.Id}");

            var iceParams = _gatherer.LocalParameters;
            OnSendMessageToPeer(iceParams.ToJson().ToString());

            // this order guarantees: alice -> bob; bob.start(); bob -> alice; alice.start(); alice -> datachannel -> bob
            if (_isInitiator)
            {
                var sctpCaps = RTCSctpTransport.GetCapabilities();
                OnSendMessageToPeer(sctpCaps.ToJson().ToString());
            }
            var dtlsParams = _dtls.LocalParameters;
            OnSendMessageToPeer(dtlsParams.ToJson().ToString());
        }


        private void IceGatherer_OnStateChange(RTCIceGathererStateChangeEvent evt)
        {
            Debug.WriteLine("IceGatherer State Change: " + evt.State);
        }

        private void IceTransport_OnStateChange(RTCIceTransportStateChangeEvent evt)
        {
            Debug.WriteLine("IceTransport State Change: " + evt.State);
            switch (evt.State)
            {
                case RTCIceTransportState.Connected:
                case RTCIceTransportState.Completed:
                {
                    NotifyIceConnected(true);
                    break;
                }
                default:
                {
                    NotifyIceConnected(false);
                    break;
                }
            }
        }

        private void Dtls_OnStateChange(RTCDtlsTransportStateChangeEvent evt)
        {
            Debug.WriteLine("Dtls State Change: " + evt.State);
        }

        private void Sctp_OnStateChange(RTCSctpTransportStateChangeEvent evt)
        {
            Debug.WriteLine("Sctp State Change: " + evt.State);
        }

        private void Sctp_OnDataChannel(RTCDataChannelEvent evt)
        {
            Debug.WriteLine("Sctp OnDataChannel");

            _dataChannel = evt.DataChannel;
            _dataChannel.OnMessage += DataChannel_OnMessage;
            _dataChannel.OnError += DataChannel_OnError;
            _dataChannel.OnClose += DataChannel_OnClose;

            NotifyDataChannelConnected(true);
        }

        private void DataChannel_OnOpen()
        {
            NotifyDataChannelConnected(true);
        }

        private void DataChannel_OnClose()
        {
            NotifyDataChannelConnected(false);
        }

        private void DataChannel_OnError(ErrorEvent evt)
        {
            Debug.WriteLine("DataChannel error: " + evt.Error);
        }

        private void DataChannel_OnMessage(RTCMessageEvent evt)
        {
            Debug.WriteLine("Datachannel message: " + evt.Text);

            OnDataChannelMessage(evt.Text);
        }

    }
}
