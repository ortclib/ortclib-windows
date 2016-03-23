#pragma once
#include <collection.h>
#include <Capabilities.h>
#include <Contraints.h>
#include <ortc/IMediaStreamTrack.h>

using namespace ortc;

using Windows::Foundation::IAsyncAction;
using Windows::Foundation::Collections::IVector;
using Windows::Media::Core::IMediaSource;
using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_CLASS_PTR(MediaStreamTrackConstraintsPromiseObserver)

      public enum class MediaStreamTrackKind
    {
      Audio,
      Video
    };

    public enum class MediaStreamTrackState
    {
      Live,
      Ended
    };

    public ref class MediaTrackCapabilities sealed
    {
    public:
      property CapabilityLong^    Width;
      property CapabilityLong^    Height;
      property CapabilityDouble^  AspectRatio;
      property CapabilityDouble^  FrameRate;
      property CapabilityString^  FacingMode;
      property CapabilityDouble^  Volume;
      property CapabilityLong^    SampleRate;
      property CapabilityLong^    SampleSize;
      property CapabilityBool^    EchoCancellation;
      property CapabilityDouble^  Latency;

      property Platform::String^  DeviceId;
      property Platform::String^  GroupId;
    };

    public ref class MediaTrackSettings sealed
    {
    public:
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
      property Platform::String^                  DeviceId;
      property Platform::String^                  GroupId;
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
      property ConstrainDouble^     Latency;
      property ConstrainString^     DeviceId;
      property ConstrainString^     GroupId;
    };

    public ref class MediaTrackConstraints sealed
    {
    public:
      property IVector<MediaTrackConstraintSet^>^ Advanced;
    };

    public ref struct Constraints sealed
    {
      property MediaTrackConstraints^ Video;
      property MediaTrackConstraints^ Audio;

      static Constraints^ create();
      static Constraints^ create(Constraints^ value);
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

    class MediaStreamTrackConstraintsPromiseObserver : public zsLib::IPromiseResolutionDelegate
    {
    public:
      MediaStreamTrackConstraintsPromiseObserver(Concurrency::task_completion_event<void> tce);

      virtual void onPromiseResolved(PromisePtr promise);
      virtual void onPromiseRejected(PromisePtr promise);

    private:
      Concurrency::task_completion_event<void> mTce;
    };

    public ref class MediaStreamTrack sealed
    {
      friend class ConvertObjectToCx;
      friend class FetchNativePointer;
      friend class CallPrivateMethod;

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

      property Platform::String^ DeviceId
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
        void set(Platform::Boolean value);
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

      IMediaSource^ CreateMediaSource();

      IAsyncAction^ ApplyConstraints(MediaTrackConstraints^ constraints);

      event MediaStreamTrackMuteDelegate^             OnMediaStreamTrackMuted;
      event MediaStreamTrackUnMuteDelegate^           OnMediaStreamTrackUnMuted;
      event MediaStreamTrackEndedDelegate^            OnMediaStreamTrackEnded;
      event MediaStreamTrackOverConstrainedDelegate^  OnMediaStreamTrackOverConstrained;

    private:
      void SetMediaElement(void* element);

    public:
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      static Platform::String^ ToString();
      [Windows::Foundation::Metadata::OverloadAttribute("MediaStreamTrackStateToString")]
      static Platform::String^ ToString(MediaStreamTrackState value);
      [Windows::Foundation::Metadata::OverloadAttribute("MediaStreamTrackKindToString")]
      static Platform::String^ ToString(MediaStreamTrackKind value);

      static MediaStreamTrackState ToState(Platform::String^ str);
      static MediaStreamTrackKind  ToKind(Platform::String^ str);
    };
  }
}
