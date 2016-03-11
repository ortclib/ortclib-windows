#include "pch.h"

#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"
#include "helpers.h"
#include <ortc/ISettings.h>

namespace SafeIntInternal
{
  using ::DWORD;
}

#include <zsLib/SafeInt.h>

using namespace Platform;
using Platform::Collections::Vector;
using zsLib::Optional;
using namespace ortc;

namespace org
{
  namespace ortc
  {
    using zsLib::BYTE;
    using zsLib::SHORT;
    using zsLib::USHORT;
    using zsLib::LONG;
    using zsLib::ULONG;
    using zsLib::LONGLONG;
    using zsLib::ULONGLONG;

    namespace internal
    {
#pragma region Enum Conversion

      //---------------------------------------------------------------------------
      // MediaDeviceKind convert methods
      //---------------------------------------------------------------------------
      static IMediaDevicesTypes::DeviceKinds convert(MediaDeviceKind kind)
      {
        switch (kind) {
        case MediaDeviceKind::AudioInput:   return IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioInput;
        case MediaDeviceKind::AudioOutput:  return IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioOutput;
        case MediaDeviceKind::Video:        return IMediaDevicesTypes::DeviceKinds::DeviceKind_Video;
        }
        throw ref new Platform::NotImplementedException();
      }

      static MediaDeviceKind convert(IMediaDevicesTypes::DeviceKinds kind)
      {
        switch (kind) {
        case IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioInput:   return MediaDeviceKind::AudioInput;
        case IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioOutput:  return MediaDeviceKind::AudioOutput;
        case IMediaDevicesTypes::DeviceKinds::DeviceKind_Video:        return MediaDeviceKind::Video;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCIceGatherPolicy convert methods
      //---------------------------------------------------------------------------
      IICEGathererTypes::FilterPolicies ConvertEnums::convert(RTCIceGatherPolicy policy)
      {
        return static_cast<IICEGathererTypes::FilterPolicies>(zsLib::to_underlying(policy));
      }

      RTCIceGatherPolicy ConvertEnums::convert(IICEGathererTypes::FilterPolicies policy)
      {
        return static_cast<RTCIceGatherPolicy>(zsLib::to_underlying(policy));
      }

      //---------------------------------------------------------------------------
      // RTCIceGathererCredentialType convert methods
      //---------------------------------------------------------------------------
      IICEGathererTypes::CredentialTypes ConvertEnums::convert(RTCIceGathererCredentialType credentialType)
      {
        switch (credentialType) {
        case RTCIceGathererCredentialType::Password:    return IICEGathererTypes::CredentialTypes::CredentialType_Password;
        case RTCIceGathererCredentialType::Token:       return IICEGathererTypes::CredentialTypes::CredentialType_Token;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceGathererCredentialType ConvertEnums::convert(IICEGathererTypes::CredentialTypes credentialType)
      {
        switch (credentialType) {
        case IICEGathererTypes::CredentialTypes::CredentialType_Password:    return RTCIceGathererCredentialType::Password;
        case IICEGathererTypes::CredentialTypes::CredentialType_Token:       return RTCIceGathererCredentialType::Token;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCIceGathererState convert methods
      //---------------------------------------------------------------------------
      IICEGathererTypes::States ConvertEnums::convert(RTCIceGathererState state)
      {
        switch (state) {
        case RTCIceGathererState::New:          return IICEGathererTypes::States::State_New;
        case RTCIceGathererState::Gathering:    return IICEGathererTypes::States::State_Gathering;
        case RTCIceGathererState::Complete:     return IICEGathererTypes::States::State_Complete;
        case RTCIceGathererState::Closed:       return IICEGathererTypes::States::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceGathererState ConvertEnums::convert(IICEGathererTypes::States state)
      {
        switch (state) {
        case IICEGathererTypes::States::State_New:          return RTCIceGathererState::New;
        case IICEGathererTypes::States::State_Gathering:    return RTCIceGathererState::Gathering;
        case IICEGathererTypes::States::State_Complete:     return RTCIceGathererState::Complete;
        case IICEGathererTypes::States::State_Closed:       return RTCIceGathererState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCDataChannelState convert methods
      //---------------------------------------------------------------------------
      IDataChannel::States ConvertEnums::convert(RTCDataChannelState state)
      {
        switch (state) {
        case RTCDataChannelState::Connecting:   return IDataChannel::States::State_Connecting;
        case RTCDataChannelState::Open:         return IDataChannel::States::State_Open;
        case RTCDataChannelState::Closing:      return IDataChannel::States::State_Closing;
        case RTCDataChannelState::Closed:       return IDataChannel::States::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDataChannelState ConvertEnums::convert(IDataChannel::States state)
      {
        switch (state) {
        case IDataChannel::States::State_Connecting:   return RTCDataChannelState::Connecting;
        case IDataChannel::States::State_Open:         return RTCDataChannelState::Open;
        case IDataChannel::States::State_Closing:      return RTCDataChannelState::Closing;
        case IDataChannel::States::State_Closed:       return RTCDataChannelState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // Logger convert methods
      //---------------------------------------------------------------------------
      zsLib::Log::Level ConvertEnums::convert(Log::Level level)
      {
        switch (level) {
        case Log::Level::Basic:   return zsLib::Log::Basic;
        case Log::Level::Detail:  return zsLib::Log::Detail;
        case Log::Level::Debug:   return zsLib::Log::Debug;
        case Log::Level::Trace:   return zsLib::Log::Trace;
        case Log::Level::Insane:  return zsLib::Log::Insane;
        }
        throw ref new Platform::NotImplementedException();
      }

      const char * ConvertEnums::toComponent(Log::Component  component)
      {
        switch (component) {
        case Log::Component::ZsLib:         return "zsLib";
        case Log::Component::ZsLibSocket:   return "zsLib_socket";
        case Log::Component::Services:      return "openpeer_services";
        case Log::Component::ServicesTurn:  return "openpeer_services_turn";
        case Log::Component::ServicesHttp:  return "openpeer_services_http";
        case Log::Component::OrtcLib:       return "ortclib";
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCDtlsTransportState convert methods
      //---------------------------------------------------------------------------
      IDtlsTransport::States ConvertEnums::convert(RTCDtlsTransportState state)
      {
        switch (state) {
        case RTCDtlsTransportState::New:            return IDtlsTransport::States::State_New;
        case RTCDtlsTransportState::Connecting:     return IDtlsTransport::States::State_Connecting;
        case RTCDtlsTransportState::Connected:      return IDtlsTransport::States::State_Connected;
        case RTCDtlsTransportState::Validated:      return IDtlsTransport::States::State_Validated;
        case RTCDtlsTransportState::Closed:         return IDtlsTransport::States::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDtlsTransportState ConvertEnums::convert(IDtlsTransport::States state)
      {
        switch (state) {
        case IDtlsTransport::States::State_New:           return RTCDtlsTransportState::New;
        case IDtlsTransport::States::State_Connecting:    return RTCDtlsTransportState::Connecting;
        case IDtlsTransport::States::State_Connected:     return RTCDtlsTransportState::Connected;
        case IDtlsTransport::States::State_Validated:     return RTCDtlsTransportState::Validated;
        case IDtlsTransport::States::State_Closed:        return RTCDtlsTransportState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCDtlsRole convert methods
      //---------------------------------------------------------------------------
      IDtlsTransport::Roles ConvertEnums::convert(RTCDtlsRole role)
      {
        switch (role) {
        case RTCDtlsRole::Auto:       return IDtlsTransport::Roles::Role_Auto;
        case RTCDtlsRole::Client:     return IDtlsTransport::Roles::Role_Client;
        case RTCDtlsRole::Server:     return IDtlsTransport::Roles::Role_Server;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDtlsRole ConvertEnums::convert(IDtlsTransport::Roles role)
      {
        switch (role) {
        case IDtlsTransport::Roles::Role_Auto:      return RTCDtlsRole::Auto;
        case IDtlsTransport::Roles::Role_Client:    return RTCDtlsRole::Client;
        case IDtlsTransport::Roles::Role_Server:    return RTCDtlsRole::Server;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCIceRole convert methods
      //---------------------------------------------------------------------------
      IICETypes::Roles ConvertEnums::convert(RTCIceRole role)
      {
        switch (role) {
        case RTCIceRole::Controlling:   return IICETypes::Roles::Role_Controlling;
        case RTCIceRole::Controlled:    return IICETypes::Roles::Role_Controlled;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceRole ConvertEnums::convert(IICETypes::Roles role)
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
      IICETypes::Components ConvertEnums::convert(RTCIceComponent component)
      {
        switch (component) {
        case RTCIceComponent::Rtp:   return IICETypes::Components::Component_RTP;
        case RTCIceComponent::Rtcp:  return IICETypes::Components::Component_RTCP;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceComponent ConvertEnums::convert(IICETypes::Components component)
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
      IICETypes::Protocols ConvertEnums::convert(RTCIceProtocol protocol)
      {
        switch (protocol) {
        case RTCIceProtocol::Udp:   return IICETypes::Protocols::Protocol_UDP;
        case RTCIceProtocol::Tcp:  return IICETypes::Protocols::Protocol_TCP;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceProtocol ConvertEnums::convert(IICETypes::Protocols protocol)
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
      IICETypes::CandidateTypes ConvertEnums::convert(RTCIceCandidateType candidateType)
      {
        switch (candidateType) {
        case RTCIceCandidateType::Host:   return IICETypes::CandidateTypes::CandidateType_Host;
        case RTCIceCandidateType::Srflex: return IICETypes::CandidateTypes::CandidateType_Srflex;
        case RTCIceCandidateType::Prflx:  return IICETypes::CandidateTypes::CandidateType_Prflx;
        case RTCIceCandidateType::Relay:  return IICETypes::CandidateTypes::CandidateType_Relay;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceCandidateType ConvertEnums::convert(IICETypes::CandidateTypes candidateType)
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
      IICETypes::TCPCandidateTypes ConvertEnums::convert(RTCIceTcpCandidateType candidateType)
      {
        switch (candidateType) {
        case RTCIceTcpCandidateType::Active:   return IICETypes::TCPCandidateTypes::TCPCandidateType_Active;
        case RTCIceTcpCandidateType::Passive:  return IICETypes::TCPCandidateTypes::TCPCandidateType_Passive;
        case RTCIceTcpCandidateType::So:       return IICETypes::TCPCandidateTypes::TCPCandidateType_SO;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceTcpCandidateType ConvertEnums::convert(IICETypes::TCPCandidateTypes candidateType)
      {
        switch (candidateType) {
        case IICETypes::TCPCandidateTypes::TCPCandidateType_Active:    return RTCIceTcpCandidateType::Active;
        case IICETypes::TCPCandidateTypes::TCPCandidateType_Passive:   return RTCIceTcpCandidateType::Passive;
        case IICETypes::TCPCandidateTypes::TCPCandidateType_SO:        return RTCIceTcpCandidateType::So;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // MediaStreamTrackState convert methods
      //---------------------------------------------------------------------------
      IMediaStreamTrack::States ConvertEnums::convert(MediaStreamTrackState state)
      {
        switch (state) {
        case MediaStreamTrackState::Live:   return IMediaStreamTrack::States::State_Live;
        case MediaStreamTrackState::Ended:  return IMediaStreamTrack::States::State_Ended;
        }
        throw ref new Platform::NotImplementedException();
      }

      MediaStreamTrackState ConvertEnums::convert(IMediaStreamTrack::States state)
      {
        switch (state) {
        case IMediaStreamTrack::States::State_Live:   return MediaStreamTrackState::Live;
        case IMediaStreamTrack::States::State_Ended:  return MediaStreamTrackState::Ended;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // MediaStreamTrackKind convert methods
      //---------------------------------------------------------------------------
      IMediaStreamTrack::Kinds ConvertEnums::convert(MediaStreamTrackKind kind)
      {
        switch (kind) {
        case MediaStreamTrackKind::Audio:   return IMediaStreamTrack::Kinds::Kind_Audio;
        case MediaStreamTrackKind::Video:   return IMediaStreamTrack::Kinds::Kind_Video;
        }
        throw ref new Platform::NotImplementedException();
      }

      MediaStreamTrackKind ConvertEnums::convert(IMediaStreamTrack::Kinds kind)
      {
        switch (kind) {
        case IMediaStreamTrack::Kinds::Kind_Audio:   return MediaStreamTrackKind::Audio;
        case IMediaStreamTrack::Kinds::Kind_Video:   return MediaStreamTrackKind::Video;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RtpTypes convert methods
      //---------------------------------------------------------------------------
      IRTPTypes::PriorityTypes ConvertEnums::convert(RTCPriorityType priority)
      {
        switch (priority) {
        case RTCPriorityType::Unknown:    return IRTPTypes::PriorityTypes::PriorityType_Unknown;
        case RTCPriorityType::Very_Low:   return IRTPTypes::PriorityTypes::PriorityType_VeryLow;
        case RTCPriorityType::Low:        return IRTPTypes::PriorityTypes::PriorityType_Low;
        case RTCPriorityType::Medium:     return IRTPTypes::PriorityTypes::PriorityType_Medium;
        case RTCPriorityType::High:       return IRTPTypes::PriorityTypes::PriorityType_High;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCPriorityType ConvertEnums::convert(IRTPTypes::PriorityTypes priority)
      {
        switch (priority) {
        case IRTPTypes::PriorityTypes::PriorityType_Unknown:  return RTCPriorityType::Unknown;
        case IRTPTypes::PriorityTypes::PriorityType_VeryLow:  return RTCPriorityType::Very_Low;
        case IRTPTypes::PriorityTypes::PriorityType_Low:      return RTCPriorityType::Low;
        case IRTPTypes::PriorityTypes::PriorityType_Medium:   return RTCPriorityType::Medium;
        case IRTPTypes::PriorityTypes::PriorityType_High:     return RTCPriorityType::High;
        }
        throw ref new Platform::NotImplementedException();
      }

      IRTPTypes::DegradationPreferences ConvertEnums::convert(RTCDegradationPreference preference)
      {
        switch (preference) {
        case RTCDegradationPreference::MaintainFramerate:    return IRTPTypes::DegradationPreferences::DegradationPreference_MaintainFramerate;
        case RTCDegradationPreference::MaintainResolution:   return IRTPTypes::DegradationPreferences::DegradationPreference_MaintainResolution;
        case RTCDegradationPreference::Balanced:             return IRTPTypes::DegradationPreferences::DegradationPreference_Balanced;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDegradationPreference ConvertEnums::convert(IRTPTypes::DegradationPreferences preference)
      {
        switch (preference) {
        case IRTPTypes::DegradationPreferences::DegradationPreference_MaintainFramerate:  return RTCDegradationPreference::MaintainFramerate;
        case IRTPTypes::DegradationPreferences::DegradationPreference_MaintainResolution: return RTCDegradationPreference::MaintainResolution;
        case IRTPTypes::DegradationPreferences::DegradationPreference_Balanced:           return RTCDegradationPreference::Balanced;
        }
        throw ref new Platform::NotImplementedException();
      }
#pragma endregion
    } // namespace internal

#pragma region Basic types

    std::string FromCx(Platform::String^ inObj) {
      if (nullptr == inObj) return std::string();
      return rtc::ToUtf8(inObj->Data());
    }

    Platform::String^ ToCx(const std::string &inObj) {
      return ref new Platform::String(rtc::ToUtf16(inObj).c_str());
    }

#pragma endregion

#pragma region Optional

    Platform::IBox<Platform::Boolean>^ ToCx(const Optional<bool> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<Platform::Boolean>(input.value());
    }

    Optional<bool> FromCx(Platform::IBox<Platform::Boolean>^ input)
    {
      Optional<bool> result;
      if (nullptr == input) return result;
      result = input->Value;
      return result;
    }

    Platform::IBox<uint8>^ ToCx(const Optional<BYTE> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<uint8>(SafeInt<uint8>(input.value()));
    }

    Optional<BYTE> FromCx(Platform::IBox<uint8>^ input)
    {
      Optional<BYTE> result;
      if (nullptr == input) return result;
      result = SafeInt<BYTE>(input->Value);
      return result;
    }

    Platform::IBox<int16>^ ToCx(const Optional<SHORT> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<int16>(SafeInt<int16>(input.value()));
    }

    Optional<SHORT> FromCx(Platform::IBox<int16>^ input)
    {
      Optional<SHORT> result;
      if (nullptr == input) return result;
      result = SafeInt<SHORT>(input->Value);
      return result;
    }

    Platform::IBox<uint16>^ ToCx(const Optional<USHORT> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<uint16>(SafeInt<uint16>(input.value()));
    }

    Optional<USHORT> FromCx(Platform::IBox<uint16>^ input)
    {
      Optional<USHORT> result;
      if (nullptr == input) return result;
      result = SafeInt<USHORT>(input->Value);
      return result;
    }

    Platform::IBox<int32>^ ToCx(const Optional<LONG> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<int32>(SafeInt<int32>(input.value()));
    }

    Optional<LONG> FromCx(Platform::IBox<int32>^ input)
    {
      Optional<LONG> result;
      if (nullptr == input) return result;
      result = SafeInt<LONG>(input->Value);
      return result;
    }

    Platform::IBox<uint32>^ ToCx(const Optional<ULONG> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<uint32>(SafeInt<uint32>(input.value()));
    }

    Optional<ULONG> FromCx(Platform::IBox<uint32>^ input)
    {
      Optional<ULONG> result;
      if (nullptr == input) return result;
      result = SafeInt<ULONG>(input->Value);
      return result;
    }

    Platform::IBox<uint64>^ ToCx(const Optional<ULONGLONG> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<uint64>(SafeInt<uint64>(input.value()));
    }

    Optional<ULONGLONG> FromCx(Platform::IBox<uint64>^ input)
    {
      Optional<ULONGLONG> result;
      if (nullptr == input) return result;
      result = SafeInt<ULONGLONG>(input->Value);
      return result;
    }

    Platform::IBox<float64>^ ToCx(const Optional<double> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ref new Platform::Box<float64>(input.value());
    }

    Optional<double> FromCx(Platform::IBox<float64>^ input)
    {
      Optional<double> result;
      if (nullptr == input) return result;
      result = input->Value;
      return result;
    }

    Platform::String^ ToCx(const Optional<zsLib::String> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<zsLib::String> FromCxToOptional(Platform::String^ input)
    {
      Optional<zsLib::String> result;
      if (nullptr == input) return result;
      result = FromCx(input);
      return result;
    }

#pragma endregion

#pragma region IceTypes

    RTCIceParameters^ ToCx(const IICETypes::Parameters &input)
    {
      auto result = ref new RTCIceParameters();
      result->UseCandidateFreezePriority = input.mUseCandidateFreezePriority;
      result->UsernameFragment = ToCx(input.mUsernameFragment);
      result->Password = ToCx(input.mPassword);
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
      result->mUsernameFragment = FromCx(input->UsernameFragment);
      result->mPassword = FromCx(input->Password);
      result->mICELite = input->IceLite;
      return result;
    }

    RTCIceCandidate^ ToCx(const IICETypes::Candidate &input)
    {
      auto result = ref new RTCIceCandidate();

      result->InterfaceType = ToCx(input.mInterfaceType);
      result->Foundation = ToCx(input.mFoundation);
      result->Priority = SafeInt<uint32>(input.mPriority);
      result->UnfreezePriority = SafeInt<uint32>(input.mUnfreezePriority);
      result->Protocol = internal::ConvertEnums::convert(input.mProtocol);
      result->Ip = ToCx(input.mIP);
      result->Port = SafeInt<uint16>(input.mPort);
      result->CandidateType = internal::ConvertEnums::convert(input.mCandidateType);
      result->TcpType = internal::ConvertEnums::convert(input.mTCPType);
      result->RelatedAddress = ToCx(input.mRelatedAddress);
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

      result->mInterfaceType = FromCx(input->InterfaceType);
      result->mFoundation = FromCx(input->Foundation);
      result->mPriority = SafeInt<uint32>(input->Priority);
      result->mUnfreezePriority = SafeInt<uint32>(input->UnfreezePriority);
      result->mProtocol = internal::ConvertEnums::convert(input->Protocol);
      result->mIP = FromCx(input->Ip);
      result->mPort = SafeInt<uint16>(input->Port);
      result->mCandidateType = internal::ConvertEnums::convert(input->CandidateType);
      result->mTCPType = internal::ConvertEnums::convert(input->TcpType);
      result->mRelatedAddress = FromCx(input->RelatedAddress);
      result->mRelatedPort = input->RelatedPort;

      return result;
    }

#pragma endregion

#pragma region IceGatherer

    RTCIceInterfacePolicy^ ToCx(const IICEGatherer::InterfacePolicy &input)
    {
      auto result = ref new RTCIceInterfacePolicy();
      result->InterfaceType = ToCx(input.mInterfaceType);
      result->GatherPolicy = internal::ConvertEnums::convert(input.mGatherPolicy);
      return result;
    }

    RTCIceInterfacePolicy^ ToCx(IICEGatherer::InterfacePolicyPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IICEGatherer::InterfacePolicyPtr FromCx(RTCIceInterfacePolicy^ input)
    {
      if (nullptr == input) return IICEGatherer::InterfacePolicyPtr();
      auto result = make_shared<IICEGatherer::InterfacePolicy>();
      result->mInterfaceType = FromCx(input->InterfaceType);
      result->mGatherPolicy = internal::ConvertEnums::convert(input->GatherPolicy);
      return result;
    }

    RTCIceServer^ ToCx(const IICEGatherer::Server &input)
    {
      auto result = ref new RTCIceServer();

      if (input.mURLs.size() > 0)
      {
        result->Urls = ref new Vector<Platform::String^>();
        for (auto iter = input.mURLs.begin(); iter != input.mURLs.end(); ++iter)
        {
          auto &value = (*iter);
          result->Urls->Append(ToCx(value));
        }
      }
      result->UserName = ToCx(input.mUserName);
      result->Credential = ToCx(input.mCredential);
      result->CredentialType = internal::ConvertEnums::convert(input.mCredentialType);
      return result;
    }

    RTCIceServer^ ToCx(IICEGatherer::ServerPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IICEGatherer::ServerPtr FromCx(RTCIceServer^ input)
    {
      if (nullptr == input) return IICEGatherer::ServerPtr();
      auto result = make_shared<IICEGatherer::Server>();
      if (input->Urls)
      {
        for (Platform::String^ value : input->Urls)
        {
          result->mURLs.push_front(FromCx(value));
        }
      }
      result->mUserName = FromCx(input->UserName);
      result->mCredential = FromCx(input->Credential);
      result->mCredentialType = internal::ConvertEnums::convert(input->CredentialType);
      return result;
    }

    RTCIceGatherOptions^ ToCx(const IICEGatherer::Options &input)
    {
      auto result = ref new RTCIceGatherOptions();
      result->ContinuousGathering = input.mContinuousGathering;
      if (input.mInterfacePolicies.size() > 0)
      {
        result->InterfacePolicies = ref new Vector<RTCIceInterfacePolicy^>();
        for (auto iter = input.mInterfacePolicies.begin(); iter != input.mInterfacePolicies.end(); ++iter)
        {
          auto &value = (*iter);
          result->InterfacePolicies->Append(ToCx(value));
        }
      }
      if (input.mICEServers.size() > 0)
      {
        result->IceServers = ref new Vector<RTCIceServer^>();
        for (auto iter = input.mICEServers.begin(); iter != input.mICEServers.end(); ++iter)
        {
          auto &value = (*iter);
          result->IceServers->Append(ToCx(value));
        }
      }
      return result;
    }

    RTCIceGatherOptions^ ToCx(IICEGatherer::OptionsPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IICEGatherer::OptionsPtr FromCx(RTCIceGatherOptions^ input)
    {
      if (nullptr == input) return IICEGatherer::OptionsPtr();
      auto result = make_shared<IICEGatherer::Options>();
      result->mContinuousGathering = input->ContinuousGathering;
      if (input->InterfacePolicies)
      {
        for (RTCIceInterfacePolicy^ value : input->InterfacePolicies)
        {
          if (nullptr == value) continue;
          result->mInterfacePolicies.push_front(*FromCx(value));
        }
      }
      if (input->IceServers)
      {
        for (RTCIceServer^ value : input->IceServers)
        {
          if (nullptr == value) continue;
          result->mICEServers.push_front(*FromCx(value));
        }
      }
      return result;
    }

#pragma endregion

#pragma region IceTransport

    RTCIceCandidatePair^ ToCx(const IICETransport::CandidatePair &input)
    {
      auto result = ref new RTCIceCandidatePair();
      result->Local = ToCx(input.mLocal);
      result->Remote = ToCx(input.mRemote);
      return result;
    }

    RTCIceCandidatePair^ ToCx(IICETransport::CandidatePairPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IICETransport::CandidatePairPtr FromCx(RTCIceCandidatePair^ input)
    {
      if (nullptr == input) return IICETransport::CandidatePairPtr();
      auto result = make_shared<IICETransport::CandidatePair>();
      result->mLocal = FromCx(input->Local);
      result->mRemote = FromCx(input->Remote);
      return result;
    }

#pragma endregion

#pragma region DtlsTransport

    RTCDtlsFingerprint^ ToCx(const ICertificateTypes::Fingerprint &input)
    {
      auto result = ref new RTCDtlsFingerprint();
      result->Algorithm = ToCx(input.mAlgorithm);
      result->Value = ToCx(input.mValue);
      return result;
    }

    RTCDtlsFingerprint^ ToCx(ICertificateTypes::FingerprintPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    ICertificateTypes::FingerprintPtr FromCx(RTCDtlsFingerprint^ input)
    {
      if (nullptr == input) return ICertificateTypes::FingerprintPtr();
      auto result = make_shared<ICertificateTypes::Fingerprint>();
      result->mAlgorithm = FromCx(input->Algorithm);
      result->mValue = FromCx(input->Value);
      return result;
    }

    RTCDtlsParameters^ ToCx(const IDTLSTransportTypes::Parameters input)
    {
      auto result = ref new RTCDtlsParameters();
      result->Role = internal::ConvertEnums::convert(input.mRole);
      if (input.mFingerprints.size() > 0)
      {
        result->Fingerprints = ref new Vector<RTCDtlsFingerprint^>();
        for (auto iter = input.mFingerprints.begin(); iter != input.mFingerprints.end(); ++iter)
        {
          auto &value = (*iter);
          result->Fingerprints->Append(ToCx(value));
        }
      }
      return result;
    }

    RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IDTLSTransportTypes::ParametersPtr FromCx(RTCDtlsParameters^ input)
    {
      if (nullptr == input) return IDTLSTransportTypes::ParametersPtr();
      auto result = make_shared<IDTLSTransportTypes::Parameters>();
      result->mRole = internal::ConvertEnums::convert(input->Role);
      if (input->Fingerprints)
      {
        for (RTCDtlsFingerprint^ value : input->Fingerprints)
        {
          if (nullptr == value) continue;
          result->mFingerprints.push_front(*FromCx(value));
        }
      }
      return result;
    }

#pragma endregion

#pragma region DataChannel

    RTCDataChannelParameters^ ToCx(const IDataChannelTypes::Parameters &input)
    {
      auto result = ref new RTCDataChannelParameters();

      result->Label = ToCx(input.mLabel);
      result->Ordered = input.mOrdered;
      if (Milliseconds() != input.mMaxPacketLifetime)
      {
        result->MaxPacketLifetime = input.mMaxPacketLifetime.count();
      }
      else
      {
        result->MaxPacketLifetime = 0;
      }
      result->MaxRetransmits = ToCx(input.mMaxRetransmits);
      result->Protocol = ToCx(input.mProtocol);
      result->Negotiated = input.mNegotiated;
      result->Id = ToCx(input.mID);

      return result;
    }

    RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr input)
    {
      if (!input) return nullptr;
      return ToCx(input);
    }

    IDataChannelTypes::ParametersPtr FromCx(RTCDataChannelParameters^ input)
    {
      if (nullptr == input) return IDataChannelTypes::ParametersPtr();
      auto result = std::make_shared<IDataChannelTypes::Parameters>();

      result->mLabel = FromCx(input->Label);
      result->mOrdered = input->Ordered;
      if (0 != input->MaxPacketLifetime) result->mMaxPacketLifetime = Milliseconds(SafeInt<Milliseconds::rep>(input->MaxPacketLifetime));
      result->mMaxRetransmits = FromCx(input->MaxRetransmits);
      result->mProtocol = FromCx(input->Protocol);
      result->mNegotiated = input->Negotiated;
      result->mID = FromCx(input->Id);

      return result;
    }

#pragma endregion
  
#pragma region Sctp Transport

    RTCSctpCapabilities^ ToCx(const ISCTPTransport::Capabilities &input)
    {
      auto result = ref new RTCSctpCapabilities();
      result->MaxMessageSize =  SafeInt<uint32>(input.mMaxMessageSize);
      result->MinPort = SafeInt<uint16>(input.mMinPort);
      result->MaxPort = SafeInt<uint16>(input.mMaxPort);
      result->MaxUsablePorts = SafeInt<uint16>(input.mMaxUsablePorts);
      result->MaxSessionsPerPort = SafeInt<uint16>(input.mMaxSessionsPerPort);
      return result;
    }

    RTCSctpCapabilities^ ToCx(ISCTPTransport::CapabilitiesPtr input)
    {
      if (!input) return nullptr;
      return ToCx(input);
    }

    ISCTPTransport::CapabilitiesPtr FromCx(RTCSctpCapabilities^ input)
    {
      if (nullptr == input) return ISCTPTransport::CapabilitiesPtr();
      auto result = std::make_shared<ISCTPTransport::Capabilities>();

      result->mMaxMessageSize = SafeInt<uint32>(input->MaxMessageSize);
      result->mMinPort = SafeInt<uint16>(input->MinPort);
      result->mMaxPort = SafeInt<uint16>(input->MaxPort);
      result->mMaxUsablePorts = SafeInt<uint16>(input->MaxUsablePorts);
      result->mMaxSessionsPerPort = SafeInt<uint16>(input->MaxSessionsPerPort);
      return result;
    }

#pragma endregion

#pragma region Capabilities

    CapabilityBool^ ToCx(const ICapabilities::CapabilityBool &input)
    {
      auto result = ref new CapabilityBool;
      if (input.size() < 1) return result;

      result->Values = ref new Vector<Platform::Boolean>();

      for (auto iter = input.begin(); iter != input.end(); ++iter)
      {
        auto &value = *iter;
        result->Values->Append(value);
      }
      return result;
    }

    CapabilityBool^ ToCx(const Optional<ICapabilities::CapabilityBool> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<ICapabilities::CapabilityBool> FromCx(CapabilityBool^ input)
    {
      Optional<ICapabilities::CapabilityBool> result;
      if (nullptr == input) return result;

      result = ICapabilities::CapabilityBool();
      if (input->Values->Size < 1) return result;

      for (Platform::Boolean value : input->Values)
      {
        result.value().insert(value);
      }
      return result;
    }

    CapabilityLong^ ToCx(const ICapabilities::CapabilityLong &input)
    {
      auto result = ref new CapabilityLong();
      result->Min = SafeInt<int32>(input.mMin);
      result->Max = SafeInt<int32>(input.mMax);
      return result;
    }

    CapabilityLong^ ToCx(const Optional<ICapabilities::CapabilityLong> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<ICapabilities::CapabilityLong> FromCx(CapabilityLong^ input)
    {
      Optional<ICapabilities::CapabilityLong> result;
      if (nullptr == input) return result;

      result = ICapabilities::CapabilityLong();
      result.value().mMin = SafeInt<decltype(ICapabilities::CapabilityLong::mMin)>(input->Min);
      result.value().mMax = SafeInt<decltype(ICapabilities::CapabilityLong::mMax)>(input->Max);
      return result;
    }

    CapabilityDouble^ ToCx(const ICapabilities::CapabilityDouble &input)
    {
      auto result = ref new CapabilityDouble();
      result->Min = static_cast<float64>(input.mMin);
      result->Max = static_cast<float64>(input.mMax);
      return result;
    }

    CapabilityDouble^ ToCx(const Optional<ICapabilities::CapabilityDouble> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<ICapabilities::CapabilityDouble> FromCx(CapabilityDouble^ input)
    {
      Optional<ICapabilities::CapabilityDouble> result;
      if (nullptr == input) return result;

      result = ICapabilities::CapabilityDouble();
      result.value().mMin = static_cast<decltype(ICapabilities::CapabilityDouble::mMin)>(input->Min);
      result.value().mMax = static_cast<decltype(ICapabilities::CapabilityDouble::mMax)>(input->Max);
      return result;
    }

    CapabilityString^ ToCx(const ICapabilities::CapabilityString &input)
    {
      auto result = ref new CapabilityString();
      if (input.size() < 1) return result;

      result->Values = ref new Vector<Platform::String^>();

      for (auto iter = input.begin(); iter != input.end(); ++iter)
      {
        auto &value = *iter;
        result->Values->Append(ToCx(value));
      }
      return result;
    }

    CapabilityString^ ToCx(const Optional<ICapabilities::CapabilityString> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<ICapabilities::CapabilityString> FromCx(CapabilityString^ input)
    {
      Optional<ICapabilities::CapabilityString> result;
      if (nullptr == input) return result;

      result = ICapabilities::CapabilityString();
      if (input->Values->Size < 1) return result;

      for (Platform::String^ value : input->Values)
      {
        result.value().insert(FromCx(value));
      }
      return result;
    }

#pragma endregion

#pragma region Constraints

    ConstrainBoolParameters^ ToCx(const IConstraints::ConstrainBoolParameters &input)
    {
      auto result = ref new ConstrainBoolParameters();
      result->Exact = ToCx(input.mExact);
      result->Ideal = ToCx(input.mIdeal);
      return result;
    }

    ConstrainBoolParameters^ ToCx(const Optional<IConstraints::ConstrainBoolParameters> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<IConstraints::ConstrainBoolParameters> FromCx(ConstrainBoolParameters^ input)
    {
      Optional<IConstraints::ConstrainBoolParameters> result;
      if (nullptr == input) return result;

      result = IConstraints::ConstrainBoolParameters();
      result.value().mExact = FromCx(input->Exact);
      result.value().mIdeal = FromCx(input->Ideal);
      return result;
    }

    ConstrainBool^ ToCx(const IConstraints::ConstrainBool &input)
    {
      auto result = ref new ConstrainBool();
      result->Value = ToCx(input.mValue);
      result->Parameters = ToCx(input.mParameters);
      return result;
    }

    IConstraints::ConstrainBool FromCx(ConstrainBool^ input)
    {
      IConstraints::ConstrainBool result;
      if (nullptr == input) return result;
      result.mValue = FromCx(input->Value);
      result.mParameters = FromCx(input->Parameters);
      return result;
    }

    ConstrainLongRange^ ToCx(const IConstraints::ConstrainLongRange &input)
    {
      auto result = ref new ConstrainLongRange();
      result->Max = ToCx(input.mMax);
      result->Min = ToCx(input.mMin);
      result->Exact = ToCx(input.mExact);
      result->Ideal = ToCx(input.mIdeal);
      return result;
    }

    ConstrainLongRange^ ToCx(const Optional<IConstraints::ConstrainLongRange> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<IConstraints::ConstrainLongRange> FromCx(ConstrainLongRange^ input)
    {
      Optional<IConstraints::ConstrainLongRange> result;
      if (nullptr == input) return result;

      result = IConstraints::ConstrainLongRange();
      result.value().mMax = FromCx(input->Max);
      result.value().mMin = FromCx(input->Min);
      result.value().mExact = FromCx(input->Exact);
      result.value().mIdeal = FromCx(input->Ideal);
      return result;
    }

    ConstrainLong^ ToCx(const IConstraints::ConstrainLong &input)
    {
      auto result = ref new ConstrainLong();
      result->Value = ToCx(input.mValue);
      result->Range = ToCx(input.mRange);
      return result;
    }

    IConstraints::ConstrainLong FromCx(ConstrainLong^ input)
    {
      IConstraints::ConstrainLong result;
      if (nullptr == input) return result;
      result.mValue = FromCx(input->Value);
      result.mRange = FromCx(input->Range);
      return result;
    }

    ConstrainDoubleRange^ ToCx(const IConstraints::ConstrainDoubleRange &input)
    {
      auto result = ref new ConstrainDoubleRange();
      result->Max = ToCx(input.mMax);
      result->Min = ToCx(input.mMin);
      result->Exact = ToCx(input.mExact);
      result->Ideal = ToCx(input.mIdeal);
      return result;
    }

    ConstrainDoubleRange^ ToCx(const Optional<IConstraints::ConstrainDoubleRange> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<IConstraints::ConstrainDoubleRange> FromCx(ConstrainDoubleRange^ input)
    {
      Optional<IConstraints::ConstrainDoubleRange> result;
      if (nullptr == input) return result;

      result = IConstraints::ConstrainDoubleRange();
      result.value().mMax = FromCx(input->Max);
      result.value().mMin = FromCx(input->Min);
      result.value().mExact = FromCx(input->Exact);
      result.value().mIdeal = FromCx(input->Ideal);
      return result;
    }

    ConstrainDouble^ ToCx(const IConstraints::ConstrainDouble &input)
    {
      auto result = ref new ConstrainDouble();
      result->Value = ToCx(input.mValue);
      result->Range = ToCx(input.mRange);
      return result;
    }

    IConstraints::ConstrainDouble FromCx(ConstrainDouble^ input)
    {
      IConstraints::ConstrainDouble result;
      if (nullptr == input) return result;
      result.mValue = FromCx(input->Value);
      result.mRange = FromCx(input->Range);
      return result;
    }

    IVector<Platform::String^>^ ToCx(const IConstraints::StringList &input)
    {
      if (input.size() < 1) return nullptr;
      auto result = ref new Vector<Platform::String^>();
      for (auto iter = input.begin(); iter != input.end(); ++iter)
      {
        auto &value = (*iter);
        result->Append(ToCx(value));
      }
      return result;
    }

    IVector<Platform::String^>^ ToCx(const Optional<IConstraints::StringList> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<IConstraints::StringList> FromCx(IVector<Platform::String^>^ input)
    {
      Optional<IConstraints::StringList> result;
      if (nullptr == input) return result;
      result = IConstraints::StringList();
      for (Platform::String^ value : input)
      {
        result.value().push_back(FromCx(value));
      }
      return result;
    }

    StringOrStringList^ ToCx(const IConstraints::StringOrStringList &input)
    {
      auto result = ref new StringOrStringList();
      result->Value = ToCx(input.mValue);
      result->Values = ToCx(input.mValues);
      return result;
    }

    Optional<IConstraints::StringOrStringList> FromCx(StringOrStringList^ input)
    {
      Optional<IConstraints::StringOrStringList> result;
      if (nullptr == input) return result;

      result = IConstraints::StringOrStringList();
      result.value().mValue = FromCxToOptional(input->Value);
      result.value().mValues = FromCx(input->Values);
      return result;
    }


    ConstrainStringParameters^ ToCx(const IConstraints::ConstrainStringParameters &input)
    {
      auto result = ref new ConstrainStringParameters();
      result->Exact = ToCx(input.mExact);
      result->Ideal = ToCx(input.mIdeal);
      return result;
    }

    ConstrainStringParameters^ ToCx(const Optional<IConstraints::ConstrainStringParameters> &input)
    {
      if (!input.hasValue()) return nullptr;
      return ToCx(input.value());
    }

    Optional<IConstraints::ConstrainStringParameters> FromCx(ConstrainStringParameters^ input)
    {
      Optional<IConstraints::ConstrainStringParameters> result;
      if (nullptr == input) return result;

      result = IConstraints::ConstrainStringParameters();
      result.value().mExact = FromCx(input->Exact);
      result.value().mIdeal = FromCx(input->Ideal);
      return result;
    }

    ConstrainString^ ToCx(const IConstraints::ConstrainString &input)
    {
      auto result = ref new ConstrainString();
      result->Value = ToCx(input.mValue);
      result->Parameters = ToCx(input.mParameters);
      return result;
    }

    IConstraints::ConstrainString FromCx(ConstrainString^ input)
    {
      IConstraints::ConstrainString result;
      if (nullptr == input) return result;
      result.mValue = FromCx(input->Value);
      result.mParameters = FromCx(input->Parameters);
      return result;
    }

#pragma endregion

#pragma region MediaStreamTrack

    MediaTrackCapabilities^ ToCx(const IMediaStreamTrackTypes::Capabilities &input)
    {
      MediaTrackCapabilities^ result = ref new MediaTrackCapabilities;
      result->Width = ToCx(input.mWidth);
      result->Height = ToCx(input.mHeight);
      result->AspectRatio = ToCx(input.mAspectRatio);
      result->FrameRate = ToCx(input.mFrameRate);
      result->FacingMode = ToCx(input.mFacingMode);
      result->Volume = ToCx(input.mVolume);
      result->SampleRate = ToCx(input.mSampleRate);
      result->SampleSize = ToCx(input.mSampleSize);
      result->EchoCancellation = ToCx(input.mEchoCancellation);
      result->Latency = ToCx(input.mLatency);

      result->DeviceId = ToCx(input.mDeviceID);
      result->GroupId = ToCx(input.mGroupID);
      return result;
    }

    MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaStreamTrackTypes::CapabilitiesPtr FromCx(MediaTrackCapabilities^ input)
    {
      if (nullptr == input) return IMediaStreamTrackTypes::CapabilitiesPtr();
      auto result(make_shared<IMediaStreamTrackTypes::Capabilities>());
      result->mWidth = FromCx(input->Width);
      result->mHeight = FromCx(input->Height);
      result->mAspectRatio = FromCx(input->AspectRatio);
      result->mFrameRate = FromCx(input->FrameRate);
      result->mFacingMode = FromCx(input->FacingMode);
      result->mVolume = FromCx(input->Volume);
      result->mSampleRate = FromCx(input->SampleRate);
      result->mSampleSize = FromCx(input->SampleSize);
      result->mEchoCancellation = FromCx(input->EchoCancellation);
      result->mLatency = FromCx(input->Latency);

      result->mDeviceID = FromCx(input->DeviceId);
      result->mGroupID = FromCx(input->GroupId);
      return result;
    }

    MediaTrackSettings^ ToCx(const IMediaStreamTrackTypes::Settings &input)
    {
      auto result = ref new MediaTrackSettings();
      result->Width = ToCx(input.mWidth);
      result->Height = ToCx(input.mHeight);
      result->AspectRatio = ToCx(input.mAspectRatio);
      result->FrameRate = ToCx(input.mFrameRate);
      result->FacingMode = ToCx(input.mFacingMode);
      result->Volume = ToCx(input.mVolume);
      result->SampleRate = ToCx(input.mSampleRate);
      result->SampleSize = ToCx(input.mSampleSize);
      result->EchoCancellation = ToCx(input.mEchoCancellation);
      result->Latency = ToCx(input.mLatency);
      result->DeviceId = ToCx(input.mDeviceID);
      result->GroupId = ToCx(input.mGroupID);
      return result;
    }

    MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaStreamTrackTypes::SettingsPtr FromCx(MediaTrackSettings^ input)
    {
      if (nullptr == input) return IMediaStreamTrackTypes::SettingsPtr();
      auto result(make_shared<IMediaStreamTrackTypes::Settings>());
      result->mWidth = FromCx(input->Width);
      result->mHeight = FromCx(input->Height);
      result->mAspectRatio = FromCx(input->AspectRatio);
      result->mFrameRate = FromCx(input->FrameRate);
      result->mFacingMode = FromCx(input->FacingMode);
      result->mVolume = FromCx(input->Volume);
      result->mSampleRate = FromCx(input->SampleRate);
      result->mSampleSize = FromCx(input->SampleSize);
      result->mEchoCancellation = FromCx(input->EchoCancellation);
      result->mLatency = FromCx(input->Latency);
      result->mDeviceID = FromCx(input->DeviceId);
      result->mGroupID = FromCx(input->GroupId);
      return result;
    }

    MediaTrackConstraintSet^ ToCx(const IMediaStreamTrackTypes::ConstraintSet &input)
    {
      auto result = ref new MediaTrackConstraintSet();
      result->Width = ToCx(input.mWidth);
      result->Height = ToCx(input.mHeight);
      result->AspectRatio = ToCx(input.mAspectRatio);
      result->FrameRate = ToCx(input.mFrameRate);
      result->FacingMode = ToCx(input.mFacingMode);
      result->Volume = ToCx(input.mVolume);
      result->SampleRate = ToCx(input.mSampleRate);
      result->SampleSize = ToCx(input.mSampleSize);
      result->EchoCancellation = ToCx(input.mEchoCancellation);
      result->Latency = ToCx(input.mLatency);
      result->DeviceId = ToCx(input.mDeviceID);
      result->GroupId = ToCx(input.mGroupID);
      return result;
    }

    MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaStreamTrackTypes::ConstraintSetPtr FromCx(MediaTrackConstraintSet^ input)
    {
      if (nullptr == input) return IMediaStreamTrackTypes::ConstraintSetPtr();
      auto result(make_shared<IMediaStreamTrackTypes::ConstraintSet>());
      result->mWidth = FromCx(input->Width);
      result->mHeight = FromCx(input->Height);
      result->mAspectRatio = FromCx(input->AspectRatio);
      result->mFrameRate = FromCx(input->FrameRate);
      result->mFacingMode = FromCx(input->FacingMode);
      result->mVolume = FromCx(input->Volume);
      result->mSampleRate = FromCx(input->SampleRate);
      result->mSampleSize = FromCx(input->SampleSize);
      result->mEchoCancellation = FromCx(input->EchoCancellation);
      result->mLatency = FromCx(input->Latency);
      result->mDeviceID = FromCx(input->DeviceId);
      result->mGroupID = FromCx(input->GroupId);
      return result;
    }

    MediaTrackConstraints^ ToCx(const IMediaStreamTrackTypes::TrackConstraints &input)
    {
      auto result = ref new MediaTrackConstraints();
      if (input.mAdvanced.size() < 1) return result;

      result->Advanced = ref new Vector<MediaTrackConstraintSet^>();
      for (auto iter = input.mAdvanced.begin(); iter != input.mAdvanced.end(); ++iter)
      {
        auto &value = *iter;
        result->Advanced->Append(ToCx(value));
      }
      return result;
    }

    MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaStreamTrackTypes::TrackConstraintsPtr FromCx(MediaTrackConstraints^ input)
    {
      if (nullptr == input) return IMediaStreamTrackTypes::TrackConstraintsPtr();
      auto result(make_shared<IMediaStreamTrackTypes::TrackConstraints>());
      if (nullptr == input->Advanced) return result;

      for (MediaTrackConstraintSet^ value : input->Advanced)
      {
        result->mAdvanced.push_back(FromCx(value));
      }
      return result;
    }

    Constraints^ ToCx(const IMediaStreamTrackTypes::Constraints &input)
    {
      auto result = ref new Constraints();
      result->Video = ToCx(input.mVideo);
      result->Audio = ToCx(input.mAudio);
      return result;
    }

    Constraints^ ToCx(IMediaStreamTrackTypes::ConstraintsPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaStreamTrackTypes::ConstraintsPtr FromCx(Constraints^ input)
    {
      if (nullptr == input) return IMediaStreamTrackTypes::ConstraintsPtr();
      auto result(make_shared<IMediaStreamTrackTypes::Constraints>());
      result->mVideo = FromCx(input->Video);
      result->mAudio = FromCx(input->Audio);
      return result;
    }

    SupportedConstraints^ ToCx(const IMediaDevices::SupportedConstraints &input)
    {
      auto result = ref new SupportedConstraints();
      result->Width = input.mWidth;
      result->Height = input.mHeight;
      result->AspectRatio = input.mAspectRatio;
      result->FrameRate = input.mFrameRate;
      result->FacingMode = input.mFacingMode;
      result->Volume = input.mVolume;
      result->SampleRate = input.mSampleRate;
      result->SampleSize = input.mSampleSize;
      result->EchoCancellation = input.mEchoCancellation;
      result->Latency = input.mLatency;
      result->DeviceId = input.mDeviceID;
      result->GroupId = input.mGroupID;
      return result;
    }

    SupportedConstraints^ ToCx(IMediaDevices::SupportedConstraintsPtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaDevices::SupportedConstraintsPtr FromCx(SupportedConstraints^ input)
    {
      if (nullptr == input) return IMediaDevices::SupportedConstraintsPtr();
      auto result(make_shared<IMediaDevices::SupportedConstraints>());
      result->mWidth = input->Width;
      result->mHeight = input->Height;
      result->mAspectRatio = input->AspectRatio;
      result->mFrameRate = input->FrameRate;
      result->mFacingMode = input->FacingMode;
      result->mVolume = input->Volume;
      result->mSampleRate = input->SampleRate;
      result->mSampleSize = input->SampleSize;
      result->mEchoCancellation = input->EchoCancellation;
      result->mLatency = input->Latency;
      result->mDeviceID = input->DeviceId;
      result->mGroupID = input->GroupId;
      return result;
    }

    MediaDeviceInfo^ ToCx(const IMediaDevices::Device &input)
    {
      auto result = ref new MediaDeviceInfo();

      result->Kind = internal::convert(input.mKind);

      result->Label = ToCx(input.mLabel);
      result->DeviceId = ToCx(input.mDeviceID);
      result->GroupId = ToCx(input.mGroupID);

      result->SupportedConstraints = ToCx(input.mSupportedConstraints);

      return result;
    }

    MediaDeviceInfo^ ToCx(IMediaDevices::DevicePtr input)
    {
      if (!input) return nullptr;
      return ToCx(*input);
    }

    IMediaDevices::DevicePtr FromCx(MediaDeviceInfo^ input)
    {
      if (nullptr == input) return IMediaDevices::DevicePtr();
      auto result(make_shared<IMediaDevices::Device>());
      result->mKind = internal::convert(input->Kind);

      result->mLabel = FromCx(input->Label);
      result->mDeviceID = FromCx(input->DeviceId);
      result->mGroupID = FromCx(input->GroupId);
      return result;
    }

#pragma endregion

#pragma region RtpTypes

    RTCRtcpFeedback^ ToCx(const IRTPTypes::RTCPFeedback &input)
    {
      auto result = ref new RTCRtcpFeedback();
      result->Type = ToCx(input.mType);
      result->Parameter = ToCx(input.mParameter);
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
      result.mParameter = FromCx(input->Parameter);
      result.mType = FromCx(input->Type);
      return result;
    }

    RTCRtpOpusCodecCapabilityOptions^ ToCx(const IRTPTypes::OpusCodecCapabilityOptions &input)
    {
      auto result = ref new RTCRtpOpusCodecCapabilityOptions();
      result->Complexity = input.mComplexity;
      result->Signal = input.mSignal;
      result->Application = input.mApplication;
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
      result->mComplexity = input->Complexity;
      result->mSignal = input->Signal;
      result->mApplication = input->Application;
      result->mPacketLossPerc = input->PacketLossPerc;
      result->mPredictionDisabled = input->PredictionDisabled;
      return result;
    }

    RTCRtpOpusCodecCapabilityParameters^ ToCx(const IRTPTypes::OpusCodecCapabilityParameters &input)
    {
      auto result = ref new RTCRtpOpusCodecCapabilityParameters();
      result->MaxPlaybackRate = ToCx(input.mMaxPlaybackRate);
      result->Ptime = ToCx(input.mPTime);
      result->MaxAverageBitrate = ToCx(input.mMaxAverageBitrate);
      result->Stereo = ToCx(input.mStereo);
      result->Cbr = ToCx(input.mCBR);
      result->UseInbandFec = ToCx(input.mUseInbandFEC);
      result->UseDtx = ToCx(input.mUseDTX);
      result->SpropMaxCaptureRate = ToCx(input.mSPropMaxCaptureRate);
      result->SpropStereo = ToCx(input.mSPropStereo);
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
      result->mMaxPlaybackRate = FromCx(input->MaxPlaybackRate);
      result->mPTime = FromCx(input->Ptime);
      result->mMaxAverageBitrate = FromCx(input->MaxAverageBitrate);
      result->mStereo = FromCx(input->Stereo);
      result->mCBR = FromCx(input->Cbr);
      result->mUseInbandFEC = FromCx(input->UseInbandFec);
      result->mUseDTX = FromCx(input->UseDtx);
      result->mSPropMaxCaptureRate = FromCx(input->SpropMaxCaptureRate);
      result->mSPropStereo = FromCx(input->SpropStereo);
      return result;
    }

    RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::VP8CodecCapabilityParameters &input)
    {
      auto result = ref new RTCRtpVp8CodecCapabilityParameters();
      result->MaxFt = ToCx(input.mMaxFT);
      result->MaxFs = ToCx(input.mMaxFS);
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
      result->mMaxFT = FromCx(input->MaxFt);
      result->mMaxFS = FromCx(input->MaxFs);
      return result;
    }

    RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::H264CodecCapabilityParameters &input)
    {
      auto result = ref new RTCRtpH264CodecCapabilityParameters();
      result->ProfileLevelId = ToCx(input.mProfileLevelID);

      if (input.mPacketizationModes.size() > 0)
      {
        result->PacketizationModes = ref new Vector<uint16>();
        for (auto iter = input.mPacketizationModes.begin(); iter != input.mPacketizationModes.end(); ++iter)
        {
          auto &value = (*iter);
          result->PacketizationModes->Append(value);
        }
      }

      result->MaxMbps = ToCx(input.mMaxMBPS);
      result->MaxSmbps = ToCx(input.mMaxSMBPS);
      result->MaxFs = ToCx(input.mMaxFS);
      result->MaxCpb = ToCx(input.mMaxCPB);
      result->MaxDpb = ToCx(input.mMaxDPB);
      result->MaxBr = ToCx(input.mMaxBR);
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
      result->mProfileLevelID = FromCx(input->ProfileLevelId);
      if (input->PacketizationModes)
      {
        for (uint16 value : input->PacketizationModes)
        {
          result->mPacketizationModes.push_back(value);
        }
      }
      result->mMaxMBPS = FromCx(input->MaxMbps);
      result->mMaxSMBPS = FromCx(input->MaxSmbps);
      result->mMaxFS = FromCx(input->MaxFs);
      result->mMaxCPB = FromCx(input->MaxCpb);
      result->mMaxDPB = FromCx(input->MaxDpb);
      result->mMaxBR = FromCx(input->MaxBr);
      return result;
    }

    RTCRtpOpusCodecParameters^ ToCx(const IRTPTypes::OpusCodecParameters &input)
    {
      auto result = ref new RTCRtpOpusCodecParameters();
      result->MaxPlaybackRate = ToCx(input.mMaxPlaybackRate);
      result->Ptime = ToCx(input.mPTime);
      result->MaxAverageBitrate = ToCx(input.mMaxAverageBitrate);
      result->Stereo = ToCx(input.mStereo);
      result->Cbr = ToCx(input.mCBR);
      result->UseInbandFec = ToCx(input.mUseInbandFEC);
      result->UseDtx = ToCx(input.mUseDTX);

      result->Complexity = ToCx(input.mComplexity);
      result->Signal = ToCx(zsLib::String(IRTPTypes::OpusCodecParameters::toString(input.mSignal)));
      result->Application = ToCx(zsLib::String(IRTPTypes::OpusCodecParameters::toString(input.mApplication)));
      result->PacketLossPerc = ToCx(input.mPacketLossPerc);
      result->PredictionDisabled = ToCx(input.mPredictionDisabled);

      result->SpropMaxCaptureRate = ToCx(input.mSPropMaxCaptureRate);
      result->SpropStereo = ToCx(input.mSPropStereo);
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
      result->mMaxPlaybackRate = FromCx(input->MaxPlaybackRate);
      result->mPTime = FromCx(input->Ptime);
      result->mMaxAverageBitrate = FromCx(input->MaxAverageBitrate);
      result->mStereo = FromCx(input->Stereo);
      result->mCBR = FromCx(input->Cbr);
      result->mUseInbandFEC = FromCx(input->UseInbandFec);
      result->mUseDTX = FromCx(input->UseDtx);

      result->mComplexity = FromCx(input->Complexity);
      result->mSignal = IRTPTypes::OpusCodecParameters::toSignal(FromCx(input->Signal).c_str());
      result->mApplication = IRTPTypes::OpusCodecParameters::toApplication(FromCx(input->Application).c_str());
      result->mPacketLossPerc = FromCx(input->PacketLossPerc);
      result->mPredictionDisabled = FromCx(input->PredictionDisabled);

      result->mSPropMaxCaptureRate = FromCx(input->SpropMaxCaptureRate);
      result->mSPropStereo = FromCx(input->SpropStereo);
      return result;
    }

    RTCRtpVp8CodecParameters^ ToCxParameters(const IRTPTypes::VP8CodecParameters &input)
    {
      auto result = ref new RTCRtpVp8CodecParameters();
      result->MaxFt = ToCx(input.mMaxFT);
      result->MaxFs = ToCx(input.mMaxFS);
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
      result->mMaxFT = FromCx(input->MaxFt);
      result->mMaxFS = FromCx(input->MaxFs);
      return result;
    }

    RTCRtpH264CodecParameters^ ToCxParameters(const IRTPTypes::H264CodecParameters &input)
    {
      auto result = ref new RTCRtpH264CodecParameters();
      result->ProfileLevelId = ToCx(input.mProfileLevelID);

      if (input.mPacketizationModes.size() > 0)
      {
        result->PacketizationModes = ref new Vector<uint16>();
        for (auto iter = input.mPacketizationModes.begin(); iter != input.mPacketizationModes.end(); ++iter)
        {
          auto &value = (*iter);
          result->PacketizationModes->Append(value);
        }
      }

      result->MaxMbps = ToCx(input.mMaxMBPS);
      result->MaxSmbps = ToCx(input.mMaxSMBPS);
      result->MaxFs = ToCx(input.mMaxFS);
      result->MaxCpb = ToCx(input.mMaxCPB);
      result->MaxDpb = ToCx(input.mMaxDPB);
      result->MaxBr = ToCx(input.mMaxBR);
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
      result->mProfileLevelID = FromCx(input->ProfileLevelId);
      if (input->PacketizationModes)
      {
        for (uint16 value : input->PacketizationModes)
        {
          result->mPacketizationModes.push_back(value);
        }
      }
      result->mMaxMBPS = FromCx(input->MaxMbps);
      result->mMaxSMBPS = FromCx(input->MaxSmbps);
      result->mMaxFS = FromCx(input->MaxFs);
      result->mMaxCPB = FromCx(input->MaxCpb);
      result->mMaxDPB = FromCx(input->MaxDpb);
      result->mMaxBR = FromCx(input->MaxBr);
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
      if (0 == input->RtxTime) return result;
      result->mRTXTime = zsLib::Milliseconds(SafeInt<Milliseconds::rep>(input->RtxTime));
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

      result->L = ToCx(input.mL);
      result->D = ToCx(input.mD);

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
      result->mL = FromCx(input->L);
      result->mD = FromCx(input->D);
      if (input->ToP)
      {
        result->mToP = static_cast<IRTPTypes::FlexFECCodecParameters::ToPs>(input->ToP->Value);
      }
      return result;
    }

    RTCRtpCodecCapability^ ToCx(const IRTPTypes::CodecCapability &input)
    {
      auto result = ref new RTCRtpCodecCapability();

      result->Name = ToCx(input.mName);
      result->Kind = ToCx(input.mKind);
      result->ClockRate = SafeInt<uint32>(input.mClockRate);
      result->PreferredPayloadType = SafeInt<uint8>(input.mPreferredPayloadType);
      result->Maxptime = SafeInt<uint32>(input.mMaxPTime);
      result->NumChannels = SafeInt<uint32>(input.mNumChannels);

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

      result->mName = FromCx(input->Name);
      result->mKind = FromCx(input->Kind);
      result->mClockRate = SafeInt<decltype(result->mClockRate)>(input->ClockRate);
      result->mPreferredPayloadType = SafeInt<decltype(result->mPreferredPayloadType)>(input->PreferredPayloadType);
      result->mMaxPTime = SafeInt<decltype(result->mMaxPTime)>(input->Maxptime);
      result->mNumChannels = SafeInt<decltype(result->mNumChannels)>(input->NumChannels);

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
      result->Kind = ToCx(input.mKind);
      result->Uri = ToCx(input.mURI);
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
      result->mKind = FromCx(input->Kind);
      result->mURI = FromCx(input->Uri);
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
          result->FecMechanisms->Append(ToCx(value));
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
          result->mFECMechanisms.push_back(FromCx(value));
        }
      }
      return result;
    }

    RTCRtcpParameters^ ToCx(const IRTPTypes::RTCPParameters &input)
    {
      auto result = ref new RTCRtcpParameters();
      result->Ssrc = SafeInt<uint32>(input.mSSRC);
      result->Cname = ToCx(input.mCName);
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
      result->mCName = FromCx(input->Cname);
      result->mReducedSize = input->ReducedSize;
      result->mMux = input->Mux;
      return result;
    }

    RTCRtpCodecParameters^ ToCx(const IRTPTypes::CodecParameters &input)
    {
      auto result = ref new RTCRtpCodecParameters();

      result->Name = ToCx(input.mName);
      result->PayloadType = SafeInt<uint8>(input.mPayloadType);
      result->ClockRate = ToCx(input.mClockRate);
      result->Maxptime = SafeInt<uint32>(input.mMaxPTime);
      result->NumChannels = SafeInt<uint32>(input.mNumChannels);

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

      result->mName = FromCx(input->Name);
      result->mPayloadType = SafeInt<decltype(result->mPayloadType)>(input->PayloadType);
      result->mClockRate = FromCx(input->ClockRate);
      result->mMaxPTime = SafeInt<decltype(result->mMaxPTime)>(input->Maxptime);
      result->mNumChannels = SafeInt<decltype(result->mNumChannels)>(input->NumChannels);

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
      result->Ssrc = ToCx(input.mSSRC);
      result->Mechanism = ToCx(input.mMechanism);
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
      result->mSSRC = FromCx(input->Ssrc);
      result->mMechanism = FromCx(input->Mechanism);
      return result;
    }

    RTCRtpRtxParameters^ ToCx(const IRTPTypes::RTXParameters &input)
    {
      auto result = ref new RTCRtpRtxParameters();
      result->Ssrc = ToCx(input.mSSRC);
      result->PayloadType = ToCx(input.mPayloadType);
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
      result->mSSRC = FromCx(input->Ssrc);
      result->mPayloadType = FromCx(input->PayloadType);
      return result;
    }

    RTCRtpEncodingParameters^ ToCx(const IRTPTypes::EncodingParameters &input)
    {
      auto result = ref new RTCRtpEncodingParameters();

      result->Ssrc = ToCx(input.mSSRC);
      result->CodecPayloadType = ToCx(input.mCodecPayloadType);
      if (input.mFEC.hasValue()) result->Fec = ToCx(input.mFEC.value());
      if (input.mRTX.hasValue()) result->Rtx = ToCx(input.mRTX.value());

      result->Priority = internal::ConvertEnums::convert(input.mPriority);

      result->MaxBitrate = SafeInt<uint64>(input.mMaxBitrate);
      result->MinQuality = input.mMinQuality;
      result->ResolutionScale = input.mResolutionScale;
      result->FramerateScale = input.mFramerateScale;
      result->Active = input.mActive;
      result->EncodingId = ToCx(input.mEncodingID);

      if (input.mDependencyEncodingIDs.size() > 0)
      {
        result->DependencyEncodingIds = ref new Vector<Platform::String^>();
        for (auto iter = input.mDependencyEncodingIDs.begin(); iter != input.mDependencyEncodingIDs.end(); ++iter)
        {
          auto &value = (*iter);
          result->DependencyEncodingIds->Append(ToCx(value));
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

      result->mSSRC = FromCx(input->Ssrc);
      result->mCodecPayloadType = FromCx(input->CodecPayloadType);
      if (input->Fec)
      {
        result->mFEC = *FromCx(input->Fec);
      }
      if (input->Rtx)
      {
        result->mRTX = *FromCx(input->Rtx);
      }
      result->mPriority = internal::ConvertEnums::convert(input->Priority);
      result->mMaxBitrate = SafeInt<decltype(result->mMaxBitrate)>(input->MaxBitrate);
      result->mMinQuality = input->MinQuality;
      result->mResolutionScale = input->ResolutionScale;
      result->mFramerateScale = input->FramerateScale;
      result->mEncodingID = FromCx(input->EncodingId);

      if (input->DependencyEncodingIds)
      {
        for (Platform::String^ value : input->DependencyEncodingIds)
        {
          result->mDependencyEncodingIDs.push_back(FromCx(value));
        }
      }

      return result;
    }

    RTCRtpHeaderExtensionParameters^ ToCx(const IRTPTypes::HeaderExtensionParameters &input)
    {
      auto result = ref new RTCRtpHeaderExtensionParameters();
      result->Uri = ToCx(input.mURI);
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
      result->mURI = FromCx(input->Uri);
      result->mID = SafeInt<decltype(result->mID)>(input->Id);
      result->mEncrypt = input->Encrypt;
      return result;
    }

    RTCRtpParameters^ ToCx(const IRTPTypes::Parameters &input)
    {
      auto result = ref new RTCRtpParameters();

      result->MuxId = ToCx(input.mMuxID);
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
      result->DegradationPreference = internal::ConvertEnums::convert(input.mDegredationPreference);

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

      result->mMuxID = FromCx(input->MuxId);

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
      result->mDegredationPreference = internal::ConvertEnums::convert(input->DegradationPreference);
      return result;
    }

#pragma endregion

#pragma region Class converters

    //***********************************************************************
    // ConvertObjectToCx class methods
    //***********************************************************************

    RTCIceGatherer^ ConvertObjectToCx::ToIceGatherer(IIceGathererPtr iceGatherer)
    {
      RTCIceGatherer^ result = ref new RTCIceGatherer();
      result->_NativePointer = iceGatherer;
      return result;
    }

    RTCIceTransport^ ConvertObjectToCx::ToIceTransport(IIceTransportPtr iceTransport)
    {
      RTCIceTransport^ result = ref new RTCIceTransport();
      result->mNativePointer = iceTransport;
      return result;
    }

    RTCCertificate^ ConvertObjectToCx::ToCertificate(ICertificatePtr certificate)
    {
      RTCCertificate^ result = ref new RTCCertificate();
      result->mNativePointer = certificate;
      return result;
    }

    RTCDtlsTransport^ ConvertObjectToCx::ToDtlsTransport(IDtlsTransportPtr dtlsTransport)
    {
      RTCDtlsTransport^ result = ref new RTCDtlsTransport();
      result->mNativePointer = dtlsTransport;
      return result;
    }

    RTCSctpTransport^ ConvertObjectToCx::ToSctpTransport(ISctpTransportPtr sctpTransport)
    {
      RTCSctpTransport^ result = ref new RTCSctpTransport();
      result->mNativePointer = sctpTransport;
      return result;
    }

    MediaStreamTrack^ ConvertObjectToCx::ToMediaStreamTrack(IMediaStreamTrackPtr mediaStreamTrackPtr)
    {
      auto result = ref new MediaStreamTrack();
      result->mNativePointer = mediaStreamTrackPtr;
      return result;
    }

#pragma endregion

  } // namespace ortc
} // namespace org
