using DataChannel.Net.Signaling;
using Org.Ortc;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DataChannel.Net
{
    public partial class PeersListForm : Form
    {
        RTCIceGatherer _gatherer;
        RTCIceTransport _ice;   // Ice transport for the currently selected peer.
        RTCDtlsTransport _dtls;
        RTCSctpTransport _sctp;
        public RTCDataChannel _dataChannel;    // Data channel for the currently selected peer.
        bool _isInitiator = false;      // True for the client that started the connection.

        RTCDataChannelParameters _dataChannelParams = new RTCDataChannelParameters
        {
            Label = "channel1",
            Negotiated = false,
            Ordered = true,
            Protocol = "ship"
        };

        private readonly HttpSignaler _httpSignaler;
        public HttpSignaler HttpSignaler => _httpSignaler;

        private Peer _remotePeer;
        public Peer RemotePeer
        {
            get
            {
                if (_remotePeer == null)
                    _remotePeer = SelectedPeer;
                return _remotePeer;
            }
            set
            {
                if (_remotePeer == value)
                    return;
                _remotePeer = value;
            }
        }

        private Peer _selectedPeer;
        public Peer SelectedPeer
        {
            get { return _selectedPeer; }
            set
            {
                if (_selectedPeer == value)
                    return;

                var oldValue = _selectedPeer;
                _selectedPeer = value;
                SelectedPeerChanged(oldValue, value);
            }
        }

        private string _message = string.Empty;
        public string Message
        {
            get { return _message; }
            set
            {
                if (_message == value)
                    return;

                _message = value;
            }
        }

        private void SelectedPeerChanged(Peer oldValue, Peer value)
        {
            if (_dataChannel != null)
            {
                _dataChannel.Close();
                _sctp.Stop();
                _dtls.Stop();
                _ice.Stop();
            }

            Message = string.Empty;
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

            _gatherer.OnLocalCandidate += async (candidate) =>
            {
                await _httpSignaler.SendToPeer(RemotePeer.Id, candidate.Candidate.ToString());
            };

            var cert = await RTCCertificate.GenerateCertificate();

            _ice = new RTCIceTransport(_gatherer);
            _ice.OnStateChange += IceTransport_OnStateChange;

            _dtls = new RTCDtlsTransport(_ice, new RTCCertificate[] { cert });
            _dtls.OnStateChange += Dtls_OnStateChange;

            _sctp = new RTCSctpTransport(_dtls);

            _gatherer.Gather(null);
        }

        private void IceGatherer_OnStateChange(RTCIceGathererStateChangeEvent evt)
        {
            Debug.WriteLine("IceGatherer State Change: " + evt.State);
        }

        private void IceTransport_OnStateChange(RTCIceTransportStateChangeEvent evt)
        {
            Debug.WriteLine("IceTransport State Change: " + evt.State);
        }

        private void Dtls_OnStateChange(RTCDtlsTransportStateChangeEvent evt)
        {
            Debug.WriteLine("Dtls State Change: " + evt.State);
        }

        static PeersListForm()
        {
            OrtcLib.Setup();
            Settings.ApplyDefaults();
        }

        public PeersListForm()
        {
            InitializeComponent();

            lstPeers.SelectedIndex = -1;

            var name = GetLocalPeerName();
            Debug.WriteLine($"Connecting to server from local peer: {name}");

            _httpSignaler =
                new HttpSignaler("peercc-server.ortclib.org", 8888, name);

            _httpSignaler.SignedIn += Signaler_SignedIn;
            _httpSignaler.ServerConnectionFailed += Signaler_ServerConnectionFailed;
            _httpSignaler.PeerConnected += Signaler_PeerConnected;
            _httpSignaler.PeerDisconnected += Signaler_PeerDisconnected;
            _httpSignaler.MessageFromPeer += Signaler_MessageFromPeer;
        }

        private void Signaler_SignedIn(object sender, EventArgs e)
        {
            this.BeginInvoke((Action)(() =>
            {
                Signaler_HandleSignedIn(sender, e);
            }));
        }

        private void Signaler_HandleSignedIn(object sender, EventArgs e)
        {
            Debug.WriteLine("Peer signed in to server.");
        }

        private void Signaler_ServerConnectionFailed(object sender, EventArgs e)
        {
            this.BeginInvoke((Action)(() =>
            {
                Signaler_HandleServerConnectionFailed(sender, e);
            }));
        }

        private void Signaler_HandleServerConnectionFailed(object sender, EventArgs e)
        {
            Debug.WriteLine("Server connection failure.");
        }

        private void Signaler_PeerConnected(object sender, Peer peer)
        {
            this.BeginInvoke((Action)(() =>
            {
                Signaler_HandlePeerConnected(sender, peer);
            }));
        }

        private void Signaler_HandlePeerConnected(object sender, Peer peer)
        {
            Debug.WriteLine($"Peer connected {peer.Name} / {peer.Id}");

            HttpSignaler._peers.Add(peer);

            if (SelectedPeer == null)
                SelectedPeer = peer;

            var listPeers = HttpSignaler._peers.ToList();
            var x = listPeers.Count;
            if (x > 0 && peer.Name != GetLocalPeerName())
                lstPeers.Items.Add(peer.ToString());
        }

        private void Signaler_PeerDisconnected(object sender, Peer peer)
        {
            this.BeginInvoke((Action)(() =>
            {
                Signaler_HandlePeerDisconnected(sender, peer);
            }));
        }

        private void Signaler_HandlePeerDisconnected(object sender, Peer peer)
        {
            Debug.WriteLine($"Peer disconnected {peer.Name} / {peer.Id}");

            HttpSignaler._peers.Remove(peer);

            for (int n = lstPeers.Items.Count - 1; n >= 0; --n)
            {
                string removePeer = peer.ToString();
                if (lstPeers.Items[n].ToString().Contains(removePeer))
                {
                    lstPeers.Items.RemoveAt(n);
                }
            }
        }

        private void Signaler_MessageFromPeer(object sender, Peer peer)
        {
            this.BeginInvoke((Action) (() =>
            {
                Signaler_HandleMessageFromPeer(sender, peer).ContinueWith((antecedent) =>
                {
                    Debug.WriteLine("Message from peer handled: " + peer.Message);
                });
            }));
        }

        private async Task Signaler_HandleMessageFromPeer(object sender, Peer peer)
        {
            var message = peer.Message;

            if (message.Contains("OpenDataChannel"))
            {
                Debug.WriteLine("contains OpenDataChannel");

                await InitializeORTC();

                // A peer has let us know that they have begun initiating a data channel. 
                // In this scenario, we are the "remote" peer so make sure _isInitiator is false. 
                // Begin gathering ice candidates.
                _isInitiator = false;
                RemotePeer = peer;
                await OpenDataChannel(peer);

                ChatForm chatForm = new ChatForm(RemotePeer);
                chatForm.ShowDialog();

                return;
            }

            if (message.Contains("IceCandidate"))
            {
                Debug.WriteLine("contains IceCandidate");

                Json jsonMessage = new Json(message);
                RTCIceGathererCandidate remoteCandidate = RTCIceGathererCandidate.Create(jsonMessage);
                _ice.AddRemoteCandidate(remoteCandidate);
                return;
            }
            if (message.Contains("IceParameters"))
            {
                Debug.WriteLine("contains IceParameters");

                Json jsonMessage = new Json(message);
                RTCIceParameters iceParameters = new RTCIceParameters(jsonMessage);

                // Start the ice transport with the appropriate role based on whether this is the initiator of the call.
                var role = _isInitiator ? RTCIceRole.Controlling : RTCIceRole.Controlled;
                _ice.Start(_gatherer, iceParameters, role);
                return;
            }
            if (message.Contains("DtlsParameters"))
            {
                Debug.WriteLine("contains DtlsParameters");

                Json jsonMessage = new Json(message);
                RTCDtlsParameters dtlsParameters = new RTCDtlsParameters(jsonMessage);

                _dtls.Start(dtlsParameters);
                Debug.WriteLine("Dtls start called.");
                return;
            }
            // this order guarantees:
            if (message.Contains("SctpCapabilities"))
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

                    await _httpSignaler.SendToPeer(RemotePeer.Id, caps.ToString());
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
                    _dataChannel.OnStateChange += DataChannel_OnStateChange;
                }
            }
        }

        private void DataChannel_OnStateChange(RTCDataChannelStateChangeEvent evt)
        {
            throw new NotImplementedException();
        }

        private void DataChannel_OnError(ErrorEvent evt)
        {
            Debug.WriteLine("DataChannel error: " + evt.Error);
        }

        private void DataChannel_OnMessage(RTCMessageEvent evt)
        {
            Debug.WriteLine("Datachannel message: " + evt.Text);
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
            _dataChannel.OnStateChange += DataChannel_OnStateChange;
        }

        /// <summary>
        /// Establishes a DataChannel with the parameter peer.
        /// </summary>
        private async Task OpenDataChannel(Peer peer)
        {
            Debug.WriteLine($"Opening data channel to peer id: {peer.Id}");

            var iceParams = _gatherer.LocalParameters;
            await _httpSignaler.SendToPeer(peer.Id, iceParams.ToString());

            // this order guarantees: alice -> bob; bob.start(); bob -> alice; alice.start(); alice -> datachannel -> bob
            if (_isInitiator)
            {
                var sctpCaps = RTCSctpTransport.GetCapabilities();
                await _httpSignaler.SendToPeer(peer.Id, sctpCaps.ToString());
            }
            var dtlsParams = _dtls.LocalParameters;
            await _httpSignaler.SendToPeer(peer.Id, dtlsParams.ToString());
        }

        private async void btnConnect_Click(object sender, EventArgs e)
        {
            Debug.WriteLine("Connects to server!");

            await HttpSignaler.Connect();

            btnConnect.Enabled = false;
            btnDisconnect.Enabled = true;
        }

        private async void btnDisconnect_Click(object sender, EventArgs e)
        {
            Debug.WriteLine("Disconnects from server!");

            await _httpSignaler.SignOut();
            lstPeers.Items.Clear();

            btnDisconnect.Enabled = false;
            btnConnect.Enabled = true;
        }

        private string GetLocalPeerName()
        {
            string hostname = IPGlobalProperties.GetIPGlobalProperties().HostName;
            return (hostname != null ? hostname : "<unknown host>") + "-dual";
        }

        private async void btnChat_Click(object sender, EventArgs e)
        {
            if (lstPeers.SelectedIndex != -1)
            {
                await InitializeORTC();

                var text = lstPeers.GetItemText(lstPeers.SelectedItem);

                string[] separaingChars = { ":" };
                string[] words = text.Split(separaingChars, StringSplitOptions.RemoveEmptyEntries);

                SelectedPeer.Id = Convert.ToInt32(words[0]);
                SelectedPeer.Name = words[1];

                RemotePeer = SelectedPeer;

                await _httpSignaler.SendToPeer(RemotePeer.Id, "OpenDataChannel");

                await OpenDataChannel(SelectedPeer);

                ChatForm chatForm = new ChatForm(RemotePeer);
                chatForm.ShowDialog();
            }
            else
            {
                MessageBox.Show("Select peer!");
            }
        }
    }
}
