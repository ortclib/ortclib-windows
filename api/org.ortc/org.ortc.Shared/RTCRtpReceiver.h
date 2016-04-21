
#pragma once

#include "MediaStreamTrack.h"

#include <ortc/IRTPReceiver.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiverDelegate, IRTPReceiverDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiver, IRTPReceiver)

    using Windows::Foundation::Collections::IVector;

    ref class MediaStreamTrack;
    ref class RTCDtlsTransport;
    ref class RTCRtpReceiver;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;

    enum class MediaStreamTrackKind;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCRtpReceiverDelegate)
      ZS_DECLARE_CLASS_PTR(RTCRtpReceiverPromiseObserver)
    }

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

      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      static RTCRtpCapabilities^          GetCapabilities();
      [Windows::Foundation::Metadata::OverloadAttribute("GetCapabilitiesWithKind")]
      static RTCRtpCapabilities^          GetCapabilities(Platform::String^ kind);

      void                                SetTransport(RTCDtlsTransport^ transport);
      void                                SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

      IAsyncAction^                       Receive(RTCRtpParameters^ parameters);
      IVector<RTCRtpContributingSource^>^ GetContributingSource();
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
      
    private:
      IRTPReceiverPtr _nativePointer;
      internal::RTCRtpReceiverDelegatePtr _nativeDelegatePointer;
    };
  }
}
