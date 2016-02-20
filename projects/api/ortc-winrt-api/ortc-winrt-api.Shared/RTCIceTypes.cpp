#include "pch.h"
#include "RTCIceTypes.h"
#include "helpers.h"

#include <openpeer/services/IHelper.h>

namespace ortc_winrt_api
{

  Platform::String^ RTCIceTypes::ToString()
  {
    throw ref new Platform::NotImplementedException();
  }

  Platform::String^ RTCIceTypes::ToString(RTCIceRole value)
  {
    return ToCx(IICETypes::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCIceTypes::ToString(RTCIceComponent value)
  {
    return ToCx(IICETypes::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCIceTypes::ToString(RTCIceProtocol value)
  {
    return ToCx(IICETypes::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCIceTypes::ToString(RTCIceCandidateType value)
  {
    return ToCx(IICETypes::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCIceTypes::ToString(RTCIceTcpCandidateType value)
  {
    return ToCx(IICETypes::toString(internal::ConvertEnums::convert(value)));
  }


  RTCIceRole RTCIceTypes::ToRole(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IICETypes::toRole(FromCx(str).c_str()));
  }

  RTCIceComponent RTCIceTypes::ToComponent(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IICETypes::toComponent(FromCx(str).c_str()));
  }

  RTCIceProtocol RTCIceTypes::ToProtocol(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IICETypes::toProtocol(FromCx(str).c_str()));
  }

  RTCIceCandidateType RTCIceTypes::ToCandidateType(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IICETypes::toCandidateType(FromCx(str).c_str()));
  }

  RTCIceTcpCandidateType RTCIceTypes::ToTcpCandidateType(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IICETypes::toTCPCandidateType(FromCx(str).c_str()));
  }

  //---------------------------------------------------------------------------
  // RTCIceParameters methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCIceParameters::ToJsonString()
  {
    IICETypes::Parameters params = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(params.createElement("IceParameters")));

  }

  RTCIceParameters^ RTCIceParameters::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCIceParameters();
    
    auto params = make_shared<IICETypes::Parameters>(IICETypes::Parameters::Parameters(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret->UsernameFragment = ToCx(params->mUsernameFragment);
    ret->Password = ToCx(params->mPassword);

    return ret;
  }

  //---------------------------------------------------------------------------
  // RTCIceCandidate methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCIceCandidate::ToJsonString()
  {
    IICETypes::Candidate candidate = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(candidate.createElement("IceCandidate")));
  }

  RTCIceCandidate^ RTCIceCandidate::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCIceCandidate();

    auto candidate = make_shared<IICETypes::Candidate>(IICETypes::Candidate::Candidate(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret = ToCx(candidate);

    return ret;
  }
} // namespace ortc_winrt_api