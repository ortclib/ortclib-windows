#pragma once

#include <ortc/IICEGatherer.h>
#include <ortc/IICETransport.h>
#include <ortc/IDTLSTransport.h>
#include <ortc/IMediaDevices.h>
#include <ortc/IMediaStreamTrack.h>
#include <ortc/IRTPSender.h>
#include <ortc/IRTPReceiver.h>
#include <zsLib/MessageQueueThread.h>
#include <zsLib/Promise.h>

ZS_DECLARE_TYPEDEF_PTR(ortc::ICertificateTypes::PromiseWithCertificate, PromiseWithCertificate)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::PromiseWithDeviceList, PromiseWithDeviceList)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::PromiseWithMediaStreamTrackList, PromiseWithMediaStreamTrackList)

namespace webrtc
{
  class TraceCallback;
}

namespace ortc_standup
{
  class MediaElementWrapper;
  ZS_DECLARE_CLASS_PTR(MediaEngine)
  ZS_DECLARE_CLASS_PTR(Signaller)
  ZS_DECLARE_CLASS_PTR(PromiseWithCertificateCallback)
  ZS_DECLARE_CLASS_PTR(PromiseWithMediaStreamTrackListCallback)
  ZS_DECLARE_CLASS_PTR(PromiseWithDeviceListCallback)

  class MediaEngine : public openpeer::services::SharedRecursiveLock,
                      public zsLib::MessageQueueAssociator, 
                      public ortc::IICETransportDelegate,
                      public ortc::IICEGathererDelegate,
                      public ortc::IDTLSTransportDelegate,
                      public ortc::IRTPSenderDelegate,
                      public ortc::IRTPReceiverDelegate
  {
    friend Signaller;
    friend PromiseWithCertificateCallback;
    friend PromiseWithMediaStreamTrackListCallback;
    friend PromiseWithDeviceListCallback;

    typedef zsLib::String Hash;
    typedef std::map<Hash, CandidatePtr> CandidateMap;

  public:
    //---------------------------------------------------------------------
    static MediaEnginePtr create(zsLib::IMessageQueuePtr queue);
    MediaEngine(zsLib::IMessageQueuePtr queue);
    ~MediaEngine();
    void init();
    void setLocalMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void setRemoteMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void setStartStopButton(Windows::UI::Xaml::Controls::Button^ button);
    void makeCall();

  protected:
    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IICETransportDelegate
    #pragma mark

    virtual void onICETransportStateChange(
                                           ortc::IICETransportPtr transport,
                                           ortc::IICETransport::States state
                                           ) override;

    virtual void onICETransportCandidatePairAvailable(
                                                      ortc::IICETransportPtr transport,
                                                      CandidatePairPtr candidatePair
                                                      ) override;

    virtual void onICETransportCandidatePairGone(
                                                 ortc::IICETransportPtr transport,
                                                 CandidatePairPtr candidatePair
                                                 ) override;

    virtual void onICETransportCandidatePairChanged(
                                                    ortc::IICETransportPtr transport,
                                                    CandidatePairPtr candidatePair
                                                    ) override;

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IICEGathererDelegate
    #pragma mark

    virtual void onICEGathererStateChange(
                                          ortc::IICEGathererPtr gatherer,
                                          ortc::IICEGatherer::States state
                                          ) override;

    virtual void onICEGathererLocalCandidate(
                                             ortc::IICEGathererPtr gatherer,
                                             CandidatePtr candidate
                                             ) override;

    virtual void onICEGathererLocalCandidateComplete(
                                                     ortc::IICEGathererPtr gatherer,
                                                     CandidateCompletePtr candidate
                                                     ) override;

    virtual void onICEGathererLocalCandidateGone(
                                                 ortc::IICEGathererPtr gatherer,
                                                 CandidatePtr candidate
                                                 ) override;

    virtual void onICEGathererError(
                                    ortc::IICEGathererPtr gatherer,
                                    ortc::IICEGathererTypes::ErrorEventPtr errorEvent
                                    ) override;

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IDTLSTransportDelegate
    #pragma mark

    virtual void onDTLSTransportStateChange(
                                            ortc::IDTLSTransportPtr transport,
                                            ortc::IDTLSTransport::States state
                                            ) override;

    virtual void onDTLSTransportError(
                                      ortc::IDTLSTransportPtr transport,
                                      ortc::ErrorAnyPtr error
                                      ) override;

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IRTPSenderDelegate
    #pragma mark

    virtual void onRTPSenderSSRCConflict(
                                         ortc::IRTPSenderPtr sender,
                                         SSRCType ssrc
                                         ) override;

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IRTPReceiverDelegate
    #pragma mark

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => (internal)
    #pragma mark

    static ortc::IRTPTypes::ParametersPtr capabilitiesToSendParameters(
                                                                       ortc::IRTPTypes::CapabilitiesPtr localSendVideoCaps,
                                                                       ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                                                                       );

    static ortc::IRTPTypes::ParametersPtr capabilitiesToReceiveParameters(
                                                                          ortc::IRTPTypes::CapabilitiesPtr localRecvVideoCaps,
                                                                          ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps
                                                                          );

    virtual void onIncomingCall(
                                ortc::IICETypes::ParametersPtr remoteVideoICEParameters,
                                ortc::IDTLSTransportTypes::ParametersPtr remoteVideoDTLSParameters,
                                ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps,
                                ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                                );

    virtual void onCallAccepted(
                                ortc::IICETypes::ParametersPtr remoteVideoICEParameters,
                                ortc::IDTLSTransportTypes::ParametersPtr remoteVideoDTLSParameters,
                                ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps,
                                ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                                );

    zsLib::Log::Params log(const char *message) const;

  protected:
    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => (data)
    #pragma mark

    Windows::UI::Core::CoreDispatcher^ mDispatcher;
    bool mStarted;

    MediaEngineWeakPtr mThisWeak;

    Windows::UI::Xaml::Controls::Button^ mStartStopButton;
    Windows::UI::Xaml::Controls::MediaElement^ mLocalMediaElement;
    Windows::UI::Xaml::Controls::MediaElement^ mRemoteMediaElement;
    webrtc::TraceCallback* mTraceCallback;
    SignallerPtr mSignaller;
    PromiseWithCertificatePtr mSendVideoPromiseWithCertificate;
    PromiseWithCertificatePtr mReceiveVideoPromiseWithCertificate;
    PromiseWithMediaStreamTrackListPtr mVideoPromiseWithMediaStreamTrackList;
    PromiseWithDeviceListPtr mVideoPromiseWithDeviceList;
    ortc::IMediaStreamTrackPtr mLocalVideoMediaStreamTrack;
    ortc::IMediaStreamTrackPtr mRemoteVideoMediaStreamTrack;
    ortc::IICEGathererPtr mSendVideoICEGatherer;
    ortc::IICEGathererPtr mReceiveVideoICEGatherer;
    ortc::IICETransportPtr mSendVideoICETransport;
    ortc::IICETransportPtr mReceiveVideoICETransport;
    ortc::IDTLSTransportPtr mSendVideoDTLSTransport;
    ortc::IDTLSTransportPtr mReceiveVideoDTLSTransport;
    ortc::IRTPSenderPtr mVideoRTPSender;
    ortc::IRTPReceiverPtr mVideoRTPReceiver;
    CandidateMap mRemoteSendVideoCandidates;
    CandidateMap mRemoteReceiveVideoCandidates;
    ortc::IICETypes::ParametersPtr mLocalSendVideoICEParameters;
    ortc::IICETypes::ParametersPtr mRemoteSendVideoICEParameters;
    ortc::IICETypes::ParametersPtr mLocalReceiveVideoICEParameters;
    ortc::IICETypes::ParametersPtr mRemoteReceiveVideoICEParameters;
    ortc::IDTLSTransportTypes::ParametersPtr mLocalSendVideoDTLSParameters;
    ortc::IDTLSTransportTypes::ParametersPtr mRemoteSendVideoDTLSParameters;
    ortc::IDTLSTransportTypes::ParametersPtr mLocalReceiveVideoDTLSParameters;
    ortc::IDTLSTransportTypes::ParametersPtr mRemoteReceiveVideoDTLSParameters;
    ortc::IRTPTypes::CapabilitiesPtr mLocalSendVideoCapabilities;
    ortc::IRTPTypes::CapabilitiesPtr mRemoteSendVideoCapabilities;
    ortc::IRTPTypes::CapabilitiesPtr mLocalReceiveVideoCapabilities;
    ortc::IRTPTypes::CapabilitiesPtr mRemoteReceiveVideoCapabilities;
  };
}
