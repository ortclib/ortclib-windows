using System;
using System.Collections.Generic;
using System.Text;
using org.ortc;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            namespace Internal
            {
                public class SDPGenerator
                {
                    internal static string GenerateMediaSDP(string type, RTCRtpCapabilities capabilities,
                        RTCIceGatherer iceGatherer, RTCDtlsTransport dtlsTransport, string ipAddress,
                        IList<UInt32> streamSourceIds, string streamName, string mediaTrackLabel, string streamId)
                    {
                        string ret = null;
                        StringBuilder sb = new StringBuilder();

                        if (capabilities != null)
                        {
                            //------------- Media lines  -------------
                            string mediaLines = SDPGenerator.GenerateMediaLines(type, capabilities.Codecs);
                            if (!string.IsNullOrEmpty(mediaLines))
                                sb.Append(mediaLines);

                            //------------- IP Address lines  -------------
                            string ipAddressLines = SDPGenerator.GenerateIPAddressLines(ipAddress);
                            if (!string.IsNullOrEmpty(ipAddressLines))
                                sb.Append(ipAddressLines);

                            //------------- ICE Parameters lines  -------------
                            RTCIceParameters iceParams = iceGatherer.GetLocalParameters();
                            string iceLines = SDPGenerator.GenerateIceLines(iceParams);
                            if (!string.IsNullOrEmpty(iceLines))
                                sb.Append(iceLines);

                            //------------- DTLS Parameters lines  -------------
                            RTCDtlsParameters dtlsParameters = dtlsTransport.GetLocalParameters();
                            string fingerprintLines = SDPGenerator.GenerateFingerprintLines(dtlsParameters);
                            if (!string.IsNullOrEmpty(fingerprintLines))
                                sb.Append(fingerprintLines);

                            //a=mid:audio
                            sb.Append("a=mid:");
                            sb.Append(type);
                            sb.Append("\r\n");

                            //------------- Extensions lines  -------------
                            string extensionsLines = SDPGenerator.GenerateExtensionsLines(capabilities.HeaderExtensions);
                            if (!string.IsNullOrEmpty(extensionsLines))
                                sb.Append(extensionsLines);

                            //a=sendrecv
                            sb.Append("a=sendrecv"); //WARNING - THIS IS HARDCODED, IT SHOULD BE HANDLED PROPERLY
                            sb.Append("\r\n");

                            //a=rtcp-mux
                            sb.Append("a=rtcp-mux"); //WARNING - THIS IS HARDCODED, IT SHOULD BE HANDLED PROPERLY
                            sb.Append("\r\n");

                            //------------- Codec Parameters lines  -------------
                            string codecLine = SDPGenerator.GenerateCodecLines(capabilities.Codecs);
                            if (!string.IsNullOrEmpty(codecLine))
                                sb.Append(codecLine);

                            //------------- SSRC Parameters lines  -------------
                            string ssrc = SDPGenerator.GenerateSSRC(streamSourceIds, streamName, mediaTrackLabel,
                                streamId);
                            if (!string.IsNullOrEmpty(ssrc))
                                sb.Append(ssrc);
                        }
                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GenerateMediaLines(string mediaType, IList<RTCRtpCodecCapability> codecs)
                    {
                        string ret = null;
                        StringBuilder sb = new StringBuilder();

                        sb.Append("m=");
                        sb.Append(mediaType);
                        sb.Append(' ');
                        sb.Append('9');
                        sb.Append(' ');
                        sb.Append("UDP/TLS/RTP/SAVPF");

                        foreach (RTCRtpCodecCapability cap in codecs)
                        {
                            sb.Append(' ');
                            sb.Append(cap.PreferredPayloadType);
                        }
                        sb.Append("\r\n");

                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GetTypeForIpAddress(string address)
                    {
                        string ret = "IP4";

                        if (address.Contains(":"))
                            ret = "IP6";
                        return ret;
                    }

                    internal static string GenerateIPAddressLines(string ipAddress)
                    {
                        string ret = null;
                        StringBuilder sb = new StringBuilder();

                        //c=IN IP4 0.0.0.0
                        sb.Append("c=IN");
                        sb.Append(' ');
                        sb.Append(GetTypeForIpAddress(ipAddress));
                        sb.Append(' ');
                        sb.Append(ipAddress);
                        sb.Append("\r\n");

                        //a=rtcp:9 IN IP4 0.0.0.0
                        sb.Append("a=");
                        sb.Append("rtcp:");
                        sb.Append('9');
                        sb.Append(' ');
                        sb.Append("IN");
                        sb.Append(' ');
                        sb.Append(GetTypeForIpAddress(ipAddress));
                        sb.Append(' ');
                        sb.Append(ipAddress);
                        sb.Append("\r\n");

                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GenerateIceLines(RTCIceParameters iceParams)
                    {
                        string ret = null;
                        StringBuilder sb = new StringBuilder();

                        // a = ice - ufrag:C / tJt6mZEG7BQVUa
                        sb.Append("a=ice-ufrag:");
                        sb.Append(iceParams.UsernameFragment);
                        sb.Append("\r\n");
                        //a = ice - pwd:coliv0yBjagPtrltwt8u025S
                        sb.Append("a=ice-pwd:");
                        sb.Append(iceParams.Password);
                        sb.Append("\r\n");

                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GenerateFingerprintLines(RTCDtlsParameters dtlsParameters)
                    {
                        string ret = null;
                        StringBuilder sb = new StringBuilder();

                        //a=fingerprint:sha-256 40:0C:E6:90:75:80:87:35:C7:F4:66:CB:C3:5E:F1:E8:63:55:12:09:0E:61:35:32:B8:79:B3:68:73:DD:EB:17
                        foreach (RTCDtlsFingerprint finger in dtlsParameters.Fingerprints)
                        {
                            sb.Append("a=fingerprint:");
                            sb.Append(finger.Algorithm);
                            sb.Append(' ');
                            sb.Append(finger.Value);
                            sb.Append("\r\n");
                        }

                        //a = setup:actpass
                        sb.Append("a=setup:");
                        switch (dtlsParameters.Role)
                        {
                            case RTCDtlsRole.Server:
                            case RTCDtlsRole.Client:
                            case RTCDtlsRole.Auto:
                                sb.Append("actpass");
                                break;
                            default:
                                sb.Append("actpass");
                                break;
                        }
                        sb.Append("\r\n");

                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GenerateExtensionsLines(IList<RTCRtpHeaderExtension> headerExtensions)
                    {
                        string ret = null;
                        StringBuilder sb = new StringBuilder();

                        //a = extmap:1 urn:ietf:params:rtp-hdrext:ssrc-audio-level
                        //a = extmap:3 http://www.webrtc.org/experiments/rtp-hdrext/abs-send-time

                        foreach (RTCRtpHeaderExtension extension in headerExtensions)
                        {
                            sb.Append("a=extmap:");
                            sb.Append(extension.PreferredId);
                            sb.Append(' ');
                            sb.Append(extension.Uri);
                            sb.Append("\r\n");
                        }
                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GenerateCodecLines(IList<RTCRtpCodecCapability> codecs)
                    {
                        string ret = null;
                        uint maxPTime = 0;

                        StringBuilder sb = new StringBuilder();

                        foreach (RTCRtpCodecCapability cap in codecs)
                        {
                            if (maxPTime == 0)
                                maxPTime = cap.Maxptime;
                            else
                                maxPTime = maxPTime > cap.Maxptime ? cap.Maxptime : maxPTime;

                            sb.Append("a=rtpmap:");
                            sb.Append(cap.PreferredPayloadType);
                            sb.Append(' ');
                            sb.Append(cap.Name);
                            sb.Append('/');
                            sb.Append(cap.ClockRate);
                            if (cap.NumChannels > 0)
                            {
                                sb.Append('/');
                                sb.Append(cap.NumChannels);
                            }
                            sb.Append("\r\n");

                            if (cap.RtcpFeedback != null)
                            {
                                foreach (RTCRtcpFeedback fb in cap.RtcpFeedback)
                                {
                                    sb.Append("a=rtcp-fb:");
                                    sb.Append(cap.PreferredPayloadType);
                                    sb.Append(' ');
                                    sb.Append(fb.Type);
                                    sb.Append(' ');
                                    sb.Append(fb.Parameter);
                                    sb.Append("\r\n");
                                }
                            }

                            //a=fmtp:111 minptime=10; useinbandfec=1
#warning TODO - a=fmtp:111 minptime=10; useinbandfec=1 SHOULD BE HANDLED BUT MINPTIME AND USEINBANDFEC ARE MISSING 
                        }
                        maxPTime = maxPTime == 0 ? 120 : maxPTime;
                        //a=maxptime:60
                        sb.Append("a=maxptime:");
                        sb.Append(maxPTime);
                        sb.Append("\r\n");

                        ret = sb.ToString();
                        return ret;
                    }

                    internal static string GenerateSSRC(IList<UInt32> ssrcIds, string cnameSSRC, string audioSSRCLabel,
                        string streamId)
                    {
                        string ret = null;
                        if (ssrcIds.Count == 0 || string.IsNullOrEmpty(cnameSSRC) ||
                            string.IsNullOrEmpty(audioSSRCLabel) || string.IsNullOrEmpty(streamId))
                            return ret;

                        StringBuilder sb = new StringBuilder();

                        foreach (UInt32 ssrcId in ssrcIds)
                        {
                            //a = ssrc:3063731557 cname: 6dj1AlWMKSYOn3hv
                            sb.Append("a=ssrc:");
                            sb.Append(ssrcId);
                            sb.Append(' ');
                            sb.Append("cname:");
                            sb.Append(cnameSSRC);
                            sb.Append("\r\n");

                            //a = ssrc:3063731557 msid: stream_label_1c26693 audio_label_3ea802dc
                            sb.Append("a=ssrc:");
                            sb.Append(ssrcId);
                            sb.Append(' ');
                            sb.Append("msid:");
                            sb.Append(streamId);
                            sb.Append(' ');
                            sb.Append(audioSSRCLabel);
                            sb.Append("\r\n");
                            //a = ssrc:3063731557 mslabel: stream_label_1c26693
                            sb.Append("a=ssrc:");
                            sb.Append(ssrcId);
                            sb.Append(' ');
                            sb.Append("mslabel:");
                            sb.Append(streamId);
                            sb.Append("\r\n");
                            //a = ssrc:3063731557 label: audio_label_3ea802dc
                            sb.Append("a=ssrc:");
                            sb.Append(ssrcId);
                            sb.Append(' ');
                            sb.Append("label:");
                            sb.Append(audioSSRCLabel);
                            sb.Append("\r\n");
                        }
                        ret = sb.ToString();
                        return ret;
                    }
                }
            }
        }
    }
}