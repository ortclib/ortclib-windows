#include "pch.h"

#include "RTCIceTypes.h"
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
      RTCIceCandidate^ ToCx(const IICETypes::Candidate &input)
      {
        auto result = ref new RTCIceCandidate();

        result->InterfaceType = Helper::ToCx(input.mInterfaceType);
        result->Foundation = Helper::ToCx(input.mFoundation);
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
        result->mComplete = input->Complete;
        return result;
      }

      RTCIceParameters^ ToCx(const IICETypes::Parameters &input)
      {
        auto result = ref new RTCIceParameters();
        result->UseCandidateFreezePriority = input.mUseCandidateFreezePriority;
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
        result->mUseCandidateFreezePriority = input->UseCandidateFreezePriority;
        result->mUsernameFragment = Helper::FromCx(input->UsernameFragment);
        result->mPassword = Helper::FromCx(input->Password);
        result->mICELite = input->IceLite;
        return result;
      }

    }

    Platform::String^ RTCIceTypes::ToString()
    {
      throw ref new Platform::NotImplementedException();
    }

    Platform::String^ RTCIceTypes::ToString(RTCIceRole value)
    {
      return UseHelper::ToCx(IICETypes::toString(UseHelper::Convert(value)));
    }

    Platform::String^ RTCIceTypes::ToString(RTCIceComponent value)
    {
      return UseHelper::ToCx(IICETypes::toString(UseHelper::Convert(value)));
    }

    Platform::String^ RTCIceTypes::ToString(RTCIceProtocol value)
    {
      return UseHelper::ToCx(IICETypes::toString(UseHelper::Convert(value)));
    }

    Platform::String^ RTCIceTypes::ToString(RTCIceCandidateType value)
    {
      return UseHelper::ToCx(IICETypes::toString(UseHelper::Convert(value)));
    }

    Platform::String^ RTCIceTypes::ToString(RTCIceTcpCandidateType value)
    {
      return UseHelper::ToCx(IICETypes::toString(UseHelper::Convert(value)));
    }


    RTCIceRole RTCIceTypes::ToRole(Platform::String^ str)
    {
      return UseHelper::Convert(IICETypes::toRole(UseHelper::FromCx(str).c_str()));
    }

    RTCIceComponent RTCIceTypes::ToComponent(Platform::String^ str)
    {
      return UseHelper::Convert(IICETypes::toComponent(UseHelper::FromCx(str).c_str()));
    }

    RTCIceProtocol RTCIceTypes::ToProtocol(Platform::String^ str)
    {
      return UseHelper::Convert(IICETypes::toProtocol(UseHelper::FromCx(str).c_str()));
    }

    RTCIceCandidateType RTCIceTypes::ToCandidateType(Platform::String^ str)
    {
      return UseHelper::Convert(IICETypes::toCandidateType(UseHelper::FromCx(str).c_str()));
    }

    RTCIceTcpCandidateType RTCIceTypes::ToTcpCandidateType(Platform::String^ str)
    {
      return UseHelper::Convert(IICETypes::toTCPCandidateType(UseHelper::FromCx(str).c_str()));
    }

    //---------------------------------------------------------------------------
    // RTCIceParameters methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCIceParameters::ToJsonString()
    {
      auto params = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(params->createElement("IceParameters")));

    }

    RTCIceParameters^ RTCIceParameters::FromJsonString(Platform::String^ jsonString)
    {
      return internal::ToCx(make_shared<IICETypes::Parameters>(IICETypes::Parameters::Parameters(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str()))));
    }

    //---------------------------------------------------------------------------
    // RTCIceCandidate methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCIceCandidate::ToJsonString()
    {
      auto candidate = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(candidate->createElement("IceCandidate")));
    }

    RTCIceCandidate^ RTCIceCandidate::FromJsonString(Platform::String^ jsonString)
    {
      return internal::ToCx(make_shared<IICETypes::Candidate>(IICETypes::Candidate::Candidate(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str()))));
    }
  } // namespace ortc
} // namespace org
