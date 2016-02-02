#pragma once
#include <collection.h>
#include <MediaTypes.h>
#include <ortc/IMediaStreamTrack.h>

using namespace ortc;

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
  public enum class MediaStreamTrackState
  {
    Live,
    Ended
  };

  public enum class MediaStreamTrackKind
  {
    Audio,
    Video
  };
  public ref class MediaTrackCapabilities sealed
  {
  public:
    property LongRange^         Width;
    property LongRange^         Height;
    property DoubleRange^       AspectRatio;
    property DoubleRange^       FrameRate;
    property Platform::String^  FacingMode;
    property DoubleRange^       Volume;
    property LongRange^         SampleRate;
    property LongRange^         SampleSize;
    property Platform::Boolean  EchoCancellation;
    property Platform::String^  DeviceId;
    property Platform::String^  GroupId;
  };

  public ref class MediaTrackConstraintSet sealed
  {
  public:
    property ConstrainLong^       Width;
    property ConstrainLong^       Height;
    property ConstrainDouble^     AspectRatio;
    property ConstrainDouble^     FrameRate;
    property ConstrainString^     FacingMode;
    property ConstrainDouble^     Volume;
    property ConstrainLong^       SampleRate;
    property ConstrainLong^       SampleSize;
    property ConstrainBool^       EchoCancellation;
    property ConstrainString^     DeviceId;
    property ConstrainString^     GroupId;
  };

  public ref class MediaTrackConstraints sealed
  {
  public:
    property IVector<MediaTrackConstraintSet^>^ Advanced;
  };
  public ref class MediaTrackSettings sealed
  {
  public:
    property uint32             Width;
    property uint32             Height;
    property double             AspectRatio;
    property double             FrameRate;
    property Platform::String^  FacingMode;
    property double             Volume;
    property uint32             SampleRate;
    property uint32             SampleSize;
    property Platform::Boolean  EchoCancellation;
    property Platform::String^  DeviceId;
    property Platform::String^  GroupId;
  };

  public ref class MediaStreamError sealed
  {
  public:
    property Platform::String^ Name;
    property Platform::String^ Message;
    property Platform::String^ ConstraintName;
  };

  public ref class MediaStreamErrorEvent sealed {
  public:
    property MediaStreamError ^ Error //it should be readonly    
    {
      MediaStreamError ^  get() { return m_error; }
      void  set(MediaStreamError ^ value) { m_error = value; }
    }

  private:
    MediaStreamError ^ m_error;
  };

  public delegate void MediaStreamTrackMuteDelegate();
  public delegate void MediaStreamTrackUnMuteDelegate();
  public delegate void MediaStreamTrackEndedDelegate(MediaStreamErrorEvent^ evt);
  public delegate void MediaStreamTrackOverConstrainedDelegate(MediaStreamErrorEvent^ evt);

  /*

  event MediaStreamTrackEndedDelegate^       OnMediaStreamTrackEnded;
  event MediaStreamTrackOverConstrainedDelegate^*/
  public ref class MediaStreamTrack sealed
  {
    friend class ConvertObjectToCx;
    friend class FetchNativePointer;

  private:
    IMediaStreamTrackPtr mNativePointer;
  public:
    property MediaStreamTrackKind Kind
    {
      MediaStreamTrackKind  get();
    }

    property Platform::String^ Id
    {
      Platform::String^  get();
    }

    property Platform::String^ Label
    {
      Platform::String^  get();
    }

    property Platform::Boolean Enabled
    {
      Platform::Boolean  get();
      void  set(Platform::Boolean value);
    }

    property Platform::Boolean Muted
    {
      Platform::Boolean  get();
    }

    property Platform::Boolean ReadOnly
    {
      Platform::Boolean  get();
    }

    property Platform::Boolean Remote
    {
      Platform::Boolean  get();
    }

    property MediaStreamTrackState ReadyState
    {
      MediaStreamTrackState  get();
    }

    MediaStreamTrack^       Clone();
    void                    Stop();
    MediaTrackCapabilities^ GetCapabilities();
    MediaTrackConstraints^  GetConstraints();
    MediaTrackSettings^     GetSettings();

    event MediaStreamTrackMuteDelegate^             OnMediaStreamTrackMuted;
    event MediaStreamTrackUnMuteDelegate^           OnMediaStreamTrackUnMuted;
    event MediaStreamTrackEndedDelegate^            OnMediaStreamTrackEnded;
    event MediaStreamTrackOverConstrainedDelegate^  OnMediaStreamTrackOverConstrained;

  public:
    [Windows::Foundation::Metadata::OverloadAttribute("MediaStreamTrackStateToString")]
    static Platform::String^ ToString(MediaStreamTrackState value);
    [Windows::Foundation::Metadata::OverloadAttribute("MediaStreamTrackKindToString")]
    static Platform::String^ ToString(MediaStreamTrackKind value);

    static MediaStreamTrackState ToState(Platform::String^ str);
    static MediaStreamTrackKind  ToKind(Platform::String^ str);
    /*


    Promise<void>          applyConstraints (MediaTrackConstraints constraints);*/
  };
}

