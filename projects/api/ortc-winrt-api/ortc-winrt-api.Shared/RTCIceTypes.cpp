#include "pch.h"
#include "RTCIceTypes.h"
#include "helpers.h"

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
} // namespace ortc_winrt_api