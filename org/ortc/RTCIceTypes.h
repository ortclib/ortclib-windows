#pragma once

#include <ortc/IICETypes.h>

//#include <ortc/IICEGatherer.h>
//#include <collection.h>
//
//using namespace ortc;
//
//using Windows::Foundation::Collections::IVector;


namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETypes, IICETypes)

    ref struct RTCIceCandidate;
    ref struct RTCIceCandidateComplete;
    ref struct RTCIceParameters;

    namespace Internal
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

    /// <summary>
    /// RTCIceRole contains the current role of the ICE transport.
    /// </summary>
    public enum class RTCIceRole
    {
      /// <summary>
      /// The controlling state, defined in [RFC5245] Section 3.
      /// </summary>
      Controlling,
      /// <summary>
      /// The controlled state, defined in [RFC5245] Section 3.
      /// </summary>
      Controlled
    };

    /// <summary>
    /// RTCIceComponent contains the component-id of the RTCIceTransport,
    /// which will be "RTP" unless RTP and RTCP are not multiplexed and the
    /// RTCIceTransport object was returned by createAssociatedTransport().
    /// </summary>
    public enum class RTCIceComponent
    {
      /// <summary>
      /// The RTP component ID, defined (as '1') in [RFC5245] Section 4.1.1.1.
      /// Protocols multiplexed with RTP (e.g. SCTP data channel) share its
      /// component ID.
      /// </summary>
      Rtp = 1,
      /// <summary>
      /// The RTCP component ID, defined (as '2') in [RFC5245] Section
      /// 4.1.1.1.
      /// </summary>
      Rtcp = 2
    };

    /// <summary>
    /// The RTCIceProtocol includes the protocol of the ICE candidate.
    /// </summary>
    public enum class RTCIceProtocol
    {
      /// <summary>
      /// A UDP candidate, as described in [RFC5245].
      /// </summary>
      Udp,
      /// <summary>
      /// A TCP candidate, as described in [RFC5245].
      /// </summary>
      Tcp
    };

    /// <summary>
    /// The RTCIceCandidateType includes the type of the ICE candidate.
    /// </summary>
    public enum class RTCIceCandidateType
    {
      /// <summary>
      /// A host candidate, defined in [RFC5245] Section 3.
      /// </summary>
      Host,
      /// <summary>
      /// A server reflexive candidate, defined in [RFC5245] Section 3.
      /// </summary>
      Srflex,
      /// <summary>
      /// A peer reflexive candidate, defined in [RFC5245] Section 3.
      /// </summary>
      Prflx,
      /// <summary>
      /// A relay candidate, defined in [RFC5245] Section 3.
      /// </summary>
      Relay
    };

    /// <summary>
    /// The RTCIceTcpCandidateType includes the type of the ICE TCP candidate,
    /// as described in [RFC6544]. 
    /// </summary>
    public enum class RTCIceTcpCandidateType
    {
      /// <summary>
      /// An active TCP candidate is one for which the transport will attempt
      /// to open an outbound connection but will not receive incoming
      /// connection requests.
      /// </summary>
      Active,
      /// <summary>
      /// A passive TCP candidate is one for which the transport will receive
      /// incoming connection attempts but not attempt a connection.
      /// </summary>
      Passive,
      /// <summary>
      /// An so candidate is one for which the transport will attempt to open
      /// a connection simultaneously with its peer.
      /// </summary>
      So
    };

    /// <summary>
    /// The RTCIceCandidate object includes information relating to an ICE
    /// candidate.
    /// </summary>
    public ref struct RTCIceCandidate sealed
    {
      /// <summary>
      /// Gets or sets the type of interface this candidate was gathered from.
      /// </summary>
      property Platform::String^            InterfaceType;
      /// <summary>
      /// Gets or sets a unique identifier that allows ICE to correlate
      /// candidates that appear on multiple RTCIceTransports.
      /// </summary>
      property Platform::String^            Foundation;
      /// <summary>
      /// Gets or sets the the component of the ICE candidate to indicate
      /// if the candidate is related to RTP or RTCP.
      /// </summary>
      property RTCIceComponent              Component;
      /// <summary>
      /// Gets or sets the assigned usage priority of the candidate. This
      /// priority represents the priority to choose this candidate over other
      /// candidates as described in [RFC5245]. If
      /// RTCIceParameters.UseUnfreezePriority is false then this priority
      /// also represents the priority in which to unfreeze ICE candidates.
      /// This is automatically populated by the RTC engine and/or browser.
      /// </summary>
      property uint32                       Priority;
      /// <summary>
      /// Gets or sets the unfreeze priority. If
      /// RTCIceParameters.UseUnfreezePriority is true then this priority is
      /// used to determine the unfreezing search order in which to check ICE
      /// candidates. The higher priority candidates will be searched as
      /// posssible pairings before lower priority candidates. This is a
      /// useful option as setting up a connection via a relay may be faster
      /// than exhaustively searching for the most optimal candidate pairing
      /// which is determined by standard Priority.
      /// </summary>
      /// <see cref="RTCIceParameters" />
      property uint32                       UnfreezePriority;
      /// <summary>
      /// Gets or sets the protocol of the candidate (UDP/TCP).
      /// </summary>
      property RTCIceProtocol               Protocol;
      /// <summary>
      /// Gets or sets the IP address of the candidate.
      /// </summary>
      property Platform::String^            Ip;
      /// <summary>
      /// Gets or sets the port for the candidate.
      /// </summary>
      property uint16                       Port;
      /// <summary>
      /// Gets or sets the type of candidate.
      /// </summary>
      property RTCIceCandidateType          CandidateType;
      /// <summary>
      /// Gets or sets the type of TCP candidate.
      /// </summary>
      property RTCIceTcpCandidateType       TcpType;
      /// <summary>
      /// Gets or sets the related address if not a host candidate type. For
      /// candidates that are derived from others, such as relay or reflexive
      /// candidates, the relatedAddress refers to the candidate that these
      /// are derived from. For host candidates, the RelatedAddress is set to
      /// the empty string.
      /// </summary>
      property Platform::String^            RelatedAddress;
      /// <summary>
      /// Gets or sets the related port if not a host candidate type. For
      /// candidates that are derived from others, such as relay or reflexive
      /// candidates, the relatedPort refers to the host candidate that these
      /// are derived from. For host candidates, the RelatedPort is null.
      /// </summary>
      property uint16                       RelatedPort;

      /// <summary>
      /// A helper routine to easily convery from an RTCIceCandidate to a JSON
      /// string.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// A helper routine to easily converty from a JSON string to an
      /// RTCIceCandidate object.
      /// </summary>
      static RTCIceCandidate^ FromJsonString(Platform::String^ jsonString);

      /// <summary>
      /// Constructs an instance of an RTCIceCandidate object.
      /// </summary>
      RTCIceCandidate()
      {
        Component = RTCIceComponent::Rtp;
        Priority = 0;
        UnfreezePriority = 0;
        Protocol = RTCIceProtocol::Udp;
        CandidateType = RTCIceCandidateType::Host;
        TcpType = RTCIceTcpCandidateType::Passive;
        Port = 0;
        RelatedPort = 0;
      }
    };

    /// <summary>
    /// This is the final ICE candidate emitted from the RTCIceGatherer. Or,
    /// in continuouos gathering mode, the final candidate "for now" after
    /// some kind of network change occurs where new candidates were
    /// discovered.
    /// </summary>
    public ref struct RTCIceCandidateComplete sealed
    {
      /// <summary>
      /// Constructs an instance of an RTCIceCandidateComplete object.
      /// </summary>
      RTCIceCandidateComplete()
      {
        Component = RTCIceComponent::Rtp;
        Complete = true;
      }

      /// <summary>
      /// Gets or sets the the component of the ICE candidate to indicate
      /// if the candidate is related to RTP or RTCP.
      /// </summary>
      property RTCIceComponent              Component;
      /// <summary>
      /// This attribute is always present and set to true, indicating that
      /// ICE candidate gathering is complete.
      /// </summary>
      property Platform::Boolean Complete;
    };

    /// <summary>
    /// The local RTCIceParameters object includes the ICE username fragment
    /// and password and other ICE-related parameters. The RTCIceParameters
    /// object corresponding to a remote peer may also include an iceLite
    /// attribute (set to "true" if the remote peer only supports ICE-lite).
    /// </summary>
    public ref struct RTCIceParameters sealed
    {
      /// <summary>
      /// Set this value to true if the remote ICE implementation also
      /// supports the separation of canddiate choise priorty versus
      /// the priority to unfreeze the candidates. This allows the ICE engine
      /// to search for a candidate that is more likely to work before trying
      /// to find the most optimal candidate to use long term.
      /// </summary>
      property Platform::Boolean  UseUnfreezePriority;
      /// <summary>
      /// The ICE username fragment.
      /// </summary>
      property Platform::String^  UsernameFragment;
      /// <summary>
      /// The ICE password.
      /// </summary>
      property Platform::String^  Password;
      /// <summary>
      /// If only ICE-lite is supported (true) or not (false or unset). Since
      /// [draft-ietf-rtcweb-transports] Section 3.4 requires client RTC
      /// engines/browsers support for full ICE, IceLite will only be "true"
      /// for a remote peer such as a gateway. GetLocalParameters().IceLite
      /// will not return true.
      /// </summary>
      property Platform::Boolean  IceLite;

      /// <summary>
      /// A helper routine to easily convery from an RTCIceParameters to a
      /// JSON string.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// A helper routine to easily converty from a JSON string to an
      /// RTCIceParameters object.
      /// </summary>
      static RTCIceParameters^ FromJsonString(Platform::String^ jsonString);
    };

  } // namespace ortc
} // namespace org
