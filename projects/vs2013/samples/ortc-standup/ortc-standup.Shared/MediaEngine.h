#pragma once

#include <zsLib/Promise.h>
#include <ortc/IMediaDevices.h>
#include <ortc/IMediaStreamTrack.h>

ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::PromiseWithDeviceList, PromiseWithDeviceList)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::PromiseWithMediaStreamTrackList, PromiseWithMediaStreamTrackList)

namespace ortc_standup
{
  class MediaElementWrapper;
  ZS_DECLARE_CLASS_PTR(PromiseWithMediaStreamTrackListCallback)
  ZS_DECLARE_CLASS_PTR(PromiseWithDeviceListCallback)

  class MediaEngine
  {
    friend PromiseWithDeviceListCallback;
    friend PromiseWithMediaStreamTrackListCallback;

  public:
    MediaEngine();
    ~MediaEngine();
    void SetLocalMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetStartStopButton(Windows::UI::Xaml::Controls::Button^ button);
    void StartStopMedia();

  private:
    Windows::UI::Core::CoreDispatcher^ mDispatcher;
    bool mStarted;

    Windows::UI::Xaml::Controls::Button^ mStartStopButton;
    MediaElementWrapper* mLocalMediaWrapper;
    PromiseWithDeviceListPtr mPromiseWithDeviceList;
    PromiseWithMediaStreamTrackListPtr mPromiseWithMediaStreamTrackList;
    ortc::IMediaStreamTrackPtr mVideoMediaStreamTrack;
  };
}
