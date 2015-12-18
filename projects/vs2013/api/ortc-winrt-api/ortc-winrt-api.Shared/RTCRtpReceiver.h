#pragma once

#include <ortc/IRTPReceiver.h>
#include <collection.h>

#include "RTCDtlsTransport.h"

using namespace ortc;

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
	ZS_DECLARE_CLASS_PTR(RTCRtpReceiverDelegate)

	ref class RTCRtpReceiver;

	class RTCRtpReceiverDelegate : public IRTPReceiverDelegate
	{
	public:
		virtual void onRTPReceiverError(
			IRTPReceiverPtr receiver,
			ErrorCode errorCode,
			zsLib::String errorReason
			);

		RTCRtpReceiver^ _owner;

		void SetOwnerObject(RTCRtpReceiver^ owner) { _owner = owner; }
	};
	// Error event and delegate
	public ref class RTCRtpReceiverError sealed
	{
	public:
		property uint16 ErrorCode;
		property Platform::String^ ErrorReason;
	};
	public ref class RTCRtpReceiverErrorEvent sealed {
	public:
		property RTCRtpReceiverError^ Error
		{
			RTCRtpReceiverError^  get() { return m_error; }
			void  set(RTCRtpReceiverError^ value) { m_error = value; }
		}

	private:
		RTCRtpReceiverError^ m_error;
	};
	public delegate void RTCRtpReceiverErrorDelegate(RTCRtpReceiverErrorEvent^ evt);


	public ref class  RTCRtcpFeedback sealed
	{
	public:
		property Platform::String^ type;
		property Platform::String^ parameter;
	};

	public ref class RTCRtpCodecCapability sealed
	{
	public:
		property Platform::String^					name;
		property Platform::String^					kind;
		property uint32								clockRate;
		//payloadtype               preferredPayloadType;
		property uint32								maxptime;
		property uint32								numChannels;
		property IVector<RTCRtcpFeedback^>^			rtcpFeedback;
		property Object^							parameters;
		property Object^							options;
		property uint16								maxTemporalLayers; //default = 0;
		property uint16								maxSpatialLayers; //default = 0;
		property boolean							svcMultiStreamSupport;
	};

	public ref class RTCRtpHeaderExtensions sealed
	{
	public:
		property Platform::String^					kind;
		property Platform::String^					uri;
		property uint16								preferredId;
		property boolean							preferredEncrypt; //default = false;
	};

	public ref class RTCRtpCapabilities sealed
	{
	public:
		property IVector<RTCRtpCodecCapability^>^			codecs;
		property IVector<RTCRtpHeaderExtensions^>^			headerExtensions;
		property IVector<Platform::String^>^					fecMechanisms;
	};

	public ref class RTCRtpContributingSource sealed
	{
		friend RTCRtpReceiver;
	private:
		std::chrono::system_clock::time_point mTimeStamp;
		uint32       mSource;
		byte		mAudioLevel;
		
	public:
		/*property std::chrono::system_clock::time_point TimeStamp
		{
			std::chrono::system_clock::time_point  get() { return mTimeStamp; }
		}*/
		property uint32 Source
		{
			uint32  get() { return mSource; }
		}
		property byte AudioLevel
		{
			byte  get() { return mAudioLevel; }
		}
	};


	public ref class RTCRtpCodecParameters sealed
	{
	public:
		property Platform::String^					name;
		//payloadtype							payloadType;
		property uint32								clockRate;
		property uint32								maxptime;
		property uint32								numChannels;
		property IVector<RTCRtcpFeedback^>^			rtcpFeedback;
		property Object^							parameters;
	};

	public ref class  RTCRtpHeaderExtensionParameters sealed 
	{
		property Platform::String^      uri;
		property uint16					id;
		property boolean				encrypt; //default = false;
	};

	public ref class  RTCRtcpParameters sealed
	{
	public:
		property uint32								ssrc;
		property Platform::String^					cname;
		property boolean							reducedSize; //default = false
		property boolean							mux; //default = true
	};

	public enum class RTCDegradationPreference 
	{
		Degradation_Preference_Maintain_Framerate,
		Degradation_Preference_Maintain_Resolution,
		Degradation_Preference_Maintain_Balanced
	};

	public ref class RTCRtpFecParameters sealed
	{
		property uint32								ssrc;
		property Platform::String^					mechanism;
	};

	public ref class RTCRtpRtxParameters sealed
	{
		property uint32								ssrc;
		//payloadtype							payloadType;
	};

	public enum class  RTCPriorityType 
	{
		Priority_Very_Low,
		Priority_Low,
		Priority_Medium,
		Priority_High
	};

	public ref class RTCRtpEncodingParameters sealed
	{
		property uint32							ssrc;
		//payloadtype						codecPayloadType;
		RTCRtpFecParameters fec;
		RTCRtpRtxParameters rtx;
		RTCPriorityType							priority;
		property uint32							maxBitrate;
		property float64						minQuality; //default = 0;
		property float64						resolutionScale;
		property float64						framerateScale;
		property boolean						active; //default = true;
		property Platform::String^				encodingId;
		IVector<Platform::String^>^				dependencyEncodingIds;
	};

	public  ref  class RTCRtpParameters sealed
	{
	public:
		property Platform::String^								muxId;	//default = ""
		property IVector<RTCRtpCodecParameters^>^				codecs;
		property IVector<RTCRtpHeaderExtensionParameters^>^		headerExtensions;
		property IVector<RTCRtpEncodingParameters^>^			encodings;
		property RTCRtcpParameters^								rtcp;
		property RTCDegradationPreference						degradationPreference;	//default = Degradation_Preference_Maintain_Balanced
	};

	public  ref  class  RTCRtpReceiver sealed
	{
		friend class RTCRtpReceiverDelegate;
		/*    readonly        attribute MediaStreamTrack? track;
    readonly        attribute RTCDtlsTransport  transport;
    readonly        attribute RTCDtlsTransport  rtcpTransport;*/
	private:
		IRTPReceiverPtr mNativePointer;
		RTCRtpReceiverDelegatePtr mNativeDelegatePointer;
		RTCDtlsTransport^ _transport;
	public:
		/*property RTCDtlsTransport^ Transport
		{
			RTCDtlsTransport^ get()
			{
				if (mNativePointer)
				{
					if (!_transport)
						_transport = ref new RTCDtlsTransport();

					mNativePointer->ge

				}
			}
		}*/
		RTCRtpReceiver();
		RTCRtpReceiver(RTCDtlsTransport^ transport);
		RTCRtpReceiver(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);

		void setTransport(RTCDtlsTransport^ transport);
		void setTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
		static RTCRtpCapabilities ^ getCapabilities(Platform::String^ kind);
		void receive(RTCRtpParameters^ parameters);
		IVector<RTCRtpContributingSource^>^ getContributingSource();
		void stop();

		event RTCRtpReceiverErrorDelegate^              OnRTCRtpReceiverError;
	};
}
