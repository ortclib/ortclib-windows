#include "pch.h"
#include "MediaStreamTrack.h"

namespace ortc_winrt_api
{
	MediaStreamTrack^	MediaStreamTrack::clone()
	{
		return nullptr;
	}
	void MediaStreamTrack::stop()
	{
		if (mNativePointer)
			mNativePointer->stop();
	}

	MediaTrackCapabilities^		MediaStreamTrack::getCapabilities()
	{
		return nullptr;
	}

	MediaTrackConstraints^		MediaStreamTrack::getConstraints()
	{
		return nullptr;
	}

	MediaTrackSettings^			MediaStreamTrack::getSettings()
	{
		return nullptr;
	}
}