#pragma once
#include <collection.h>
#include <ortc\IRTPTypes.h>

using Windows::Foundation::Collections::IVector;

using namespace ortc;

namespace ortc_winrt_api
{
  public enum class RTCDegradationPreference
  {
    MaintainFramerate,
    MaintainResolution,
    Balanced
  };

  public ref class RTCRtcpFeedback sealed
  {
  public:
    property Platform::String^ Type;
    property Platform::String^ Parameter;
  public:
    Platform::String^ ToJsonString();
    static RTCRtcpFeedback^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpCodecCapability sealed
  {
  public:
    property Platform::String^          Name;
    property Platform::String^          Kind;
    property uint32                     ClockRate;
    property uint8                      PreferredPayloadType;
    property uint32                     Maxptime;
    property uint32                     NumChannels;
    property IVector<RTCRtcpFeedback^>^ RtcpFeedback;
    property Object^                    Parameters;
    property Object^                    Options;
    property uint16                     MaxTemporalLayers; //default = 0;
    property uint16                     MaxSpatialLayers; //default = 0;
    property Platform::Boolean          SvcMultiStreamSupport;
  public:
    Platform::String^ ToJsonString();
    static RTCRtpCodecCapability^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpOpusCodecCapabilityOptions sealed
  {
  public:
    property Platform::Boolean Complexity;
    property Platform::Boolean Signal;
    property Platform::Boolean Application;
    property Platform::Boolean PacketLossPerc;
    property Platform::Boolean PredictionDisabled;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpOpusCodecCapabilityOptions^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpOpusCodecCapabilityParameters sealed
  {
  public:
    property Platform::IBox<uint32>^            MaxPlaybackRate;
    property Platform::IBox<uint32>^            Ptime;
    property Platform::IBox<uint32>^            MaxAverageBitrate;
    property Platform::IBox<Platform::Boolean>^ Stereo;
    property Platform::IBox<Platform::Boolean>^ Cbr;
    property Platform::IBox<Platform::Boolean>^ UseInbandFec;
    property Platform::IBox<Platform::Boolean>^ UseDtx;

    // sender capability parameters
    property Platform::IBox<uint32>^            SpropMaxCaptureRate;
    property Platform::IBox<Platform::Boolean>^ SpropStereo;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpOpusCodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpVp8CodecCapabilityParameters sealed
  {
  public:
    property Platform::IBox<uint32>^  MaxFt;
    property Platform::IBox<uint64>^  MaxFs;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpVp8CodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpH264CodecCapabilityParameters sealed
  {
  public:
    property Platform::IBox<uint32>^ ProfileLevelId;
    property IVector<uint16>^        PacketizationModes;

    property Platform::IBox<uint64>^ MaxMbps;
    property Platform::IBox<uint64>^ MaxSmbps;
    property Platform::IBox<uint64>^ MaxFs;
    property Platform::IBox<uint64>^ MaxCpb;
    property Platform::IBox<uint64>^ MaxDpb;
    property Platform::IBox<uint64>^ MaxBr;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpH264CodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpHeaderExtension sealed
  {
  public:
    property Platform::String^    Kind;
    property Platform::String^    Uri;
    property uint16               PreferredId;
    property Platform::Boolean    PreferredEncrypt; //default = false;

  public:
    Platform::String^ ToJsonString();
    static RTCRtpHeaderExtension^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpCapabilities sealed
  {
  public:
    property IVector<RTCRtpCodecCapability^>^   Codecs;
    property IVector<RTCRtpHeaderExtension^>^   HeaderExtensions;
    property IVector<Platform::String^>^        FecMechanisms;

  public:
    Platform::String^ ToJsonString();
    static RTCRtpCapabilities^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class  RTCRtcpParameters sealed
  {
  public:
    property uint32               Ssrc;
    property Platform::String^    Cname;
    property Platform::Boolean    ReducedSize; //default = false
    property Platform::Boolean    Mux; //default = true
  
  public:
    //Platform::String^ ToJsonString();
    //static RTCRtcpParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpCodecParameters sealed
  {
  public:
    property Platform::String^          Name;
    property uint8                      PayloadType;
    property Platform::IBox<uint32>^    ClockRate;
    property uint32                     Maxptime;
    property uint32                     NumChannels;
    property IVector<RTCRtcpFeedback^>^ RtcpFeedback;
    property Object^                    Parameters;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpCodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpOpusCodecParameters sealed
  {
  public:
    property Platform::IBox<uint32>^            MaxPlaybackRate;
    property Platform::IBox<uint32>^            Ptime;
    property Platform::IBox<uint32>^            MaxAverageBitrate;
    property Platform::IBox<Platform::Boolean>^ Stereo;
    property Platform::IBox<Platform::Boolean>^ Cbr;
    property Platform::IBox<Platform::Boolean>^ UseInbandFec;
    property Platform::IBox<Platform::Boolean>^ UseDtx;

    property Platform::IBox<uint32>^            Complexity;
    property Platform::String^                  Signal;
    property Platform::String^                  Application;
    property Platform::IBox<uint32>^            PacketLossPerc;
    property Platform::IBox<Platform::Boolean>^ PredictionDisabled;

    // receiver parameters
    property Platform::IBox<uint32>^            SpropMaxCaptureRate;
    property Platform::IBox<Platform::Boolean>^ SpropStereo;
    
  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpOpusCodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpVp8CodecParameters sealed
  {
  public:
    property Platform::IBox<uint32>^  MaxFt;
    property Platform::IBox<uint64>^  MaxFs;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpVp8CodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpH264CodecParameters sealed
  {
  public:
    property Platform::IBox<uint32>^  ProfileLevelId;
    property IVector<uint16>^         PacketizationModes;

    property Platform::IBox<uint64>^  MaxMbps;
    property Platform::IBox<uint64>^  MaxSmbps;
    property Platform::IBox<uint64>^  MaxFs;
    property Platform::IBox<uint64>^  MaxCpb;
    property Platform::IBox<uint64>^  MaxDpb;
    property Platform::IBox<uint64>^  MaxBr;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpH264CodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpRtxCodecParameters sealed
  {
  public:
    property uint32                   RtxTime;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpRtxCodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpRedCodecParameters sealed
  {
  public:
    property IVector<uint8>^          PayloadTypes;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpRedCodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpFlexFecCodecParameters sealed
  {
  public:
    property uint64                   RepairWindow;

    property Platform::IBox<uint32>^  L;
    property Platform::IBox<uint32>^  D;
    property Platform::IBox<uint16>^  ToP;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpFlexFecCodecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class  RTCRtpHeaderExtensionParameters sealed
  {
  public:
    property Platform::String^        Uri;
    property uint16                   Id;
    property Platform::Boolean        Encrypt; //default = false;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpHeaderExtensionParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpFecParameters sealed
  {
  public:
    property Platform::IBox<uint32>^  Ssrc;
    property Platform::String^        Mechanism;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpFecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpRtxParameters sealed
  {
  public:
    property Platform::IBox<uint32>^  Ssrc;
    property Platform::IBox<uint8>^   PayloadType;

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpFecParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public enum class  RTCPriorityType
  {
    Unknown,
    Very_Low,
    Low,
    Medium,
    High
  };

  public ref class RTCRtpEncodingParameters sealed
  {
  public:
    property Platform::IBox<uint32>^      Ssrc;
    property Platform::IBox<uint8>^       CodecPayloadType;
    property RTCRtpFecParameters^         Fec;
    property RTCRtpRtxParameters^         Rtx;
    property RTCPriorityType              Priority;
    property uint64                       MaxBitrate;
    property float64                      MinQuality; //default = 0;
    property float64                      ResolutionScale;
    property float64                      FramerateScale;
    property Platform::Boolean            Active; //default = true;
    property Platform::String^            EncodingId;
    property IVector<Platform::String^>^  DependencyEncodingIds;

  public:
    RTCRtpEncodingParameters() { Active = true; }

    //Platform::String^ ToJsonString();
    //static RTCRtpEncodingParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCRtpParameters sealed
  {
  public:
    property Platform::String^                          MuxId;  //default = ""
    property IVector<RTCRtpCodecParameters^>^           Codecs;
    property IVector<RTCRtpHeaderExtensionParameters^>^ HeaderExtensions;
    property IVector<RTCRtpEncodingParameters^>^        Encodings;
    property RTCRtcpParameters^                         Rtcp;
    property RTCDegradationPreference                   DegradationPreference;  //default = MaintainBalanced

  public:
    //Platform::String^ ToJsonString();
    //static RTCRtpParameters^ FromJsonString(Platform::String^ jsonString);
  };

}
