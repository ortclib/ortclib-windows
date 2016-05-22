#include "pch.h"

#include "RTCStatsReport.h"
#include "helpers.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/SafeInt.h>

namespace org
{
  namespace ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

#pragma region RTCStateProviderObserver

      class RTCStateProviderObserver
      {
      public:
        static void setOriginal(
          RTCStats^ stats,
          IStatsReportTypes::StatsPtr original
          )
        {
          stats->mOriginal = original;
        }
      };

#pragma endregion


#pragma region Stats types conversions

      RTCStats^ ToCx(const IStatsReportTypes::Stats &input)
      {
        auto result = ref new RTCStats();

        result->Timestamp = Helper::ToCx(input.mTimestamp);
        result->StatsType = Helper::Convert(IStatsReportTypes::toStatsType(input.mStatsType));
        result->Id = Helper::ToCx(input.mID);

        return result;
      }

      RTCStats^ ToCx(IStatsReportTypes::StatsPtr input)
      {
        if (!input) return nullptr;
        auto result = ToCx(*input);
        RTCStateProviderObserver::setOriginal(result, input);
        return result;
      }

      IStatsReportTypes::StatsPtr FromCx(RTCStats^ input)
      {
        if (nullptr == input) return IStatsReportTypes::StatsPtr();
        auto result = make_shared<IStatsReportTypes::Stats>();

        result->mTimestamp = Helper::FromCx(input->Timestamp);
        result->mStatsType = IStatsReportTypes::toString(Helper::Convert(input->StatsType));
        result->mID = Helper::FromCx(input->Id);

        return result;
      }

      RTCRTPStreamStats^ ToCx(const IStatsReportTypes::RTPStreamStats &input)
      {
        auto result = ref new RTCRTPStreamStats();

        return result;
      }

      RTCRTPStreamStats^ ToCx(IStatsReportTypes::RTPStreamStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCCodec^ ToCx(const IStatsReportTypes::Codec &input)
      {
        auto result = ref new RTCCodec();

        return result;
      }

      RTCCodec^ ToCx(IStatsReportTypes::CodecPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCInboundRtpStreamStats^ ToCx(const IStatsReportTypes::InboundRTPStreamStats &input)
      {
        auto result = ref new RTCInboundRtpStreamStats();

        return result;
      }

      RTCInboundRtpStreamStats^ ToCx(IStatsReportTypes::InboundRTPStreamStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCOutboundRtpStreamStats^ ToCx(const IStatsReportTypes::OutboundRTPStreamStats &input)
      {
        auto result = ref new RTCOutboundRtpStreamStats();

        return result;
      }

      RTCOutboundRtpStreamStats^ ToCx(IStatsReportTypes::OutboundRTPStreamStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCSctpTransportStats^ ToCx(const IStatsReportTypes::SCTPTransportStats &input)
      {
        auto result = ref new RTCSctpTransportStats();

        return result;
      }

      RTCSctpTransportStats^ ToCx(IStatsReportTypes::SCTPTransportStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCMediaStreamStats^ ToCx(const IStatsReportTypes::MediaStreamStats &input)
      {
        auto result = ref new RTCMediaStreamStats();

        return result;
      }

      RTCMediaStreamStats^ ToCx(IStatsReportTypes::MediaStreamStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCMediaStreamTrackStats^ ToCx(const IStatsReportTypes::MediaStreamTrackStats &input)
      {
        auto result = ref new RTCMediaStreamTrackStats();

        return result;
      }

      RTCMediaStreamTrackStats^ ToCx(IStatsReportTypes::MediaStreamTrackStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCDataChannelStats^ ToCx(const IStatsReportTypes::DataChannelStats &input)
      {
        auto result = ref new RTCDataChannelStats();

        return result;
      }

      RTCDataChannelStats^ ToCx(IStatsReportTypes::DataChannelStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCIceGathererStats^ ToCx(const IStatsReportTypes::ICEGathererStats &input)
      {
        auto result = ref new RTCIceGathererStats();

        return result;
      }

      RTCIceGathererStats^ ToCx(IStatsReportTypes::ICEGathererStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCIceTransportStats^ ToCx(const IStatsReportTypes::ICETransportStats &input)
      {
        auto result = ref new RTCIceTransportStats();

        return result;
      }

      RTCIceTransportStats^ ToCx(IStatsReportTypes::ICETransportStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCDtlsTransportStats^ ToCx(const IStatsReportTypes::DTLSTransportStats &input)
      {
        auto result = ref new RTCDtlsTransportStats();

        return result;
      }

      RTCDtlsTransportStats^ ToCx(IStatsReportTypes::DTLSTransportStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCSrtpTransportStats^ ToCx(const IStatsReportTypes::SRTPTransportStats &input)
      {
        auto result = ref new RTCSrtpTransportStats();

        return result;
      }

      RTCSrtpTransportStats^ ToCx(IStatsReportTypes::SRTPTransportStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCIceCandidateAttributes^ ToCx(const IStatsReportTypes::ICECandidateAttributes &input)
      {
        auto result = ref new RTCIceCandidateAttributes();

        return result;
      }

      RTCIceCandidateAttributes^ ToCx(IStatsReportTypes::ICECandidateAttributesPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCIceCandidatePairStats^ ToCx(const IStatsReportTypes::ICECandidatePairStats &input)
      {
        auto result = ref new RTCIceCandidatePairStats();

        return result;
      }

      RTCIceCandidatePairStats^ ToCx(IStatsReportTypes::ICECandidatePairStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      RTCCertificateStats^ ToCx(const IStatsReportTypes::CertificateStats &input)
      {
        auto result = ref new RTCCertificateStats();

        return result;
      }

      RTCCertificateStats^ ToCx(IStatsReportTypes::CertificateStatsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

#pragma endregion

    } // namespace internal

    RTCInboundRtpStreamStats^ RTCStats::ToInboundRtp()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::InboundRTPStreamStats, mOriginal));
    }

    RTCOutboundRtpStreamStats^ RTCStats::ToOutboundRtp()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::OutboundRTPStreamStats, mOriginal));
    }

    RTCCodec^ RTCStats::ToCodec()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::Codec, mOriginal));
    }

    RTCSctpTransportStats^ RTCStats::ToSctpTransport()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::SCTPTransportStats, mOriginal));
    }

    RTCDataChannelStats^ RTCStats::ToDataChannel()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::DataChannelStats, mOriginal));
    }

    RTCMediaStreamStats^ RTCStats::ToStream()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::MediaStreamStats, mOriginal));
    }

    RTCMediaStreamTrackStats^ RTCStats::ToTrack()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::MediaStreamTrackStats, mOriginal));
    }

    RTCIceGathererStats^ RTCStats::ToIceGatherer()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::ICEGathererStats, mOriginal));
    }

    RTCIceTransportStats^ RTCStats::ToIceTransport()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::ICETransportStats, mOriginal));
    }

    RTCDtlsTransportStats^ RTCStats::ToDtlsTransport()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::DTLSTransportStats, mOriginal));
    }

    RTCSrtpTransportStats^ RTCStats::ToSrtpTransport()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::SRTPTransportStats, mOriginal));
    }

    RTCCertificateStats^ RTCStats::ToCertificate()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::CertificateStats, mOriginal));
    }

    RTCIceCandidateAttributes^ RTCStats::ToCandidate()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::ICECandidateAttributes, mOriginal));
    }

    RTCIceCandidatePairStats^ RTCStats::ToCandidatePair()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::ICECandidatePairStats, mOriginal));
    }

    RTCIceCandidateAttributes^ RTCStats::ToLocalCandidate()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::ICECandidateAttributes, mOriginal));
    }

    RTCIceCandidateAttributes^ RTCStats::ToRemoteCandidate()
    {
      if (!mOriginal) return nullptr;
      return internal::ToCx(ZS_DYNAMIC_PTR_CAST(IStatsReportTypes::ICECandidateAttributes, mOriginal));
    }

  } // namespace ortc
} // namespace org
