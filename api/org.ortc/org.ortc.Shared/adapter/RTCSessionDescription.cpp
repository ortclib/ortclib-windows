#include "pch.h"

#include "RTCSessionDescription.h"
#include "../Error.h"
#include "../helpers.h"
#include "../RTCIceGatherer.h"
#include "../RTCDtlsTransport.h"
#include "../RTCSrtpSdesTransport.h"
#include "../RTPTypes.h"
#include "../RTCSctpTransport.h"

#include <ortc/internal/ortc_Helper.h>
#include <ortc/adapter/IHelper.h>
#include <openpeer/services/IHelper.h>
#include <zsLib/SafeInt.h>

using Platform::Collections::Vector;

using namespace ortc;

namespace org
{
  namespace ortc
  {
    namespace adapter
    {
      ZS_DECLARE_TYPEDEF_PTR(org::ortc::internal::Helper, UseHelper);
      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IHelper, UseAdapterHelper);
      ZS_DECLARE_TYPEDEF_PTR(openpeer::services::IHelper, UseServicesHelper);

      using std::make_shared;

      namespace internal
      {

#pragma region RTCSessionDescriptionHelper types converters

        RTCSessionDescriptionDescription^ ToCx(const ISessionDescriptionTypes::Description &input)
        {
          auto result = ref new RTCSessionDescriptionDescription;
          result->Transports = ref new Vector<RTCSessionDescriptionTransport^>();
          result->RtpMediaLines = ref new Vector<RTCSessionDescriptionRtpMediaLine^>();
          result->SctpMediaLines = ref new Vector<RTCSessionDescriptionSctpMediaLine^>();
          result->RtpSenders = ref new Vector<RTCSessionDescriptionRtpSender^>();          
          result->Details = ToCx(input.mDetails);

          for (auto iter = input.mTransports.begin(); iter != input.mTransports.end(); ++iter)
          {
            auto &value = (*iter);
            if (nullptr == value) continue;
            result->Transports->Append(ToCx(value));
          }
          for (auto iter = input.mRTPMediaLines.begin(); iter != input.mRTPMediaLines.end(); ++iter)
          {
            auto &value = (*iter);
            if (nullptr == value) continue;
            result->RtpMediaLines->Append(ToCx(value));
          }
          for (auto iter = input.mSCTPMediaLines.begin(); iter != input.mSCTPMediaLines.end(); ++iter)
          {
            auto &value = (*iter);
            if (nullptr == value) continue;
            result->SctpMediaLines->Append(ToCx(value));
          }
          for (auto iter = input.mRTPSenders.begin(); iter != input.mRTPSenders.end(); ++iter)
          {
            auto &value = (*iter);
            if (nullptr == value) continue;
            result->RtpSenders->Append(ToCx(value));
          }
          return result;
        }

        RTCSessionDescriptionDescription^ ToCx(ISessionDescriptionTypes::DescriptionPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }

        ISessionDescriptionTypes::DescriptionPtr FromCx(RTCSessionDescriptionDescription^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::DescriptionPtr();
          auto result(make_shared<ISessionDescriptionTypes::Description>());

          result->mDetails = FromCx(input->Details);

          for (RTCSessionDescriptionTransport^ value : input->Transports)
          {
            if (nullptr == value) continue;
            result->mTransports.push_back(FromCx(value));
          }
          for (RTCSessionDescriptionRtpMediaLine^ value : input->RtpMediaLines)
          {
            if (nullptr == value) continue;
            result->mRTPMediaLines.push_back(FromCx(value));
          }
          for (RTCSessionDescriptionSctpMediaLine^ value : input->SctpMediaLines)
          {
            if (nullptr == value) continue;
            result->mSCTPMediaLines.push_back(FromCx(value));
          }
          for (RTCSessionDescriptionRtpSender^ value : input->RtpSenders)
          {
            if (nullptr == value) continue;
            result->mRTPSenders.push_back(FromCx(value));
          }

          return result;
        }

        RTCSessionDescriptionDescriptionDetails^ ToCx(const ISessionDescriptionTypes::Description::Details &input)
        {
          auto result = ref new RTCSessionDescriptionDescriptionDetails;
          result->Username = UseHelper::ToCx(input.mUsername);
          result->SessionId = SafeInt<decltype(result->SessionId)>(input.mSessionID);
          result->SessionVersion = SafeInt<decltype(result->SessionVersion)>(input.mSessionVersion);
          result->UnicastAddress = ToCx(input.mUnicaseAddress);
          result->SessionName = UseHelper::ToCx(input.mSessionName);
          result->StartTime = SafeInt<decltype(result->StartTime)>(input.mStartTime);
          result->EndTime = SafeInt<decltype(result->EndTime)>(input.mEndTime);
          result->ConnectionData = ToCx(input.mConnectionData);
          return result;
        }

        RTCSessionDescriptionDescriptionDetails^ ToCx(ISessionDescriptionTypes::Description::DetailsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }

        ISessionDescriptionTypes::Description::DetailsPtr FromCx(RTCSessionDescriptionDescriptionDetails^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::Description::DetailsPtr();
          auto result(make_shared<ISessionDescriptionTypes::Description::Details>());

          result->mUsername = UseHelper::FromCx(input->Username);
          result->mSessionID = SafeInt<decltype(result->mSessionID)>(input->SessionId);
          result->mSessionVersion = SafeInt<decltype(result->mSessionVersion)>(input->SessionVersion);
          result->mUnicaseAddress = FromCx(input->UnicastAddress);
          result->mSessionName = UseHelper::FromCx(input->SessionName);
          result->mStartTime = SafeInt<decltype(result->mStartTime)>(input->StartTime);
          result->mEndTime = SafeInt<decltype(result->mEndTime)>(input->EndTime);
          return result;
        }

        RTCSessionDescriptionConnectionData^ ToCx(const ISessionDescriptionTypes::ConnectionData &input)
        {
          auto result = ref new RTCSessionDescriptionConnectionData;
          result->Rtp = ToCx(input.mRTP);
          result->Rtcp = ToCx(input.mRTCP);
          return result;
        }

        RTCSessionDescriptionConnectionData^ ToCx(ISessionDescriptionTypes::ConnectionDataPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }

        ISessionDescriptionTypes::ConnectionDataPtr FromCx(RTCSessionDescriptionConnectionData^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::ConnectionDataPtr();
          auto result(make_shared<ISessionDescriptionTypes::ConnectionData>());

          result->mRTP = FromCx(input->Rtp);
          result->mRTCP = FromCx(input->Rtcp);
          return result;
        }

        RTCSessionDescriptionConnectionDataDetails^ ToCx(const ISessionDescriptionTypes::ConnectionData::Details &input)
        {
          auto result = ref new RTCSessionDescriptionConnectionDataDetails;
          result->Port = UseHelper::ToCx(input.mPort);
          result->NetType = UseHelper::ToCx(input.mNetType);
          result->AddrType = UseHelper::ToCx(input.mAddrType);
          result->ConnectionAddress = UseHelper::ToCx(input.mConnectionAddress);
          return result;
        }

        RTCSessionDescriptionConnectionDataDetails^ ToCx(ISessionDescriptionTypes::ConnectionData::DetailsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }

        ISessionDescriptionTypes::ConnectionData::DetailsPtr FromCx(RTCSessionDescriptionConnectionDataDetails^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::ConnectionData::DetailsPtr();
          auto result(make_shared<ISessionDescriptionTypes::ConnectionData::Details>());

          result->mPort = UseHelper::FromCx(input->Port);
          result->mNetType = UseHelper::FromCx(input->NetType);
          result->mAddrType = UseHelper::FromCx(input->AddrType);
          result->mConnectionAddress = UseHelper::FromCx(input->AddrType);
          return result;
        }

        RTCSessionDescriptionTransport^ ToCx(const ISessionDescriptionTypes::Transport &input)
        {
          auto result = ref new RTCSessionDescriptionTransport;
          result->Id = UseHelper::ToCx(input.mID);
          result->Rtp = ToCx(input.mRTP);
          result->Rtcp = ToCx(input.mRTCP);
          return result;
        }
        RTCSessionDescriptionTransport^ ToCx(ISessionDescriptionTypes::TransportPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        ISessionDescriptionTypes::TransportPtr FromCx(RTCSessionDescriptionTransport^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::TransportPtr();
          auto result(make_shared<ISessionDescriptionTypes::Transport>());

          result->mID = UseHelper::FromCx(input->Id);
          result->mRTP = FromCx(input->Rtp);
          result->mRTCP = FromCx(input->Rtcp);
          return result;
        }

        RTCSessionDescriptionTransportParameters^ ToCx(const ISessionDescriptionTypes::Transport::Parameters &input)
        {
          auto result = ref new RTCSessionDescriptionTransportParameters;
          result->IceParameters = ortc::internal::ToCx(input.mICEParameters);
          result->DtlsParameters = ortc::internal::ToCx(input.mDTLSParameters);
          result->SrtpSdesParameters = ortc::internal::ToCx(input.mSRTPSDESParameters);
          result->IceCandidates = ref new Vector<ortc::RTCIceCandidate^>();
          result->EndOfCandidates = input.mEndOfCandidates;
          for (auto iter = input.mICECandidates.begin(); iter != input.mICECandidates.end(); ++iter)
          {
            auto &value = (*iter);
            if (nullptr == value) continue;

            {
              auto converted = ZS_DYNAMIC_PTR_CAST(IICETypes::Candidate, value);
              if (converted) {
                result->IceCandidates->Append(org::ortc::internal::ToCx(value));
                continue;
              }
              {
                auto converted = ZS_DYNAMIC_PTR_CAST(IICETypes::CandidateComplete, value);
                if (converted) {
                  result->EndOfCandidates = true;
                  continue;
                }
              }
            }
          }
          return result;
        }
        RTCSessionDescriptionTransportParameters^ ToCx(ISessionDescriptionTypes::Transport::ParametersPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        ISessionDescriptionTypes::Transport::ParametersPtr FromCx(RTCSessionDescriptionTransportParameters^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::Transport::ParametersPtr();
          auto result(make_shared<ISessionDescriptionTypes::Transport::Parameters>());
          result->mICEParameters = ortc::internal::FromCx(input->IceParameters);
          result->mDTLSParameters = ortc::internal::FromCx(input->DtlsParameters);
          result->mSRTPSDESParameters = ortc::internal::FromCx(input->SrtpSdesParameters);
          for (ortc::RTCIceCandidate^ value : input->IceCandidates)
          {
            if (nullptr == value) continue;
            result->mICECandidates.push_back(org::ortc::internal::FromCx(value));
          }
          result->mEndOfCandidates = input->EndOfCandidates;
          return result;
        }

        RTCSessionDescriptionMediaLineDetails^ ToCx(const ISessionDescriptionTypes::MediaLine::Details &input)
        {
          auto result = ref new RTCSessionDescriptionMediaLineDetails;
          if (input.mInternalIndex.hasValue()) {
            Optional<WORD> value(SafeInt<WORD>(input.mInternalIndex.value()));
            result->InternalIndex = UseHelper::ToCx(value);
          }
          result->Protocol = UseHelper::ToCx(input.mProtocol);
          result->ConnectionData = ToCx(input.mConnectionData);
          result->MediaDirection = UseHelper::Convert(input.mMediaDirection);
          return result;
        }
        RTCSessionDescriptionMediaLineDetails^ ToCx(ISessionDescriptionTypes::MediaLine::DetailsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        ISessionDescriptionTypes::MediaLine::DetailsPtr FromCx(RTCSessionDescriptionMediaLineDetails^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::MediaLine::DetailsPtr();
          auto result(make_shared<ISessionDescriptionTypes::MediaLine::Details>());
          {
            auto optValue = UseHelper::FromCx(input->InternalIndex);
            if (optValue.hasValue()) {
              auto value = optValue.value();
              result->mInternalIndex = Optional<decltype(result->mInternalIndex.mType)>(SafeInt<decltype(result->mInternalIndex.mType)>(value));
            }
          }
          result->mProtocol = UseHelper::FromCx(input->Protocol);
          result->mConnectionData = FromCx(input->ConnectionData);
          result->mMediaDirection = UseHelper::Convert(input->MediaDirection);
          return result;
        }

        RTCSessionDescriptionRtpMediaLine^ ToCx(const ISessionDescriptionTypes::RTPMediaLine &input)
        {
          auto result = ref new RTCSessionDescriptionRtpMediaLine;
          result->Id = UseHelper::ToCx(input.mID);
          result->TransportId = UseHelper::ToCx(input.mTransportID);
          result->MediaType = UseHelper::ToCx(input.mMediaType);
          result->Details = ToCx(input.mDetails);
          result->SenderCapabilities = ortc::internal::ToCx(input.mSenderCapabilities);
          result->ReceiverCapabilities = ortc::internal::ToCx(input.mReceiverCapabilities);
          return result;
        }
        RTCSessionDescriptionRtpMediaLine^ ToCx(ISessionDescriptionTypes::RTPMediaLinePtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        ISessionDescriptionTypes::RTPMediaLinePtr FromCx(RTCSessionDescriptionRtpMediaLine^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::RTPMediaLinePtr();
          auto result(make_shared<ISessionDescriptionTypes::RTPMediaLine>());
          result->mID = UseHelper::FromCx(input->Id);
          result->mTransportID = UseHelper::FromCx(input->TransportId);
          result->mMediaType = UseHelper::FromCx(input->MediaType);
          result->mDetails = FromCx(input->Details);
          result->mSenderCapabilities = ortc::internal::FromCx(input->SenderCapabilities);
          result->mReceiverCapabilities = ortc::internal::FromCx(input->ReceiverCapabilities);
          return result;
        }

        RTCSessionDescriptionSctpMediaLine^ ToCx(const ISessionDescriptionTypes::SCTPMediaLine &input)
        {
          auto result = ref new RTCSessionDescriptionSctpMediaLine;
          result->Id = UseHelper::ToCx(input.mID);
          result->TransportId = UseHelper::ToCx(input.mTransportID);
          result->MediaType = UseHelper::ToCx(input.mMediaType);
          result->Details = ToCx(input.mDetails);
          result->Capabilities = ortc::internal::ToCx(input.mCapabilities);
          result->Port = UseHelper::ToCx(input.mPort);
          return result;
        }

        RTCSessionDescriptionSctpMediaLine^ ToCx(ISessionDescriptionTypes::SCTPMediaLinePtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }

        ISessionDescriptionTypes::SCTPMediaLinePtr FromCx(RTCSessionDescriptionSctpMediaLine^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::SCTPMediaLinePtr();
          auto result(make_shared<ISessionDescriptionTypes::SCTPMediaLine>());
          result->mID = UseHelper::FromCx(input->Id);
          result->mTransportID = UseHelper::FromCx(input->TransportId);
          result->mMediaType = UseHelper::FromCx(input->MediaType);
          result->mDetails = FromCx(input->Details);
          result->mCapabilities = ortc::internal::FromCx(input->Capabilities);
          result->mPort = UseHelper::FromCx(input->Port);
          return result;
        }

        RTCSessionDescriptionRtpSender^ ToCx(const ISessionDescriptionTypes::RTPSender &input)
        {
          auto result = ref new RTCSessionDescriptionRtpSender;
          result->Id = UseHelper::ToCx(input.mID);
          result->RtpMediaLineId = UseHelper::ToCx(input.mRTPMediaLineID);
          result->Parameters = ortc::internal::ToCx(input.mParameters);
          result->MediaStreamIds = ref new Vector<Platform::String^>();

          for (auto iter = input.mMediaStreamIDs.begin(); iter != input.mMediaStreamIDs.end(); ++iter)
          {
            auto &value = (*iter);
            if (value.isEmpty()) continue;
            result->MediaStreamIds->Append(UseHelper::ToCx(value));
          }

          return result;
        }
        RTCSessionDescriptionRtpSender^ ToCx(ISessionDescriptionTypes::RTPSenderPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        ISessionDescriptionTypes::RTPSenderPtr FromCx(RTCSessionDescriptionRtpSender^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::RTPSenderPtr();
          auto result(make_shared<ISessionDescriptionTypes::RTPSender>());
          result->mID = UseHelper::FromCx(input->Id);
          result->mRTPMediaLineID = UseHelper::FromCx(input->RtpMediaLineId);
          result->mDetails = FromCx(input->Details);
          result->mParameters = ortc::internal::FromCx(input->Parameters);

          for (Platform::String^ value : input->MediaStreamIds)
          {
            if (nullptr == value) continue;
            result->mMediaStreamIDs.insert(UseHelper::FromCx(value));
          }
          return result;
        }

        RTCSessionDescriptionRtpSenderDetails^ ToCx(const ISessionDescriptionTypes::RTPSender::Details &input)
        {
          auto result = ref new RTCSessionDescriptionRtpSenderDetails;
          if (input.mInternalRTPMediaLineIndex.hasValue()) {
            Optional<WORD> value(SafeInt<WORD>(input.mInternalRTPMediaLineIndex.value()));
            result->InternalRtpMediaLineIndex = UseHelper::ToCx(value);
          }
          return result;
        }
        RTCSessionDescriptionRtpSenderDetails^ ToCx(ISessionDescriptionTypes::RTPSender::DetailsPtr input)
        {
          if (!input) return nullptr;
          return ToCx(*input);
        }
        ISessionDescriptionTypes::RTPSender::DetailsPtr FromCx(RTCSessionDescriptionRtpSenderDetails^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::RTPSender::DetailsPtr();
          auto result(make_shared<ISessionDescriptionTypes::RTPSender::Details>());
          {
            auto optValue = UseHelper::FromCx(input->InternalRtpMediaLineIndex);
            if (optValue.hasValue()) {
              auto value = optValue.value();
              result->mInternalRTPMediaLineIndex = Optional<decltype(result->mInternalRTPMediaLineIndex.mType)>(SafeInt<decltype(result->mInternalRTPMediaLineIndex.mType)>(value));
            }
          }
          return result;
        }

        adapter::RTCIceCandidate^ ToIceCandidateCx(const ISessionDescriptionTypes::ICECandidate &input)
        {
          auto result = ref new adapter::RTCIceCandidate;
          auto candidate = ZS_DYNAMIC_PTR_CAST(IICETypes::Candidate, input.mCandidate);
          if (NULL == candidate) return nullptr;

          result->Candidate = UseHelper::ToCx(input.getCandidateSDP());
          result->SdpMid = UseHelper::ToCx(input.mMid);
          if (input.mMLineIndex.hasValue()) {
            auto value = input.mMLineIndex.value();
            result->SdpMLineIndex = UseHelper::ToCx(Optional<WORD>(SafeInt<WORD>(value)));
          }
          result->Component = UseHelper::Convert(input.mComponent);
          result->InterfaceType = UseHelper::ToCx(candidate->mInterfaceType);
          result->Foundation = UseHelper::ToCx(candidate->mFoundation);
          result->Priority = SafeInt<decltype(result->Priority)>(candidate->mPriority);
          result->UnfreezePriority = SafeInt<decltype(result->UnfreezePriority)>(candidate->mUnfreezePriority);
          result->Protocol = UseHelper::Convert(candidate->mProtocol);
          result->Ip = UseHelper::ToCx(candidate->mIP);
          result->Port = SafeInt<decltype(result->Port)>(candidate->mPort);
          result->CandidateType = UseHelper::Convert(candidate->mCandidateType);
          result->TcpType = UseHelper::Convert(candidate->mTCPType);
          result->RelatedAddress = UseHelper::ToCx(candidate->mRelatedAddress);
          result->RelatedPort = SafeInt<decltype(result->RelatedPort)>(candidate->mRelatedPort);
          return result;
        }
        adapter::RTCIceCandidate^ ToIceCandidateCx(ISessionDescriptionTypes::ICECandidatePtr input)
        {
          if (!input) return nullptr;
          return ToIceCandidateCx(*input);
        }
        ISessionDescriptionTypes::ICECandidatePtr FromCx(adapter::RTCIceCandidate^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::ICECandidatePtr();
          auto result(make_shared<ISessionDescriptionTypes::ICECandidate>());
          auto candidate = make_shared<IICETypes::Candidate>();
          result->mCandidate = candidate;
          result->mMid = UseHelper::FromCx(input->SdpMid);
          if (nullptr != input->SdpMLineIndex)
          {
            result->mMLineIndex = UseHelper::FromCx(input->SdpMLineIndex);
          }
          result->mComponent = UseHelper::Convert(input->Component);
          candidate->mInterfaceType = UseHelper::FromCx(input->InterfaceType);
          candidate->mFoundation = UseHelper::FromCx(input->Foundation);
          candidate->mPriority = SafeInt<decltype(candidate->mPriority)>(input->Priority);
          candidate->mUnfreezePriority = SafeInt<decltype(candidate->mUnfreezePriority)>(input->UnfreezePriority);
          candidate->mProtocol = UseHelper::Convert(input->Protocol);
          candidate->mIP = UseHelper::FromCx(input->Ip);
          candidate->mPort = SafeInt<decltype(candidate->mPort)>(input->Port);
          candidate->mCandidateType = UseHelper::Convert(input->CandidateType);
          candidate->mTCPType = UseHelper::Convert(input->TcpType);
          candidate->mRelatedAddress = UseHelper::FromCx(input->RelatedAddress);
          candidate->mRelatedPort = SafeInt<decltype(candidate->mRelatedPort)>(input->RelatedPort);
          return result;
        }

        adapter::RTCIceCandidateComplete^ ToIceCandidateCompleteCx(const ISessionDescriptionTypes::ICECandidate &input)
        {
          auto result = ref new adapter::RTCIceCandidateComplete;
          auto candidate = ZS_DYNAMIC_PTR_CAST(IICETypes::CandidateComplete, input.mCandidate);
          if (NULL == candidate) return nullptr;

          result->Candidate = UseHelper::ToCx(input.getCandidateSDP());
          result->SdpMid = UseHelper::ToCx(input.mMid);
          if (input.mMLineIndex.hasValue()) {
            auto value = input.mMLineIndex.value();
            result->SdpMLineIndex = UseHelper::ToCx(Optional<WORD>(SafeInt<WORD>(value)));
          }
          result->Component = UseHelper::Convert(input.mComponent);
          result->Complete = candidate->mComplete;
          return result;
        }
        adapter::RTCIceCandidateComplete^ ToIceCandidateCompleteCx(ISessionDescriptionTypes::ICECandidatePtr input)
        {
          if (!input) return nullptr;
          return ToIceCandidateCompleteCx(*input);
        }
        ISessionDescriptionTypes::ICECandidatePtr FromCx(adapter::RTCIceCandidateComplete^ input)
        {
          if (nullptr == input) return ISessionDescriptionTypes::ICECandidatePtr();
          auto result(make_shared<ISessionDescriptionTypes::ICECandidate>());
          auto candidate = make_shared<IICETypes::CandidateComplete>();
          result->mCandidate = candidate;
          result->mMid = UseHelper::FromCx(input->SdpMid);
          if (nullptr != input->SdpMLineIndex)
          {
            result->mMLineIndex = UseHelper::FromCx(input->SdpMLineIndex);
          }
          result->mComponent = UseHelper::Convert(input->Component);
          candidate->mComplete = input->Complete;
          return result;
        }

#pragma endregion

      }

#pragma region RTCIceCandidate

      Platform::String^ RTCIceCandidate::ToJsonString()
      {
        auto nativeCandidate = internal::FromCx(this);
        return UseHelper::ToCx(UseServicesHelper::toString(nativeCandidate->toJSON()));
      }
      RTCIceCandidate^ RTCIceCandidate::FromJsonString(Platform::String^ jsonString)
      {
        auto candidateEl = UseServicesHelper::toJSON(UseHelper::FromCx(jsonString).c_str());
        auto nativeCandidate = ISessionDescription::ICECandidate::create(candidateEl);
        return internal::ToIceCandidateCx(nativeCandidate);
      }
      Platform::String^ RTCIceCandidate::ToSdpString()
      {
        auto nativeCandidate = internal::FromCx(this);
        return UseHelper::ToCx(UseServicesHelper::toString(nativeCandidate->toSDP()));
      }
      RTCIceCandidate^ RTCIceCandidate::FromSdpString(Platform::String^ sdpString)
      {
        auto nativeCandidate = ISessionDescription::ICECandidate::createFromSDP(UseHelper::FromCx(sdpString).c_str());
        return internal::ToIceCandidateCx(nativeCandidate);
      }

#pragma endregion

#pragma region RTCIceCandidateComplete

      Platform::String^ RTCIceCandidateComplete::ToJsonString()
      {
        auto nativeCandidate = internal::FromCx(this);
        return UseHelper::ToCx(UseServicesHelper::toString(nativeCandidate->toJSON()));
      }
      RTCIceCandidateComplete^ RTCIceCandidateComplete::FromJsonString(Platform::String^ jsonString)
      {
        auto candidateEl = UseServicesHelper::toJSON(UseHelper::FromCx(jsonString).c_str());
        auto nativeCandidate = ISessionDescription::ICECandidate::create(candidateEl);
        return internal::ToIceCandidateCompleteCx(nativeCandidate);
      }
      Platform::String^ RTCIceCandidateComplete::ToSdpString()
      {
        auto nativeCandidate = internal::FromCx(this);
        return UseHelper::ToCx(UseServicesHelper::toString(nativeCandidate->toSDP()));
      }
      RTCIceCandidateComplete^ RTCIceCandidateComplete::FromSdpString(Platform::String^ sdpString)
      {
        auto nativeCandidate = ISessionDescription::ICECandidate::createFromSDP(UseHelper::FromCx(sdpString).c_str());
        return internal::ToIceCandidateCompleteCx(nativeCandidate);
      }

#pragma endregion

#pragma region RTCSessionDescription

      RTCSessionDescription::RTCSessionDescription(ISessionDescriptionPtr nativeSession) :
        _nativePointer(nativeSession)
      {
      }

      RTCSessionDescription::RTCSessionDescription(
        RTCSessionDescriptionSignalingType type,
        Platform::String^ description
        )
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == description);

        try
        {
          _nativePointer = ISessionDescription::create(UseHelper::Convert(type), UseHelper::FromCx(description).c_str());
        }
        catch (const InvalidParameters &)
        {
          ORG_ORTC_THROW_INVALID_PARAMETERS()
        }
        catch (const InvalidStateError &e)
        {
          ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
        }
      }

      RTCSessionDescription::RTCSessionDescription(
        RTCSessionDescriptionSignalingType type,
        RTCSessionDescriptionDescription^ description
        )
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == description);

        try
        {
          _nativePointer = ISessionDescription::create(UseHelper::Convert(type), *internal::FromCx(description));
        }
        catch (const InvalidParameters &)
        {
          ORG_ORTC_THROW_INVALID_PARAMETERS()
        }
        catch (const InvalidStateError &e)
        {
          ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
        }
      }

      RTCRtpParameters^ RTCSessionDescription::ConvertCapabilitiesToParameters(RTCRtpCapabilities^ capabilitites)
      {
        if (nullptr == capabilitites) return nullptr;
        auto converted = UseAdapterHelper::capabilitiesToParameters(*ortc::internal::FromCx(capabilitites));
        return ortc::internal::ToCx(converted);
      }

      RTCRtpCapabilities^ RTCSessionDescription::ConvertParametersToCapabilitites(RTCRtpParameters^ parameters)
      {
        if (nullptr == parameters) return nullptr;
        auto converted = UseAdapterHelper::parametersToCapabilities(*ortc::internal::FromCx(parameters));
        return ortc::internal::ToCx(converted);
      }

      RTCSdpType RTCSessionDescription::Type::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        auto type = _nativePointer->type();
        ORG_ORTC_THROW_INVALID_STATE_IF(ISessionDescription::SignalingType_JSON == type);
        RTCSdpType result = RTCSdpType::SdpOffer;
        switch (type)
        {
        case ISessionDescription::SignalingType_JSON:         throw ref new Platform::NotImplementedException();
        case ISessionDescription::SignalingType_SDPOffer:     return RTCSdpType::SdpOffer;
        case ISessionDescription::SignalingType_SDPPreanswer: return RTCSdpType::SdpPreanswer;
        case ISessionDescription::SignalingType_SDPAnswer:    return RTCSdpType::SdpAnswer;
        case ISessionDescription::SignalingType_SDPRollback:  return RTCSdpType::SdpRollback;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCSessionDescriptionSignalingType RTCSessionDescription::SignalingType::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return UseHelper::Convert(_nativePointer->type());
      }

      Platform::Boolean RTCSessionDescription::IsJsonSignaling::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return ISessionDescription::SignalingType_JSON == _nativePointer->type();
      }

      Platform::Boolean RTCSessionDescription::IsSdpSignaling::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return ISessionDescription::SignalingType_JSON != _nativePointer->type();
      }

      RTCSessionDescriptionDescription^  RTCSessionDescription::Description::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return internal::ToCx(_nativePointer->description());
      }

      Platform::String^ RTCSessionDescription::FormattedDescription::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        return UseHelper::ToCx(_nativePointer->formattedDescription());
      }

      Platform::String^ RTCSessionDescription::Sdp::get()
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer);
        ORG_ORTC_THROW_INVALID_STATE_IF(!IsSdpSignaling);
        return UseHelper::ToCx(_nativePointer->formattedDescription());
      }

#pragma endregion

    } // namespace adapter
  } // namespace ortc
} // namespace org
