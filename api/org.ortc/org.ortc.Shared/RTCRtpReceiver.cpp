#include "pch.h"

#include "RTCRtpReceiver.h"
#include "RTCDtlsTransport.h"
#include "RtpTypes.h"
#include "helpers.h"

using namespace ortc;

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)
    
    RTCRtpReceiver::RTCRtpReceiver() :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCRtpReceiver::RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport ^ transport) :
      _nativeDelegatePointer(new RTCRtpReceiverDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer = IRTPReceiver::create(_nativeDelegatePointer, UseHelper::convert(kind), nativeTransport);
    }

    RTCRtpReceiver::RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport ^ transport, RTCDtlsTransport^ rtcpTransport)
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);
      _nativePointer = IRTPReceiver::create(_nativeDelegatePointer, UseHelper::convert(kind), nativeTransport, nativeRtcpTransport);
    }

    void RTCRtpReceiver::SetTransport(RTCDtlsTransport ^ transport)
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer->setTransport(nativeTransport);
    }

    void RTCRtpReceiver::SetTransport(RTCDtlsTransport ^ transport, RTCDtlsTransport ^ rtcpTransport)
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      auto nativeRtcpTransport = RTCDtlsTransport::Convert(rtcpTransport);
      _nativePointer->setTransport(nativeTransport, nativeRtcpTransport);
    }

    RTCRtpCapabilities^ RTCRtpReceiver::GetCapabilities()
    {
      return internal::ToCx(IRTPReceiver::getCapabilities());
    }

    RTCRtpCapabilities^ RTCRtpReceiver::GetCapabilities(Platform::String^ kind)
    {
      if (nullptr != kind)
      {
        if (Platform::String::CompareOrdinal(kind, "audio") == 0)
          return internal::ToCx(IRTPReceiver::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Audio));

        if (Platform::String::CompareOrdinal(kind, "video") == 0)
          return internal::ToCx(IRTPReceiver::getCapabilities(IMediaStreamTrackTypes::Kinds::Kind_Video));
      }

      return internal::ToCx(IRTPReceiver::getCapabilities());
    }


    void RTCRtpReceiver::Receive(RTCRtpParameters^ parameters)
    {
      if (_nativePointer)
      {
        assert(nullptr != parameters);
        _nativePointer->receive(*internal::FromCx(parameters));
      }
    }

    IVector<RTCRtpContributingSource^>^ org::ortc::RTCRtpReceiver::GetContributingSource()
    {
      auto ret = ref new Vector<RTCRtpContributingSource^>();

      if (_nativePointer)
      {
        auto contributingSource = _nativePointer->getContributingSources();
        for (IRTPReceiver::ContributingSourceList::iterator it = contributingSource.begin(); it != contributingSource.end(); ++it)
        {
          RTCRtpContributingSource^ source = ref new RTCRtpContributingSource();

          source->mSource = it->mCSRC;
          source->mAudioLevel = it->mAudioLevel;
        }
      }

      return ret;
    }

    void RTCRtpReceiver::Stop()
    {
      if (_nativePointer)
        _nativePointer->stop();
    }

    MediaStreamTrack^ RTCRtpReceiver::Track::get()
    {
      if (!_nativePointer) return nullptr;
      return MediaStreamTrack::Convert(_nativePointer->track());
    }

    RTCDtlsTransport^ RTCRtpReceiver::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->transport()));
    }

    RTCDtlsTransport^ RTCRtpReceiver::RtcpTransport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->rtcpTransport()));
    }

    //-----------------------------------------------------------------
    #pragma mark RTCRtpReceiverDelegate
    //-----------------------------------------------------------------
    void RTCRtpReceiverDelegate::onRTPReceiverError(IRTPReceiverPtr receiver, ErrorCode errorCode, zsLib::String errorReason)
    {
      auto evt = ref new RTCRtpReceiverErrorEvent();
      evt->Error->ErrorCode = errorCode;
      evt->Error->ErrorReason = UseHelper::ToCx(errorReason);
      _owner->OnRTCRtpReceiverError(evt);
    }

  } // namespace ortc
} // namespace org
