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

namespace ortc_standup
{
  class MediaElementWrapper;
  ZS_DECLARE_CLASS_PTR(MediaEngine)
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
    friend PromiseWithCertificateCallback;
    friend PromiseWithMediaStreamTrackListCallback;
    friend PromiseWithDeviceListCallback;

  public:
    //---------------------------------------------------------------------
    static MediaEnginePtr create(zsLib::IMessageQueuePtr queue);
    MediaEngine(zsLib::IMessageQueuePtr queue);
    ~MediaEngine();
    void init();
    void SetLocalMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetRemoteMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetStartStopButton(Windows::UI::Xaml::Controls::Button^ button);
    void StartStopMedia();

  protected:
    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IICETransportDelegate
    #pragma mark

    virtual void onICETransportStateChanged(
                                            ortc::IICETransportPtr transport,
                                            ortc::IICETransport::States state
                                            );

    virtual void onICETransportCandidatePairAvailable(
                                                      ortc::IICETransportPtr transport,
                                                      CandidatePairPtr candidatePair
                                                      );

    virtual void onICETransportCandidatePairGone(
                                                 ortc::IICETransportPtr transport,
                                                 CandidatePairPtr candidatePair
                                                 );

    virtual void onICETransportCandidatePairChanged(
                                                    ortc::IICETransportPtr transport,
                                                    CandidatePairPtr candidatePair
                                                    );

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IICEGathererDelegate
    #pragma mark

    virtual void onICEGathererStateChanged(
                                           ortc::IICEGathererPtr gatherer,
                                           ortc::IICEGatherer::States state
                                           );

    virtual void onICEGathererLocalCandidate(
                                             ortc::IICEGathererPtr gatherer,
                                             CandidatePtr candidate
                                             );

    virtual void onICEGathererLocalCandidateComplete(
                                                     ortc::IICEGathererPtr gatherer,
                                                     CandidateCompletePtr candidate
                                                     );

    virtual void onICEGathererLocalCandidateGone(
                                                 ortc::IICEGathererPtr gatherer,
                                                 CandidatePtr candidate
                                                 );

    virtual void onICEGathererError(
                                    ortc::IICEGathererPtr gatherer,
                                    ortc::IICEGathererDelegate::ErrorCode errorCode,
                                    zsLib::String errorReason
                                    );

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IDTLSTransportDelegate
    #pragma mark

    virtual void onDTLSTransportStateChanged(
                                             ortc::IDTLSTransportPtr transport,
                                             ortc::IDTLSTransport::States state
                                             );

    virtual void onDTLSTransportError(
                                      ortc::IDTLSTransportPtr transport,
                                      IDTLSTransportDelegate::ErrorCode errorCode,
                                      zsLib::String errorReason
                                      );

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IRTPSenderDelegate
    #pragma mark

    virtual void onRTPSenderError(
                                  ortc::IRTPSenderPtr sender,
                                  IRTPSenderDelegate::ErrorCode errorCode,
                                  zsLib::String errorReason
                                  );

    virtual void onRTPSenderSSRCConflict(
                                         ortc::IRTPSenderPtr sender,
                                         SSRCType ssrc
                                         );

    //-----------------------------------------------------------------------
    #pragma mark
    #pragma mark MediaEngine => IRTPReceiverDelegate
    #pragma mark

    virtual void onRTPReceiverError(
                                    ortc::IRTPReceiverPtr sender,
                                    IRTPReceiverDelegate::ErrorCode errorCode,
                                    zsLib::String errorReason
                                    );

  protected:
    //-----------------------------------------------------------------------

    Windows::UI::Core::CoreDispatcher^ mDispatcher;
    bool mStarted;

    MediaEngineWeakPtr mThisWeak;

    Windows::UI::Xaml::Controls::Button^ mStartStopButton;
    MediaElementWrapper* mLocalMediaWrapper;
    MediaElementWrapper* mRemoteMediaWrapper;
    PromiseWithCertificatePtr mPromiseWithCertificate;
    PromiseWithMediaStreamTrackListPtr mPromiseWithMediaStreamTrackList;
    PromiseWithDeviceListPtr mPromiseWithDeviceList;
    ortc::IMediaStreamTrackPtr mLocalVideoMediaStreamTrack;
    ortc::IMediaStreamTrackPtr mRemoteVideoMediaStreamTrack;
    ortc::IICEGathererPtr mICEGatherer;
    ortc::IICETransportPtr mVideoICETransport;
    ortc::IDTLSTransportPtr mVideoDTLSTransport;
    ortc::IRTPSenderPtr mVideoRTPSender;
    ortc::IRTPReceiverPtr mVideoRTPReceiver;
  };
}
