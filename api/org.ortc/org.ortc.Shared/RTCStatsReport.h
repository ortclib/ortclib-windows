#pragma once

#include <ortc/IStatsReport.h>

#include "RTCDtlsTransport.h"
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

    enum class RTCDtlsTransportState;
    enum class RTCIceCandidateType;

    namespace internal
    {
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
    /// </summary>
    public enum class RTCStatsType
    {
      /// <summary>
      /// </summary>
      Unknown,
      /// <summary>
      /// </summary>
      InboundRtp,
      /// <summary>
      /// </summary>
      OutboundRtp,
      /// <summary>
      /// </summary>
      Codec,
      /// <summary>
      /// </summary>
      SctpTransport,
      /// <summary>
      /// </summary>
      DataChannel,
      /// <summary>
      /// </summary>
      Stream,
      /// <summary>
      /// </summary>
      Track,
      /// <summary>
      /// </summary>
      IceGatherer,
      /// <summary>
      /// </summary>
      IceTransport,
      /// <summary>
      /// </summary>
      DtlsTransport,
      /// <summary>
      /// </summary>
      SrtpTransport,
      /// <summary>
      /// </summary>
      Certificate,
      /// <summary>
      /// </summary>
      Candidate,
      /// <summary>
      /// </summary>
      CandidatePair,
      /// <summary>
      /// </summary>
      LocalCandidate,
      /// <summary>
      /// </summary>
      RemoteCandidate
    };

    /// <summary>
    /// </summary>
    public enum class RTCStatsIceCandidatePairState
    {
      /// <summary>
      /// </summary>
      Frozen,
      /// <summary>
      /// </summary>
      Waiting,
      /// <summary>
      /// </summary>
      InProgress,
      /// <summary>
      /// </summary>
      Failed,
      /// <summary>
      /// </summary>
      Succeeded,
      /// <summary>
      /// </summary>
      Cancelled
    };

    /// <summary>
    /// </summary>
    public ref struct RTCStats sealed
    {
      friend class internal::RTCStateProviderObserver;

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
      property IVector<Platform::String^>^    TrackId;
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
      property RTCDtlsTransportState          State;
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
      property Platform::String^              Transport;
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
      property Platform::String^              LocalCandidateID;
      /// <summary>
      /// </summary>
      property Platform::String^              RemoteCandidateID;
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

  } // namespace ortc
} // namespace org
