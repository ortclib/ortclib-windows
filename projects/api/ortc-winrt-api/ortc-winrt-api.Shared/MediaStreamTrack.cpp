#include "pch.h"
#include "WebRtcMediaSource.h"
#include "RTMediaStreamSource.h"
#include <windows.media.h>
#include <windows.media.core.h>
#include "MediaStreamTrack.h"
#include "helpers.h"

namespace Concurrency
{
  using ::LONG;
}

namespace Microsoft {
  namespace WRL {
    using ::ULONG;
    using ::DWORD;
  }
}
using Microsoft::WRL::ComPtr;

void MediaStreamTrack_SetMediaElement(Platform::Object^ obj, void* element)
{
  if (obj == nullptr) return;

  ortc_winrt_api::MediaStreamTrack^ track = static_cast<ortc_winrt_api::MediaStreamTrack^>(obj);

  ortc_winrt_api::CallPrivateMethod::SetMediaElement(track, element);
};

namespace ortc_winrt_api
{

  MediaStreamTrackKind  MediaStreamTrack::Kind::get()
  {
    return internal::ConvertEnums::convert(mNativePointer->kind());
  }

  Platform::String^ MediaStreamTrack::Id::get()
  {
    return ToCx(mNativePointer->id());
  }

  Platform::String^ MediaStreamTrack::DeviceId::get()
  {
    return ToCx(mNativePointer->deviceID());
  }

  Platform::String^ MediaStreamTrack::Label::get()
  {
    return ToCx(mNativePointer->label());
  }

  Platform::Boolean MediaStreamTrack::Enabled::get()
  {
    return mNativePointer->enabled();
  }

  void MediaStreamTrack::Enabled::set(Platform::Boolean value)
  {
    mNativePointer->enabled(value);
  }

  Platform::Boolean MediaStreamTrack::Muted::get()
  {
    return mNativePointer->muted();
  }

  void MediaStreamTrack::Muted::set(Platform::Boolean value)
  {
    return mNativePointer->muted(value);
  }

  Platform::Boolean MediaStreamTrack::ReadOnly::get()
  {
    return mNativePointer->readOnly();
  }

  Platform::Boolean MediaStreamTrack::Remote::get()
  {
    return mNativePointer->remote();
  }

  MediaStreamTrackState MediaStreamTrack::ReadyState::get()
  {
    return internal::ConvertEnums::convert(mNativePointer->readyState());
  }

	MediaStreamTrack^ MediaStreamTrack::Clone()
	{
		if (mNativePointer)
		{
			IMediaStreamTrackPtr newNativePointer = mNativePointer->clone();
			if (newNativePointer)
			{
				auto ret = ref new MediaStreamTrack();
				ret->mNativePointer = newNativePointer;
				return ret;
			}
		}

		return nullptr;
	}
	void MediaStreamTrack::Stop()
	{
		if (mNativePointer)
			mNativePointer->stop();
	}

	MediaTrackCapabilities^		MediaStreamTrack::GetCapabilities()
	{
		if (mNativePointer)
		{
			return ToCx(mNativePointer->getCapabilities());
		}
		return nullptr;
	}

	MediaTrackConstraints^		MediaStreamTrack::GetConstraints()
	{
		if (mNativePointer)
		{
      return ToCx(mNativePointer->getConstraints());
		}
		return nullptr;
	}

	MediaTrackSettings^			MediaStreamTrack::GetSettings()
	{
		if (mNativePointer)
		{
			return ToCx(mNativePointer->getSettings());
		}
		return nullptr;
	}

  IMediaSource^ MediaStreamTrack::CreateMediaSource()
  {
    //return globals::RunOnGlobalThread<IMediaSource^>([track, id]() -> IMediaSource^ {
    //  ComPtr<ABI::Windows::Media::Core::IMediaSource> comSource;
    //  ortc_winrt_api::WebRtcMediaSource::CreateMediaSource(&comSource, track, id);
    //  IMediaSource^ source = reinterpret_cast<IMediaSource^>(comSource.Get());
    //  return source;
    //});

#ifdef USE_OLD_RENDERER
    Platform::String^ id = "stream";
    uint32 framerate = 30;
    return RTMediaStreamSource::CreateMediaSource(this, framerate, id);
#else
    Platform::String^ id = "stream";
    ComPtr<ABI::Windows::Media::Core::IMediaSource> comSource;
    WebRtcMediaSource::CreateMediaSource(&comSource, this, id);
    IMediaSource^ source = reinterpret_cast<IMediaSource^>(comSource.Get());
    return source;
#endif
  }

  IAsyncAction^ MediaStreamTrack::ApplyConstraints(MediaTrackConstraints^ constraints)
  {
    IAsyncAction^ ret = Concurrency::create_async([this, constraints]()
    {
      Concurrency::task_completion_event<void> tce;

      if (constraints == nullptr || mNativePointer == nullptr)
      {
        tce.set();
        return;
      }

      PromisePtr promise = mNativePointer->applyConstraints(*FromCx(constraints));
      MediaStreamTrackConstraintsPromiseObserverPtr pDelegate(make_shared<MediaStreamTrackConstraintsPromiseObserver>(tce));

      promise->then(pDelegate);
      promise->background();
      auto tceTask = Concurrency::task<void>(tce);

      return tceTask.get();
    });

    return ret;
  }

  void MediaStreamTrack::SetMediaElement(void* element)
  {
    if (mNativePointer)
    {
      mNativePointer->setMediaElement(element);
    }
  }

  Platform::String^ MediaStreamTrack::ToString()
  {
    throw ref new Platform::NotImplementedException();
  }

  Platform::String^ MediaStreamTrack::ToString(MediaStreamTrackState value)
  {
    return ToCx(IMediaStreamTrack::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ MediaStreamTrack::ToString(MediaStreamTrackKind value)
  {
    return ToCx(IMediaStreamTrack::toString(internal::ConvertEnums::convert(value)));
  }

  MediaStreamTrackState MediaStreamTrack::ToState(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IMediaStreamTrack::toState(FromCx(str).c_str()));
  }

  MediaStreamTrackKind MediaStreamTrack::ToKind(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IMediaStreamTrack::toKind(FromCx(str).c_str()));
  }

  MediaStreamTrackConstraintsPromiseObserver::MediaStreamTrackConstraintsPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce)
  {

  }

  void MediaStreamTrackConstraintsPromiseObserver::onPromiseResolved(PromisePtr promise)
  {
    mTce.set();
  }

  void MediaStreamTrackConstraintsPromiseObserver::onPromiseRejected(PromisePtr promise)
  {
  }
}
