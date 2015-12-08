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
        private ortc_winrt_api::RTCIceGatherer iceGatherer { get; set; }

        private ortc_winrt_api::RTCIceTransport iceTransport { get; set; }

        static public void SetPreferredVideoCaptureFormat(int frame_width,
                                            int frame_height, int fps)
        {

        }

        public delegate void RTCPeerConnectionIceEventDelegate(RTCPeerConnectionIceEvent evt);
        public delegate void MediaStreamEventEventDelegate(MediaStreamEvent evt);
        public delegate void RTCPeerConnectionHealthStatsDelegate(RTCPeerConnectionHealthStats stats);

        public event RTCPeerConnectionIceEventDelegate OnIceCandidate;
        public event MediaStreamEventEventDelegate OnAddStream;
        public event MediaStreamEventEventDelegate OnRemoveStream;
        public event RTCPeerConnectionHealthStatsDelegate OnConnectionHealthStats;

        public RTCPeerConnection(RTCConfiguration configuration)
        {
            options = new RTCIceGatherOptions();
            options.IceServers = new List<RTCIceServer>();

            foreach (RTCIceServer server in configuration.IceServers)
            {
                ortc_winrt_api::RTCIceServer ortcServer = new ortc_winrt_api::RTCIceServer();

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

            iceGatherer = new ortc_winrt_api::RTCIceGatherer(options);
            iceGatherer.OnICEGathererStateChanged += OnICEGathererStateChanged;

            iceTransport = new ortc_winrt_api::RTCIceTransport(iceGatherer);
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
        }
        public Task<RTCSessionDescription> CreateOffer()//async
        {
            RTCSessionDescription ret = new RTCSessionDescription();
            ret.Type = Offer;

            return null;
        }
        public Task SetLocalDescription(RTCSessionDescription description) //async
        {
            return null;
        }

        public Task AddIceCandidate(RTCIceCandidate candidate) //async
        {
            return null;
        }

        private void OnICEGathererStateChanged(RTCIceGathererStateChangeEvent evt)
        {
            if (evt.State == RTCIceGathererState.State_Complete)
            {
                _iceTransport.start(_iceGatherer, _iceGatherer2.getLocalParameters(), RTCIceRole.Role_Controlled);
            }
        }

        private void RTCIceGatherer_onICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
        {
            _iceTransport2.addRemoteCandidate(evt.Candidate);
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
