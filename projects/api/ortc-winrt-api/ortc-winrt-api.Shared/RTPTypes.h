#pragma once
#include <collection.h>

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
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
		property uint8               preferredPayloadType;
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


	public ref class RTCRtpCodecParameters sealed
	{
	public:
		property Platform::String^					name;
		property uint8								payloadType;
		property uint32								clockRate;
		property uint32								maxptime;
		property uint32								numChannels;
		property IVector<RTCRtcpFeedback^>^			rtcpFeedback;
		property Object^							parameters;
	};

	public ref class  RTCRtpHeaderExtensionParameters sealed
	{
	public:
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
	public:
		property uint32								ssrc;
		property Platform::String^					mechanism;
	};

	public ref class RTCRtpRtxParameters sealed
	{
	public:
		property uint32								ssrc;
		property uint8							payloadType;
	};

	public enum class  RTCPriorityType
	{
		Priority_Unknown,
		Priority_Very_Low,
		Priority_Low,
		Priority_Medium,
		Priority_High
	};

	public ref class RTCRtpEncodingParameters sealed
	{
	public:
		property uint32							ssrc;
		property uint8							codecPayloadType;
		property RTCRtpFecParameters^ fec;
		property RTCRtpRtxParameters^ rtx;
		property RTCPriorityType							priority;
		property uint32							maxBitrate;
		property float64						minQuality; //default = 0;
		property float64						resolutionScale;
		property float64						framerateScale;
		property boolean						active; //default = true;
		property Platform::String^				encodingId;
		property IVector<Platform::String^>^				dependencyEncodingIds;
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

}