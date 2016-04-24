
#include "pch.h"

#include "RtpTypes.h"
#include "helpers.h"
#include "Error.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/SafeInt.h>

using namespace ortc;

using namespace Platform;
using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    using std::make_shared;
    using zsLib::Milliseconds;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

      RTCRtcpFeedback^ ToCx(const IRTPTypes::RTCPFeedback &input)
      {
        auto result = ref new RTCRtcpFeedback();
        result->Type = Helper::ToCx(input.mType);
        result->Parameter = Helper::ToCx(input.mParameter);
        return result;
      }

      RTCRtcpFeedback^ ToCx(IRTPTypes::RTCPFeedbackPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::RTCPFeedback FromCx(RTCRtcpFeedback^ input)
      {
        IRTPTypes::RTCPFeedback result;
        if (nullptr == input) return result;
        result.mParameter = Helper::FromCx(input->Parameter);
        result.mType = Helper::FromCx(input->Type);
        return result;
      }

      RTCRtpOpusCodecCapabilityOptions^ ToCx(const IRTPTypes::OpusCodecCapabilityOptions &input)
      {
        auto result = ref new RTCRtpOpusCodecCapabilityOptions();
        result->Complexity = input.mComplexity;
        result->Signal = UseHelper::ToCx(IRTPTypes::OpusCodecCapabilityOptions::toString(input.mSignal));
        result->Application = UseHelper::ToCx(IRTPTypes::OpusCodecCapabilityOptions::toString(input.mApplication));
        result->PacketLossPerc = input.mPacketLossPerc;
        result->PredictionDisabled = input.mPredictionDisabled;
        return result;
      }

      RTCRtpOpusCodecCapabilityOptions^ ToCx(IRTPTypes::OpusCodecCapabilityOptionsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::OpusCodecCapabilityOptionsPtr FromCx(RTCRtpOpusCodecCapabilityOptions^ input)
      {
        if (nullptr == input) return IRTPTypes::OpusCodecCapabilityOptionsPtr();

        auto result = make_shared<IRTPTypes::OpusCodecCapabilityOptions>();

        try
        {
          result->mComplexity = input->Complexity;
          if (!UseHelper::IsNullOrEmpty(input->Signal))
          {
            result->mSignal = IRTPTypes::OpusCodecCapabilityOptions::toSignal(UseHelper::FromCx(input->Signal).c_str());
          }
          if (!UseHelper::IsNullOrEmpty(input->Application))
          {
            result->mApplication = IRTPTypes::OpusCodecCapabilityOptions::toApplication(UseHelper::FromCx(input->Application).c_str());
          }
          result->mPacketLossPerc = input->PacketLossPerc;
          result->mPredictionDisabled = input->PredictionDisabled;
        }
        catch (const InvalidParameters &)
        {
          ORG_ORTC_THROW_INVALID_PARAMETERS()
        }
        return result;
      }

      RTCRtpOpusCodecCapabilityParameters^ ToCx(const IRTPTypes::OpusCodecCapabilityParameters &input)
      {
        auto result = ref new RTCRtpOpusCodecCapabilityParameters();
        result->MaxPlaybackRate = Helper::ToCx(input.mMaxPlaybackRate);
        result->MaxAverageBitrate = Helper::ToCx(input.mMaxAverageBitrate);
        result->Stereo = Helper::ToCx(input.mStereo);
        result->Cbr = Helper::ToCx(input.mCBR);
        result->UseInbandFec = Helper::ToCx(input.mUseInbandFEC);
        result->UseDtx = Helper::ToCx(input.mUseDTX);
        result->SpropMaxCaptureRate = Helper::ToCx(input.mSPropMaxCaptureRate);
        result->SpropStereo = Helper::ToCx(input.mSPropStereo);
        return result;
      }

      RTCRtpOpusCodecCapabilityParameters^ ToCx(IRTPTypes::OpusCodecCapabilityParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::OpusCodecCapabilityParametersPtr FromCx(RTCRtpOpusCodecCapabilityParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::OpusCodecCapabilityParametersPtr();
        auto result = make_shared<IRTPTypes::OpusCodecCapabilityParameters>();
        result->mMaxPlaybackRate = Helper::FromCx(input->MaxPlaybackRate);
        result->mMaxAverageBitrate = Helper::FromCx(input->MaxAverageBitrate);
        result->mStereo = Helper::FromCx(input->Stereo);
        result->mCBR = Helper::FromCx(input->Cbr);
        result->mUseInbandFEC = Helper::FromCx(input->UseInbandFec);
        result->mUseDTX = Helper::FromCx(input->UseDtx);
        result->mSPropMaxCaptureRate = Helper::FromCx(input->SpropMaxCaptureRate);
        result->mSPropStereo = Helper::FromCx(input->SpropStereo);
        return result;
      }

      RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::VP8CodecCapabilityParameters &input)
      {
        auto result = ref new RTCRtpVp8CodecCapabilityParameters();
        result->MaxFr = Helper::ToCx(input.mMaxFR);
        result->MaxFs = Helper::ToCx(input.mMaxFS);
        return result;
      }

      RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::VP8CodecCapabilityParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCxCapabilityParameters(*input);
      }

      IRTPTypes::VP8CodecCapabilityParametersPtr FromCx(RTCRtpVp8CodecCapabilityParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::VP8CodecCapabilityParametersPtr();
        auto result = make_shared<IRTPTypes::VP8CodecCapabilityParameters>();
        result->mMaxFR = Helper::FromCx(input->MaxFr);
        result->mMaxFS = Helper::FromCx(input->MaxFs);
        return result;
      }

      RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::H264CodecCapabilityParameters &input)
      {
        auto result = ref new RTCRtpH264CodecCapabilityParameters();
        result->ProfileLevelId = Helper::ToCx(input.mProfileLevelID);

        if (input.mPacketizationModes.size() > 0)
        {
          result->PacketizationModes = ref new Vector<uint16>();
          for (auto iter = input.mPacketizationModes.begin(); iter != input.mPacketizationModes.end(); ++iter)
          {
            auto &value = (*iter);
            result->PacketizationModes->Append(value);
          }
        }

        result->MaxMbps = Helper::ToCx(input.mMaxMBPS);
        result->MaxSmbps = Helper::ToCx(input.mMaxSMBPS);
        result->MaxFs = Helper::ToCx(input.mMaxFS);
        result->MaxCpb = Helper::ToCx(input.mMaxCPB);
        result->MaxDpb = Helper::ToCx(input.mMaxDPB);
        result->MaxBr = Helper::ToCx(input.mMaxBR);
        return result;
      }

      RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::H264CodecCapabilityParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCxCapabilityParameters(*input);
      }

      IRTPTypes::H264CodecCapabilityParametersPtr FromCx(RTCRtpH264CodecCapabilityParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::H264CodecCapabilityParametersPtr();
        auto result = make_shared<IRTPTypes::H264CodecCapabilityParameters>();
        result->mProfileLevelID = Helper::FromCx(input->ProfileLevelId);
        if (input->PacketizationModes)
        {
          for (uint16 value : input->PacketizationModes)
          {
            result->mPacketizationModes.push_back(value);
          }
        }
        result->mMaxMBPS = Helper::FromCx(input->MaxMbps);
        result->mMaxSMBPS = Helper::FromCx(input->MaxSmbps);
        result->mMaxFS = Helper::FromCx(input->MaxFs);
        result->mMaxCPB = Helper::FromCx(input->MaxCpb);
        result->mMaxDPB = Helper::FromCx(input->MaxDpb);
        result->mMaxBR = Helper::FromCx(input->MaxBr);
        return result;
      }

      RTCRtpRtxCodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::RTXCodecCapabilityParameters &input)
      {
        auto result = ref new RTCRtpRtxCodecCapabilityParameters();
        if (zsLib::Milliseconds() != input.mRTXTime)
        {
          result->RtxTime = SafeInt<uint32>(input.mRTXTime.count());
        }
        else
        {
          result->RtxTime = 0;
        }
        result->Apt = SafeInt<uint8>(input.mApt);
        return result;
      }

      RTCRtpRtxCodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::RTXCodecCapabilityParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCxCapabilityParameters(*input);
      }

      IRTPTypes::RTXCodecCapabilityParametersPtr FromCx(RTCRtpRtxCodecCapabilityParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::RTXCodecCapabilityParametersPtr();
        auto result = make_shared<IRTPTypes::RTXCodecCapabilityParameters>();
        result->mApt = SafeInt<decltype(result->mApt)>(input->Apt);
        if (0 != input->RtxTime)
        {
          result->mRTXTime = Milliseconds(SafeInt<Milliseconds::rep>(input->RtxTime));
        }
        return result;
      }

      RTCRtpFlexFecCodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::FlexFECCodecCapabilityParameters &input)
      {
        auto result = ref new RTCRtpFlexFecCodecCapabilityParameters();
        if (zsLib::Milliseconds() != input.mRepairWindow)
        {
          result->RepairWindow = SafeInt<uint64>(input.mRepairWindow.count());
        }
        else
        {
          result->RepairWindow = 0;
        }

        result->L = Helper::ToCx(input.mL);
        result->D = Helper::ToCx(input.mD);

        if (input.mToP.hasValue())
        {
          result->ToP = ref new Box<uint16>(SafeInt<uint16>(zsLib::to_underlying(input.mToP.value())));
        }

        return result;
      }

      RTCRtpFlexFecCodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::FlexFECCodecCapabilityParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCxCapabilityParameters(*input);
      }

      IRTPTypes::FlexFECCodecCapabilityParametersPtr FromCx(RTCRtpFlexFecCodecCapabilityParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::FlexFECCodecCapabilityParametersPtr();
        auto result = make_shared<IRTPTypes::FlexFECCodecCapabilityParameters>();
        if (0 != input->RepairWindow)
        {
          result->mRepairWindow = zsLib::Milliseconds(SafeInt<Milliseconds::rep>(input->RepairWindow));
        }
        result->mL = Helper::FromCx(input->L);
        result->mD = Helper::FromCx(input->D);
        if (input->ToP)
        {
          result->mToP = static_cast<IRTPTypes::FlexFECCodecCapabilityParameters::ToPs>(input->ToP->Value);
        }
        return result;
      }

      RTCRtpOpusCodecParameters^ ToCx(const IRTPTypes::OpusCodecParameters &input)
      {
        auto result = ref new RTCRtpOpusCodecParameters();
        result->MaxPlaybackRate = Helper::ToCx(input.mMaxPlaybackRate);
        result->MaxAverageBitrate = Helper::ToCx(input.mMaxAverageBitrate);
        result->Stereo = Helper::ToCx(input.mStereo);
        result->Cbr = Helper::ToCx(input.mCBR);
        result->UseInbandFec = Helper::ToCx(input.mUseInbandFEC);
        result->UseDtx = Helper::ToCx(input.mUseDTX);

        result->Complexity = Helper::ToCx(input.mComplexity);
        result->Signal = Helper::ToCx(zsLib::String(IRTPTypes::OpusCodecCapabilityOptions::toString(input.mSignal)));
        result->Application = Helper::ToCx(zsLib::String(IRTPTypes::OpusCodecCapabilityOptions::toString(input.mApplication)));
        result->PacketLossPerc = Helper::ToCx(input.mPacketLossPerc);
        result->PredictionDisabled = Helper::ToCx(input.mPredictionDisabled);

        result->SpropMaxCaptureRate = Helper::ToCx(input.mSPropMaxCaptureRate);
        result->SpropStereo = Helper::ToCx(input.mSPropStereo);
        return result;
      }

      RTCRtpOpusCodecParameters^ ToCx(IRTPTypes::OpusCodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::OpusCodecParametersPtr FromCx(RTCRtpOpusCodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::OpusCodecParametersPtr();
        auto result = make_shared<IRTPTypes::OpusCodecParameters>();

        try
        {
          result->mMaxPlaybackRate = Helper::FromCx(input->MaxPlaybackRate);
          result->mMaxAverageBitrate = Helper::FromCx(input->MaxAverageBitrate);
          result->mStereo = Helper::FromCx(input->Stereo);
          result->mCBR = Helper::FromCx(input->Cbr);
          result->mUseInbandFEC = Helper::FromCx(input->UseInbandFec);
          result->mUseDTX = Helper::FromCx(input->UseDtx);

          result->mComplexity = Helper::FromCx(input->Complexity);
          result->mSignal = IRTPTypes::OpusCodecCapabilityOptions::toSignal(Helper::FromCx(input->Signal).c_str());
          result->mApplication = IRTPTypes::OpusCodecCapabilityOptions::toApplication(Helper::FromCx(input->Application).c_str());
          result->mPacketLossPerc = Helper::FromCx(input->PacketLossPerc);
          result->mPredictionDisabled = Helper::FromCx(input->PredictionDisabled);

          result->mSPropMaxCaptureRate = Helper::FromCx(input->SpropMaxCaptureRate);
          result->mSPropStereo = Helper::FromCx(input->SpropStereo);
        }
        catch (const InvalidParameters &)
        {
          ORG_ORTC_THROW_INVALID_PARAMETERS()
        }
        return result;
      }

      RTCRtpVp8CodecParameters^ ToCxParameters(const IRTPTypes::VP8CodecParameters &input)
      {
        auto result = ref new RTCRtpVp8CodecParameters();
        result->MaxFr = Helper::ToCx(input.mMaxFR);
        result->MaxFs = Helper::ToCx(input.mMaxFS);
        return result;
      }

      RTCRtpVp8CodecParameters^ ToCxParameters(IRTPTypes::VP8CodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCxParameters(*input);
      }

      IRTPTypes::VP8CodecCapabilityParametersPtr FromCx(RTCRtpVp8CodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::VP8CodecCapabilityParametersPtr();
        auto result = make_shared<IRTPTypes::VP8CodecCapabilityParameters>();
        result->mMaxFR = Helper::FromCx(input->MaxFr);
        result->mMaxFS = Helper::FromCx(input->MaxFs);
        return result;
      }

      RTCRtpH264CodecParameters^ ToCxParameters(const IRTPTypes::H264CodecParameters &input)
      {
        auto result = ref new RTCRtpH264CodecParameters();
        result->ProfileLevelId = Helper::ToCx(input.mProfileLevelID);

        if (input.mPacketizationModes.size() > 0)
        {
          result->PacketizationModes = ref new Vector<uint16>();
          for (auto iter = input.mPacketizationModes.begin(); iter != input.mPacketizationModes.end(); ++iter)
          {
            auto &value = (*iter);
            result->PacketizationModes->Append(value);
          }
        }

        result->MaxMbps = Helper::ToCx(input.mMaxMBPS);
        result->MaxSmbps = Helper::ToCx(input.mMaxSMBPS);
        result->MaxFs = Helper::ToCx(input.mMaxFS);
        result->MaxCpb = Helper::ToCx(input.mMaxCPB);
        result->MaxDpb = Helper::ToCx(input.mMaxDPB);
        result->MaxBr = Helper::ToCx(input.mMaxBR);
        return result;
      }

      RTCRtpH264CodecParameters^ ToCxParameters(IRTPTypes::H264CodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCxParameters(*input);
      }

      IRTPTypes::H264CodecParametersPtr FromCx(RTCRtpH264CodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::H264CodecParametersPtr();
        auto result = make_shared<IRTPTypes::H264CodecParameters>();
        result->mProfileLevelID = Helper::FromCx(input->ProfileLevelId);
        if (input->PacketizationModes)
        {
          for (uint16 value : input->PacketizationModes)
          {
            result->mPacketizationModes.push_back(value);
          }
        }
        result->mMaxMBPS = Helper::FromCx(input->MaxMbps);
        result->mMaxSMBPS = Helper::FromCx(input->MaxSmbps);
        result->mMaxFS = Helper::FromCx(input->MaxFs);
        result->mMaxCPB = Helper::FromCx(input->MaxCpb);
        result->mMaxDPB = Helper::FromCx(input->MaxDpb);
        result->mMaxBR = Helper::FromCx(input->MaxBr);
        return result;
      }

      RTCRtpRtxCodecParameters^ ToCx(const IRTPTypes::RTXCodecParameters &input)
      {
        auto result = ref new RTCRtpRtxCodecParameters();
        if (zsLib::Milliseconds() != input.mRTXTime)
        {
          result->RtxTime = SafeInt<uint32>(input.mRTXTime.count());
        }
        else
        {
          result->RtxTime = 0;
        }
        result->Apt = SafeInt<uint8>(input.mApt);
        return result;
      }

      RTCRtpRtxCodecParameters^ ToCx(IRTPTypes::RTXCodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::RTXCodecParametersPtr FromCx(RTCRtpRtxCodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::RTXCodecParametersPtr();
        auto result = make_shared<IRTPTypes::RTXCodecParameters>();
        result->mApt = SafeInt<decltype(result->mApt)>(input->Apt);
        if (0 != input->RtxTime)
        {
          result->mRTXTime = zsLib::Milliseconds(SafeInt<Milliseconds::rep>(input->RtxTime));
        }
        return result;
      }

      RTCRtpRedCodecParameters^ ToCx(const IRTPTypes::REDCodecParameters &input)
      {
        auto result = ref new RTCRtpRedCodecParameters();
        if (input.mPayloadTypes.size() > 0)
        {
          result->PayloadTypes = ref new Vector<uint8>();
          for (auto iter = input.mPayloadTypes.begin(); iter != input.mPayloadTypes.end(); ++iter)
          {
            auto &value = (*iter);
            result->PayloadTypes->Append(SafeInt<uint8>(value));
          }
        }
        return result;
      }

      RTCRtpRedCodecParameters^ ToCx(IRTPTypes::REDCodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::REDCodecParametersPtr FromCx(RTCRtpRedCodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::REDCodecParametersPtr();
        auto result = make_shared<IRTPTypes::REDCodecParameters>();
        if (input->PayloadTypes)
        {
          for (uint8 value : input->PayloadTypes)
          {
            result->mPayloadTypes.push_back(SafeInt<IRTPTypes::PayloadType>(value));
          }
        }
        return result;
      }

      RTCRtpFlexFecCodecParameters^ ToCx(const IRTPTypes::FlexFECCodecParameters &input)
      {
        auto result = ref new RTCRtpFlexFecCodecParameters();
        if (zsLib::Milliseconds() != input.mRepairWindow)
        {
          result->RepairWindow = SafeInt<uint64>(input.mRepairWindow.count());
        }
        else
        {
          result->RepairWindow = 0;
        }

        result->L = Helper::ToCx(input.mL);
        result->D = Helper::ToCx(input.mD);

        if (input.mToP.hasValue())
        {
          result->ToP = ref new Box<uint16>(SafeInt<uint16>(zsLib::to_underlying(input.mToP.value())));
        }

        return result;
      }

      RTCRtpFlexFecCodecParameters^ ToCx(IRTPTypes::FlexFECCodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::FlexFECCodecParametersPtr FromCx(RTCRtpFlexFecCodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::FlexFECCodecParametersPtr();
        auto result = make_shared<IRTPTypes::FlexFECCodecParameters>();
        if (0 != input->RepairWindow)
        {
          result->mRepairWindow = zsLib::Milliseconds(SafeInt<Milliseconds::rep>(input->RepairWindow));
        }
        result->mL = Helper::FromCx(input->L);
        result->mD = Helper::FromCx(input->D);
        if (input->ToP)
        {
          result->mToP = static_cast<IRTPTypes::FlexFECCodecParameters::ToPs>(input->ToP->Value);
        }
        return result;
      }

      RTCRtpCodecCapability^ ToCx(const IRTPTypes::CodecCapability &input)
      {
        auto result = ref new RTCRtpCodecCapability();

        result->Name = Helper::ToCx(input.mName);
        result->Kind = Helper::ToCx(input.mKind);
        result->ClockRate = SafeInt<uint32>(input.mClockRate);
        result->PreferredPayloadType = SafeInt<uint8>(input.mPreferredPayloadType);
        result->Ptime = SafeInt<decltype(result->Ptime)>(input.mPTime.count());
        result->Maxptime = SafeInt<decltype(result->Maxptime)>(input.mMaxPTime.count());
        result->NumChannels = UseHelper::ToCx(input.mNumChannels);
      
        if (input.mRTCPFeedback.size() > 0) {
          result->RtcpFeedback = ref new Vector<RTCRtcpFeedback^>();

          for (auto iter = input.mRTCPFeedback.begin(); iter != input.mRTCPFeedback.end(); ++iter)
          {
            result->RtcpFeedback->Append(ToCx(*iter));
          }
        }

        if (input.mOptions)
        {
          {
            auto value = IRTPTypes::OpusCodecCapabilityOptions::convert(input.mOptions);
            if (value)
            {
              result->Options = ToCx(value);
              goto done_convert_options;
            }
          }
        done_convert_options:
          {}
        }

        if (input.mParameters)
        {
          {
            auto value = IRTPTypes::OpusCodecCapabilityParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCx(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::VP8CodecCapabilityParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCxCapabilityParameters(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::H264CodecCapabilityParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCxCapabilityParameters(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::RTXCodecCapabilityParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCxCapabilityParameters(value);
              goto done_convert_params;
            }
          }
        done_convert_params:
          {}
        }

        result->MaxTemporalLayers = SafeInt<uint16>(input.mMaxTemporalLayers);
        result->MaxSpatialLayers = SafeInt<uint16>(input.mMaxSpatialLayers);
        result->SvcMultiStreamSupport = input.mSVCMultiStreamSupport;

        return result;
      }

      RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::CodecCapabilityPtr FromCx(RTCRtpCodecCapability^ input)
      {
        if (nullptr == input) return IRTPTypes::CodecCapabilityPtr();
        auto result = make_shared<IRTPTypes::CodecCapability>();

        result->mName = Helper::FromCx(input->Name);
        result->mKind = Helper::FromCx(input->Kind);
        result->mClockRate = SafeInt<decltype(result->mClockRate)>(input->ClockRate);
        result->mPreferredPayloadType = SafeInt<decltype(result->mPreferredPayloadType)>(input->PreferredPayloadType);
        result->mPTime = Milliseconds(SafeInt<Milliseconds::rep>(input->Ptime));
        result->mMaxPTime = Milliseconds(SafeInt<Milliseconds::rep>(input->Maxptime));
        result->mNumChannels = UseHelper::FromCx(input->NumChannels);

        if (input->RtcpFeedback)
        {
          for (RTCRtcpFeedback^ value : input->RtcpFeedback)
          {
            result->mRTCPFeedback.push_back(FromCx(value));
          }
        }

        if (nullptr != input->Options)
        {
          {
            RTCRtpOpusCodecCapabilityOptions^ value = dynamic_cast<RTCRtpOpusCodecCapabilityOptions^>(input->Options);
            if (nullptr != value)
            {
              result->mOptions = FromCx(value);
              goto done_convert_options;
            }
          }
        done_convert_options:
          {}
        }

        if (nullptr != input->Parameters)
        {
          {
            RTCRtpOpusCodecCapabilityParameters^ value = dynamic_cast<RTCRtpOpusCodecCapabilityParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpVp8CodecCapabilityParameters^ value = dynamic_cast<RTCRtpVp8CodecCapabilityParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpH264CodecCapabilityParameters^ value = dynamic_cast<RTCRtpH264CodecCapabilityParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpRtxCodecCapabilityParameters^ value = dynamic_cast<RTCRtpRtxCodecCapabilityParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpFlexFecCodecCapabilityParameters^ value = dynamic_cast<RTCRtpFlexFecCodecCapabilityParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
        done_convert_params:
          {}
        }

        result->mMaxTemporalLayers = SafeInt<uint16>(input->MaxTemporalLayers);
        result->mMaxSpatialLayers = SafeInt<uint16>(input->MaxSpatialLayers);
        result->mSVCMultiStreamSupport = input->SvcMultiStreamSupport;
        return result;
      }

      RTCRtpHeaderExtension^ ToCx(const IRTPTypes::HeaderExtension &input)
      {
        auto result = ref new RTCRtpHeaderExtension();
        result->Kind = Helper::ToCx(input.mKind);
        result->Uri = Helper::ToCx(input.mURI);
        result->PreferredId = SafeInt<uint16>(input.mPreferredID);
        result->PreferredEncrypt = input.mPreferredEncrypt;
        return result;
      }

      RTCRtpHeaderExtension^ ToCx(IRTPTypes::HeaderExtensionPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::HeaderExtensionPtr FromCx(RTCRtpHeaderExtension^ input)
      {
        if (nullptr == input) return IRTPTypes::HeaderExtensionPtr();
        auto result = make_shared<IRTPTypes::HeaderExtension>();
        result->mKind = Helper::FromCx(input->Kind);
        result->mURI = Helper::FromCx(input->Uri);
        result->mPreferredID = SafeInt<decltype(result->mPreferredID)>(input->PreferredId);
        result->mPreferredEncrypt = input->PreferredEncrypt;
        return result;
      }

      RTCRtpCapabilities^ ToCx(const IRTPTypes::Capabilities &input)
      {
        auto result = ref new RTCRtpCapabilities();
        if (input.mCodecs.size() > 0)
        {
          result->Codecs = ref new Vector<RTCRtpCodecCapability^>();
          for (auto iter = input.mCodecs.begin(); iter != input.mCodecs.end(); ++iter)
          {
            auto &value = (*iter);
            result->Codecs->Append(ToCx(value));
          }
        }
        if (input.mHeaderExtensions.size() > 0)
        {
          result->HeaderExtensions = ref new Vector<RTCRtpHeaderExtension^>();
          for (auto iter = input.mHeaderExtensions.begin(); iter != input.mHeaderExtensions.end(); ++iter)
          {
            auto &value = (*iter);
            result->HeaderExtensions->Append(ToCx(value));
          }
        }
        if (input.mFECMechanisms.size() > 0)
        {
          result->FecMechanisms = ref new Vector<Platform::String^>();
          for (auto iter = input.mFECMechanisms.begin(); iter != input.mFECMechanisms.end(); ++iter)
          {
            auto &value = (*iter);
            result->FecMechanisms->Append(Helper::ToCx(value));
          }
        }
        return result;
      }

      RTCRtpCapabilities^ ToCx(IRTPTypes::CapabilitiesPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::CapabilitiesPtr FromCx(RTCRtpCapabilities^ input)
      {
        if (nullptr == input) return IRTPTypes::CapabilitiesPtr();
        auto result = make_shared<IRTPTypes::Capabilities>();
        if (input->Codecs)
        {
          for (RTCRtpCodecCapability^ value : input->Codecs)
          {
            auto codec = FromCx(value);
            if (!codec) continue;
            result->mCodecs.push_back(*codec);
          }
        }

        if (input->HeaderExtensions)
        {
          for (RTCRtpHeaderExtension^ value : input->HeaderExtensions)
          {
            auto ext = FromCx(value);
            if (!ext) continue;
            result->mHeaderExtensions.push_back(*ext);
          }
        }

        if (input->FecMechanisms)
        {
          for (Platform::String^ value : input->FecMechanisms)
          {
            result->mFECMechanisms.push_back(Helper::FromCx(value));
          }
        }
        return result;
      }

      RTCRtcpParameters^ ToCx(const IRTPTypes::RTCPParameters &input)
      {
        auto result = ref new RTCRtcpParameters();
        result->Ssrc = SafeInt<uint32>(input.mSSRC);
        result->Cname = Helper::ToCx(input.mCName);
        result->ReducedSize = input.mReducedSize;
        result->Mux = input.mMux;
        return result;
      }

      RTCRtcpParameters^ ToCx(IRTPTypes::RTCPParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::RTCPParametersPtr FromCx(RTCRtcpParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::RTCPParametersPtr();
        auto result = make_shared<IRTPTypes::RTCPParameters>();
        result->mSSRC = SafeInt<decltype(result->mSSRC)>(input->Ssrc);
        result->mCName = Helper::FromCx(input->Cname);
        result->mReducedSize = input->ReducedSize;
        result->mMux = input->Mux;
        return result;
      }

      RTCRtpCodecParameters^ ToCx(const IRTPTypes::CodecParameters &input)
      {
        auto result = ref new RTCRtpCodecParameters();

        result->Name = Helper::ToCx(input.mName);
        result->PayloadType = SafeInt<uint8>(input.mPayloadType);
        result->ClockRate = Helper::ToCx(input.mClockRate);
        result->Ptime = SafeInt<decltype(result->Ptime)>(input.mPTime.count());
        result->Maxptime = SafeInt<decltype(result->Maxptime)>(input.mMaxPTime.count());
        result->NumChannels = UseHelper::ToCx(input.mNumChannels);

        if (input.mRTCPFeedback.size() > 0) {
          result->RtcpFeedback = ref new Vector<RTCRtcpFeedback^>();

          for (auto iter = input.mRTCPFeedback.begin(); iter != input.mRTCPFeedback.end(); ++iter)
          {
            result->RtcpFeedback->Append(ToCx(*iter));
          }
        }

        if (input.mParameters)
        {
          {
            auto value = IRTPTypes::OpusCodecParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCx(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::VP8CodecParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCxParameters(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::H264CodecParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCxParameters(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::RTXCodecParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCx(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::REDCodecParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCx(value);
              goto done_convert_params;
            }
          }
          {
            auto value = IRTPTypes::FlexFECCodecParameters::convert(input.mParameters);
            if (value)
            {
              result->Parameters = ToCx(value);
              goto done_convert_params;
            }
          }
        done_convert_params:
          {}
        }

        return result;
      }

      RTCRtpCodecParameters^ ToCx(IRTPTypes::CodecParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::CodecParametersPtr FromCx(RTCRtpCodecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::CodecParametersPtr();
        auto result = make_shared<IRTPTypes::CodecParameters>();

        result->mName = Helper::FromCx(input->Name);
        result->mPayloadType = SafeInt<decltype(result->mPayloadType)>(input->PayloadType);
        result->mClockRate = Helper::FromCx(input->ClockRate);
        result->mPTime = Milliseconds(SafeInt<Milliseconds::rep>(input->Ptime));
        result->mMaxPTime = Milliseconds(SafeInt<Milliseconds::rep>(input->Maxptime));
        result->mNumChannels = UseHelper::FromCx(input->NumChannels);

        if (input->RtcpFeedback)
        {
          for (RTCRtcpFeedback^ value : input->RtcpFeedback)
          {
            result->mRTCPFeedback.push_back(FromCx(value));
          }
        }

        if (nullptr != input->Parameters)
        {
          {
            RTCRtpOpusCodecParameters^ value = dynamic_cast<RTCRtpOpusCodecParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpVp8CodecParameters^ value = dynamic_cast<RTCRtpVp8CodecParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpH264CodecParameters^ value = dynamic_cast<RTCRtpH264CodecParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpRtxCodecParameters^ value = dynamic_cast<RTCRtpRtxCodecParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpRedCodecParameters^ value = dynamic_cast<RTCRtpRedCodecParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
          {
            RTCRtpFlexFecCodecParameters^ value = dynamic_cast<RTCRtpFlexFecCodecParameters^>(input->Parameters);
            if (nullptr != value)
            {
              result->mParameters = FromCx(value);
              goto done_convert_params;
            }
          }
        done_convert_params:
          {}
        }

        return result;
      }

      RTCRtpFecParameters^ ToCx(const IRTPTypes::FECParameters &input)
      {
        auto result = ref new RTCRtpFecParameters();
        result->Ssrc = Helper::ToCx(input.mSSRC);
        result->Mechanism = Helper::ToCx(input.mMechanism);
        return result;
      }

      RTCRtpFecParameters^ ToCx(IRTPTypes::FECParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::FECParametersPtr FromCx(RTCRtpFecParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::FECParametersPtr();
        auto result = make_shared<IRTPTypes::FECParameters>();
        result->mSSRC = Helper::FromCx(input->Ssrc);
        result->mMechanism = Helper::FromCx(input->Mechanism);
        return result;
      }

      RTCRtpRtxParameters^ ToCx(const IRTPTypes::RTXParameters &input)
      {
        auto result = ref new RTCRtpRtxParameters();
        result->Ssrc = Helper::ToCx(input.mSSRC);
        return result;
      }

      RTCRtpRtxParameters^ ToCx(IRTPTypes::RTXParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::RTXParametersPtr FromCx(RTCRtpRtxParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::RTXParametersPtr();
        auto result = make_shared<IRTPTypes::RTXParameters>();
        result->mSSRC = Helper::FromCx(input->Ssrc);
        return result;
      }

      RTCRtpEncodingParameters^ ToCx(const IRTPTypes::EncodingParameters &input)
      {
        auto result = ref new RTCRtpEncodingParameters();

        result->Ssrc = Helper::ToCx(input.mSSRC);
        result->CodecPayloadType = Helper::ToCx(input.mCodecPayloadType);
        if (input.mFEC.hasValue()) result->Fec = ToCx(input.mFEC.value());
        if (input.mRTX.hasValue()) result->Rtx = ToCx(input.mRTX.value());

        result->Priority = Helper::Convert(input.mPriority);

        result->MaxBitrate = UseHelper::ToCx(input.mMaxBitrate);
        result->MinQuality = UseHelper::ToCx(input.mMinQuality);
        result->ResolutionScale = UseHelper::ToCx(input.mResolutionScale);
        result->FramerateScale = UseHelper::ToCx(input.mFramerateScale);
        result->Active = input.mActive;
        result->EncodingId = Helper::ToCx(input.mEncodingID);

        if (input.mDependencyEncodingIDs.size() > 0)
        {
          result->DependencyEncodingIds = ref new Vector<Platform::String^>();
          for (auto iter = input.mDependencyEncodingIDs.begin(); iter != input.mDependencyEncodingIDs.end(); ++iter)
          {
            auto &value = (*iter);
            result->DependencyEncodingIds->Append(Helper::ToCx(value));
          }
        }
        return result;
      }

      RTCRtpEncodingParameters^ ToCx(IRTPTypes::EncodingParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::EncodingParametersPtr FromCx(RTCRtpEncodingParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::EncodingParametersPtr();
        auto result = make_shared<IRTPTypes::EncodingParameters>();

        result->mSSRC = Helper::FromCx(input->Ssrc);
        result->mCodecPayloadType = Helper::FromCx(input->CodecPayloadType);
        if (input->Fec)
        {
          result->mFEC = *FromCx(input->Fec);
        }
        if (input->Rtx)
        {
          result->mRTX = *FromCx(input->Rtx);
        }
        result->mPriority = Helper::Convert(input->Priority);
        result->mMaxBitrate = UseHelper::FromCx(input->MaxBitrate);
        result->mMinQuality = UseHelper::FromCx(input->MinQuality);
        result->mResolutionScale = UseHelper::FromCx(input->ResolutionScale);
        result->mFramerateScale = UseHelper::FromCx(input->FramerateScale);
        result->mEncodingID = Helper::FromCx(input->EncodingId);

        if (input->DependencyEncodingIds)
        {
          for (Platform::String^ value : input->DependencyEncodingIds)
          {
            result->mDependencyEncodingIDs.push_back(Helper::FromCx(value));
          }
        }

        return result;
      }

      RTCRtpHeaderExtensionParameters^ ToCx(const IRTPTypes::HeaderExtensionParameters &input)
      {
        auto result = ref new RTCRtpHeaderExtensionParameters();
        result->Uri = Helper::ToCx(input.mURI);
        result->Id = SafeInt<uint16>(input.mID);
        result->Encrypt = input.mEncrypt;
        return result;
      }

      RTCRtpHeaderExtensionParameters^ ToCx(IRTPTypes::HeaderExtensionParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::HeaderExtensionParametersPtr FromCx(RTCRtpHeaderExtensionParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::HeaderExtensionParametersPtr();
        auto result = make_shared<IRTPTypes::HeaderExtensionParameters>();
        result->mURI = Helper::FromCx(input->Uri);
        result->mID = SafeInt<decltype(result->mID)>(input->Id);
        result->mEncrypt = input->Encrypt;
        return result;
      }

      RTCRtpParameters^ ToCx(const IRTPTypes::Parameters &input)
      {
        auto result = ref new RTCRtpParameters();

        result->MuxId = Helper::ToCx(input.mMuxID);
        if (input.mCodecs.size() > 0)
        {
          result->Codecs = ref new Vector<RTCRtpCodecParameters^>();

          for (auto iter = input.mCodecs.begin(); iter != input.mCodecs.end(); ++iter)
          {
            auto &value = (*iter);
            result->Codecs->Append(ToCx(value));
          }
        }
        if (input.mHeaderExtensions.size() > 0)
        {
          result->HeaderExtensions = ref new Vector<RTCRtpHeaderExtensionParameters^>();

          for (auto iter = input.mHeaderExtensions.begin(); iter != input.mHeaderExtensions.end(); ++iter)
          {
            auto &value = (*iter);
            result->HeaderExtensions->Append(ToCx(value));
          }
        }
        if (input.mEncodings.size() > 0)
        {
          result->Encodings = ref new Vector<RTCRtpEncodingParameters^>();

          for (auto iter = input.mEncodings.begin(); iter != input.mEncodings.end(); ++iter)
          {
            auto &value = (*iter);
            result->Encodings->Append(ToCx(value));
          }
        }

        result->Rtcp = ToCx(input.mRTCP);
        result->DegradationPreference = Helper::Convert(input.mDegredationPreference);

        return result;
      }

      RTCRtpParameters^ ToCx(IRTPTypes::ParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IRTPTypes::ParametersPtr FromCx(RTCRtpParameters^ input)
      {
        if (nullptr == input) return IRTPTypes::ParametersPtr();
        auto result = make_shared<IRTPTypes::Parameters>();

        result->mMuxID = Helper::FromCx(input->MuxId);

        if (input->Codecs)
        {
          for (RTCRtpCodecParameters^ value : input->Codecs)
          {
            if (nullptr == value) continue;
            result->mCodecs.push_back(*FromCx(value));
          }
        }
        if (input->HeaderExtensions)
        {
          for (RTCRtpHeaderExtensionParameters^ value : input->HeaderExtensions)
          {
            if (nullptr == value) continue;
            result->mHeaderExtensions.push_back(*FromCx(value));
          }
        }
        if (input->Encodings)
        {
          for (RTCRtpEncodingParameters^ value : input->Encodings)
          {
            if (nullptr == value) continue;
            result->mEncodings.push_back(*FromCx(value));
          }
        }

        if (input->Rtcp)
        {
          result->mRTCP = *FromCx(input->Rtcp);
        }
        result->mDegredationPreference = Helper::Convert(input->DegradationPreference);
        return result;
      }

    } // namespace internal

    //---------------------------------------------------------------------------
    // RTCRtpCapabilities methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCRtpCapabilities::ToJsonString()
    {
      auto caps = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(caps->createElement("RtpCapabilities")));
    }

    RTCRtpCapabilities^ RTCRtpCapabilities::FromJsonString(Platform::String^ jsonString)
    {
      auto ret = ref new RTCRtpCapabilities();

      auto capabilitiesPtr = make_shared<IRTPTypes::Capabilities>(IRTPTypes::Capabilities::Capabilities(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));

      return internal::ToCx(capabilitiesPtr);
    }

    //---------------------------------------------------------------------------
    // RTCRtpCodecCapability methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCRtpCodecCapability::ToJsonString()
    {
      auto capability = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(capability->createElement("CodecCapability")));
    }

    RTCRtpCodecCapability^ RTCRtpCodecCapability::FromJsonString(Platform::String^ jsonString)
    {
      auto capability = make_shared<IRTPTypes::CodecCapability>(IRTPTypes::CodecCapability::CodecCapability(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return internal::ToCx(capability);
    }

    //---------------------------------------------------------------------------
    // RTCRtcpFeedback methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCRtcpFeedback::ToJsonString()
    {
      IRTPTypes::RTCPFeedback feedback = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(feedback.createElement("RtcpFeedback")));
    }

    RTCRtcpFeedback^ RTCRtcpFeedback::FromJsonString(Platform::String^ jsonString)
    {
      auto ret = make_shared<IRTPTypes::RTCPFeedback>(IRTPTypes::RTCPFeedback::RTCPFeedback(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return internal::ToCx(ret);
    }

    //---------------------------------------------------------------------------
    // RTCRtpHeaderExtension methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCRtpHeaderExtension::ToJsonString()
    {
      auto ext = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(ext->createElement("RtpHeaderExtension")));
    }

    RTCRtpHeaderExtension^ RTCRtpHeaderExtension::FromJsonString(Platform::String^ jsonString)
    {
      auto ret = make_shared<IRTPTypes::HeaderExtension>(IRTPTypes::HeaderExtension::HeaderExtension(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return internal::ToCx(ret);
    }

  } // namespace ortc
} // namespace org
