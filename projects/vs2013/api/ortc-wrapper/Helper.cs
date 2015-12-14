using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
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
        public static RTCIceCandidateType ToIceCandidateType(string type)
        {
            RTCIceCandidateType ret = RTCIceCandidateType.CandidateType_Host;

            switch (type)
            {
                case "host":
                    ret = RTCIceCandidateType.CandidateType_Host;
                break;
                case "prflx":
                    ret = RTCIceCandidateType.CandidateType_Prflx;
                    break;
                case "relay":
                    ret = RTCIceCandidateType.CandidateType_Relay;
                    break;
                case "srflex":
                    ret = RTCIceCandidateType.CandidateType_Srflex;
                    break;

                default:
                    ret = RTCIceCandidateType.CandidateType_Host;
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
            sb.Append(' ');
            sb.Append(0);

            string sdpMid = "audio";
            UInt16 sdpMLineIndex = 0;
            var ret = new RTCIceCandidate(sb.ToString(),sdpMid,sdpMLineIndex);

            ortc_winrt_api.RTCIceCandidate iceCandidate2 = iceCandidateFromSdp(sb.ToString());

            return ret; 
        }

        public static ortc_winrt_api.RTCIceCandidate iceCandidateFromSdp(string sdp)
        {
            var ice = new ortc_winrt_api.RTCIceCandidate();

            //candidate:704553097 1 udp 2122260223 192.168.1.3 62723 typ host generation 0
            TextReader reader = new StringReader(sdp);
            string line = reader.ReadLine();

            if (!String.IsNullOrEmpty(line))
            {
                string[] substrings = line.Split(' ');

                if (substrings.Length == 10)
                {
                    ice.Foundation = substrings[0];
                    ice.Protocol = String.Equals(substrings[2],"udp") ? RTCIceProtocol.Protocol_UDP : RTCIceProtocol.Protocol_TCP;
                    ice.Priority = uint.Parse(substrings[3]);
                    ice.IP = substrings[4];
                    ice.Port = ushort.Parse(substrings[5]);
                    ice.CandidateType = ToIceCandidateType(substrings[7]);
                }
            }
            return ice;
        }
        public static void createFromSDP(string sdp)
        {
            TextReader reader = new StringReader(sdp);

            string line = reader.ReadLine();
            while (line != null)
            {
                if ((line.Length == 0))
                {
                    line = reader.ReadLine();
                    continue;
                }
                if ( (line.Length < 3) || (line[1] != '=') )
                {
                    throw new Exception(string.Format("Invalid Line {0}", line));
                }
                string value = line.Substring(2);
                string[] parts = null;
                int sep = -1;
                try
                {
                    switch (line[0])
                    {
                        case 'v':
                            int version = 0;
                            if (!int.TryParse(value, out version))
                            {
                                throw new Exception(string.Format("Invalid Line {0}", line));
                            }
                            if (version != 0)
                            {
                                throw new Exception(string.Format("Not supported version"));
                            }
                            break;

                        case 'o':
                            break;
                        case 's':
                            break;
                        case 't':
                            break;

                        case 'c':
                            parts = value.Split(' ');
                            if (parts.Length != 3)
                            {
                                throw new Exception(string.Format("Invalid Line {0}", line));
                            }
                            string networkType = parts[0];
                            string addressType = parts[1];
                            parts = parts[2].Split('/');
                            if (parts.Length > 3)
                            {
                                throw new Exception(string.Format("Invalid Line {0}", line));
                            }
                            //TODO: Check if this is required
                            break;
                        case 'a':
                            sep = value.IndexOf(':');
                            string attrName;
                            string attrValue;
                            ortc_winrt_api.RTCIceCandidate iceCandidate;
                            if (sep != -1)
                            {
                                attrName = value.Substring(0, sep);
                                attrValue = value.Substring(sep + 1);
                           
                                if (attrName.Equals("candidate"))
                                {
                                    iceCandidate = iceCandidateFromSdp(attrValue);
                                }
                            }
                            else
                            {
                                attrName = value;
                                attrValue = null;
                            }
                            /*if (media != null)
                            {
                                media.Attributes.Add(attrName, attrValue);
                            }
                            else
                            {
                                sd.Attributes.Add(attrName, attrValue);
                            }*/
                            break;
                        case 'm':
                            /*parts = value.Split(' ');
                            if (parts.Length < 4)
                            {
                                goto invalidline;
                            }
                            string mediaType = parts[0];
                            string protocol = parts[2];
                            var formats = parts.Skip(3).ToList();
                            parts = parts[1].Split('/');
                            if (parts.Length > 2)
                            {
                                goto invalidline;
                            }
                            uint port = 0;
                            uint portCount = 1;
                            Grammar.ValidateDigits(parts[0], false);
                            if (!uint.TryParse(parts[0], out port))
                            {
                                goto invalidline;
                            }
                            if (parts.Length == 2)
                            {
                                Grammar.ValidateDigits(parts[1], true);
                                if (!uint.TryParse(parts[1], out portCount))
                                {
                                    goto invalidline;
                                }
                            }
                            media = new Media(mediaType, port, portCount, protocol, formats);
                            sd.Medias.Add(media);*/
                            break;
                    }
                }
                catch
                {
                    throw new Exception(string.Format("Invalid Line {0}", line));
                }
                line = reader.ReadLine();
            }
        }
    }
}
