#pragma once

#include "RTCIceTypes.h"

#include <ortc/IICETransport.h>

//#include <collection.h>
//
//using namespace ortc;
//

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransport, IICETransport)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransportDelegate, IICETransportDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransportSubscription, IICETransportSubscription)

    ref class RTCIceGatherer;
    ref class RTCIceTransport;
    ref class RTCIceTransportController;

    ref struct RTCIceCandidate;
    ref struct RTCIceCandidateComplete;
    ref struct RTCIceCandidatePair;
    ref struct RTCIceParameters;
    ref struct RTCIceTransportOptions;

    enum class RTCIceComponent;
    enum class RTCIceRole;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCIceTransportDelegate)
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransportTypes, IICETransportTypes)

      RTCIceCandidatePair^ ToCx(const IICETransportTypes::CandidatePair &input);
      RTCIceCandidatePair^ ToCx(IICETransportTypes::CandidatePairPtr input);
      IICETransportTypes::CandidatePairPtr FromCx(RTCIceCandidatePair^ input);

      RTCIceTransportOptions^ ToCx(const IICETransportTypes::Options &input);
      RTCIceTransportOptions^ ToCx(IICETransportTypes::OptionsPtr input);
      IICETransportTypes::OptionsPtr FromCx(RTCIceTransportOptions^ input);
    }

    /// <summary>
    /// RTCIceTransportState represents the current state of the ICE
    /// transport.
    /// </summary>
    public enum class RTCIceTransportState
    {
      /// <summary>
      /// The RTCIceTransport object is waiting for remote candidates to be
      /// supplied. In this state the object can respond to incoming
      /// connectivity checks.
      /// </summary>
      New,
      /// <summary>
      /// The RTCIceTransport has received at least one remote candidate, and
      /// a local and remote RTCIceCandidateComplete dictionary was not added
      /// as the last candidate. In this state the RTCIceTransport is checking
      /// candidate pairs but has not yet found a successful candidate pair,
      /// or consent checks [RFC7675] have failed on a previously successful
      /// candidate pair.
      /// </summary>
      Checking,
      /// <summary>
      /// The RTCIceTransport has received a response to an outgoing
      /// connectivity check, or has received incoming DTLS/media after a
      /// successful response to an incoming connectivity check, but is still
      /// checking other candidate pairs to see if there is a better
      /// connection. In this state outgoing media is permitted.
      /// </summary>
      Connected,
      /// <summary>
      /// A local and remote RTCIceCandidateComplete dictionary was added as
      /// the last candidate to the RTCIceTransport and all appropriate
      /// candidate pairs have been tested and at least one functioning
      /// candidate pair has been found.
      /// </summary>
      Completed,
      /// <summary>
      /// The RTCIceTransport has received at least one local and remote
      /// candidate, and a local and remote RTCIceCandidateComplete dictionary
      /// was not added as the last candidate, but all appropriate candidate
      /// pairs thus far have been tested and failed (or consent checks
      /// [RFC7675] once successful, have now failed). Other candidate pairs
      /// may become available for testing as new candidates are trickled, and
      /// therefore the "failed" state has not been reached.
      /// </summary>
      Disconnected,
      /// <summary>
      /// A local and remote RTCIceCandidateComplete dictionary was added as
      /// the last candidate to the RTCIceTransport and all appropriate
      /// candidate pairs have been tested and failed.
      /// </summary>
      Failed,
      /// <summary>
      /// The RTCIceTransport has shut down and is no longer responding to
      /// STUN requests.
      /// </summary>
      Closed,
    };

    /// <summary>
    /// The RTCIceTransportOptions represents the ICE transport options
    /// specified for the RTCIceTransport object.
    /// </summary>
    public ref struct RTCIceTransportOptions sealed
    {
      /// <summary>
      /// Gets or sets if aggressive ICE nomination should be used. If true,
      /// the RTCIceTransport will use aggressive ICE nomination. If false,
      /// the RTCICeTransport will use regular ICE nomination.
      /// </summary>
      property Platform::Boolean  AggressiveIce;
      /// <summary>
      /// Gets of sets the ICE role to use for the ICE transport.
      /// </summary>
      property RTCIceRole         Role;
    };

    /// <summary>
    /// The RTCIceCandidatePair contains the currently selected ICE candidate
    /// pair.
    /// </summary>
    public ref struct RTCIceCandidatePair sealed
    {
      friend class internal::RTCIceTransportDelegate;

      /// <summary>
      /// Gets or sets the local ICE candidate.
      /// </summary>
      property RTCIceCandidate^ Local;
      /// <summary>
      /// Gets or sets the remote ICE candidate.
      /// </summary>
      property RTCIceCandidate^ Remote;
    };

    /// <summary>
    /// This object represents the event data associated when the
    /// RTCIceTransportState changes.
    /// </summary>
    public ref struct RTCIceTransportStateChangedEvent sealed
    {
      friend class internal::RTCIceTransportDelegate;

      /// <summary>
      /// Gets the RTCIceTransportState state at the time the event fired.
      /// </summary>
      property RTCIceTransportState State
      {
        RTCIceTransportState  get() { return _state; }
      }

    private:
      RTCIceTransportState _state;
    };


    /// <summary>
    /// This object represents the candidate pair events changes the
    /// RTCIceTransport object uses the RTCIceCandidatePairChangedEvent
    /// interface.
    /// </summary>
    public ref struct RTCIceCandidatePairChangedEvent sealed
    {
      friend class internal::RTCIceTransportDelegate;

      /// <summary>
      /// Gets the selected RTCIceCandidatePair that caused the event.
      /// </summary>
      property RTCIceCandidatePair^ CandidatePair
      {
        RTCIceCandidatePair^  get() { return _candidatePair; }
      }

    private:
      RTCIceCandidatePair^ _candidatePair;
    };

    public delegate void RTCIceTransportStateChangedDelegate(RTCIceTransportStateChangedEvent^ evt);
    public delegate void RTCIceTransportCandidatePairAvailableDelegate(RTCIceCandidatePairChangedEvent^ evt);
    public delegate void RTCIceTransportCandidatePairGoneDelegate(RTCIceCandidatePairChangedEvent^ evt);
    public delegate void RTCIceTransportCandidatePairChangedDelegate(RTCIceCandidatePairChangedEvent^ evt);

    /// <summary>
    /// The RTCIceTransport includes information relating to Interactive
    /// Connectivity Establishment (ICE).
    /// </summary>
    public ref class RTCIceTransport sealed
    {
    private:
      struct noop {};
      friend class internal::RTCIceTransportDelegate;
      friend ref class RTCDtlsTransport;
      friend ref class RTCIceTransportController;

    private:
      RTCIceTransport(const noop &);

      static RTCIceTransport^ Convert(IICETransportPtr transport);
      static IICETransportPtr Convert(RTCIceTransport^ gatherer) { return gatherer->_nativePointer; }

    public:
      /// <summary>
      /// Constructs an instance of the RTCIceTransport object.
      /// </summary>
      RTCIceTransport();
      /// <summary>
      /// Constructs an instance of the RTCIceTransport object using the
      /// specified RTCIceGatherer. 
      /// </summary>
      RTCIceTransport(RTCIceGatherer^ Gatherer);

      /// <summary>
      /// Retrieve the sequence of candidates associated with the remote
      /// RTCIceTransport. Only returns the candidates previously added using
      /// SetRemoteCandidates() or AddRemoteCandidate(). If there are no
      /// remote candidates, an empty list is returned.
      /// </summary>
      IVector<RTCIceCandidate^>^ GetRemoteCandidates();
      /// <summary>
      /// Retrieves the selected candidate pair on which packets are sent. If
      /// there is no selected pair yet, or consent [RFC7675] is lost on the
      /// selected pair, nullptr is returned.
      /// </summary>
      RTCIceCandidatePair^       GetSelectedCandidatePair();
      /// <summary>
      /// The first time Start() is called, candidate connectivity checks are
      /// started and the ICE transport attempts to connect to the remote
      /// RTCIceTransport. If start() is called with invalid parameters, throw
      /// an InvalidParameters exception. For example, if gatherer.component
      /// has a value different from iceTransport.component, throw an
      /// InvalidParameters exception. If state or gatherer.state is "closed",
      /// throw an InvalidStateError exception. When start() is called again,
      /// RTCIceTransportState transitions to the "connected" state, all
      /// remote candidates are flushed, and AddRemoteCandidate() or
      /// SetRemoteCandidates() must be called to add the remote candidates
      /// back or replace them.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters);
      /// <summary>
      /// The first time Start() is called, candidate connectivity checks are
      /// started and the ICE transport attempts to connect to the remote
      /// RTCIceTransport. If start() is called with invalid parameters, throw
      /// an InvalidParameters exception. For example, if gatherer.component
      /// has a value different from iceTransport.component, throw an
      /// InvalidParameters exception. If state or gatherer.state is "closed",
      /// throw an InvalidStateError exception. When start() is called again,
      /// RTCIceTransportState transitions to the "connected" state, all
      /// remote candidates are flushed, and AddRemoteCandidate() or
      /// SetRemoteCandidates() must be called to add the remote candidates
      /// back or replace them.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("StartWithRole")]
      void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role);
      /// <summary>
      /// The first time Start() is called, candidate connectivity checks are
      /// started and the ICE transport attempts to connect to the remote
      /// RTCIceTransport. If start() is called with invalid parameters, throw
      /// an InvalidParameters exception. For example, if gatherer.component
      /// has a value different from iceTransport.component, throw an
      /// InvalidParameters exception. If state or gatherer.state is "closed",
      /// throw an InvalidStateError exception. When start() is called again,
      /// RTCIceTransportState transitions to the "connected" state, all
      /// remote candidates are flushed, and AddRemoteCandidate() or
      /// SetRemoteCandidates() must be called to add the remote candidates
      /// back or replace them.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("StartWithOptions")]
      void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceTransportOptions^ options);
      /// <summary>
      /// Stops and closes the current object. Also removes the object from
      /// the RTCIceTransportController. Calling stop() when state is "closed"
      /// has no effect.
      /// </summary>
      void                       Stop();
      /// <summary>
      /// Obtain the current ICE parameters of the remote RTCIceTransport.
      /// </summary>
      RTCIceParameters^          GetRemoteParameters();
      /// <summary>
      /// Create an associated RTCIceTransport for RTCP. If called more than
      /// once for the same component, or if state is "closed", throw an
      /// InvalidStateError exception. If called when component is "RTCP",
      /// throw an InvalidStateError exception.
      /// </summary>
      RTCIceTransport^           CreateAssociatedTransport();
      /// <summary>
      /// Add a remote candidate associated with the remote RTCIceTransport.
      /// If state is "closed", throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       AddRemoteCandidate(RTCIceCandidate^ remoteCandidate);
      /// <summary>
      /// When the remote RTCIceGatherer emits its final candidate,
      /// AddRemoteCandidate() should be called with an
      /// RTCIceCandidateComplete dictionary as an argument, so that the local
      /// RTCIceTransport can know there are no more remote candidates
      /// expected, and can enter the "completed" state.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("AddRemoteCandidateComplete")]
      void                       AddRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate);
      /// <summary>
      /// Set the sequence of candidates associated with the remote
      /// RTCIceTransport. If state is "closed", throw an InvalidStateError
      /// exception.
      /// </summary>
      void                       SetRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates);
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      /// <summary>
      /// </summary>
      void                       KeepWarm(RTCIceCandidatePair^ candidatePair);
      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetKeepWarm")]
      void                       KeepWarm(RTCIceCandidatePair^ candidatePair, Platform::Boolean keepWarm);

      /// <summary>
      /// Gets the iceGatherer attribute is set to the value of gatherer if
      /// passed in the constructor or in the latest call to Start().
      /// </summary>
      property RTCIceGatherer^ IceGatherer  { RTCIceGatherer^ get(); }
      /// <summary>
      /// Gets the component-id of the RTCIceTransport object. In
      /// RTCIceTransport objects returned by CreateAssociatedTransport(), the
      /// value of component is "RTCP". In all other RTCIceTransport objects,
      /// the value of component is "RTP".
      /// </summary>
      property RTCIceComponent Component    { RTCIceComponent get(); }
      /// <summary>
      /// Gets the current role of the ICE transport.
      /// </summary>
      property RTCIceRole Role              { RTCIceRole get(); }
      /// <summary>
      /// Gets the current state of the ICE transport.
      /// </summary>
      property RTCIceTransportState State   { RTCIceTransportState get(); }

      /// <summary>
      /// The RTCIceTransportState changed.
      /// </summary>
      event RTCIceTransportStateChangedDelegate^            OnStateChange;
      /// <summary>
      /// A new RTCIceCandidatePair is available.
      /// </summary>
      event RTCIceTransportCandidatePairAvailableDelegate^  OnCandidatePairAvailable;
      /// <summary>
      /// A previous RTCIceCandidatePair is gone.
      /// </summary>
      event RTCIceTransportCandidatePairGoneDelegate^       OnCandidatePairGone;
      /// <summary>
      /// The selected RTCIceCandidatePair changed.
      /// </summary>
      event RTCIceTransportCandidatePairChangedDelegate^    OnCandidatePairChange;

    private:
      IICETransportPtr _nativePointer;
      internal::RTCIceTransportDelegatePtr _nativeDelegatePointer;
      IICETransportSubscriptionPtr _nativeDelegateSubscription;
    };
  }
}
