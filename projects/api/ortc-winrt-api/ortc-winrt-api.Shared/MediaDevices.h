#pragma once
#include <collection.h>
#include "MediaStreamTrack.h"
#include <ortc/IMediaDevices.h>

using Platform::Array;
using Platform::Collections::Vector;
using Windows::Foundation::Collections::IVector;
using Windows::Foundation::IAsyncOperation;
using zsLib::PromisePtr;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_CLASS_PTR(MediaStreamTrackPromiseObserver)
      ZS_DECLARE_CLASS_PTR(MediaDevicesPromiseObserver)

      public enum class MediaDeviceKind
    {
      AudioInput,
      AudioOutput,
      Video,
    };

    public ref struct SupportedConstraints sealed
    {
      property Platform::Boolean Width;
      property Platform::Boolean Height;
      property Platform::Boolean AspectRatio;
      property Platform::Boolean FrameRate;
      property Platform::Boolean FacingMode;
      property Platform::Boolean Volume;
      property Platform::Boolean SampleRate;
      property Platform::Boolean SampleSize;
      property Platform::Boolean EchoCancellation;
      property Platform::Boolean Latency;
      property Platform::Boolean DeviceId;
      property Platform::Boolean GroupId;
    };

    public ref struct MediaDeviceInfo sealed
    {
      property MediaDeviceKind Kind;

      property Platform::String^ Label;
      property Platform::String^ DeviceId;
      property Platform::String^ GroupId;

      property SupportedConstraints^ SupportedConstraints;
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

      static void OnAppSuspending();

      static void SetDisplayOrientation(
        Windows::Graphics::Display::DisplayOrientations display_orientation
        );

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
}
