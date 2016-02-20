#include "pch.h"
#include <ortc/types.h>
#include "RTCRtpSender.h"
#include "helpers.h"


using namespace ortc_winrt_api;

using Platform::Collections::Vector;
using Windows::Foundation::IAsyncAction;
namespace Concurrency
{
	using ::LONG;
}

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

MediaStreamTrack^ RTCRtpSender::GetTrack()
{
		return ConvertObjectToCx::ToMediaStreamTrack(mNativePointer->track());
}

RTCDtlsTransport^ RTCRtpSender::GetDtlsTransport(Platform::Boolean isRtcp)
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

void RTCRtpSender::SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport)
{
  if (mNativePointer)
  {
    mNativePointer->setTransport(FetchNativePointer::FromDtlsTransport(transport), FetchNativePointer::FromDtlsTransport(rtcpTransport));
  }
}

IAsyncAction^ RTCRtpSender::SetTrack(MediaStreamTrack^ track)
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

RTCRtpCapabilities^ RTCRtpSender::GetCapabilities(Platform::String^ kind)
{
  if (kind != nullptr)
  {
    if (Platform::String::CompareOrdinal(kind, "audio") == 0)
      return ToCx(IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Audio));
    if (Platform::String::CompareOrdinal(kind, "video") == 0)
      return ToCx(IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Video));
  }

  return ToCx(IRtpReceiver::getCapabilities());
}

void RTCRtpSender::Send(RTCRtpParameters^ parameters)
{
	if (mNativePointer)
	{
    assert(nullptr != parameters);
		mNativePointer->send(*FromCx(parameters));
	}
}

void RTCRtpSender::Stop()
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
