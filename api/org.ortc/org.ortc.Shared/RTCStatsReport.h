#pragma once

#include <ortc/IStatsReport.h>

#include "RTCDataChannel.h"
#include "RTCIceTypes.h"

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IStatsReportTypes, IStatsReportTypes);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IStatsReport, IStatsReport);

    using Windows::Foundation::Collections::IVector;

    ref struct RTCStats;
    ref struct RTCRTPStreamStats;
    ref struct RTCCodec;
    ref struct RTCInboundRtpStreamStats;
    ref struct RTCOutboundRtpStreamStats;
    ref struct RTCSctpTransportStats;
    ref struct RTCMediaStreamStats;
    ref struct RTCMediaStreamTrackStats;
    ref struct RTCDataChannelStats;
    ref struct RTCIceGathererStats;
    ref struct RTCIceTransportStats;
    ref struct RTCDtlsTransportStats;
    ref struct RTCSrtpTransportStats;
    ref struct RTCIceCandidateAttributes;
    ref struct RTCIceCandidatePairStats;
    ref struct RTCCertificateStats;

    ref class RTCStatsReport;
    ref class RTCStatsProvider;

    enum class RTCDataChannelState;
    enum class RTCIceCandidateType;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCStateReportHelper);
      ZS_DECLARE_CLASS_PTR(RTCStateProviderObserver);

      RTCStats^ ToCx(const IStatsReportTypes::Stats &input);
      RTCStats^ ToCx(IStatsReportTypes::StatsPtr input);
      IStatsReportTypes::StatsPtr FromCx(RTCStats^ input);

      RTCRTPStreamStats^ ToCx(const IStatsReportTypes::RTPStreamStats &input);
      RTCRTPStreamStats^ ToCx(IStatsReportTypes::RTPStreamStatsPtr input);

      RTCCodec^ ToCx(const IStatsReportTypes::Codec &input);
      RTCCodec^ ToCx(IStatsReportTypes::CodecPtr input);

      RTCInboundRtpStreamStats^ ToCx(const IStatsReportTypes::InboundRTPStreamStats &input);
      RTCInboundRtpStreamStats^ ToCx(IStatsReportTypes::InboundRTPStreamStatsPtr input);

      RTCOutboundRtpStreamStats^ ToCx(const IStatsReportTypes::OutboundRTPStreamStats &input);
      RTCOutboundRtpStreamStats^ ToCx(IStatsReportTypes::OutboundRTPStreamStatsPtr input);

      RTCSctpTransportStats^ ToCx(const IStatsReportTypes::SCTPTransportStats &input);
      RTCSctpTransportStats^ ToCx(IStatsReportTypes::SCTPTransportStatsPtr input);

      RTCMediaStreamStats^ ToCx(const IStatsReportTypes::MediaStreamStats &input);
      RTCMediaStreamStats^ ToCx(IStatsReportTypes::MediaStreamStatsPtr input);

      RTCMediaStreamTrackStats^ ToCx(const IStatsReportTypes::MediaStreamTrackStats &input);
      RTCMediaStreamTrackStats^ ToCx(IStatsReportTypes::MediaStreamTrackStatsPtr input);

      RTCDataChannelStats^ ToCx(const IStatsReportTypes::DataChannelStats &input);
      RTCDataChannelStats^ ToCx(IStatsReportTypes::DataChannelStatsPtr input);

      RTCIceGathererStats^ ToCx(const IStatsReportTypes::ICEGathererStats &input);
      RTCIceGathererStats^ ToCx(IStatsReportTypes::ICEGathererStatsPtr input);

      RTCIceTransportStats^ ToCx(const IStatsReportTypes::ICETransportStats &input);
      RTCIceTransportStats^ ToCx(IStatsReportTypes::ICETransportStatsPtr input);

      RTCDtlsTransportStats^ ToCx(const IStatsReportTypes::DTLSTransportStats &input);
      RTCDtlsTransportStats^ ToCx(IStatsReportTypes::DTLSTransportStatsPtr input);

      RTCSrtpTransportStats^ ToCx(const IStatsReportTypes::SRTPTransportStats &input);
      RTCSrtpTransportStats^ ToCx(IStatsReportTypes::SRTPTransportStatsPtr input);

      RTCIceCandidateAttributes^ ToCx(const IStatsReportTypes::ICECandidateAttributes &input);
      RTCIceCandidateAttributes^ ToCx(IStatsReportTypes::ICECandidateAttributesPtr input);

      RTCIceCandidatePairStats^ ToCx(const IStatsReportTypes::ICECandidatePairStats &input);
      RTCIceCandidatePairStats^ ToCx(IStatsReportTypes::ICECandidatePairStatsPtr input);

      RTCCertificateStats^ ToCx(const IStatsReportTypes::CertificateStats &input);
      RTCCertificateStats^ ToCx(IStatsReportTypes::CertificateStatsPtr input);

    } // namespace internal

    /// <summary>
    /// RTCStatsType object is initialized to the name of the dictionary that
    /// the RTCStats represents.
    /// </summary>
    public enum class RTCStatsType
    {
      /// <summary>
      /// Statistics returned are unknown and thus should be ignored.
      /// </summary>
      Unknown,
      /// <summary>
      /// Statistics for the inbound RTP stream that is currently received
      /// with this RTCRtpReceiver object. It is accessed by the
      /// RTCInboundRtpStreamStats.
      /// </summary>
      InboundRtp,
      /// <summary>
      /// Statistics for the outbound RTP stream that is currently sent with
      /// this RTCRtpSender object. It is accessed by the
      /// RTCOutboundRtpStreamStats.
      /// </summary>
      OutboundRtp,
      /// <summary>
      /// Codec information for the RTCRtpReceiver or RTCRtpSender objects. It
      /// is accessed by the RTCCodec.
      /// </summary>
      Codec,
      /// <summary>
      /// Statistics for the RTCSctpTransport object. It is accessed by the
      /// RTCSctpTransportStats.
      /// </summary>
      SctpTransport,
      /// <summary>
      /// Statistics for the RTCDataChannel object. It is accessed by the
      /// RTCDataChannelStats.
      /// </summary>
      DataChannel,
      /// <summary>
      /// Statistics for the RTCMediaStream object. It is accessed by the
      /// RTCMediaStreamStats.
      /// </summary>
      Stream,
      /// <summary>
      /// Statistics for the RTCMediaStreamTrack object. It is accessed by the
      /// RTCMediaStreamTrackStats.
      /// </summary>
      Track,
      /// <summary>
      /// Statistics for the RTCIceGatherer object. It is accessed by the
      /// RTCIceGathererStats.
      /// </summary>
      IceGatherer,
      /// <summary>
      /// Statistics for the RTCIceTransport object. It is accessed by the
      /// RTCIceTransportStats.
      /// </summary>
      IceTransport,
      /// <summary>
      /// Statistics for the RTCDtlsTransport object. It is accessed by the
      /// RTCDtlsTransportStats.
      /// </summary>
      DtlsTransport,
      /// <summary>
      /// Statistics for the RTCSrtpTransport object. It is accessed by the
      /// RTCSrtpTransportStats.
      /// </summary>
      SrtpTransport,
      /// <summary>
      /// Statistics for the RTCCertificate object. It is accessed by the
      /// RTCCertificateStats.
      /// </summary>
      Certificate,
      /// <summary>
      /// Statistics for the RTCIceGatherer or RTCIceTransport object. It is
      /// accessed by the RTCIceCandidateAttributes.
      /// </summary>
      Candidate,
      /// <summary>
      /// Statistics for the RTCIceCandidate object from the RTCIceGatherer
      /// object. It is accessed by the RTCIceCandidatePairStats.
      /// </summary>
      CandidatePair,
      /// <summary>
      /// Statistics for the local RTCIceCandidate from the RTCIceTransport
      /// object. It is accessed by the RTCIceCandidateAttributes.
      /// </summary>
      LocalCandidate,
      /// <summary>
      /// Statistics for the remote RTCIceCandidate from the RTCIceTransport
      /// object. It is accessed by the RTCIceCandidateAttributes.
      /// </summary>
      RemoteCandidate
    };

    /// <summary>
    /// Canadidate pair state is defined in Section 5.7.4 of [RFC5245].
    /// </summary>
    public enum class RTCStatsIceCandidatePairState
    {
      /// <summary>
      /// Defined in Section 5.7.4 of [RFC5245].
      /// </summary>
      Frozen,
      /// <summary>
      /// Defined in Section 5.7.4 of [RFC5245].
      /// </summary>
      Waiting,
      /// <summary>
      /// Defined in Section 5.7.4 of [RFC5245].
      /// </summary>
      InProgress,
      /// <summary>
      /// Defined in Section 5.7.4 of [RFC5245].
      /// </summary>
      Failed,
      /// <summary>
      /// Defined in Section 5.7.4 of [RFC5245].
      /// </summary>
      Succeeded,
      /// <summary>
      /// Defined in Section 5.7.4 of [RFC5245].
      /// </summary>
      Cancelled
    };

    /// <summary>
    /// The RTCStats contains statistics information related to an inspected
    /// object.
    /// </summary>
    public ref struct RTCStats sealed
    {
      friend class internal::RTCStateReportHelper;

      /// <summary>
      /// Gets or sets the timestamp when this statistic object was generated.
      /// </summary>
      property Windows::Foundation::DateTime  Timestamp;
      /// <summary>
      /// Gets or sets the type of statistics object contained within this
      /// RTCStats object.
      /// </summary>
      property RTCStatsType                   StatsType;
      /// <summary>
      /// Gets or sets the identifer for the RTCStats object. The identifer
      /// is unique within a RTCStatsReport but is not guarenteed to be unique
      /// across RTCStatsReports.
      /// </summary>
      property Platform::String^              Id;

      /// <summary>
      /// Convert the RTCStats object to inbound RTP stats represented by the
      /// RTCInboundRtpStreamStats object.
      /// </summary>
      RTCInboundRtpStreamStats^               ToInboundRtp();
      /// <summary>
      /// Convert the RTCStats object to outbound RTP stats represented by the
      /// RTCOutboundRtpStreamStats object.
      /// </summary>
      RTCOutboundRtpStreamStats^              ToOutboundRtp();
      /// <summary>
      /// Convert the RTCStats object to codec information represented by the
      /// RTCCodec object.
      /// </summary>
      RTCCodec^                               ToCodec();
      /// <summary>
      /// Convert the RTCStats object to SCTP transport stats information
      /// represented by the RTCSctpTransportStats object.
      /// </summary>
      RTCSctpTransportStats^                  ToSctpTransport();
      /// <summary>
      /// Convert the RTCStats object to data channel stats information
      /// represented by the RTCDataChannelStats object.
      /// </summary>
      RTCDataChannelStats^                    ToDataChannel();
      /// <summary>
      /// Convert the RTCStats object to media stream stats information
      /// represented by the RTCMediaStreamStats object.
      /// </summary>
      RTCMediaStreamStats^                    ToStream();
      /// <summary>
      /// Convert the RTCStats object to media stream track stats information
      /// represented by the RTCMediaStreamTrackStats object.
      /// </summary>
      RTCMediaStreamTrackStats^               ToTrack();
      /// <summary>
      /// Convert the RTCStats object to ICE gatherer stats information
      /// represented by the RTCIceGathererStats object.
      /// </summary>
      RTCIceGathererStats^                    ToIceGatherer();
      /// <summary>
      /// Convert the RTCStats object to ICE transport stats information
      /// represented by the RTCIceTransportStats object.
      /// </summary>
      RTCIceTransportStats^                   ToIceTransport();
      /// <summary>
      /// Convert the RTCStats object to DTLS transport stats information
      /// represented by the RTCDtlsTransportStats object.
      /// </summary>
      RTCDtlsTransportStats^                  ToDtlsTransport();
      /// <summary>
      /// Convert the RTCStats object to SRTP transport stats information
      /// represented by the RTCSrtpTransportStats object.
      /// </summary>
      RTCSrtpTransportStats^                  ToSrtpTransport();
      /// <summary>
      /// Convert the RTCStats object to security certificate information
      /// represented by the RTCCertificateStats object.
      /// </summary>
      RTCCertificateStats^                    ToCertificate();
      /// <summary>
      /// Convert the RTCStats object to ICE candidate information represented
      /// by the RTCIceCandidateAttributes object.
      /// </summary>
      RTCIceCandidateAttributes^              ToCandidate();
      /// <summary>
      /// Convert the RTCStats object to ICE candidate pair information
      /// represented by the RTCIceCandidatePairStats object.
      /// </summary>
      RTCIceCandidatePairStats^               ToCandidatePair();
      /// <summary>
      /// Convert the RTCStats object to local ICE candidate information
      /// represented by the RTCIceCandidateAttributes object.
      /// </summary>
      RTCIceCandidateAttributes^              ToLocalCandidate();
      /// <summary>
      /// Convert the RTCStats object to remote ICE candidate information
      /// represented by the RTCIceCandidateAttributes object.
      /// </summary>
      RTCIceCandidateAttributes^              ToRemoteCandidate();

    private:
      IStatsReportTypes::StatsPtr mOriginal;
    };


    /// <summary>
    /// The RTCRTPStreamStats object represents statistics information for
    /// both inbound and outbound RTP streams.
    /// </summary>
    public ref struct RTCRTPStreamStats sealed
    {
      /// <summary>
      /// Gets or sets the SSRC for the RTP stream.
      /// </summary>
      property Platform::IBox<uint32>^        Ssrc;
      /// <summary>
      /// Gets or sets the associated stats identifier for looking up the
      /// corresponding (local/remote) RTCStats object for a given SSRC.
      /// </summary>
      property Platform::String^              AssociatedStatId;
      /// <summary>
      /// Gets or sets if the measurements are calculated locally ore
      /// remotely. false indicates that the statistics are measured
      /// locally, while true indicates that the measurements were done
      /// at the remote endpoint and reported in an RTCP RR/XR.
      /// </summary>
      property Platform::Boolean              IsRemote;
      /// <summary>
      /// Gets or sets the media type, either "audio" or "video". This must
      /// match the media type part of the information in the corresponding
      /// codec member of RTCCodec.
      /// </summary>
      property Platform::String^              MediaType;
      /// <summary>
      /// Gets or sets the media stream tracks ID associated with this RTP
      /// stream stats object.
      /// </summary>
      property Platform::String^              MediaTrackId;
      /// <summary>
      /// Gets or sets the transport ID associated with this RTP media
      /// stream stats object. It is a unique identifier that is associated
      /// to the object that was inspected to produce the RTCTransportStats
      /// associated with this RTP stream.
      /// </summary>
      property Platform::String^              TransportId;
      /// <summary>
      /// Gets or set the codec ID associated with this RTP media stream stats
      /// object.
      /// </summary>
      property Platform::String^              CodecId;
      /// <summary>
      /// Gets or sets the total number of Full Intra Request (FIR) packets
      /// received by the sender. This metric is only valid for video and is
      /// sent by receiver. Calculated as defined in [RFC5104] section 4.3.1.
      /// and does not use the metric indicated in [RFC2032], because it was
      /// deprecated by [RFC4587].
      /// </summary>
      property uint32                         FirCount;
      /// <summary>
      /// Gets or sets the total number of Packet Loss Indication (PLI)
      /// packets received by the sender and is sent by receiver. Calculated
      /// as defined in [RFC4585] section 6.3.1.
      /// </summary>
      property uint32                         PliCount;
      /// <summary>
      /// Gets or sets the total number of Negative ACKnowledgement (NACK)
      /// packets received by the sender and is sent by receiver. Calculated
      /// as defined in [RFC4585] section 6.2.1.
      /// </summary>
      property uint32                         NackCount;
      /// <summary>
      /// Gets or sets the total number of Slice Loss Indication (SLI) packets
      /// received by the sender. This metric is only valid for video and is
      /// sent by receiver. Calculated as defined in [RFC4585] section 6.3.2.
      /// </summary>
      property uint32                         SliCount;
    };

    /// <summary>
    /// The RTCCodec object contains information about the codec associated
    /// with an RTP stream.
    /// </summary>
    public ref struct RTCCodec sealed
    {
      /// <summary>
      /// Gets or sets the payload type as used in RTP encoding.
      /// </summary>
      property Platform::IBox<uint8>^         PayloadType;
      /// <summary>
      /// Gets or sets the IANA name of the codec associated with the RTP
      /// stream. For example, "vp8" for "video/vp8" or equivalent.
      /// </summary>
      property Platform::String^              Codec;
      /// <summary>
      /// Gets or sets the media sampling rate.
      /// </summary>
      property uint32                         ClockRate;
      /// <summary>
      /// Gets or sets the number of audio channels used by the codec. For
      /// example, 2 for stereo, 1 for mono, and missing for non-audio codecs.
      /// </summary>
      property Platform::IBox<uint32>^        Channels;
      /// <summary>
      /// Gets or sets the SDP description line.
      /// </summary>
      property Platform::String^              Parameters;
    };

    /// <summary>
    /// The RTCInboundRTPStreamStats object represents the measurement metrics
    /// for the incoming RTP media stream.
    /// </summary>
    public ref struct RTCInboundRtpStreamStats sealed
    {
      /// <summary>
      /// Gets or sets the base RTCRTPStreamStats associated with this
      /// inbound RTP stream.
      /// </summary>
      property RTCRTPStreamStats^             RtpStreamStats;
      /// <summary>
      /// Gets or sets the total number of RTP packets received for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1.
      /// </summary>
      property uint32                         PacketsReceived;
      /// <summary>
      /// Gets or sets the total number of bytes received for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1.
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// Gets or sets the total number of RTP packets lost for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1.
      /// </summary>
      property uint32                         PacketsLost;
      /// <summary>
      /// Gets or sets the packet jitter measured in seconds for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1.
      /// </summary>
      property float64                        Jitter;
      /// <summary>
      /// Gets or sets the fraction packet loss reported for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1 and Appendix A.3.
      /// </summary>
      property float64                        FractionLost;
      /// <summary>
      /// Gets or sets the end to end media delay in milliseconds. This value
      /// represents the total duration between the time the media sampling was
      /// captured to the time the media sampling was rendered.
      /// </summary>
      property uint64                         EndToEndDelay;
    };


    /// <summary>
    /// The RTCOutboundRTPStreamStats object represents the measurement
    /// metrics for the outgoing media stream.
    /// </summary>
    public ref struct RTCOutboundRtpStreamStats sealed
    {
      /// <summary>
      /// Gets or sets the base RTCRTPStreamStats associated with this
      /// outbound RTP stream.
      /// </summary>
      property RTCRTPStreamStats^             RtpStreamStats;
      /// <summary>
      /// Gets or sets the total number of RTP packets sent for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1.
      /// </summary>
      property uint32                         PacketsSent;
      /// <summary>
      /// Gets or sets the total number of bytes sent for this SSRC.
      /// Calculated as defined in [RFC3550] section 6.4.1.
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// Gets or sets the presently configured bitrate target of this SSRC,
      /// in bits per second. Typically this is a configuration parameter
      /// provided to the codec's encoder.
      /// </summary>
      property float64                        TargetBitrate;
      /// <summary>
      /// Gets or sets the estimated round trip time (seconds) for this SSRC
      /// based on the RTCP timestamp. Calculated as defined in [RFC3550]
      /// section 6.4.1.
      /// </summary>
      property float64                        RoundTripTime;
    };

    /// <summary>
    /// The RTCSctpTransportStats contains the statistics for the
    /// RTCSctpTransport object.
    /// </summary>
    public ref struct RTCSctpTransportStats sealed
    {
      /// <summary>
      /// Gets or sets the number of unique datachannels opened.
      /// </summary>
      property uint32                         DataChannelsOpened;
      /// <summary>
      /// Gets or sets the number of unique datachannels closed.
      /// </summary>
      property uint32                         DataChannelsClosed;
    };

    /// <summary>
    /// The RTCMediaStreamStats object contains the stastics for the
    /// RTCMediaStream object.
    /// </summary>
    public ref struct RTCMediaStreamStats sealed
    {
      /// <summary>
      /// Gets or sets the stream.id property.
      /// </summary>
      property Platform::String^              StreamId;
      /// <summary>
      /// Gets or sets the the id of the media stream track stats object, not
      /// the track.id.
      /// </summary>
      property IVector<Platform::String^>^    TrackIds;
    };

    /// <summary>
    /// The RTCMediaStreamTrackStats object contains the stastics for the
    /// RTCMediaStreamTrack object.
    /// </summary>
    public ref struct RTCMediaStreamTrackStats sealed
    {
      /// <summary>
      /// Gets or sets the track.id property.
      /// </summary>
      property Platform::String^              TrackId;
      /// <summary>
      /// Gets or sets if this source represents a local or remote media
      /// stream track source. The value is true for remote, false for local.
      /// </summary>
      property Platform::Boolean              RemoteSource;
      /// <summary>
      /// Gets or sets the SSRCs associated with this media stream track stats
      /// object.
      /// </summary>
      property IVector<uint32>^               SsrcIds;
      /// <summary>
      /// Gets or sets the width value for video. The property only makes
      /// sense for video media streams and represents the width of the video
      /// frame for this SSRC.
      /// </summary>
      property uint32                         FrameWidth;
      /// <summary>
      /// Gets or sets the height value for video. The property only makes
      /// sense for video media streams and represents the height of the video
      /// frame for this SSRC.
      /// </summary>
      property uint32                         FrameHeight;
      /// <summary>
      /// Gets or sets the video frames per second. Only valid for video.
      /// </summary>
      property float64                        FramesPerSecond;
      /// <summary>
      /// Gets or sets the total number of frames sent for this SSRC. Only
      /// valid for video. 
      /// </summary>
      property uint32                         FramesSent;
      /// <summary>
      /// Gets or sets the total number of frames received for this SSRC. Only
      /// valid for video and when RemoteSource is set to true.
      /// </summary>
      property uint32                         FramesReceived;
      /// <summary>
      /// Gets or sets the total number of frames correctly decoded for this
      /// SSRC. Only valid for video. Same definition as totalVideoFrames in
      /// Section 5 of [http://www.w3.org/TR/media-source/].
      /// </summary>
      property uint32                         FramesDecoded;
      /// <summary>
      /// Gets or sets the total number of frames dropped. Only valid for
      /// video. Same definition as droppedVideoFrames in Section 5 of
      /// [http://www.w3.org/TR/media-source/].
      /// </summary>
      property uint32                         FramesDropped;
      /// <summary>
      /// Gets or sets the total number of frames determined to be corrupted.
      /// Only valid for video. Same definition as corruptedVideoFrames in
      /// Section 5 of [http://www.w3.org/TR/media-source/].
      /// </summary>
      property uint32                         FramesCorrupted;
      /// <summary>
      /// Gets or sets the audio level. Only valid for audio, and the value is
      /// between 0..1 (linear), where 1.0 represents 0 dBov. Calculated as
      /// defined in [RFC6464].
      /// </summary>
      property float64                        AudioLevel;
      /// <summary>
      /// Gets or sets the echo return loss calculated in decibels, as defined
      /// in [https://www.itu.int/rec/T-REC-G.168/en] (2012) section 3.14.
      /// Only present on audio tracks sourced from a microphone where echo
      /// cancellation is applied. 
      /// </summary>
      property float64                        EchoReturnLoss;
      /// <summary>
      /// Gets or sets the echo return loss enhancement as calculated in
      /// decibels, as defined in [https://www.itu.int/rec/T-REC-G.168/en]
      /// (2012) section 3.15.Only present on audio tracks sourced from a
      /// microphone where echo cancellation is applied. 
      /// </summary>
      property float64                        EchoReturnLossEnhancement;
    };

    /// <summary>
    /// The RTCDataChannelStats object contains the statistics related to the
    /// associated RTCDataChannel object.
    /// </summary>
    public ref struct RTCDataChannelStats sealed
    {
      /// <summary>
      /// Gets or sets the data channle label.
      /// </summary>
      property Platform::String^              Label;
      /// <summary>
      /// Gets or sets the data channel protocol.
      /// </summary>
      property Platform::String^              Protocol;
      /// <summary>
      /// Gets or sets the "id" of the RTCDataChannel object.
      /// </summary>
      property int32                          DataChannelId;
      /// <summary>
      /// Gets or sets the RTCDataChannelState for the RTCDataChannel object.
      /// </summary>
      property RTCDataChannelState            State;
      /// <summary>
      /// Gets or sets the total number of API "message" events sent.
      /// </summary>
      property uint32                         MessagesSent;
      /// <summary>
      /// Gets or sets the total number of payload bytes sent on this
      /// RTCDatachannel, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// Gets or sets the total number of bytes received on this
      /// RTCDatachannel, i.e., not including headers or padding.
      /// </summary>
      property uint32                         MessagesReceived;
      /// <summary>
      /// Gets or sets the total number of bytes received on this
      /// RTCDatachannel, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesReceived;
    };


    /// <summary>
    /// The RTCIceGathererStats object contains the stastics information
    /// related to the RTCIceGatherer object.
    /// </summary>
    public ref struct RTCIceGathererStats sealed
    {
      /// <summary>
      /// Gets or sets the total number of bytes received on this
      /// RTCIceGatherer, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// Gets or sets the total number of payload bytes sent on this
      /// RTCIceGatherer, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// Gets or sets the RTCIceGathererStats identifier for the associated
      /// RTCP RTCIceGatherer (if applicable).
      /// </summary>
      property Platform::String^              RtcpGathererStatsId;
    };

    /// <summary>
    /// The RTCIceTransportStats object contains the statistics related to the
    /// RTCIceTransport object.
    /// </summary>
    public ref struct RTCIceTransportStats sealed
    {
      /// <summary>
      /// Gets or sets the total number of bytes received on this
      /// RTCIceTransport, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// Gets or sets the total number of payload bytes sent on this
      /// RTCIceTransport, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// Gets or sets the RTCIceGathererStats identifier for the associated
      /// RTCP RTCIceTransport (if applicable).
      /// </summary>
      property Platform::String^              RtcpTransportStatsId;
      /// <summary>
      /// Gets or sets if the connection is active. Set to true when transport
      /// is active.
      /// </summary>
      property Platform::Boolean              ActiveConnection;
      /// <summary>
      /// Gets or sets a unique identifier that is associated to the object
      /// that was inspected to produce the RTCIceCandidatePairStats
      /// associated with this transport.
      /// </summary>
      property Platform::String^              SelectedCandidatePairId;
    };

    /// <summary>
    /// The RTCDtlsTransportStats object contains the statistics related to the
    /// RTCDtlsTransport object.
    /// </summary>
    public ref struct RTCDtlsTransportStats sealed
    {
      /// <summary>
      /// Gets or sets the local certificate stats object identifier.
      /// </summary>
      property Platform::String^              LocalCertificateId;
      /// <summary>
      /// Gets or sets the remote certificate stats object identifier.
      /// </summary>
      property Platform::String^              RemoteCertificateId;
    };

    /// <summary>
    /// The RTCSrtpTransportStats object contains the statistics related to the
    /// RTCSrtpSdesTransport object.
    /// </summary>
    public ref struct RTCSrtpTransportStats sealed
    {
    };

    /// <summary>
    /// RTCIceCandidateAttributes reflects the properties of a candidate in
    /// Section 15.1 of [RFC5245].
    /// </summary>
    public ref struct RTCIceCandidateAttributes sealed
    {
      /// <summary>
      /// Gets or sets a unique identifier that is associated to the object
      /// that was inspected to produce the RTCTransportStats associated with
      /// this candidate.
      /// </summary>
      property Platform::String^              RelatedId;
      /// <summary>
      /// Gets or sets the IP address of the candidate, allowing for IPv4
      /// addresses, IPv6 addresses, and fully qualified domain names
      /// (FQDNs). See [RFC5245] section 15.1 for details.
      /// </summary>
      property Platform::String^              IpAddress;
      /// <summary>
      /// Gets or sets the port number of the candidate.
      /// </summary>
      property uint32                         PortNumber;
      /// <summary>
      /// Gets or sets the transport type. Valid values for transport is one o
      /// udp and tcp. Based on the "transport" defined in [RFC5245] section
      /// 15.1.
      /// </summary>
      property Platform::String^              Transport;
      /// <summary>
      /// Gets or sets the enumeration RTCStatsIceCandidateType based on the
      /// cand-type defined in [RFC5245] section 15.1.
      /// </summary>
      property RTCIceCandidateType            CandidateType;
      /// <summary>
      /// Gets or sets the candidate priority as calculated as defined in 
      /// [RFC5245] section 15.1.
      /// </summary>
      property uint32                         Priority;
      /// <summary>
      /// Gets or sets the URL of the TURN or STUN server indicated in the
      /// RTCIceServers that translated this IP address.
      /// </summary>
      property Platform::String^              AddressSourceUrl;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCIceCandidatePairStats sealed
    {
      /// <summary>
      /// Gets or sets a unique identifier that is associated to the object
      /// that was inspected to produce the RTCTransportStats associated with
      /// this candidate pair.
      /// </summary>
      property Platform::String^              TransportId;
      /// <summary>
      /// Gets or sets a unique identifier that is associated to the object
      /// that was inspected to produce the RTCIceCandidateAttributes for the
      /// local candidate associated with this candidate pair.
      /// </summary>
      property Platform::String^              LocalCandidateId;
      /// <summary>
      /// Gets or sets a unique identifier that is associated to the object
      /// that was inspected to produce the RTCIceCandidateAttributes for the
      /// remote candidate associated with this candidate pair.
      /// </summary>
      property Platform::String^              RemoteCandidateId;
      /// <summary>
      /// Gets or sets the state of the checklist for the local and remote
      /// candidates in a pair.
      /// </summary>
      property RTCStatsIceCandidatePairState  State;
      /// <summary>
      /// Gets or sets the calculated candidate pair priority as as defined in
      /// [RFC5245] section 5.7.2.
      /// </summary>
      property uint64                         Priority;
      /// <summary>
      /// Gets or sets the nominated flag described in Section 7.1.3.2.4 of [RFC5245].
      /// </summary>
      property Platform::Boolean              Nominated;
      /// <summary>
      /// Gets or sets if the candidate pair has received an ACK to an ICE
      /// request.
      /// </summary>
      property Platform::Boolean              Writable;
      /// <summary>
      /// Gets or sets if the candidate pair has received a valid incoming
      /// ICE request.
      /// </summary>
      property Platform::Boolean              Readable;
      /// <summary>
      /// Gets or sets the total number of payload bytes sent on this
      /// candidate pair, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// Gets or sets the total number of payload bytes received on this
      /// candidate pair, i.e., not including headers or padding.
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// Gets or sets the RTT computed by the STUN connectivity checks
      /// [draft-reddy-tram-stun-path-data].
      /// </summary>
      property float64                        RoundTripTime;
      /// <summary>
      /// Gets or sets the available outgoing bitrate measured in Bits per
      /// second, and is implementation dependent. It may be calculated by the
      /// underlying congestion control.
      /// </summary>
      property float64                        AvailableOutgoingBitrate;
      /// <summary>
      /// Gets or sets the available incoming bitrate measured in Bits per
      /// second, and is implementation dependent. It may be calculated by the
      /// underlying congestion control.
      /// </summary>
      property float64                        AvailableIncomingBitrate;
    };

    /// <summary>
    /// The RTCCertificateStats contains the statistics information related
    /// to an RTCCertificate object.
    /// </summary>
    public ref struct RTCCertificateStats sealed
    {
      /// <summary>
      /// Gets or sets the the fingerprint value as defined in Section 5 of
      /// [RFC4572].
      /// </summary>
      property Platform::String^              Fingerprint;
      /// <summary>
      /// Gets or sets the fingerprint algorithm used with this certificate.
      /// For instance, "sha-256".
      /// </summary>
      property Platform::String^              FingerprintAlgorithm;
      /// <summary>
      /// Gets or sets the DER-encoded, base-64 representation of a
      /// certifiate.
      /// </summary>
      property Platform::String^              Base64Certificate;
      /// <summary>
      /// Gets or sets the issuer certificate identifier associates with this
      /// RTCCertificateStats object.
      /// </summary>
      property Platform::String^              IssuerCertificateId;
    };


    /// <summary>
    /// The RTCStatsReport contains a collection of statistic object for
    /// statistics for the inspected objects.
    /// </summary>
    public ref class RTCStatsReport sealed
    {
      friend class internal::RTCStateProviderObserver;
      friend ref class RTCStatsProvider;

    private:
      RTCStatsReport(IStatsReportPtr nativePointer);

      static RTCStatsReport^ Convert(IStatsReportPtr nativePointer) { return ref new RTCStatsReport(nativePointer); }
      static IStatsReportPtr Convert(RTCStatsReport^ channel) { if (!channel) return nullptr; return channel->_nativePointer; }

    public:
      /// <summary>
      /// Gets the list of statistics object identifiers contained within this
      /// stats report.
      /// </summary>
      property IVector<Platform::String^>^ StatsIds
      {
        IVector<Platform::String^>^ get();
      }

      /// <summary>
      /// Gets an RTCStats object for the specified statistic object
      /// identifier.
      /// </summary>
      RTCStats^ GetStats(Platform::String^ statsId);

    private:
      IStatsReportPtr _nativePointer;
    };

  } // namespace ortc
} // namespace org
