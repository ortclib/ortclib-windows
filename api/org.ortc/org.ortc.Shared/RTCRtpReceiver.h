
#pragma once

#include "MediaStreamTrack.h"

#include <ortc/IRTPReceiver.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiverDelegate, IRTPReceiverDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiver, IRTPReceiver)

    ZS_DECLARE_CLASS_PTR(RTCRtpReceiverDelegate)

    using Windows::Foundation::Collections::IVector;

    ref class MediaStreamTrack;
    ref class RTCDtlsTransport;
    ref class RTCRtpReceiver;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;

    enum class MediaStreamTrackKind;

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

    private:
      RTCRtpReceiver();

    public:
      RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport^ transport);
      RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

      void SetTransport(RTCDtlsTransport^ transport);
      void SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      static RTCRtpCapabilities ^ GetCapabilities();
      [Windows::Foundation::Metadata::OverloadAttribute("GetCapabilitiesWithKind")]
      static RTCRtpCapabilities ^ GetCapabilities(Platform::String^ kind);
      void Receive(RTCRtpParameters^ parameters);
      IVector<RTCRtpContributingSource^>^ GetContributingSource();
      void Stop();

      property MediaStreamTrack^ Track          { MediaStreamTrack^ get(); }
      property RTCDtlsTransport^ Transport      { RTCDtlsTransport^ get(); }
      property RTCDtlsTransport^ RtcpTransport  { RTCDtlsTransport^ get(); }
      
      event RTCRtpReceiverErrorDelegate^              OnRTCRtpReceiverError;

    private:
      IRTPReceiverPtr _nativePointer;
      RTCRtpReceiverDelegatePtr _nativeDelegatePointer;
    };
  }
}
