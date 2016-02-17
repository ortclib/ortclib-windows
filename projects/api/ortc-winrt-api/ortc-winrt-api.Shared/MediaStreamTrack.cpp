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
			IMediaStreamTrackTypes::CapabilitiesPtr  capabilitiesPtr = mNativePointer->getCapabilities();
			if (capabilitiesPtr)
			{
				MediaTrackCapabilities^ ret = ToCx(capabilitiesPtr);
				return ret;
			}
		}
		return nullptr;
	}

	MediaTrackConstraints^		MediaStreamTrack::GetConstraints()
	{
		if (mNativePointer)
		{
			IMediaStreamTrackTypes::TrackConstraintsPtr  trackConstraintsPtr = mNativePointer->getConstraints();
			if (trackConstraintsPtr)
			{
				MediaTrackConstraints^ ret = ToCx(trackConstraintsPtr);
				return ret;
			}
		}
		return nullptr;
	}

	MediaTrackSettings^			MediaStreamTrack::GetSettings()
	{
		if (mNativePointer)
		{
			IMediaStreamTrackTypes::SettingsPtr  settingsPtr = mNativePointer->getSettings();
			if (settingsPtr)
			{
				MediaTrackSettings^ ret = ToCx(settingsPtr);
				return ret;
			}
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

#ifdef USE_NEW_RENDERER
    Platform::String^ id = "stream";

    ComPtr<ABI::Windows::Media::Core::IMediaSource> comSource;
    WebRtcMediaSource::CreateMediaSource(&comSource, this, id);
    IMediaSource^ source = reinterpret_cast<IMediaSource^>(comSource.Get());
    return source;
#else
    Platform::String^ id = "stream";
    uint32 framerate = 30;
    return RTMediaStreamSource::CreateMediaSource(this, framerate, id);
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

      IMediaStreamTrackTypes::TrackConstraints trackConstraints;
      
      for (MediaTrackConstraintSet^ iterator : constraints->Advanced)
      {
        IMediaStreamTrackTypes::ConstraintSetPtr set = FromCx(iterator);
        trackConstraints.mAdvanced.push_back(set);
      }
      PromisePtr promise = mNativePointer->applyConstraints(trackConstraints);
      MediaStreamTrackConstraintsPromiseObserverPtr pDelegate(make_shared<MediaStreamTrackConstraintsPromiseObserver>(tce));

      promise->then(pDelegate);
      promise->background();
      auto tceTask = Concurrency::task<void>(tce);

      return tceTask.get();
    });

    return ret;
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