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
	ZS_DECLARE_CLASS_PTR(OrtcMediaStreamTrackPromiseObserver)
	ZS_DECLARE_CLASS_PTR(OrtcMediaDevicesPromiseObserver)
	public ref struct Constraints sealed
	{
		property MediaTrackConstraints^ Video;
		property MediaTrackConstraints^ Audio;

		static Constraints^ create();
		static Constraints^ create(Constraints^ value);
	};

	public delegate void OrtcMediaDevicesChangeDelegate();

	class OrtcMediaDevicesPromiseObserver : public zsLib::IPromiseResolutionDelegate
	{
	public:
		OrtcMediaDevicesPromiseObserver(Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> tce);

		virtual void onPromiseResolved(PromisePtr promise);
		virtual void onPromiseRejected(PromisePtr promise);

	private:
		Concurrency::task_completion_event<IVector<MediaDeviceInfo^>^> mTce;
	};

	class OrtcMediaStreamTrackPromiseObserver : public zsLib::IPromiseResolutionDelegate
	{
	public:
		OrtcMediaStreamTrackPromiseObserver(Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> tce);

		virtual void onPromiseResolved(PromisePtr promise);
		virtual void onPromiseRejected(PromisePtr promise);

	private:
		Concurrency::task_completion_event<IVector<MediaStreamTrack^>^> mTce;
	};


	public ref class OrtcMediaDevices sealed
	{
		friend class OrtcMediaDevicesDelegate;
	public:
		static SupportedConstraints^ GetSupportedConstraints();

		static IAsyncOperation<IVector<MediaDeviceInfo^>^>^ enumerateDevices();

		static IAsyncOperation<IVector<MediaStreamTrack^>^>^ getUserMedia(Constraints^ constraints);

		event OrtcMediaDevicesChangeDelegate^       OnDeviceChange;
	};

	class OrtcMediaDevicesDelegate : public IMediaDevicesDelegate
	{
	public:
		void onMediaDevicesChanged();

		OrtcMediaDevices^ _mediaDevices;

		void SetOwnerObject(OrtcMediaDevices^ owner) { _mediaDevices = owner; }
	};
}

