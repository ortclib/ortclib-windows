#include "pch.h"
#include "RTCIceTypes.h"
#include "helpers.h"

#include <openpeer/services/IHelper.h>

namespace org
{
  namespace ortc
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
      auto params = FromCx(this);
      return ToCx(openpeer::services::IHelper::toString(params->createElement("IceParameters")));

    }

    RTCIceParameters^ RTCIceParameters::FromJsonString(Platform::String^ jsonString)
    {
      return ToCx(make_shared<IICETypes::Parameters>(IICETypes::Parameters::Parameters(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str()))));
    }

    //---------------------------------------------------------------------------
    // RTCIceCandidate methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCIceCandidate::ToJsonString()
    {
      auto candidate = FromCx(this);
      return ToCx(openpeer::services::IHelper::toString(candidate->createElement("IceCandidate")));
    }

    RTCIceCandidate^ RTCIceCandidate::FromJsonString(Platform::String^ jsonString)
    {
      return ToCx(make_shared<IICETypes::Candidate>(IICETypes::Candidate::Candidate(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str()))));
    }
  } // namespace ortc
} // namespace org
