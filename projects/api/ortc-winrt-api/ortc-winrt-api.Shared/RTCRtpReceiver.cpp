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

void RTCRtpReceiver::setTransport(RTCDtlsTransport ^ transport)
{
	if (mNativePointer)
	{
		mNativePointer->setTransport(FetchNativePointer::FromDtlsTransport(transport));
	}
}

void RTCRtpReceiver::setTransport(RTCDtlsTransport ^ transport, RTCDtlsTransport ^ rtcpTransport)
{
	if (mNativePointer)
	{
		mNativePointer->setTransport(FetchNativePointer::FromDtlsTransport(transport), FetchNativePointer::FromDtlsTransport(rtcpTransport));
	}
}


RTCRtpCapabilities ^ RTCRtpReceiver::getCapabilities(Platform::String^ kind)
{
	auto ret = ref new RTCRtpCapabilities();
	IRTPTypes::CapabilitiesPtr capabilitiesPtr;
	if (kind != nullptr)
	{
		if (Platform::String::CompareOrdinal(kind, "audio") == 0)
			capabilitiesPtr = IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Audio);
		else if (Platform::String::CompareOrdinal(kind, "video") == 0)
			capabilitiesPtr = IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Video);
		else
			capabilitiesPtr = IRtpReceiver::getCapabilities();
	}

	if (capabilitiesPtr)
	{
		for (IRTPTypes::CodecCapabilitiesList::iterator it = capabilitiesPtr->mCodecs.begin(); it != capabilitiesPtr->mCodecs.end(); ++it)
		{
			auto codec = ToCx((make_shared<IRTPTypes::CodecCapability>(*it)));
			ret->codecs->Append(codec);
		}

		for (IRTPTypes::HeaderExtensionsList::iterator it = capabilitiesPtr->mHeaderExtensions.begin(); it != capabilitiesPtr->mHeaderExtensions.end(); ++it)
		{
			auto codec = ToCx((make_shared<IRTPTypes::HeaderExtension>(*it)));
			ret->headerExtensions->Append(codec);
		}

		for (std::list<zsLib::String>::iterator it = capabilitiesPtr->mFECMechanisms.begin(); it != capabilitiesPtr->mFECMechanisms.end(); ++it)
		{
			ret->fecMechanisms->Append(ToCx(*it));
		}
		
	}

	return ret;
}


void RTCRtpReceiver::receive(RTCRtpParameters ^ parameters)
{
	if (mNativePointer)
	{
		mNativePointer->receive(FromCx(parameters));
	}
}

IVector<RTCRtpContributingSource^>^ ortc_winrt_api::RTCRtpReceiver::getContributingSource()
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

void RTCRtpReceiver::stop()
{
	if (mNativePointer)
		mNativePointer->stop();
}

MediaStreamTrack^ RTCRtpReceiver::GetTrack()
{
  return ConvertObjectToCx::ToMediaStreamTrack(mNativePointer->track());
}

RTCDtlsTransport^ RTCRtpReceiver::GetDtlsTransport(boolean isRtcp)
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