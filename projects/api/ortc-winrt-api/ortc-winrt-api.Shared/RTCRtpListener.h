#pragma once
#include <ortc/IRTPListener.h>
#include <collection.h>
#include "RTCDtlsTransport.h"

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_CLASS_PTR(RTCRtpListenerDelegate)

    ref class RTCRtpListener;

    class RTCRtpListenerDelegate : public IRTPListenerDelegate
    {
    public:
      virtual void onRTPListenerUnhandledRTP(
        IRTPListenerPtr listener,
        SSRCType ssrc,
        PayloadType payloadType,
        const char *mid,
        const char *rid
        );

      RTCRtpListener^ _listener;

      void SetOwnerObject(RTCRtpListener^ owner) { _listener = owner; }
    };

    public ref class RTCRtpRListenerUnhandledRtp sealed
    {
    public:
      property uint64             Ssrc;
      property byte               PayloadType;
      property Platform::String^  MuxId;
      property Platform::String^  Rid;
    };

    public ref class RTCRtpRListenerUnhandledRtpEvent sealed
    {
    public:
      property RTCRtpRListenerUnhandledRtp^ UnhandledRtp
      {
        RTCRtpRListenerUnhandledRtp^  get() { return m_rtp; }
        void  set(RTCRtpRListenerUnhandledRtp^ value) { m_rtp = value; }
      }

    private:
      RTCRtpRListenerUnhandledRtp^ m_rtp;
    };
    public delegate void RTCRtpListenerUnhandledRtpDelegate(RTCRtpRListenerUnhandledRtpEvent^ evt);

    ref class RTCRtpHeaderExtensionParameters;

    public ref class RTCRtpListener sealed
    {
      friend class FetchNativePointer;
      friend class RTCRtpListenerDelegate;
      friend class ConvertObjectToCx;
    public:
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCRtpListener(RTCDtlsTransport^ transport);
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithHeaderExtensions")]
      RTCRtpListener(RTCDtlsTransport^ transport, IVector<RTCRtpHeaderExtensionParameters^>^ headerExtensions);
    private:
      RTCDtlsTransport^ GetDtlsTransport();

    public:
      property RTCDtlsTransport^ Transport
      {
        RTCDtlsTransport^ get()
        {
          if (mNativePointer)
            return GetDtlsTransport();
          else
            return nullptr;
        }
      }

    private:
      IRTPListenerPtr mNativePointer;
      RTCRtpListenerDelegatePtr mNativeDelegatePointer;

    public:
      event RTCRtpListenerUnhandledRtpDelegate^  OnRTCRtpListenerUnhandledRtp;
    };
  }
}
