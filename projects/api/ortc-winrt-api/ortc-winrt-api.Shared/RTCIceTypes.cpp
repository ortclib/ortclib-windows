#include "pch.h"
#include "RTCIceTypes.h"
#include "helpers.h"

using namespace ortc_winrt_api;

namespace internal
{
  //---------------------------------------------------------------------------
  // RTCIceRole convert methods
  //---------------------------------------------------------------------------
  static IICETypes::Roles convert(RTCIceRole role)
  {
    switch (role) {
    case RTCIceRole::Controlling:   return IICETypes::Roles::Role_Controlling;
    case RTCIceRole::Controlled:    return IICETypes::Roles::Role_Controlled;
    }
    throw ref new Platform::NotImplementedException();
  }

  static RTCIceRole convert(IICETypes::Roles role)
  {
    switch (role) {
    case IICETypes::Roles::Role_Controlling:   return RTCIceRole::Controlling;
    case IICETypes::Roles::Role_Controlled:    return RTCIceRole::Controlled;
    }
    throw ref new Platform::NotImplementedException();
  }

  //---------------------------------------------------------------------------
  // RTCIceComponent convert methods
  //---------------------------------------------------------------------------
  static IICETypes::Components convert(RTCIceComponent component)
  {
    switch (component) {
    case RTCIceComponent::Rtp:   return IICETypes::Components::Component_RTP;
    case RTCIceComponent::Rtcp:  return IICETypes::Components::Component_RTCP;
    }
    throw ref new Platform::NotImplementedException();
  }

  static RTCIceComponent convert(IICETypes::Components component)
  {
    switch (component) {
    case IICETypes::Components::Component_RTP:   return RTCIceComponent::Rtp;
    case IICETypes::Components::Component_RTCP:  return RTCIceComponent::Rtcp;
    }
    throw ref new Platform::NotImplementedException();
  }

  //---------------------------------------------------------------------------
  // RTCIceProtocol convert methods
  //---------------------------------------------------------------------------
  static IICETypes::Protocols convert(RTCIceProtocol protocol)
  {
    switch (protocol) {
    case RTCIceProtocol::Udp:   return IICETypes::Protocols::Protocol_UDP;
    case RTCIceProtocol::Tcp:  return IICETypes::Protocols::Protocol_TCP;
    }
    throw ref new Platform::NotImplementedException();
  }

  static RTCIceProtocol convert(IICETypes::Protocols protocol)
  {
    switch (protocol) {
    case IICETypes::Protocols::Protocol_UDP:   return RTCIceProtocol::Udp;
    case IICETypes::Protocols::Protocol_TCP:  return RTCIceProtocol::Tcp;
    }
    throw ref new Platform::NotImplementedException();
  }

  //---------------------------------------------------------------------------
  // RTCIceCandidateType convert methods
  //---------------------------------------------------------------------------
  static IICETypes::CandidateTypes convert(RTCIceCandidateType candidateType)
  {
    switch (candidateType) {
    case RTCIceCandidateType::Host:   return IICETypes::CandidateTypes::CandidateType_Host;
    case RTCIceCandidateType::Srflex: return IICETypes::CandidateTypes::CandidateType_Srflex;
    case RTCIceCandidateType::Prflx:  return IICETypes::CandidateTypes::CandidateType_Prflx;
    case RTCIceCandidateType::Relay:  return IICETypes::CandidateTypes::CandidateType_Relay;
    }
    throw ref new Platform::NotImplementedException();
  }

  static RTCIceCandidateType convert(IICETypes::CandidateTypes candidateType)
  {
    switch (candidateType) {
    case IICETypes::CandidateTypes::CandidateType_Host:    return RTCIceCandidateType::Host;
    case IICETypes::CandidateTypes::CandidateType_Srflex:  return RTCIceCandidateType::Srflex;
    case IICETypes::CandidateTypes::CandidateType_Prflx:   return RTCIceCandidateType::Prflx;
    case IICETypes::CandidateTypes::CandidateType_Relay:   return RTCIceCandidateType::Relay;
    }
    throw ref new Platform::NotImplementedException();
  }

  //---------------------------------------------------------------------------
  // RTCIceTcpCandidateType convert methods
  //---------------------------------------------------------------------------
  static IICETypes::TCPCandidateTypes convert(RTCIceTcpCandidateType candidateType)
  {
    switch (candidateType) {
    case RTCIceTcpCandidateType::Active:   return IICETypes::TCPCandidateTypes::TCPCandidateType_Active;
    case RTCIceTcpCandidateType::Passive:  return IICETypes::TCPCandidateTypes::TCPCandidateType_Passive;
    case RTCIceTcpCandidateType::So:       return IICETypes::TCPCandidateTypes::TCPCandidateType_SO;
    }
    throw ref new Platform::NotImplementedException();
  }

  static RTCIceTcpCandidateType convert(IICETypes::TCPCandidateTypes candidateType)
  {
    switch (candidateType) {
    case IICETypes::TCPCandidateTypes::TCPCandidateType_Active:    return RTCIceTcpCandidateType::Active;
    case IICETypes::TCPCandidateTypes::TCPCandidateType_Passive:   return RTCIceTcpCandidateType::Passive;
    case IICETypes::TCPCandidateTypes::TCPCandidateType_SO:        return RTCIceTcpCandidateType::So;
    }
    throw ref new Platform::NotImplementedException();
  }
} // namespace internal

Platform::String^ RTCIceTypes::ToString(RTCIceRole value)
{
  return ToCx(IICETypes::toString(internal::convert(value)));
}

Platform::String^ RTCIceTypes::ToString(RTCIceComponent value)
{
  return ToCx(IICETypes::toString(internal::convert(value)));
}

Platform::String^ RTCIceTypes::ToString(RTCIceProtocol value)
{
  return ToCx(IICETypes::toString(internal::convert(value)));
}

Platform::String^ RTCIceTypes::ToString(RTCIceCandidateType value)
{
  return ToCx(IICETypes::toString(internal::convert(value)));
}

Platform::String^ RTCIceTypes::ToString(RTCIceTcpCandidateType value)
{
  return ToCx(IICETypes::toString(internal::convert(value)));
}


RTCIceRole RTCIceTypes::ToRole(Platform::String^ str)
{
  return internal::convert(IICETypes::toRole(FromCx(str).c_str()));
}

RTCIceComponent RTCIceTypes::ToComponent(Platform::String^ str)
{
  return internal::convert(IICETypes::toComponent(FromCx(str).c_str()));
}

RTCIceProtocol RTCIceTypes::ToProtocol(Platform::String^ str)
{
  return internal::convert(IICETypes::toProtocol(FromCx(str).c_str()));
}

RTCIceCandidateType RTCIceTypes::ToCandidateType(Platform::String^ str)
{
  return internal::convert(IICETypes::toCandidateType(FromCx(str).c_str()));
}

RTCIceTcpCandidateType RTCIceTypes::ToTcpCandidateType(Platform::String^ str)
{
  return internal::convert(IICETypes::toTCPCandidateType(FromCx(str).c_str()));
}
