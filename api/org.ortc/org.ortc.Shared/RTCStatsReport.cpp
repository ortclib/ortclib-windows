#include "pch.h"

#include "RTCStatsReport.h"
#include "helpers.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/SafeInt.h>

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

#pragma region RTCStateReportHelper

      class RTCStateReportHelper
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
        RTCStateReportHelper::setOriginal(result, input);
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

        result->Ssrc = UseHelper::ToCx(input.mSSRC);
        result->AssociatedStatId = UseHelper::ToCx(input.mAssociatedStatID);
        result->IsRemote = input.mIsRemote;
        result->MediaType = UseHelper::ToCx(input.mMediaType);
        result->MediaTrackId = UseHelper::ToCx(input.mMediaTrackID);
        result->TransportId = UseHelper::ToCx(input.mTransportID);
        result->CodecId = UseHelper::ToCx(input.mCodecID);
        result->FirCount = SafeInt<decltype(result->FirCount)>(input.mFIRCount);
        result->PliCount = SafeInt<decltype(result->PliCount)>(input.mPLICount);
        result->NackCount = SafeInt<decltype(result->FirCount)>(input.mNACKCount);
        result->SliCount = SafeInt<decltype(result->FirCount)>(input.mSLICount);

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

        result->PayloadType = UseHelper::ToCx(input.mPayloadType);
        result->Codec = UseHelper::ToCx(input.mCodec);
        result->ClockRate = SafeInt<decltype(result->ClockRate)>(input.mClockRate);
        result->Channels = UseHelper::ToCx(input.mChannels);
        result->Parameters = UseHelper::ToCx(input.mParameters);

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

        result->RtpStreamStats = ToCx((const IStatsReportTypes::RTPStreamStats &)input);
        result->PacketsReceived = SafeInt<decltype(result->PacketsReceived)>(input.mPacketsReceived);
        result->BytesReceived = SafeInt<decltype(result->BytesReceived)>(input.mBytesReceived);
        result->PacketsLost = SafeInt<decltype(result->PacketsLost)>(input.mPacketsLost);
        result->Jitter = input.mJitter;
        result->FractionLost = input.mFractionLost;
        result->EndToEndDelay = SafeInt<decltype(result->EndToEndDelay)>(input.mEndToEndDelay.count());

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

        result->RtpStreamStats = ToCx((const IStatsReportTypes::RTPStreamStats &)input);
        result->PacketsSent = SafeInt<decltype(result->PacketsSent)>(input.mPacketsSent);
        result->BytesSent = SafeInt<decltype(result->BytesSent)>(input.mBytesSent);
        result->TargetBitrate = input.mTargetBitrate;
        result->RoundTripTime = input.mRoundTripTime;

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

        result->DataChannelsOpened = SafeInt<decltype(result->DataChannelsOpened)>(input.mDataChannelsOpened);
        result->DataChannelsClosed = SafeInt<decltype(result->DataChannelsClosed)>(input.mDataChannelsClosed);

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

        result->StreamId = UseHelper::ToCx(input.mStreamID);
        result->TrackIds = ref new Vector<Platform::String^>();

        result->StreamId = UseHelper::ToCx(input.mStreamID);
        for (auto iter = input.mTrackIDs.begin(); iter != input.mTrackIDs.end(); ++iter)
        {
          auto &trackId = (*iter);
          result->TrackIds->Append(UseHelper::ToCx(trackId));
        }

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

        result->TrackId = UseHelper::ToCx(input.mTrackID);
        result->RemoteSource = input.mRemoteSource;
        result->SsrcIds = ref new Vector<uint32>();

        for (auto iter = input.mSSRCIDs.begin(); iter != input.mSSRCIDs.end(); ++iter)
        {
          auto &ssrc = (*iter);
          result->SsrcIds->Append(SafeInt<uint32>(ssrc));
        }

        result->FrameWidth = SafeInt<decltype(result->FrameWidth)>(input.mFrameWidth);
        result->FrameHeight = SafeInt<decltype(result->FrameHeight)>(input.mFrameHeight);
        result->FramesPerSecond = input.mFramesPerSecond;
        result->FramesSent = SafeInt<decltype(result->FramesSent)>(input.mFramesSent);
        result->FramesReceived = SafeInt<decltype(result->FramesReceived)>(input.mFramesReceived);
        result->FramesDecoded = SafeInt<decltype(result->FramesDecoded)>(input.mFramesDecoded);
        result->FramesDropped = SafeInt<decltype(result->FramesDropped)>(input.mFramesDropped);
        result->FramesCorrupted = SafeInt<decltype(result->FramesCorrupted)>(input.mFramesCorrupted);
        result->AudioLevel = input.mAudioLevel;
        result->EchoReturnLoss = input.mEchoReturnLoss;
        result->EchoReturnLossEnhancement = input.mEchoReturnLossEnhancement;

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

        result->Label = UseHelper::ToCx(input.mLabel);
        result->Protocol = UseHelper::ToCx(input.mProtocol);
        result->DataChannelId = SafeInt<decltype(result->DataChannelId)>(input.mDataChannelID);
        result->State = UseHelper::Convert(input.mState);
        result->MessagesSent = SafeInt<decltype(result->MessagesSent)>(input.mMessagesSent);
        result->BytesSent = SafeInt<decltype(result->BytesSent)>(input.mBytesSent);
        result->MessagesReceived = SafeInt<decltype(result->MessagesReceived)>(input.mMessagesReceived);
        result->BytesReceived = SafeInt<decltype(result->BytesReceived)>(input.mBytesReceived);

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

        result->BytesSent = SafeInt<decltype(result->BytesSent)>(input.mBytesSent);
        result->BytesReceived = SafeInt<decltype(result->BytesReceived)>(input.mBytesReceived);
        result->RtcpGathererStatsId = UseHelper::ToCx(input.mRTCPGathererStatsID);

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

        result->BytesSent = SafeInt<decltype(result->BytesSent)>(input.mBytesSent);
        result->BytesReceived = SafeInt<decltype(result->BytesReceived)>(input.mBytesReceived);
        result->RtcpTransportStatsId = UseHelper::ToCx(input.mRTCPTransportStatsID);
        result->ActiveConnection = input.mActiveConnection;
        result->SelectedCandidatePairId = UseHelper::ToCx(input.mSelectedCandidatePairID);

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

        result->LocalCertificateId = UseHelper::ToCx(input.mLocalCertificateID);
        result->RemoteCertificateId = UseHelper::ToCx(input.mRemoteCertificateID);

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

        result->RelatedId = UseHelper::ToCx(input.mRelatedID);
        result->IpAddress = UseHelper::ToCx(input.mIPAddress);
        result->PortNumber = SafeInt<decltype(result->PortNumber)>(input.mPortNumber);
        result->Transport = UseHelper::ToCx(input.mTransport);
        result->CandidateType = UseHelper::Convert(input.mCandidateType);
        result->Priority = SafeInt<decltype(result->Priority)>(input.mPriority);
        result->AddressSourceUrl = UseHelper::ToCx(input.mAddressSourceURL);

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

        result->TransportId = UseHelper::ToCx(input.mTransportID);
        result->LocalCandidateId = UseHelper::ToCx(input.mLocalCandidateID);
        result->RemoteCandidateId = UseHelper::ToCx(input.mRemoteCandidateID);
        result->State = UseHelper::Convert(input.mState);
        result->Priority = SafeInt<decltype(result->Priority)>(input.mPriority);
        result->Nominated = input.mNominated;
        result->Writable = input.mWritable;
        result->Readable = input.mReadable;
        result->BytesSent = SafeInt<decltype(result->BytesSent)>(input.mBytesSent);
        result->BytesReceived = SafeInt<decltype(result->BytesReceived)>(input.mBytesReceived);
        result->RoundTripTime = input.mRoundTripTime;
        result->AvailableOutgoingBitrate = input.mAvailableOutgoingBitrate;
        result->AvailableIncomingBitrate = input.mAvailableIncomingBitrate;

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

        result->Fingerprint = UseHelper::ToCx(input.mFingerprint);
        result->FingerprintAlgorithm = UseHelper::ToCx(input.mFingerprintAlgorithm);
        result->Base64Certificate = UseHelper::ToCx(input.mBase64Certificate);
        result->IssuerCertificateId = UseHelper::ToCx(input.mIssuerCertificateID);

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

    RTCStatsReport::RTCStatsReport(IStatsReportPtr nativePointer) :
      _nativePointer(nativePointer)
    {
    }

    IVector<Platform::String^>^ RTCStatsReport::StatsIds::get()
    {
      auto result = ref new Vector<Platform::String^>();

      if (_nativePointer)
      {
        auto ids = _nativePointer->getStatesIDs();
        if (ids)
        {
          for (auto iter = ids->begin(); iter != ids->end(); ++iter)
          {
            result->Append(UseHelper::ToCx(*iter));
          }
        }
      }

      return result;
    }

    RTCStats^ RTCStatsReport::GetStats(Platform::String^ statsId)
    {
      if (!_nativePointer) return nullptr;

      return internal::ToCx(_nativePointer->getStats(UseHelper::FromCx(statsId).c_str()));
    }

  } // namespace ortc
} // namespace org
