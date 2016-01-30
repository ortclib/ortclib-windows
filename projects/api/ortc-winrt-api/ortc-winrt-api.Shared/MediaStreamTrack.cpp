#include "pch.h"
#include "MediaStreamTrack.h"
#include "helpers.h"

namespace ortc_winrt_api
{
	MediaStreamTrack^ MediaStreamTrack::clone()
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
	void MediaStreamTrack::stop()
	{
		if (mNativePointer)
			mNativePointer->stop();
	}

	MediaTrackCapabilities^		MediaStreamTrack::getCapabilities()
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

	MediaTrackConstraints^		MediaStreamTrack::getConstraints()
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

	MediaTrackSettings^			MediaStreamTrack::getSettings()
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