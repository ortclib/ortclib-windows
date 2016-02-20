#pragma once

#include <ortc/IICEGatherer.h>
#include <collection.h>

using namespace ortc;

using Windows::Foundation::Collections::IVector;


namespace ortc_winrt_api
{
  ref class RTCIceCandidate;

  public enum class RTCIceRole
  {
    Controlling,
    Controlled
  };

  public enum class RTCIceComponent
  {
    Rtp,
    Rtcp
  };

  public enum class RTCIceProtocol
  {
    Udp,
    Tcp
  };

  public enum class RTCIceCandidateType {
    Host,
    Srflex,
    Prflx,
    Relay,
  };

  public enum class RTCIceTcpCandidateType {
    Active,
    Passive,
    So,
  };

  public ref class RTCIceCandidate sealed
  {
  public:
    property Platform::String^            InterfaceType;
    property Platform::String^            Foundation;
    property uint32                       Priority;
    property uint32                       UnfreezePriority;
    property RTCIceProtocol               Protocol;
    property Platform::String^            Ip;
    property uint16                       Port;
    property RTCIceCandidateType          CandidateType;
    property RTCIceTcpCandidateType       TcpType;
    property Platform::String^            RelatedAddress;
    property uint16                       RelatedPort;
  public:
    Platform::String^ ToJsonString();
    static RTCIceCandidate^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCIceCandidateComplete sealed
  {
  public:
    property Platform::Boolean Complete;
  };

  public ref class RTCIceParameters sealed
  {
  public:
    property Platform::Boolean  UseCandidateFreezePriority;
    property Platform::String^  UsernameFragment;
    property Platform::String^  Password;
    property Platform::Boolean  IceLite;

  public:
    Platform::String^ ToJsonString();
    static RTCIceParameters^ FromJsonString(Platform::String^ jsonString);
  };

  public ref class RTCIceTypes sealed
  {
  public:
    [Windows::Foundation::Metadata::DefaultOverloadAttribute]
    static Platform::String^ ToString();
    [Windows::Foundation::Metadata::OverloadAttribute("IceRoleToString")]
    static Platform::String^ ToString(RTCIceRole value);
    [Windows::Foundation::Metadata::OverloadAttribute("IceComponentToString")]
    static Platform::String^ ToString(RTCIceComponent value);
    [Windows::Foundation::Metadata::OverloadAttribute("IceProtocolToString")]
    static Platform::String^ ToString(RTCIceProtocol value);
    [Windows::Foundation::Metadata::OverloadAttribute("IceCandidateTypeToString")]
    static Platform::String^ ToString(RTCIceCandidateType value);
    [Windows::Foundation::Metadata::OverloadAttribute("IceTcpCandidateTypeToString")]
    static Platform::String^ ToString(RTCIceTcpCandidateType value);

    static RTCIceRole ToRole(Platform::String^ str);
    static RTCIceComponent ToComponent(Platform::String^ str);
    static RTCIceProtocol ToProtocol(Platform::String^ str);
    static RTCIceCandidateType ToCandidateType(Platform::String^ str);
    static RTCIceTcpCandidateType ToTcpCandidateType(Platform::String^ str);
  };
} // namespace 
