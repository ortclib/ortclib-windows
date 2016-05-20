#include "pch.h"

#include "MediaStreamTrack.h"
#include "Capabilities.h"
#include "Constraints.h"
#include "WebRtcMediaSource.h"
#include "helpers.h"
#include "Error.h"

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

        const IMediaStreamTrackTypes::ConstraintSet &baseSet = (input);
        result->Set = ToCx(baseSet);
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

        IMediaStreamTrackTypes::ConstraintSet &baseSet = *result;
        auto baseResult = FromCx(input->Set);
        if (baseResult) {
          baseSet = *baseResult;
        }

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

#pragma region Settings

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

#pragma endregion

#pragma region MediaStreamTrack delegates

      class MediaStreamTrackDelegate : public IMediaStreamTrackDelegate
      {
      public:
        MediaStreamTrackDelegate(MediaStreamTrack^ owner) { _owner = owner; }

        virtual void onMediaStreamTrackMute(
                                            IMediaStreamTrackPtr track,
                                            bool isMuted
                                            ) override
        {
          if (!_owner) return;
          if (isMuted)
          {
            _owner->OnMute();
          }
          else
          {
            _owner->OnUnmuted();
          }
        }

        virtual void onMediaStreamTrackEnded(IMediaStreamTrackPtr track) override
        {
          if (!_owner) return;

          Error^ error = ref new Error;
          error->Name = "Ended";
          ErrorEvent^ evt = ref new ErrorEvent(error);
          _owner->OnEnded(evt);
        }

        virtual void onMediaStreamTrackOverConstrained(
                                                       IMediaStreamTrackPtr track,
                                                       IMediaStreamTrackTypes::OverconstrainedErrorPtr inError
                                                       ) override
        {
          if (!_owner) return;

          auto error = org::ortc::OverconstrainedError::CreateIfOverconstrainedError(inError);
          OverconstrainedErrorEvent ^evt = ref new OverconstrainedErrorEvent(error);

          _owner->OnOverconstrained(evt);
        }

      private:
        MediaStreamTrack^ _owner;
      };

#pragma endregion

#pragma region MediaStreamTrack observers

      class MediaStreamTrackConstraintsPromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        MediaStreamTrackConstraintsPromiseObserver(Concurrency::task_completion_event<void> tce) : mTce(tce) {}

        virtual void onPromiseResolved(PromisePtr promise) override
        {
          mTce.set();
        }

        virtual void onPromiseRejected(PromisePtr promise) override
        {
          auto reason = promise->reason<Any>();
          auto overError = OverconstrainedError::CreateIfOverconstrainedError(reason);
          if (nullptr != overError)
          {
            mTce.set_exception(overError);
            return;
          }

          auto error = Error::CreateIfGeneric(reason);
          mTce.set_exception(error);
        }

      private:
        Concurrency::task_completion_event<void> mTce;
      };

#pragma endregion

    } // namespace internal

#pragma region MediaStreamTrack errors

    OverconstrainedError^ OverconstrainedError::CreateIfOverconstrainedError(AnyPtr any)
    {
      if (!any) return nullptr;

      auto overError = ZS_DYNAMIC_PTR_CAST(IMediaStreamTrackTypes::OverconstrainedError, any);
      if (!overError) return nullptr;

      OverconstrainedError^ error = ref new OverconstrainedError();
      error->Name = UseHelper::ToCx(overError->mName);
      error->Constraint = UseHelper::ToCx(overError->mConstraint);
      error->Message = UseHelper::ToCx(overError->mMessage);

      return error;
    }

#pragma endregion

#pragma region MediaStreamTrack

    MediaStreamTrack::MediaStreamTrack(IMediaStreamTrackPtr nativeTrack) :
      _nativePointer(nativeTrack),
      _nativeDelegatePointer(make_shared<internal::MediaStreamTrackDelegate>(this))
    {
      if (_nativePointer) {
        _nativeDelegateSubscription = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    void MediaStreamTrack::SetVideoRenderCallback(void* callback)
    {
      if (!_nativePointer) return;
      _nativePointer->setVideoRenderCallback(callback);
    }

    MediaStreamTrackKind  MediaStreamTrack::Kind::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::Convert(_nativePointer->kind());
    }

    Platform::String^ MediaStreamTrack::Id::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::ToCx(_nativePointer->id());
    }

    Platform::String^ MediaStreamTrack::DeviceId::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::ToCx(_nativePointer->deviceID());
    }

    Platform::String^ MediaStreamTrack::Label::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::ToCx(_nativePointer->label());
    }

    Platform::Boolean MediaStreamTrack::Enabled::get()
    {
      if (!_nativePointer) return false;
      return _nativePointer->enabled();
    }

    void MediaStreamTrack::Enabled::set(Platform::Boolean value)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      _nativePointer->enabled(value);
    }

    Platform::Boolean MediaStreamTrack::Muted::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return _nativePointer->muted();
    }

    void MediaStreamTrack::Muted::set(Platform::Boolean value)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return _nativePointer->muted(value);
    }

    Platform::Boolean MediaStreamTrack::Remote::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return _nativePointer->remote();
    }

    MediaStreamTrackState MediaStreamTrack::ReadyState::get()
    {
      if (!_nativePointer) return UseHelper::Convert(IMediaStreamTrackTypes::State_Ended);
      return UseHelper::Convert(_nativePointer->readyState());
    }

    MediaStreamTrack^ MediaStreamTrack::Clone()
    {
      if (!_nativePointer) return nullptr;
      return ref new  MediaStreamTrack(_nativePointer->clone());
    }

    void MediaStreamTrack::Stop()
    {
      if (!_nativePointer) return;
      _nativePointer->stop();
    }

    MediaTrackCapabilities^		MediaStreamTrack::GetCapabilities()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->getCapabilities());
    }

    MediaTrackConstraints^		MediaStreamTrack::GetConstraints()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->getConstraints());
    }

    MediaTrackSettings^			MediaStreamTrack::GetSettings()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->getSettings());    
    }

    void MediaStreamTrack::SetH264Rendering(bool h264Rendering)
    {
      if (!_nativePointer) return;
      _nativePointer->setH264Rendering(h264Rendering);
    }

    bool MediaStreamTrack::IsH264Rendering()
    {
      if (!_nativePointer) return false;
      return _nativePointer->isH264Rendering();
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

    void MediaStreamTrack::SetVideoRenderCallback(Platform::IntPtr callback)
    {
      if (!_nativePointer) return;
      _nativePointer->setVideoRenderCallback((void *)callback);
    }

    IAsyncAction^ MediaStreamTrack::ApplyConstraints(MediaTrackConstraints^ constraints)
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!constraints);
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);

      auto promise = _nativePointer->applyConstraints(*internal::FromCx(constraints));

      IAsyncAction^ ret = Concurrency::create_async([promise, constraints]()
      {
        Concurrency::task_completion_event<void> tce;

        internal::MediaStreamTrackConstraintsPromiseObserverPtr pDelegate(make_shared<internal::MediaStreamTrackConstraintsPromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<void>(tce);

        return tceTask.get();
      });

      return ret;
    }

#pragma endregion

  } // namespace ortc
} // namespace org
