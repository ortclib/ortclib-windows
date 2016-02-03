#pragma once
#include <collection.h>
#include <ortc\IRTPTypes.h>

using Windows::Foundation::Collections::IVector;

using namespace ortc;

namespace ortc_winrt_api
{
  public ref class  RTCRtcpFeedback sealed
  {
  public:
    property Platform::String^ Type;
    property Platform::String^ Parameter;
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
  };

  public ref class RTCRtpHeaderExtensions sealed
  {
  public:
    property Platform::String^    Kind;
    property Platform::String^    Uri;
    property uint16               PreferredId;
    property Platform::Boolean    PreferredEncrypt; //default = false;
  };

  public ref class RTCRtpCapabilities sealed
  {
    friend class PushNativePointer;
  public:
    property IVector<RTCRtpCodecCapability^>^     Codecs;
    property IVector<RTCRtpHeaderExtensions^>^    HeaderExtensions;
    property IVector<Platform::String^>^          FecMechanisms;

  public:
    Platform::String^ ToJsonString();
    static RTCRtpCapabilities^ FromJsonString(Platform::String^ jsonString);

  private:
    IRTPTypes::CapabilitiesPtr mNativePointer;
  };


  public ref class RTCRtpCodecParameters sealed
  {
  public:
    property Platform::String^          Name;
    property uint8                      PayloadType;
    property uint32                     ClockRate;
    property uint32                     Maxptime;
    property uint32                     NumChannels;
    property IVector<RTCRtcpFeedback^>^ RtcpFeedback;
    property Object^                    Parameters;
  };

  public ref class  RTCRtpHeaderExtensionParameters sealed
  {
  public:
    property Platform::String^  Uri;
    property uint16             Id;
    property Platform::Boolean  Encrypt; //default = false;
  };

  public ref class  RTCRtcpParameters sealed
  {
  public:
    property uint32               Ssrc;
    property Platform::String^    CName;
    property Platform::Boolean    ReducedSize; //default = false
    property Platform::Boolean    Mux; //default = true
  };

  public enum class RTCDegradationPreference
  {
    MaintainFramerate,
    MaintainResolution,
    MaintainBalanced
  };

  public ref class RTCRtpFecParameters sealed
  {
  public:
    property uint32                Ssrc;
    property Platform::String^     Mechanism;
  };

  public ref class RTCRtpRtxParameters sealed
  {
  public:
    property uint32             Ssrc;
    property uint8              PayloadType;
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
    property uint32                       Ssrc;
    property uint8                        CodecPayloadType;
    property RTCRtpFecParameters^         Fec;
    property RTCRtpRtxParameters^         Rtx;
    property RTCPriorityType              Priority;
    property uint32                       MaxBitrate;
    property float64                      MinQuality; //default = 0;
    property float64                      ResolutionScale;
    property float64                      FramerateScale;
    property Platform::Boolean            Active; //default = true;
    property Platform::String^            EncodingId;
    property IVector<Platform::String^>^  DependencyEncodingIds;
  };

  public  ref  class RTCRtpParameters sealed
  {
  public:
    property Platform::String^                          MuxId;  //default = ""
    property IVector<RTCRtpCodecParameters^>^           Codecs;
    property IVector<RTCRtpHeaderExtensionParameters^>^ HeaderExtensions;
    property IVector<RTCRtpEncodingParameters^>^        Encodings;
    property RTCRtcpParameters^                         Rtcp;
    property RTCDegradationPreference                   DegradationPreference;  //default = MaintainBalanced
  };

}
