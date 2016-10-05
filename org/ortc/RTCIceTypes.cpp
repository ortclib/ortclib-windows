#include "pch.h"

#include <org/ortc/RTCIceTypes.h>
#include <org/ortc/helpers.h>

#include <ortc/services/IHelper.h>

#include <zsLib/SafeInt.h>

namespace org
{
  namespace ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::services::IHelper, UseServicesHelper);

    namespace internal
    {

#pragma region ICE types conversions

      RTCIceCandidate^ ToCx(const IICETypes::Candidate &input)
      {
        auto result = ref new RTCIceCandidate();

        result->InterfaceType = Helper::ToCx(input.mInterfaceType);
        result->Foundation = Helper::ToCx(input.mFoundation);
        result->Component = Helper::Convert(input.mComponent);
        result->Priority = SafeInt<uint32>(input.mPriority);
        result->UnfreezePriority = SafeInt<uint32>(input.mUnfreezePriority);
        result->Protocol = Helper::Convert(input.mProtocol);
        result->Ip = Helper::ToCx(input.mIP);
        result->Port = SafeInt<uint16>(input.mPort);
        result->CandidateType = Helper::Convert(input.mCandidateType);
        result->TcpType = Helper::Convert(input.mTCPType);
        result->RelatedAddress = Helper::ToCx(input.mRelatedAddress);
        result->RelatedPort = input.mRelatedPort;

        return result;
      }

      RTCIceCandidate^ ToCx(IICETypes::CandidatePtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICETypes::CandidatePtr FromCx(RTCIceCandidate^ input)
      {
        if (nullptr == input) return IICETypes::CandidatePtr();
        auto result = make_shared<IICETypes::Candidate>();

        result->mInterfaceType = Helper::FromCx(input->InterfaceType);
        result->mFoundation = Helper::FromCx(input->Foundation);
        result->mComponent = Helper::Convert(input->Component);
        result->mPriority = SafeInt<uint32>(input->Priority);
        result->mUnfreezePriority = SafeInt<uint32>(input->UnfreezePriority);
        result->mProtocol = Helper::Convert(input->Protocol);
        result->mIP = Helper::FromCx(input->Ip);
        result->mPort = SafeInt<uint16>(input->Port);
        result->mCandidateType = Helper::Convert(input->CandidateType);
        result->mTCPType = Helper::Convert(input->TcpType);
        result->mRelatedAddress = Helper::FromCx(input->RelatedAddress);
        result->mRelatedPort = input->RelatedPort;

        return result;
      }

      RTCIceCandidateComplete^ ToCx(const IICETypes::CandidateComplete &input)
      {
        auto result = ref new RTCIceCandidateComplete();
        result->Component = Helper::Convert(input.mComponent);
        result->Complete = input.mComplete;
        return result;
      }

      RTCIceCandidateComplete^ ToCx(IICETypes::CandidateCompletePtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICETypes::CandidateCompletePtr FromCx(RTCIceCandidateComplete^ input)
      {
        if (nullptr == input) return IICETypes::CandidateCompletePtr();
        auto result = make_shared<IICETypes::CandidateComplete>();
        result->mComponent = Helper::Convert(input->Component);
        result->mComplete = input->Complete;
        return result;
      }

      RTCIceParameters^ ToCx(const IICETypes::Parameters &input)
      {
        auto result = ref new RTCIceParameters();
        result->UseUnfreezePriority = input.mUseUnfreezePriority;
        result->UsernameFragment = Helper::ToCx(input.mUsernameFragment);
        result->Password = Helper::ToCx(input.mPassword);
        result->IceLite = input.mICELite;
        return result;
      }

      RTCIceParameters^ ToCx(IICETypes::ParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICETypes::ParametersPtr FromCx(RTCIceParameters^ input)
      {
        if (nullptr == input) return IICETypes::ParametersPtr();
        auto result = make_shared<IICETypes::Parameters>();
        result->mUseUnfreezePriority = input->UseUnfreezePriority;
        result->mUsernameFragment = Helper::FromCx(input->UsernameFragment);
        result->mPassword = Helper::FromCx(input->Password);
        result->mICELite = input->IceLite;
        return result;
      }

#pragma endregion

    } // namespace internal

#pragma region RTCIceParameters

    Platform::String^ RTCIceParameters::ToJsonString()
    {
      auto params = internal::FromCx(this);
      return UseHelper::ToCx(UseServicesHelper::toString(params->createElement("IceParameters")));

    }

    RTCIceParameters^ RTCIceParameters::FromJsonString(Platform::String^ jsonString)
    {
      return internal::ToCx(make_shared<IICETypes::Parameters>(IICETypes::Parameters::Parameters(UseServicesHelper::toJSON(UseHelper::FromCx(jsonString).c_str()))));
    }

#pragma endregion

#pragma region RTCIceCandidate

    Platform::String^ RTCIceCandidate::ToJsonString()
    {
      auto candidate = internal::FromCx(this);
      return UseHelper::ToCx(UseServicesHelper::toString(candidate->createElement("IceCandidate")));
    }

    RTCIceCandidate^ RTCIceCandidate::FromJsonString(Platform::String^ jsonString)
    {
      return internal::ToCx(make_shared<IICETypes::Candidate>(IICETypes::Candidate::Candidate(UseServicesHelper::toJSON(UseHelper::FromCx(jsonString).c_str()))));
    }

#pragma endregion

  } // namespace ortc
} // namespace org
