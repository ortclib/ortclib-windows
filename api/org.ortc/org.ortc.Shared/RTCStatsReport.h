#pragma once

#include <ortc/IStatsReport.h>

#include "RTCDataChannel.h"
#include "RTCIceTypes.h"

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
      /// Statistics for the RTCCandidate object. It is accessed by the
      /// RTCIceCandidatePairStats.
      /// </summary>
      CandidatePair,
      /// <summary>
      /// Statistics for the RTCIceGatherer or RTCIceTransport object. It is
      /// accessed by the RTCIceCandidateAttributes.
      /// </summary>
      LocalCandidate,
      /// <summary>
      /// Statistics for the RTCIceTransport object. It is accessed by the
      /// RTCIceCandidateAttributes.
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
    /// </summary>
    public ref struct RTCStats sealed
    {
      friend class internal::RTCStateReportHelper;

      /// <summary>
      /// </summary>
      property Windows::Foundation::DateTime  Timestamp;
      /// <summary>
      /// </summary>
      property RTCStatsType                   StatsType;
      /// <summary>
      /// </summary>
      property Platform::String^              Id;

      /// <summary>
      /// </summary>
      RTCInboundRtpStreamStats^               ToInboundRtp();
      /// <summary>
      /// </summary>
      RTCOutboundRtpStreamStats^              ToOutboundRtp();
      /// <summary>
      /// </summary>
      RTCCodec^                               ToCodec();
      /// <summary>
      /// </summary>
      RTCSctpTransportStats^                  ToSctpTransport();
      /// <summary>
      /// </summary>
      RTCDataChannelStats^                    ToDataChannel();
      /// <summary>
      /// </summary>
      RTCMediaStreamStats^                    ToStream();
      /// <summary>
      /// </summary>
      RTCMediaStreamTrackStats^               ToTrack();
      /// <summary>
      /// </summary>
      RTCIceGathererStats^                    ToIceGatherer();
      /// <summary>
      /// </summary>
      RTCIceTransportStats^                   ToIceTransport();
      /// <summary>
      /// </summary>
      RTCDtlsTransportStats^                  ToDtlsTransport();
      /// <summary>
      /// </summary>
      RTCSrtpTransportStats^                  ToSrtpTransport();
      /// <summary>
      /// </summary>
      RTCCertificateStats^                    ToCertificate();
      /// <summary>
      /// </summary>
      RTCIceCandidateAttributes^              ToCandidate();
      /// <summary>
      /// </summary>
      RTCIceCandidatePairStats^               ToCandidatePair();
      /// <summary>
      /// </summary>
      RTCIceCandidateAttributes^              ToLocalCandidate();
      /// <summary>
      /// </summary>
      RTCIceCandidateAttributes^              ToRemoteCandidate();

    private:
      IStatsReportTypes::StatsPtr mOriginal;
    };


    /// <summary>
    /// </summary>
    public ref struct RTCRTPStreamStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::IBox<uint32>^        Ssrc;
      /// <summary>
      /// </summary>
      property Platform::String^              AssociatedStatId;
      /// <summary>
      /// </summary>
      property Platform::Boolean              IsRemote;
      /// <summary>
      /// </summary>
      property Platform::String^              MediaType;
      /// <summary>
      /// </summary>
      property Platform::String^              MediaTrackId;
      /// <summary>
      /// </summary>
      property Platform::String^              TransportId;
      /// <summary>
      /// </summary>
      property Platform::String^              CodecId;
      /// <summary>
      /// </summary>
      property uint32                         FirCount;
      /// <summary>
      /// </summary>
      property uint32                         PliCount;
      /// <summary>
      /// </summary>
      property uint32                         NackCount;
      /// <summary>
      /// </summary>
      property uint32                         SliCount;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCCodec sealed
    {
      /// <summary>
      /// </summary>
      property Platform::IBox<uint8>^         PayloadType;
      /// <summary>
      /// </summary>
      property Platform::String^              Codec;
      /// <summary>
      /// </summary>
      property uint32                         ClockRate;
      /// <summary>
      /// </summary>
      property Platform::IBox<uint32>^        Channels;
      /// <summary>
      /// </summary>
      property Platform::String^              Parameters;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCInboundRtpStreamStats sealed
    {
      /// <summary>
      /// </summary>
      property RTCRTPStreamStats^             RtpStreamStats;
      /// <summary>
      /// </summary>
      property uint32                         PacketsReceived;
      /// <summary>
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// </summary>
      property uint32                         PacketsLost;
      /// <summary>
      /// </summary>
      property float64                        Jitter;
      /// <summary>
      /// </summary>
      property float64                        FractionLost;
    };


    /// <summary>
    /// </summary>
    public ref struct RTCOutboundRtpStreamStats sealed
    {
      /// <summary>
      /// </summary>
      property RTCRTPStreamStats^             RtpStreamStats;
      /// <summary>
      /// </summary>
      property uint32                         PacketsSent;
      /// <summary>
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// </summary>
      property float64                        TargetBitrate;
      /// <summary>
      /// </summary>
      property float64                        RoundTripTime;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCSctpTransportStats sealed
    {
      /// <summary>
      /// </summary>
      property uint32                         DataChannelsOpened;
      /// <summary>
      /// </summary>
      property uint32                         DataChannelsClosed;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCMediaStreamStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              StreamId;
      /// <summary>
      /// </summary>
      property IVector<Platform::String^>^    TrackIds;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCMediaStreamTrackStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              TrackId;
      /// <summary>
      /// </summary>
      property Platform::Boolean              RemoteSource;
      /// <summary>
      /// </summary>
      property IVector<uint32>^               SsrcIds;
      /// <summary>
      /// </summary>
      property uint32                         FrameWidth;
      /// <summary>
      /// </summary>
      property uint32                         FrameHeight;
      /// <summary>
      /// </summary>
      property float64                        FramesPerSecond;
      /// <summary>
      /// </summary>
      property uint32                         FramesSent;
      /// <summary>
      /// </summary>
      property uint32                         FramesReceived;
      /// <summary>
      /// </summary>
      property uint32                         FramesDecoded;
      /// <summary>
      /// </summary>
      property uint32                         FramesDropped;
      /// <summary>
      /// </summary>
      property uint32                         FramesCorrupted;
      /// <summary>
      /// </summary>
      property float64                        AudioLevel;
      /// <summary>
      /// </summary>
      property float64                        EchoReturnLoss;
      /// <summary>
      /// </summary>
      property float64                        EchoReturnLossEnhancement;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCDataChannelStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              Label;
      /// <summary>
      /// </summary>
      property Platform::String^              Protocol;
      /// <summary>
      /// </summary>
      property int32                          DatachannelId;
      /// <summary>
      /// </summary>
      property RTCDataChannelState            State;
      /// <summary>
      /// </summary>
      property uint32                         MessagesSent;
      /// <summary>
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// </summary>
      property uint32                         MessagesReceived;
      /// <summary>
      /// </summary>
      property uint64                         BytesReceived;
    };


    /// <summary>
    /// </summary>
    public ref struct RTCIceGathererStats sealed
    {
      /// <summary>
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// </summary>
      property Platform::String^              RtcpGathererStatsId;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCIceTransportStats sealed
    {
      /// <summary>
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// </summary>
      property Platform::String^              RtcpTransportStatsId;
      /// <summary>
      /// </summary>
      property Platform::Boolean              ActiveConnection;
      /// <summary>
      /// </summary>
      property Platform::String^              SelectedCandidatePairId;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCDtlsTransportStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              LocalCertificateId;
      /// <summary>
      /// </summary>
      property Platform::String^              RemoteCertificateId;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCSrtpTransportStats sealed
    {
    };

    /// <summary>
    /// </summary>
    public ref struct RTCIceCandidateAttributes sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              IpAddress;
      /// <summary>
      /// </summary>
      property uint32                         PortNumber;
      /// <summary>
      /// </summary>
      property Platform::String^              TransportId;
      /// <summary>
      /// </summary>
      property RTCIceCandidateType            CandidateType;
      /// <summary>
      /// </summary>
      property uint32                         Priority;
      /// <summary>
      /// </summary>
      property Platform::String^              AddressSourceUrl;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCIceCandidatePairStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              TransportId;
      /// <summary>
      /// </summary>
      property Platform::String^              LocalCandidateId;
      /// <summary>
      /// </summary>
      property Platform::String^              RemoteCandidateId;
      /// <summary>
      /// </summary>
      property RTCStatsIceCandidatePairState  State;
      /// <summary>
      /// </summary>
      property uint64                         Priority;
      /// <summary>
      /// </summary>
      property Platform::Boolean              Nominated;
      /// <summary>
      /// </summary>
      property Platform::Boolean              Writable;
      /// <summary>
      /// </summary>
      property Platform::Boolean              Readable;
      /// <summary>
      /// </summary>
      property uint64                         BytesSent;
      /// <summary>
      /// </summary>
      property uint64                         BytesReceived;
      /// <summary>
      /// </summary>
      property float64                        RoundTripTime;
      /// <summary>
      /// </summary>
      property float64                        AvailableOutgoingBitrate;
      /// <summary>
      /// </summary>
      property float64                        AvailableIncomingBitrate;
    };

    /// <summary>
    /// </summary>
    public ref struct RTCCertificateStats sealed
    {
      /// <summary>
      /// </summary>
      property Platform::String^              Fingerprint;
      /// <summary>
      /// </summary>
      property Platform::String^              FingerprintAlgorithm;
      /// <summary>
      /// </summary>
      property Platform::String^              Base64Certificate;
      /// <summary>
      /// </summary>
      property Platform::String^              IssuerCertificateId;
    };


    /// <summary>
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
      /// </summary>
      property IVector<Platform::String^>^ StatsIds
      {
        IVector<Platform::String^>^ get();
      }

      RTCStats^ GetStats(Platform::String^ statsId);

    private:
      IStatsReportPtr _nativePointer;
    };

  } // namespace ortc
} // namespace org
