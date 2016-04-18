
#include "pch.h"

#include "MediaStreamTrack.h"
#include "RTCDtlsTransport.h"
#include "RTCRtpSender.h"
#include "RtpTypes.h"
#include "helpers.h"

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)
    
    RTCRtpSender::RTCRtpSender() :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(new RTCRtpSenderDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport);
    }

    RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport) :
      _nativeDelegatePointer(new RTCRtpSenderDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTrack = MediaStreamTrack::Convert(track);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);
      _nativePointer = IRTPSender::create(_nativeDelegatePointer, nativeTrack, nativeTransport, nativeRtcpTransport);
    }

    void RTCRtpSender::SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport)
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);
      _nativePointer->setTransport(nativeTransport, nativeRtcpTransport);
    }

    IAsyncAction^ RTCRtpSender::SetTrack(MediaStreamTrack^ track)
    {
      IAsyncAction^ ret = Concurrency::create_async([this, track]()
      {
        Concurrency::task_completion_event<void> tce;

        auto nativeTrack = MediaStreamTrack::Convert(track);

        if (!_nativePointer)
        {
          tce.set();
          return;
        }

        PromisePtr promise = _nativePointer->setTrack(nativeTrack);
        RTCSenderPromiseObserverPtr pDelegate(make_shared<RTCSenderPromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

    RTCRtpCapabilities^ RTCRtpSender::GetCapabilities(Platform::String^ kind)
    {
      if (kind != nullptr)
      {
        if (Platform::String::CompareOrdinal(kind, "audio") == 0)
          return internal::ToCx(IRTPSender::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Audio));
        if (Platform::String::CompareOrdinal(kind, "video") == 0)
          return internal::ToCx(IRTPSender::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Video));
      }

      return internal::ToCx(IRTPSender::getCapabilities());
    }

    void RTCRtpSender::Send(RTCRtpParameters^ parameters)
    {
      if (_nativePointer)
      {
        assert(nullptr != parameters);
        _nativePointer->send(*internal::FromCx(parameters));
      }
    }

    void RTCRtpSender::Stop()
    {
      if (_nativePointer)
      {
        _nativePointer->stop();
      }
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


    //-----------------------------------------------------------------
    #pragma mark RTCRtpSenderDelegate
    //-----------------------------------------------------------------

    void RTCRtpSenderDelegate::onRTPSenderError(
      IRTPSenderPtr sender,
      ErrorCode errorCode,
      String errorReason
      )
    {
      auto evt = ref new RTCRtpSenderErrorEvent();
      evt->Error->ErrorCode = errorCode;
      evt->Error->ErrorReason = UseHelper::ToCx(errorReason);
      _sender->OnRTCRtpSenderError(evt);
    }

    void RTCRtpSenderDelegate::onRTPSenderSSRCConflict(
      IRTPSenderPtr sender,
      SSRCType ssrc
      )
    {
      auto evt = ref new RTCRtpSenderSSRCConflictEvent();
      evt->SSRCConflict = ssrc;
      _sender->OnRTCRtpSenderSSRCConflict(evt);
    }

    RTCSenderPromiseObserver::RTCSenderPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce)
    {
    }

    void RTCSenderPromiseObserver::onPromiseSettled(PromisePtr promise)
    {
      mTce.set();
    }

  } // namespacr ortc
} // namespace org
