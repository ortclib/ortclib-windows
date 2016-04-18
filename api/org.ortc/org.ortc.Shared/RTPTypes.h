
#pragma once
//#include <collection.h>
#include <ortc\IRTPTypes.h>
//
//
//using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPTypes, IRTPTypes)
      
    using Windows::Foundation::Collections::IVector;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpCodecCapability;
    ref struct RTCRtpOpusCodecCapabilityOptions;
    ref struct RTCRtpOpusCodecCapabilityParameters;
    ref struct RTCRtpVp8CodecCapabilityParameters;
    ref struct RTCRtpH264CodecCapabilityParameters;
    ref struct RTCRtpRtxCodecCapabilityParameters;
    ref struct RTCRtpFlexFecCodecCapabilityParameters;

    ref struct RTCRtpParameters;
    ref struct RTCRtcpParameters;
    ref struct RTCRtpCodecParameters;
    ref struct RTCRtpOpusCodecParameters;
    ref struct RTCRtpVp8CodecParameters;
    ref struct RTCRtpH264CodecParameters;
    ref struct RTCRtpRtxCodecParameters;
    ref struct RTCRtpRedCodecParameters;
    ref struct RTCRtpFlexFecCodecParameters;
    ref struct RTCRtpEncodingParameters;
    ref struct RTCRtpRtxParameters;
    ref struct RTCRtpFecParameters;
    ref struct RTCRtpHeaderExtensionParameters;

    ref struct RTCRtcpFeedback;
    ref struct RTCRtpHeaderExtension;

    namespace internal
    {
      RTCRtpCapabilities^ ToCx(const IRTPTypes::Capabilities &input);
      RTCRtpCapabilities^ ToCx(IRTPTypes::CapabilitiesPtr input);
      IRTPTypes::CapabilitiesPtr FromCx(RTCRtpCapabilities^ input);

      RTCRtpCodecCapability^ ToCx(const IRTPTypes::CodecCapability &input);
      RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr input);
      IRTPTypes::CodecCapabilityPtr FromCx(RTCRtpCodecCapability^ input);

      RTCRtpOpusCodecCapabilityOptions^ ToCx(const IRTPTypes::OpusCodecCapabilityOptions &input);
      RTCRtpOpusCodecCapabilityOptions^ ToCx(IRTPTypes::OpusCodecCapabilityOptionsPtr input);
      IRTPTypes::OpusCodecCapabilityOptionsPtr FromCx(RTCRtpOpusCodecCapabilityOptions^ input);

      RTCRtpOpusCodecCapabilityParameters^ ToCx(const IRTPTypes::OpusCodecCapabilityParameters &input);
      RTCRtpOpusCodecCapabilityParameters^ ToCx(IRTPTypes::OpusCodecCapabilityParametersPtr input);
      IRTPTypes::OpusCodecCapabilityParametersPtr FromCx(RTCRtpOpusCodecCapabilityParameters^ input);

      RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::VP8CodecCapabilityParameters &input);
      RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::VP8CodecCapabilityParametersPtr input);
      IRTPTypes::VP8CodecCapabilityParametersPtr FromCx(RTCRtpVp8CodecCapabilityParameters^ input);

      RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::H264CodecCapabilityParameters &input);
      RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::H264CodecCapabilityParametersPtr input);
      IRTPTypes::H264CodecCapabilityParametersPtr FromCx(RTCRtpH264CodecCapabilityParameters^ input);

      RTCRtpRtxCodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::RTXCodecCapabilityParameters &input);
      RTCRtpRtxCodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::RTXCodecCapabilityParametersPtr input);
      IRTPTypes::RTXCodecCapabilityParametersPtr FromCx(RTCRtpRtxCodecCapabilityParameters^ input);

      RTCRtpFlexFecCodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::FlexFECCodecCapabilityParameters &input);
      RTCRtpFlexFecCodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::FlexFECCodecCapabilityParametersPtr input);
      IRTPTypes::FlexFECCodecCapabilityParametersPtr FromCx(RTCRtpFlexFecCodecCapabilityParameters^ input);


      RTCRtpParameters^ ToCx(const IRTPTypes::Parameters &input);
      RTCRtpParameters^ ToCx(IRTPTypes::ParametersPtr input);
      IRTPTypes::ParametersPtr FromCx(RTCRtpParameters^ input);

      RTCRtcpParameters^ ToCx(const IRTPTypes::RTCPParameters &input);
      RTCRtcpParameters^ ToCx(IRTPTypes::RTCPParametersPtr input);
      IRTPTypes::RTCPParametersPtr FromCx(RTCRtcpParameters^ input);

      RTCRtpCodecParameters^ ToCx(const IRTPTypes::CodecParameters &input);
      RTCRtpCodecParameters^ ToCx(IRTPTypes::CodecParametersPtr input);
      IRTPTypes::CodecParametersPtr FromCx(RTCRtpCodecParameters^ input);

      RTCRtpOpusCodecParameters^ ToCx(const IRTPTypes::OpusCodecParameters &input);
      RTCRtpOpusCodecParameters^ ToCx(IRTPTypes::OpusCodecParametersPtr input);
      IRTPTypes::OpusCodecParametersPtr FromCx(RTCRtpOpusCodecParameters^ input);

      RTCRtpVp8CodecParameters^ ToCxParameters(const IRTPTypes::VP8CodecParameters &input);
      RTCRtpVp8CodecParameters^ ToCxParameters(IRTPTypes::VP8CodecParametersPtr input);
      IRTPTypes::VP8CodecCapabilityParametersPtr FromCx(RTCRtpVp8CodecParameters^ input);

      RTCRtpH264CodecParameters^ ToCxParameters(const IRTPTypes::H264CodecParameters &input);
      RTCRtpH264CodecParameters^ ToCxParameters(IRTPTypes::H264CodecParametersPtr input);
      IRTPTypes::H264CodecParametersPtr FromCx(RTCRtpH264CodecParameters^ input);

      RTCRtpRtxCodecParameters^ ToCx(const IRTPTypes::RTXCodecParameters &input);
      RTCRtpRtxCodecParameters^ ToCx(IRTPTypes::RTXCodecParametersPtr input);
      IRTPTypes::RTXCodecParametersPtr FromCx(RTCRtpRtxCodecParameters^ input);

      RTCRtpRedCodecParameters^ ToCx(const IRTPTypes::REDCodecParameters &input);
      RTCRtpRedCodecParameters^ ToCx(IRTPTypes::REDCodecParametersPtr input);
      IRTPTypes::REDCodecParametersPtr FromCx(RTCRtpRedCodecParameters^ input);

      RTCRtpFlexFecCodecParameters^ ToCx(const IRTPTypes::FlexFECCodecParameters &input);
      RTCRtpFlexFecCodecParameters^ ToCx(IRTPTypes::FlexFECCodecParametersPtr input);
      IRTPTypes::FlexFECCodecParametersPtr FromCx(RTCRtpFlexFecCodecParameters^ input);

      RTCRtpEncodingParameters^ ToCx(const IRTPTypes::EncodingParameters &input);
      RTCRtpEncodingParameters^ ToCx(IRTPTypes::EncodingParametersPtr input);
      IRTPTypes::EncodingParametersPtr FromCx(RTCRtpEncodingParameters^ input);

      RTCRtpRtxParameters^ ToCx(const IRTPTypes::RTXParameters &input);
      RTCRtpRtxParameters^ ToCx(IRTPTypes::RTXParametersPtr input);
      IRTPTypes::RTXParametersPtr FromCx(RTCRtpRtxParameters^ input);

      RTCRtpFecParameters^ ToCx(const IRTPTypes::FECParameters &input);
      RTCRtpFecParameters^ ToCx(IRTPTypes::FECParametersPtr input);
      IRTPTypes::FECParametersPtr FromCx(RTCRtpFecParameters^ input);

      RTCRtpHeaderExtensionParameters^ ToCx(const IRTPTypes::HeaderExtensionParameters &input);
      RTCRtpHeaderExtensionParameters^ ToCx(IRTPTypes::HeaderExtensionParametersPtr input);
      IRTPTypes::HeaderExtensionParametersPtr FromCx(RTCRtpHeaderExtensionParameters^ input);


      RTCRtcpFeedback^ ToCx(const IRTPTypes::RTCPFeedback &input);
      RTCRtcpFeedback^ ToCx(IRTPTypes::RTCPFeedbackPtr input);
      IRTPTypes::RTCPFeedback FromCx(RTCRtcpFeedback^ input);

      RTCRtpHeaderExtension^ ToCx(const IRTPTypes::HeaderExtension &input);
      RTCRtpHeaderExtension^ ToCx(IRTPTypes::HeaderExtensionPtr input);
      IRTPTypes::HeaderExtensionPtr FromCx(RTCRtpHeaderExtension^ input);

    } // namespace internal

    public enum class RTCDegradationPreference
    {
      MaintainFramerate,
      MaintainResolution,
      Balanced
    };

    public ref struct RTCRtcpFeedback sealed
    {
      property Platform::String^ Type;
      property Platform::String^ Parameter;

      Platform::String^ ToJsonString();
      static RTCRtcpFeedback^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpCodecCapability sealed
    {
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

      Platform::String^ ToJsonString();
      static RTCRtpCodecCapability^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpOpusCodecCapabilityOptions sealed
    {
      property Platform::Boolean Complexity;
      property Platform::Boolean Signal;
      property Platform::Boolean Application;
      property Platform::Boolean PacketLossPerc;
      property Platform::Boolean PredictionDisabled;

      //Platform::String^ ToJsonString();
      //static RTCRtpOpusCodecCapabilityOptions^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpOpusCodecCapabilityParameters sealed
    {
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

      //Platform::String^ ToJsonString();
      //static RTCRtpOpusCodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpVp8CodecCapabilityParameters sealed
    {
      property Platform::IBox<uint32>^  MaxFt;
      property Platform::IBox<uint64>^  MaxFs;

      //Platform::String^ ToJsonString();
      //static RTCRtpVp8CodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpH264CodecCapabilityParameters sealed
    {
      property Platform::IBox<uint32>^ ProfileLevelId;
      property IVector<uint16>^        PacketizationModes;

      property Platform::IBox<uint64>^ MaxMbps;
      property Platform::IBox<uint64>^ MaxSmbps;
      property Platform::IBox<uint64>^ MaxFs;
      property Platform::IBox<uint64>^ MaxCpb;
      property Platform::IBox<uint64>^ MaxDpb;
      property Platform::IBox<uint64>^ MaxBr;

      //Platform::String^ ToJsonString();
      //static RTCRtpH264CodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpRtxCodecCapabilityParameters sealed
    {
      property uint32                   RtxTime;
      property uint8                    Apt;

      //Platform::String^ ToJsonString();
      //static RTCRtpRtxCodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpFlexFecCodecCapabilityParameters sealed
    {
      property uint64                   RepairWindow;

      property Platform::IBox<uint32>^  L;
      property Platform::IBox<uint32>^  D;
      property Platform::IBox<uint16>^  ToP;

      //Platform::String^ ToJsonString();
      //static RTCRtpFlexFecCodecCapabilityParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpHeaderExtension sealed
    {
      property Platform::String^    Kind;
      property Platform::String^    Uri;
      property uint16               PreferredId;
      property Platform::Boolean    PreferredEncrypt; //default = false;

      Platform::String^ ToJsonString();
      static RTCRtpHeaderExtension^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpCapabilities sealed
    {
      property IVector<RTCRtpCodecCapability^>^   Codecs;
      property IVector<RTCRtpHeaderExtension^>^   HeaderExtensions;
      property IVector<Platform::String^>^        FecMechanisms;

      Platform::String^ ToJsonString();
      static RTCRtpCapabilities^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct  RTCRtcpParameters sealed
    {
      property uint32               Ssrc;
      property Platform::String^    Cname;
      property Platform::Boolean    ReducedSize; //default = false
      property Platform::Boolean    Mux; //default = true

      //Platform::String^ ToJsonString();
      //static RTCRtcpParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpCodecParameters sealed
    {
      property Platform::String^          Name;
      property uint8                      PayloadType;
      property Platform::IBox<uint32>^    ClockRate;
      property uint32                     Maxptime;
      property uint32                     NumChannels;
      property IVector<RTCRtcpFeedback^>^ RtcpFeedback;
      property Object^                    Parameters;

      //Platform::String^ ToJsonString();
      //static RTCRtpCodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpOpusCodecParameters sealed
    {
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

      //Platform::String^ ToJsonString();
      //static RTCRtpOpusCodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpVp8CodecParameters sealed
    {
      property Platform::IBox<uint32>^  MaxFt;
      property Platform::IBox<uint64>^  MaxFs;

      //Platform::String^ ToJsonString();
      //static RTCRtpVp8CodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpH264CodecParameters sealed
    {
      property Platform::IBox<uint32>^  ProfileLevelId;
      property IVector<uint16>^         PacketizationModes;

      property Platform::IBox<uint64>^  MaxMbps;
      property Platform::IBox<uint64>^  MaxSmbps;
      property Platform::IBox<uint64>^  MaxFs;
      property Platform::IBox<uint64>^  MaxCpb;
      property Platform::IBox<uint64>^  MaxDpb;
      property Platform::IBox<uint64>^  MaxBr;

      //Platform::String^ ToJsonString();
      //static RTCRtpH264CodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpRtxCodecParameters sealed
    {
      property uint32                   RtxTime;
      property uint8                    Apt;

      //Platform::String^ ToJsonString();
      //static RTCRtpRtxCodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpRedCodecParameters sealed
    {
      property IVector<uint8>^          PayloadTypes;

      //Platform::String^ ToJsonString();
      //static RTCRtpRedCodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpFlexFecCodecParameters sealed
    {
      property uint64                   RepairWindow;

      property Platform::IBox<uint32>^  L;
      property Platform::IBox<uint32>^  D;
      property Platform::IBox<uint16>^  ToP;

      //Platform::String^ ToJsonString();
      //static RTCRtpFlexFecCodecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct  RTCRtpHeaderExtensionParameters sealed
    {
      property Platform::String^        Uri;
      property uint16                   Id;
      property Platform::Boolean        Encrypt; //default = false;

      //Platform::String^ ToJsonString();
      //static RTCRtpHeaderExtensionParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpFecParameters sealed
    {
      property Platform::IBox<uint32>^  Ssrc;
      property Platform::String^        Mechanism;

      //Platform::String^ ToJsonString();
      //static RTCRtpFecParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpRtxParameters sealed
    {
      property Platform::IBox<uint32>^  Ssrc;
      property Platform::IBox<uint8>^   PayloadType;

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

    public ref struct RTCRtpEncodingParameters sealed
    {
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

      RTCRtpEncodingParameters() { Active = true; }

      //Platform::String^ ToJsonString();
      //static RTCRtpEncodingParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCRtpParameters sealed
    {
      property Platform::String^                          MuxId;  //default = ""
      property IVector<RTCRtpCodecParameters^>^           Codecs;
      property IVector<RTCRtpHeaderExtensionParameters^>^ HeaderExtensions;
      property IVector<RTCRtpEncodingParameters^>^        Encodings;
      property RTCRtcpParameters^                         Rtcp;
      property RTCDegradationPreference                   DegradationPreference;  //default = MaintainBalanced

      //Platform::String^ ToJsonString();
      //static RTCRtpParameters^ FromJsonString(Platform::String^ jsonString);
    };
  }
}
