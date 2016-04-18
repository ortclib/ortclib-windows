#include "pch.h"

#include "Capabilities.h"
#include "Constraints.h"
#include "MediaStreamTrack.h"
#include "WebRtcMediaSource.h"
#include "helpers.h"

using Microsoft::WRL::ComPtr;
using Platform::Collections::Vector;

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    using std::make_shared;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackConstraintsPromiseObserver)

#pragma region Capabilities

      MediaTrackCapabilities^ ToCx(const IMediaStreamTrackTypes::Capabilities &input)
      {
        MediaTrackCapabilities^ result = ref new MediaTrackCapabilities;
        result->Width = ToCx(input.mWidth);
        result->Height = ToCx(input.mHeight);
        result->AspectRatio = ToCx(input.mAspectRatio);
        result->FrameRate = ToCx(input.mFrameRate);
        result->FacingMode = ToCx(input.mFacingMode);
        result->Volume = ToCx(input.mVolume);
        result->SampleRate = ToCx(input.mSampleRate);
        result->SampleSize = ToCx(input.mSampleSize);
        result->EchoCancellation = ToCx(input.mEchoCancellation);
        result->Latency = ToCx(input.mLatency);
        result->ChannelCount = ToCx(input.mChannelCount);

        result->DeviceId = Helper::ToCx(input.mDeviceID);
        result->GroupId = Helper::ToCx(input.mGroupID);
        return result;
      }

      MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaStreamTrackTypes::CapabilitiesPtr FromCx(MediaTrackCapabilities^ input)
      {
        if (nullptr == input) return IMediaStreamTrackTypes::CapabilitiesPtr();
        auto result(make_shared<IMediaStreamTrackTypes::Capabilities>());
        result->mWidth = FromCx(input->Width);
        result->mHeight = FromCx(input->Height);
        result->mAspectRatio = FromCx(input->AspectRatio);
        result->mFrameRate = FromCx(input->FrameRate);
        result->mFacingMode = FromCx(input->FacingMode);
        result->mVolume = FromCx(input->Volume);
        result->mSampleRate = FromCx(input->SampleRate);
        result->mSampleSize = FromCx(input->SampleSize);
        result->mEchoCancellation = FromCx(input->EchoCancellation);
        result->mLatency = FromCx(input->Latency);
        result->mChannelCount = FromCx(input->ChannelCount);

        result->mDeviceID = Helper::FromCx(input->DeviceId);
        result->mGroupID = Helper::FromCx(input->GroupId);
        return result;
      }

#pragma endregion

#pragma region Constraints

      MediaTrackConstraints^ ToCx(const IMediaStreamTrackTypes::TrackConstraints &input)
      {
        auto result = ref new MediaTrackConstraints();
        if (input.mAdvanced.size() < 1) return result;

        result->Advanced = ref new Vector<MediaTrackConstraintSet^>();
        for (auto iter = input.mAdvanced.begin(); iter != input.mAdvanced.end(); ++iter)
        {
          auto &value = *iter;
          result->Advanced->Append(ToCx(value));
        }
        return result;
      }

      MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaStreamTrackTypes::TrackConstraintsPtr FromCx(MediaTrackConstraints^ input)
      {
        if (nullptr == input) return IMediaStreamTrackTypes::TrackConstraintsPtr();
        auto result(make_shared<IMediaStreamTrackTypes::TrackConstraints>());
        if (nullptr == input->Advanced) return result;

        for (MediaTrackConstraintSet^ value : input->Advanced)
        {
          result->mAdvanced.push_back(FromCx(value));
        }
        return result;
      }

      MediaTrackConstraintSet^ ToCx(const IMediaStreamTrackTypes::ConstraintSet &input)
      {
        auto result = ref new MediaTrackConstraintSet();
        result->Width = ToCx(input.mWidth);
        result->Height = ToCx(input.mHeight);
        result->AspectRatio = ToCx(input.mAspectRatio);
        result->FrameRate = ToCx(input.mFrameRate);
        result->FacingMode = ToCx(input.mFacingMode);
        result->Volume = ToCx(input.mVolume);
        result->SampleRate = ToCx(input.mSampleRate);
        result->SampleSize = ToCx(input.mSampleSize);
        result->EchoCancellation = ToCx(input.mEchoCancellation);
        result->Latency = ToCx(input.mLatency);
        result->ChannelCount = ToCx(input.mChannelCount);
        result->DeviceId = ToCx(input.mDeviceID);
        result->GroupId = ToCx(input.mGroupID);
        return result;
      }

      MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaStreamTrackTypes::ConstraintSetPtr FromCx(MediaTrackConstraintSet^ input)
      {
        if (nullptr == input) return IMediaStreamTrackTypes::ConstraintSetPtr();
        auto result(make_shared<IMediaStreamTrackTypes::ConstraintSet>());
        result->mWidth = FromCx(input->Width);
        result->mHeight = FromCx(input->Height);
        result->mAspectRatio = FromCx(input->AspectRatio);
        result->mFrameRate = FromCx(input->FrameRate);
        result->mFacingMode = FromCx(input->FacingMode);
        result->mVolume = FromCx(input->Volume);
        result->mSampleRate = FromCx(input->SampleRate);
        result->mSampleSize = FromCx(input->SampleSize);
        result->mEchoCancellation = FromCx(input->EchoCancellation);
        result->mLatency = FromCx(input->Latency);
        result->mChannelCount = FromCx(input->ChannelCount);
        result->mDeviceID = FromCx(input->DeviceId);
        result->mGroupID = FromCx(input->GroupId);
        return result;
      }

#pragma endregion

      MediaTrackSettings^ ToCx(const IMediaStreamTrackTypes::Settings &input)
      {
        auto result = ref new MediaTrackSettings();
        result->Width = Helper::ToCx(input.mWidth);
        result->Height = Helper::ToCx(input.mHeight);
        result->AspectRatio = Helper::ToCx(input.mAspectRatio);
        result->FrameRate = Helper::ToCx(input.mFrameRate);
        result->FacingMode = Helper::ToCx(input.mFacingMode);
        result->Volume = Helper::ToCx(input.mVolume);
        result->SampleRate = Helper::ToCx(input.mSampleRate);
        result->SampleSize = Helper::ToCx(input.mSampleSize);
        result->EchoCancellation = Helper::ToCx(input.mEchoCancellation);
        result->Latency = Helper::ToCx(input.mLatency);
        result->ChannelCount = Helper::ToCx(input.mChannelCount);
        result->DeviceId = Helper::ToCx(input.mDeviceID);
        result->GroupId = Helper::ToCx(input.mGroupID);
        return result;
      }

      MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaStreamTrackTypes::SettingsPtr FromCx(MediaTrackSettings^ input)
      {
        if (nullptr == input) return IMediaStreamTrackTypes::SettingsPtr();
        auto result(make_shared<IMediaStreamTrackTypes::Settings>());
        result->mWidth = Helper::FromCx(input->Width);
        result->mHeight = Helper::FromCx(input->Height);
        result->mAspectRatio = Helper::FromCx(input->AspectRatio);
        result->mFrameRate = Helper::FromCx(input->FrameRate);
        result->mFacingMode = Helper::FromCx(input->FacingMode);
        result->mVolume = Helper::FromCx(input->Volume);
        result->mSampleRate = Helper::FromCx(input->SampleRate);
        result->mSampleSize = Helper::FromCx(input->SampleSize);
        result->mEchoCancellation = Helper::FromCx(input->EchoCancellation);
        result->mLatency = Helper::FromCx(input->Latency);
        result->mChannelCount = Helper::FromCx(input->ChannelCount);
        result->mDeviceID = Helper::FromCx(input->DeviceId);
        result->mGroupID = Helper::FromCx(input->GroupId);
        return result;
      }

      class MediaStreamTrackConstraintsPromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        MediaStreamTrackConstraintsPromiseObserver(Concurrency::task_completion_event<void> tce);

        virtual void onPromiseResolved(PromisePtr promise);
        virtual void onPromiseRejected(PromisePtr promise);

      private:
        Concurrency::task_completion_event<void> mTce;
      };

      MediaStreamTrackConstraintsPromiseObserver::MediaStreamTrackConstraintsPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce)
      {
      }

      void MediaStreamTrackConstraintsPromiseObserver::onPromiseResolved(PromisePtr promise)
      {
        mTce.set();
      }

      void MediaStreamTrackConstraintsPromiseObserver::onPromiseRejected(PromisePtr promise)
      {
      }

    } // namespace internal

    MediaStreamTrack^ MediaStreamTrack::Convert(IMediaStreamTrackPtr track)
    {
      auto result = ref new MediaStreamTrack();
      result->_nativePointer = track;
      return result;
    }

    MediaStreamTrackKind  MediaStreamTrack::Kind::get()
    {
      return UseHelper::convert(_nativePointer->kind());
    }

    Platform::String^ MediaStreamTrack::Id::get()
    {
      return UseHelper::ToCx(_nativePointer->id());
    }

    Platform::String^ MediaStreamTrack::DeviceId::get()
    {
      return UseHelper::ToCx(_nativePointer->deviceID());
    }

    Platform::String^ MediaStreamTrack::Label::get()
    {
      return UseHelper::ToCx(_nativePointer->label());
    }

    Platform::Boolean MediaStreamTrack::Enabled::get()
    {
      return _nativePointer->enabled();
    }

    void MediaStreamTrack::Enabled::set(Platform::Boolean value)
    {
      _nativePointer->enabled(value);
    }

    Platform::Boolean MediaStreamTrack::Muted::get()
    {
      return _nativePointer->muted();
    }

    void MediaStreamTrack::Muted::set(Platform::Boolean value)
    {
      return _nativePointer->muted(value);
    }

    Platform::Boolean MediaStreamTrack::ReadOnly::get()
    {
      return _nativePointer->readOnly();
    }

    Platform::Boolean MediaStreamTrack::Remote::get()
    {
      return _nativePointer->remote();
    }

    MediaStreamTrackState MediaStreamTrack::ReadyState::get()
    {
      return UseHelper::convert(_nativePointer->readyState());
    }

    MediaStreamTrack^ MediaStreamTrack::Clone()
    {
      if (_nativePointer)
      {
        IMediaStreamTrackPtr newNativePointer = _nativePointer->clone();
        if (newNativePointer)
        {
          auto ret = ref new MediaStreamTrack();
          ret->_nativePointer = newNativePointer;
          return ret;
        }
      }

      return nullptr;
    }
    void MediaStreamTrack::Stop()
    {
      if (_nativePointer)
        _nativePointer->stop();
    }

    MediaTrackCapabilities^		MediaStreamTrack::GetCapabilities()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->getCapabilities());
      }
      return nullptr;
    }

    MediaTrackConstraints^		MediaStreamTrack::GetConstraints()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->getConstraints());
      }
      return nullptr;
    }

    MediaTrackSettings^			MediaStreamTrack::GetSettings()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->getSettings());
      }
      return nullptr;
    }

    IMediaSource^ MediaStreamTrack::CreateMediaSource()
    {
      //return globals::RunOnGlobalThread<IMediaSource^>([track, id]() -> IMediaSource^ {
      //  ComPtr<ABI::Windows::Media::Core::IMediaSource> comSource;
      //  org::ortc::WebRtcMediaSource::CreateMediaSource(&comSource, track, id);
      //  IMediaSource^ source = reinterpret_cast<IMediaSource^>(comSource.Get());
      //  return source;
      //});

#ifdef USE_OLD_RENDERER
      Platform::String^ id = "stream";
      uint32 framerate = 30;
      return RTMediaStreamSource::CreateMediaSource(this, framerate, id);
#else
      Platform::String^ id = "stream";
      ComPtr<ABI::Windows::Media::Core::IMediaSource> comSource;
      WebRtcMediaSource::CreateMediaSource(&comSource, this, id);
      IMediaSource^ source = reinterpret_cast<IMediaSource^>(comSource.Get());
      return source;
#endif
    }

    IAsyncAction^ MediaStreamTrack::ApplyConstraints(MediaTrackConstraints^ constraints)
    {
      IAsyncAction^ ret = Concurrency::create_async([this, constraints]()
      {
        Concurrency::task_completion_event<void> tce;

        if (constraints == nullptr || _nativePointer == nullptr)
        {
          tce.set();
          return;
        }

        PromisePtr promise = _nativePointer->applyConstraints(*internal::FromCx(constraints));
        internal::MediaStreamTrackConstraintsPromiseObserverPtr pDelegate(make_shared<internal::MediaStreamTrackConstraintsPromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

    void MediaStreamTrack::SetMediaElement(void* element)
    {
      if (_nativePointer)
      {
        _nativePointer->setMediaElement(element);
      }
    }

    Platform::String^ MediaStreamTrack::ToString()
    {
      throw ref new Platform::NotImplementedException();
    }

    Platform::String^ MediaStreamTrack::ToString(MediaStreamTrackState value)
    {
      return UseHelper::ToCx(IMediaStreamTrack::toString(UseHelper::convert(value)));
    }

    Platform::String^ MediaStreamTrack::ToString(MediaStreamTrackKind value)
    {
      return UseHelper::ToCx(IMediaStreamTrack::toString(UseHelper::convert(value)));
    }

    MediaStreamTrackState MediaStreamTrack::ToState(Platform::String^ str)
    {
      return UseHelper::convert(IMediaStreamTrack::toState(UseHelper::FromCx(str).c_str()));
    }

    MediaStreamTrackKind MediaStreamTrack::ToKind(Platform::String^ str)
    {
      return UseHelper::convert(IMediaStreamTrack::toKind(UseHelper::FromCx(str).c_str()));
    }

  } // namespace ortc
} // namespace org
