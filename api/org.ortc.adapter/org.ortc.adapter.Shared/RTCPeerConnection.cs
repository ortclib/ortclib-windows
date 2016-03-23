using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using org.ortc;
using Log = org.ortc.Log;
using Windows.Foundation;
using org.ortc.adapter.Internal;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            public class RTCPeerConnection
            {
                private bool _closed = false;
                private string audioSSRCLabel = null;
                private string videoSSRCLabel = null;
                private string cnameSSRC = null;
                private UInt32 ssrcId;

                //private RtcIceRole _iceRole = RtcIceRole.Controlling;
                //private RTCSessionDescription _localCapabilities;
                // private RTCSessionDescription _localCapabilitiesFinal;
                //private RTCSessionDescription _remoteCapabilities;

                private bool _installedIceEvents;

                private RTCRtpSender AudioSender { get; set; }
                private RTCRtpSender VideoSender { get; set; }
                private RTCRtpReceiver AudioReceiver { get; set; }
                private RTCRtpReceiver VideoReceiver { get; set; }
                private MediaDevice AudioPlaybackDevice { get; set; }

                private MediaStream LocalStream { get; set; }
                private MediaStream remoteStream { get; set; }

                private RTCIceGatherOptions options { get; set; }
                private RTCIceGatherer iceGatherer { get; set; }
                private RTCIceGatherer iceGathererRTCP { get; set; }
                private RTCDtlsTransport dtlsTransport { get; set; }
                private RTCIceTransport iceTransport { get; set; }

                private RTCRtpCapabilities audioSenderCaps { get; set; }
                private RTCRtpCapabilities videoSenderCaps { get; set; }
                private RTCRtpCapabilities audioReceiverCaps { get; set; }
                private RTCRtpCapabilities videoReceiverCaps { get; set; }

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
                    add { _OnIceCandidate += value; }
                    remove { _OnIceCandidate -= value; }
                }

                public event MediaStreamEventEventDelegate OnAddStream;
                public event MediaStreamEventEventDelegate OnRemoveStream;
                public event RTCPeerConnectionHealthStatsDelegate OnConnectionHealthStats;

                public RTCPeerConnection(RTCConfiguration configuration)
                {
                    Logger.SetLogLevel(Log.Level.Trace);
                    Logger.SetLogLevel(Log.Component.ZsLib, Log.Level.Trace);
                    Logger.SetLogLevel(Log.Component.Services, Log.Level.Trace);
                    Logger.SetLogLevel(Log.Component.ServicesHttp, Log.Level.Trace);
                    Logger.SetLogLevel(Log.Component.OrtcLib, Log.Level.Insane);
                    Logger.SetLogLevel("ortc_standup", Log.Level.Insane);


                    //openpeer::services::ILogger::installDebuggerLogger();
                    Logger.InstallTelnetLogger(59999, 60, true);

                    Settings.ApplyDefaults();
                    Ortc.Setup();

                    //_installedIceEvents = false;

                    PrepareGatherer(configuration);
                    PrepareIceTransport();
                    PrepareDtlsTransport();
                    SetCapabilities();

                    sessionID =
                        (ulong)
                            (DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0).ToUniversalTime()).TotalMilliseconds;
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
                    LocalStream = stream;
                }

                public void Close()
                {

                }

                public IAsyncAction SetRemoteDescription(RTCSessionDescription description)
                {
                    Task ret = Task.Run(() =>
                    {

                    });
                    return ret.AsAsyncAction();
                }

                public Task<RTCSessionDescription> CreateAnswer() //async
                {
                    return null;
                }

                private UInt64 sessionID { get; set; }
                private UInt16 sessionVersion { get; set; }

                void parseSDP(string sdp)
                {

                }

                string createSDP()
                {
                    StringBuilder sb = new StringBuilder();

                    Boolean containsAudio = (LocalStream.GetAudioTracks() != null) &&
                                            LocalStream.GetAudioTracks().Count > 0;
                    Boolean containsVideo = (LocalStream.GetVideoTracks() != null) &&
                                            LocalStream.GetVideoTracks().Count > 0;

                    //------------- Global lines START -------------
                    //v=0
                    sb.Append("v=");
                    sb.Append(0);
                    sb.Append("\r\n");


                    //o=- 1045717134763489491 2 IN IP4 127.0.0.1
                    sb.Append("o=- ");
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
                    if (containsAudio)
                    {
                        sb.Append("audio");
                        sb.Append(' ');
                    }
                    if (containsVideo)
                        sb.Append("video");

                    sb.Append("\r\n");

                    //a=msid-semantic: WMS stream_label_ce8753d3
                    sb.Append("a=");
                    sb.Append("msid-semantic:");
                    sb.Append(' ');
                    sb.Append("WMS");
                    sb.Append(' ');
                    sb.Append(LocalStream.Id);
                    sb.Append("\r\n");
                    //------------- Global lines END -------------

                    //IList<MediaAudioTrack>  audioTracks = localStream.GetAudioTracks();

                    List<UInt32> listOfSsrcIds = new List<UInt32>();
                    if (ssrcId == 0)
                    {
                        ssrcId = (UInt32) Guid.NewGuid().GetHashCode();
                        listOfSsrcIds.Add(ssrcId);
                    }
                    if (cnameSSRC == null)
                        cnameSSRC = Guid.NewGuid().ToString();

                    //------------- Media lines START -------------
                    //m = audio 9 UDP / TLS / RTP / SAVPF 111 103 104 9 102 0 8 106 105 13 127 126
                    if (containsAudio)
                    {
                        if (audioSSRCLabel == null)
                            audioSSRCLabel = Guid.NewGuid().ToString();
                        var audioCapabilities = RTCRtpReceiver.GetCapabilities("audio");

                        if (audioCapabilities != null)
                        {
                            string mediaLine = SDPGenerator.GenerateMediaSDP("audio", audioCapabilities, iceGatherer,
                                dtlsTransport, "0.0.0.0", listOfSsrcIds, cnameSSRC, audioSSRCLabel, LocalStream.Id);

                            if (!string.IsNullOrEmpty(mediaLine))
                                sb.Append(mediaLine);
                        }
                    }

                    if (containsVideo)
                    {
                        if (videoSSRCLabel == null)
                            videoSSRCLabel = Guid.NewGuid().ToString();
                        var videoCapabilities = RTCRtpReceiver.GetCapabilities("video");

                        if (videoCapabilities != null)
                        {
                            string mediaLine = SDPGenerator.GenerateMediaSDP("video", videoCapabilities, iceGatherer,
                                dtlsTransport, "0.0.0.0", listOfSsrcIds, cnameSSRC, videoSSRCLabel, LocalStream.Id);

                            if (!string.IsNullOrEmpty(mediaLine))
                                sb.Append(mediaLine);
                        }
                    }
                    string ret = sb.ToString();
                    return ret;

                }

                public IAsyncOperation<RTCSessionDescription> CreateOffer()
                {
                    Task<RTCSessionDescription> ret = Task.Run<RTCSessionDescription>(() =>
                    {
                        RTCSessionDescription sd = new RTCSessionDescription(RTCSdpType.Offer, this.createSDP());
                        return sd;
                    });

                    return ret.AsAsyncOperation<RTCSessionDescription>();
                }

                public IAsyncAction SetLocalDescription(RTCSessionDescription description)
                {
                    Task ret = Task.Run(() =>
                    {
                        PrepareReceiver();
                    });
                    return ret.AsAsyncAction();
                }

                public Task AddIceCandidate(RTCIceCandidate candidate) //async
                {
                    return null;
                }


                private void PrepareGatherer(RTCConfiguration configuration)
                {
                    options = new RTCIceGatherOptions();
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

                    try
                    {
                        iceGatherer = new org.ortc.RTCIceGatherer(options);
                        iceGatherer.OnICEGathererStateChanged += OnICEGathererStateChanged;
                        iceGatherer.OnICEGathererLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate;
                        iceGatherer.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
                        iceGatherer.OnICEGathererLocalCandidateGone +=
                            this.RTCIceGatherer_onICEGathererLocalCandidateGone;
                        iceGatherer.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;

                        iceGathererRTCP = iceGatherer.CreateAssociatedGatherer();
                        //iceGathererRTCP.OnICEGathererStateChanged += OnICEGathererStateChanged;
                        iceGathererRTCP.OnICEGathererLocalCandidate +=
                            this.RTCIceGatherer_onRTCPICEGathererLocalCandidate;
                        //iceGathererRTCP.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
                        //iceGathererRTCP.OnICEGathererLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone;
                        //iceGathererRTCP.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;
                    }
                    catch (Exception e)
                    {
                        return;
                    }
                }

                private void PrepareIceTransport()
                {
                    iceTransport = new org.ortc.RTCIceTransport(iceGatherer);
                }

                private void PrepareDtlsTransport()
                {
                    RTCCertificate.GenerateCertificate("").AsTask<RTCCertificate>().ContinueWith((cert) =>
                    {
                        //using (var @lock = new AutoLock(_lock))
                        {
                            // Since the DTLS certificate is ready the RtcDtlsTransport can now be constructed.
                            var certs = new List<RTCCertificate>();
                            certs.Add(cert.Result);
                            dtlsTransport = new RTCDtlsTransport(iceTransport, certs);
                            if (_closed) dtlsTransport.Stop();
                        }
                    });
                }

                private void SetCapabilities()
                {
                    audioSenderCaps = RTCRtpSender.GetCapabilities("audio");
                    videoSenderCaps = RTCRtpSender.GetCapabilities("video");
                    audioReceiverCaps = RTCRtpReceiver.GetCapabilities("audio");
                    videoReceiverCaps = RTCRtpReceiver.GetCapabilities("video");
                }

                private void PrepareReceiver()
                {
                    Boolean containsAudio = (LocalStream.GetAudioTracks() != null) &&
                                            LocalStream.GetAudioTracks().Count > 0;
                    Boolean containsVideo = (LocalStream.GetVideoTracks() != null) &&
                                            LocalStream.GetVideoTracks().Count > 0;

                    if (containsAudio)
                    {
                        AudioReceiver = new RTCRtpReceiver(dtlsTransport);
                        var audioParams = Helper.CapabilitiesToParameters("a", audioSenderCaps);
                        AudioReceiver.Receive(audioParams);
                    }

                    if (containsVideo)
                    {
                        VideoReceiver = new RTCRtpReceiver(dtlsTransport);
                        var videoParams = Helper.CapabilitiesToParameters("v", audioSenderCaps);
                        VideoReceiver.Receive(videoParams);
                    }
                }

/*
        private void PrepareSender()
        {
            Boolean containsAudio = (localStream.GetAudioTracks() != null) && localStream.GetAudioTracks().Count > 0;
            Boolean containsVideo = (localStream.GetVideoTracks() != null) && localStream.GetVideoTracks().Count > 0;

            //From SDP obtain remote IceParameters and IceRole
            iceTransport.Start(iceGatherer, _remoteCapabilities.Description.IceParameters, _remoteCapabilities.Description.IceRole);

            //From SDP obtain remote dtls parameters
            dtlsTransport.Start(_remoteCapabilities.Description.DtlsParameters);

            if (containsAudio)
            {
                if (null == audioSender)
                {
                    // Figure out if the application has audio media streams to send to the remote party.
                    var tracks = localStream.GetAudioTracks();
                    MediaAudioTrack mediaTrack = null;
                    MediaStreamTrack track = null;
                    if (null != tracks) { if (tracks.Count > 0) { mediaTrack = tracks.First(); } }
                    if (null != mediaTrack) track = mediaTrack.Track;

                    if (null != track)
                    {
                        // If a track was found then setup the audio RtcRtpSender.
                        audioSender = new RTCRtpSender(track, dtlsTransport);
                        var @params = Helper.CapabilitiesToParameters("a", _remoteCapabilities.Description.AudioReceiverCapabilities);
                        RtcHelper.SetupSenderEncodings(@params);
                        audioSender.Send(@params);
                    }
                }
            }

            // If the local and remote party are sending video then setup the video RtcRtpSender.
            if (containsVideo)
            {
                if (null == videoSender)
                {
                    // Figure out if the application has video media streams to send to the remote party.
                    var tracks = localStream.GetVideoTracks();
                    MediaVideoTrack mediaTrack = null;
                    MediaStreamTrack track = null;
                    if (null != tracks) { if (tracks.Count > 0) { mediaTrack = tracks.First(); } }
                    if (null != mediaTrack) track = mediaTrack.Track;

                    if (null != track)
                    {
                        // If a track was found then setup the video RtcRtpSender.
                        videoSender = new RTCRtpSender(track, dtlsTransport);
                        var @params = Helper.CapabilitiesToParameters("v", _remoteCapabilities.Description.VideoReceiverCapabilities);
                        RtcHelper.SetupSenderEncodings(@params);
                        videoSender.Send(@params);
                    }
                }
            }
        }*/

                private void OnICEGathererStateChanged(RTCIceGathererStateChangeEvent evt)
                {
                    if (evt.State == RTCIceGathererState.Complete)
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
    }
}