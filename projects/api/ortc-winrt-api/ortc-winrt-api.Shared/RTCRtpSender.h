#pragma once

#include <ortc/IRTPSender.h>
#include "RTCDtlsTransport.h"
#include "RTCRtpReceiver.h"
#include "MediaStreamTrack.h"

using namespace ortc;

using Windows::Foundation::IAsyncAction;
using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
  ZS_DECLARE_CLASS_PTR(RTCRtpSenderDelegate)
  ZS_DECLARE_CLASS_PTR(RTCSenderPromiseObserver)
  
  ref class RTCRtpSender;


  class RTCRtpSenderDelegate : public IRTPSenderDelegate
  {
  public:
    virtual void onRTPSenderError(
      IRTPSenderPtr sender,
      ErrorCode errorCode,
      String errorReason
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
      RTCRtpSenderError^  get(){ return m_error; }
      void  set(RTCRtpSenderError^ value){ m_error = value; }
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


  class RTCSenderPromiseObserver : public zsLib::IPromiseResolutionDelegate
  {
  public:
	  RTCSenderPromiseObserver(Concurrency::task_completion_event<void> tce);

	  virtual void onPromiseResolved(PromisePtr promise);
	  virtual void onPromiseRejected(PromisePtr promise);

  private:
	  Concurrency::task_completion_event<void> mTce;
  };

  public  ref  class  RTCRtpSender sealed
  {
    friend class RTCRtpSenderDelegate;
    friend class FetchNativePointer;
	friend class ConvertObjectToCx;
  private:
    IRTPSenderPtr mNativePointer;
    RTCRtpSenderDelegatePtr mNativeDelegatePointer;

  private:
    RTCDtlsTransport^ GetDtlsTransport(boolean isRtcp);
    MediaStreamTrack^ GetTrack();
  public:

	  property MediaStreamTrack^ Track
	  {
      MediaStreamTrack^ get()
      {
        if (mNativePointer)
          return GetTrack();
        else
          return nullptr;
      }
	  }

    property RTCDtlsTransport^ Transport
    {
      RTCDtlsTransport^ get()
      {
        if (mNativePointer)
          return GetDtlsTransport(false);
        else
          return nullptr;
      }
    }

    property RTCDtlsTransport^ RtcpTransport
    {
      RTCDtlsTransport^ get()
      {
        if (mNativePointer)
          return GetDtlsTransport(true);
        else
          return nullptr;
      }
    }

    RTCRtpSender();
    RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport);
    RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

    void setTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
    IAsyncAction^   setTrack(MediaStreamTrack^ track);
    static RTCRtpCapabilities^          getCapabilities(Platform::String^ kind);
    void                                send(RTCRtpParameters^ parameters);
    void                                stop();

    event RTCRtpSenderErrorDelegate^              OnRTCRtpSenderError;
    event RTCRtpSenderSSRCConflictDelegate^       OnRTCRtpSenderSSRCConflict;
  };
} // namespace ortc_winrt_api
