#include "pch.h"
#include "MediaEngine.h"

#include <ppltasks.h>

#include <webrtc/modules/video_render/windows/video_render_winrt.h>

using namespace ortc_standup;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

using zsLib::IPromiseResolutionDelegate;

using ortc::IMediaDevices;

ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::Constraints, Constraints)

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

namespace ortc_standup
{
  class MediaElementWrapper : public webrtc::IWinRTMediaElement
  {
  public:
    MediaElementWrapper(MediaElement^ mediaElement) :mMediaElement(mediaElement){};

    virtual void Play(){
      mMediaElement->Play();
    };

    virtual void Stop(){
      mMediaElement->Stop();
    };

    virtual void SetMediaStreamSource(Windows::Media::Core::IMediaSource^ mss){
      mMediaElement->SetMediaStreamSource(mss);
    };

    MediaElement^ getMediaElement(){ return mMediaElement; }

  private:
    MediaElement^ mMediaElement;
  };

  class PromiseWithCertificateCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithCertificateCallback(MediaEngine* mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithCertificateCallbackPtr create(MediaEngine* mediaEngine)
    {
      return PromiseWithCertificateCallbackPtr(new PromiseWithCertificateCallback(mediaEngine));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      ortc::ICertificatePtr certificate = promise->value<ortc::ICertificate>();
      mMediaEngine->mVideoDTLSTransport = ortc::IDTLSTransport::create(
                                                                       mMediaEngine->mThisWeak.lock(),
                                                                       mMediaEngine->mVideoICETransport,
                                                                       certificate
                                                                       );

      ortc::IICETypes::Parameters iceParameters;
      iceParameters.mUseCandidateFreezePriority = false;
      iceParameters.mUsernameFragment = "";
      iceParameters.mPassword = "";
      ortc::IICETransport::Options iceOptions;
      iceOptions.mAggressiveICE = false;
      iceOptions.mRole = ortc::IICETypes::Role_Controlling;

      mMediaEngine->mVideoICETransport->start(mMediaEngine->mICEGatherer, iceParameters, iceOptions);

      ortc::IDTLSTransportTypes::Parameters dtlsParameters;
      dtlsParameters.mRole = ortc::IDTLSTransportTypes::Roles::Role_Auto;
      ortc::ICertificateTypes::Fingerprint fingerprint;
      fingerprint.mAlgorithm = "";
      fingerprint.mValue = "";
      dtlsParameters.mFingerprints.push_back(fingerprint);

      mMediaEngine->mVideoDTLSTransport->start(dtlsParameters);

      mMediaEngine->mVideoRTPSender = ortc::IRTPSender::create(
                                                               ortc::IRTPSenderDelegatePtr(mMediaEngine),
                                                               mMediaEngine->mLocalVideoMediaStreamTrack,
                                                               NULL
                                                               );

      mMediaEngine->mVideoRTPReceiver = ortc::IRTPReceiver::create(
                                                                   ortc::IRTPReceiverDelegatePtr(mMediaEngine),
                                                                   NULL
                                                                   );

      ortc::IRTPTypes::CapabilitiesPtr sendVideoCaps =
        mMediaEngine->mVideoRTPSender->getCapabilities(ortc::IRTPReceiverTypes::Kinds::Kind_Video);
      ortc::IRTPTypes::CapabilitiesPtr recvVideoCaps =
        mMediaEngine->mVideoRTPReceiver->getCapabilities(ortc::IRTPReceiverTypes::Kinds::Kind_Video);

      ortc::IRTPTypes::Parameters videoSendParams;
      ortc::IRTPTypes::Parameters videoRecvParams;

      ortc::IMediaStreamTrackPtr remoteVideoTrack = mMediaEngine->mVideoRTPReceiver->track();
      remoteVideoTrack->setMediaElement(mMediaEngine->mRemoteMediaWrapper);

      mMediaEngine->mVideoRTPSender->send(videoSendParams);

      mMediaEngine->mVideoRTPReceiver->receive(videoRecvParams);

      mMediaEngine->mStarted = true;
      mMediaEngine->mStartStopButton->Content = "Stop";
      mMediaEngine->mStartStopButton->IsEnabled = true;
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithCertificateCallback()
    {
    }

  private:
    MediaEngine* mMediaEngine;
  };

  class PromiseWithMediaStreamTrackListCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithMediaStreamTrackListCallback(MediaEngine* mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithMediaStreamTrackListCallbackPtr create(MediaEngine* mediaEngine)
    {
      return PromiseWithMediaStreamTrackListCallbackPtr(new PromiseWithMediaStreamTrackListCallback(mediaEngine));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      ortc::IMediaDevicesTypes::MediaStreamTrackListPtr trackList = promise->value<ortc::IMediaDevicesTypes::MediaStreamTrackList>();
      ortc::IMediaStreamTrackPtr localVideoTrack = *trackList->begin();
      localVideoTrack->setMediaElement(mMediaEngine->mLocalMediaWrapper);
      mMediaEngine->mLocalVideoMediaStreamTrack = localVideoTrack;

      ortc::IICEGathererTypes::Options options;
      ortc::IICEGathererTypes::InterfacePolicy interfacePolicy;
      interfacePolicy.mGatherPolicy = ortc::IICEGathererTypes::FilterPolicy_None;
      options.mInterfacePolicy.push_back(interfacePolicy);
      ortc::IICEGathererTypes::Server iceServer;
      zsLib::String url = zsLib::String("stun:stun.l.google.com:19302");
      iceServer.mURLs.push_back(url);
      options.mICEServers.push_back(iceServer);
      mMediaEngine->mICEGatherer = ortc::IICEGatherer::create(mMediaEngine->mThisWeak.lock(), options);
      mMediaEngine->mVideoICETransport = ortc::IICETransport::create(mMediaEngine->mThisWeak.lock(), mMediaEngine->mICEGatherer);

      mMediaEngine->mPromiseWithCertificate = ortc::ICertificate::generateCertificate();
      mMediaEngine->mPromiseWithCertificate->then(PromiseWithCertificateCallback::create(mMediaEngine));
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithMediaStreamTrackListCallback()
    {
    }

  private:
    MediaEngine* mMediaEngine;
  };

  class PromiseWithDeviceListCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithDeviceListCallback(MediaEngine* mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithDeviceListCallbackPtr create(MediaEngine* mediaEngine)
    {
      return PromiseWithDeviceListCallbackPtr(new PromiseWithDeviceListCallback(mediaEngine));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      ortc::IMediaDevicesTypes::DeviceListPtr deviceList = promise->value<ortc::IMediaDevicesTypes::DeviceList>();
      ortc::IMediaDevicesTypes::Device device = deviceList->front();
      ortc::IMediaDevicesTypes::ConstraintSetPtr constraintSet = ortc::IMediaDevicesTypes::ConstraintSet::create();
      constraintSet->mDeviceID.mValue.value().mValue.value() = device.mDeviceID;
      constraintSet->mWidth.mValue.value() = 800;
      constraintSet->mHeight.mValue.value() = 600;
      constraintSet->mFrameRate.mValue.value() = 30;
      ConstraintsPtr constraints = Constraints::create();
      constraints->mVideo = ortc::IMediaStreamTrackTypes::TrackConstraints::create();
      constraints->mVideo->mAdvanced.push_back(constraintSet);
      mMediaEngine->mPromiseWithMediaStreamTrackList = IMediaDevices::getUserMedia(*constraints);
      mMediaEngine->mPromiseWithMediaStreamTrackList->then(PromiseWithMediaStreamTrackListCallback::create(mMediaEngine));
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithDeviceListCallback()
    {
    }

  private:
    MediaEngine* mMediaEngine;
  };
}

MediaEnginePtr MediaEngine::create(zsLib::IMessageQueuePtr queue)
{
  MediaEnginePtr pThis(new MediaEngine(queue));
  pThis->mThisWeak = pThis;
  pThis->init();
  return pThis;
}

MediaEngine::MediaEngine(zsLib::IMessageQueuePtr queue) :
  SharedRecursiveLock(SharedRecursiveLock::create()),
  MessageQueueAssociator(queue),
  mStarted(false),
  mLocalMediaWrapper(NULL),
  mRemoteMediaWrapper(NULL)
{
}

MediaEngine::~MediaEngine()
{
}

void MediaEngine::init()
{
}

void MediaEngine::SetLocalMediaElement(MediaElement^ element)
{
  mLocalMediaWrapper = new MediaElementWrapper(element);
}

void MediaEngine::SetRemoteMediaElement(MediaElement^ element)
{
  mRemoteMediaWrapper = new MediaElementWrapper(element);
}

void MediaEngine::SetStartStopButton(Button^ button)
{
  mStartStopButton = button;
}

void MediaEngine::StartStopMedia()
{
  mPromiseWithDeviceList = IMediaDevices::enumerateDevices();
  if (!mStarted) {
    mStartStopButton->IsEnabled = false;
    mPromiseWithDeviceList->then(PromiseWithDeviceListCallback::create(this));
  } else {
    if (mLocalVideoMediaStreamTrack)
      mLocalVideoMediaStreamTrack->stop();
    mStarted = false;
    mStartStopButton->Content = "Start";
  }
}

void MediaEngine::onICETransportStateChanged(
                                             ortc::IICETransportPtr transport,
                                             ortc::IICETransport::States state
                                             )
{

}

void MediaEngine::onICETransportCandidatePairAvailable(
                                                       ortc::IICETransportPtr transport,
                                                       CandidatePairPtr candidatePair
                                                       )
{

}

void MediaEngine::onICETransportCandidatePairGone(
                                                  ortc::IICETransportPtr transport,
                                                  CandidatePairPtr candidatePair
                                                  )
{

}

void MediaEngine::onICETransportCandidatePairChanged(
                                                     ortc::IICETransportPtr transport,
                                                     CandidatePairPtr candidatePair
                                                     )
{

}

void MediaEngine::onICEGathererStateChanged(
                                            ortc::IICEGathererPtr gatherer,
                                            ortc::IICEGatherer::States state
                                            )
{

}

void MediaEngine::onICEGathererLocalCandidate(
                                              ortc::IICEGathererPtr gatherer,
                                              CandidatePtr candidate
                                              )
{

}

void MediaEngine::onICEGathererLocalCandidateComplete(
                                                      ortc::IICEGathererPtr gatherer,
                                                      CandidateCompletePtr candidate
                                                      )
{

}

void MediaEngine::onICEGathererLocalCandidateGone(
                                                  ortc::IICEGathererPtr gatherer,
                                                  CandidatePtr candidate
                                                  )
{

}

void MediaEngine::onICEGathererError(
                                     ortc::IICEGathererPtr gatherer,
                                     ortc::IICEGathererDelegate::ErrorCode errorCode,
                                     zsLib::String errorReason
                                     )
{

}

void MediaEngine::onDTLSTransportStateChanged(
                                              ortc::IDTLSTransportPtr transport,
                                              ortc::IDTLSTransport::States state
                                              )
{

}

void MediaEngine::onDTLSTransportError(
                                       ortc::IDTLSTransportPtr transport,
                                       IDTLSTransportDelegate::ErrorCode errorCode,
                                       zsLib::String errorReason
                                       )
{

}

void MediaEngine::onRTPSenderError(
                                   ortc::IRTPSenderPtr sender,
                                   IRTPSenderDelegate::ErrorCode errorCode,
                                   zsLib::String errorReason
                                   )
{
}

void MediaEngine::onRTPSenderSSRCConflict(
                                          ortc::IRTPSenderPtr sender,
                                          SSRCType ssrc
                                          )
{

}

void MediaEngine::onRTPReceiverError(
                                     ortc::IRTPReceiverPtr sender,
                                     IRTPReceiverDelegate::ErrorCode errorCode,
                                     zsLib::String errorReason
                                     )
{

}
