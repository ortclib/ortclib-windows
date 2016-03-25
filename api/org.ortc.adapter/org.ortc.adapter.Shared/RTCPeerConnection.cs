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
                internal bool Closed { get; set; } = false;
                internal string AudioSsrcLabel { get; set; } = null;
                internal string VideoSsrcLabel { get; set; } = null;
                internal string CnameSsrc { get; set; } = null;
                internal uint SsrcId { get; set; }

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

                internal  MediaStream LocalStream { get; set; }
                internal MediaStream RemoteStream { get; set; }

                private RTCIceGatherOptions Options { get; set; }
                internal RTCIceGatherer IceGatherer { get; set; }
                private RTCIceGatherer IceGathererRtcp { get; set; }
                internal RTCDtlsTransport DtlsTransport { get; set; }
                private RTCIceTransport IceTransport { get; set; }

                private RTCRtpCapabilities AudioSenderCaps { get; set; }
                private RTCRtpCapabilities VideoSenderCaps { get; set; }
                private RTCRtpCapabilities AudioReceiverCaps { get; set; }
                private RTCRtpCapabilities VideoReceiverCaps { get; set; }

                internal ulong SessionId { get; set; }
                internal ushort SessionVersion { get; set; }

                static public void SetPreferredVideoCaptureFormat(int frame_width,
                    int frame_height, int fps)
                {

                }

                public delegate void RtcPeerConnectionIceEventDelegate(RTCPeerConnectionIceEvent evt);

                public delegate void MediaStreamEventEventDelegate(MediaStreamEvent evt);

                public delegate void RtcPeerConnectionHealthStatsDelegate(RTCPeerConnectionHealthStats stats);

                public event RtcPeerConnectionIceEventDelegate _OnIceCandidate;

                public event RtcPeerConnectionIceEventDelegate OnIceCandidate
                {
                    add { _OnIceCandidate += value; }
                    remove { _OnIceCandidate -= value; }
                }

                public event MediaStreamEventEventDelegate OnAddStream;
                public event MediaStreamEventEventDelegate OnRemoveStream;
                public event RtcPeerConnectionHealthStatsDelegate OnConnectionHealthStats;

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

                    SessionId = (ulong) (DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0).ToUniversalTime()).TotalMilliseconds;
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
                        SDPConvertor.ParseSdp(description.Sdp,description.Type,this);
                    });
                    return ret.AsAsyncAction();
                }

                public Task<RTCSessionDescription> CreateAnswer() //async
                {
                    return null;
                }

                
                
                public IAsyncOperation<RTCSessionDescription> CreateOffer()
                {
                    Task<RTCSessionDescription> ret = Task.Run<RTCSessionDescription>(() =>
                    {
                        //RTCSessionDescription sd = new RTCSessionDescription(RTCSdpType.Offer, this.createSDP());
                        RTCSessionDescription sd = new RTCSessionDescription(RTCSdpType.Offer, SdpGenerator.CreateSdp(this));
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

                public IAsyncAction AddIceCandidate(RTCIceCandidate candidate)
                {
                    return Task.Run(() =>
                    {
                        org.ortc.RTCIceCandidate iceCandidate = Helper.iceCandidateFromSdp(candidate.Candidate);
                        IceTransport.AddRemoteCandidate(iceCandidate);
                    }).AsAsyncAction();
                    
                }


                private void PrepareGatherer(RTCConfiguration configuration)
                {
                    Options = new RTCIceGatherOptions();
                    Options.IceServers = new List<org.ortc.RTCIceServer>();

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
                        Options.IceServers.Add(ortcServer);
                    }

                    try
                    {
                        IceGatherer = new org.ortc.RTCIceGatherer(Options);
                        IceGatherer.OnICEGathererStateChanged += OnICEGathererStateChanged;
                        IceGatherer.OnICEGathererLocalCandidate += RTCIceGatherer_onICEGathererLocalCandidate;
                        IceGatherer.OnICEGathererCandidateComplete += RTCIceGatherer_onICEGathererCandidateComplete;
                        IceGatherer.OnICEGathererLocalCandidateGone +=
                            RTCIceGatherer_onICEGathererLocalCandidateGone;
                        IceGatherer.OnICEGathererError += RTCIceGatherer_onICEGathererError;

                        IceGathererRtcp = IceGatherer.CreateAssociatedGatherer();
                        //iceGathererRTCP.OnICEGathererStateChanged += OnICEGathererStateChanged;
                        IceGathererRtcp.OnICEGathererLocalCandidate +=
                            RTCIceGatherer_onRTCPICEGathererLocalCandidate;
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
                    IceTransport = new org.ortc.RTCIceTransport(IceGatherer);
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
                            DtlsTransport = new RTCDtlsTransport(IceTransport, certs);
                            if (Closed) DtlsTransport.Stop();
                        }
                    });
                }

                private void SetCapabilities()
                {
                    AudioSenderCaps = RTCRtpSender.GetCapabilities("audio");
                    VideoSenderCaps = RTCRtpSender.GetCapabilities("video");
                    AudioReceiverCaps = RTCRtpReceiver.GetCapabilities("audio");
                    VideoReceiverCaps = RTCRtpReceiver.GetCapabilities("video");
                }

                private void PrepareReceiver()
                {
                    Boolean containsAudio = (LocalStream.GetAudioTracks() != null) &&
                                            LocalStream.GetAudioTracks().Count > 0;
                    Boolean containsVideo = (LocalStream.GetVideoTracks() != null) &&
                                            LocalStream.GetVideoTracks().Count > 0;

                    if (containsAudio)
                    {
                        AudioReceiver = new RTCRtpReceiver(DtlsTransport);
                        var audioParams = Helper.CapabilitiesToParameters("a", AudioReceiverCaps);
                        AudioReceiver.Receive(audioParams);
                    }

                    if (containsVideo)
                    {
                        VideoReceiver = new RTCRtpReceiver(DtlsTransport);
                        var videoParams = Helper.CapabilitiesToParameters("v", VideoReceiverCaps);
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