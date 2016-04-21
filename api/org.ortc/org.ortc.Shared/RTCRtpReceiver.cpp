#include "pch.h"

#include "RTCRtpReceiver.h"
#include "RTCDtlsTransport.h"
#include "RtpTypes.h"
#include "helpers.h"
#include "Error.h"

using namespace ortc;

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
#pragma region RTCRtpReceiver delegates

      class RTCRtpReceiverDelegate : public IRTPReceiverDelegate
      {
      public:
        void SetOwnerObject(RTCRtpReceiver^ owner) { _owner = owner; }

      private:
        RTCRtpReceiver^ _owner;
      };

#pragma endregion

#pragma region RTCRtpReceiver observers 

      class RTCRtpReceiverPromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        RTCRtpReceiverPromiseObserver(Concurrency::task_completion_event<void> tce);

        virtual void onPromiseResolved(PromisePtr promise) override;
        virtual void onPromiseRejected(PromisePtr promise) override;

      private:
        Concurrency::task_completion_event<void> mTce;
      };

      RTCRtpReceiverPromiseObserver::RTCRtpReceiverPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce)
      {
      }

      void RTCRtpReceiverPromiseObserver::onPromiseResolved(PromisePtr promise)
      {
        mTce.set();
      }

      void RTCRtpReceiverPromiseObserver::onPromiseRejected(PromisePtr promise)
      {
        auto reason = promise->reason<Any>();
        auto error = Error::CreateIfGeneric(reason);
        mTce.set_exception(error);
      }

#pragma endregion

    }
    
    RTCRtpReceiver::RTCRtpReceiver() :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCRtpReceiver::RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(make_shared<internal::RTCRtpReceiverDelegate>())
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);

      try
      {
        _nativePointer = IRTPReceiver::create(_nativeDelegatePointer, UseHelper::Convert(kind), nativeTransport);
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    RTCRtpReceiver::RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport ^ transport, RTCDtlsTransport^ rtcpTransport)
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);
      _nativePointer = IRTPReceiver::create(_nativeDelegatePointer, UseHelper::Convert(kind), nativeTransport, nativeRtcpTransport);
    }

    void RTCRtpReceiver::SetTransport(RTCDtlsTransport ^ transport)
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer->setTransport(nativeTransport);
    }

    void RTCRtpReceiver::SetTransport(RTCDtlsTransport ^ transport, RTCDtlsTransport ^ rtcpTransport)
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);
      try
      {
        _nativePointer->setTransport(nativeTransport, nativeRtcpTransport);
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
    }

    RTCRtpCapabilities^ RTCRtpReceiver::GetCapabilities()
    {
      return internal::ToCx(IRTPReceiver::getCapabilities());
    }

    RTCRtpCapabilities^ RTCRtpReceiver::GetCapabilities(Platform::String^ kind)
    {
      if (nullptr != kind)
      {
        if (Platform::String::CompareOrdinal(kind, MediaStreamTrackKind::Audio.ToString()) == 0)
          return internal::ToCx(IRTPReceiver::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Audio));

        if (Platform::String::CompareOrdinal(kind, MediaStreamTrackKind::Video.ToString()) == 0)
          return internal::ToCx(IRTPReceiver::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Video));
      }

      return internal::ToCx(IRTPReceiver::getCapabilities());
    }

    IAsyncAction^ RTCRtpReceiver::Receive(RTCRtpParameters^ parameters)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!parameters)

      PromisePtr promise;

      try
      {
        promise = _nativePointer->receive(*internal::FromCx(parameters));
      }
      catch (const InvalidParameters & )
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }

      IAsyncAction^ ret = Concurrency::create_async([this, promise]()
      {
        Concurrency::task_completion_event<void> tce;

        auto observer = make_shared<internal::RTCRtpReceiverPromiseObserver>(tce);

        promise->then(observer);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

    IVector<RTCRtpContributingSource^>^ org::ortc::RTCRtpReceiver::GetContributingSource()
    {
      auto ret = ref new Vector<RTCRtpContributingSource^>();

      if (_nativePointer)
      {
        auto contributingSource = _nativePointer->getContributingSources();
        for (IRTPReceiver::ContributingSourceList::iterator it = contributingSource.begin(); it != contributingSource.end(); ++it)
        {
          RTCRtpContributingSource^ source = ref new RTCRtpContributingSource();

          source->mSource = it->mCSRC;
          source->mAudioLevel = it->mAudioLevel;
        }
      }

      return ret;
    }

    void RTCRtpReceiver::Stop()
    {
      if (_nativePointer)
        _nativePointer->stop();
    }

    MediaStreamTrack^ RTCRtpReceiver::Track::get()
    {
      if (!_nativePointer) return nullptr;
      return MediaStreamTrack::Convert(_nativePointer->track());
    }

    RTCDtlsTransport^ RTCRtpReceiver::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->transport()));
    }

    RTCDtlsTransport^ RTCRtpReceiver::RtcpTransport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->rtcpTransport()));
    }

  } // namespace ortc
} // namespace org
