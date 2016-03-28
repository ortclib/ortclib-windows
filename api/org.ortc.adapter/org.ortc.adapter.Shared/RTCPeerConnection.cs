using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Windows.Foundation;
using org.ortc.adapter.Internal;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            internal delegate void StepEventHandler();

            public class RtcPeerConnection
            {
                private SemaphoreSlim _lock = new SemaphoreSlim(1, 1);

                internal bool Closed { get; set; } = false;
                internal string AudioSsrcLabel { get; set; } = null;
                internal string VideoSsrcLabel { get; set; } = null;
                internal string CnameSsrc { get; set; } = null;
                internal uint SsrcId { get; set; }

                internal RTCIceRole IceRole { get; set; } = RTCIceRole.Controlling;

                private bool _installedIceEvents;

                private RTCRtpSender AudioSender { get; set; }
                private RTCRtpSender VideoSender { get; set; }
                private RTCRtpReceiver AudioReceiver { get; set; }
                private RTCRtpReceiver VideoReceiver { get; set; }
                private MediaDevice AudioPlaybackDevice { get; set; }

                internal MediaStream LocalStream { get; set; }
                internal MediaStream RemoteStream { get; set; }
                private RTCIceGatherOptions Options { get; set; }
                internal RTCIceGatherer IceGatherer { get; set; }
                private RTCIceGatherer IceGathererRtcp { get; set; }
                internal RTCDtlsTransport DtlsTransport { get; set; }
                private RTCIceTransport IceTransport { get; set; }

                internal RTCRtpParameters AudioSenderRtpParameters { get; set; }
                internal RTCRtpParameters VideoSenderRtpParameters { get; set; }
                internal RTCDtlsParameters DtlsParameters { get; set; }
                internal RTCRtpCapabilities AudioSenderCaps { get; set; }
                internal RTCRtpCapabilities VideoSenderCaps { get; set; }
                internal RTCRtpCapabilities AudioReceiverCaps { get; set; }
                internal RTCRtpCapabilities VideoReceiverCaps { get; set; }

                internal RTCRtpParameters AudioReceiverRtpParameters { get; set; }
                internal RTCRtpParameters VideoReceiverRtpParameters { get; set; }

                internal ulong SessionId { get; set; }
                internal ushort SessionVersion { get; set; }

                internal RTCIceParameters RemoteRtcIceParameters { get; set; }
                internal RTCDtlsParameters RemoteRtcDtlsParameters { get; set; }

                private TaskCompletionSource<RTCSessionDescription> CapabilitiesTcs { get; set; }
                private TaskCompletionSource<RTCSessionDescription> CapabilitiesFinalTcs { get; set; }
                private TaskCompletionSource<RTCSessionDescription> RemoteCapabilitiesTcs { get; set; }
                private event StepEventHandler OnStep;

                public static void SetPreferredVideoCaptureFormat(int frameWidth,
                    int frameHeight, int fps)
                {

                }

                public delegate void RtcPeerConnectionIceEventDelegate(RTCPeerConnectionIceEvent evt);

                public delegate void MediaStreamEventEventDelegate(MediaStreamEvent evt);

                public delegate void RtcPeerConnectionHealthStatsDelegate(RTCPeerConnectionHealthStats stats);

                //private event RtcPeerConnectionIceEventDelegate _onIceCandidate;

                public event RtcPeerConnectionIceEventDelegate OnIceCandidate;
                /*{
                    add { _onIceCandidate += value; }
                    remove { _onIceCandidate -= value; }
                }*/

                public event MediaStreamEventEventDelegate OnAddStream;
                public event MediaStreamEventEventDelegate OnRemoveStream;
                public event RtcPeerConnectionHealthStatsDelegate OnConnectionHealthStats;

                public RtcPeerConnection(RTCConfiguration configuration)
                {
                    Settings.ApplyDefaults();
                    Ortc.Setup();

                    var options = Helper.ToGatherOptions(configuration);
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        _installedIceEvents = false;

                        IceGatherer = new RTCIceGatherer(options);
                        IceTransport = new RTCIceTransport(IceGatherer);

                        // An internal event handler to continue the setup process various media and
                        // device selection APIs are called by the application layer.
                        OnStep += RTCPeerConnection_OnStep;
                    }

                    RTCCertificate.GenerateCertificate("").AsTask<RTCCertificate>().ContinueWith((cert) =>
                    {
                        using (var @lock = new AutoLock(_lock))
                        {
                            @lock.WaitAsync().Wait();
                            // Since the DTLS certificate is ready the RtcDtlsTransport can now be constructed.
                            var certs = new List<RTCCertificate>();
                            certs.Add(cert.Result);
                            DtlsTransport = new RTCDtlsTransport(IceTransport, certs);
                            if (Closed) DtlsTransport.Stop();
                        }

                        // Kick start the continued setup process.
                        Wake();
                    });

                    /*Logger.SetLogLevel(Log.Level.Trace);
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
                    SetCapabilities();*/

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
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        LocalStream = stream;
                    }
                    Wake();
                }

                public void Close()
                {
                    IceGatherer.Close();
                    IceTransport.Stop();

                    TaskCompletionSource<RTCSessionDescription> capabilitiesTcs;
                    TaskCompletionSource<RTCSessionDescription> capabilitiesFinalTcs;

                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        if (_installedIceEvents)
                        {
                            IceGatherer.OnICEGathererLocalCandidate -= RTCIceGatherer_onICEGathererLocalCandidate;
                            IceGatherer.OnICEGathererCandidateComplete -= RTCIceGatherer_onICEGathererCandidateComplete;
                            IceGatherer.OnICEGathererLocalCandidateGone -= RTCIceGatherer_onICEGathererLocalCandidateGone;
                            _installedIceEvents = false;
                        }

                        if (null != DtlsTransport) DtlsTransport.Stop();

                        capabilitiesTcs = CapabilitiesTcs;
                        capabilitiesFinalTcs = CapabilitiesFinalTcs;
                        //if (null != RemoteStream) { RemoteStream.Stop(); }
                        if (null != AudioSender) { AudioSender.Stop(); }
                        if (null != VideoSender) { VideoSender.Stop(); }
                        if (null != AudioReceiver) { AudioReceiver.Stop(); }
                        if (null != VideoReceiver) { VideoReceiver.Stop(); }
                    }

                    if (null != capabilitiesTcs)
                    {
                        capabilitiesTcs.SetResult(null);
                    }
                    if (null != capabilitiesFinalTcs)
                    {
                        capabilitiesFinalTcs.SetResult(null);
                    }
                }

                public IAsyncAction SetRemoteDescription(RTCSessionDescription description)
                {
                    var remoteCapabilitiesTcs = new TaskCompletionSource<RTCSessionDescription>();
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        //_remoteCapabilities = description;
                        RemoteCapabilitiesTcs = remoteCapabilitiesTcs;
                    }
                    Wake();

                    var result = remoteCapabilitiesTcs.Task.AsAsyncAction();
                    remoteCapabilitiesTcs.SetResult(description);
                    return result;
                    /*Task ret = Task.Run(() =>
                    {
                        SDPConvertor.ParseSdp(description.Sdp,description.Type,this);
                        PrepareSender();
                    });
                    return ret.AsAsyncAction();*/
                }

                public IAsyncOperation<RTCSessionDescription> CreateAnswer()
                {
                    var capabilitiesTcs = new TaskCompletionSource<RTCSessionDescription>();
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        IceRole = RTCIceRole.Controlled;
                        CapabilitiesTcs = capabilitiesTcs;
                    }
                    Wake();
                    return capabilitiesTcs.Task.AsAsyncOperation();

                    /*Task<RTCSessionDescription> ret = Task.Run<RTCSessionDescription>(() =>
                    {
                        RTCSessionDescription sd = new RTCSessionDescription(RTCSdpType.Answer, SdpGenerator.CreateSdp(this, RTCSdpType.Answer));
                        return sd;
                    });

                    return ret.AsAsyncOperation<RTCSessionDescription>();*/
                }

                
                
                public IAsyncOperation<RTCSessionDescription> CreateOffer()
                {
                    var capabilitiesTcs = new TaskCompletionSource<RTCSessionDescription>();
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        IceRole = RTCIceRole.Controlling;
                        CapabilitiesTcs = capabilitiesTcs;
                    }
                    Wake();
                    return capabilitiesTcs.Task.AsAsyncOperation();
                    /*Task<RTCSessionDescription> ret = Task.Run<RTCSessionDescription>(() =>
                    {
                        RTCSessionDescription sd = new RTCSessionDescription(RTCSdpType.Offer, SdpGenerator.CreateSdp(this, RTCSdpType.Offer));
                        return sd;
                    });

                    return ret.AsAsyncOperation<RTCSessionDescription>();*/
                }

                public IAsyncAction SetLocalDescription(RTCSessionDescription description)
                {
                    var capabilitiesFinalTcs = new TaskCompletionSource<RTCSessionDescription>();
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        //_localCapabilitiesFinal = description;
                        CapabilitiesFinalTcs = capabilitiesFinalTcs;
                    }
                    Wake();
                    return capabilitiesFinalTcs.Task.AsAsyncAction();
                    /*Task ret = Task.Run(() =>
                    {
                        PrepareReceiver();
                    });
                    return ret.AsAsyncAction();*/
                }

                public IAsyncAction AddIceCandidate(RTCIceCandidate candidate)
                {
                    return Task.Run(() =>
                    {
                        org.ortc.RTCIceCandidate iceCandidate = Helper.IceCandidateFromSdp(candidate.Candidate);
                        IceTransport.AddRemoteCandidate(iceCandidate);
                    }).AsAsyncAction();
                    
                }


                /*private void PrepareGatherer(RTCConfiguration configuration)
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
                }*/

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
                        var audioParams = Helper.CapabilitiesToParameters(SsrcId,CnameSsrc,"a",AudioReceiverCaps);
                        AudioReceiver.Receive(audioParams);
                    }

                    if (containsVideo)
                    {
                        VideoReceiver = new RTCRtpReceiver(DtlsTransport);
                        var videoParams = Helper.CapabilitiesToParameters(SsrcId, CnameSsrc, "v",VideoReceiverCaps);
                        VideoReceiver.Receive(videoParams);
                    }
                }
                private void PrepareSender()
                {
                    Boolean containsAudio = (LocalStream.GetAudioTracks() != null) && LocalStream.GetAudioTracks().Count > 0;
                    Boolean containsVideo = (LocalStream.GetVideoTracks() != null) && LocalStream.GetVideoTracks().Count > 0;

                    //From SDP obtain remote IceParameters and IceRole
                    IceTransport.Start(IceGatherer, RemoteRtcIceParameters, IceRole);

                    //From SDP obtain remote dtls parameters
                    DtlsTransport.Start(RemoteRtcDtlsParameters);

                    if (containsAudio)
                    {
                        if (null == AudioSender)
                        {
                            // Figure out if the application has audio media streams to send to the remote party.
                            var tracks = LocalStream.GetAudioTracks();
                            MediaAudioTrack mediaTrack = null;
                            MediaStreamTrack track = null;
                            if (tracks?.Count > 0) mediaTrack = tracks.First();
                            if (null != mediaTrack) track = mediaTrack.Track;

                            if (null != track)
                            {
                                // If a track was found then setup the audio RtcRtpSender.
                                AudioSender = new RTCRtpSender(track, DtlsTransport);
                                AudioSender.Send(AudioSenderRtpParameters);
                            }
                        }
                    }

                    // If the local and remote party are sending video then setup the video RtcRtpSender.
                    if (containsVideo)
                    {
                        if (null == VideoSender)
                        {
                            // Figure out if the application has video media streams to send to the remote party.
                            var tracks = LocalStream.GetVideoTracks();
                            MediaVideoTrack mediaTrack = null;
                            MediaStreamTrack track = null;
                            if (tracks?.Count > 0) mediaTrack = tracks.First();
                            if (null != mediaTrack) track = mediaTrack.Track;

                            if (null != track)
                            {
                                // If a track was found then setup the video RtcRtpSender.
                                VideoSender = new RTCRtpSender(track, DtlsTransport);
                                VideoSender.Send(VideoSenderRtpParameters);
                            }
                        }
                    }
                }

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
                    if (OnIceCandidate != null)
                        OnIceCandidate(iceEvent);
                }

                private void RTCIceGatherer_onRTCPICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
                {
                    var iceEvent = new RTCPeerConnectionIceEvent();
                    iceEvent.Candidate = Helper.ToWrapperIceCandidate(evt.Candidate, 2); //RTCP component
                    if (OnIceCandidate != null)
                        OnIceCandidate(iceEvent);
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

                /// <summary>
                /// Setup of an ORTC connection must be done in 3 main steps:
                /// 1. Obtain the capabilities of the ORTC API.
                /// 2. Setup an ORTC RtcRtpReciever to receiver incoming audio and video media.
                /// 3. Setup an ORTC RtcRtpSender to send outgoing audio and video media.
                /// </summary>
                private void RTCPeerConnection_OnStep()
                {
                    TaskCompletionSource<RTCSessionDescription> capabilitiesTcs;
                    TaskCompletionSource<RTCSessionDescription> capabilitiesFinalTcs;
                    RTCSessionDescription capabilitiesTcsResult;
                    RTCSessionDescription capabilitiesFinalTcsResult;

                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        // If the object was closed before the setup event is triggered then abort early.
                        if (Closed)
                        {
                            Close();
                            return;
                        }

                        // Can only obtain and setup the senders and receivers if the RtcDtlsTransport is ready.
                        if (null == DtlsTransport) return;

                        // Perform the three main steps to setup the capabilities, and incoming and outgoing media.
                        StepGetCapabilities(out capabilitiesTcs, out capabilitiesTcsResult);
                        StepSetupReceiver(out capabilitiesFinalTcs, out capabilitiesFinalTcsResult);
                        StepSetupSender();
                    }

                    // Return the "blob" information asynchronously to the calling application outside the lock.
                    capabilitiesTcs?.SetResult(capabilitiesTcsResult);
                    capabilitiesFinalTcs?.SetResult(capabilitiesFinalTcsResult);
                }
                private void Wake()
                {
                    Task.Run(() =>
                    {
                        OnStep?.Invoke();
                    });
                }

                /// <summary>
                /// Obtain the capabilities of the ORTC engine. The application will need to exchange
                /// these capabilities with the remote peer so they can mutually configuring the codecs,
                /// media and RTP parameters necessary to establish an RTC peer session.
                /// </summary>
                private void StepGetCapabilities(
                    out TaskCompletionSource<RTCSessionDescription> tcs,
                    out RTCSessionDescription result
                    )
                {
                    tcs = null;
                    result = null;

                    if (null == CapabilitiesTcs) return;

                    // Obtain basic ICE configuration information like the usernameFragment and password.
                    var localParams = IceGatherer.GetLocalParameters();

                    // Obtain the complete capabilities of the RtcRtpSenders/RtcRtpReceivers for both
                    // audio and video.
                    AudioSenderCaps = RTCRtpSender.GetCapabilities("audio");
                    VideoSenderCaps = RTCRtpSender.GetCapabilities("video");
                    AudioReceiverCaps = RTCRtpReceiver.GetCapabilities("audio");
                    VideoReceiverCaps = RTCRtpReceiver.GetCapabilities("video");
                    DtlsParameters = DtlsTransport.GetLocalParameters();

                    if (IceRole == RTCIceRole.Controlling)
                        result = new RTCSessionDescription(RTCSdpType.Offer, SdpGenerator.CreateSdp(this, RTCSdpType.Offer));
                    else
                        result = new RTCSessionDescription(RTCSdpType.Answer, SdpGenerator.CreateSdp(this, RTCSdpType.Answer));

                    /*
                    // Put all this information into a "blob" that can be JSONified and sent to the remote party.
                    var caps = new RTCSessionDescription(new RtcDescription(hasAudio, hasVideo, _iceRole, localParams, audioSenderCaps, videoSenderCaps, audioReceiverCaps, videoReceiverCaps, dtlsParameters));

                    // If the remote peer's codec information is known, readjust the codec ordering to match the
                    // remote peer's order to ensure both sender and receiver are using the same codec. This is not
                    // a requirement of ORTC but typically the same codec is used bidirectionally.
                    if (null != _remoteCapabilities) { RtcHelper.PickLocalCodecBasedOnRemote(caps, _remoteCapabilities); }

                    // Remember the capabilities for use later when setting up the RtcRtpSenders/RtcRtpReceivers
                    _localCapabilities = caps;
                    */
                    tcs = CapabilitiesTcs;
                    CapabilitiesTcs = null;
                    //result = _localCapabilities;
                }

                /// <summary>
                /// Setup ORTC to receive incoming audio and/or video media from the remote peer.
                /// </summary>
                /// <remarks>
                /// The application is given the opportunity to override, modify, or tweak and of the RTP
                /// settings before commiting to a particular set of media options. This section uses the
                /// final set of parameters specificied by the application to setup incoming media.
                /// </remarks>
                private void StepSetupReceiver(
                    out TaskCompletionSource<RTCSessionDescription> tcs,
                    out RTCSessionDescription result
                    )
                {
                    tcs = null;
                    result = null;

                    if (null == CapabilitiesFinalTcs) return;

                    MediaAudioTrack incomingAudioTrack = null;
                    MediaVideoTrack incomingVideoTrack = null;

                    // Figure out of the local and remote desire to send/receive audio and video.
                    bool hasAudio = HasAudio();
                    bool hasVideo = HasVideo();
                    /*bool hasAudio = _localCapabilitiesFinal.Description.HasAudio;
                    bool hasVideo = _localCapabilitiesFinal.Description.HasVideo;
                    if (null != _remoteCapabilities)
                    {
                        hasAudio = hasAudio && _remoteCapabilities.Description.HasAudio;
                        hasVideo = hasVideo && _remoteCapabilities.Description.HasVideo;
                    }*/

                    if (hasAudio)
                    {
                        AudioReceiver = new RTCRtpReceiver(DtlsTransport);
                        var audioParams = Helper.CapabilitiesToParameters(SsrcId, CnameSsrc, "a", AudioReceiverCaps);
                        AudioReceiver.Receive(audioParams);
                        incomingAudioTrack = new MediaAudioTrack(AudioReceiver.Track);
                    }

                    if (hasVideo)
                    {
                        VideoReceiver = new RTCRtpReceiver(DtlsTransport);
                        var videoParams = Helper.CapabilitiesToParameters(SsrcId, CnameSsrc, "v", VideoReceiverCaps);
                        VideoReceiver.Receive(videoParams);
                        incomingVideoTrack = new MediaVideoTrack(VideoReceiver.Track);
                    }
/*
                    // If audio is desired then setup the incoming audio RTP receiver
                    if (hasAudio)
                    {
                        AudioReceiver = new RTCRtpReceiver(DtlsTransport);

                        // Given the local capabilities offered setup a receiver by converting the capabilities
                        // offered into settings needed to configure the receiver using the helper routine and
                        // label the receiver stream with identifier "a" for audio.
                        var @params = Helper.CapabilitiesToParameters("a", _localCapabilities.Description.AudioReceiverCapabilities);
                        _audioReceiver.Receive(@params);
                        incomingAudioTrack = new MediaAudioTrack(AudioReceiver.Track);
                    }

                    // If video is desired then setup the incoming audio RTP receiver
                    if (hasVideo)
                    {
                        VideoReceiver = new RTCRtpReceiver(DtlsTransport);

                        // Given the local capabilities offered setup a receiver by converting the capabilities
                        // offered into settings needed to configure the receiver using the helper routine and
                        // label the receiver stream with identifier "v" for video.
                        var @params = Helper.CapabilitiesToParameters("v", _localCapabilities.Description.VideoReceiverCapabilities);
                        _videoReceiver.Receive(@params);
                        incomingVideoTrack = new MediaVideoTrack(_videoReceiver.Track);
                    }
                    */
                    // ORTC allows for simulcasting so return the list of audio/video streams created to the
                    // application. However, in this particular example only one audio and video stream
                    // are used since it's a peer to peer scenario.
                    List<MediaAudioTrack> incomingAudioTracks = null;
                    List<MediaVideoTrack> incomingVideoTracks = null;
                    if (null != incomingAudioTrack)
                    {
                        incomingAudioTracks = new List<MediaAudioTrack>();
                        incomingAudioTracks.Add(incomingAudioTrack);
                    }
                    if (null != incomingVideoTrack)
                    {
                        incomingVideoTracks = new List<MediaVideoTrack>();
                        incomingVideoTracks.Add(incomingVideoTrack);
                    }

                    // Setup a helper media stream containing all the incoming audio and video media "tracks".
                    var mediaStream = new MediaStream(incomingAudioTracks, incomingVideoTracks);
                    RemoteStream = mediaStream;
                    var evt = new MediaStreamEvent();
                    evt.Stream = RemoteStream;
                    Task.Run(() => { OnAddStream(evt); });

                    if (!_installedIceEvents)
                    {
                        IceGatherer.OnICEGathererLocalCandidate += RTCIceGatherer_onICEGathererLocalCandidate;
                        IceGatherer.OnICEGathererCandidateComplete += RTCIceGatherer_onICEGathererCandidateComplete;
                        IceGatherer.OnICEGathererLocalCandidateGone += RTCIceGatherer_onICEGathererLocalCandidateGone;
                        _installedIceEvents = true;
                    }

                    tcs = CapabilitiesFinalTcs;
                    CapabilitiesFinalTcs = null;
                    result = null;
                }

                /// <summary>
                /// Setup ORTC to sender outgoing audio and/or video media from the remote peer.
                /// </summary>
                /// <remarks>
                /// The application is given the opportunity to override, modify, or tweak and of the RTP
                /// settings before commiting to a particular set of media options. This section uses the
                /// final set of parameters specificied by the application to setup outgoing media.
                /// </remarks>
                private void StepSetupSender()
                {
                    // Only setup the sender information if all the information is ready and the remote
                    // peer's capabilities are known.
                    if (null == RemoteCapabilitiesTcs) return;
                    if (null == LocalStream) return;
                    if (null == AudioSenderRtpParameters && null == VideoSenderRtpParameters) return;
                    if (null == RemoteRtcDtlsParameters) return;

                    // Start the ICE engine communication with the remote peer.
                    IceTransport.Start(IceGatherer, RemoteRtcIceParameters, IceRole);

                    // Start the DTLS transport and expect a connection with a DTLS certificate provide by the remote party.
                    DtlsTransport.Start(RemoteRtcDtlsParameters);

                    // If the local and remote party are sending audio then setup the audio RtcRtpSender.
                    if (HasAudio())
                    {
                        if (null == AudioSender)
                        {
                            // Figure out if the application has audio media streams to send to the remote party.
                            var tracks = LocalStream.GetAudioTracks();
                            MediaAudioTrack mediaTrack = null;
                            MediaStreamTrack track = null;
                            if (null != tracks) { if (tracks.Count > 0) { mediaTrack = tracks.First(); } }
                            if (null != mediaTrack) track = mediaTrack.Track;

                            if (null != track)
                            {
                                // If a track was found then setup the audio RtcRtpSender.
                                AudioSender = new RTCRtpSender(track, DtlsTransport);
                                AudioSender.Send(AudioSenderRtpParameters);
                            }
                        }
                    }

                    // If the local and remote party are sending video then setup the video RtcRtpSender.
                    if (HasVideo())
                    {
                        if (null == VideoSender)
                        {
                            // Figure out if the application has video media streams to send to the remote party.
                            var tracks = LocalStream.GetVideoTracks();
                            MediaVideoTrack mediaTrack = null;
                            MediaStreamTrack track = null;
                            if (null != tracks) { if (tracks.Count > 0) { mediaTrack = tracks.First(); } }
                            if (null != mediaTrack) track = mediaTrack.Track;

                            if (null != track)
                            {
                                // If a track was found then setup the video RtcRtpSender.
                                VideoSender = new RTCRtpSender(track, DtlsTransport);
                                VideoSender.Send(VideoSenderRtpParameters);
                            }
                        }
                    }

                    RemoteCapabilitiesTcs = null;
                }

                internal bool HasAudio()
                {
                    bool hasAudio = false;
                    

                    if (null != LocalStream)
                    {
                        var audioTracks = LocalStream.GetAudioTracks();
                        if (null != audioTracks) { hasAudio = (audioTracks.Count > 0); }
                    }

                    // If the remote peer's capabilities are already known at this time do not offer media beyond
                    // what the remote side desires even if the local application offers the media devices needed
                    // for more.
                    if (null != AudioSenderRtpParameters)
                        hasAudio = true;

                    return hasAudio;
                }

                internal bool HasVideo()
                {
                    bool hasVideo = false;

                    if (null != LocalStream)
                    {
                        var videoTracks = LocalStream.GetVideoTracks();
                        if (null != videoTracks) { hasVideo = (videoTracks.Count > 0); }
                    }

                    // If the remote peer's capabilities are already known at this time do not offer media beyond
                    // what the remote side desires even if the local application offers the media devices needed
                    // for more.
                    if (null != VideoSenderRtpParameters)
                        hasVideo = true;

                    return hasVideo;
                }
            }
        }
    }
}