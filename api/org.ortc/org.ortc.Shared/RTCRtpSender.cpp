
#include "pch.h"

#include "MediaStreamTrack.h"
#include "RTCDtlsTransport.h"
#include "RTCRtpSender.h"
#include "RtpTypes.h"
#include "helpers.h"
#include "Error.h"

#include <zsLib/SafeInt.h>

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

#pragma region RTCRtpSender delegates 

      class RTCRtpSenderDelegate : public IRTPSenderDelegate
      {
      public:
        RTCRtpSenderDelegate(RTCRtpSender^ owner) { _owner = owner; }

        virtual void onRTPSenderSSRCConflict(
          IRTPSenderPtr sender,
          SSRCType ssrc
          ) override
        {
          auto evt = ref new RTCSsrcConflictEvent();
          evt->_ssrc = SafeInt<decltype(evt->_ssrc)>(ssrc);
          _owner->OnSsrcConflict(evt);
        }

      private:
        RTCRtpSender^ _owner;

      };

#pragma endregion

#pragma region RTCRtpSender observers 

      class RTCRtpSenderPromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        RTCRtpSenderPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce) {}

        virtual void onPromiseResolved(PromisePtr promise) override
        {
          mTce.set();
        }

        virtual void onPromiseRejected(PromisePtr promise) override
        {
          auto reason = promise->reason<Any>();
          auto error = Error::CreateIfGeneric(reason);
          mTce.set_exception(error);
        }

      private:
        Concurrency::task_completion_event<void> mTce;
      };

#pragma endregion

    }

#pragma region RTCRtpSender 

    RTCRtpSender::RTCRtpSender(IRTPSenderPtr nativeSender) :
      _nativeDelegatePointer(make_shared<internal::RTCRtpSenderDelegate>(this)),
      _nativePointer(nativeSender)
    {
      if (_nativePointer)
      {
        _nativeSubscriptionPointer = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(make_shared<internal::RTCRtpSenderDelegate>(this))
    {
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);

      try
      {
        _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport);
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport) :
      _nativeDelegatePointer(make_shared<internal::RTCRtpSenderDelegate>(this))
    {
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);

      try
      {
        _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport, nativeRtcpTransport);
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCRtpSender::SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport)
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
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    IAsyncAction^ RTCRtpSender::SetTrack(MediaStreamTrack^ track)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(nullptr == _nativePointer)

      auto nativeTrack = MediaStreamTrack::Convert(track);

      PromisePtr promise;
      
      try
      {
        promise = _nativePointer->setTrack(nativeTrack);
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }

      IAsyncAction^ ret = Concurrency::create_async([this, track, promise]()
      {
        Concurrency::task_completion_event<void> tce;

        auto observer = make_shared<internal::RTCRtpSenderPromiseObserver>(tce);

        promise->then(observer);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

    RTCRtpCapabilities^ RTCRtpSender::GetCapabilities()
    {
      return internal::ToCx(IRTPSender::getCapabilities());
    }

    RTCRtpCapabilities^ RTCRtpSender::GetCapabilities(Platform::String^ kind)
    {
      if (kind != nullptr)
      {
        if (Platform::String::CompareOrdinal(kind, MediaStreamTrackKind::Audio.ToString()) == 0)
          return internal::ToCx(IRTPSender::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Audio));
        if (Platform::String::CompareOrdinal(kind, MediaStreamTrackKind::Video.ToString()) == 0)
          return internal::ToCx(IRTPSender::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Video));
      }

      return internal::ToCx(IRTPSender::getCapabilities());
    }
    
    IAsyncAction^ RTCRtpSender::Send(RTCRtpParameters^ parameters)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!parameters)

      PromisePtr promise;

      try
      {
        promise = _nativePointer->send(*internal::FromCx(parameters));
      }
      catch (const InvalidParameters &)
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

        auto observer = make_shared<internal::RTCRtpSenderPromiseObserver>(tce);

        promise->then(observer);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

    void RTCRtpSender::Stop()
    {
      if (!_nativePointer) return;
      _nativePointer->stop();
    }

    MediaStreamTrack^ RTCRtpSender::Track::get()
    {
      if (!_nativePointer) return nullptr;
      return MediaStreamTrack::Convert(_nativePointer->track());
    }

    RTCDtlsTransport^ RTCRtpSender::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->transport()));
    }

    RTCDtlsTransport^ RTCRtpSender::RtcpTransport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->rtcpTransport()));
    }

#pragma endregion

  } // namespacr ortc
} // namespace org
