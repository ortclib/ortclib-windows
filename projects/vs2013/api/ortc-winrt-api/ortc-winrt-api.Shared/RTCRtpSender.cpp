#include <ortc/types.h>
#include "pch.h"
#include "RTCRtpSender.h"
#include "helpers.h"


using namespace ortc_winrt_api;

using Platform::Collections::Vector;
using Windows::Foundation::IAsyncAction;
namespace Concurrency
{
	using ::LONG;
}

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

RTCRtpSender::RTCRtpSender() :
mNativeDelegatePointer(nullptr),
mNativePointer(nullptr)
{
}

RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport) :
mNativeDelegatePointer(new RTCRtpSenderDelegate())
{
  if (!track && !transport)
  {
    return;
  }

  if (FetchNativePointer::FromDtlsTransport(transport)) // add mediaStreamTrack too
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IRTPSender::create(mNativeDelegatePointer, IMediaStreamTrackPtr(), FetchNativePointer::FromDtlsTransport(transport));
  }
}

RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport) :
mNativeDelegatePointer(new RTCRtpSenderDelegate())
{
  if (!track && !transport && !rtcpTransport)
  {
    return;
  }

  if (FetchNativePointer::FromDtlsTransport(transport) && FetchNativePointer::FromDtlsTransport(rtcpTransport)) // add mediaStreamTrack too
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IRTPSender::create(mNativeDelegatePointer, IMediaStreamTrackPtr(), FetchNativePointer::FromDtlsTransport(transport), FetchNativePointer::FromDtlsTransport(rtcpTransport));
  }
}

MediaStreamTrack^ RTCRtpSender::Track::get()
{
	if (_track == nullptr)
	{
		if (mNativePointer)
		{
			_track = ConvertObjectToCx::ToMediaStreamTrack(mNativePointer->track());
		}
	}
	return _track;
}

void RTCRtpSender::setTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport)
{
  if (mNativePointer)
  {
    mNativePointer->setTransport(FetchNativePointer::FromDtlsTransport(transport), FetchNativePointer::FromDtlsTransport(rtcpTransport));
  }
}

IAsyncAction^ RTCRtpSender::setTrack(MediaStreamTrack^ track)
{
	IAsyncAction^ ret = Concurrency::create_async([this,track]()
	{
		Concurrency::task_completion_event<void> tce;

		if (track == nullptr || mNativePointer == nullptr)
		{
			tce.set();
			return;
		}
		PromisePtr promise = mNativePointer->setTrack(FetchNativePointer::FromMediaTrack(track));
		RTCSenderPromiseObserverPtr pDelegate(make_shared<RTCSenderPromiseObserver>(tce));

		promise->then(pDelegate);
		promise->background();
		auto tceTask = Concurrency::task<void>(tce);

		return tceTask.get();
	});
	
	return ret;
}

RTCRtpCapabilities^ RTCRtpSender::getCapabilities(Platform::String^ kind)
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

void RTCRtpSender::send(RTCRtpParameters^ parameters)
{
	if (mNativePointer)
	{
		mNativePointer->send(FromCx(parameters));
	}
}

void RTCRtpSender::stop()
{
  if (mNativePointer)
  {
    mNativePointer->stop();
  }
}

//-----------------------------------------------------------------
#pragma mark RTCRtpSenderDelegate
//-----------------------------------------------------------------

void RTCRtpSenderDelegate::onRTPSenderError(
		IRTPSenderPtr sender,
		ErrorCode errorCode,
		String errorReason
		)
{
	auto evt = ref new RTCRtpSenderErrorEvent();
	evt->Error->ErrorCode = errorCode;
	evt->Error->ErrorReason = ToCx(errorReason);
	_sender->OnRTCRtpSenderError(evt);
}

void RTCRtpSenderDelegate::onRTPSenderSSRCConflict(
		IRTPSenderPtr sender,
		SSRCType ssrc
		)
{
	auto evt = ref new RTCRtpSenderSSRCConflictEvent();
	evt->SSRCConflict = ssrc;
	_sender->OnRTCRtpSenderSSRCConflict(evt);
}

RTCSenderPromiseObserver::RTCSenderPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce)
{

}

void RTCSenderPromiseObserver::onPromiseResolved(PromisePtr promise)
{
	mTce.set();
}
	
void RTCSenderPromiseObserver::onPromiseRejected(PromisePtr promise)
{

}
