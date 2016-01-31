#include "pch.h"
#include "MediaStreamTrack.h"
#include "helpers.h"

namespace ortc_winrt_api
{
  MediaStreamTrackKind  MediaStreamTrack::Kind::get()
  {
    return static_cast<MediaStreamTrackKind>(mNativePointer->kind());
  }

  Platform::String^ MediaStreamTrack::Id::get()
  {
    return ToCx(mNativePointer->id());
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
    return static_cast<MediaStreamTrackState>(mNativePointer->readyState());
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
}