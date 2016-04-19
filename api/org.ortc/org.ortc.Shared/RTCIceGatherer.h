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

    public enum class RTCIceGatherPolicy
    {
      None = 0,
      NoIPv4Host = 0x00000001,
      NoIPv4Srflx = 0x00000002,
      NoIPv4Prflx = 0x00000004,
      NoIPv4Relay = 0x00000008,
      NoIPv4Private = 0x00000010,
      NoIPv4 = 0x000000FF,
      NoIPv6Host = 0x00000100,
      NoIPv6Srflx = 0x00000200,
      NoIPv6Prflx = 0x00000400,
      NoIPv6Relay = 0x00000800,
      NoIPv6Private = 0x00001000,
      NoIPv6Tunnel = 0x00002000,
      NoIPv6Permanent = 0x00004000,
      NoIPv6 = 0x0000FF00,
      NoHost = (NoIPv4Host | NoIPv6Host),
      NoSrflx = (NoIPv4Srflx | NoIPv6Srflx),
      NoPrflx = (NoIPv4Prflx | NoIPv6Prflx),
      NoRelay = (NoIPv4Relay | NoIPv6Relay),
      NoPrivate = (NoIPv4Private | NoIPv6Private),
      RelayOnly = (NoIPv4Host | NoSrflx | NoPrflx),
      NoCandidates = (0x7FFFFFFF)
    };

    public enum class RTCIceGathererCredentialType
    {
      Password,
      Token,
    };

    public enum class RTCIceGathererState
    {
      New,
      Gathering,
      Complete,
      Closed,
    };

    public ref struct RTCIceServer sealed
    {
      property IVector<Platform::String^>^      Urls;
      property Platform::String^                UserName;
      property Platform::String^                Credential;
      property RTCIceGathererCredentialType     CredentialType;
    };

    public ref struct RTCIceInterfacePolicy sealed
    {
      property Platform::String^                InterfaceType;
      property RTCIceGatherPolicy               GatherPolicy;
    };

    public ref struct RTCIceGatherOptions sealed
    {
      property Platform::Boolean                ContinuousGathering;
      property IVector<RTCIceInterfacePolicy^>^ InterfacePolicies;
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
      /// Convert from an RTCIceGatherPolicy to a string.
      /// </summary>
      static Platform::String^ IceGatherPolicyToString(RTCIceGatherPolicy value);

      /// <summary>
      /// Convert from a string to a RTCIceGatherPolicy.
      /// </summary>
      static RTCIceGatherPolicy ToIceGatherPolicy(Platform::String^ str);

    private:
      zsLib::RecursiveLock _lock;
      IICEGathererPtr _nativePointer;
      internal::RTCIceGathererDelegatePtr _nativeDelegatePointer;
    };

#pragma endregion

  }
}
