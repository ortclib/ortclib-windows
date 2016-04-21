#pragma once

#include <ortc/IRTPSender.h>

#include <ppltasks.h>

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::IAsyncAction;

    ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)

    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPSender, IRTPSender)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPSenderDelegate, IRTPSenderDelegate)

    ref class MediaStreamTrack;
    ref class RTCDtlsTransport;
    ref class RTCRtpSender;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCRtpSenderDelegate)
      ZS_DECLARE_CLASS_PTR(RTCRtpSenderPromiseObserver)
    } // namespace internal

    public ref struct RTCSsrcConflictEvent sealed
    {
      friend class internal::RTCRtpSenderDelegate;

      property uint32 Ssrc
      {
        uint32 get() { return _ssrc; }
      }

    private:
      uint32 _ssrc;
    };

    public delegate void RTCRtpSenderSSRCConflictDelegate(RTCSsrcConflictEvent^ evt);

    public ref class RTCRtpSender sealed
    {
      friend class internal::RTCRtpSenderDelegate;

    private:
      RTCRtpSender();
    public:
      RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport);
      RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

      static RTCRtpCapabilities^  GetCapabilities(Platform::String^ kind);

      void                        SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      IAsyncAction^               SetTrack(MediaStreamTrack^ track);
      IAsyncAction^               Send(RTCRtpParameters^ parameters);
      void                        Stop();

      property MediaStreamTrack^ Track
      {
        MediaStreamTrack^ get();
      }

      property RTCDtlsTransport^ Transport
      {
        RTCDtlsTransport^ get();
      }

      property RTCDtlsTransport^ RtcpTransport
      {
        RTCDtlsTransport^ get();
      }

      event RTCRtpSenderSSRCConflictDelegate^       OnSsrcConflict;

    private:
      IRTPSenderPtr _nativePointer;
      internal::RTCRtpSenderDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
