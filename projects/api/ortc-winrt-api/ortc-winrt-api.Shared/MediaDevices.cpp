#include "pch.h"
#include "MediaDevices.h"
#include <ortc/types.h>
#include "helpers.h"

namespace rtc
{
  using ::LONG;
}

#include "webrtc/modules/video_capture/windows/device_info_winrt.h"
#include "webrtc/modules/video_capture/windows/video_capture_winrt.h"

using namespace ortc;
using namespace org::ortc;
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

void MediaDevicesDelegate::onMediaDevicesChanged()
{
	_mediaDevices->OnDeviceChange();
}


SupportedConstraints^ MediaDevices::GetSupportedConstraints()
{
	return ToCx(IMediaDevices::getSupportedConstraints());
}

IAsyncOperation<IVector<MediaDeviceInfo^>^>^ MediaDevices::EnumerateDevices()
{
	IAsyncOperation<IVector<MediaDeviceInfo^>^>^ ret = Concurrency::create_async([]() -> IVector<MediaDeviceInfo^>^
	{
		Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce;

		IMediaDevicesTypes::PromiseWithDeviceListPtr promise = IMediaDevices::enumerateDevices();
		MediaDevicesPromiseObserverPtr pDelegate(make_shared<MediaDevicesPromiseObserver>(tce));

		promise->then(pDelegate);
		promise->background();
		auto tceTask = Concurrency::task<IVector<MediaDeviceInfo^>^>(tce);

		return tceTask.get();
	});

	return ret;
}

IAsyncOperation<IVector<MediaStreamTrack^>^>^ MediaDevices::GetUserMedia(Constraints^ constraints)
{
	IAsyncOperation<IVector<MediaStreamTrack^>^>^ ret = Concurrency::create_async([constraints]() -> IVector<MediaStreamTrack^>^
	{
		Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce;

    assert(nullptr != constraints);

		IMediaDevicesTypes::PromiseWithMediaStreamTrackListPtr promise = IMediaDevices::getUserMedia(*(FromCx(constraints).get()));
		MediaStreamTrackPromiseObserverPtr pDelegate(make_shared<MediaStreamTrackPromiseObserver>(tce));

		promise->then(pDelegate);
		promise->background();
		auto tceTask = Concurrency::task<IVector<MediaStreamTrack^>^>(tce);

		return tceTask.get();
	});

	return ret;
}

void MediaDevices::OnAppSuspending() {
  // not implemented in current webrtc version - uncomment after webrtc is updated
  //webrtc::videocapturemodule::MediaCaptureDevicesWinRT::Instance()->ClearCaptureDevicesCache();
}

void MediaDevices::SetDisplayOrientation(
  Windows::Graphics::Display::DisplayOrientations display_orientation) {
  webrtc::videocapturemodule::AppStateDispatcher::Instance()->DisplayOrientationChanged(display_orientation);
}

MediaDevicesPromiseObserver::MediaDevicesPromiseObserver(Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce) : mTce(tce)
{
}

void MediaDevicesPromiseObserver::onPromiseResolved(PromisePtr promise)
{
	auto ret = ref new Vector<MediaDeviceInfo^>();

	IMediaDevicesTypes::DeviceListPtr deviceList = promise->value<IMediaDevicesTypes::DeviceList>();

  if (deviceList)
  {
    for (IMediaDevicesTypes::DeviceList::iterator it = deviceList->begin(); it != deviceList->end(); ++it)
    {
      MediaDeviceInfo^ med = ToCx(*it);
      ret->Append(med);
    }
  }

	mTce.set(ret);
}

void MediaDevicesPromiseObserver::onPromiseRejected(PromisePtr promise)
{
	IMediaDevicesTypes::PromiseWithDeviceListPtr deviceListPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithDeviceList, promise);
	mTce.set_exception(deviceListPromise->reason());
}

MediaStreamTrackPromiseObserver::MediaStreamTrackPromiseObserver(Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce) : mTce(tce)
{
}

void MediaStreamTrackPromiseObserver::onPromiseResolved(PromisePtr promise)
{
	auto ret = ref new Vector<MediaStreamTrack^>();

	IMediaDevicesTypes::MediaStreamTrackListPtr mediaStreamTrackListPtr2 = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::MediaStreamTrackList, promise);

	//test
	IMediaDevicesTypes::MediaStreamTrackListPtr mediaStreamTrackListPtr = promise->value<IMediaDevicesTypes::MediaStreamTrackList>();

	if (mediaStreamTrackListPtr)
	{
		for (IMediaDevicesTypes::MediaStreamTrackList::iterator it = mediaStreamTrackListPtr->begin(); it != mediaStreamTrackListPtr->end(); ++it)
		{
			MediaStreamTrack^ track = ConvertObjectToCx::ToMediaStreamTrack(*it);
			ret->Append(track);
		}
	}
	mTce.set(ret);
}

void MediaStreamTrackPromiseObserver::onPromiseRejected(PromisePtr promise)
{
	IMediaDevicesTypes::PromiseWithMediaStreamTrackListPtr streamTrackPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithMediaStreamTrackList, promise);
	mTce.set_exception(streamTrackPromise->reason());
}