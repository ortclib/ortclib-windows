#pragma once
#include <collection.h>



using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
	public enum class MediaStreamTrackState
	{
		TrackState_Live,
		TrackState_Ended
	};
	public ref class MediaTrackCapabilities sealed
	{
	public:
		property uint32					width;
		property uint32					height;
		property double					aspectRatio;
		property double					frameRate;
		property Platform::String^      facingMode;
		property double					volume;
		property uint32					sampleRate;
		property uint32					sampleSize;
		property IVector<boolean>^      echoCancellation;
		property Platform::String^      deviceId;
		property Platform::String^      groupId;
	};

	public ref class MediaTrackConstraintSet sealed
	{
	public:
		property uint32				width;
		property uint32				height;
		property double				aspectRatio;
		property double				frameRate;
		property Platform::String^	facingMode;
		property double				volume;
		property uint32				sampleRate;
		property uint32				sampleSize;
		property boolean			echoCancellation;
		property Platform::String^	deviceId;
		property Platform::String^	groupId;
	};

	public ref class MediaTrackConstraints sealed
	{
	public:
		property IVector<MediaTrackConstraintSet^>^ advanced;
	};
	public ref class MediaTrackSettings sealed
	{
	public:
		property uint32				width;
		property uint32				height;
		property double				aspectRatio;
		property double				frameRate;
		property Platform::String^	facingMode;
		property double				volume;
		property uint32				sampleRate;
		property uint32				sampleSize;
		property boolean			echoCancellation;
		property Platform::String^	deviceId;
		property Platform::String^	groupId;
	};

	public ref class MediaStreamError sealed
	{
	public:
		property Platform::String^ name;
		property Platform::String^ message;
		property Platform::String^ constraintName;
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
		Platform::String^		_kind;
		Platform::String^		_id;
		Platform::String^		_label;
		boolean					_enabled;
		boolean					_muted;
		boolean					_readonly;
		boolean					_remote;
		MediaStreamTrackState	_readyState;
	public:
		property Platform::String^ Kind
		{
			Platform::String^  get() { return _kind; }
		}

		property Platform::String^ Id
		{
			Platform::String^  get() { return _id; }
		}

		property Platform::String^ Label
		{
			Platform::String^  get() { return _label; }
		}

		property boolean Enabled
		{
			boolean  get() { return _enabled; }
			void  set(boolean value) { _enabled = value; }
		}

		property boolean Muted
		{
			boolean  get() { return _muted; }
		}

		property boolean ReadOnly
		{
			boolean  get() { return _readonly; }
		}

		property boolean Remote
		{
			boolean  get() { return _remote; }
		}

		property MediaStreamTrackState ReadyState
		{
			MediaStreamTrackState  get() { return _readyState; }
		}

		MediaStreamTrack^			clone();
		void						stop();
		MediaTrackCapabilities^		getCapabilities();
		MediaTrackConstraints^		getConstraints();
		MediaTrackSettings^			getSettings();

		event MediaStreamTrackMuteDelegate^       OnMediaStreamTrackMuted;
		event MediaStreamTrackUnMuteDelegate^		OnMediaStreamTrackUnMuted;
		event MediaStreamTrackEndedDelegate^       OnMediaStreamTrackEnded;
		event MediaStreamTrackOverConstrainedDelegate^       OnMediaStreamTrackOverConstrained;
		/*


				Promise<void>          applyConstraints (MediaTrackConstraints constraints);*/
	};
}

