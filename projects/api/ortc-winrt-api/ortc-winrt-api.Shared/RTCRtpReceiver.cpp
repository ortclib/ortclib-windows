#include "pch.h"
#include "RTCRtpReceiver.h"
#include "helpers.h"
#include <ortc/types.h>

using namespace ortc_winrt_api;
using namespace Platform;
using namespace zsLib;
using namespace ortc;
using ortc::IRtpReceiver;
using Platform::Collections::Vector;

RTCRtpReceiver::RTCRtpReceiver() : mNativeDelegatePointer(nullptr), mNativePointer(nullptr)
{
}

RTCRtpReceiver::RTCRtpReceiver(RTCDtlsTransport ^ transport) : mNativeDelegatePointer(new RTCRtpReceiverDelegate())
{
	if (!transport)
	{
		return;
	}

	if (FetchNativePointer::FromDtlsTransport(transport)) // add mediaStreamTrack too
	{
		mNativeDelegatePointer->SetOwnerObject(this);
		mNativePointer = IRtpReceiver::create(mNativeDelegatePointer, FetchNativePointer::FromDtlsTransport(transport));
	}
}

RTCRtpReceiver::RTCRtpReceiver(RTCDtlsTransport ^ transport, RTCDtlsTransport^ rtcpTransport)
{
	if (!transport && !rtcpTransport)
	{
		return;
	}

	if (FetchNativePointer::FromDtlsTransport(transport) && FetchNativePointer::FromDtlsTransport(rtcpTransport)) // add mediaStreamTrack too
	{
		mNativeDelegatePointer->SetOwnerObject(this);
		mNativePointer = IRtpReceiver::create(mNativeDelegatePointer, FetchNativePointer::FromDtlsTransport(transport), FetchNativePointer::FromDtlsTransport(rtcpTransport));
	}
}

void RTCRtpReceiver::SetTransport(RTCDtlsTransport ^ transport)
{
	if (mNativePointer)
	{
		mNativePointer->setTransport(FetchNativePointer::FromDtlsTransport(transport));
	}
}

void RTCRtpReceiver::SetTransport(RTCDtlsTransport ^ transport, RTCDtlsTransport ^ rtcpTransport)
{
	if (mNativePointer)
	{
		mNativePointer->setTransport(FetchNativePointer::FromDtlsTransport(transport), FetchNativePointer::FromDtlsTransport(rtcpTransport));
	}
}

RTCRtpCapabilities^ RTCRtpReceiver::GetCapabilities()
{
  return ToCx(IRtpReceiver::getCapabilities());
}

RTCRtpCapabilities^ RTCRtpReceiver::GetCapabilities(Platform::String^ kind)
{
	if (nullptr != kind)
	{
		if (Platform::String::CompareOrdinal(kind, "audio") == 0)
			return ToCx(IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Audio));

	  if (Platform::String::CompareOrdinal(kind, "video") == 0)
			return ToCx(IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Video));
  }

  return ToCx(IRtpReceiver::getCapabilities());
}


void RTCRtpReceiver::Receive(RTCRtpParameters^ parameters)
{
	if (mNativePointer)
	{
    assert(nullptr != parameters);
		mNativePointer->receive(*FromCx(parameters));
	}
}

IVector<RTCRtpContributingSource^>^ ortc_winrt_api::RTCRtpReceiver::GetContributingSource()
{
	auto ret = ref new Vector <RTCRtpContributingSource^>();

	if (mNativePointer)
	{
		auto contributingSource = mNativePointer->getContributingSources();
		for (IRtpReceiver::ContributingSourceList::iterator it = contributingSource.begin(); it != contributingSource.end(); ++it) 
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
	if (mNativePointer)
		mNativePointer->stop();
}

MediaStreamTrack^ RTCRtpReceiver::GetTrack()
{
  return ConvertObjectToCx::ToMediaStreamTrack(mNativePointer->track());
}

RTCDtlsTransport^ RTCRtpReceiver::GetDtlsTransport(Platform::Boolean isRtcp)
{
  if (!isRtcp)
  {
    return ConvertObjectToCx::ToDtlsTransport(IDTLSTransport::convert(mNativePointer->transport()));
  }
  else
  {
    return ConvertObjectToCx::ToDtlsTransport(IDTLSTransport::convert(mNativePointer->rtcpTransport()));
  }
}
//-----------------------------------------------------------------
#pragma mark RTCRtpReceiverDelegate
//-----------------------------------------------------------------
void RTCRtpReceiverDelegate::onRTPReceiverError(IRTPReceiverPtr receiver, ErrorCode errorCode, zsLib::String errorReason)
{
	auto evt = ref new RTCRtpReceiverErrorEvent();
	evt->Error->ErrorCode = errorCode;
	evt->Error->ErrorReason = ToCx(errorReason);
	_owner->OnRTCRtpReceiverError(evt);
}