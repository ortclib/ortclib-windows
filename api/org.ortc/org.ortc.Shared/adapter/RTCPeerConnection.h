#pragma once

#include <ortc/adapter/IPeerConnection.h>
#include "../RTCIceTypes.h"
#include "MediaStream.h"
#include "RTCSessionDescription.h"

namespace org
{
  namespace ortc
  {
    ref class RTCCertificate;
    ref class RTCRtpSender;
    ref class RTCRtpReceiver;
    ref class MediaStreamTrack;
    ref class RTCDataChannel;
    ref struct RTCRtpParameters;
    ref struct RTCIceGatherOptions;
    ref struct RTCRtpCapabilities;
    ref struct RTCIceServer;
    ref struct RTCDataChannelParameters;

    namespace adapter
    {
      using Windows::Foundation::Collections::IVector;
      using Windows::Foundation::IAsyncOperation;
      using Windows::Foundation::IAsyncAction;

      enum class RTCBundlePolicy;
      enum class RTCRtcpMuxPolicy;
      enum class RTCSignalingState;
      enum class RTCPeerConnectionState;

      ref struct RTCConfiguration;
      ref struct RTCOfferOptions;
      ref struct RTCAnswerOptions;
      ref struct RTCCapabilityOptions;
      ref struct RTCMediaStreamTrackConfiguration;

      ref struct RTCPeerConnectionIceErrorEvent;
      ref struct RTCTrackEvent;

      ref class RTCPeerConnection;

      enum class RTCSessionDescriptionSignalingType;
      ref class MediaStream;

      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IPeerConnection, IPeerConnection);
      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IPeerConnectionSubscription, IPeerConnectionSubscription);

      namespace internal
      {
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionDelegate);
        ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IPeerConnectionTypes, IPeerConnectionTypes);

        RTCConfiguration^ ToCx(const IPeerConnectionTypes::Configuration &input);
        RTCConfiguration^ ToCx(IPeerConnectionTypes::ConfigurationPtr input);
        IPeerConnectionTypes::ConfigurationPtr FromCx(RTCConfiguration^ input);

        RTCOfferOptions^ ToCx(const IPeerConnectionTypes::OfferOptions &input);
        RTCOfferOptions^ ToCx(IPeerConnectionTypes::OfferOptionsPtr input);
        IPeerConnectionTypes::OfferOptionsPtr FromCx(RTCOfferOptions^ input);

        RTCAnswerOptions^ ToCx(const IPeerConnectionTypes::AnswerOptions &input);
        RTCAnswerOptions^ ToCx(IPeerConnectionTypes::AnswerOptionsPtr input);
        IPeerConnectionTypes::AnswerOptionsPtr FromCx(RTCAnswerOptions^ input);

        RTCCapabilityOptions^ ToCx(const IPeerConnectionTypes::CapabilityOptions &input);
        RTCCapabilityOptions^ ToCx(IPeerConnectionTypes::CapabilityOptionsPtr input);
        IPeerConnectionTypes::CapabilityOptionsPtr FromCx(RTCCapabilityOptions^ input);

        RTCMediaStreamTrackConfiguration^ ToCx(const IPeerConnectionTypes::MediaStreamTrackConfiguration &input);
        RTCMediaStreamTrackConfiguration^ ToCx(IPeerConnectionTypes::MediaStreamTrackConfigurationPtr input);
        IPeerConnectionTypes::MediaStreamTrackConfigurationPtr FromCx(RTCMediaStreamTrackConfiguration^ input);

      } // namespace internal


      /// <summary>
      /// As described in [draft-ietf-rtcweb-jsep] (section 4.1.1.), BUNDLE
      /// policy affects which media tracks are negotiated if the remote
      /// endpoint is not BUNDLE-aware, and what ICE candidates are gathered.
      /// </summary>
      public enum class RTCBundlePolicy
      {
        /// <summary>
        /// Gather ICE candidates for each media type in use (audio, video,
        /// and data). If the remote endpoint is not BUNDLE-aware, negotiate
        /// only one audio and video track on separate transports.
        /// </summary>
        Balanced,
        /// <summary>
        /// Gather ICE candidates for each track. If the remote endpoint is
        /// not BUNDLE-aware, negotiate all media tracks on separate
        /// transports.
        /// </summary>
        MaxCompat,
        /// <summary>
        /// Gather ICE candidates for only one track. If the remote endpoint
        /// is not BUNDLE-aware, negotiate only one media track.
        /// </summary>
        MaxBundle
      };

      /// <summary>
      /// The RtcpMuxPolicy affects what ICE candidates are gathered to
      /// support non-multiplexed RTCP.
      /// </summary>
      public enum class RTCRtcpMuxPolicy
      {
        /// <summary>
        /// Gather ICE candidates for both RTP and RTCP candidates. If the
        /// remote-endpoint is capable of multiplexing RTCP, multiplex RTCP on
        /// the RTP candidates. If it is not, use both the RTP and RTCP
        /// candidates separately.
        /// </summary>
        Negotiated,
        /// <summary>
        /// Gather ICE candidates only for RTP and multiplex RTCP on the RTP
        /// candidates. If the remote endpoint is not capable of rtcp-mux,
        /// session negotiation will fail.
        /// </summary>
        Require
      };

      /// <summary>
      /// The RTCSignalingState describes the current state of an offer / 
      /// answer exchanged as it relates to the SDP negotiation process. WHen
      /// JSON style signaling is used the state is always Stable.
      /// </summary>
      public enum class RTCSignalingState
      {
        /// <summary>
        /// There is no offer­answer exchange in progress. This is also the
        /// initial state in which case the local and remote descriptions are
        /// empty.
        /// </summary>
        Stable,
        /// <summary>
        /// A local description, of type "offer", has been successfully
        /// applied.
        /// </summary>
        HaveLocalOffer,
        /// <summary>
        /// A remote description, of type "offer", has been successfully
        /// applied.
        /// </summary>
        HaveRemoteOffer,
        /// <summary>
        /// A remote description of type "offer" has been successfully
        /// applied and a local description of type "pranswer" has been
        /// successfully applied.
        /// </summary>
        HaveLocalPranswer,
        /// <summary>
        /// A local description of type "offer" has been successfully applied
        /// and a remote description of type "pranswer" has been successfully
        /// applied.
        /// </summary>
        HaveRemotePranswer,
        /// <summary>
        /// The connection is closed.
        /// </summary>
        Closed
      };

      /// <summary>
      /// The RTCIceGatheringState indicates the combined state of all the
      /// RTCIceGatherer objects states.
      /// </summary>
      public enum class RTCIceGatheringState
      {
        /// <summary>
        /// The object was just created, and no networking has occurred yet.
        /// </summary>
        New,
        /// <summary>
        /// The ICE agent is in the process of gathering candidates for this
        /// RTCPeerConnection.
        /// </summary>
        Gathering,
        /// <summary>
        /// The ICE agent has completed gathering. Events such as adding a new
        /// interface or a new TURN server will cause the state to go back to
        /// gathering.
        /// </summary>
        Complete
      };

      /// <summary>
      /// The RTCIceConnectionState indicates the combined state of all the
      /// RTCIceTransport objects states.
      /// </summary>
      public enum class RTCIceConnectionState
      {
        /// <summary>
        /// The ICE Agent is gathering addresses and/or waiting for remote
        /// candidates to be supplied.
        /// </summary>
        New,
        /// <summary>
        /// The ICE Agent has received remote candidates on at least one
        /// component, and is checking candidate pairs but has not yet found a
        /// connection. In addition to checking, it may also still be
        /// gathering.
        /// </summary>
        Checking,
        /// <summary>
        /// The ICE Agent has found a usable connection for all components but
        /// is still checking other candidate pairs to see if there is a
        /// better connection. It may also still be gathering.
        /// </summary>
        Connected,
        /// <summary>
        /// The ICE Agent has finished gathering and checking and found a
        /// connection for all components. Details on how the completed state
        /// in ICE is reached are covered in [RFC5245].
        /// </summary>
        Completed,
        /// <summary>
        /// The ICE Agent is finished checking all candidate pairs and failed
        /// to find a connection for at least one component. Connections may
        /// have been found for some components.
        /// </summary>
        Failed,
        /// <summary>
        /// Liveness checks have failed for one or more components. This is
        /// more aggressive than failed, and may trigger intermittently (and
        /// resolve itself without action) on a flaky network.
        /// </summary>
        Disconnected,
        /// <summary>
        /// The ICE Agent has shut down and is no longer responding to STUN
        /// requests.
        /// </summary>
        Closed
      };

      /// <summary>
      /// The RTCPeerConnectionState indicates the combined state of all the
      /// RTCIceTransport and RTCDtlsTransport objects states.
      /// </summary>
      public enum class RTCPeerConnectionState
      {
        /// <summary>
        /// Any of the RTCIceTransports or RTCDtlsTransports are in the new
        /// state and none of the transports are in the connecting, checking,
        /// failed or disconnected state, or all transports are in the closed
        /// state.
        /// </summary>
        New,
        /// <summary>
        /// Any of the RTCIceTransports or RTCDtlsTransports are in the
        /// connecting or checking state and none of them is in the failed
        /// state.
        /// </summary>
        Connecting,
        /// <summary>
        /// All RTCIceTransports and RTCDtlsTransports are in the connected,
        /// completed or closed state and at least of them is in the connected
        /// or completed state.
        /// </summary>
        Connected,
        /// <summary>
        /// Any of the RTCIceTransports or RTCDtlsTransports are in the
        /// disconnected state and none of them are in the failed or
        /// connecting or checking state.
        /// </summary>
        Disconnected,
        /// <summary>
        /// Any of the RTCIceTransports or RTCDtlsTransports are in a failed
        /// state.
        /// </summary>
        Failed
      };

      /// <summary>
      /// The RTCPeerConnectionSignalingMode defines the signaling type to be
      /// used with signaling modes.
      /// </summary>
      public enum class RTCPeerConnectionSignalingMode
      {
        /// <summary>
        /// Use JSON capabilities exchange style signaling with the peer
        /// connection.
        /// </summary>
        Json,
        /// <summary>
        ///  Use SDP offer/answer exchange style signaling with the peer
        /// connection.
        /// </summary>
        Sdp
      };

      /// <summary>
      /// The RTCConfiguration defines a set of parameters to configure how
      /// the peer to peer communication established via RTCPeerConnection is
      /// established or re-established.
      /// </summary>
      public ref struct RTCConfiguration sealed
      {
        /// <summary>
        /// Gets or sets the ice gatherer policy as well as an array of
        /// objects describing servers available to be used by ICE, such as
        /// STUN and TURN server.
        /// </summary>
        property RTCIceGatherOptions^                 GatherOptions;
        /// <summary>
        /// Gets or sets the signaling mode as either JSON based or SDP based.
        /// The signaling type must be specified as Json, SdpOffer, or
        /// SdpAnswer.
        /// </summary>
        property RTCPeerConnectionSignalingMode       SignalingMode;
        /// <summary>
        /// Gets or sets if the SRTP/SDES should be used with the peer
        /// connection.
        /// </summary>
        property Platform::Boolean                    NegotiateSrtpSdes;
        /// <summary>
        /// Gets or sets a media-bundling policy to use when gathering ICE.
        /// candidates.
        /// </summary>
        property RTCBundlePolicy                      BundlePolicy;
        /// <summary>
        /// Gets or sets rtcp-mux policy to use when gathering ICE candidates.
        /// </summary>
        property RTCRtcpMuxPolicy                     RtcpMuxPolicy;
        /// <summary>
        /// A set of certificates that the RTCPeerConnection uses to
        /// authenticate.
        /// </summary>
        property IVector<RTCCertificate^>^            Certificates;

        /// <summary>
        /// Consturcts an instance of an RTCConfiguration object.
        /// authenticate.
        /// </summary>
        RTCConfiguration()
        {
          SignalingMode = RTCPeerConnectionSignalingMode::Json;
          BundlePolicy = RTCBundlePolicy::Balanced;
          RtcpMuxPolicy = RTCRtcpMuxPolicy::Require;
        }
      };

      /// <summary>
      /// The RTCOfferOptions object contains configuration options related
      /// to an offer.
      /// </summary>
      public ref struct RTCOfferOptions sealed
      {
        /// <summary>
        /// Gets or sets if VAD is enabled. Many codecs and systems are
        /// capable of detecting "silence" and changing their behavior in this
        /// case by doing things such as not transmitting any media. In many
        /// cases, such as when dealing with emergency calling or sounds other
        /// than spoken voice, it is desirable to be able to turn off this
        /// behavior. This option allows the application to provide
        /// information about whether it wishes this type of processing
        /// enabled or disabled.
        /// </summary>
        property Platform::Boolean VoiceActivityDetection;

        /// <summary>
        /// Gets or sets if the ice gatherer should be restarted. When the
        /// value of this dictionary member is true, the generated description
        /// will have ICE credentials that are different from the current
        /// credentials(as visible in the localDescription attribute's SDP).
        /// Applying the generated description will restart ICE.
        /// </summary>
        property Platform::Boolean IceRestart;

        /// <summary>
        /// Constructs an instance of an RTCOfferOptions object.
        /// </summary>
        RTCOfferOptions()
        {
          VoiceActivityDetection = false;
          IceRestart = false;
        }
      };

      /// <summary>
      /// The RTCAnswerOptions object contains configuration options related
      /// to an answer.
      /// </summary>
      public ref struct RTCAnswerOptions sealed
      {
        /// <summary>
        /// Gets or sets if VAD is enabled. Many codecs and systems are
        /// capable of detecting "silence" and changing their behavior in this
        /// case by doing things such as not transmitting any media. In many
        /// cases, such as when dealing with emergency calling or sounds other
        /// than spoken voice, it is desirable to be able to turn off this
        /// behavior. This option allows the application to provide
        /// information about whether it wishes this type of processing
        /// enabled or disabled.
        /// </summary>
        property Platform::Boolean VoiceActivityDetection;

        /// <summary>
        /// Constructs an instance of an RTCOfferOptions object.
        /// </summary>
        RTCAnswerOptions()
        {
          VoiceActivityDetection = false;
        }
      };

      /// <summary>
      /// The RTCCapabilityOptions object contains configuration options related
      /// to capability options as part of a capabilities signaling exchange.
      /// </summary>
      public ref struct RTCCapabilityOptions sealed
      {
        /// <summary>
        /// Gets or sets if VAD is enabled. Many codecs and systems are
        /// capable of detecting "silence" and changing their behavior in this
        /// case by doing things such as not transmitting any media. In many
        /// cases, such as when dealing with emergency calling or sounds other
        /// than spoken voice, it is desirable to be able to turn off this
        /// behavior. This option allows the application to provide
        /// information about whether it wishes this type of processing
        /// enabled or disabled.
        /// </summary>
        property Platform::Boolean VoiceActivityDetection;

        /// <summary>
        /// Constructs an instance of an RTCOfferOptions object.
        /// </summary>
        RTCCapabilityOptions()
        {
          VoiceActivityDetection = false;
        }
      };

      /// <summary>
      /// The RTCMediaStreamTrackConfiguration contains the information needed
      /// to configure the tracks to send over the RTP transport.
      /// </summary>
      public ref struct RTCMediaStreamTrackConfiguration sealed
      {
        /// <summary>
        /// Gets or sets the RTP parameters associated with the media
        /// stream track configuration.
        /// </summary>
        property RTCRtpParameters^            Parameters;
      };

      /// <summary>
      /// This event fires whenever a new ICE candidate has been discovered.
      /// </summary>
      public ref struct RTCPeerConnectionIceEvent sealed
      {
        friend class internal::RTCPeerConnectionDelegate;

        /// <summary>
        /// Gets the RTCIceCandidate object with the new ICE candidate that
        /// caused the event.
        /// </summary>
        property RTCIceCandidate^         Candidate
        {
          RTCIceCandidate^ get() { return _candidate; }
        }

        /// <summary>
        /// Gets the STUN or TURN URL that identifies the STUN or TURN server
        /// used to gather this candidate. If the candidate was not gathered
        /// from a STUN or TURN server, this parameter will be set to null.
        /// </summary>
        property Platform::String^        Url
        {
          Platform::String^ get() { return _url; }
        }

      private:
        RTCIceCandidate^          _candidate;
        Platform::String^         _url;
      };

      /// <summary>
      /// This event fires whenever the final ICE candidate has been
      /// discovered for an ICE transport.
      /// </summary>
      public ref struct RTCPeerConnectionIceCompleteEvent sealed
      {
        friend class internal::RTCPeerConnectionDelegate;

        /// <summary>
        /// Gets the RTCIceCandidateComplete object with the final ICE
        /// transport candidate that caused the event.
        /// </summary>
        property RTCIceCandidateComplete^         Candidate
        {
          RTCIceCandidateComplete^ get() { return _candidate; }
        }

      private:
        RTCIceCandidateComplete^          _candidate;
      };

      /// <summary>
      /// The RTCPeerConnectionIceErrorEvent object is fired when an error
      /// connecting to a STUN or TURN server is detected.
      /// </summary>
      public ref struct RTCPeerConnectionIceErrorEvent sealed
      {
        friend class internal::RTCPeerConnectionDelegate;

        /// <summary>
        /// Gets the local candidate used to communicate with the STUN or TURN
        /// server.
        /// </summary>
        property RTCIceCandidate^         HostCandidate
        {
          RTCIceCandidate^ get()        { return _hostCandidate; }
        }

        /// <summary>
        /// Gets the STUN or TURN URL that identifies the STUN or TURN server
        /// for which the failure occurred.
        /// </summary>
        property Platform::String^        Url
        {
          Platform::String^ get()       { return _url; }
        }

        /// <summary>
        /// Gets the errorCode attribute is the numeric STUN error code
        /// returned by the STUN or TURN server.
        /// </summary>
        property Platform::IBox<uint16>^  ErrorCode
        {
          Platform::IBox<uint16>^ get() { return _errorCode; }
        }

        /// <summary>
        /// Gets the errorText attribute is the STUN reason text returned by
        /// the STUN or TURN server.
        /// </summary>
        property Platform::String^        ErrorText
        {
          Platform::String^ get()       { return _errorText; }
        }

      private:
        RTCIceCandidate^          _hostCandidate;
        Platform::String^         _url;
        Platform::IBox<uint16>^   _errorCode;
        Platform::String^         _errorText;
      };


      /// <summary>
      /// The RTCTrackEvent is fired when a new remote track is detected.
      /// </summary>
      public ref struct RTCTrackEvent sealed
      {
        friend class internal::RTCPeerConnectionDelegate;

        /// <summary>
        /// Gets the RTCRtpReceiver object associated with the event.
        /// </summary>
        property RTCRtpReceiver^                Receiver
        {
          RTCRtpReceiver^ get()               { return _receiver; }
        }

        /// <summary>
        /// Gets the MediaStreamTrack object that is associated with the
        /// RTCRtpReceiver identified by receiver.
        /// </summary>
        property MediaStreamTrack^              Track
        {
          MediaStreamTrack^ get()             { return _track; }
        }

        /// <summary>
        /// Gets an array of MediaStream objects representing the MediaStreams
        /// that this event's track is a part of.
        /// </summary>
        property IVector<MediaStream^>^    Streams
        {
          IVector<MediaStream^>^ get()     { return _streams; }
        }

      private:
        RTCRtpReceiver^               _receiver;
        MediaStreamTrack^             _track;
        IVector<MediaStream^>^        _streams;
      };

      public delegate void RTCPeerConnectionNegotiationNeededDelegate();
      public delegate void RTCPeerConnectionIceCandidateDelegate(RTCPeerConnectionIceEvent^ evt);
      public delegate void RTCPeerConnectionIceCandidateCompleteDelegate(RTCPeerConnectionIceCompleteEvent^ evt);
      public delegate void RTCPeerConnectionIceCandidateErrorDelegate(RTCPeerConnectionIceErrorEvent^ evt);
      public delegate void RTCPeerConnectionSignalingStateChangeDelegate();
      public delegate void RTCPeerConnectionIceConnectionStateChaneDelegate();
      public delegate void RTCPeerConnectionIceGatheringStateChaneDelegate();
      public delegate void RTCPeerConnectionConnectionStateChaneDelegate();
      public delegate void RTCPeerConnectionTrackDelegate(RTCTrackEvent^ evt);
      public delegate void RTCPeerConnectionDataChannelDelegate(RTCDataChannel^ evt);

      /// <summary>
      /// The general operation of the RTCPeerConnection is described in
      /// [draft-ietf-rtcweb-jsep].
      /// </summary>
      public ref class RTCPeerConnection sealed
      {
      private:
        friend class internal::RTCPeerConnectionDelegate;

      private:
        RTCPeerConnection(IPeerConnectionPtr session);

        static RTCPeerConnection^ Convert(IPeerConnectionPtr session) { if (!session) return nullptr; return ref new RTCPeerConnection(session); }
        static IPeerConnectionPtr Convert(RTCPeerConnection^ session) { if (!session) return nullptr; return session->_nativePointer; }

      public:
        /// <summary>
        /// Constructs an instance of an RTCPeerConnection object. 
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        RTCPeerConnection();
        /// <summary>
        /// Constructs an instance of an RTCPeerConnection object with 
        /// configuration options specified.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("CreateWithConfiguration")]
        RTCPeerConnection(RTCConfiguration^ configuration);

        /// <summary>
        /// The CreateOffer method generates a blob of SDP that contains an
        /// [RFC3264] offer with the supported configurations for the session,
        /// including descriptions of the local MediaStreamTracks attached to
        /// this RTCPeerConnection, the codec/RTP/RTCP options supported by
        /// this implementation, and any candidates that have been gathered by
        /// the ICE Agent. The options parameter may be supplied to provide
        /// additional control over the offer generated.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        IAsyncOperation<RTCSessionDescription^>^            CreateOffer();

        /// <summary>
        /// The CreateOffer method generates a blob of SDP that contains an
        /// [RFC3264] offer with the supported configurations for the session,
        /// including descriptions of the local MediaStreamTracks attached to
        /// this RTCPeerConnection, the codec/RTP/RTCP options supported by
        /// this implementation, and any candidates that have been gathered by
        /// the ICE Agent. The options parameter may be supplied to provide
        /// additional control over the offer generated.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("CreateOfferWithOptions")]
        IAsyncOperation<RTCSessionDescription^>^            CreateOffer(RTCOfferOptions^ options);

        /// <summary>
        /// The CreateAnswer method generates an [RFC3264] answer with the
        /// supported configuration for the session that is compatible with
        /// the parameters in the remote configuration. Like createOffer, the
        /// returned blob contains descriptions of the local MediaStreamTracks
        /// attached to this RTCPeerConnection, the codec/RTP/RTCP options
        /// negotiated for this session, and any candidates that have been
        /// gathered by the ICE Agent. The options parameter may be supplied
        /// to provide additional control over the generated answer.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        IAsyncOperation<RTCSessionDescription^>^            CreateAnswer();

        /// <summary>
        /// The CreateAnswer method generates an [RFC3264] answer with the
        /// supported configuration for the session that is compatible with
        /// the parameters in the remote configuration. Like createOffer, the
        /// returned blob contains descriptions of the local MediaStreamTracks
        /// attached to this RTCPeerConnection, the codec/RTP/RTCP options
        /// negotiated for this session, and any candidates that have been
        /// gathered by the ICE Agent. The options parameter may be supplied
        /// to provide additional control over the generated answer.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("CreateAnswerWithOptions")]
        IAsyncOperation<RTCSessionDescription^>^            CreateAnswer(RTCAnswerOptions^ options);

        /// <summary>
        /// When operating in JSON signaling type, this method gets the local
        /// description which consists of ICE transport parameters, secure
        /// transport parameters, and RTP/RTCP capabilities without needing
        /// shared negioated state with an offer answer exchange. In this
        /// signaling model, each side creates it's capabilities (and
        /// parameters) and exchanges with the remote party.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        IAsyncOperation<RTCSessionDescription^>^            CreateCapabilities();

        /// <summary>
        /// When operating in JSON signaling type, this method gets the local
        /// description which consists of ICE transport parameters, secure
        /// transport parameters, and RTP/RTCP capabilities without needing
        /// shared negioated state with an offer answer exchange. In this
        /// signaling model, each side creates it's capabilities (and
        /// parameters) and exchanges with the remote party.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("CreateCapabilitiesWithOptions")]
        IAsyncOperation<RTCSessionDescription^>^            CreateCapabilities(RTCCapabilityOptions^ options);
        

        /// <summary>
        /// The SetLocalDescription() method instructs the RTCPeerConnection
        /// to apply the supplied RTCSessionDescriptionInit as the local
        /// description.
        /// </summary>
        IAsyncAction^                                       SetLocalDescription(RTCSessionDescription^ description);

        /// <summary>
        /// Gets the PendingLocalDescription if it is not null and otherwise
        /// it must return CurrentLocalDescription.
        /// </summary>
        property RTCSessionDescription^                     LocalDescription
        {
          RTCSessionDescription^ get();
        }
        /// <summary>
        /// Gets the local RTCSessionDescription that was successfully
        /// negotiated the last time theRTCPeerConnection transitioned into
        /// the stable state plus any local candidates that have been
        /// generated by the ICE Agent since the offer or answer was created.
        /// This attribute is updated by SetLocalDescription().
        /// </summary>
        property RTCSessionDescription^                     CurrentDescription
        {
          RTCSessionDescription^ get();
        }
        /// <summary>
        /// Gets a local RTCSessionDescription that is in the process of being
        /// negotiated plus any local candidates that have been generated by
        /// the ICE Agent since the offer or answer was created. If the
        /// RTCPeerConnection is in the stable state, the value is null. This
        /// attribute is updated by setLocalDescription().
        /// </summary>
        property RTCSessionDescription^                     PendingDescription
        {
          RTCSessionDescription^ get();
        }

        /// <summary>
        /// The SetRemoteDescription() method instructs the RTCPeerConnection
        /// to apply the supplied RTCSessionDescriptionInit as the remote
        /// offer or answer (or remote description if in JSON signalling).
        /// This API may change the local media state.
        /// </summary>
        IAsyncAction^                                       SetRemoteDescription(RTCSessionDescription^ description);

        /// <summary>
        /// Gets the PendingRemoteDescription if it is not null and otherwise
        /// it must return currentRemoteDescription.
        /// </summary>
        property RTCSessionDescription^                     RemoteDescription
        {
          RTCSessionDescription^ get();
        }
        /// <summary>
        /// Gets the last remote RTCSessionDescription that was successfully
        /// negotiated the last time theRTCPeerConnection transitioned into
        /// the stable state plus any remote candidates that have been
        /// supplied via AddIceCandidate() since the offer or answer was
        /// created. This attribute is updated by SetRemoteDescription().
        /// </summary>
        property RTCSessionDescription^                     CurrentRemoteDescription
        {
          RTCSessionDescription^ get();
        }
        /// <summary>
        /// Gets a remote RTCSessionDescription that is in the process of
        /// being negotiated, completed with any remote candidates that have
        /// been supplied via AddIceCandidate() since the offer or answer was
        /// created. If the RTCPeerConnection is in the stable state, the
        /// value is null. This attribute is updated by
        /// SetRemoteDescription().
        /// </summary>
        property RTCSessionDescription^                     PendingRemoteDescription
        {
          RTCSessionDescription^ get();
        }

        /// <summary>
        /// The AddIceCandidate() method provides a remote candidate to the
        /// ICE Agent. In addition to being added to the remote description,
        /// connectivity checks will be sent to the new candidates as long as
        /// the ICE Transports setting is not set to none. This call will
        /// result in a change to the ICE connection state, and may result in
        /// a change to media state if it results in different connectivity
        /// being established. The only members of candidate used by this
        /// method are candidate, sdpMid and sdpMLineIndex; the rest are
        /// ignored.
        /// </summary>
        void                                                AddIceCandidate(RTCIceCandidate^ candidate);
        /// <summary>
        /// The AddIceCandidateComplete() method provides an indication that
        /// no further remote candidates are being gatherered for a
        /// particular transort.
        /// </summary>
        void                                                AddIceCandidateComplete(RTCIceCandidateComplete^ candidate);

        /// <summary>
        /// Gets the RTCPeerConnection object's signaling state.
        /// </summary>
        property RTCSignalingState                          SignalingState
        {
          RTCSignalingState get();
        }

        /// <summary>
        /// Gets the ICE gathering state of the RTCPeerConnection instance.
        /// </summary>
        property RTCIceGatheringState                       IceGatheringState
        {
          RTCIceGatheringState get();
        }

        /// <summary>
        /// Gets the ICE connection state of the RTCPeerConnection instance.
        /// </summary>
        property RTCIceConnectionState                      IceConnectionState
        {
          RTCIceConnectionState get();
        }

        /// <summary>
        /// Gets the aggregate of the states of the DtlsTransports and
        /// IceTransports of the RTCPeerConnection, as describe in the values
        /// of the RTCPeerConnectionState enum.
        /// </summary>
        property RTCPeerConnectionState                     ConnectionState
        {
          RTCPeerConnectionState get();
        }

        /// <summary>
        /// Gets whether the remote peer is able to accept trickled ICE
        /// candidates [draft-ietf-ice-trickle-01].
        /// </summary>
        property Platform::Boolean                          CanTrickleIceCandidates
        {
          Platform::Boolean get();
        }

        /// <summary>
        /// Gets a list of ICE servers that are configured into the engine.
        /// An engine/browser might be configured to use local or private
        /// STUN or TURN servers. This method allows an application to learn
        /// about these servers and optionally use them.
        /// </summary>
        static IVector<RTCIceServer^>^                      GetDefaultIceServers();

        /// <summary>
        /// Returns a RTCConfiguration object representing the current
        /// configuration of this RTCPeerConnection object.
        /// </summary>
        RTCConfiguration^                                   GetConfiguration();
        /// <summary>
        /// The SetConfiguration method updates the ICE Agent process of
        /// gathering local candidates and pinging remote candidates.
        /// </summary>
        void                                                SetConfiguration(RTCConfiguration^ configuration);

        /// <summary>
        /// Closes the RTCPeerConnection object.
        /// </summary>
        void                                                Close();

        /// <summary>
        /// Returns a sequence of RTCRtpSender objects representing the RTP
        /// senders that are currently attached to this RTCPeerConnection
        /// object.
        /// </summary>
        IVector<RTCRtpSender^>^                             GetSenders();
        /// <summary>
        /// Returns a sequence of RTCRtpReceiver objects representing the RTP
        /// receivers that are currently attached to this RTCPeerConnection
        /// object.
        /// </summary>
        IVector<RTCRtpReceiver^>^                           GetReceivers();

        /// <summary>
        /// Adds a new track to the RTCPeerConnection.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        RTCRtpSender^                                       AddTrack(MediaStreamTrack^ track);
        /// <summary>
        /// Adds a new track to the RTCPeerConnection and specifies how the
        /// track must be encoded.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("AddTrackWithConfiguration")]
        RTCRtpSender^                                       AddTrack(
          MediaStreamTrack^ track,
          RTCMediaStreamTrackConfiguration^ configuration
          );
        /// <summary>
        /// Adds a new track to the RTCPeerConnection, and indicates that it
        /// is contained in the specified MediaStreams.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("AddTrackWithStreams")]
        RTCRtpSender^                                       AddTrack(
          MediaStreamTrack^ track,
          IVector<MediaStream^>^ streams
          );
        /// <summary>
        /// Adds a new track to the RTCPeerConnection and specifies how the
        /// track must be encoded.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("AddTrackWithStreamsAndConfiguration")]
        RTCRtpSender^                                       AddTrack(
          MediaStreamTrack^ track,
          IVector<MediaStream^>^ streams,
          RTCMediaStreamTrackConfiguration^ configuration
          );

        /// <summary>
        /// Stops sending media from sender. The RTCRtpSender will still
        /// appear in GetSenders().
        /// </summary>
        void                                                RemoveTrack(RTCRtpSender^ track);

        /// <summary>
        /// Creates a new RTCDataChannel object with the given data channel
        /// parameters.
        /// </summary>
        RTCDataChannel^                                     CreateDataChannel(RTCDataChannelParameters^ parameters);

        /// <summary>
        /// The engine wishes to inform the application that session
        /// negotiation needs to be done (e.g. a CreateOffer() call followed
        /// by SetLocalDescription). Alternatively, CreateCapabilities() can
        /// be called for JSON style signaling.
        /// </summary>
        event RTCPeerConnectionNegotiationNeededDelegate^         OnNegotiationNeeded;

        /// <summary>
        /// A new RTCIceCandidate is made available to the script.
        /// </summary>
        event RTCPeerConnectionIceCandidateDelegate^              OnIceCandidate;

        /// <summary>
        /// The final RTCIceCandidateComplete is discovered for a transport.
        /// </summary>
        event RTCPeerConnectionIceCandidateCompleteDelegate^      OnIceCandidateComplete;

        /// <summary>
        /// A failure occured when gathering ICE candidates.
        /// </summary>
        event RTCPeerConnectionIceCandidateErrorDelegate^         OnIceCandidateError;

        /// <summary>
        /// The signaling state has changed. This state change is the result
        /// of either setLocalDescription() or setRemoteDescription() being
        /// invoked.
        /// </summary>
        event RTCPeerConnectionSignalingStateChangeDelegate^      OnSignalingStateChange;

        /// <summary>
        /// The RTCPeerConnection's ICE connection state has changed.
        /// </summary>
        event RTCPeerConnectionIceConnectionStateChaneDelegate^   OnIceConnectionStateChange;

        /// <summary>
        /// The RTCPeerConnection's ICE gathering state has changed.
        /// </summary>
        event RTCPeerConnectionIceGatheringStateChaneDelegate^    OnIceGatheringStateChange;

        /// <summary>
        /// The RTCPeerConnection ConnectionState has changed.
        /// </summary>
        event RTCPeerConnectionConnectionStateChaneDelegate^      OnConnectionStateChange;
        
        /// <summary>
        /// A new incoming MediaStreamTrack has been created, and an
        /// associated RTCRtpReceiver has been added to the set of receivers.
        /// </summary>
        event RTCPeerConnectionTrackDelegate^                     OnTrack;

        /// <summary>
        /// An incoming MediaStreamTrack has disappeared, and the associated
        /// RTCRtpReceiver is now removed.
        /// </summary>
        event RTCPeerConnectionTrackDelegate^                     OnTrackGone;

        /// <summary>
        /// A new RTCDataChannel is dispatched to the script in response to
        /// the other peer creating a channel.
        /// </summary>
        event RTCPeerConnectionDataChannelDelegate^               OnDataChannel;

      private:
        IPeerConnectionPtr _nativePointer;
        internal::RTCPeerConnectionDelegatePtr _nativeDelegatePointer;
        IPeerConnectionSubscriptionPtr _nativeDelegateSubscription;
      };

    }

  } // namespace ortc
} // namespace org
