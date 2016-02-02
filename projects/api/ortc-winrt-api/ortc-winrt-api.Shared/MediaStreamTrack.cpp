#include "pch.h"
#include "MediaStreamTrack.h"
#include "helpers.h"


namespace ortc_winrt_api
{
  namespace internal
  {
    //---------------------------------------------------------------------------
    // MediaStreamTrackState convert methods
    //---------------------------------------------------------------------------
    static IMediaStreamTrack::States convert(MediaStreamTrackState state)
    {
      switch (state) {
      case MediaStreamTrackState::Live:   return IMediaStreamTrack::States::State_Live;
      case MediaStreamTrackState::Ended:  return IMediaStreamTrack::States::State_Ended;
      }
      throw ref new Platform::NotImplementedException();
    }

    static MediaStreamTrackState convert(IMediaStreamTrack::States state)
    {
      switch (state) {
      case IMediaStreamTrack::States::State_Live:   return MediaStreamTrackState::Live;
      case IMediaStreamTrack::States::State_Ended:         return MediaStreamTrackState::Ended;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // MediaStreamTrackKind convert methods
    //---------------------------------------------------------------------------
    static IMediaStreamTrack::Kinds convert(MediaStreamTrackKind kind)
    {
      switch (kind) {
      case MediaStreamTrackKind::Audio:   return IMediaStreamTrack::Kinds::Kind_Audio;
      case MediaStreamTrackKind::Video:   return IMediaStreamTrack::Kinds::Kind_Video;
      }
      throw ref new Platform::NotImplementedException();
    }

    static MediaStreamTrackKind convert(IMediaStreamTrack::Kinds kind)
    {
      switch (kind) {
      case IMediaStreamTrack::Kinds::Kind_Audio:   return MediaStreamTrackKind::Audio;
      case IMediaStreamTrack::Kinds::Kind_Video:   return MediaStreamTrackKind::Video;
      }
      throw ref new Platform::NotImplementedException();
    }
  } // namespace internal
  MediaStreamTrackKind  MediaStreamTrack::Kind::get()
  {
    return internal::convert(mNativePointer->kind());
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
    return internal::convert(mNativePointer->readyState());
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

  Platform::String^ MediaStreamTrack::ToString(MediaStreamTrackState value)
  {
    return ToCx(IMediaStreamTrack::toString(internal::convert(value)));
  }

  Platform::String^ MediaStreamTrack::ToString(MediaStreamTrackKind value)
  {
    return ToCx(IMediaStreamTrack::toString(internal::convert(value)));
  }

  MediaStreamTrackState MediaStreamTrack::ToState(Platform::String^ str)
  {
    return internal::convert(IMediaStreamTrack::toState(FromCx(str).c_str()));
  }

  MediaStreamTrackKind MediaStreamTrack::ToKind(Platform::String^ str)
  {
    return internal::convert(IMediaStreamTrack::ToKind(FromCx(str).c_str()));
  }
}