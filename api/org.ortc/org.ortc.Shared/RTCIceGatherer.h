#pragma once

#include "RTCIceTypes.h"

#include <ortc/IICEGatherer.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICEGatherer, IICEGatherer)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICEGathererDelegate, IICEGathererDelegate)

    using Windows::Foundation::Collections::IVector;

    ref class RTCIceGatherer;
    ref class RTCIceTransport;

    ref struct RTCIceCandidate;
    ref struct RTCIceCandidateComplete;
    ref struct RTCIceInterfacePolicy;
    ref struct RTCIceGatherOptions;
    ref struct RTCIceParameters;
    ref struct RTCIceServer;

    namespace internal
    {
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IICEGathererTypes, IICEGathererTypes)
      ZS_DECLARE_CLASS_PTR(RTCIceGathererDelegate)
        
      RTCIceInterfacePolicy^ ToCx(const IICEGathererTypes::InterfacePolicy &input);
      RTCIceInterfacePolicy^ ToCx(IICEGathererTypes::InterfacePolicyPtr input);
      IICEGathererTypes::InterfacePolicyPtr FromCx(RTCIceInterfacePolicy^ input);

      RTCIceGatherOptions^ ToCx(const IICEGathererTypes::Options &input);
      RTCIceGatherOptions^ ToCx(IICEGathererTypes::OptionsPtr input);
      IICEGathererTypes::OptionsPtr FromCx(RTCIceGatherOptions^ options);

      RTCIceServer^ ToCx(const IICEGathererTypes::Server &input);
      RTCIceServer^ ToCx(IICEGathererTypes::ServerPtr input);
      IICEGathererTypes::ServerPtr FromCx(RTCIceServer^ input);
    }

    /// <summary>
    /// RTCIceGatherFilterPolicy represents the detailed gatherer filter
    /// options to restrict ICE candidate discovery.
    /// </summary>
    public enum class RTCIceGatherFilterPolicy
    {
      /// <summary>
      /// No gatherer filtering is required.
      /// </summary>
      None = 0,
      /// <summary>
      /// Filter all IPv4 host candidates.
      /// </summary>
      NoIPv4Host = 0x00000001,
      /// <summary>
      /// Filter all IPv4 server reflexive candidates.
      /// </summary>
      NoIPv4Srflx = 0x00000002,
      /// <summary>
      /// Filter all IPv4 peer reflexive candidates.
      /// </summary>
      NoIPv4Prflx = 0x00000004,
      /// <summary>
      /// Filter all IPv4 relay candidates.
      /// </summary>
      NoIPv4Relay = 0x00000008,
      /// <summary>
      /// Filter all IPv4 private candidates.
      /// See: https://en.wikipedia.org/wiki/Private_network
      /// </summary>
      NoIPv4Private = 0x00000010,
      /// <summary>
      /// Filter all IPv4 candidates.
      /// </summary>
      NoIPv4 = 0x000000FF,
      /// <summary>
      /// Filter all IPv6 host candidates.
      /// </summary>
      NoIPv6Host = 0x00000100,
      /// <summary>
      /// Filter all IPv6 server reflexive candidates.
      /// </summary>
      NoIPv6Srflx = 0x00000200,
      /// <summary>
      /// Filter all IPv6 peer reflexive candidates.
      /// </summary>
      NoIPv6Prflx = 0x00000400,
      /// <summary>
      /// Filter all IPv6 relay candidates.
      /// </summary>
      NoIPv6Relay = 0x00000800,
      /// <summary>
      /// Filter all IPv6 private candidates.
      /// See: https://en.wikipedia.org/wiki/Private_network
      /// </summary>
      NoIPv6Private = 0x00001000,
      /// <summary>
      /// Filter all IPv6 Teredo tunnel or other tunneled candidates.
      /// </summary>
      NoIPv6Tunnel = 0x00002000,
      /// <summary>
      /// Filter all IPv6 permanent candidates.
      /// </summary>
      NoIPv6Permanent = 0x00004000,
      /// <summary>
      /// Filter all IPv6 candidates.
      /// </summary>
      NoIPv6 = 0x0000FF00,
      /// <summary>
      /// Filter all host candidates.
      /// </summary>
      NoHost = (NoIPv4Host | NoIPv6Host),
      /// <summary>
      /// Filter all server reflexive candidates.
      /// </summary>
      NoSrflx = (NoIPv4Srflx | NoIPv6Srflx),
      /// <summary>
      /// Filter all server peer reflexive candidates.
      /// </summary>
      NoPrflx = (NoIPv4Prflx | NoIPv6Prflx),
      /// <summary>
      /// Filter all relay candidates.
      /// </summary>
      NoRelay = (NoIPv4Relay | NoIPv6Relay),
      /// <summary>
      /// Filter all private candidates.
      /// See: https://en.wikipedia.org/wiki/Private_network
      /// </summary>
      NoPrivate = (NoIPv4Private | NoIPv6Private),
      /// <summary>
      /// Filter all but relay candidates.
      /// </summary>
      RelayOnly = (NoIPv4Host | NoSrflx | NoPrflx),
      /// <summary>
      /// Filter all candidates.
      /// </summary>
      NoCandidates = (0x7FFFFFFF)
    };

    /// <summary>
    /// Use the string form of these enums with the RTCIceInterfacePolicy
    /// Interface type variable to specify per interface type policies.
    /// </summary>
    public enum class RTCIceGatherInterfaceType
    {
      /// <summary>
      /// Applies to all unknown interface types.
      /// </summary>
      Unknown,
      /// <summary>
      /// Applies to all interface types where there is no specific interface
      /// type policy was specified.
      /// </summary>
      Default,
      /// <summary>
      /// Applies to all Local Area Network interface types.
      /// </summary>
      Lan,
      /// <summary>
      /// Applies to all Tunnel interface types.
      /// </summary>
      Tunnel,
      /// <summary>
      /// Applies to all Wireless Local Area Network interface types.
      /// </summary>
      Wlan,
      /// <summary>
      /// Applies to all Wireless Wide Area Network interface types.
      /// </summary>
      Wwan,
      /// <summary>
      /// Applies to all Virtual Private Network interface types.
      /// </summary>
      Vpn
    };

    /// <summary>
    /// RTCIceCredentialType represents the type of credential used by a TURN
    /// server.
    /// </summary>
    public enum class RTCIceGathererCredentialType
    {
      /// <summary>
      /// The credential is a long-term authentication password.
      /// </summary>
      Password,
      /// <summary>
      /// The credential is an access token.
      /// </summary>
      Token,
    };

    /// <summary>
    /// RTCIceGathererState represents the current state of the ICE gatherer.
    /// </summary>
    public enum class RTCIceGathererState
    {
      /// <summary>
      /// The object has been created but Gather() has not been called.
      /// </summary>
      New,
      /// <summary>
      /// Gather() has been called, and the RTCIceGatherer is in the process
      /// of gathering candidates (which includes adding new candidates and
      /// removing invalidated candidates).
      /// </summary>
      Gathering,
      /// <summary>
      /// The RTCIceGatherer has completed gathering. Events such as adding,
      /// updating or removing an interface, or adding, changing or removing a
      /// TURN server will cause the state to go back to gathering before
      /// re-entering "complete" once all candidate changes are finalized.
      /// </summary>
      Complete,
      /// <summary>
      /// The RTCIceGatherer has been closed intentionally (by calling
      /// Close()) or as the result of an error.
      /// </summary>
      Closed,
    };

    /// <summary>
    /// </summary>
    public ref struct RTCIceServer sealed
    {
      /// <summary>
      /// STUN or TURN URI(s) as defined in [RFC7064] and [RFC7065] or other
      /// URI types.
      /// </summary>
      property IVector<Platform::String^>^      Urls;
      /// <summary>
      /// Gets or sets the username to use with that TURN server, if this
      /// RTCIceServer object represents a TURN Server.
      /// </summary>
      property Platform::String^                UserName;
      /// <summary>
      /// Gets or sets the TURN server credential, if this RTCIceServer object
      /// represents a TURN Server.
      /// </summary>
      property Platform::String^                Credential;
      /// <summary>
      /// Gets or sets how credential should be used when that TURN server
      /// requests authorization, if this RTCIceServer object represents a
      /// TURN Server.
      /// </summary>
      property RTCIceGathererCredentialType     CredentialType;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCIceInterfacePolicy sealed
    {
      /// <summary>
      /// Gets or sets the string form of the RTCIceGatherInterfaceType enums
      /// to specify which interface types this policy object applies.
      /// </summary>
      property Platform::String^                InterfaceType;
      /// <summary>
      /// Gets or sets the candidate filter policy to apply to the specified
      /// interface types.
      /// </summary>
      property RTCIceGatherFilterPolicy         GatherPolicy;
    };

    /// <summary>
    /// RTCIceGatherOptions provides options relating to the gathering of ICE
    /// candidates.
    /// </summary>
    public ref struct RTCIceGatherOptions sealed
    {
      /// <summary>
      /// Gets or sets the option to specify continous gathering mode.
      ///
      /// If true, then the RTCIceGatherer will gather all candidates
      /// available "for now" and fire a RTCIceGathererCandidateCompleteEvent.
      /// However, should more candidates be discovered or Gather() get called
      /// again after the the RTCIceGathererCandidateCompleteEvent then the
      /// RTCIceGatherer is allowed to go back into the Gatherering state and
      /// fire newly discovered candidates until another fired
      /// RTCIceGathererCandidateCompleteEvent occurs.
      ///
      /// If false, then the RTCIceGatherer will not be able to discover any
      /// new candidates after going into the Complete state, or go back
      /// into the Gatherering state.
      /// </summary>
      property Platform::Boolean                ContinuousGathering;
      /// <summary>
      /// Gets or sets the ICE gather policy per interface type.
      /// </summary>
      property IVector<RTCIceInterfacePolicy^>^ InterfacePolicies;
      /// <summary>
      /// Gets or sets additional ICE servers to be configured. Since
      /// implementations may provide default ICE servers, and applications
      /// can desire to restrict communications to the local LAN, iceServers
      /// need not be set.
      /// </summary>
      property IVector<RTCIceServer^>^          IceServers;

      RTCIceGatherOptions() { ContinuousGathering = true; }
    };

#pragma region RTCIceGatherer events

    /// <summary>
    /// This object represents the error event when the RTCIceGatherer object
    /// has experienced an ICE gathering failure (such as an authentication
    /// failure with TURN credentials).
    /// </summary>
    public ref struct RTCIceGathererIceErrorEvent sealed
    {
      friend class internal::RTCIceGathererDelegate;

      /// <summary>
      /// Gets the local IP address and port used to communicate with the
      /// STUN or TURN server.
      /// </summary>
      property RTCIceCandidate^ HostCandidate
      {
        RTCIceCandidate^ get() { return _hostCandidate; }
      }
      /// <summary>
      /// Gets the STUN or TURN URL identifying the server on which the
      /// failure ocurred.
      /// </summary>
      property Platform::String^ Url
      {
        Platform::String^ get() { return _url; }
      }
      /// <summary>
      /// Gets the numeric STUN error code returned by the STUN or TURN
      /// server.
      /// </summary>
      property uint16            ErrorCode
      {
        uint16 get() { return _errorCode; }
      }
      /// <summary>
      /// Gets the STUN reason text returned by the STUN or TURN server.
      /// </summary>
      property Platform::String^ ErrorText
      {
        Platform::String^ get() { return _errorText; }
      }

    private:
      RTCIceCandidate^  _hostCandidate;
      Platform::String^ _url;
      uint16            _errorCode;
      Platform::String^ _errorText;
    };

    /// <summary>
    /// This object represents the event for when the RTCIceGathererState
    /// changed.
    /// </summary>
    public ref struct RTCIceGathererStateChangedEvent sealed
    {
      friend class internal::RTCIceGathererDelegate;

      /// <summary>
      /// Gets the new RTCIceGathererState that caused the event.
      /// </summary>
      property RTCIceGathererState State
      {
        RTCIceGathererState  get() { return _state; }
      }

    private:
      RTCIceGathererState _state;
    };

    /// <summary>
    /// This object represents the event for when a new RTCIceGatherCandidate
    /// is made available (or is gone).
    /// </summary>
    public ref struct RTCIceGathererCandidateEvent sealed
    {
      friend class internal::RTCIceGathererDelegate;

      /// <summary>
      /// Gets the ICE candidate that caused the event.
      /// </summary>
      property RTCIceCandidate^ Candidate
      {
        RTCIceCandidate^  get() { return _candidate; }
      }
      /// <summary>
      /// Gets the URL of the server from which the candidate was obtained.
      /// </summary>
      property Platform::String^ Url
      {
        Platform::String^ get() { return _url; }
      }

    private:
      RTCIceCandidate^ _candidate;
      Platform::String^ _url;
    };

    /// <summary>
    /// This object represents the event for when a new
    /// RTCIceCandidateComplete is made available.
    /// </summary>
    public ref struct RTCIceGathererCandidateCompleteEvent sealed
    {
      friend class internal::RTCIceGathererDelegate;

      /// <summary>
      /// Gets the ICE candidate that caused the event.
      /// </summary>
      property RTCIceCandidateComplete^ Candidate
      {
        RTCIceCandidateComplete^  get() { return _candidate; }
      }

    private:
      RTCIceCandidateComplete^ _candidate;
    };

#pragma endregion

    public delegate void RTCIceGathererLocalCandidateDelegate(RTCIceGathererCandidateEvent^ evt);
    public delegate void RTCIceGathererLocalCandidateGoneDelegate(RTCIceGathererCandidateEvent^ evt);
    public delegate void RTCIceGathererLocalCandidateCompleteDelegate(RTCIceGathererCandidateCompleteEvent^ evt);
    public delegate void RTCIceGathererErrorDelegate(RTCIceGathererIceErrorEvent^ evt);
    public delegate void RTCIceGathererStateChangedDelegate(RTCIceGathererStateChangedEvent^ evt);

#pragma region RTCIceGatherer

    /// <summary>
    /// The RTCIceGatherer gathers local host, server reflexive and relay
    /// candidates, as well as enabling the retrieval of local Interactive
    /// Connectivity Establishment (ICE) parameters which can be exchanged in
    /// signaling. By enabling an endpoint to use a set of local candidates to
    /// construct multiple RTCIceTransport objects, the RTCIceGatherer enables
    /// support for scenarios such as parallel forking.
    /// </summary>
    public ref class RTCIceGatherer sealed
    {
      friend class internal::RTCIceGathererDelegate;
      friend ref class RTCIceTransport;

    private:
      RTCIceGatherer();

      static RTCIceGatherer^ Convert(IICEGathererPtr gatherer);
      static IICEGathererPtr Convert(RTCIceGatherer^ gatherer) { if (!gatherer) return nullptr; return gatherer->_nativePointer; }

    public:
      /// <summary>
      /// Constructs an instance of an RTCIceGatherer.
      /// </summary>
      RTCIceGatherer(RTCIceGatherOptions^ options);

      /// <summary>
      /// Obtain the ICE parameters of the RTCIceGatherer.
      /// </summary>
      RTCIceParameters^ GetLocalParameters();
      /// <summary>
      /// Retrieve the sequence of valid local candidates associated with the
      /// RTCIceGatherer. This retrieves all unpruned local candidates
      /// currently known (except for peer reflexive candidates), even if an
      /// onlocalcandidate event hasn't been processed yet. Prior to calling
      /// Gather() an empty list will be returned.
      /// </summary>
      IVector<RTCIceCandidate^>^ GetLocalCandidates();
      /// <summary>
      /// Create an associated RTCIceGatherer for RTCP, with the same
      /// RTCIceParameters and RTCIceGatherOptions. If state is "closed",
      /// throw an InvalidStateError exception. If an RTCIceGatherer calls the
      /// method more than once, or if component is "RTCP", throw an
      /// InvalidStateError exception.
      /// </summary>
      RTCIceGatherer^ CreateAssociatedGatherer();

      /// <summary>
      /// Gather ICE candidates. If options is omitted, utilize the value of
      /// options passed in the constructor.
      /// </summary>
      void Gather(RTCIceGatherOptions^ options);

      /// <summary>
      /// Prunes all local candidates, and closes the port. Associated
      /// RTCIceTransport objects transition to the "disconnected" state
      /// (unless they were in the "failed" state). Calling close() when state
      /// is "closed" has no effect.
      /// </summary>
      void Close();

    public:

      /// <summary>
      /// Gets the component-id of the RTCIceGatherer object. In
      /// RTCIceGatherer objects returned by createAssociatedGatherer() the
      /// value of component is "RTCP". In all other RTCIceGatherer objects,
      /// the value of component is "RTP".
      /// </summary>
      property RTCIceComponent Component
      {
        RTCIceComponent get();
      }

      /// <summary>
      /// Gets the current state of the ICE gatherer.
      /// </summary>
      property RTCIceGathererState State
      {
        RTCIceGathererState get();
      }

      /// <summary>
      /// The RTCIceGathererState changed.
      /// </summary>
      event RTCIceGathererStateChangedDelegate^           OnStateChange;
      /// <summary>
      /// A new RTCIceGatherCandidate is made available.
      /// </summary>
      event RTCIceGathererLocalCandidateDelegate^         OnLocalCandidate;
      /// <summary>
      /// A new RTCIceGatherCandidateComplete is made available.
      /// </summary>
      event RTCIceGathererLocalCandidateCompleteDelegate^ OnLocalCandidateComplete;
      /// <summary>
      /// A new RTCIceGatherCandidate is no longer available.
      /// </summary>
      event RTCIceGathererLocalCandidateGoneDelegate^     OnLocalCandidateGone;
      /// <summary>
      /// The RTCIceGatherer object has experienced an ICE gathering failure
      /// (such as an authentication failure with TURN credentials).
      /// </summary>
      event RTCIceGathererErrorDelegate^                  OnError;

    public:
      /// <summary>
      /// Convert from an RTCIceGatherFilterPolicy to a string.
      /// </summary>
      static Platform::String^ IceGatherPolicyToString(RTCIceGatherFilterPolicy value);

      /// <summary>
      /// Convert from a string to a RTCIceGatherFilterPolicy.
      /// </summary>
      static RTCIceGatherFilterPolicy ToIceGatherPolicy(Platform::String^ str);

    private:
      zsLib::RecursiveLock _lock;
      IICEGathererPtr _nativePointer;
      internal::RTCIceGathererDelegatePtr _nativeDelegatePointer;
    };

#pragma endregion

  }
}
