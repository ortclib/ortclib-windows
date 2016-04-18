#include "pch.h"

#include "RTCDtlsTransport.h"
#include "RTCRtpListener.h"
#include "RtpTypes.h"
#include "helpers.h"


namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    RTCRtpListener::RTCRtpListener(RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(new RTCRtpListenerDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer = IRTPListener::create(_nativeDelegatePointer, nativeTransport);
    }

    RTCRtpListener::RTCRtpListener(RTCDtlsTransport^ transport, IVector<RTCRtpHeaderExtensionParameters^>^ headerExtensions) :
      _nativeDelegatePointer(new RTCRtpListenerDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);

      IRTPListener::HeaderExtensionParametersList list;
      if (headerExtensions)
      {
        for (RTCRtpHeaderExtensionParameters^ ext : headerExtensions)
        {
          if (nullptr == ext) continue;
          list.push_back(*internal::FromCx(ext));
        }
      }
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer = IRTPListener::create(_nativeDelegatePointer, nativeTransport, list);
    }

    RTCDtlsTransport^ RTCRtpListener::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->transport()));
    }

    void RTCRtpListenerDelegate::onRTPListenerUnhandledRTP(
      IRTPListenerPtr listener,
      SSRCType ssrc,
      PayloadType payloadType,
      const char *mid,
      const char *rid
      )
    {
      auto evt = ref new RTCRtpRListenerUnhandledRtpEvent();
      evt->UnhandledRtp->Ssrc = ssrc;
      evt->UnhandledRtp->PayloadType = payloadType;
      evt->UnhandledRtp->MuxId = UseHelper::ToCx(mid);
      evt->UnhandledRtp->Rid = UseHelper::ToCx(rid);
      _listener->OnRTCRtpListenerUnhandledRtp(evt);
    }

  } // namespace ortc
} // namespace org
