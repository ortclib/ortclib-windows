#include "pch.h"
#include "RTCRtpListener.h"
#include "helpers.h"

using namespace ortc_winrt_api;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

RTCRtpListener::RTCRtpListener(RTCDtlsTransport^ transport, IVector<RTCRtpHeaderExtensionParameters^>^ headerExtensions) :
mNativeDelegatePointer(new RTCRtpListenerDelegate())
{
  if (!transport)
  {
    return;
  }

  if (FetchNativePointer::FromDtlsTransport(transport))
  {
    IRTPListener::HeaderExtensionParametersList list;
    if (headerExtensions)
    {
      for (RTCRtpHeaderExtensionParameters^ ext : headerExtensions)
      {
        list.push_back(FromCx(ext));
      }
    }
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IRTPListener::create(mNativeDelegatePointer, FetchNativePointer::FromDtlsTransport(transport), list);
  }
}

RTCDtlsTransport^ RTCRtpListener::GetDtlsTransport()
{
  return ConvertObjectToCx::ToDtlsTransport(IDTLSTransport::convert(mNativePointer->transport()));
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
  evt->UnhandledRtp->MuxId = ToCx(zsLib::String(mid));
  evt->UnhandledRtp->Rid = ToCx(zsLib::String(rid));
  _listener->OnRTCRtpListenerUnhandledRtp(evt);
}