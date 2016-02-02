#include "pch.h"
#include "MediaStreamTrack.h"
#include "helpers.h"


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
}