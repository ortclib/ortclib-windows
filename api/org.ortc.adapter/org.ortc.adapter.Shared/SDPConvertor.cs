using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Linq;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {

            class SdpConvertor
            {
                internal static void ParseSdp(string sdp, RTCSdpType? type, RTCPeerConnection peerConnection)
                {
                    string sessionId;
                    string sessionVersion;
                    string streamId;
                    bool audioSupported;
                    bool videoSupported;
                 

                    //string[] parts = sdp.Split("m=".ToCharArray());
                    string[] parts = sdp.Split(new[] {"m="}, StringSplitOptions.None);
                    if (parts.Length < 2)
                        throw new Exception("invalid SDP");

                    bool parsedSuccessfully = ParseCommonSdp(parts[0], out sessionId, out sessionVersion, out streamId, out audioSupported,
                        out videoSupported);

                    if (!parsedSuccessfully)
                        throw new Exception("invalid SDP");

                    for (int i = 1; i < parts.Length; i++)
                    {
                        ParseMediaSdp(parts[i], peerConnection);
                    }

                    if (type == RTCSdpType.Offer)
                    {
                        peerConnection.IceRole = RTCIceRole.Controlled;
                        peerConnection.AudioReceiverRtpParameters = peerConnection.AudioSenderRtpParameters;
                        peerConnection.VideoReceiverRtpParameters = peerConnection.VideoSenderRtpParameters;
                    }

                }

                internal static bool ParseCommonSdp(string sdp, out string sessionId, out string sessionVersion,
                    out string streamId, out bool audioSupported, out bool videoSupported)
                {
                    bool valid = true;
                    sessionId = null;
                    sessionVersion = null;
                    streamId = null;
                    audioSupported = false;
                    videoSupported = false;

                    try
                    {
                        TextReader reader = new StringReader(sdp);

                        string line = reader.ReadLine();
                        while (line != null)
                        {
                            if (line.Length == 0)
                            {
                                line = reader.ReadLine();
                                continue;
                            }
                            string value = line.Substring(2);
                            try
                            {
                                switch (line[0])
                                {
                                    case 'v':
                                        int version;
                                        if (!int.TryParse(value, out version))
                                        {
                                            throw new Exception(string.Format("Invalid Line {0}", line));
                                        }
                                        if (version != 0)
                                        {
                                            throw new Exception("Not supported version");
                                        }
                                        break;
                                    case 'o':
                                        var parts = value.Split(' ');
                                        if (parts.Length != 6)
                                        {
                                            throw new Exception(string.Format("Invalid Line {0}", line));
                                        }
                                        sessionId = parts[1];
                                        sessionVersion = parts[2];
                                        break;
                                    case 's':
                                        break;
                                    case 't':
                                        break;
                                    case 'a':
                                        var sep = value.IndexOf(':');

                                        if (sep == -1)
                                            break;

                                        var attrName = value.Substring(0, sep);
                                        var attrValue = value.Substring(sep + 1);

                                        if (attrName.Equals("group"))
                                        {
                                            audioSupported = attrValue.Contains("audio");
                                            videoSupported = attrValue.Contains("video");
                                        }
                                        else if (attrName.Equals("msid-semantic"))
                                        {
                                            streamId = attrValue;
                                        }
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
                    catch
                    {
                        valid = false;
                    }
                    return valid;
                }

                internal static bool ParseMediaSdp(string sdp, RTCPeerConnection peerConnection)
                {
                    bool valid = true;
                    RTCRtpParameters parameters = null;

                    if (null == peerConnection.RemoteRtcIceParameters) peerConnection.RemoteRtcIceParameters  = new RTCIceParameters();
                    if (null == peerConnection.RemoteRtcDtlsParameters)
                    {
                        peerConnection.RemoteRtcDtlsParameters = new RTCDtlsParameters
                        {
                            Fingerprints = new List<RTCDtlsFingerprint>()
                        };
                    }

                    //Dictionary<string,RTCRtpCodecCapability> codecsDictionary = new Dictionary<string, RTCRtpCodecCapability>();
                    //List<RTCRtpHeaderExtension> headerExtensions = new List<RTCRtpHeaderExtension>();
      
                    TextReader reader = new StringReader(sdp);

                    IList<UInt32> streamSourceIds = new List<uint>();

                    string line = reader.ReadLine();
                    while (line != null)
                    {
                        if (line.Length == 0)
                        {
                            line = reader.ReadLine();
                            continue;
                        }
                        
                        try
                        {
                            if (line.StartsWith("audio") || line.StartsWith("video"))
                            {
                                string[] list = line.Split(' ');
                                var codecsList = new List<string>(list);
                                codecsList.RemoveRange(0,3);
                                if (codecsList.Count > 0)
                                {
                                    parameters = new RTCRtpParameters
                                    {
                                        Codecs = new List<RTCRtpCodecParameters>(),
                                        HeaderExtensions = new List<RTCRtpHeaderExtensionParameters>(),
                                        Rtcp = new RTCRtcpParameters(),
                                        Encodings = new List<RTCRtpEncodingParameters>()
                                    };
                                    
                                    foreach (var codecId in codecsList)
                                    {
                                        RTCRtpCodecParameters codecParameters = new RTCRtpCodecParameters
                                        {
                                            PayloadType = byte.Parse(codecId)
                                        };
                                        parameters.Codecs.Add(codecParameters);
                                        //codecsDictionary.Add(codecId,codecCapability);
                                    }

                                    if (line.StartsWith("audio"))
                                        peerConnection.AudioSenderRtpParameters = parameters;
                                    else
                                        peerConnection.VideoSenderRtpParameters = parameters;
                                }
                            }
                            else
                            {
                                string value = line.Substring(2);

                                switch (line[0])
                                {
                                    case 'c':
                                        var parts = value.Split(' ');
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
                                        var sep = value.IndexOf(':');

                                        if (sep == -1)
                                            break;

                                        var attrName = value.Substring(0, sep);
                                        var attrValue = value.Substring(sep + 1);

                                        if (attrName.Equals("candidate"))
                                        {
                                            //org.ortc.RTCIceCandidate iceCandidate;
                                            //iceCandidate = iceCandidateFromSdp(attrValue);
                                        }
                                        else if (attrName.Equals("rtcp"))
                                        {

                                        }
                                        else if (attrName.Equals("ice-ufrag"))
                                        {
                                            peerConnection.RemoteRtcIceParameters.UsernameFragment = attrValue;
                                        }
                                        else if (attrName.Equals("ice-pwd"))
                                        {
                                            peerConnection.RemoteRtcIceParameters.Password = attrValue;
                                        }
                                        else if (attrName.Equals("fingerprint"))
                                        {
                                            sep = attrValue.IndexOf(' ');

                                            if (sep == -1)
                                                break;

                                            RTCDtlsFingerprint dtlsFingerprint = new RTCDtlsFingerprint
                                            {
                                                Algorithm = attrValue.Substring(0, sep),
                                                Value = attrValue.Substring(sep + 1)
                                            };
                                            peerConnection.RemoteRtcDtlsParameters.Fingerprints.Add(dtlsFingerprint);
                                        }
                                        else if (attrName.Equals("setup"))
                                        {
                                            //TODO Check when should be set client or server
                                            //dtlsParameters.Role = attrValue.Equals("actpass") ? RTCDtlsRole.Auto : RTCDtlsRole.Auto;
                                            peerConnection.RemoteRtcDtlsParameters.Role = RTCDtlsRole.Auto;
                                        }
                                        else if (attrName.Equals("mid"))
                                        {

                                        }
                                        else if (attrName.Equals("extmap"))
                                        {
                                            sep = attrValue.IndexOf(' ');

                                            if (sep == -1)
                                                break;
                                            RTCRtpHeaderExtensionParameters headerExtensionParameters =
                                                new RTCRtpHeaderExtensionParameters
                                                {
                                                    Id = ushort.Parse(attrValue.Substring(0, sep)),
                                                    Uri = attrValue.Substring(sep + 1)
                                                };
                                            parameters.HeaderExtensions.Add(headerExtensionParameters);
                                        }
                                        else if (attrName.Equals("sendrecv"))
                                        {

                                        }
                                        else if (attrName.Equals("rtcp-mux"))
                                        {
                                            parameters.Rtcp.Mux = true;
                                        }
                                        else if (attrName.Equals("rtcp-rsize"))
                                        {
                                            parameters.Rtcp.ReducedSize = true;
                                        }
                                        else if (attrName.Equals("rtpmap"))
                                        {
                                            //a = rtpmap:100 VP8 / 90000
                                            sep = attrValue.IndexOf(' ');

                                            if (sep == -1)
                                                break;

                                            string codecId = attrValue.Substring(0, sep);
                                            if (null == codecId) break;

                                            RTCRtpCodecParameters codecParameters = parameters.Codecs.FirstOrDefault(i => i.PayloadType == byte.Parse(codecId));
                                            if (null != codecParameters)
                                            {
                                                string temp = attrValue.Substring(sep + 1);
                                                if (null == temp)
                                                    break;

                                                string[] codecInfoStrings = temp.Split('/');

                                                if (codecInfoStrings.Length > 1)
                                                {
                                                    codecParameters.Name = codecInfoStrings[0];
                                                    codecParameters.ClockRate = uint.Parse(codecInfoStrings[1]);
                                                    if (codecInfoStrings.Length == 3)
                                                        codecParameters.NumChannels = uint.Parse(codecInfoStrings[2]);
                                                }
                                            }

                                        }
                                        else if (attrName.Equals("rtcp-fb"))
                                        {
                                            //rtcp-fb:100 ccm goog-remb
                                            string[] caodecInfoStrings = attrValue.Split(' ');
                                            string codecId = caodecInfoStrings[0];

                                            RTCRtpCodecParameters codecParameters = parameters.Codecs.FirstOrDefault(i => i.PayloadType == byte.Parse(codecId));
                                            if (null != codecParameters)
                                            {
                                                if (null == codecParameters.RtcpFeedback)
                                                    codecParameters.RtcpFeedback = new List<RTCRtcpFeedback>();

                                                RTCRtcpFeedback fb = new RTCRtcpFeedback
                                                {
                                                    Type = caodecInfoStrings[1],
                                                    Parameter =
                                                        caodecInfoStrings.Length == 3 ? caodecInfoStrings[2] : ""
                                                };
                                                codecParameters.RtcpFeedback.Add(fb);
                                            }
                                            
                                        }
                                        else if (attrName.Equals("fmtp"))
                                        {

                                        }
                                        else if (attrName.Equals("maxptime"))
                                        {
                                            if (null != peerConnection.AudioSenderCaps)
                                            {
                                                uint maxptime = uint.Parse(attrValue);
                                                foreach (var codecCapability in peerConnection.AudioSenderCaps.Codecs)
                                                {
                                                    codecCapability.Maxptime = maxptime;
                                                }
                                            }
                                        }
                                        else if (attrName.Equals("ssrc-group"))
                                        {
                                            string[] ssrcStrings = attrValue.Split(' ');
                                            foreach (var str in ssrcStrings)
                                            {
                                                if (!str.Equals("FID"))
                                                {
                                                    streamSourceIds.Add(uint.Parse(str));
                                                }
                                            }
                                        }
                                        else if (attrName.Equals("ssrc"))
                                        {
                                            //a=ssrc:1928739697 cname:0SagpZSf2NzVtTYz
                                            sep = attrValue.IndexOf(' ');

                                            if (sep == -1)
                                                break;
                                            string ssrcId = attrValue.Substring(0, sep);

                                            RTCRtpEncodingParameters encodingParameters = null;
                                            foreach (var i in parameters.Encodings)
                                            {
                                                if (i.Ssrc == parameters.Rtcp.Ssrc)
                                                {
                                                    encodingParameters = i;
                                                    break;
                                                }
                                            }
                                            parameters.Rtcp.Ssrc = uint.Parse(ssrcId);

                                            if (null == encodingParameters)
                                            {
                                                encodingParameters = new RTCRtpEncodingParameters
                                                {
                                                    Ssrc = parameters.Rtcp.Ssrc
                                                };
                                                parameters.Encodings.Add(encodingParameters);
                                            }

                                            

                                            string ssrcString = attrValue.Substring(sep+1);
                                            string[] ssrcStrings = ssrcString.Split(':');

                                            string ssrcType = ssrcStrings[0];
                                            if (ssrcType.Equals("cname"))
                                            {
                                                parameters.Rtcp.Cname = ssrcStrings[1];
                                            }
                                            else if (ssrcType.Equals("msid"))
                                            {

                                            }
                                            else if (ssrcType.Equals("mslabel"))
                                            {
                                                //streamId = ssrcStrings[1];
                                            }
                                            else if (ssrcType.Equals("label"))
                                            {
                                                //mediaTrackLabel = ssrcStrings[1];
                                            }
                                        }
                                        break;
                                }
                            }
                        }
                        catch (Exception e)
                        {
                            valid = false;
                            //throw new Exception(string.Format("Invalid Line {0}", line));
                        }
                        line = reader.ReadLine();
                    }
                    return valid;
                }
            }
        }
    }
}