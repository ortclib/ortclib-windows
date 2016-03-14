#pragma once

#include "RTPTypes.h"
#include <ortc/IRTPReceiver.h>
#include <collection.h>
#include "RTCRtpReceiver.h"
#include "RTCDtlsTransport.h"
#include "MediaStreamTrack.h"
#include "helpers.h"

using namespace ortc;

using Windows::Foundation::Collections::IVector;

namespace org
{
  namespace ortc
  {

    ZS_DECLARE_CLASS_PTR(RTCRtpReceiverDelegate)

    ref class RTCRtpReceiver;

    class RTCRtpReceiverDelegate : public IRTPReceiverDelegate
    {
    public:
      virtual void onRTPReceiverError(
        IRTPReceiverPtr receiver,
        ErrorCode errorCode,
        zsLib::String errorReason
        );

      RTCRtpReceiver^ _owner;

      void SetOwnerObject(RTCRtpReceiver^ owner) { _owner = owner; }
    };

    // Error event and delegate
    public ref class RTCRtpReceiverError sealed
    {
    public:
      property uint16 ErrorCode;
      property Platform::String^ ErrorReason;
    };
    public ref class RTCRtpReceiverErrorEvent sealed
    {
    public:
      property RTCRtpReceiverError^ Error
      {
        RTCRtpReceiverError^  get() { return m_error; }
        void  set(RTCRtpReceiverError^ value) { m_error = value; }
      }

    private:
      RTCRtpReceiverError^ m_error;
    };
    public delegate void RTCRtpReceiverErrorDelegate(RTCRtpReceiverErrorEvent^ evt);




    public ref class RTCRtpContributingSource sealed
    {
      friend RTCRtpReceiver;
    private:
      std::chrono::system_clock::time_point mTimeStamp;
      uint32       mSource;
      byte		mAudioLevel;

    public:
      /*property std::chrono::system_clock::time_point TimeStamp
      {
        std::chrono::system_clock::time_point  get() { return mTimeStamp; }
      }*/
      property uint32 Source
      {
        uint32  get() { return mSource; }
      }
      property byte AudioLevel
      {
        byte  get() { return mAudioLevel; }
      }
    };


    public  ref  class  RTCRtpReceiver sealed
    {
      friend class RTCRtpReceiverDelegate;
      friend class ConvertObjectToCx;
    private:
      IRTPReceiverPtr mNativePointer;
      RTCRtpReceiverDelegatePtr mNativeDelegatePointer;

    private:
      RTCDtlsTransport^ GetDtlsTransport(Platform::Boolean isRtcp);
      MediaStreamTrack^ GetTrack();

    private:
      RTCRtpReceiver();
    public:
      RTCRtpReceiver(RTCDtlsTransport^ transport);
      RTCRtpReceiver(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

      void SetTransport(RTCDtlsTransport^ transport);
      void SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      static RTCRtpCapabilities ^ GetCapabilities();
      [Windows::Foundation::Metadata::OverloadAttribute("GetCapabilitiesWithKind")]
      static RTCRtpCapabilities ^ GetCapabilities(Platform::String^ kind);
      void Receive(RTCRtpParameters^ parameters);
      IVector<RTCRtpContributingSource^>^ GetContributingSource();
      void Stop();

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

      event RTCRtpReceiverErrorDelegate^              OnRTCRtpReceiverError;
    };
  }
}
