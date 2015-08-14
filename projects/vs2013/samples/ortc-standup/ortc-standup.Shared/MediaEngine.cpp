#include "pch.h"
#include "MediaEngine.h"

#include <ppltasks.h>

#include <webrtc/modules/video_render/windows/video_render_winrt.h>

using namespace ortc_standup;

using namespace Windows::UI::Xaml::Controls;

using zsLib::IPromiseResolutionDelegate;

using ortc::IMediaDevices;

ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::Constraints, Constraints)

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

namespace ortc_standup
{
  class MediaElementWrapper : public webrtc::IWinRTMediaElement
  {
  public:
    MediaElementWrapper(MediaElement^ mediaElement) :mMediaElement(mediaElement){};

    virtual void Play(){
      mMediaElement->Play();
    };

    virtual void Stop(){
      mMediaElement->Stop();
    };

    virtual void SetMediaStreamSource(Windows::Media::Core::IMediaSource^ mss){
      mMediaElement->SetMediaStreamSource(mss);
    };

    MediaElement^ getMediaElement(){ return mMediaElement; }

  private:
    MediaElement^ mMediaElement;
  };

  class PromiseWithMediaStreamTrackListCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithMediaStreamTrackListCallback(MediaEngine* mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithMediaStreamTrackListCallbackPtr create(MediaEngine* mediaEngine)
    {
      return PromiseWithMediaStreamTrackListCallbackPtr(new PromiseWithMediaStreamTrackListCallback(mediaEngine));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      ortc::IMediaDevicesTypes::MediaStreamTrackListPtr trackList = promise->value<ortc::IMediaDevicesTypes::MediaStreamTrackList>();
      ortc::IMediaStreamTrackPtr track = *trackList->begin();
      track->setMediaElement(mMediaEngine->mLocalMediaWrapper);
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithMediaStreamTrackListCallback()
    {
    }

  private:
    MediaEngine* mMediaEngine;
  };

  class PromiseWithDeviceListCallback : public IPromiseResolutionDelegate
  {
  private:
    PromiseWithDeviceListCallback(MediaEngine* mediaEngine) : mMediaEngine(mediaEngine)
    {
    }

  public:
    static PromiseWithDeviceListCallbackPtr create(MediaEngine* mediaEngine)
    {
      return PromiseWithDeviceListCallbackPtr(new PromiseWithDeviceListCallback(mediaEngine));
    }

    virtual void onPromiseResolved(PromisePtr promise)
    {
      ortc::IMediaDevicesTypes::DeviceListPtr deviceList = promise->value<ortc::IMediaDevicesTypes::DeviceList>();
      ortc::IMediaDevicesTypes::Device device = deviceList->front();
      ortc::IMediaDevicesTypes::ConstraintSetPtr constraintSet = ortc::IMediaDevicesTypes::ConstraintSet::create();
      constraintSet->mDeviceID.mValue.value().mValue.value() = device.mDeviceID;
      constraintSet->mWidth.mValue.value() = 800;
      constraintSet->mHeight.mValue.value() = 600;
      ConstraintsPtr constraints = Constraints::create();
      constraints->mVideo = ortc::IMediaStreamTrackTypes::TrackConstraints::create();
      constraints->mVideo->mAdvanced.push_back(constraintSet);
      mMediaEngine->mPromiseWithMediaStreamTrackList = IMediaDevices::getUserMedia(*constraints);
      mMediaEngine->mPromiseWithMediaStreamTrackList->then(PromiseWithMediaStreamTrackListCallback::create(mMediaEngine));
    }

    virtual void onPromiseRejected(PromisePtr promise)
    {
    }

    ~PromiseWithDeviceListCallback()
    {
    }

  private:
    MediaEngine* mMediaEngine;
  };
}

MediaEngine::MediaEngine() :
  mStarted(false),
  mLocalMediaWrapper(NULL)
{
}

MediaEngine::~MediaEngine()
{
}

void MediaEngine::SetLocalMediaElement(MediaElement^ element)
{
  mLocalMediaWrapper = new MediaElementWrapper(element);
}

void MediaEngine::SetStartStopButton(Button^ button)
{
  mStartStopButton = button;
}

void MediaEngine::StartStopMedia()
{
  mPromiseWithDeviceList = IMediaDevices::enumerateDevices();
  mPromiseWithDeviceList->then(PromiseWithDeviceListCallback::create(this));
}
