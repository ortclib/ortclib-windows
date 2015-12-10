using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ortc_winrt_api;

namespace OrtcWrapper
{
    class Helper
    {
        public static string ToString(RTCIceCandidateType type)
        {
            string ret = "host";

            switch (type)
            {
                case RTCIceCandidateType.CandidateType_Host:
                    ret = "host";
                    break;
                case RTCIceCandidateType.CandidateType_Prflx:
                    ret = "prflx";
                    break;
                case RTCIceCandidateType.CandidateType_Relay:
                    ret = "relay";
                    break;
                case RTCIceCandidateType.CandidateType_Srflex:
                    ret = "srflex";
                    break;

                default:
                    break;
            }

            return ret;
        }
        public static RTCIceCandidate ToWrapperIceCandidate(ortc_winrt_api.RTCIceCandidate iceCandidate, int sdpComponentId)
        {
            StringBuilder sb = new StringBuilder();

            //candidate:704553097 1 udp 2122260223 192.168.1.3 62723 typ host generation 0

            sb.Append("candidate:");
            sb.Append(iceCandidate.Foundation);
            sb.Append(' ');
            sb.Append(sdpComponentId);
            sb.Append(' ');
            sb.Append(iceCandidate.Protocol == RTCIceProtocol.Protocol_UDP ? "udp" : "tcp");
            sb.Append(' ');
            sb.Append(iceCandidate.Priority);
            sb.Append(' ');
            sb.Append(iceCandidate.IP);
            sb.Append(' ');
            sb.Append(iceCandidate.Port);
            sb.Append(' ');
            sb.Append("typ");
            sb.Append(' ');
            sb.Append(ToString(iceCandidate.CandidateType));
            sb.Append(' ');
            sb.Append("generation");
            sb.Append(0);

            string sdpMid = "audio";
            UInt16 sdpMLineIndex = 0;
            var ret = new RTCIceCandidate(sb.ToString(),sdpMid,sdpMLineIndex);

            return ret; 
        }
    }
}
