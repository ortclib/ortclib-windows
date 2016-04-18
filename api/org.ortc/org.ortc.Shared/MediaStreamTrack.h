#pragma once

#include <ortc/IMediaStreamTrack.h>

#include <zsLib/types.h>

namespace org
{
  namespace ortc
  {
    using Windows::Media::Core::IMediaSource;
    using Windows::Foundation::IAsyncAction;
    using Windows::Foundation::Collections::IVector;

    ref struct Error;
    ref struct ErrorEvent;

    ref struct CapabilityBoolean;
    ref struct CapabilityLong;
    ref struct CapabilityDouble;
    ref struct CapabilityString;

    ref struct ConstrainBoolean;
    ref struct ConstrainDouble;
    ref struct ConstrainLong;
    ref struct ConstrainString;

    ref struct ConstrainBoolParameters;
    ref struct MediaStreamConstraints;

    ref struct MediaTrackConstraints;
    ref struct MediaTrackCapabilities;
    ref struct MediaTrackSettings;
    ref struct MediaTrackConstraintSet;

    ref class MediaStreamTrack;
    ref class RTMediaStreamSource;
    ref class RTCRtpReceiver;
    ref class RTCRtpSender;

    ZS_DECLARE_CLASS_PTR(WebRtcMediaStream)

    ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IMediaStreamTrack, IMediaStreamTrack)
    ZS_DECLARE_TYPEDEF_PTR(zsLib::Any, Any)

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackDelegate)
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackPromiseObserver)
      ZS_DECLARE_CLASS_PTR(MediaDevicesPromiseObserver)
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackConstraintsPromiseObserver)
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IMediaStreamTrackTypes, IMediaStreamTrackTypes)

      MediaTrackCapabilities^ ToCx(const IMediaStreamTrackTypes::Capabilities &input);
      MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr input);
      IMediaStreamTrackTypes::CapabilitiesPtr FromCx(MediaTrackCapabilities^ input);

      MediaTrackConstraints^ ToCx(const IMediaStreamTrackTypes::TrackConstraints &input);
      MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr input);

      IMediaStreamTrackTypes::TrackConstraintsPtr FromCx(MediaTrackConstraints^ input);
      MediaTrackConstraintSet^ ToCx(const IMediaStreamTrackTypes::ConstraintSet &input);

      MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr input);
      IMediaStreamTrackTypes::ConstraintSetPtr FromCx(MediaTrackConstraintSet^ input);

      MediaTrackSettings^ ToCx(const IMediaStreamTrackTypes::Settings &input);
      MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr input);
      IMediaStreamTrackTypes::SettingsPtr FromCx(MediaTrackSettings^ input);
    }

    /// <summary>
    /// The kind attribute must represent either an audio or video track.
    /// </summary>
    public enum class MediaStreamTrackKind
    {
      /// <summary>
      /// This object represents an audio track.
      /// </summary>
      Audio,
      /// <summary>
      /// This object represents an video track.
      /// </summary>
      Video
    };

    /// <summary>
    /// The state attribute represents the state of the track. 
    /// </summary>
    public enum class MediaStreamTrackState
    {
      /// <summary>
      /// The track is active (the track's underlying media source is making a
      /// best-effort attempt to provide data in real time).
      ///
      /// The output of a track in the live state can be switched on and off
      /// with the enabled attribute.
      /// </summary>
      Live,
      /// <summary>
      /// The track has ended (the track's underlying media source is no longer
      /// providing data, and will never provide more data for this track).
      /// Once a track enters this state, it never exits it.
      ///
      /// For example, a video track ends when the user unplugs the USB web
      /// camera that acts as the track's media source.
      /// </summary>
      Ended
    };

    public enum class VideoFacingModeEnum
    {
      /// <summary>
      /// The source is facing toward the user (a self-view camera).
      /// </summary>
      User,
      /// <summary>
      /// The source is facing away from the user (viewing the environment).
      /// </summary>
      Environment,
      /// <summary>
      /// The source is facing to the left of the user.
      /// </summary>
      Left,
      /// <summary>
      /// The source is facing to the right of the user.
      /// </summary>
      Right
    };

    /// <summary>
    /// The type of the source of the MediaStreamTrack.
    /// </summary>
    public enum class SourceTypeEnum
    {
      /// <summary>
      /// A valid source type only for video MediaStreamTrack s. The source is
      /// a local video-producing camera source.
      /// </summary>
      Camera,
      /// <summary>
      /// A valid source type only for audio MediaStreamTrack s. The source is
      /// a local audio-producing microphone source.
      /// </summary>
      Microphone
    };

    /// <summary>
    /// Capabilities is a dictionary containing one or more key-value pairs,
    /// where each key must be a constrainable property, and each value must
    /// be a subset of the set of values allowed for that property. The exact
    /// type of the value expression depends on the type of the property.
    /// The Capabilities dictionary specifies which constrainable properties
    /// that can be applied, as constraints, to the constrainable object. 
    /// </summary>
    public ref struct MediaTrackCapabilities sealed
    {
      property CapabilityLong^    Width;
      property CapabilityLong^    Height;
      property CapabilityDouble^  AspectRatio;
      property CapabilityDouble^  FrameRate;
      /// <summary>
      /// A camera can report multiple facing modes. For example, in a
      /// high-end telepresence solution with several cameras facing the user,
      /// a camera to the left of the user can report both "left" and "user".
      /// </summary>
      property CapabilityString^  FacingMode;
      property CapabilityDouble^  Volume;
      property CapabilityLong^    SampleRate;
      property CapabilityLong^    SampleSize;
      /// <summary>
      /// If the source cannot do echo cancellation a single false is
      /// reported. If echo cancellation cannot be turned off, a single true
      /// is reported. If the script can control the feature, the source
      /// reports a list with both true and false as possible values.
      /// </summary>
      property CapabilityBoolean^ EchoCancellation;
      property CapabilityDouble^  Latency;
      property CapabilityLong^    ChannelCount;

      property Platform::String^  DeviceId;
      property Platform::String^  GroupId;
    };

    /// <summary>
    /// Settings is a dictionary containing one or more key-value pairs.
    /// It must contain each key returned in GetCapabilities(). There must be
    /// a single value for each key and the value must be a member of the set
    /// defined for that property by getCapabilities(). The Settings
    /// dictionary contains the actual values that the User Agent has chosen
    /// for the object's constrainable properties. The exact syntax of the
    /// value depends on the type of the property.
    /// </summary>
    public ref struct MediaTrackSettings sealed
    {
      property Platform::IBox<int32>^             Width;
      property Platform::IBox<int32>^             Height;
      property Platform::IBox<float64>^           AspectRatio;
      property Platform::IBox<float64>^           FrameRate;
      property Platform::String^                  FacingMode;
      property Platform::IBox<float64>^           Volume;
      property Platform::IBox<int32>^             SampleRate;
      property Platform::IBox<int32>^             SampleSize;
      property Platform::IBox<Platform::Boolean>^ EchoCancellation;
      property Platform::IBox<float64>^           Latency;
      property Platform::IBox<int32>^             ChannelCount;
      property Platform::String^                  DeviceId;
      property Platform::String^                  GroupId;
    };

    /// <summary>
    /// Each member of a ConstraintSet corresponds to a constrainable property
    /// and specifies a subset of the property's legal Capability values.
    /// Applying a ConstraintSet instructs the User Agent to restrict the
    /// settings of the corresponding constrainable properties to the
    /// specified values or ranges of values. A given property may occur both
    /// in the basic Constraints set and in the advanced ConstraintSets list,
    /// and may occur at most once in each ConstraintSet in the advanced list.
    /// </summary>
    public ref struct MediaTrackConstraintSet sealed
    {
      /// <summary>
      /// The width or width range, in pixels. As a capability, the range
      /// should span the video source's pre-set width values with min being
      /// the smallest width and max being the largest width.
      /// </summary>
      property ConstrainLong^       Width;
      /// <summary>
      /// The height or height range, in pixels. As a capability, the range
      /// should span the video source's pre-set height values with min being
      /// the smallest height and max being the largest height.
      /// </summary>
      property ConstrainLong^       Height;
      /// <summary>
      /// The exact aspect ratio (width in pixels divided by height in pixels,
      /// represented as a double rounded to the tenth decimal place) or
      /// aspect ratio range.
      /// </summary>
      property ConstrainDouble^     AspectRatio;
      /// <summary>
      /// The exact frame rate (frames per second) or frame rate range. If
      /// this frame rate cannot be determined (e.g. the source does not
      /// natively provide a frame rate, or the frame rate cannot be
      /// determined from the source stream), then this value must refer to
      /// the User Agent's vsync display rate.
      /// </summary>
      property ConstrainDouble^     FrameRate;
      /// <summary>
      /// This string (or each string, when a list) should be one of the
      /// members of VideoFacingModeEnum. The members describe the directions
      /// that the camera can face, as seen from the user's perspective. Note
      /// that getConstraints may not return exactly the same string for
      /// strings not in this enum. This preserves the possibility of using a
      /// future version of WebIDL enum for this property.
      /// </summary>
      property ConstrainString^     FacingMode;
      /// <summary>
      /// The volume or volume range, as a multiplier of the linear audio
      /// sample values. A volume of 0.0 is silence, while a volume of 1.0 is
      /// the maximum supported volume. A volume of 0.5 will result in an
      /// approximately 6 dBSPL change in the sound pressure level from the
      /// maximum volume. Note that any ConstraintSet that specifies values
      /// outside of this range of 0 to 1 can never be satisfied.
      /// </summary>
      property ConstrainDouble^     Volume;
      /// <summary>
      /// The sample rate in samples per second for the audio data.
      /// </summary>
      property ConstrainLong^       SampleRate;
      /// <summary>
      /// The linear sample size in bits. This constraint can only be
      /// satisfied for audio devices that produce linear samples.
      /// </summary>
      property ConstrainLong^       SampleSize;
      /// <summary>
      /// When one or more audio streams is being played in the processes of
      /// various microphones, it is often desirable to attempt to remove the
      /// sound being played from the input signals recorded by the
      /// microphones. This is referred to as echo cancellation. There are
      /// cases where it is not needed and it is desirable to turn it off so
      /// that no audio artifacts are introduced. This allows applications to
      /// control this behavior.
      /// </summary>
      property ConstrainBoolean^    EchoCancellation;
      /// <summary>
      /// The latency or latency range, in seconds. The latency is the time
      /// between start of processing (for instance, when sound occurs in the
      /// real world) to the data being available to the next step in the
      /// process. Low latency is critical for some applications; high latency
      /// may be acceptable for other applications because it helps with power
      /// constraints. The number is expected to be the target latency of the
      /// configuration; the actual latency may show some variation from that.
      /// </summary>
      property ConstrainDouble^     Latency;
      /// <summary>
      /// The number of independent channels of sound that the audio data
      /// contains, i.e. the number of audio samples per sample frame.
      /// </summary>
      property ConstrainLong^       ChannelCount;
      /// <summary>
      /// The origin-unique identifier for the source of the MediaStreamTrack.
      /// The same identifier must be valid between browsing sessions of this
      /// origin, but must also be different for other origins. Some sort of
      /// GUID is recommended for the identifier. Note that the setting of
      /// this property is uniquely determined by the source that is attached
      /// to the MediaStreamTrack. In particular, getCapabilities() will
      /// return only a single value for deviceId. This property can therefore
      /// be used for initial media selection with getUserMedia(). However,
      /// it is not useful for subsequent media control with
      /// applyConstraints(), since any attempt to set a different value will
      /// result in an unsatisfiable ConstraintSet.
      /// </summary>
      property ConstrainString^     DeviceId;
      /// <summary>
      /// The browsing session-unique group identifier for the source of the
      /// MediaStreamTrack. Two devices have the same group identifier if they
      /// belong to the same physical device; for example, the audio input and
      /// output devices representing the speaker and microphone of the same
      /// headset would have the same groupId. Note that the setting of this
      /// property is uniquely determined by the source that is attached to
      /// the MediaStreamTrack. In particular, getCapabilities() will return
      /// only a single value for groupId. Since this property is not stable 
      /// between browsing sessions its usefulness for initial media selection
      /// with getUserMedia() is limited. It is not useful for subsequent
      /// media control with applyConstraints(), since any attempt to set a
      /// different value will result in an unsatisfiable ConstraintSet.
      /// </summary>
      property ConstrainString^     GroupId;
    };

    public ref struct MediaTrackConstraints sealed
    {
      /// <summary>
      /// This is the list of ConstraintSets that the User Agent must attempt
      /// to satisfy, in order, skipping only those that cannot be satisfied.
      /// The order of these ConstraintSets is significant. In particular,
      /// when they are passed as an argument to applyConstraints, the User
      /// Agent must try to satisfy them in the order that is specified. Thus
      /// if optional ConstraintSets C1 and C2 can be satisfied individually,
      /// but not together, then whichever of C1 and C2 is first in this list
      /// will be satisfied, and the other will not. The User Agent must
      /// attempt to satisfy all optional ConstraintSets in the list, even if
      /// some cannot be satisfied. Thus, in the preceding example, if
      /// optional constraint C3 is specified after C1 and C2, the User Agent
      /// will attempt to satisfy C3 even though C2 cannot be satisfied. Note
      /// that a given property name may occur only once in each ConstraintSet
      /// but may occur in more than one ConstraintSet.
      /// </summary>
      property IVector<MediaTrackConstraintSet^>^ Advanced;
    };

    /// <summary>
    /// The constraint attribute is set to the name of any one of the
    /// constraints that caused the error, if any.
    /// </summary>
    public ref struct OverconstrainedError sealed
    {
    private:
      friend class internal::MediaStreamTrackDelegate;
      friend class internal::MediaDevicesPromiseObserver;
      friend class internal::MediaStreamTrackConstraintsPromiseObserver;

      static OverconstrainedError^ CreateIfOverconstrainedError(AnyPtr any);

    public:
      /// <summary>
      /// Gets a string representing one of the error type names.
      /// </summary>
      property Platform::String^ Name;
      /// <summary>
      /// The constraint property name where the error originated.
      /// </summary>
      property Platform::String^ Constraint;
      /// <summary>
      /// A message describing the error condition for the constraint.
      /// </summary>
      property Platform::String^ Message;
    };

    /// <summary>
    /// This error event fires for each affected track (when multiple tracks
    /// share the same source) after the User Agent has evaluated the current
    /// constraints against a given source and is not able to configure the
    /// source within the limitations established by the intersection of
    /// imposed constraints.
    ///
    /// Due to being over - constrained, the User Agent must mute each
    /// affected track.
    ///
    /// The affected track(s) will remain muted until the application adjusts
    /// the constraints to accommodate the source's current effective
    /// capabilities.
    /// </summary>
    public ref struct OverconstrainedErrorEvent sealed
    {
    private:
      friend class internal::MediaStreamTrackDelegate;

      OverconstrainedErrorEvent(OverconstrainedError^ error)
      {
        _error = error;
      }

    public:
      /// <summary>
      /// Gets the OverconstrainedError describing the error associated with
      /// the event (if any).
      /// </summary>
      property OverconstrainedError ^ Error
      {
        OverconstrainedError^ get() { return _error; }
      }

    private:
      OverconstrainedError^ _error;
    };

    public delegate void MediaStreamTrackMuteDelegate();
    public delegate void MediaStreamTrackUnMuteDelegate();
    public delegate void MediaStreamTrackOverConstrainedDelegate(OverconstrainedErrorEvent^ evt);
    public delegate void MediaStreamTrackEndedDelegate(ErrorEvent^ evt);

    /// <summary>
    /// A MediaStreamTrack object represents a media source in the User Agent.
    /// An example source is a device connected to the User Agent. Other
    /// specifications may define sources for MediaStreamTrack that override
    //// the behavior specified here. Several MediaStreamTrack objects can
    //// represent the same media source, e.g., when the user chooses the same
    //// camera in the UI shown by two consecutive calls to getUserMedia() .
    /// </summary>
    public ref class MediaStreamTrack sealed
    {
    private:
      friend class internal::MediaStreamTrackDelegate;
      friend class internal::MediaStreamTrackPromiseObserver;
      friend class WebRtcMediaStream;
      friend ref class RTMediaStreamSource;
      friend ref class RTCRtpReceiver;
      friend ref class RTCRtpSender;

    private:
      static MediaStreamTrack^ Convert(IMediaStreamTrackPtr track);
      static IMediaStreamTrackPtr Convert(MediaStreamTrack^ track) { if (!track) return nullptr; return track->_nativePointer; }

      void SetupDelegate();

      void SetMediaElement(void* element);

    public:
      /// <summary>
      /// Gets a kind attribute that must return the string "Audio" if this
      /// object represents an audio track or "Video" if this object
      /// represents a video track.
      /// </summary>
      property MediaStreamTrackKind Kind
      {
        MediaStreamTrackKind  get();
      }

      /// <summary>
      /// Gets a User Agent generated an identifier string.
      /// </summary>
      property Platform::String^ Id
      {
        Platform::String^  get();
      }

      /// <summary>
      /// Gets a unique identifier for the represented device.
      /// </summary>
      property Platform::String^ DeviceId
      {
        Platform::String^  get();
      }

      /// <summary>
      /// Gets the label of the object's corresponding source, if any. If
      /// the corresponding source has or had no label, the attribute must
      /// instead return the empty string..
      /// User Agents may label audio and video sources (e.g., "Internal
      /// microphone" or "External USB Webcam").
      /// </summary>
      property Platform::String^ Label
      {
        Platform::String^  get();
      }

      /// <summary>
      /// Gets or sets the enabled state for the object. On getting, the
      /// attribute must return the value to which it was last set. On
      /// setting, it must be set to the new value.
      /// </summary>
      property Platform::Boolean Enabled
      {
        Platform::Boolean  get();
        void  set(Platform::Boolean value);
      }

      /// <summary>
      /// Gets or sets the muted state of the object.
      /// </summary>
      /// <returns>true if the track is muted, and false otherwise.</returns>
      property Platform::Boolean Muted
      {
        Platform::Boolean  get();
        void set(Platform::Boolean value);
      }

      /// <summary>
      /// Gets if the track source origin is local or remote.
      /// </summary>
      /// <returns>true if the source origin is remote, otherwise false</returns>
      property Platform::Boolean Remote
      {
        Platform::Boolean  get();
      }

      /// <summary>
      /// Gets the state of the track.
      /// </summary>
      /// <returns>The value as most recently set by the User Agent.</returns>
      property MediaStreamTrackState ReadyState
      {
        MediaStreamTrackState  get();
      }

      /// <summary>
      /// Clones this MediaStreamTrack.
      /// </summary>
      MediaStreamTrack^       Clone();
      /// When a MediaStreamTrack object's stop() method is invoked, the User Agent must run following steps:
      /// 1. Let track be the current MediaStreamTrack object.
      /// 2. Notify track's source that track is ended.
      ///    A source that is notified of a track ending will be stopped,
      ///    unless other MediaStreamTrack objects depend on it
      /// 3. Set track's readyState attribute to ended.
      void                    Stop();
      /// <summary>
      /// Returns the dictionary of the names of the constrainable propertie
      /// that the object supports.
      /// </summary>
      MediaTrackCapabilities^ GetCapabilities();
      /// <summary>
      /// Returns the Constraints that were the argument to the most recent
      /// successful call of ApplyConstraints(), maintaining the order in
      /// which they were specified. Note that some of the optional
      /// ConstraintSets returned may not be currently satisfied. To check
      /// which ConstraintSets are currently in effect, the application should
      /// use GetSettings.
      /// </summary>
      MediaTrackConstraints^  GetConstraints();
      /// <summary>
      /// Returns the current settings of all the constrainable properties of
      /// the object, whether they are platform defaults or have been set by
      /// ApplyConstraints(). Note that the actual setting of a property must
      /// be a single value.
      /// </summary>
      MediaTrackSettings^     GetSettings();

      /// <summary>
      /// Create a IMediaSource capable of being rendered to an output
      /// window based upon the current track.
      /// </summary>
      IMediaSource^ CreateMediaSource();

      /// <summary>
      /// The User Agent may choose new settings for the constrainable
      /// properties of the object at any time. When it does so it must
      /// attempt to satisfy the current Constraints
      /// </summary>
      IAsyncAction^ ApplyConstraints(MediaTrackConstraints^ constraints);

      /// <summary>
      /// The MediaStreamTrack object's source is temporarily unable to
      /// provide data.
      /// </summary>
      event MediaStreamTrackMuteDelegate^             OnMute;
      /// <summary>
      /// The MediaStreamTrack object's source is live again after having been
      /// temporarily unable to provide data.
      /// </summary>
      event MediaStreamTrackUnMuteDelegate^           OnUnmuted;
      /// <summary>
      /// The MediaStreamTrack object's source will no longer provide any
      /// data, either because the user revoked the permissions, or because
      /// the source device has been ejected, or because the remote peer
      /// permanently stopped sending data.
      /// </summary>
      event MediaStreamTrackEndedDelegate^            OnEnded;
      /// <summary>
      /// This error event fires for each affected track(when multiple tracks
      /// share the same source) after the User Agent has evaluated the
      /// current constraints against a given source and is not able to
      /// configure the source within the limitations established by the
      /// intersection of imposed constraints.
      ///
      /// Due to being over - constrained, the User Agent must mute each
      /// affected track.
      ///
      /// The affected track(s) will remain muted until the application
      /// adjusts the constraints to accommodate the source's current
      /// effective capabilities.
      /// </summary>
      event MediaStreamTrackOverConstrainedDelegate^  OnOverconstrained;

    private:
      IMediaStreamTrackPtr _nativePointer;
      internal::MediaStreamTrackDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
