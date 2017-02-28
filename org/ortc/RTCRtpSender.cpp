
#include "pch.h"

#include <org/ortc/MediaStreamTrack.h>
#include <org/ortc/RTCDtlsTransport.h>
#include <org/ortc/RTCIceTransport.h>
#include <org/ortc/RTCSrtpSdesTransport.h>
#include <org/ortc/RTCRtpSender.h>
#include <org/ortc/RtpTypes.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

#include <zsLib/SafeInt.h>

using namespace ortc;

namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper)

    namespace Internal
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
      _nativeDelegatePointer(make_shared<Internal::RTCRtpSenderDelegate>(this)),
      _nativePointer(nativeSender)
    {
      if (_nativePointer)
      {
        _nativeSubscriptionPointer = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(make_shared<Internal::RTCRtpSenderDelegate>(this))
    {
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);

      try
      {
        _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport);
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

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCSrtpSdesTransport^ transport)
    {
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCSrtpSdesTransport::Convert(transport);

      try
      {
        _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport);
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

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport) :
      _nativeDelegatePointer(make_shared<Internal::RTCRtpSenderDelegate>(this))
    {
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);

      try
      {
        _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport, nativeRtcpTransport);
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

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCSrtpSdesTransport^ transport, RTCIceTransport^ rtcpTransport)
    {
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCSrtpSdesTransport::Convert(transport);
      auto nativeRtcpTransport = RTCIceTransport::Convert(rtcpTransport);

      try
      {
        _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport, nativeRtcpTransport);
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

    void RTCRtpSender::SetTransport(RTCDtlsTransport^ transport)
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      try
      {
        _nativePointer->setTransport(nativeTransport);
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

    void RTCRtpSender::SetTransport(RTCSrtpSdesTransport^ transport)
    {
      auto nativeTransport = RTCSrtpSdesTransport::Convert(transport);
      try
      {
        _nativePointer->setTransport(nativeTransport);
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

    void RTCRtpSender::SetTransport(RTCSrtpSdesTransport^ transport, RTCIceTransport^ rtcpTransport)
    {
      auto nativeTransport = RTCSrtpSdesTransport::Convert(transport);
      auto nativeRtcpTransport = RTCIceTransport::Convert(rtcpTransport);
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

      IAsyncAction^ ret = Concurrency::create_async([promise]()
      {
        Concurrency::task_completion_event<void> tce;

        auto observer = make_shared<Internal::RTCRtpSenderPromiseObserver>(tce);

        promise->then(observer);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

    RTCRtpCapabilities^ RTCRtpSender::GetCapabilities(Platform::String^ kind)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(kind == nullptr)

      String kindStr = UseHelper::FromCx(kind);

      if (0 == kindStr.compareNoCase(UseHelper::FromCx(MediaStreamTrackKind::Audio.ToString())))
        return Internal::ToCx(IRTPSender::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Audio));
      if (0 == kindStr.compareNoCase(UseHelper::FromCx(MediaStreamTrackKind::Video.ToString())))
        return Internal::ToCx(IRTPSender::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Video));

      ORG_ORTC_THROW_INVALID_PARAMETERS()
    }
    
    IAsyncAction^ RTCRtpSender::Send(RTCRtpParameters^ parameters)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!parameters)

      PromisePtr promise;

      try
      {
        promise = _nativePointer->send(*Internal::FromCx(parameters));
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }

      IAsyncAction^ ret = Concurrency::create_async([promise]()
      {
        Concurrency::task_completion_event<void> tce;

        auto observer = make_shared<Internal::RTCRtpSenderPromiseObserver>(tce);

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

    Platform::Object^ RTCRtpSender::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      auto nativeTransport = _nativePointer->transport();
      if (!nativeTransport) return nullptr;
      {
        auto dtlsTransport = IDTLSTransport::convert(nativeTransport);
        if (nullptr != dtlsTransport) {
          return RTCDtlsTransport::Convert(dtlsTransport);
        }
      }
      {
        auto srtpTransport = ISRTPSDESTransport::convert(nativeTransport);
        if (nullptr != srtpTransport) {
          return RTCSrtpSdesTransport::Convert(srtpTransport);
        }
      }
      return nullptr;
    }

    Platform::Object^ RTCRtpSender::RtcpTransport::get()
    {
      if (!_nativePointer) return nullptr;
      auto nativeTransport = _nativePointer->rtcpTransport();
      if (!nativeTransport) return nullptr;
      {
        auto dtlsTransport = IDTLSTransport::convert(nativeTransport);
        if (nullptr != dtlsTransport) {
          return RTCDtlsTransport::Convert(dtlsTransport);
        }
      }
      {
        auto iceTransport = IICETransport::convert(nativeTransport);
        if (nullptr != iceTransport) {
          return RTCIceTransport::Convert(iceTransport);
        }
      }
      return nullptr;
    }

#pragma endregion

  } // namespacr ortc
} // namespace org
