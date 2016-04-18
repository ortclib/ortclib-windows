
#pragma once

#include <ortc/IMediaDevices.h>

#include <zsLib/types.h>

#include <collection.h>

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;
    using Windows::Foundation::IAsyncOperation;
    using zsLib::PromisePtr;

    ref struct MediaStreamConstraints;
    ref struct MediaTrackConstraints;
    ref struct MediaDeviceInfo;
    ref struct MediaTrackSupportedConstraints;
    ref class MediaStreamTrack;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(MediaDevicesDelegate)
      
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IMediaDevicesTypes, IMediaDevicesTypes)
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IMediaStreamTrackTypes, IMediaStreamTrackTypes)

      MediaStreamConstraints^ ToCx(const IMediaStreamTrackTypes::Constraints &input);
      MediaStreamConstraints^ ToCx(IMediaStreamTrackTypes::ConstraintsPtr input);
      IMediaStreamTrackTypes::ConstraintsPtr FromCx(MediaStreamConstraints^ input);

      MediaDeviceInfo^ ToCx(const IMediaDevicesTypes::Device &input);
      MediaDeviceInfo^ ToCx(IMediaDevicesTypes::DevicePtr input);
      IMediaDevicesTypes::DevicePtr FromCx(MediaDeviceInfo^ input);

      MediaTrackSupportedConstraints^ ToCx(const IMediaDevicesTypes::SupportedConstraints &input);
      MediaTrackSupportedConstraints^ ToCx(IMediaDevicesTypes::SupportedConstraintsPtr input);
      IMediaDevicesTypes::SupportedConstraintsPtr FromCx(MediaTrackSupportedConstraints^ input);

    } // namespace internal

    /// <summary>
    /// Describes the kind of the represented device.
    /// </summary>
    public enum class MediaDeviceKind
    {
      /// <summary>
      /// Represents an audio input device; for example a microphone.
      /// </summary>
      AudioInput,
      /// <summary>
      /// Represents an audio output device; for example a pair of headphones.
      /// </summary>
      AudioOutput,
      /// <summary>
      /// Represents a video input device; for example a webcam.
      /// </summary>
      VideoInput
    };

    /// <summary>
    /// MediaTrackSupportedConstraints represents the list of constraints
    /// recognized by a User Agent for controlling the Capabilities of a
    /// MediaStreamTrack object.This dictionary is used as a function return
    /// value, and never as an operation argument.
    /// </summary>
    public ref struct MediaTrackSupportedConstraints sealed
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

    /// <summary>
    /// The MediaStreamConstraints dictionary is used to instruct the User
    /// Agent what sort of MediaStreamTracks to include in the MediaStream
    /// returned by GetUserMedia().
    /// </summary>
    public ref struct MediaStreamConstraints sealed
    {
      /// <summary>
      /// If set, it requests that the returned MediaStream contain a video
      /// track. If a Constraints structure is provided, it further specifies
      /// the nature and settings of the video Track. If not set, the
      /// MediaStream must not contain a video Track.
      /// </summary>
      property MediaTrackConstraints^ Video;
      /// <summary>
      /// If set, it requests that the returned MediaStream contain an audio
      /// track. If a Constraints structure is provided, it further specifies
      /// the nature and settings of the audio Track. If not set, the
      /// MediaStream must not contain an audio Track.
      /// </summary>
      property MediaTrackConstraints^ Audio;

      static MediaStreamConstraints^ create();
      static MediaStreamConstraints^ create(MediaStreamConstraints^ value);
    };

    /// <summary>
    /// Media device information representing the User Agent's available
    /// media input and output devices.
    /// </summary>
    public ref struct MediaDeviceInfo sealed
    {
      /// <summary>
      /// Describes the kind of the represented device.
      /// </summary>
      property MediaDeviceKind   Kind;

      /// <summary>
      /// A label describing this device (for example "External USB Webcam").
      /// If the device has no associated label, then this attribute must
      /// return the empty string.
      /// </summary>
      property Platform::String^ Label;

      /// <summary>
      /// A unique identifier for the represented device.
      /// </summary>
      property Platform::String^ DeviceId;

      /// <summary>
      /// Returns the group identifier of the represented device. Two devices
      /// have the same group identifier if they belong to the same physical
      /// device; for example a monitor with a built-in camera and microphone.
      /// </summary>
      property Platform::String^ GroupId;
    };

    public delegate void MediaDevicesChangeDelegate();

    public ref class MediaDevices sealed
    {
    private:
      friend class internal::MediaDevicesDelegate;

    public:
      ///<summary>
      /// Returns a dictionary whose members are the constrainable properties
      /// known to the User Agent. A supported constrainable property must be
      /// represented and any constrainable properties not supported by the
      /// User Agent must not be present in the returned dictionary. The
      /// values returned represent what the browser implements and will not
      /// change during a browsing session.
      ///</summary>
      ///<returns>MediaTrackSupportedConstraints</returns>
      static MediaTrackSupportedConstraints^ GetSupportedConstraints();

      /// <summary>
      /// Collects information about the User Agent's available media input
      /// and output devices.
      /// </summary>
      /// <returns>IAsyncOperation vector list of MediaDeviceInfo</returns>
      static IAsyncOperation<IVector<MediaDeviceInfo^>^>^ EnumerateDevices();

      /// <summary>
      /// Grant access to the user for permission to use their Web cam or
      /// other video or audio input.
      /// </summary>
      static IAsyncOperation<IVector<MediaStreamTrack^>^>^ GetUserMedia(MediaStreamConstraints^ constraints);

      /// <summary>
      /// Notify that media engine the application is being suspended to the
      /// background.
      /// </summary>
      static void OnAppSuspending();

      /// <summary>
      /// Notify that media engine the application orientation has changed.
      /// </summary>
      static void SetDisplayOrientation(
        Windows::Graphics::Display::DisplayOrientations display_orientation
        );

      /// <summary>
      /// The set of media devices, available to the User Agent, has changed.
      /// The current list devices can be retrieved with the
      /// EnumerateDevices() method.
      /// </summary>
      /// <see cref="EnumerateDevices" />
      event MediaDevicesChangeDelegate^   OnDeviceChange;
    };

  }
}
