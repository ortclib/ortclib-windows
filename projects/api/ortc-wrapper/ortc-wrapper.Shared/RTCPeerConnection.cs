using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ortc_winrt_api;

namespace OrtcWrapper
{
    public class RTCPeerConnection
    {
        private RTCIceGatherOptions options { get; set; }
        private RTCIceGatherer iceGatherer { get; set; }
        private RTCIceGatherer iceGathererRTCP   { get; set; }

        private RTCIceTransport iceTransport { get; set; }

        static public void SetPreferredVideoCaptureFormat(int frame_width,
                                            int frame_height, int fps)
        {

        }

        public delegate void RTCPeerConnectionIceEventDelegate(RTCPeerConnectionIceEvent evt);
        public delegate void MediaStreamEventEventDelegate(MediaStreamEvent evt);
        public delegate void RTCPeerConnectionHealthStatsDelegate(RTCPeerConnectionHealthStats stats);

        public event RTCPeerConnectionIceEventDelegate _OnIceCandidate;
        public event RTCPeerConnectionIceEventDelegate OnIceCandidate
        {
            add
            {
                _OnIceCandidate += value;
                //iceGatherer.OnICEGathererLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate;
            }
            remove
            {
                _OnIceCandidate -= value;
                //iceGatherer.OnICEGathererLocalCandidate -= this.RTCIceGatherer_onICEGathererLocalCandidate;
            }
        }
        public event MediaStreamEventEventDelegate OnAddStream;
        public event MediaStreamEventEventDelegate OnRemoveStream;
        public event RTCPeerConnectionHealthStatsDelegate OnConnectionHealthStats;

        public RTCPeerConnection(RTCConfiguration configuration)
        {
            ConfigureOrtcEngine conf = new ConfigureOrtcEngine();

            options = new RTCIceGatherOptions();
            options.IceServers = new List<ortc_winrt_api.RTCIceServer>();

            foreach (RTCIceServer server in configuration.IceServers)
            {
                ortc_winrt_api.RTCIceServer ortcServer = new ortc_winrt_api.RTCIceServer();
                ortcServer.URLs = new List<string>();

                if (!string.IsNullOrEmpty(server.Credential))
                {
                    ortcServer.Credential = server.Credential;
                }

                if (!string.IsNullOrEmpty(server.Username))
                {
                    ortcServer.UserName = server.Username;
                }

                ortcServer.URLs.Add(server.Url);
                options.IceServers.Add(ortcServer);
            }
        }

        /// <summary>
        /// Enable/Disable WebRTC statistics to ETW.
        /// </summary>
        public void ToggleETWStats(bool enable)
        {

        }

        /// <summary>
        /// Enable/Disable connection health statistics.
        /// When new connection health stats are available OnConnectionHealthStats
        //  event is raised.
        /// </summary>
        public void ToggleConnectionHealthStats(bool enable)
        {

        }

        /// <summary>
        /// Adds a new local <see cref="MediaStream"/> to be sent on this connection.
        /// </summary>
        /// <param name="stream"><see cref="MediaStream"/> to be added.</param>
        public void AddStream(MediaStream stream)
        {

        }

        public void Close()
        {

        }

        public Task SetRemoteDescription(RTCSessionDescription description)
        {
            return null;
        }

        public Task<RTCSessionDescription> CreateAnswer()//async
        {
            return null;
        }

        private UInt64 sessionID { get; set; }
        private UInt16 sessionVersion { get; set; }
        private string streamID { get; set; }
        string createSDP()
        {
            StringBuilder sb = new StringBuilder();

            //------------- Global lines START -------------
            //v=0
            sb.Append("v=");
            sb.Append(0);
            sb.Append("\r\n");

            //o=- 1045717134763489491 2 IN IP4 127.0.0.1
            sb.Append("o=");
            sb.Append(sessionID);
            sb.Append(' ');
            sb.Append(sessionVersion);
            sb.Append(' ');
            sb.Append("IN");
            sb.Append(' ');
            sb.Append("IP4");
            sb.Append(' ');
            sb.Append("127.0.0.1");
            sb.Append("\r\n");

            //s=-
            sb.Append("s=");
            sb.Append("-");
            sb.Append("\r\n");

            //t=0 0
            sb.Append("t=");
            sb.Append(0);
            sb.Append(' ');
            sb.Append(0);
            sb.Append("\r\n");

            //a=group:BUNDLE audio video
            sb.Append("a=");
            sb.Append("group:BUNDLE");
            sb.Append(' ');
            sb.Append("audio");
            sb.Append(' ');
            sb.Append("video");
            sb.Append("\r\n");

            //a=msid-semantic: WMS stream_label_ce8753d3
            sb.Append("a=");
            sb.Append("msid-semantic:");
            sb.Append(' ');
            sb.Append("WMS");
            sb.Append(' ');
            sb.Append("streamID");
            sb.Append("\r\n");
            //------------- Global lines END -------------


            //------------- Media lines START -------------
            //m = audio 9 UDP / TLS / RTP / SAVPF 111 103 104 9 102 0 8 106 105 13 127 126

            return sb.ToString();
        }
        public async Task<RTCSessionDescription> CreateOffer()
        {
            Task<RTCSessionDescription> ret = Task.Run<RTCSessionDescription>(() =>
            {
                PrepareGatherer();
                RTCSessionDescription sd = new RTCSessionDescription(RTCSdpType.Offer, this.createSDP());
                return sd;
            });

            return null;
        }
        public Task SetLocalDescription(RTCSessionDescription description) //async
        {
            Task ret = Task.Run(() =>
            {
                //TODO update modifications
            });
            return ret;
        }

        public Task AddIceCandidate(RTCIceCandidate candidate) //async
        {
            return null;
        }

        private void PrepareGatherer()
        {
            try
            {
                iceGatherer = new ortc_winrt_api.RTCIceGatherer(options);
                iceGatherer.OnICEGathererStateChanged += OnICEGathererStateChanged;
                iceGatherer.OnICEGathererLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate;
                iceGatherer.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
                iceGatherer.OnICEGathererLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone;
                iceGatherer.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;

                iceGathererRTCP = iceGatherer.createAssociatedGatherer();
                //iceGathererRTCP.OnICEGathererStateChanged += OnICEGathererStateChanged;
                iceGathererRTCP.OnICEGathererLocalCandidate += this.RTCIceGatherer_onRTCPICEGathererLocalCandidate;
                //iceGathererRTCP.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
                //iceGathererRTCP.OnICEGathererLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone;
                //iceGathererRTCP.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;
            }
            catch (Exception e)
            {
                return;
            }
            iceTransport = new ortc_winrt_api.RTCIceTransport(iceGatherer);
        }

        private void OnICEGathererStateChanged(RTCIceGathererStateChangeEvent evt)
        {
            if (evt.State == RTCIceGathererState.State_Complete)
            {
                
            }
        }

        private void RTCIceGatherer_onICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
        {
            var iceEvent = new RTCPeerConnectionIceEvent();
            iceEvent.Candidate = Helper.ToWrapperIceCandidate(evt.Candidate, 1); //RTP component
            if (_OnIceCandidate != null)
                _OnIceCandidate(iceEvent);
        }

        private void RTCIceGatherer_onRTCPICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
        {
            var iceEvent = new RTCPeerConnectionIceEvent();
            iceEvent.Candidate = Helper.ToWrapperIceCandidate(evt.Candidate, 2); //RTCP component
            if (_OnIceCandidate != null)
                _OnIceCandidate(iceEvent);
        }

        private void RTCIceGatherer_onICEGathererCandidateComplete(RTCIceGathererCandidateCompleteEvent evt)
        {
            int i = 0;
            i++;
        }

        private void RTCIceGatherer_onICEGathererLocalCandidateGone(RTCIceGathererCandidateEvent evt)
        {
            int i = 0;
            i++;
        }

        private void RTCIceGatherer_onICEGathererError(RTCIceGathererErrorEvent evt)
        {
            int i = 0;
            i++;
        }
    }
}
