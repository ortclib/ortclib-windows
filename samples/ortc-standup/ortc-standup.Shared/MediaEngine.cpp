#include "pch.h"
#include "MediaEngine.h"

#include <ppltasks.h>

#include <zsLib/XML.h>

#include <openpeer/services/ILogger.h>

#include <ortc/ISettings.h>

#include <webrtc/base/logging.h>
#include <webrtc/modules/video_render/windows/video_render_winrt.h>
#include <webrtc/system_wrappers/include/trace.h>

using namespace ortc_standup;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

using zsLib::IPromiseResolutionDelegate;

using ortc::IMediaDevices;

namespace ortc_standup { ZS_IMPLEMENT_SUBSYSTEM(ortc_standup) }
namespace ortc_standup { ZS_DECLARE_SUBSYSTEM(ortc_standup) }

ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::Constraints, Constraints)

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

namespace ortc_standup
{
  class StandupTraceCallback : public webrtc::TraceCallback
  {
  public:

    virtual void Print(webrtc::TraceLevel level, const char* message, int length)
    {
      rtc::LoggingSeverity sev = rtc::LS_VERBOSE;
      if (level == webrtc::kTraceError || level == webrtc::kTraceCritical)
        sev = rtc::LS_ERROR;
      else if (level == webrtc::kTraceWarning)
        sev = rtc::LS_WARNING;
      else if (level == webrtc::kTraceStateInfo || level == webrtc::kTraceInfo)
        sev = rtc::LS_INFO;
      else if (level == webrtc::kTraceTerseInfo)
        sev = rtc::LS_INFO;

      // Skip past boilerplate prefix text
      if (length < 72) {
        std::string msg(message, length);
        LOG(LS_ERROR) << "Malformed webrtc log message: ";
        LOG_V(sev) << msg;
      } else {
        std::string msg(message + 71, length - 72);
        LOG_V(sev) << "webrtc: " << msg;
      }
    }
  };

  class Signaller
  {
  private:
    Signaller(MediaEnginePtr mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static SignallerPtr create(MediaEnginePtr mediaEngine)
    {
      return SignallerPtr(new Signaller(mediaEngine));
    }

    void sendLocalSendCandidate(ortc::IICETypes::CandidatePtr candidate)
    {
      if (mMediaEngine.lock()->mReceiveVideoICETransport) {
        MediaEngine::CandidateMap::iterator iter = mMediaEngine.lock()->mRemoteReceiveVideoCandidates.begin();
        while (iter != mMediaEngine.lock()->mRemoteReceiveVideoCandidates.end()) {
          mMediaEngine.lock()->mReceiveVideoICETransport->addRemoteCandidate(*iter->second);
          iter++;
        }
        mMediaEngine.lock()->mRemoteReceiveVideoCandidates.clear();
        mMediaEngine.lock()->mReceiveVideoICETransport->addRemoteCandidate(*candidate);
      } else {
        auto hash = candidate->hash();
        mMediaEngine.lock()->mRemoteReceiveVideoCandidates[hash] = candidate;
      }
    }

    void sendLocalSendCandidate(ortc::IICETypes::CandidateCompletePtr candidate)
    {
      if (mMediaEngine.lock()->mReceiveVideoICETransport) {
        MediaEngine::CandidateMap::iterator iter = mMediaEngine.lock()->mRemoteReceiveVideoCandidates.begin();
        while (iter != mMediaEngine.lock()->mRemoteReceiveVideoCandidates.end()) {
          mMediaEngine.lock()->mReceiveVideoICETransport->addRemoteCandidate(*iter->second);
          iter++;
        }
        mMediaEngine.lock()->mRemoteReceiveVideoCandidates.clear();
        mMediaEngine.lock()->mReceiveVideoICETransport->addRemoteCandidate(*candidate);
      }
    }

    void sendLocalReceiveCandidate(ortc::IICETypes::CandidatePtr candidate)
    {
      if (mMediaEngine.lock()->mSendVideoICETransport) {
        MediaEngine::CandidateMap::iterator iter = mMediaEngine.lock()->mRemoteSendVideoCandidates.begin();
        while (iter != mMediaEngine.lock()->mRemoteSendVideoCandidates.end()) {
          mMediaEngine.lock()->mSendVideoICETransport->addRemoteCandidate(*iter->second);
          iter++;
        }
        mMediaEngine.lock()->mRemoteSendVideoCandidates.clear();
        mMediaEngine.lock()->mSendVideoICETransport->addRemoteCandidate(*candidate);
      } else {
        auto hash = candidate->hash();
        mMediaEngine.lock()->mRemoteSendVideoCandidates[hash] = candidate;
      }
    }

    void sendLocalReceiveCandidate(ortc::IICETypes::CandidateCompletePtr candidate)
    {
      if (mMediaEngine.lock()->mSendVideoICETransport) {
        MediaEngine::CandidateMap::iterator iter = mMediaEngine.lock()->mRemoteSendVideoCandidates.begin();
        while (iter != mMediaEngine.lock()->mRemoteSendVideoCandidates.end()) {
          mMediaEngine.lock()->mSendVideoICETransport->addRemoteCandidate(*iter->second);
          iter++;
        }
        mMediaEngine.lock()->mRemoteSendVideoCandidates.clear();
        mMediaEngine.lock()->mSendVideoICETransport->addRemoteCandidate(*candidate);
      }
    }

    void sendOffer(
                   ortc::IICETypes::ParametersPtr localVideoICEParameters,
                   ortc::IDTLSTransportTypes::ParametersPtr localVideoDTLSParameters,
                   ortc::IRTPTypes::CapabilitiesPtr localSendVideoCaps,
                   ortc::IRTPTypes::CapabilitiesPtr localRecvVideoCaps
                   )
    {
      onOfferReceived(
                      localVideoICEParameters,
                      localVideoDTLSParameters,
                      localSendVideoCaps,
                      localRecvVideoCaps
                      );
    }

    void sendAnswer(
                    ortc::IICETypes::ParametersPtr localVideoICEParameters,
                    ortc::IDTLSTransportTypes::ParametersPtr localVideoDTLSParameters,
                    ortc::IRTPTypes::CapabilitiesPtr localSendVideoCaps,
                    ortc::IRTPTypes::CapabilitiesPtr localRecvVideoCaps
                    )
    {
      onAnswerReceived(
                       localVideoICEParameters,
                       localVideoDTLSParameters,
                       localSendVideoCaps,
                       localRecvVideoCaps
                       );
    }

    void onOfferReceived(
                         ortc::IICETypes::ParametersPtr remoteVideoICEParameters,
                         ortc::IDTLSTransportTypes::ParametersPtr remoteVideoDTLSParameters,
                         ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps,
                         ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                         )
    {
      mMediaEngine.lock()->onIncomingCall(
                                          remoteVideoICEParameters,
                                          remoteVideoDTLSParameters,
                                          remoteSendVideoCaps,
                                          remoteRecvVideoCaps
                                          );
    }

    void onAnswerReceived(
                          ortc::IICETypes::ParametersPtr remoteVideoICEParameters,
                          ortc::IDTLSTransportTypes::ParametersPtr remoteVideoDTLSParameters,
                          ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps,
                          ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                          )
    {
      mMediaEngine.lock()->onCallAccepted(
                                          remoteVideoICEParameters,
                                          remoteVideoDTLSParameters,
                                          remoteSendVideoCaps,
                                          remoteRecvVideoCaps
                                          );
    }

  private:
    MediaEngineWeakPtr mMediaEngine;
  };

  class PromiseWithCertificateCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithCertificateCallback(MediaEnginePtr mediaEngine, bool sender) : 
      mMediaEngine(mediaEngine), mSender(sender)
    {
    }

  public:
    static PromiseWithCertificateCallbackPtr create(MediaEnginePtr mediaEngine, bool sender)
    {
      return PromiseWithCertificateCallbackPtr(new PromiseWithCertificateCallback(mediaEngine, sender));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      if (mSender) {
        ortc::ICertificatePtr certificate = promise->value<ortc::ICertificate>();
        std::list<ortc::ICertificatePtr> certificates;
        certificates.push_back(certificate);
        mMediaEngine.lock()->mSendVideoDTLSTransport = ortc::IDTLSTransport::create(
                                                                                    mMediaEngine.lock(),
                                                                                    mMediaEngine.lock()->mSendVideoICETransport,
                                                                                    certificates
                                                                                    );

        mMediaEngine.lock()->mVideoRTPSender = ortc::IRTPSender::create(
                                                                        mMediaEngine.lock(),
                                                                        mMediaEngine.lock()->mLocalVideoMediaStreamTrack,
                                                                        mMediaEngine.lock()->mSendVideoDTLSTransport
                                                                        );

        mMediaEngine.lock()->mLocalSendVideoICEParameters =
          mMediaEngine.lock()->mSendVideoICEGatherer->getLocalParameters();
        mMediaEngine.lock()->mLocalSendVideoDTLSParameters =
          mMediaEngine.lock()->mSendVideoDTLSTransport->getLocalParameters();
        mMediaEngine.lock()->mLocalSendVideoCapabilities =
          mMediaEngine.lock()->mVideoRTPSender->getCapabilities(ortc::IRTPReceiverTypes::Kinds::Kind_Video);

        mMediaEngine.lock()->mSignaller->sendOffer(
                                                   mMediaEngine.lock()->mLocalSendVideoICEParameters,
                                                   mMediaEngine.lock()->mLocalSendVideoDTLSParameters,
                                                   mMediaEngine.lock()->mLocalSendVideoCapabilities,
                                                   NULL
                                                   );
      } else {
        ortc::ICertificatePtr certificate = promise->value<ortc::ICertificate>();
        std::list<ortc::ICertificatePtr> certificates;
        certificates.push_back(certificate);
        mMediaEngine.lock()->mReceiveVideoDTLSTransport = ortc::IDTLSTransport::create(
                                                                                       mMediaEngine.lock(),
                                                                                       mMediaEngine.lock()->mReceiveVideoICETransport,
                                                                                       certificates
                                                                                       );

        mMediaEngine.lock()->mVideoRTPReceiver = ortc::IRTPReceiver::create(
                                                                            mMediaEngine.lock(),
                                                                            ortc::IMediaStreamTrackTypes::Kind_Video,
                                                                            mMediaEngine.lock()->mReceiveVideoDTLSTransport
                                                                            );

        mMediaEngine.lock()->mLocalReceiveVideoICEParameters =
          mMediaEngine.lock()->mReceiveVideoICEGatherer->getLocalParameters();
        mMediaEngine.lock()->mLocalReceiveVideoDTLSParameters =
          mMediaEngine.lock()->mReceiveVideoDTLSTransport->getLocalParameters();
        mMediaEngine.lock()->mLocalReceiveVideoCapabilities =
          mMediaEngine.lock()->mVideoRTPReceiver->getCapabilities(ortc::IRTPReceiverTypes::Kinds::Kind_Video);

        ortc::IICETransport::Options videoICEOptions;
        videoICEOptions.mAggressiveICE = false;
        videoICEOptions.mRole = ortc::IICETypes::Role_Controlled;

        mMediaEngine.lock()->mReceiveVideoICETransport->start(
                                                              mMediaEngine.lock()->mReceiveVideoICEGatherer,
                                                              *(mMediaEngine.lock()->mRemoteSendVideoICEParameters),
                                                              videoICEOptions
                                                              );

        mMediaEngine.lock()->mReceiveVideoDTLSTransport->start(*(mMediaEngine.lock()->mRemoteSendVideoDTLSParameters));

        ortc::IRTPTypes::ParametersPtr videoRecvParams = MediaEngine::capabilitiesToReceiveParameters(
                                                                                                      mMediaEngine.lock()->mLocalReceiveVideoCapabilities,
                                                                                                      mMediaEngine.lock()->mRemoteSendVideoCapabilities
                                                                                                      );

        ortc::IMediaStreamTrackPtr remoteVideoTrack = mMediaEngine.lock()->mVideoRTPReceiver->track();
        IInspectable* remoteMediaElementPtr = reinterpret_cast<IInspectable*>(mMediaEngine.lock()->mRemoteMediaElement);
        remoteVideoTrack->setMediaElement(remoteMediaElementPtr);

        mMediaEngine.lock()->mVideoRTPReceiver->receive(*videoRecvParams);

        mMediaEngine.lock()->mSignaller->sendAnswer(
                                                    mMediaEngine.lock()->mLocalReceiveVideoICEParameters,
                                                    mMediaEngine.lock()->mLocalReceiveVideoDTLSParameters,
                                                    NULL,
                                                    mMediaEngine.lock()->mLocalReceiveVideoCapabilities
                                                    );
      }
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithCertificateCallback()
    {
    }

  private:
    MediaEngineWeakPtr mMediaEngine;
    bool mSender;
  };

  class PromiseWithMediaStreamTrackListCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithMediaStreamTrackListCallback(MediaEnginePtr mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithMediaStreamTrackListCallbackPtr create(MediaEnginePtr mediaEngine)
    {
      return PromiseWithMediaStreamTrackListCallbackPtr(new PromiseWithMediaStreamTrackListCallback(mediaEngine));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      ortc::IMediaDevicesTypes::MediaStreamTrackListPtr trackList = promise->value<ortc::IMediaDevicesTypes::MediaStreamTrackList>();
      mMediaEngine.lock()->mLocalVideoMediaStreamTrack = *trackList->begin();
      IInspectable* localMediaElementPtr = reinterpret_cast<IInspectable*>(mMediaEngine.lock()->mLocalMediaElement);
      mMediaEngine.lock()->mLocalVideoMediaStreamTrack->setMediaElement(localMediaElementPtr);

      ortc::IICEGathererTypes::Options gathererOptions;
      ortc::IICEGathererTypes::InterfacePolicy interfacePolicy;
      interfacePolicy.mGatherPolicy = ortc::IICEGathererTypes::FilterPolicy_None;
      gathererOptions.mInterfacePolicies.push_back(interfacePolicy);
      ortc::IICEGathererTypes::Server iceServer;
//      zsLib::String url = zsLib::String("stun:stun.l.google.com:19302");
      zsLib::String url = zsLib::String("stun:stun.vline.com");
      iceServer.mURLs.push_back(url);
      gathererOptions.mICEServers.push_back(iceServer);
      mMediaEngine.lock()->mSendVideoICEGatherer = ortc::IICEGatherer::create(mMediaEngine.lock(), gathererOptions);
      mMediaEngine.lock()->mSendVideoICETransport = ortc::IICETransport::create(mMediaEngine.lock(), mMediaEngine.lock()->mSendVideoICEGatherer);

      {
        zsLib::AutoRecursiveLock lock(*mMediaEngine.lock());

        MediaEngine::CandidateMap::iterator iter = mMediaEngine.lock()->mRemoteSendVideoCandidates.begin();
        while (iter != mMediaEngine.lock()->mRemoteSendVideoCandidates.end()) {
          mMediaEngine.lock()->mSendVideoICETransport->addRemoteCandidate(*iter->second);
          iter++;
        }
        mMediaEngine.lock()->mRemoteSendVideoCandidates.clear();
      }

      mMediaEngine.lock()->mSendVideoPromiseWithCertificate = ortc::ICertificate::generateCertificate();
      mMediaEngine.lock()->mSendVideoPromiseWithCertificate->then(PromiseWithCertificateCallback::create(mMediaEngine.lock(), true));
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithMediaStreamTrackListCallback()
    {
    }

  private:
    MediaEngineWeakPtr mMediaEngine;
  };

  class PromiseWithDeviceListCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithDeviceListCallback(MediaEnginePtr mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithDeviceListCallbackPtr create(MediaEnginePtr mediaEngine)
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
      mMediaEngine.lock()->mVideoPromiseWithMediaStreamTrackList = IMediaDevices::getUserMedia(*constraints);
      mMediaEngine.lock()->mVideoPromiseWithMediaStreamTrackList->then(PromiseWithMediaStreamTrackListCallback::create(mMediaEngine.lock()));
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithDeviceListCallback()
    {
    }

  private:
    MediaEngineWeakPtr mMediaEngine;
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
  mTraceCallback(new StandupTraceCallback())
{
}

MediaEngine::~MediaEngine()
{
}

void MediaEngine::init()
{
  mSignaller = Signaller::create(mThisWeak.lock());
  
  openpeer::services::ILogger::setLogLevel(zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("zsLib", zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("openpeer_services", zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("openpeer_services_http", zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("ortclib", zsLib::Log::Insane);
  openpeer::services::ILogger::setLogLevel("ortc_standup", zsLib::Log::Insane);
  
  //openpeer::services::ILogger::installDebuggerLogger();
  openpeer::services::ILogger::installTelnetLogger(59999, 60, true);
  for (int tries = 0; tries < 60; ++tries)
  {
    if (openpeer::services::ILogger::isTelnetLoggerListening()) {
      break;
    }
    Sleep(1000);
  }

  rtc::LogMessage::LogToDebug(rtc::LS_SENSITIVE);

  webrtc::Trace::CreateTrace();
  webrtc::Trace::SetTraceCallback(mTraceCallback);
  webrtc::Trace::set_level_filter(webrtc::kTraceAll);

  ortc::ISettings::applyDefaults();
}

void MediaEngine::setLocalMediaElement(MediaElement^ element)
{
  mLocalMediaElement = element;
}

void MediaEngine::setRemoteMediaElement(MediaElement^ element)
{
  mRemoteMediaElement = element;
}

void MediaEngine::setStartStopButton(Button^ button)
{
  mStartStopButton = button;
}

void MediaEngine::makeCall()
{
  mVideoPromiseWithDeviceList = IMediaDevices::enumerateDevices();
  if (!mStarted) {
    mStartStopButton->IsEnabled = false;
    mVideoPromiseWithDeviceList->then(PromiseWithDeviceListCallback::create(mThisWeak.lock()));
  } else {
    if (mLocalVideoMediaStreamTrack)
      mLocalVideoMediaStreamTrack->stop();
    mStarted = false;
    mStartStopButton->Content = "Start";
  }
}

void MediaEngine::onICETransportStateChange(
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

void MediaEngine::onICEGathererStateChange(
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
  zsLib::AutoRecursiveLock lock(*this);

  ZS_LOG_BASIC(log("gatherer local candidate") + ortc::IICEGatherer::toDebug(gatherer) + (candidate ? candidate->toDebug() : ortc::ElementPtr()))

  if (gatherer == mSendVideoICEGatherer)
    mSignaller->sendLocalSendCandidate(candidate);
  else if (gatherer == mReceiveVideoICEGatherer)
    mSignaller->sendLocalReceiveCandidate(candidate);
}

void MediaEngine::onICEGathererLocalCandidateComplete(
                                                      ortc::IICEGathererPtr gatherer,
                                                      CandidateCompletePtr candidate
                                                      )
{
  zsLib::AutoRecursiveLock lock(*this);

  if (gatherer == mSendVideoICEGatherer)
    mSignaller->sendLocalSendCandidate(candidate);
  else if (gatherer == mReceiveVideoICEGatherer)
    mSignaller->sendLocalReceiveCandidate(candidate);
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

void MediaEngine::onDTLSTransportStateChange(
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

ortc::IRTPTypes::ParametersPtr MediaEngine::capabilitiesToSendParameters(
                                                                         ortc::IRTPTypes::CapabilitiesPtr localSendVideoCaps,
                                                                         ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                                                                         )
{
  return ortc::IRTPTypes::ParametersPtr(new ortc::IRTPTypes::Parameters());
}

ortc::IRTPTypes::ParametersPtr MediaEngine::capabilitiesToReceiveParameters(
                                                                            ortc::IRTPTypes::CapabilitiesPtr localRecvVideoCaps,
                                                                            ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps
                                                                            )
{
  return ortc::IRTPTypes::ParametersPtr(new ortc::IRTPTypes::Parameters());
}

void MediaEngine::onIncomingCall(
                                 ortc::IICETypes::ParametersPtr remoteVideoICEParameters,
                                 ortc::IDTLSTransportTypes::ParametersPtr remoteVideoDTLSParameters,
                                 ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps,
                                 ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                                 )
{
  mRemoteSendVideoICEParameters = remoteVideoICEParameters;
  mRemoteSendVideoDTLSParameters = remoteVideoDTLSParameters;
  mRemoteSendVideoCapabilities = remoteSendVideoCaps;

  ortc::IICEGathererTypes::Options gathererOptions;
  ortc::IICEGathererTypes::InterfacePolicy interfacePolicy;
  interfacePolicy.mGatherPolicy = ortc::IICEGathererTypes::FilterPolicy_None;
  gathererOptions.mInterfacePolicies.push_back(interfacePolicy);
  ortc::IICEGathererTypes::Server iceServer;
//  zsLib::String url = zsLib::String("stun:stun.l.google.com:19302");
  zsLib::String url = zsLib::String("stun:stun.vline.com");
  iceServer.mURLs.push_back(url);
  gathererOptions.mICEServers.push_back(iceServer);
  mReceiveVideoICEGatherer = ortc::IICEGatherer::create(mThisWeak.lock(), gathererOptions);
  mReceiveVideoICETransport = ortc::IICETransport::create(mThisWeak.lock(), mReceiveVideoICEGatherer);

  {
    zsLib::AutoRecursiveLock lock(*this);

    MediaEngine::CandidateMap::iterator iter = mRemoteReceiveVideoCandidates.begin();
    while (iter != mRemoteReceiveVideoCandidates.end()) {
      mReceiveVideoICETransport->addRemoteCandidate(*iter->second);
      iter++;
    }
    mRemoteReceiveVideoCandidates.clear();
  }

  mReceiveVideoPromiseWithCertificate = ortc::ICertificate::generateCertificate();
  mReceiveVideoPromiseWithCertificate->then(PromiseWithCertificateCallback::create(mThisWeak.lock(), false));
}

void MediaEngine::onCallAccepted(
                                 ortc::IICETypes::ParametersPtr remoteVideoICEParameters,
                                 ortc::IDTLSTransportTypes::ParametersPtr remoteVideoDTLSParameters,
                                 ortc::IRTPTypes::CapabilitiesPtr remoteSendVideoCaps,
                                 ortc::IRTPTypes::CapabilitiesPtr remoteRecvVideoCaps
                                 )
{
  mRemoteReceiveVideoICEParameters = remoteVideoICEParameters;
  mRemoteReceiveVideoDTLSParameters = remoteVideoDTLSParameters;
  mRemoteReceiveVideoCapabilities = remoteSendVideoCaps;

  ortc::IICETransport::Options videoICEOptions;
  videoICEOptions.mAggressiveICE = false;
  videoICEOptions.mRole = ortc::IICETypes::Role_Controlling;

  mSendVideoICETransport->start(mSendVideoICEGatherer, *mRemoteReceiveVideoICEParameters, videoICEOptions);

  mSendVideoDTLSTransport->start(*mRemoteReceiveVideoDTLSParameters);

  ortc::IRTPTypes::ParametersPtr videoSendParams = capabilitiesToSendParameters(mLocalSendVideoCapabilities, mRemoteReceiveVideoCapabilities);

  mVideoRTPSender->send(*videoSendParams);

  mStarted = true;
  mStartStopButton->Content = "Stop";
  mStartStopButton->IsEnabled = true;
}

zsLib::Log::Params MediaEngine::log(const char *message) const
{
  ortc::ElementPtr objectEl = ortc::Element::create("ortc_standup::MediaEngine");
  return zsLib::Log::Params(message, objectEl);
}
