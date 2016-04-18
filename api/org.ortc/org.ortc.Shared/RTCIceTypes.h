#pragma once

#include <ortc/IICETypes.h>

//#include <ortc/IICEGatherer.h>
//#include <collection.h>
//
//using namespace ortc;
//
//using Windows::Foundation::Collections::IVector;


namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETypes, IICETypes)

    ref struct RTCIceCandidate;
    ref struct RTCIceCandidateComplete;
    ref struct RTCIceParameters;

    namespace internal
    {
      RTCIceCandidate^ ToCx(IICETypes::CandidatePtr candidate);
      RTCIceCandidate^ ToCx(const IICETypes::Candidate &candidate);
      IICETypes::CandidatePtr FromCx(RTCIceCandidate^ candidate);

      RTCIceCandidateComplete^ ToCx(IICETypes::CandidateCompletePtr candidate);
      RTCIceCandidateComplete^ ToCx(const IICETypes::CandidateComplete &candidate);
      IICETypes::CandidateCompletePtr FromCx(RTCIceCandidateComplete^ candidate);

      RTCIceParameters^ ToCx(const IICETypes::Parameters &input);
      RTCIceParameters^ ToCx(IICETypes::ParametersPtr input);
      IICETypes::ParametersPtr FromCx(RTCIceParameters^ input);
    } // namespace internal

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

    public ref struct RTCIceCandidate sealed
    {
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

      Platform::String^ ToJsonString();
      static RTCIceCandidate^ FromJsonString(Platform::String^ jsonString);
    };

    public ref struct RTCIceCandidateComplete sealed
    {
      property Platform::Boolean Complete;
    };

    public ref struct RTCIceParameters sealed
    {
      property Platform::Boolean  UseCandidateFreezePriority;
      property Platform::String^  UsernameFragment;
      property Platform::String^  Password;
      property Platform::Boolean  IceLite;

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
  } // namespace ortc
} // namespace org
