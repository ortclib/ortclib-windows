#pragma once

#include <ortc/adapter/ISessionDescription.h>

#include "../RTCIceTypes.h"

namespace org
{
  namespace ortc
  {
    ref struct RTCIceParameters;
    ref struct RTCDtlsParameters;
    ref struct RTCSrtpSdesParameters;
    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;
    ref struct RTCSctpCapabilities;
    ref struct RTCDtlsParameters;
    ref struct RTCSrtpSdesParameters;
    ref struct RTCIceCandidate;
    ref struct RTCIceCandidateComplete;

    namespace adapter
    {
      using Windows::Foundation::Collections::IVector;

      enum class RTCSessionDescriptionMediaType;
      enum class RTCSessionDescriptionSignalingType;

      ref struct RTCSessionDescriptionDescription;
      ref struct RTCSessionDescriptionDescriptionDetails;
      ref struct RTCSessionDescriptionConnectionData;
      ref struct RTCSessionDescriptionConnectionDataDetails;
      ref struct RTCSessionDescriptionTransport;
      ref struct RTCSessionDescriptionTransportParameters;
      ref struct RTCSessionDescriptionMediaLineDetails;
      ref struct RTCSessionDescriptionRtpMediaLine;
      ref struct RTCSessionDescriptionSctpMediaLine;
      ref struct RTCSessionDescriptionRtpSender;
      ref struct RTCSessionDescriptionRtpSenderDetails;
      ref struct RTCIceCandidate;
      ref struct RTCIceCandidateComplete;

      ref class RTCPeerConnection;

      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::ISessionDescription, ISessionDescription)
      
      namespace internal
      {
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionPromiseWithDescriptionObserver);

        ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::ISessionDescriptionTypes, ISessionDescriptionTypes);
        ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::ISessionDescription, ISessionDescription);

        RTCSessionDescriptionDescription^ ToCx(const ISessionDescriptionTypes::Description &input);
        RTCSessionDescriptionDescription^ ToCx(ISessionDescriptionTypes::DescriptionPtr input);
        ISessionDescriptionTypes::DescriptionPtr FromCx(RTCSessionDescriptionDescription^ input);

        RTCSessionDescriptionDescriptionDetails^ ToCx(const ISessionDescriptionTypes::Description::Details &input);
        RTCSessionDescriptionDescriptionDetails^ ToCx(ISessionDescriptionTypes::Description::DetailsPtr input);
        ISessionDescriptionTypes::Description::DetailsPtr FromCx(RTCSessionDescriptionDescriptionDetails^ input);

        RTCSessionDescriptionConnectionData^ ToCx(const ISessionDescriptionTypes::ConnectionData &input);
        RTCSessionDescriptionConnectionData^ ToCx(ISessionDescriptionTypes::ConnectionDataPtr input);
        ISessionDescriptionTypes::ConnectionDataPtr FromCx(RTCSessionDescriptionConnectionData^ input);

        RTCSessionDescriptionConnectionDataDetails^ ToCx(const ISessionDescriptionTypes::ConnectionData::Details &input);
        RTCSessionDescriptionConnectionDataDetails^ ToCx(ISessionDescriptionTypes::ConnectionData::DetailsPtr input);
        ISessionDescriptionTypes::ConnectionData::DetailsPtr FromCx(RTCSessionDescriptionConnectionDataDetails^ input);

        RTCSessionDescriptionTransport^ ToCx(const ISessionDescriptionTypes::Transport &input);
        RTCSessionDescriptionTransport^ ToCx(ISessionDescriptionTypes::TransportPtr input);
        ISessionDescriptionTypes::TransportPtr FromCx(RTCSessionDescriptionTransport^ input);

        RTCSessionDescriptionTransportParameters^ ToCx(const ISessionDescriptionTypes::Transport::Parameters &input);
        RTCSessionDescriptionTransportParameters^ ToCx(ISessionDescriptionTypes::Transport::ParametersPtr input);
        ISessionDescriptionTypes::Transport::ParametersPtr FromCx(RTCSessionDescriptionTransportParameters^ input);

        RTCSessionDescriptionMediaLineDetails^ ToCx(const ISessionDescriptionTypes::MediaLine::Details &input);
        RTCSessionDescriptionMediaLineDetails^ ToCx(ISessionDescriptionTypes::MediaLine::DetailsPtr input);
        ISessionDescriptionTypes::MediaLine::DetailsPtr FromCx(RTCSessionDescriptionMediaLineDetails^ input);

        RTCSessionDescriptionRtpMediaLine^ ToCx(const ISessionDescriptionTypes::RTPMediaLine &input);
        RTCSessionDescriptionRtpMediaLine^ ToCx(ISessionDescriptionTypes::RTPMediaLinePtr input);
        ISessionDescriptionTypes::RTPMediaLinePtr FromCx(RTCSessionDescriptionRtpMediaLine^ input);

        RTCSessionDescriptionSctpMediaLine^ ToCx(const ISessionDescriptionTypes::SCTPMediaLine &input);
        RTCSessionDescriptionSctpMediaLine^ ToCx(ISessionDescriptionTypes::SCTPMediaLinePtr input);
        ISessionDescriptionTypes::SCTPMediaLinePtr FromCx(RTCSessionDescriptionSctpMediaLine^ input);

        RTCSessionDescriptionRtpSender^ ToCx(const ISessionDescriptionTypes::RTPSender &input);
        RTCSessionDescriptionRtpSender^ ToCx(ISessionDescriptionTypes::RTPSenderPtr input);
        ISessionDescriptionTypes::RTPSenderPtr FromCx(RTCSessionDescriptionRtpSender^ input);

        RTCSessionDescriptionRtpSenderDetails^ ToCx(const ISessionDescriptionTypes::RTPSender::Details &input);
        RTCSessionDescriptionRtpSenderDetails^ ToCx(ISessionDescriptionTypes::RTPSender::DetailsPtr input);
        ISessionDescriptionTypes::RTPSender::DetailsPtr FromCx(RTCSessionDescriptionRtpSenderDetails^ input);

        adapter::RTCIceCandidate^ ToIceCandidateCx(const ISessionDescriptionTypes::ICECandidate &input);
        adapter::RTCIceCandidate^ ToIceCandidateCx(ISessionDescriptionTypes::ICECandidatePtr input);
        ISessionDescriptionTypes::ICECandidatePtr FromCx(adapter::RTCIceCandidate^ input);

        adapter::RTCIceCandidateComplete^ ToIceCandidateCompleteCx(const ISessionDescriptionTypes::ICECandidate &input);
        adapter::RTCIceCandidateComplete^ ToIceCandidateCompleteCx(ISessionDescriptionTypes::ICECandidatePtr input);
        ISessionDescriptionTypes::ICECandidatePtr FromCx(adapter::RTCIceCandidateComplete^ input);
      }

      /// <summary>
      /// The RTCSdpType enum describes the type of RTCSessionDescription
      /// instance.
      /// </summary>
      public enum class RTCSdpType
      {
        /// <summary>
        /// An RTCSdpType of offer indicates that a description must be
        /// treated as an [RTC3264] offer.
        /// </summary>
        SdpOffer,
        /// <summary>
        /// An RTCSdpType of pranswer indicates that a description must be
        /// treated as an [RTC3264] answer, but not a final answer. A
        /// description used as an SDP pranswer may be applied as a response
        /// to an SDP offer, or an update to a previously sent SDP pranswer.
        /// </summary>
        SdpPreanswer,
        /// <summary>
        /// The format of the description is that of an SDP answer.
        /// </summary>
        SdpAnswer,
        /// <summary>
        /// The format of the description is that of an SDP rollback.
        /// </summary>
        SdpRollback,
      };

      /// <summary>
      /// Defines the session description's signalling mechanism.
      /// </summary>
      public enum class RTCSessionDescriptionSignalingType
      {
        /// <summary>
        /// The format of the description is that of a JSON blob.
        /// </summary>
        Json,
        /// <summary>
        /// The format of the description is that of an SDP offer.
        /// </summary>
        SdpOffer,
        /// <summary>
        /// The format of the description is that of an SDP pre-answer.
        /// </summary>
        SdpPreanswer,
        /// <summary>
        /// The format of the description is that of an SDP answer.
        /// </summary>
        SdpAnswer,
        /// <summary>
        /// The format of the description is that of an SDP rollback.
        /// </summary>
        SdpRollback,
      };

      /// <summary>
      /// Defines the session description's media type as described in
      /// [RFC4566] Section 5.14 for the media line.
      /// </summary>
      /// <example>
      /// <code>
      /// m=audio 49170 RTP/AVP 0
      /// m=video 51372 RTP/AVP 99
      /// </code>
      /// </example>
      public enum class RTCSessionDescriptionMediaType
      {
        /// <summary>
        /// This object represents an unknown media type.
        /// </summary>
        Unknown,
        /// <summary>
        /// This object represents an audio media type.
        /// </summary>
        Audio,
        /// <summary>
        /// This object represents a video media type.
        /// </summary>
        Video,
        /// <summary>
        /// This object represents a text media type.
        /// </summary>
        Text,
        /// <summary>
        /// This object represents an application media type.
        /// </summary>
        Application,
      };

      /// <summary>
      /// Defines the session description direction in which media is flowing.
      /// </summary>
      /// <example>
      /// <code>
      /// a=sendrecv
      /// a=sendonly
      /// a=recvonly
      /// a=inactive
      /// </code>
      /// </example>
      public enum class RTCSessionDescriptionMediaDirection
      {
        /// <summary>
        /// As defined in [RFC4566], media should be started in send and
        /// receive mode.
        /// </summary>
        SendReceive,
        /// <summary>
        /// As defined in [RFC4566], media should be started in send-only mode.
        /// </summary>
        SendOnly,
        /// <summary>
        /// As defined in [RFC4566], media should be started in receive-only
        /// mode.
        /// </summary>
        ReceiveOnly,
        /// <summary>
        /// As defined in [RFC4566], media should be started in inactive mode.
        /// </summary>
        Inactive
      };

      /// <summary>
      /// The RTCSessionDescriptionDescription represents a descriptive
      /// format capable of carrying either media SDP [RFC4566] or JSON-style
      /// media descriptions. The media descriptions contain information about
      /// the session, all transports, media lines with media capabilities,
      /// and senders related to the media lines. This format can be
      /// converted to/from SDP and JSON.
      /// </summary>
      /// <example>
      /// <code>
      /// {
      ///  "session": {
      ///    "transports": {
      ///      "transport": {
      ///        "id": "transport1",
      ///        "rtp": {
      ///          "ice": {
      ///            "parameters": {
      ///              "usernameFragment": "ufrag1",
      ///              "password": "password1",
      ///              "useUnfreezePriority": "true",
      ///              "iceLite": "false"
      ///            }
      ///          },
      ///          "dtls": {
      ///            "parameters": {
      ///              "role": "auto",
      ///              "fingerprints": {
      ///                "fingerprint": [
      ///                  "AB:32:...",
      ///                  "FF:EE:..."
      ///                ]
      ///              }
      ///            }
      ///          },
      ///          "sdesSrtp": { "parameters": "..." },
      ///          "iceCandidates": {
      ///            "iceCandidate": [
      ///              {
      ///                "mid": "a1",
      ///                "index": "0",
      ///                "component": "rtp",
      ///                "candidate": {...}
      ///              },
      ///              { 
      ///                "mid": "a1",
      ///                "index": "0",
      ///                "component": "rtp",
      ///                "candidateComplete": { complete: "true" }
      ///              }
      ///            ]
      ///          }
      ///        },
      ///        "rtcp": {...}
      ///      }
      ///    }
      ///  },
      ///  "rtpMediaLines" : {
      ///    "rtpMediaLine": {
      ///    },
      ///    ...
      ///  },
      ///  "sctpMediaLines" : {
      ///    "sctpMediaLine": {
      ///    },
      ///    ...
      ///  },
      ///  "rtpSenders" : {
      ///    "rtpSender": {
      ///    },
      ///    ...
      ///  },
      /// }
      /// </code>
      /// </example>
      public ref struct RTCSessionDescriptionDescription sealed
      {
        /// <summary>
        /// Gets or sets the detailed session information (which is typically
        /// related to SDP-only [RFC4566]. This value can be null if is it
        /// not needed.
        /// </summary>
        property RTCSessionDescriptionDescriptionDetails^       Details;
        /// <summary>
        /// Gets or sets a list of transports associated with the session.
        /// These transports include the ICE and secure transport properties.
        /// </summary>
        property IVector<RTCSessionDescriptionTransport^>^      Transports;
        /// <summary>
        /// Gets or sets a list of media capabilities which are associated
        /// with transports. These media lines describe the intention of the
        /// media flow and the capabilities and configuration expectations of
        /// the media.
        /// </summary>
        property IVector<RTCSessionDescriptionRtpMediaLine^>^   RtpMediaLines;
        /// <summary>
        /// Gets or sets a list of SCTP data transports associated with
        /// the wire transports.
        /// </summary>
        property IVector<RTCSessionDescriptionSctpMediaLine^>^  SctpMediaLines;
        /// <summary>
        /// Gets or sets a list of RTP senders associated with a
        /// configurated media line.
        /// </summary>
        property IVector<RTCSessionDescriptionRtpSender^>^      RtpSenders;
      };

      /// <summary>
      /// The RTCSessionDescriptionDescriptionDetails represents the session
      /// level properties typically contained within an SDP origin or
      /// other session level properties.
      /// </summary>
      /// <example>
      /// <code>
      /// o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
      /// t=3034423619 3042462419
      /// s=SDP Seminar
      /// c=IN IP4 224.2.17.12/127
      /// a=rtcp:53020
      /// </code>
      /// <code>
      /// o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
      /// t=0 0
      /// s=-
      /// c=IN IP4 224.2.17.12/127
      /// a=rtcp:53020 IN IP4 126.16.64.4
      /// </code>
      /// </example>
      public ref struct RTCSessionDescriptionDescriptionDetails sealed
      {
        /// <summary>
        /// Gets or sets the user's login on the originating host, or it is "-"
        /// if the originating host does not support the concept of user IDs.
        /// This is the origin's username as described in [RFC4566] Section
        /// 5.2.
        /// </summary>
        property Platform::String^                            Username;
        /// <summary>
        /// Gets or sets a numeric string such that the tuple of username,
        /// sess - id, nettype, addrtype, and unicast - address forms a
        /// globally unique identifier for the session. This is the origin's
        /// sess-id as described in [RFC4566] Section 5.2.
        /// </summary>
        property uint64                                       SessionId;
        /// <summary>
        /// Gets or sets a version number for this session description. This
        /// is the origin's sess-version as described in [RFC4566] Section
        /// 5.2.
        /// </summary>
        property uint64                                       SessionVersion;
        /// <summary>
        /// Gets or sets the unicast address information for a session. This
        /// represents the origin's nettype, addrtype, unicast-address as
        /// described in [RFC4566] Section 5.2.
        /// </summary>
        property RTCSessionDescriptionConnectionDataDetails^  UnicastAddress;
        /// <summary>
        /// Gets or sets the textual session name as described in [RFC4566]
        /// Section 5.3.
        /// </summary>
        property Platform::String^                            SessionName;
        /// <summary>
        /// Gets or sets the start time of the session in NTP format as
        /// described in [RFC4566] Section 5.9. A value of 0 has an unknown
        /// start time.
        /// </summary>
        property uint64                                       StartTime;
        /// <summary>
        /// Gets or sets the start end of the session in NTP format as
        /// described in [RFC4566] Section 5.9. A value of 0 has an unbounded
        /// end time.
        /// </summary>
        property uint64                                       EndTime;
        /// <summary>
        /// Gets or sets the connection data associated with the session as 
        /// described in [RFC4566] Section 5.7 for RTP and [RFC3605]
        /// Section 2.1 for RTCP (at the session level).
        /// </summary>
        property RTCSessionDescriptionConnectionData^         ConnectionData;

        /// <summary>
        /// Constructs an instance of an
        /// RTCSessionDescriptionDescriptionDetails object.
        /// </summary>
        RTCSessionDescriptionDescriptionDetails()
        {
          Username = "-";
          StartTime = 0;
          EndTime = 0;
        }
      };

      /// <summary>
      /// The RTCSessionDescriptionConnectionData object describes the
      /// connection data information as described in [RFC4566] Section 5.7
      /// for RTP and [RFC3605] Section 2.1 for RTCP (at either the session
      /// level or the media level).
      /// </summary>
      public ref struct RTCSessionDescriptionConnectionData sealed
      {
        /// <summary>
        /// Gets or sets the RTP connection data information "c=" as described
        /// in [RFC4566] Section 5.7.
        /// </summary>
        property RTCSessionDescriptionConnectionDataDetails^ Rtp;
        /// <summary>
        /// Gets or sets the RTCP connection data information "a=rtcp" as
        /// described in [RFC3605] Section 2.1.
        /// </summary>
        property RTCSessionDescriptionConnectionDataDetails^ Rtcp;
      };

      /// <summary>
      /// The RTCSessionDescriptionConnectionDataDetails object describes the
      /// RTP/RTCP connection data at the session or media level or the
      /// unicast address information for the origin at the session level as
      /// described in [RFC4566] Section 5.7 for RTP and [RFC3605] Section 2.1
      /// for RTCP and [RFC4566] Section 5.2 for the origin's unicast address.
      /// </summary>
      /// <example>
      /// <code>
      /// o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
      /// </code>
      /// <code>
      /// c=IN IP4 224.2.17.12/127
      /// </code>
      /// <code>
      /// a=rtcp:53020
      /// </code>
      /// <code>
      /// a=rtcp:53020 IN IP4 126.16.64.4
      /// </code>
      /// </example>
      public ref struct RTCSessionDescriptionConnectionDataDetails sealed
      {
        /// <summary>
        /// Gets or sets the port for RTP or RTCP. For RTP at the media level
        /// this is the media description's port as described in [RFC4566]
        /// Section 5.14. For RTCP, the port is the RTCP port to use for the
        /// connection as described in [RFC3605] Section 2.1. This value is
        /// null and not used for origin unicast address or at the session
        /// level's connection address.
        /// </summary>
        property Platform::IBox<uint16>^                    Port;
        /// <summary>
        /// Gets or sets the network type as described in [RFC4566] Section
        /// 8.2.6. The value "IN" represents the Internet.
        /// </summary>
        property Platform::String^                          NetType;
        /// <summary>
        /// Gets or sets the address type as described in [RFC4566] Section
        /// 8.2.7. The value "IP4" represents the IPv4 and "IP6" represents
        /// IPv6.
        /// </summary>
        property Platform::String^                          AddrType;
        /// <summary>
        /// Gets or sets the connection address as described in [RFC4566]
        /// Section 5.7 for session or media level connection data or the
        /// unicast address for the session level origin as describe in 
        /// [RFC4566] Section 5.2
        /// </summary>
        property Platform::String^                          ConnectionAddress;
      };


      /// <summary>
      /// The RTCSessionDescriptionTransport object represents the ICE and
      /// secure transport parameters for RTP and optionally RTCP if non-muxed
      /// RTP/RTCP data is used.
      /// </summary>
      public ref struct RTCSessionDescriptionTransport sealed
      {
        /// <summary>
        /// Gets or sets a unique identifier for the transport. For SDP,
        /// this is the mid identification tag as defined in [RFC5888] Section
        /// 4 for the media line's transport or the primary bundled transport
        /// transport media line identifier.
        /// </summary>
        /// <example>
        /// <code>
        /// m=audio 30000 RTP/AVP 0
        /// a=mid:1
        /// m=video 30002 RTP/AVP 31
        /// a=mid:2
        /// </code>
        /// </example>
        property Platform::String^                          Id;
        /// <summary>
        /// Gets or sets the RTP transport information for use with RTP and
        /// RTCP media in the case of muxed RTP/RTCP and only the RTP media
        /// in the non-muxed scenario. This contains the information for ICE
        /// and the secure transport.
        /// </summary>
        property RTCSessionDescriptionTransportParameters^  Rtp;
        /// <summary>
        /// Gets or sets the RTCP transport information for use with RTCP if
        /// non-muxed RTCP data is used otherwise the value is null. This
        /// contains the ICE transport information and secure transport
        /// information for use with RTCP.
        /// </summary>
        property RTCSessionDescriptionTransportParameters^  Rtcp;
      };

      /// <summary>
      /// The RTCSessionDescriptionTransportParameters represents the ICE
      /// and secure transport parameters for a transport as well as any
      /// candidates that are signaled as part of the session description for
      /// a given transport.
      /// </summary>
      public ref struct RTCSessionDescriptionTransportParameters sealed
      {
        /// <summary>
        /// Gets or sets all the ICE parameters needed to configure the ICE
        /// ICE transport as described in [RFC5245].
        /// </summary>
        property RTCIceParameters^                      IceParameters;
        /// <summary>
        /// Gets or sets the DTLS parameters need to configure the DTLS 
        /// transport as described in [RFC4572] and [RFC5763].
        /// </summary>
        property RTCDtlsParameters^                     DtlsParameters;
        /// <summary>
        /// Gets or sets the SRTP/SDES parameters as described in [RFC4568].
        /// This transport is mutually exclusive with the DTLS transport and
        /// must always be set null for RTCP transports.
        /// </summary>
        property RTCSrtpSdesParameters^                 SrtpSdesParameters;
        /// <summary>
        /// Gets or sets the list of ICE candidates as described in [RFC5245].
        /// </summary>
        property IVector<ortc::RTCIceCandidate^>^       IceCandidates;
        /// <summary>
        /// Gets or sets if the end-of-candidates flag as described in
        /// [draft-ietf-ice-trickle] Section 10.
        /// </summary>
        property Platform::Boolean                      EndOfCandidates;
      };

      /// <summary>
      /// Gets or sets the media line detailed information associated with a
      /// type of media and related capabilities for the media on a particular
      /// transport as describe in [RFC4566] Section 5.14 as well as in
      /// [draft-ietf-rtcweb-jsep-14].
      /// </summary>
      public ref struct RTCSessionDescriptionMediaLineDetails sealed
      {
        /// <summary>
        /// Gets or sets the internal index of a media line based upon its
        /// order within the entire session. This is used only when the
        /// media line has no identifier. This value is used internally
        /// to coordinate matching m lines for SDP negotiation but otherwise
        /// is not recommended.
        /// </summary>
        property Platform::IBox<uint16>^                InternalIndex;
        /// <summary>
        /// Gets or sets the private transport to use when the media line
        /// cannot be bundled withinn the session. This is allows a legacy
        /// systems that are not capable of bundling media lines together an
        /// option of including their own private alternative non-bundled
        /// transport as described in
        /// [draft-ietf-mmusic-sdp-bundle-negotiation]. If the media line's
        /// TransportId and PrivateTransportId are identical then this
        /// transport either is part of a bundle or the transport will be used
        /// privately and exclusively by this transport depending on what is
        /// negotiated. If these values differ then the media line will be
        /// either bundled into a single transport or use a private transport.
        /// </summary>
        property Platform::String^                      PrivateTransportId;
        /// <summary>
        /// Gets or sets the protocol for use with DTLS and RTP as descibed in
        /// [RFC5764] Section 8 or for DTLS and SCTP as described in
        /// [draft-ietf-mmusic-sctp-sdp] Section 7 and 8. The following values
        /// match any values are are legal for RTP "RTP/[S]AVP[F]" and
        /// "(UDP/TCP)/TLS/RTP/SAVP[F]" and for SCTP the values are
        /// "UDP/DTLS/SCTP", "TCP/DTLS/SCTP", or "DTLS/SCTP
        /// </summary>
        property Platform::String^                      Protocol;
        /// <summary>
        /// Gets or sets the media line connection information as described in
        /// [RFC4566] Section 5.7 for RTP and [RFC3605] Section 2.1 for RTCP.
        /// </summary>
        property RTCSessionDescriptionConnectionData^   ConnectionData;
        /// <summary>
        /// Gets or sets the media direction flow expectations for the media
        /// line as described in [RFC4566] Section 6.
        /// </summary>
        property RTCSessionDescriptionMediaDirection    MediaDirection;

        /// <summary>
        /// Constructs and instance of an
        /// RTCSessionDescriptionMediaLineDetails object.
        /// </summary>
        RTCSessionDescriptionMediaLineDetails()
        {
          MediaDirection = RTCSessionDescriptionMediaDirection::SendReceive;
        }
      };

      /// <summary>
      /// The RTCSessionDescriptionRtpMediaLine object represents the media
      /// capabilities and properties associated with a particular media type
      /// on a given transport. Multiple media lines are legally allowed to
      /// be bundled within the same transport so long as the payload type
      /// and/or RTP header extension IDs are not remapped to mean
      /// different codecs or header extension URIs within the same transport
      /// for the same direction for JSOn or any direction for SDP. In SDP,
      /// a media line is required per sender and the if the media line is
      /// send receive the both sides should attempt to map their respective
      /// senders to the same media line.
      /// </summary>
      /// <example>
      /// <code>
      /// m=audio 56500 UDP/TLS/RTP/SAVPF 96 0 8 97 98
      /// c=IN IP4 192.0.2.1
      /// a=sendrecv
      /// a=mid:a1
      /// m=video 56502 UDP/TLS/RTP/SAVPF 100 101
      /// c=IN IP4 192.0.2.1
      /// a=rtcp:56503 IN IP4 192.0.2.1
      /// a=sendonly
      /// a=mid:v1
      /// m=audio 56500 TCP/DTLS/RTP/SAVPF 96 0 8 97 98
      /// c=IN IP4 192.0.2.1
      /// a=sendrecv
      /// a=mid:a2
      /// </code>
      /// </example>
      public ref struct RTCSessionDescriptionRtpMediaLine sealed
      {
        /// <summary>
        /// Gets or sets the media identifier for the media line as described
        /// in [RFC5888] Section 4.
        /// </summary>
        property Platform::String^                      Id;
        /// <summary>
        /// Gets or sets the transport identifier for the media line. Multiple
        /// media lines can point to the same transport id so long as the
        /// payload type or RTP header extension IDs are not redefined within
        /// the same transport in the same direction for JSON or any direction
        /// for SDP. All media lines that share the same transport id are
        /// grouped together into the same bundle as described in
        /// [draft-ietf-mmusic-sdp-bundle-negotiation].
        /// </summary>
        /// <example>
        /// <code>
        /// v=0
        /// o=alice 2890844526 2890844526 IN IP4 atlanta.example.com
        /// s=
        /// c=IN IP4 atlanta.example.com
        /// t=0 0
        /// a=group:BUNDLE foo bar
        /// m=audio 10000 RTP/AVP 0 8 97
        /// b=AS:200
        /// a=mid:foo
        /// a=rtpmap:0 PCMU/8000
        /// a=rtpmap:8 PCMA/8000
        /// a=rtpmap:97 iLBC/8000
        /// a=extmap 1 urn:ietf:params:rtp-hdrext:sdes:mid
        /// m=video 10002 RTP/AVP 31 32
        /// b=AS:1000
        /// a=mid:bar
        /// a=rtpmap:31 H261/90000
        /// a=rtpmap:32 MPV/90000
        /// a=extmap 1 urn:ietf:params:rtp-hdrext:sdes:mid
        /// </code>
        /// </example>
        property Platform::String^                      TransportId;
        /// <summary>
        /// Gets or sets the RTP media type. This value must be either "audio"
        /// or "video".
        /// </summary>
        property Platform::String^                      MediaType;
        /// <summary>
        /// Gets or sets the details of the media line which are required for
        /// SDP but optional for JSON based signalling. If not usedt his value
        /// must be null.
        /// </summary>
        property RTCSessionDescriptionMediaLineDetails^ Details;
        /// <summary>
        /// Gets or sets the sender RTP capabilities described for this media
        /// line. The usage of each codec is optional within the parameters of
        /// a sender within the media line but the payload type must not
        /// be changed within the same media line. The RTP header extension
        /// IDs specified within the capabilies do not need to be used within
        /// the RTP parameters but the mapping between RTP header extension
        /// ID and header extension value must not be changed within the RTP
        /// parameters.
        /// </summary>
        property RTCRtpCapabilities^                    SenderCapabilities;
        /// <summary>
        /// Gets or sets the sender RTP capabilities described for this media
        /// line. The usage of each codec is optional within the parameters of
        /// a sender within the media line but the payload type must not
        /// be changed within the same media line. The RTP header extension
        /// IDs specified within the capabilies do not need to be used within
        /// the RTP parameters but the mapping between RTP header extension
        /// ID and header extension value must not be changed within the RTP
        /// parameters.
        /// </summary>
        property RTCRtpCapabilities^                    ReceiverCapabilities;
      };

      /// <summary>
      /// The RTCSessionDescriptionSctpMediaLine object represents the media
      /// lines related to an SCTP transport assoacated to an ICE/secure
      /// transport on a given SCTP port. Multiple data channels can
      /// operate over the same SCTP transport thus creating multiple
      /// SCTP transports is not needed unless for application specific logic
      /// reasons.
      /// </summary>
      /// <example>
      /// <code>
      /// m=application 0 UDP/DTLS/SCTP webrtc-datachannel
      /// c=IN IP4 0.0.0.0
      /// a=bundle-only
      /// a=mid:d1
      /// a=fmtp:webrtc-datachannel max-message-size=65536
      /// a=sctp-port 5000
      /// </code>
      /// </example>
      public ref struct RTCSessionDescriptionSctpMediaLine sealed
      {
        /// <summary>
        /// Gets or sets the data media line identifier for the SCTP
        /// transport.
        /// </summary>
        property Platform::String^                      Id;
        /// <summary>
        /// Gets or sets the associated transport identifier where the SCTP
        /// transport data will flow.
        /// </summary>
        property Platform::String^                      TransportId;
        /// <summary>
        /// Gets or sets the media type associated with the SCTP transport.
        /// This value must be "application" for SCTP.
        /// </summary>
        property Platform::String^                      MediaType;
        /// <summary>
        /// Gets or sets the detailed SCTP media line information. This is
        /// required for SDP signaling but is optional for JSON signaling.
        /// </summary>
        property RTCSessionDescriptionMediaLineDetails^ Details;
        /// <summary>
        /// Gets or sets the SCTP transport's capabilities associated with
        /// the SCTP transport.
        /// </summary>
        property RTCSctpCapabilities^                   Capabilities;
        /// <summary>
        /// Gets or sets the SCTP transport port associated with the SCTP
        /// transport. This is required for SDP based singaling but is
        /// optional for JSON based signaling but requires the engine support
        /// listening for incoming SCTP transports on any port within the
        /// transport. Only one non-specified port (null value) is allowed per
        /// transport and all non-signaled SCTP transport must share the same
        /// SCTP capabilities for the transport. Setting to null implies
        /// the SCTP transport is capable of listening for incoming SCTP
        /// transport connections.
        /// </summary>
        property Platform::IBox<uint16>^                Port;
      };

      /// <summary>
      /// The RTCSessionDescriptionRtpSender object represents the sender
      /// parameters for an encoded audio or video media stream track.
      /// </summary>
      public ref struct RTCSessionDescriptionRtpSender sealed
      {
        /// <summary>
        /// Gets or sets the sender identifier. In the case of SDP based
        /// signaling this value must correspond to the media stream track
        /// identifier that the remote party will adopt as it's media
        /// stream track identification as described in
        /// [draft-ietf-mmusic-msid] Section 2, i.e. the "msid-appdata" value.
        /// </summary>
        property Platform::String^                      Id;
        /// <summary>
        /// Gets or sets the sender details associated with this sender. These
        /// values are typically only used within the context of SDP and
        /// thus property may be null if unused.
        /// </summary>
        property RTCSessionDescriptionRtpSenderDetails^ Details;
        /// <summary>
        /// Gets or sets the associated media line. In the case of SDP
        /// signaling only one sender is allowed to be associated to a single
        /// media line at a time. In the case of JSON, multiple senders are
        /// allowed to be associated to a single media line. The sender media
        /// type and media line media type must be identical.
        /// </summary>
        property Platform::String^                      RtpMediaLineId;
        /// <summary>
        /// </summary>
        property RTCRtpParameters^                      Parameters;
        /// <summary>
        /// Gets or sets the media stream group ID associated with the media
        /// line [RFC5888]  Section 5. All media lines that share the same
        /// group identification are considered part of the same group and
        /// thus are part of the same media stream.
        /// </summary>
        /// <example>
        /// <code>
        /// v=0
        /// o=- 6729291447651054566 1 IN IP4 0.0.0.0
        /// s=-
        /// t=0 0
        /// a=group:BUNDLE a1 v1
        /// m=audio 20000 UDP/TLS/RTP/SAVPF 96 0 8 97 98
        /// c=IN IP4 192.0.2.2
        /// a=mid:a1
        /// a=rtcp:20000 IN IP4 192.0.2.2
        /// a=msid:PI39StLS8W7ZbQl1sJsWUXkr3Zf12fJUvzQ1
        ///        PI39StLS8W7ZbQl1sJsWUXkr3Zf12fJUvzQ1a0
        /// a=msid:fdsafjdksajlfEjalkfjDlskajaflJakfjd1
        ///        PI39StLS8W7ZbQl1sJsWUXkr3Zf12fJUvzQ1a0
        /// ...
        /// m=video 20000 UDP/TLS/RTP/SAVPF 100 101
        /// c=IN IP4 192.0.2.2
        /// a=rtcp 20001 IN IP4 192.0.2.2
        /// a=mid:v1
        /// a=msid:PI39StLS8W7ZbQl1sJsWUXkr3Zf12fJUvzQ1
        ///        PI39StLS8W7ZbQl1sJsWUXkr3Zf12fJUvzQ1v0
        /// </code>
        /// </example>
        property IVector<Platform::String^>^            MediaStreamIds;
      };

      /// <summary>
      /// The RTCSessionDescriptionRtpSenderDetails object represents the
      /// sender details that are typically only related to SDP properties for
      /// a specific sender.
      /// </summary>
      public ref struct RTCSessionDescriptionRtpSenderDetails sealed
      {
        /// <summary>
        /// Gets or sets the associated internal index of a media line
        /// associated with a sender based upon the media line order within
        /// the entire session. This is used only when the media line has no
        /// identifier. This value is used internally to coordinate matching
        /// m (media) lines for SDP negotiation but otherwise is not
        /// recommended.
        /// </summary>
        property Platform::IBox<uint16>^              InternalRtpMediaLineIndex;
      };

      /// <summary>
      /// The Session description version of an RTCIceCandidate object
      /// includes information relating to an ICE candidate.
      /// </summary>
      public ref struct RTCIceCandidate sealed
      {
        /// <summary>
        /// Gets or sets the candidate-attribute as defined in section 15.1 of
        /// [RFC5245].
        /// </summary>
        property Platform::String^            Candidate;
        /// <summary>
        /// Gets or sets the identifier of the "media stream identification"
        /// as defined in [RFC5888] for the media component this candidate is
        /// associated with.
        /// </summary>
        property Platform::String^            SdpMid;
        /// <summary>
        /// Gets or sets the index (starting at zero) of the media description
        /// in the SDP this candidate is associated with.
        /// </summary>
        property Platform::IBox<uint16>^      SdpMLineIndex;
        /// <summary>
        /// Gets or sets the the component of the ICE candidate to indicate
        /// if the candidate is related to RTP or RTCP.
        /// </summary>
        property RTCIceComponent              Component;
        /// <summary>
        /// Gets or sets the type of interface this candidate was gathered
        /// from.
        /// </summary>
        property Platform::String^            InterfaceType;
        /// <summary>
        /// Gets or sets a unique identifier that allows ICE to correlate
        /// candidates that appear on multiple RTCIceTransports.
        /// </summary>
        property Platform::String^            Foundation;
        /// <summary>
        /// Gets or sets the assigned usage priority of the candidate. This
        /// priority represents the priority to choose this candidate over
        /// other candidates as described in [RFC5245]. If
        /// RTCIceParameters.UseUnfreezePriority is false then this priority
        /// also represents the priority in which to unfreeze ICE candidates.
        /// This is automatically populated by the RTC engine and/or browser.
        /// </summary>
        property uint32                       Priority;
        /// <summary>
        /// Gets or sets the unfreeze priority. If
        /// RTCIceParameters.UseUnfreezePriority is true then this priority
        /// is used to determine the unfreezing search order in which to check
        /// ICE candidates. The higher priority candidates will be searched
        /// as posssible pairings before lower priority candidates.
        /// This is a useful option as setting up a connection via a relay
        /// may be faster than exhaustively searching for the most optimal
        /// candidate pairing which is determined by standard Priority.
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
        /// candidates that are derived from others, such as relay or
        /// reflexive candidates, the relatedAddress refers to the candidate
        /// that these are derived from. For host candidates, the
        /// RelatedAddress is set to the empty string.
        /// </summary>
        property Platform::String^            RelatedAddress;
        /// <summary>
        /// Gets or sets the related port if not a host candidate type. For
        /// candidates that are derived from others, such as relay or
        /// reflexive candidates, the relatedPort refers to the host candidate
        /// that these are derived from. For host candidates, the RelatedPort
        /// is null.
        /// </summary>
        property uint16                       RelatedPort;

        /// <summary>
        /// A helper routine to easily convery from an RTCIceCandidate to a
        /// JSON string.
        /// </summary>
        Platform::String^ ToJsonString();
        /// <summary>
        /// A helper routine to easily converty from a JSON string to an
        /// RTCIceCandidate object.
        /// </summary>
        static RTCIceCandidate^ FromJsonString(Platform::String^ jsonString);

        /// <summary>
        /// A helper routine to easily convery from an RTCIceCandidate to a
        /// SDP-candidate string.
        /// </summary>
        Platform::String^ ToSdpString();
        /// <summary>
        /// A helper routine to easily converty from a SDP-candidate string to
        /// an RTCIceCandidate object.
        /// </summary>
        static RTCIceCandidate^ FromSdpString(Platform::String^ sdpString);

        /// <summary>
        /// Constructs and instance of an RTCIceCandidate object.
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
      /// The Session description version of an RTCIceCandidateComplete object
      /// includes information relating to the end-of-candidate flag.
      /// </summary>
      public ref struct RTCIceCandidateComplete sealed
      {
        /// <summary>
        /// Gets or sets the candidate-attribute as defined in section 15.1 of
        /// [RFC5245].
        /// </summary>
        property Platform::String^            Candidate;
        /// <summary>
        /// Gets or sets the identifier of the "media stream identification"
        /// as defined in [RFC5888] for the media component this candidate is
        /// associated with.
        /// </summary>
        property Platform::String^            SdpMid;
        /// <summary>
        /// Gets or sets the index (starting at zero) of the media description
        /// in the SDP this candidate is associated with.
        /// </summary>
        property Platform::IBox<uint16>^      SdpMLineIndex;
        /// <summary>
        /// Gets or sets the the component of the ICE candidate to indicate
        /// if the candidate is related to RTP or RTCP.
        /// </summary>
        property RTCIceComponent              Component;

        /// <summary>
        /// Gets or sets the completed attribute indication that end of
        /// candidates has been reached. This property should always be true.
        /// </summary>
        property Platform::Boolean            Complete;

        /// <summary>
        /// A helper routine to easily convery from an RTCIceCandidateComplete
        /// to a JSON string.
        /// </summary>
        Platform::String^ ToJsonString();
        /// <summary>
        /// A helper routine to easily converty from a JSON string to an
        /// RTCIceCandidateComplete object.
        /// </summary>
        static RTCIceCandidateComplete^ FromJsonString(Platform::String^ jsonString);

        /// <summary>
        /// A helper routine to easily convery from an RTCIceCandidateComplete
        /// to a SDP-candidate string.
        /// </summary>
        Platform::String^ ToSdpString();
        /// <summary>
        /// A helper routine to easily converty from a SDP-candidate string to
        /// an RTCIceCandidateComplete object.
        /// </summary>
        static RTCIceCandidateComplete^ FromSdpString(Platform::String^ sdpString);

        /// <summary>
        /// Constructs an instance of the RTCIceCandidateComplete object.
        /// </summary>
        RTCIceCandidateComplete()
        {
          Component = RTCIceComponent::Rtp;
          Complete = true;
        }
      };

      /// <summary>
      /// The RTCSessionDescription class is used by RTCPeerConnection to
      /// expose local and remote session descriptions.
      /// </summary>
      public ref class RTCSessionDescription sealed
      {
      private:
        friend class internal::RTCPeerConnectionPromiseWithDescriptionObserver;
        friend ref class RTCPeerConnection;

      private:
        RTCSessionDescription(ISessionDescriptionPtr description);

        static RTCSessionDescription^ Convert(ISessionDescriptionPtr session) { if (!session) return nullptr; return ref new RTCSessionDescription(session); }
        static ISessionDescriptionPtr Convert(RTCSessionDescription^ session) { if (!session) return nullptr; return session->_nativePointer; }

      public:

        /// <summary>
        /// Helper routine to convert RTP capabilities into the appropriate
        /// matching RTP parameters.
        /// </summary>
        static RTCRtpParameters^ ConvertCapabilitiesToParameters(RTCRtpCapabilities^ capabilitites);
        /// <summary>
        /// Helper routine to convert existing RTP parameters into the RTP
        /// capabilities.
        /// </summary>
        static RTCRtpCapabilities^ ConvertParametersToCapabilitites(RTCRtpParameters^ parameters);

        /// <summary>
        /// Constructs an instance of an RTCSessionDescription from a string
        /// representation of the session description.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        RTCSessionDescription(
                              RTCSessionDescriptionSignalingType type,
                              Platform::String^ description
                              );

        /// <summary>
        /// Constructs an instance of an RTCSessionDescription from a
        /// structured session description.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("CreateWithDescription")]
        RTCSessionDescription(
                              RTCSessionDescriptionSignalingType type,
                              RTCSessionDescriptionDescription^ description
                              );

        /// <summary>
        /// Gets the SDP type of this RTCSessionDescription. This property
        /// may only be obtained if the signaling type is SDP.
        /// </summary>
        property RTCSdpType Type
        {
          RTCSdpType get();
        }

        /// <summary>
        /// Gets the type of this RTCSessionDescription.
        /// </summary>
        property RTCSessionDescriptionSignalingType SignalingType
        {
          RTCSessionDescriptionSignalingType get();
        }

        /// <summary>
        /// Gets a indiciation if the signaling type is JSON based.
        /// </summary>
        property Platform::Boolean IsJsonSignaling
        {
          Platform::Boolean get();
        }
        /// <summary>
        /// Gets a indiciation if the signaling type is SDP based.
        /// </summary>
        property Platform::Boolean IsSdpSignaling
        {
          Platform::Boolean get();
        }

        /// <summary>
        /// Gets the structured representation of the SDP [RFC3264] or JSON
        /// contents.
        /// </summary>
        property RTCSessionDescriptionDescription^  Description
        {
          RTCSessionDescriptionDescription^ get();
        }
        /// <summary>
        /// Gets the string representation of the SDP [RFC3264] or JSON.
        /// </summary>
        property Platform::String^                  FormattedDescription
        {
          Platform::String^ get();
        }
        /// <summary>
        /// Gets the string representation of the SDP [RFC3264]. This property
        /// may only be obtained if the signaling type is SDP.
        /// </summary>
        property Platform::String^                  Sdp
        {
          Platform::String^ get();
        }

      private:
        ISessionDescriptionPtr _nativePointer;
      };

    }

  } // namespace ortc
} // namespace org
