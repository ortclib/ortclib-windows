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
                case RTCIceCandidateType.Host:
                    ret = "host";
                    break;
                case RTCIceCandidateType.Prflx:
                    ret = "prflx";
                    break;
                case RTCIceCandidateType.Relay:
                    ret = "relay";
                    break;
                case RTCIceCandidateType.Srflex:
                    ret = "srflex";
                    break;

                default:
                    break;
            }

            return ret;
        }
        public static RTCIceCandidateType ToIceCandidateType(string type)
        {
            RTCIceCandidateType ret = RTCIceCandidateType.Host;

            switch (type)
            {
                case "host":
                    ret = RTCIceCandidateType.Host;
                break;
                case "prflx":
                    ret = RTCIceCandidateType.Prflx;
                    break;
                case "relay":
                    ret = RTCIceCandidateType.Relay;
                    break;
                case "srflex":
                    ret = RTCIceCandidateType.Srflex;
                    break;

                default:
                    ret = RTCIceCandidateType.Host;
                    break;
            }
            return ret;
        }

        public static List<MediaDeviceInfo> Filter(
            MediaDeviceKind kind,
            IList<MediaDeviceInfo> infos
            )
        {
            var results = new List<MediaDeviceInfo>();
            foreach (var info in infos)
            {
                if (kind != info.Kind) continue;
                results.Add(info);
            }
            return results;
        }
        public static MediaDevice ToMediaDevice(MediaDeviceInfo device)
        {
            return new MediaDevice(device.DeviceID, device.Label);
        }
        public static IList<MediaDevice> ToMediaDevices(IList<MediaDeviceInfo> devices)
        {
            var results = new List<MediaDevice>();
            foreach (var device in devices)
            {
                results.Add(ToMediaDevice(device));
            }
            return results;
        }
        public static CodecInfo ToDto(RTCRtpCodecCapability codec, int index)
        {
            return new CodecInfo(index, (int)codec.ClockRate, codec.Name);
        }
        public static IList<CodecInfo> GetCodecs(string kind)
        {
            var caps = RTCRtpSender.GetCapabilities(kind);
            var codecs = caps.Codecs;
            var results = new List<CodecInfo>();

            int index = 0;
            foreach (var codec in codecs)
            {
                ++index;
                results.Add(ToDto(codec, index));
            }
            return results;
        }

        public static Constraints MakeConstraints(
            bool shouldDoThis,
            Constraints existingConstraints,
            MediaDeviceKind kind,
            MediaDevice device
            )
        {
            if (!shouldDoThis) return existingConstraints;
            if (null == device) return existingConstraints;

            if (null == existingConstraints) existingConstraints = new Constraints();
            MediaTrackConstraints trackConstraints = null;

            switch (kind)
            {
                case MediaDeviceKind.AudioInput: trackConstraints = existingConstraints.Audio; break;
                case MediaDeviceKind.AudioOutput: trackConstraints = existingConstraints.Audio; break;
                case MediaDeviceKind.Video: trackConstraints = existingConstraints.Video; break;
            }
            if (null == trackConstraints) trackConstraints = new MediaTrackConstraints();

            if (null == trackConstraints.Advanced) trackConstraints.Advanced = new List<MediaTrackConstraintSet>();
           
            var constraintSet = new MediaTrackConstraintSet();
            constraintSet.DeviceId = new ConstrainString();
            constraintSet.DeviceId.Parameters = new ConstrainStringParameters();
            constraintSet.DeviceId.Parameters.Exact = new StringOrStringList();
            constraintSet.DeviceId.Parameters.Exact.Value = device.Id;
            constraintSet.DeviceId.Value = new StringOrStringList();
            constraintSet.DeviceId.Value.Value = device.Id;
#warning  TODO Removed hardcoded values
            constraintSet.Height = new ConstrainLong();
            constraintSet.Height.Value = 600;

            constraintSet.Width = new ConstrainLong();
            constraintSet.Width.Value = 800;

            constraintSet.FrameRate = new ConstrainDouble();
            constraintSet.FrameRate.Value = 30;

            constraintSet.SampleRate = new ConstrainLong();
            constraintSet.SampleRate.Value = 48000;

            trackConstraints.Advanced.Add(constraintSet);

            switch (kind)
            {
                case MediaDeviceKind.AudioInput: existingConstraints.Audio = trackConstraints; break;
                case MediaDeviceKind.AudioOutput: existingConstraints.Audio = trackConstraints; break;
                case MediaDeviceKind.Video: existingConstraints.Video = trackConstraints; break;
            }
            return existingConstraints;
        }

        public static MediaStreamTrack findTrack(
            IList<MediaStreamTrack> tracks,
            MediaDevice device
            )
        {
            if (null == device) return null;
            foreach (var track in tracks) { if (track.DeviceId != device.Id) return track; }
            return null;
        }

        public static MediaStreamTrack findTrack(
            IList<MediaStreamTrack> tracks,
            MediaStreamTrackKind kind
            )
        {
            //if (null == device) return null;
            foreach (var track in tracks) { if (track.Kind == kind) return track; }
            return null;
        }
        public static List<MediaAudioTrack> InsertAudioIfValid(
            bool shouldDoThis,
            List<MediaAudioTrack> existingList,
            IList<MediaStreamTrack> tracks,
            MediaDevice device
            )
        {
            if (!shouldDoThis) return existingList;
            if (null == device) return existingList;
            if (null == tracks) return existingList;

            var found = findTrack(tracks, MediaStreamTrackKind.Audio);
            if (null == found) return existingList;
            if (null == existingList) existingList = new List<MediaAudioTrack>();
            //existingList.Add(new MediaAudioTrack(found.Id, found.Enabled));
            existingList.Add(new MediaAudioTrack(found));
            return existingList;
        }
        public static List<MediaVideoTrack> InsertVideoIfValid(
            bool shouldDoThis,
            List<MediaVideoTrack> existingList,
            IList<MediaStreamTrack> tracks,
            MediaDevice device
            )
        {
            if (!shouldDoThis) return existingList;
            if (null == device) return existingList;
            if (null == tracks) return existingList;

            var found = findTrack(tracks, MediaStreamTrackKind.Video);
            if (null == found) return existingList;
            if (null == existingList) existingList = new List<MediaVideoTrack>();
            //existingList.Add(new MediaVideoTrack(found.Id,found.Enabled));
            existingList.Add(new MediaVideoTrack(found));
            return existingList;
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
            sb.Append(iceCandidate.Protocol == RTCIceProtocol.Udp ? "udp" : "tcp");
            sb.Append(' ');
            sb.Append(iceCandidate.Priority);
            sb.Append(' ');
            sb.Append(iceCandidate.Ip);
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
                    ice.Protocol = String.Equals(substrings[2],"udp") ? RTCIceProtocol.Udp : RTCIceProtocol.Tcp;
                    ice.Priority = uint.Parse(substrings[3]);
                    ice.Ip = substrings[4];
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

        public static Constraints ToApiConstraints(RTCMediaStreamConstraints mediaStreamConstraints)
        {
            Constraints ret = new Constraints();

            ret.Audio = mediaStreamConstraints.audioEnabled ? new MediaTrackConstraints() : null;
            ret.Video = mediaStreamConstraints.videoEnabled ? new MediaTrackConstraints() : null;

            return ret;
        }
    }
}
