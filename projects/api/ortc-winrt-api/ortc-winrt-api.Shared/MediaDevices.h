#pragma once
#include <collection.h>
#include "MediaTypes.h"
#include "MediaStreamTrack.h"
#include <ortc/IMediaDevices.h>


using Platform::Array;
using Platform::Collections::Vector;
using Windows::Foundation::Collections::IVector;
using Windows::Foundation::IAsyncOperation;

namespace ortc_winrt_api
{
  ZS_DECLARE_CLASS_PTR(MediaStreamTrackPromiseObserver)
  ZS_DECLARE_CLASS_PTR(MediaDevicesPromiseObserver)
  public ref struct Constraints sealed
  {
    property MediaTrackConstraints^ Video;
    property MediaTrackConstraints^ Audio;

    static Constraints^ create();
    static Constraints^ create(Constraints^ value);
  };

  public delegate void MediaDevicesChangeDelegate();

  class MediaDevicesPromiseObserver : public zsLib::IPromiseResolutionDelegate
  {
  public:
    MediaDevicesPromiseObserver(Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce);

    virtual void onPromiseResolved(PromisePtr promise);
    virtual void onPromiseRejected(PromisePtr promise);

  private:
    Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> mTce;
  };

  class MediaStreamTrackPromiseObserver : public zsLib::IPromiseResolutionDelegate
  {
  public:
    MediaStreamTrackPromiseObserver(Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce);

    virtual void onPromiseResolved(PromisePtr promise);
    virtual void onPromiseRejected(PromisePtr promise);

  private:
    Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> mTce;
  };


  public ref class MediaDevices sealed
  {
    friend class MediaDevicesDelegate;
  public:
    static SupportedConstraints^ GetSupportedConstraints();

    static IAsyncOperation<IVector<MediaDeviceInfo^>^>^ EnumerateDevices();

    static IAsyncOperation<IVector<MediaStreamTrack^>^>^ GetUserMedia(Constraints^ constraints);

    event MediaDevicesChangeDelegate^   OnDeviceChange;
  };

  class MediaDevicesDelegate : public IMediaDevicesDelegate
  {
  public:
    void onMediaDevicesChanged();

    MediaDevices^ _mediaDevices;

    void SetOwnerObject(MediaDevices^ owner) { _mediaDevices = owner; }
  };
}

