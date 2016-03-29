using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using org.ortc;

namespace org
{
    namespace ortc
    {
        namespace adapter
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
                    return infos.Where(info => kind == info.Kind).ToList();
                }

                public static MediaDevice ToMediaDevice(MediaDeviceInfo device)
                {
                    return new MediaDevice(device.DeviceId, device.Label);
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
                    return new CodecInfo(codec.PreferredPayloadType, (int) codec.ClockRate, codec.Name);
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

                public static RTCRtpParameters CapabilitiesToParameters(
                    uint ssrcId, 
                    string cnameSsrc,
                    string muxId,
                    RTCRtpCapabilities caps
                    )
                {
                    var result = new RTCRtpParameters();

                    result.Codecs = new List<RTCRtpCodecParameters>();
                    foreach (var codec in caps.Codecs)
                    {
                        result.Codecs.Add(CapabilitiesToParameters(codec));
                    }
                    result.DegradationPreference = RTCDegradationPreference.Balanced;
                    result.HeaderExtensions = new List<RTCRtpHeaderExtensionParameters>();
                    foreach (var extension in caps.HeaderExtensions)
                    {
                        result.HeaderExtensions.Add(ExtensionToParameters(extension));
                    }
                    result.Encodings = new List<RTCRtpEncodingParameters>();

                    result.Rtcp = new RTCRtcpParameters
                    {
                        Cname = cnameSsrc,
                        Ssrc = ssrcId,
                        Mux = true,
                        ReducedSize = true
                    };

                    return result;
                }

                public static RTCRtpCodecParameters CapabilitiesToParameters(
                    RTCRtpCodecCapability caps
                    )
                {
                    var result = new RTCRtpCodecParameters
                    {
                        ClockRate = caps.ClockRate,
                        Maxptime = caps.Maxptime,
                        Name = caps.Name,
                        NumChannels = caps.NumChannels,
                        PayloadType = caps.PreferredPayloadType,
                        RtcpFeedback = caps.RtcpFeedback,
                        Parameters = caps.Parameters
                    };


                    return result;
                }

                public static RTCRtpHeaderExtensionParameters ExtensionToParameters(
                    RTCRtpHeaderExtension extension
                    )
                {
                    var result = new RTCRtpHeaderExtensionParameters
                    {
                        Id = extension.PreferredId,
                        Encrypt = extension.PreferredEncrypt,
                        Uri = extension.Uri
                    };


                    return result;
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
                        case MediaDeviceKind.AudioInput:
                            trackConstraints = existingConstraints.Audio;
                            break;
                        case MediaDeviceKind.AudioOutput:
                            trackConstraints = existingConstraints.Audio;
                            break;
                        case MediaDeviceKind.Video:
                            trackConstraints = existingConstraints.Video;
                            break;
                    }
                    if (null == trackConstraints) trackConstraints = new MediaTrackConstraints();

                    if (null == trackConstraints.Advanced)
                        trackConstraints.Advanced = new List<MediaTrackConstraintSet>();

                    var constraintSet = new MediaTrackConstraintSet();
                    constraintSet.DeviceId = new ConstrainString();
                    constraintSet.DeviceId.Parameters = new ConstrainStringParameters();
                    constraintSet.DeviceId.Parameters.Exact = new StringOrStringList();
                    constraintSet.DeviceId.Parameters.Exact.Value = device.Id;
                    constraintSet.DeviceId.Value = new StringOrStringList();
                    constraintSet.DeviceId.Value.Value = device.Id;
//#warning  TODO Removed hardcoded values
//                    constraintSet.Height = new ConstrainLong();
//                    constraintSet.Height.Value = 600;

//                    constraintSet.Width = new ConstrainLong();
//                    constraintSet.Width.Value = 800;

//                    constraintSet.FrameRate = new ConstrainDouble();
//                    constraintSet.FrameRate.Value = 30;

//                    constraintSet.SampleRate = new ConstrainLong();
//                    constraintSet.SampleRate.Value = 48000;

                    trackConstraints.Advanced.Add(constraintSet);

                    switch (kind)
                    {
                        case MediaDeviceKind.AudioInput:
                            existingConstraints.Audio = trackConstraints;
                            break;
                        case MediaDeviceKind.AudioOutput:
                            existingConstraints.Audio = trackConstraints;
                            break;
                        case MediaDeviceKind.Video:
                            existingConstraints.Video = trackConstraints;
                            break;
                    }
                    return existingConstraints;
                }

                public static MediaStreamTrack findTrack(
                    IList<MediaStreamTrack> tracks,
                    MediaDevice device
                    )
                {
                    if (null == device) return null;
                    foreach (var track in tracks)
                    {
                        if (track.DeviceId != device.Id) return track;
                    }
                    return null;
                }

                public static MediaStreamTrack findTrack(
                    IList<MediaStreamTrack> tracks,
                    MediaStreamTrackKind kind
                    )
                {
                    //if (null == device) return null;
                    return tracks.FirstOrDefault(track => track.Kind == kind);
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


                public static RTCIceCandidate ToWrapperIceCandidate(org.ortc.RTCIceCandidate iceCandidate,
                    int sdpComponentId)
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
                    var ret = new RTCIceCandidate(sb.ToString(), sdpMid, sdpMLineIndex);

                    //org.ortc.RTCIceCandidate iceCandidate2 = IceCandidateFromSdp(sb.ToString());

                    return ret;
                }

                public static org.ortc.RTCIceCandidate IceCandidateFromSdp(string sdp)
                {
                    ortc.RTCIceCandidate ice = null;//new org.ortc.RTCIceCandidate();
                    try
                    {
                        //candidate:704553097 1 udp 2122260223 192.168.1.3 62723 typ host generation 0
                        TextReader reader = new StringReader(sdp);
                        string line = reader.ReadLine() ?? sdp;

                        if (!String.IsNullOrEmpty(line))
                        {
                            ice = new org.ortc.RTCIceCandidate();
                            string[] substrings = line.Split(' ');

                            if (substrings.Length >= 10)
                            {
                                ice.Foundation = substrings[0];
                                ice.Protocol = String.Equals(substrings[2], "udp") ? RTCIceProtocol.Udp : RTCIceProtocol.Tcp;
                                ice.Priority = uint.Parse(substrings[3]);
                                ice.Ip = substrings[4];
                                ice.Port = ushort.Parse(substrings[5]);
                                ice.CandidateType = ToIceCandidateType(substrings[7]);

                                if (substrings.Length > 10)
                                {
                                    ice.RelatedAddress = substrings[9];
                                    ice.RelatedPort = ushort.Parse(substrings[11]);
                                }
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        Debug.WriteLine($"Exception ice parsing: {e.Message}");
                        //Common.Logger.Warn("Disconnected during socket read operation due to exception", e);
                    }
                    return ice;
                }

                public static Constraints ToApiConstraints(RTCMediaStreamConstraints mediaStreamConstraints)
                {
                    Constraints ret = new Constraints();

                    ret.Audio = mediaStreamConstraints.audioEnabled ? new MediaTrackConstraints() : null;
                    ret.Video = mediaStreamConstraints.videoEnabled ? new MediaTrackConstraints() : null;

                    return ret;
                }

                public static void PickLocalCodecBasedOnRemote(
                    RTCRtpCapabilities localCaps,
                    RTCRtpCapabilities remoteCaps
                    )
                {
                    if (null == localCaps) return;
                    if (null == remoteCaps) return;
                    if (null == localCaps.Codecs) return;
                    if (null == remoteCaps.Codecs) return;

                    var newList = new List<RTCRtpCodecCapability>();

                    // insert sort local codecs based on remote codec list
                    foreach (var remoteCodec in remoteCaps.Codecs)
                    {
                        foreach (var localCodec in localCaps.Codecs)
                        {
                            if (remoteCodec.Name != localCodec.Name) continue;
                            newList.Add(localCodec);
                        }
                    }

                    // insert sort local codecs based on codec not being found in remote codec list
                    foreach (var localCodec in localCaps.Codecs)
                    {
                        bool found = false;
                        foreach (var remoteCodec in remoteCaps.Codecs)
                        {
                            if (remoteCodec.Name != localCodec.Name) continue;
                            found = true;
                            break;
                        }
                        if (found) continue;
                        newList.Add(localCodec);
                    }

                    localCaps.Codecs = newList;
                }

                public static RTCIceGatherOptions ToGatherOptions(RTCConfiguration configuration)
                {
                    RTCIceGatherOptions options = new RTCIceGatherOptions();
                    options.IceServers = new List<org.ortc.RTCIceServer>();

                    foreach (RTCIceServer server in configuration.IceServers)
                    {
                        org.ortc.RTCIceServer ortcServer = new org.ortc.RTCIceServer();
                        ortcServer.Urls = new List<string>();

                        if (!string.IsNullOrEmpty(server.Credential))
                        {
                            ortcServer.Credential = server.Credential;
                        }

                        if (!string.IsNullOrEmpty(server.Username))
                        {
                            ortcServer.UserName = server.Username;
                        }

                        ortcServer.Urls.Add(server.Url);
                        options.IceServers.Add(ortcServer);
                    }
                    return options;
                }

            }
        }
    }
}