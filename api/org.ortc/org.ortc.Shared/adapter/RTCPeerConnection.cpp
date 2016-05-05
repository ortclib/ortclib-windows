#include "pch.h"

#include "RTCPeerConnection.h"
#include "RTCSessionDescription.h"
#include "MediaStream.h"
#include "../helpers.h"
#include "../Error.h"
#include "../RTCIceGatherer.h"
#include "../RTCCertificate.h"
#include "../RtpTypes.h"
#include "../RTCRtpSender.h"
#include "../RTCRtpReceiver.h"
#include "../MediaStreamTrack.h"
#include "../RTCDataChannel.h"

#include <ortc/internal/ortc_Helper.h>
#include <zsLib/SafeInt.h>

using namespace ortc;
using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IPeerConnectionTypes, IPeerConnectionTypes);

    namespace adapter
    {
      ZS_DECLARE_TYPEDEF_PTR(org::ortc::internal::Helper, UseHelper);
      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IPeerConnectionDelegate, IPeerConnectionDelegate);

      using std::make_shared;

      namespace internal
      {
        class RTCPeerConnectionHelper
        {
        public:
          static RTCCertificate^ Convert(ICertificatePtr certificate) { return RTCCertificate::Convert(certificate); }
          static ICertificatePtr Convert(RTCCertificate^ certificate) { return RTCCertificate::Convert(certificate); }
        };

#pragma region RTCPeerConnection types converters

        RTCConfiguration^ ToCx(const IPeerConnectionTypes::Configuration &input)
        {
          auto result = ref new RTCConfiguration;
          result->GatherOptions = ortc::internal::ToCx(input.mGatherOptions);
          result->SignalingType = UseHelper::Convert(input.mSignalingType);
          result->BundlePolicy = UseHelper::Convert(input.mBundlePolicy);
          result->RtcpMuxPolicy = UseHelper::Convert(input.mRTCPMuxPolicy);
          result->Certificates = ref new Vector<RTCCertificate^>();

          for (auto iter = input.mCertificates.begin(); iter != input.mCertificates.end(); ++iter)
          {
            auto cert = (*iter);
            result->Certificates->Append(RTCPeerConnectionHelper::Convert(cert));
          }

          return result;
        }

        RTCConfiguration^ ToCx(IPeerConnectionTypes::ConfigurationPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }

        IPeerConnectionTypes::ConfigurationPtr FromCx(RTCConfiguration^ input)
        {
          if (nullptr == input) return IPeerConnectionTypes::ConfigurationPtr();
          auto result(make_shared<IPeerConnectionTypes::Configuration>());
          result->mGatherOptions = ortc::internal::FromCx(input->GatherOptions);
          result->mSignalingType = UseHelper::Convert(input->SignalingType);
          result->mBundlePolicy = UseHelper::Convert(input->BundlePolicy);
          result->mRTCPMuxPolicy = UseHelper::Convert(input->RtcpMuxPolicy);

          for (RTCCertificate^ value : input->Certificates)
          {
            if (nullptr == value) continue;
            result->mCertificates.push_back(RTCPeerConnectionHelper::Convert(value));
          }
          return result;
        }

        RTCOfferOptions^ ToCx(const IPeerConnectionTypes::OfferOptions &input)
        {
          auto result = ref new RTCOfferOptions;
          result->VoiceActivityDetection = input.mVoiceActivityDetection;
          result->IceRestart = input.mICERestart;
          return result;
        }
        RTCOfferOptions^ ToCx(IPeerConnectionTypes::OfferOptionsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        IPeerConnectionTypes::OfferOptionsPtr FromCx(RTCOfferOptions^ input)
        {
          if (nullptr == input) return IPeerConnectionTypes::OfferOptionsPtr();
          auto result(make_shared<IPeerConnectionTypes::OfferOptions>());
          result->mVoiceActivityDetection = input->VoiceActivityDetection;
          result->mICERestart = input->IceRestart;
          return result;
        }

        RTCAnswerOptions^ ToCx(const IPeerConnectionTypes::AnswerOptions &input)
        {
          auto result = ref new RTCAnswerOptions;
          result->VoiceActivityDetection = input.mVoiceActivityDetection;
          return result;
        }
        RTCAnswerOptions^ ToCx(IPeerConnectionTypes::AnswerOptionsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        IPeerConnectionTypes::AnswerOptionsPtr FromCx(RTCAnswerOptions^ input)
        {
          if (nullptr == input) return IPeerConnectionTypes::AnswerOptionsPtr();
          auto result(make_shared<IPeerConnectionTypes::AnswerOptions>());
          result->mVoiceActivityDetection = input->VoiceActivityDetection;
          return result;
        }

        RTCCapabilityOptions^ ToCx(const IPeerConnectionTypes::CapabilityOptions &input)
        {
          auto result = ref new RTCCapabilityOptions;
          result->VoiceActivityDetection = input.mVoiceActivityDetection;
          return result;
        }
        RTCCapabilityOptions^ ToCx(IPeerConnectionTypes::CapabilityOptionsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        IPeerConnectionTypes::CapabilityOptionsPtr FromCx(RTCCapabilityOptions^ input)
        {
          if (nullptr == input) return IPeerConnectionTypes::CapabilityOptionsPtr();
          auto result(make_shared<IPeerConnectionTypes::CapabilityOptions>());
          result->mVoiceActivityDetection = input->VoiceActivityDetection;
          return result;
        }

        RTCMediaStreamTrackConfiguration^ ToCx(const IPeerConnectionTypes::MediaStreamTrackConfiguration &input)
        {
          auto result = ref new RTCMediaStreamTrackConfiguration;
          result->Parameters = ortc::internal::ToCx(input.mParameters);
          return result;
        }
        RTCMediaStreamTrackConfiguration^ ToCx(IPeerConnectionTypes::MediaStreamTrackConfigurationPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        IPeerConnectionTypes::MediaStreamTrackConfigurationPtr FromCx(RTCMediaStreamTrackConfiguration^ input)
        {
          if (nullptr == input) return IPeerConnectionTypes::MediaStreamTrackConfigurationPtr();
          auto result(make_shared<IPeerConnectionTypes::MediaStreamTrackConfiguration>());
          result->mParameters = ortc::internal::FromCx(input->Parameters);
          return result;
        }

#pragma endregion

#pragma region RTCPeerConnection delegates

        class RTCPeerConnectionDelegate : public IPeerConnectionDelegate
        {
        public:
          RTCPeerConnectionDelegate(RTCPeerConnection^ owner) { _owner = owner; }

          virtual void onPeerConnectionNegotiationNeeded(IPeerConnectionPtr connection) override
          {
            if (!_owner) return;
            _owner->OnNegotiationNeeded();
          }

          virtual void onPeerConnectionIceCandidate(
            IPeerConnectionPtr connection,
            ICECandidatePtr candidate,
            const char *url
            ) override
          {
            if (!_owner) return;

            if (!candidate)
            {
              auto evt = ref new RTCPeerConnectionIceEvent;
              evt->_url = UseHelper::ToCx(url);
              _owner->OnIceCandidate(evt);
              return;
            }

            {
              auto convertedCandidate = ZS_DYNAMIC_PTR_CAST(IICETypes::Candidate, candidate->mCandidate);
              if (convertedCandidate)
              {
                auto evt = ref new RTCPeerConnectionIceEvent;
                evt->_url = UseHelper::ToCx(url);
                evt->_candidate = ToIceCandidateCx(candidate);
                _owner->OnIceCandidate(evt);
              }
            }
            {
              auto convertedCandidate = ZS_DYNAMIC_PTR_CAST(IICETypes::CandidateComplete, candidate->mCandidate);
              if (convertedCandidate)
              {
                auto evt = ref new RTCPeerConnectionIceCompleteEvent;
                evt->_candidate = ToIceCandidateCompleteCx(candidate);
                _owner->OnIceCandidateComplete(evt);
              }
            }
          }
          virtual void onPeerConnectionIceCandidateError(
            IPeerConnectionPtr connection,
            ICECandidateErrorEventPtr error
            ) override
          {
            if (!_owner) return;
            auto evt = ref new RTCPeerConnectionIceErrorEvent;
            if (!error)
            {
              _owner->OnIceCandidateError(evt);
              return;
            }
            evt->_hostCandidate = ToIceCandidateCx(error->mHostCandidate);
            evt->_url = UseHelper::ToCx(error->mURL);
            evt->_errorCode = UseHelper::ToCx(error->mErrorCode);
            evt->_errorText = UseHelper::ToCx(error->mErrorText);
            _owner->OnIceCandidateError(evt);
          }
          virtual void onPeerConnectionSignalingStateChange(
            IPeerConnectionPtr connection,
            SignalingStates state
            ) override
          {
            if (!_owner) return;
            _owner->OnSignalingStateChange();
          }
          virtual void onPeerConnectionIceGatheringStateChange(
            IPeerConnectionPtr connection,
            ICEGatheringStates state
            )
          {
            if (!_owner) return;
            _owner->OnIceGatheringStateChange();
          }
          virtual void onPeerConnectionIceConnectionStateChange(
            IPeerConnectionPtr connection,
            ICEConnectionStates state
            )
          {
            if (!_owner) return;
            _owner->OnIceConnectionStateChange();
          }
          virtual void onPeerConnectionConnectionStateChange(
            IPeerConnectionPtr connection,
            PeerConnectionStates state
            )
          {
            if (!_owner) return;
            _owner->OnConnectionStateChange();
          }
          virtual void onPeerConnectionTrack(
            IPeerConnectionPtr connection,
            MediaStreamTrackEventPtr event
            )
          {
            if (!_owner) return;
            auto evt = ref new RTCTrackEvent;
            if (!event)
            {
              _owner->OnTrack(evt);
              return;
            }

            evt->_receiver = RTCRtpReceiver::Convert(event->mReceiver);
            evt->_track = MediaStreamTrack::Convert(event->mTrack);

            evt->_streams = ref new Vector<MediaStream^>();

            for (auto iter = event->mMediaStreams.begin(); iter != event->mMediaStreams.end(); ++iter)
            {
              auto nativeStream = (*iter);
              evt->_streams->Append(MediaStream::Convert(nativeStream));
            }

            _owner->OnTrack(evt);
          }

          virtual void onPeerConnectionDataChannel(
            IPeerConnectionPtr connection,
            IDataChannelPtr dataChannel
            )
          {
            if (!_owner) return;
            _owner->OnDataChannel(RTCDataChannel::Convert(dataChannel));
          }

        private:
          RTCPeerConnection^ _owner;
        };

#pragma endregion

#pragma region RTCPeerConnection observers

        class RTCPeerConnectionPromiseWithDescriptionObserver : public zsLib::IPromiseResolutionDelegate
        {
        public:
          RTCPeerConnectionPromiseWithDescriptionObserver(Concurrency::task_completion_event<RTCSessionDescription^> tce) : mTce(tce) {}

          virtual void onPromiseResolved(PromisePtr promise) override
          {
            ISessionDescriptionPtr nativeSessionDescription = promise->value<ISessionDescription>();

            auto result = RTCSessionDescription::Convert(nativeSessionDescription);
            mTce.set(result);
          }

          virtual void onPromiseRejected(PromisePtr promise) override
          {
            auto reason = promise->reason<ErrorAny>();
            auto error = Error::CreateIfGeneric(reason);
            mTce.set_exception(error);
          }

        private:
          Concurrency::task_completion_event<RTCSessionDescription^> mTce;
        };

        class RTCPeerConnectionPromiseObserver : public zsLib::IPromiseResolutionDelegate
        {
        public:
          RTCPeerConnectionPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce) {}

          virtual void onPromiseResolved(PromisePtr promise) override
          {
            ISessionDescriptionPtr nativeSessionDescription = promise->value<ISessionDescription>();
            mTce.set();
          }

          virtual void onPromiseRejected(PromisePtr promise) override
          {
            auto reason = promise->reason<ErrorAny>();
            auto error = Error::CreateIfGeneric(reason);
            mTce.set_exception(error);
          }

        private:
          Concurrency::task_completion_event<void> mTce;
        };

#pragma endregion

      }

#pragma region RTCPeerConnection

      RTCPeerConnection::RTCPeerConnection(IPeerConnectionPtr nativeSession) :
        _nativePointer(nativeSession),
        _nativeDelegatePointer(make_shared<internal::RTCPeerConnectionDelegate>(this))
      {
        if (_nativePointer)
        {
          _nativeDelegateSubscription = _nativePointer->subscribe(_nativeDelegatePointer);
        }
      }

      RTCPeerConnection::RTCPeerConnection() :
        _nativeDelegatePointer(make_shared<internal::RTCPeerConnectionDelegate>(this))
      {
        _nativePointer = IPeerConnection::create(_nativeDelegatePointer);
      }

      RTCPeerConnection::RTCPeerConnection(RTCConfiguration^ configuration) :
        _nativeDelegatePointer(make_shared<internal::RTCPeerConnectionDelegate>(this))
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!configuration);
        auto nativeConfiguration = internal::FromCx(configuration);
        _nativePointer = IPeerConnection::create(_nativeDelegatePointer, *nativeConfiguration);
      }

      IAsyncOperation<RTCSessionDescription^>^ RTCPeerConnection::CreateOffer()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        auto promise = _nativePointer->createOffer();

        IAsyncOperation<RTCSessionDescription^>^ ret = Concurrency::create_async([promise]() -> RTCSessionDescription^
        {
          Concurrency::task_completion_event<RTCSessionDescription^> tce;

          auto pDelegate(make_shared<internal::RTCPeerConnectionPromiseWithDescriptionObserver>(tce));

          promise->then(pDelegate);
          promise->background();

          auto tceTask = Concurrency::task<RTCSessionDescription^>(tce);
          return tceTask.get();
        });

        return ret;
      }

      IAsyncOperation<RTCSessionDescription^>^ RTCPeerConnection::CreateOffer(RTCOfferOptions^ options)
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!options);

        auto promise = _nativePointer->createOffer(*internal::FromCx(options));

        IAsyncOperation<RTCSessionDescription^>^ ret = Concurrency::create_async([promise]() -> RTCSessionDescription^
        {
          Concurrency::task_completion_event<RTCSessionDescription^> tce;

          auto pDelegate(make_shared<internal::RTCPeerConnectionPromiseWithDescriptionObserver>(tce));

          promise->then(pDelegate);
          promise->background();

          auto tceTask = Concurrency::task<RTCSessionDescription^>(tce);
          return tceTask.get();
        });

        return ret;
      }

      IAsyncOperation<RTCSessionDescription^>^ RTCPeerConnection::CreateCapabilities()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        auto nativePointer = _nativePointer;

        auto promise = nativePointer->createCapabilities();

        IAsyncOperation<RTCSessionDescription^>^ ret = Concurrency::create_async([promise]() -> RTCSessionDescription^
        {
          Concurrency::task_completion_event<RTCSessionDescription^> tce;

          auto pDelegate(make_shared<internal::RTCPeerConnectionPromiseWithDescriptionObserver>(tce));

          promise->then(pDelegate);
          promise->background();

          auto tceTask = Concurrency::task<RTCSessionDescription^>(tce);
          return tceTask.get();
        });

        return ret;
      }

      IAsyncOperation<RTCSessionDescription^>^ RTCPeerConnection::CreateCapabilities(RTCCapabilityOptions^ options)
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!options);

        auto nativePointer = _nativePointer;
        auto nativeOptions = internal::FromCx(options);

        IAsyncOperation<RTCSessionDescription^>^ ret = Concurrency::create_async([nativePointer, nativeOptions]() -> RTCSessionDescription^
        {
          Concurrency::task_completion_event<RTCSessionDescription^> tce;

          IPeerConnectionTypes::PromiseWithDescriptionPtr promise = nativePointer->createCapabilities(*nativeOptions);
          auto pDelegate(make_shared<internal::RTCPeerConnectionPromiseWithDescriptionObserver>(tce));

          promise->then(pDelegate);
          promise->background();

          auto tceTask = Concurrency::task<RTCSessionDescription^>(tce);
          return tceTask.get();
        });

        return ret;
      }

      IAsyncAction^ RTCPeerConnection::SetLocalDescription(RTCSessionDescription^ description)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!description);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        auto promise = _nativePointer->setLocalDescription(RTCSessionDescription::Convert(description));

        IAsyncAction^ ret = Concurrency::create_async([promise]()
        {
          Concurrency::task_completion_event<void> tce;

          auto pDelegate(make_shared<internal::RTCPeerConnectionPromiseObserver>(tce));

          promise->then(pDelegate);
          promise->background();
          auto tceTask = Concurrency::task<void>(tce);

          return tceTask.get();
        });

        return ret;
      }

      RTCSessionDescription^ RTCPeerConnection::LocalDescription::get()
      {
        if (!_nativePointer) return nullptr;
        return RTCSessionDescription::Convert(_nativePointer->localDescription());
      }

      RTCSessionDescription^ RTCPeerConnection::CurrentDescription::get()
      {
        if (!_nativePointer) return nullptr;
        return RTCSessionDescription::Convert(_nativePointer->currentDescription());
      }

      RTCSessionDescription^ RTCPeerConnection::PendingDescription::get()
      {
        if (!_nativePointer) return nullptr;
        return RTCSessionDescription::Convert(_nativePointer->pendingDescription());
      }

      IAsyncAction^ RTCPeerConnection::SetRemoteDescription(RTCSessionDescription^ description)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!description);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        auto promise = _nativePointer->setRemoteDescription(RTCSessionDescription::Convert(description));

        IAsyncAction^ ret = Concurrency::create_async([promise]()
        {
          Concurrency::task_completion_event<void> tce;

          auto pDelegate(make_shared<internal::RTCPeerConnectionPromiseObserver>(tce));

          promise->then(pDelegate);
          promise->background();
          auto tceTask = Concurrency::task<void>(tce);

          return tceTask.get();
        });

        return ret;
      }

      RTCSessionDescription^ RTCPeerConnection::RemoteDescription::get()
      {
        if (!_nativePointer) return nullptr;
        return RTCSessionDescription::Convert(_nativePointer->remoteDescription());
      }

      RTCSessionDescription^ RTCPeerConnection::CurrentRemoteDescription::get()
      {
        if (!_nativePointer) return nullptr;
        return RTCSessionDescription::Convert(_nativePointer->currentRemoteDescription());
      }

      RTCSessionDescription^ RTCPeerConnection::PendingRemoteDescription::get()
      {
        if (!_nativePointer) return nullptr;
        return RTCSessionDescription::Convert(_nativePointer->pendingRemoteDescription());
      }

      void RTCPeerConnection::AddIceCandidate(RTCIceCandidate^ candidate)
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        if (!candidate)
        {
          AddIceCandidateComplete(ref new RTCIceCandidateComplete);
          return;
        }

        _nativePointer->addIceCandidate(*internal::FromCx(candidate));
      }

      void RTCPeerConnection::AddIceCandidateComplete(RTCIceCandidateComplete^ candidate)
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        _nativePointer->addIceCandidate(*internal::FromCx(candidate));
      }

      RTCSignalingState RTCPeerConnection::SignalingState::get()
      {
        if (!_nativePointer) return RTCSignalingState::Closed;
        return UseHelper::Convert(_nativePointer->signalingState());
      }

      RTCIceGatheringState RTCPeerConnection::IceGatheringState::get()
      {
        if (!_nativePointer) return RTCIceGatheringState::Complete;
        return UseHelper::ConvertToGatheringState(_nativePointer->iceGatheringState());
      }

      RTCIceConnectionState RTCPeerConnection::IceConnectionState::get()
      {
        if (!_nativePointer) return RTCIceConnectionState::Closed;
        return UseHelper::ConvertToConnectionState(_nativePointer->iceConnectionState());
      }

      RTCPeerConnectionState RTCPeerConnection::ConnectionState::get()
      {
        if (!_nativePointer) return RTCPeerConnectionState::Disconnected;
        return UseHelper::Convert(_nativePointer->connectionState());
      }

      Platform::Boolean RTCPeerConnection::CanTrickleIceCandidates::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return _nativePointer->canTrickleCandidates();
      }

      IVector<RTCIceServer^>^ RTCPeerConnection::GetDefaultIceServers()
      {
        auto result = ref new Vector<RTCIceServer^>();

        auto nativeResult = IPeerConnection::getDefaultIceServers();

        for (auto iter = nativeResult->begin(); iter != nativeResult->end(); ++iter)
        {
          auto server = (*iter);
          result->Append(ortc::internal::ToCx(server));
        }

        return result;
      }

      RTCConfiguration^ RTCPeerConnection::GetConfiguration()
      {
        if (!_nativePointer) return nullptr;
        return internal::ToCx(_nativePointer->getConfiguration());
      }

      void RTCPeerConnection::SetConfiguration(RTCConfiguration^ configuration)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!configuration);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        _nativePointer->setConfiguration(*internal::FromCx(configuration));
      }

      void RTCPeerConnection::Close()
      {
        if (!_nativePointer) return;
        _nativePointer->close();
      }

      IVector<RTCRtpSender^>^ RTCPeerConnection::GetSenders()
      {
        auto result = ref new Vector<RTCRtpSender^>();
        if (!_nativePointer) return result;

        auto nativeResult = _nativePointer->getSenders();

        for (auto iter = nativeResult->begin(); iter != nativeResult->end(); ++iter)
        {
          auto server = (*iter);
          result->Append(RTCRtpSender::Convert(server));
        }

        return result;
      }

      IVector<RTCRtpReceiver^>^ RTCPeerConnection::GetReceivers()
      {
        auto result = ref new Vector<RTCRtpReceiver^>();
        if (!_nativePointer) return result;

        auto nativeResult = _nativePointer->getReceivers();

        for (auto iter = nativeResult->begin(); iter != nativeResult->end(); ++iter)
        {
          auto server = (*iter);
          result->Append(RTCRtpReceiver::Convert(server));
        }

        return result;
      }

      RTCRtpSender^ RTCPeerConnection::AddTrack(MediaStreamTrack^ track)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!track);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return RTCRtpSender::Convert(_nativePointer->addTrack(MediaStreamTrack::Convert(track)));
      }

      RTCRtpSender^ RTCPeerConnection::AddTrack(
        MediaStreamTrack^ track,
        IVector<MediaStream^>^ streams
        )
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!track);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!streams);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        IPeerConnectionTypes::MediaStreamList nativeStreams;

        for (MediaStream^ stream : streams)
        {
          ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!stream);
          nativeStreams.push_back(MediaStream::Convert(stream));
        }

        return RTCRtpSender::Convert(_nativePointer->addTrack(MediaStreamTrack::Convert(track), nativeStreams));
      }

      RTCRtpSender^ RTCPeerConnection::AddTrack(
        MediaStreamTrack^ track,
        RTCMediaStreamTrackConfiguration^ configuration
        )
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!track);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!configuration);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return RTCRtpSender::Convert(_nativePointer->addTrack(MediaStreamTrack::Convert(track), *internal::FromCx(configuration)));
      }

      RTCRtpSender^ RTCPeerConnection::AddTrack(
        MediaStreamTrack^ track,
        IVector<MediaStream^>^ streams,
        RTCMediaStreamTrackConfiguration^ configuration
        )
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!track);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!configuration);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        IPeerConnectionTypes::MediaStreamList nativeStreams;

        for (MediaStream^ stream : streams)
        {
          ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!stream);
          nativeStreams.push_back(MediaStream::Convert(stream));
        }

        return RTCRtpSender::Convert(_nativePointer->addTrack(MediaStreamTrack::Convert(track), nativeStreams, *internal::FromCx(configuration)));
      }

      void RTCPeerConnection::RemoveTrack(RTCRtpSender^ track)
      {
        if (!track) return;
        if (!_nativePointer) return;
        _nativePointer->removeTrack(RTCRtpSender::Convert(track));
      }

      RTCDataChannel^ RTCPeerConnection::CreateDataChannel(RTCDataChannelParameters^ parameters)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!parameters);
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

        return RTCDataChannel::Convert(_nativePointer->createDataChannel(*ortc::internal::FromCx(parameters)));
      }

#pragma endregion

    } // namespace adapter
  } // namespace ortc
} // namespace org
