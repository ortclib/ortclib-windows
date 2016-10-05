#include "pch.h"

#include <org/ortc/MediaDevices.h>
#include <org/ortc/MediaStreamTrack.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

#include <webrtc/modules/video_capture/windows/video_capture_winrt.h>

#include <ortc/services/IHelper.h>

using Platform::Collections::Vector;

using namespace ortc;

namespace org
{
  namespace ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::services::IHelper, UseServicesHelper)

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(MediaDevicesPromiseObserver)
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackPromiseObserver)
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackConstraintsPromiseObserver)

#pragma region MediaStreamConstraints / IMediaStreamTrackTypes::Constraints conversion

      MediaStreamConstraints^ ToCx(const IMediaStreamTrackTypes::Constraints &input)
      {
        auto result = ref new MediaStreamConstraints();
        result->Video = ToCx(input.mVideo);
        result->Audio = ToCx(input.mAudio);
        return result;
      }

      MediaStreamConstraints^ ToCx(IMediaStreamTrackTypes::ConstraintsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaStreamTrackTypes::ConstraintsPtr FromCx(MediaStreamConstraints^ input)
      {
        if (nullptr == input) return IMediaStreamTrackTypes::ConstraintsPtr();
        auto result(make_shared<IMediaStreamTrackTypes::Constraints>());
        result->mVideo = FromCx(input->Video);
        result->mAudio = FromCx(input->Audio);
        return result;
      }

#pragma endregion

#pragma region MediaDeviceInfo / IMediaDevicesTypes::Device conversion

      MediaDeviceInfo^ ToCx(const IMediaDevicesTypes::Device &input)
      {
        auto result = ref new MediaDeviceInfo();
        result->Kind = Helper::Convert(input.mKind);
        result->Label = Helper::ToCx(input.mLabel);
        result->DeviceId = Helper::ToCx(input.mDeviceID);
        result->GroupId = Helper::ToCx(input.mGroupID);
        return result;
      }

      MediaDeviceInfo^ ToCx(IMediaDevicesTypes::DevicePtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaDevicesTypes::DevicePtr FromCx(MediaDeviceInfo^ input)
      {
        if (nullptr == input) return IMediaDevicesTypes::DevicePtr();
        auto result(make_shared<IMediaDevicesTypes::Device>());
        result->mKind = Helper::Convert(input->Kind);
        result->mLabel = Helper::FromCx(input->Label);
        result->mDeviceID = Helper::FromCx(input->DeviceId);
        result->mGroupID = Helper::FromCx(input->GroupId);
        return result;
      }

#pragma endregion

#pragma region MediaTrackSupportedConstraints / IMediaDevicesTypes::SupportedConstraints conversion

      MediaTrackSupportedConstraints^ ToCx(const IMediaDevicesTypes::SupportedConstraints &input)
      {
        auto result = ref new MediaTrackSupportedConstraints();
        result->Width = input.mWidth;
        result->Height = input.mHeight;
        result->AspectRatio = input.mAspectRatio;
        result->FrameRate = input.mFrameRate;
        result->FacingMode = input.mFacingMode;
        result->Volume = input.mVolume;
        result->SampleRate = input.mSampleRate;
        result->SampleSize = input.mSampleSize;
        result->EchoCancellation = input.mEchoCancellation;
        result->Latency = input.mLatency;
        result->DeviceId = input.mDeviceID;
        result->GroupId = input.mGroupID;
        return result;
      }

      MediaTrackSupportedConstraints^ ToCx(IMediaDevicesTypes::SupportedConstraintsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IMediaDevicesTypes::SupportedConstraintsPtr FromCx(MediaTrackSupportedConstraints^ input)
      {
        if (nullptr == input) return IMediaDevicesTypes::SupportedConstraintsPtr();
        auto result(make_shared<IMediaDevicesTypes::SupportedConstraints>());
        result->mWidth = input->Width;
        result->mHeight = input->Height;
        result->mAspectRatio = input->AspectRatio;
        result->mFrameRate = input->FrameRate;
        result->mFacingMode = input->FacingMode;
        result->mVolume = input->Volume;
        result->mSampleRate = input->SampleRate;
        result->mSampleSize = input->SampleSize;
        result->mEchoCancellation = input->EchoCancellation;
        result->mLatency = input->Latency;
        result->mDeviceID = input->DeviceId;
        result->mGroupID = input->GroupId;
        return result;
      }

#pragma endregion

#pragma region MediaDevices delegates

      class MediaDevicesDelegate : public IMediaDevicesDelegate
      {
      public:
        MediaDevicesDelegate(MediaDevices^ owner) : _owner(owner) { }

        void onMediaDevicesChanged()
        {
          _owner->OnDeviceChange();
        }

      private:
        MediaDevices^ _owner;
      };

#pragma endregion

#pragma region MediaDevices observers

      class MediaDevicesPromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        MediaDevicesPromiseObserver(Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce) : mTce(tce) {}

        virtual void onPromiseResolved(PromisePtr promise) override
        {
          auto ret = ref new Vector<MediaDeviceInfo^>();

          IMediaDevicesTypes::DeviceListPtr deviceList = promise->value<IMediaDevicesTypes::DeviceList>();

          if (deviceList)
          {
            for (IMediaDevicesTypes::DeviceList::iterator it = deviceList->begin(); it != deviceList->end(); ++it)
            {
              MediaDeviceInfo^ med = internal::ToCx(*it);
              ret->Append(med);
            }
          }

          mTce.set(ret);
        }

        virtual void onPromiseRejected(PromisePtr promise) override
        {
          IMediaDevicesTypes::PromiseWithDeviceListPtr deviceListPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithDeviceList, promise);

          auto reason = deviceListPromise->reason();

          auto overError = OverconstrainedError::CreateIfOverconstrainedError(reason);
          if (overError)
          {
            mTce.set_exception(overError);
            return;
          }
          auto error = Error::CreateIfGeneric(reason);
          mTce.set_exception(error);
        }

      private:
        Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> mTce;
      };

      class MediaStreamTrackPromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        MediaStreamTrackPromiseObserver(Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce) : mTce(tce) {}

        virtual void onPromiseResolved(PromisePtr promise) override
        {
          auto ret = ref new Vector<MediaStreamTrack^>();

          IMediaDevicesTypes::MediaStreamTrackListPtr mediaStreamTrackListPtr2 = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::MediaStreamTrackList, promise);

          IMediaDevicesTypes::MediaStreamTrackListPtr mediaStreamTrackListPtr = promise->value<IMediaDevicesTypes::MediaStreamTrackList>();

          if (mediaStreamTrackListPtr)
          {
            for (IMediaDevicesTypes::MediaStreamTrackList::iterator it = mediaStreamTrackListPtr->begin(); it != mediaStreamTrackListPtr->end(); ++it)
            {
              auto nativeTrack = (*it);
              MediaStreamTrack^ track = MediaStreamTrack::Convert(nativeTrack);
              ret->Append(track);
            }
          }
          mTce.set(ret);
        }

        virtual void onPromiseRejected(PromisePtr promise) override
        {
          IMediaDevicesTypes::PromiseWithMediaStreamTrackListPtr streamTrackPromise = ZS_DYNAMIC_PTR_CAST(IMediaDevicesTypes::PromiseWithMediaStreamTrackList, promise);
          mTce.set_exception(streamTrackPromise->reason());
        }

      private:
        Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> mTce;
      };

#pragma endregion

    } // namespace internal

    MediaStreamConstraints^ MediaStreamConstraints::create()
    {
      return ref new MediaStreamConstraints();
    }

    MediaStreamConstraints^ MediaStreamConstraints::create(MediaStreamConstraints^ value)
    {
      auto temp = internal::FromCx(value);
      return internal::ToCx(temp);
    }


#pragma region MediaDevices

    MediaDevices::MediaDevices() :
      _nativeDelegatePointer(make_shared<internal::MediaDevicesDelegate>(this))
    {
      _nativeDelegateSubscription = IMediaDevices::subscribe(_nativeDelegatePointer);
    }

    MediaDevices^ MediaDevices::_singleton = nullptr;

    MediaDevices^ MediaDevices::Singleton::get()
    {
      zsLib::AutoRecursiveLock lock(*UseServicesHelper::getGlobalLock());
      if (nullptr != _singleton) return _singleton;

      _singleton = ref new MediaDevices();
      return _singleton;
    }

    MediaTrackSupportedConstraints^ MediaDevices::GetSupportedConstraints()
    {
      return internal::ToCx(IMediaDevices::getSupportedConstraints());
    }

    IAsyncOperation<IVector<MediaDeviceInfo^>^>^ MediaDevices::EnumerateDevices()
    {
      auto promise = IMediaDevices::enumerateDevices();

      IAsyncOperation<IVector<MediaDeviceInfo^>^>^ ret = Concurrency::create_async([promise]() -> IVector<MediaDeviceInfo^>^
      {
        Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce;

        internal::MediaDevicesPromiseObserverPtr pDelegate(make_shared<internal::MediaDevicesPromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<IVector<MediaDeviceInfo^>^>(tce);

        return tceTask.get();
      });

      return ret;
    }

    IAsyncOperation<IVector<MediaStreamTrack^>^>^ MediaDevices::GetUserMedia(MediaStreamConstraints^ constraints)
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!constraints)
      auto promise = IMediaDevices::getUserMedia(*(internal::FromCx(constraints).get()));

      IAsyncOperation<IVector<MediaStreamTrack^>^>^ ret = Concurrency::create_async([promise]() -> IVector<MediaStreamTrack^>^
      {
        Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce;

        internal::MediaStreamTrackPromiseObserverPtr pDelegate(make_shared<internal::MediaStreamTrackPromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<IVector<MediaStreamTrack^>^>(tce);

        return tceTask.get();
      });

      return ret;
    }

    void MediaDevices::OnAppSuspending() {
      // not implemented in current webrtc version - uncomment after webrtc is updated
      //webrtc::videocapturemodule::MediaCaptureDevicesWinRT::Instance()->ClearCaptureDevicesCache();
    }

    void MediaDevices::SetDisplayOrientation(
      Windows::Graphics::Display::DisplayOrientations display_orientation) {
      webrtc::videocapturemodule::AppStateDispatcher::Instance()->DisplayOrientationChanged(display_orientation);
    }

#pragma endregion

  } // namespace ortc
} // namespace org
