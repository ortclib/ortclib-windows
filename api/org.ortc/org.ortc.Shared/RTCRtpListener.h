
#pragma once

#include <ortc/IRTPListener.h>

//#include <collection.h>
//#include "RTCDtlsTransport.h"
//
//using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPListener, IRTPListener)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPListenerDelegate, IRTPListenerDelegate)

    ZS_DECLARE_CLASS_PTR(RTCRtpListenerDelegate)

    using Windows::Foundation::Collections::IVector;

    ref class RTCDtlsTransport;
    ref class RTCRtpListener;
    ref struct RTCRtpHeaderExtensionParameters;

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

    public ref class RTCRtpListener sealed
    {
      friend class RTCRtpListenerDelegate;

    public:
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCRtpListener(RTCDtlsTransport^ transport);
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithHeaderExtensions")]
      RTCRtpListener(RTCDtlsTransport^ transport, IVector<RTCRtpHeaderExtensionParameters^>^ headerExtensions);

    public:
      property RTCDtlsTransport^ Transport { RTCDtlsTransport^ get(); }

    public:
      event RTCRtpListenerUnhandledRtpDelegate^  OnRTCRtpListenerUnhandledRtp;

    private:
      IRTPListenerPtr _nativePointer;
      RTCRtpListenerDelegatePtr _nativeDelegatePointer;
    };
  }
}
