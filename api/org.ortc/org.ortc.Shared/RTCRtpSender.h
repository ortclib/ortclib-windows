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

    ZS_DECLARE_CLASS_PTR(RTCRtpSenderDelegate)
    ZS_DECLARE_CLASS_PTR(RTCSenderPromiseObserver)

    ref class MediaStreamTrack;
    ref class RTCDtlsTransport;
    ref class RTCRtpSender;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;

    class RTCRtpSenderDelegate : public IRTPSenderDelegate
    {
    public:
      virtual void onRTPSenderError(
        IRTPSenderPtr sender,
        ErrorCode errorCode,
        zsLib::String errorReason
        );

      virtual void onRTPSenderSSRCConflict(
        IRTPSenderPtr sender,
        SSRCType ssrc
        );

      RTCRtpSender^ _sender;

      void SetOwnerObject(RTCRtpSender^ owner) { _sender = owner; }
    };


    public ref class RTCRtpSenderError sealed
    {
    public:
      property uint16 ErrorCode;
      property Platform::String^ ErrorReason;
    };

    //------------------------------------------
    // Events and Delegates
    //------------------------------------------

    // Error event and delegate
    public ref class RTCRtpSenderErrorEvent sealed {
    public:
      property RTCRtpSenderError^ Error
      {
        RTCRtpSenderError^  get() { return m_error; }
        void  set(RTCRtpSenderError^ value) { m_error = value; }
      }

    private:
      RTCRtpSenderError^ m_error;
    };

    public delegate void RTCRtpSenderErrorDelegate(RTCRtpSenderErrorEvent^ evt);

    // SSRC conflict event and delegate
    public ref class RTCRtpSenderSSRCConflictEvent sealed {
    public:
      property uint64 SSRCConflict;
    };

    public delegate void RTCRtpSenderSSRCConflictDelegate(RTCRtpSenderSSRCConflictEvent^ evt);
    //------------------------------------------
    // End Events and Delegates
    //------------------------------------------


    class RTCSenderPromiseObserver : public zsLib::IPromiseSettledDelegate
    {
    public:
      RTCSenderPromiseObserver(Concurrency::task_completion_event<void> tce);

      virtual void onPromiseSettled(PromisePtr promise);

    private:
      Concurrency::task_completion_event<void> mTce;
    };

    public ref class RTCRtpSender sealed
    {
      friend class RTCRtpSenderDelegate;

    private:
      RTCRtpSender();
    public:
      RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport);
      RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

      void SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      IAsyncAction^   SetTrack(MediaStreamTrack^ track);
      static RTCRtpCapabilities^          GetCapabilities(Platform::String^ kind);
      void                                Send(RTCRtpParameters^ parameters);
      void                                Stop();

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

      event RTCRtpSenderErrorDelegate^              OnRTCRtpSenderError;
      event RTCRtpSenderSSRCConflictDelegate^       OnRTCRtpSenderSSRCConflict;

    private:
      IRTPSenderPtr _nativePointer;
      RTCRtpSenderDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
