#include "pch.h"
#include "OrtcMediaDevices.h"
#include <ortc/types.h>
#include "helpers.h"


using namespace ortc;
using namespace ortc_winrt_api;
using Platform::Collections::Vector;
namespace Concurrency
{
	using ::LONG;
}

Constraints^ Constraints::create()
{
	return ref new Constraints();
}
Constraints^ Constraints::create(Constraints^ value)
{
	return nullptr;
}

void OrtcMediaDevicesDelegate::onMediaDevicesChanged()
{
	_mediaDevices->OnDeviceChange();
}


SupportedConstraints^ OrtcMediaDevices::GetSupportedConstraints()
{
	IMediaDevicesTypes::SupportedConstraintsPtr supportedConstraintsPtr = IMediaDevices::getSupportedConstraints();

	if (supportedConstraintsPtr)
	{
		return ToCx(*supportedConstraintsPtr.get());
	}
	return nullptr;
}

IAsyncOperation<IVector<MediaDeviceInfo^>^>^ OrtcMediaDevices::enumerateDevices()
{
	return Concurrency::create_async([]() -> IVector<MediaDeviceInfo^>^
	{
		Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce;

		IMediaDevicesTypes::PromiseWithDeviceListPtr promise = IMediaDevices::enumerateDevices();
		std::shared_ptr<OrtcMediaDevicesPromiseObserver> pDelegate(make_shared<OrtcMediaDevicesPromiseObserver>(tce));

		promise->then(pDelegate);
		promise->background();
		auto tceTask = Concurrency::task<IVector<MediaDeviceInfo^>^>(tce);

		return tceTask.get();
	});
}

IAsyncOperation<IVector<MediaStreamTrack^>^>^ OrtcMediaDevices::getUserMedia(Constraints^ constraints)
{
	IAsyncOperation<IVector<MediaStreamTrack^>^>^ ret = Concurrency::create_async([]() -> IVector<MediaStreamTrack^>^
	{
		Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce;

		IMediaDevicesTypes::PromiseWithMediaStreamTrackListPtr promise = IMediaDevices::getUserMedia();
		std::shared_ptr<OrtcMediaStreamTrackPromiseObserver> pDelegate(make_shared<OrtcMediaStreamTrackPromiseObserver>(tce));

		promise->then(pDelegate);
		promise->background();
		auto tceTask = Concurrency::task<IVector<MediaStreamTrack^>^>(tce);

		return tceTask.get();
	});

	return ret;
}
OrtcMediaDevicesPromiseObserver::OrtcMediaDevicesPromiseObserver(Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce) :
	mTce(tce)
{
}

void OrtcMediaDevicesPromiseObserver::onPromiseResolved(PromisePtr promise)
{
	auto ret = ref new Vector<MediaDeviceInfo^>();

	IMediaDevicesTypes::PromiseWithDeviceListPtr deviceListPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithDeviceList, promise);

	std::shared_ptr<IMediaDevicesTypes::DeviceListPtr> tempPtr  = deviceListPromise->value();

	for (IMediaDevicesTypes::DeviceList::iterator it = tempPtr->get()->begin(); it != tempPtr->get()->end(); ++it)
	{
		MediaDeviceInfo^ med = ToCx(*it);

		ret->Append(med);
	}

	mTce.set(ret);
}
void OrtcMediaDevicesPromiseObserver::onPromiseRejected(PromisePtr promise)
{
	IMediaDevicesTypes::PromiseWithDeviceListPtr deviceListPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithDeviceList, promise);
	mTce.set_exception(deviceListPromise->reason());
}

OrtcMediaStreamTrackPromiseObserver::OrtcMediaStreamTrackPromiseObserver(Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce)
{

}

void OrtcMediaStreamTrackPromiseObserver::onPromiseResolved(PromisePtr promise)
{
	auto ret = ref new Vector<MediaStreamTrack^>();

	IMediaDevicesTypes::PromiseWithMediaStreamTrackListPtr streamTrackPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithMediaStreamTrackList, promise);

	std::shared_ptr<IMediaDevicesTypes::MediaStreamTrackListPtr> tempPtr = streamTrackPromise->value();

	for (IMediaDevicesTypes::MediaStreamTrackList::iterator it = tempPtr->get()->begin(); it != tempPtr->get()->end(); ++it)
	{
		MediaStreamTrack^ track = ConvertObjectToCx::ToMediaStreamTrack(*it);

		ret->Append(track);
	}

	mTce.set(ret);
}
void OrtcMediaStreamTrackPromiseObserver::onPromiseRejected(PromisePtr promise)
{
	IMediaDevicesTypes::PromiseWithMediaStreamTrackListPtr streamTrackPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithMediaStreamTrackList, promise);
	mTce.set_exception(streamTrackPromise->reason());
}