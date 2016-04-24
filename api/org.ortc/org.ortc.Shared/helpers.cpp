#include "pch.h"

#include "helpers.h"
#include "MediaDevices.h"
#include "MediaStreamTrack.h"
#include "RTCDataChannel.h"
#include "RTCDtlsTransport.h"
#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RtpTypes.h"

#include "webrtc/base/win32.h"

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
#pragma region Basic types

      bool Helper::IsNullOrEmpty(Platform::String ^input)
      {
        if (nullptr == input) return true;
        if (input->IsEmpty()) return true;
        return false;
      }

      std::string Helper::FromCx(Platform::String^ inObj)
      {
        if (nullptr == inObj) return std::string();
        return rtc::ToUtf8(inObj->Data());
      }

      Platform::String^ Helper::ToCx(const std::string &inObj)
      {
        return ref new Platform::String(rtc::ToUtf16(inObj).c_str());
      }

      Platform::String^ Helper::ToCx(const char *str)
      {
        if (NULL == str) return nullptr;
        return ref new Platform::String(rtc::ToUtf16(str, strlen(str)).c_str());
      }

#pragma endregion

#pragma region Optional

      Platform::IBox<Platform::Boolean>^ Helper::ToCx(const Optional<bool> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<Platform::Boolean>(input.value());
      }

      Optional<bool> Helper::FromCx(Platform::IBox<Platform::Boolean>^ input)
      {
        Optional<bool> result;
        if (nullptr == input) return result;
        result = input->Value;
        return result;
      }

      Platform::IBox<uint8>^ Helper::ToCx(const Optional<BYTE> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<uint8>(SafeInt<uint8>(input.value()));
      }

      Optional<BYTE> Helper::FromCx(Platform::IBox<uint8>^ input)
      {
        Optional<BYTE> result;
        if (nullptr == input) return result;
        result = SafeInt<BYTE>(input->Value);
        return result;
      }

      Platform::IBox<int16>^ Helper::ToCx(const Optional<SHORT> &input)
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

      Platform::IBox<uint16>^ Helper::ToCx(const Optional<USHORT> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<uint16>(SafeInt<uint16>(input.value()));
      }

      Optional<USHORT> Helper::FromCx(Platform::IBox<uint16>^ input)
      {
        Optional<USHORT> result;
        if (nullptr == input) return result;
        result = SafeInt<USHORT>(input->Value);
        return result;
      }

      Platform::IBox<int32>^ Helper::ToCx(const Optional<LONG> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<int32>(SafeInt<int32>(input.value()));
      }

      Optional<LONG> Helper::FromCx(Platform::IBox<int32>^ input)
      {
        Optional<LONG> result;
        if (nullptr == input) return result;
        result = SafeInt<LONG>(input->Value);
        return result;
      }

      Platform::IBox<uint32>^ Helper::ToCx(const Optional<ULONG> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<uint32>(SafeInt<uint32>(input.value()));
      }

      Optional<ULONG> Helper::FromCx(Platform::IBox<uint32>^ input)
      {
        Optional<ULONG> result;
        if (nullptr == input) return result;
        result = SafeInt<ULONG>(input->Value);
        return result;
      }

      Platform::IBox<uint64>^ Helper::ToCx(const Optional<ULONGLONG> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<uint64>(SafeInt<uint64>(input.value()));
      }

      Optional<ULONGLONG> Helper::FromCx(Platform::IBox<uint64>^ input)
      {
        Optional<ULONGLONG> result;
        if (nullptr == input) return result;
        result = SafeInt<ULONGLONG>(input->Value);
        return result;
      }

      Platform::IBox<float64>^ Helper::ToCx(const Optional<double> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ref new Platform::Box<float64>(input.value());
      }

      Optional<double> Helper::FromCx(Platform::IBox<float64>^ input)
      {
        Optional<double> result;
        if (nullptr == input) return result;
        result = input->Value;
        return result;
      }

      Platform::String^ Helper::ToCx(const Optional<zsLib::String> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<zsLib::String> Helper::FromCxToOptional(Platform::String^ input)
      {
        Optional<zsLib::String> result;
        if (nullptr == input) return result;
        result = FromCx(input);
        return result;
      }

#pragma endregion

#pragma region Enum Conversion

      //---------------------------------------------------------------------------
      // MediaDeviceKind convert methods
      //---------------------------------------------------------------------------
      IMediaDevicesTypes::DeviceKinds Helper::Convert(MediaDeviceKind kind)
      {
        switch (kind) {
        case MediaDeviceKind::AudioInput:   return IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioInput;
        case MediaDeviceKind::AudioOutput:  return IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioOutput;
        case MediaDeviceKind::VideoInput:   return IMediaDevicesTypes::DeviceKinds::DeviceKind_VideoInput;
        }
        throw ref new Platform::NotImplementedException();
      }

      MediaDeviceKind Helper::Convert(IMediaDevicesTypes::DeviceKinds kind)
      {
        switch (kind) {
        case IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioInput:   return MediaDeviceKind::AudioInput;
        case IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioOutput:  return MediaDeviceKind::AudioOutput;
        case IMediaDevicesTypes::DeviceKinds::DeviceKind_VideoInput:   return MediaDeviceKind::VideoInput;
        }
        throw ref new Platform::NotImplementedException();
      }

      IICEGathererTypes::FilterPolicies Helper::Convert(RTCIceGatherFilterPolicy policy)
      {
        return static_cast<IICEGathererTypes::FilterPolicies>(zsLib::to_underlying(policy));
      }

      RTCIceGatherFilterPolicy Helper::Convert(IICEGathererTypes::FilterPolicies policy)
      {
        return static_cast<RTCIceGatherFilterPolicy>(zsLib::to_underlying(policy));
      }

      IICEGathererTypes::CredentialTypes Helper::Convert(RTCIceGathererCredentialType credentialType)
      {
        switch (credentialType) {
        case RTCIceGathererCredentialType::Password:    return IICEGathererTypes::CredentialTypes::CredentialType_Password;
        case RTCIceGathererCredentialType::Token:       return IICEGathererTypes::CredentialTypes::CredentialType_Token;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceGathererCredentialType Helper::Convert(IICEGathererTypes::CredentialTypes credentialType)
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
      IICEGathererTypes::States Helper::Convert(RTCIceGathererState state)
      {
        switch (state) {
        case RTCIceGathererState::New:          return IICEGathererTypes::States::State_New;
        case RTCIceGathererState::Gathering:    return IICEGathererTypes::States::State_Gathering;
        case RTCIceGathererState::Complete:     return IICEGathererTypes::States::State_Complete;
        case RTCIceGathererState::Closed:       return IICEGathererTypes::States::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceGathererState Helper::Convert(IICEGathererTypes::States state)
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
      IDataChannel::States Helper::Convert(RTCDataChannelState state)
      {
        switch (state) {
        case RTCDataChannelState::Connecting:   return IDataChannel::States::State_Connecting;
        case RTCDataChannelState::Open:         return IDataChannel::States::State_Open;
        case RTCDataChannelState::Closing:      return IDataChannel::States::State_Closing;
        case RTCDataChannelState::Closed:       return IDataChannel::States::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDataChannelState Helper::Convert(IDataChannel::States state)
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
      zsLib::Log::Level Helper::Convert(Log::Level level)
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

      const char *Helper::ToComponent(Log::Component  component)
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
      IDTLSTransport::States Helper::Convert(RTCDtlsTransportState state)
      {
        switch (state) {
        case RTCDtlsTransportState::New:            return IDTLSTransport::States::State_New;
        case RTCDtlsTransportState::Connecting:     return IDTLSTransport::States::State_Connecting;
        case RTCDtlsTransportState::Connected:      return IDTLSTransport::States::State_Connected;
        case RTCDtlsTransportState::Closed:         return IDTLSTransport::States::State_Closed;
        case RTCDtlsTransportState::Failed:         return IDTLSTransport::States::State_Failed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDtlsTransportState Helper::Convert(IDTLSTransport::States state)
      {
        switch (state) {
        case IDTLSTransport::States::State_New:           return RTCDtlsTransportState::New;
        case IDTLSTransport::States::State_Connecting:    return RTCDtlsTransportState::Connecting;
        case IDTLSTransport::States::State_Connected:     return RTCDtlsTransportState::Connected;
        case IDTLSTransport::States::State_Closed:        return RTCDtlsTransportState::Closed;
        case IDTLSTransport::States::State_Failed:        return RTCDtlsTransportState::Failed;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCDtlsRole convert methods
      //---------------------------------------------------------------------------
      IDTLSTransport::Roles Helper::Convert(RTCDtlsRole role)
      {
        switch (role) {
        case RTCDtlsRole::Auto:       return IDTLSTransport::Roles::Role_Auto;
        case RTCDtlsRole::Client:     return IDTLSTransport::Roles::Role_Client;
        case RTCDtlsRole::Server:     return IDTLSTransport::Roles::Role_Server;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDtlsRole Helper::Convert(IDTLSTransport::Roles role)
      {
        switch (role) {
        case IDTLSTransport::Roles::Role_Auto:      return RTCDtlsRole::Auto;
        case IDTLSTransport::Roles::Role_Client:    return RTCDtlsRole::Client;
        case IDTLSTransport::Roles::Role_Server:    return RTCDtlsRole::Server;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCIceRole convert methods
      //---------------------------------------------------------------------------
      IICETypes::Roles Helper::Convert(RTCIceRole role)
      {
        switch (role) {
        case RTCIceRole::Controlling:   return IICETypes::Roles::Role_Controlling;
        case RTCIceRole::Controlled:    return IICETypes::Roles::Role_Controlled;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceRole Helper::Convert(IICETypes::Roles role)
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
      IICETypes::Components Helper::Convert(RTCIceComponent component)
      {
        switch (component) {
        case RTCIceComponent::Rtp:   return IICETypes::Components::Component_RTP;
        case RTCIceComponent::Rtcp:  return IICETypes::Components::Component_RTCP;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceComponent Helper::Convert(IICETypes::Components component)
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
      IICETypes::Protocols Helper::Convert(RTCIceProtocol protocol)
      {
        switch (protocol) {
        case RTCIceProtocol::Udp:   return IICETypes::Protocols::Protocol_UDP;
        case RTCIceProtocol::Tcp:  return IICETypes::Protocols::Protocol_TCP;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceProtocol Helper::Convert(IICETypes::Protocols protocol)
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
      IICETypes::CandidateTypes Helper::Convert(RTCIceCandidateType candidateType)
      {
        switch (candidateType) {
        case RTCIceCandidateType::Host:   return IICETypes::CandidateTypes::CandidateType_Host;
        case RTCIceCandidateType::Srflex: return IICETypes::CandidateTypes::CandidateType_Srflex;
        case RTCIceCandidateType::Prflx:  return IICETypes::CandidateTypes::CandidateType_Prflx;
        case RTCIceCandidateType::Relay:  return IICETypes::CandidateTypes::CandidateType_Relay;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceCandidateType Helper::Convert(IICETypes::CandidateTypes candidateType)
      {
        switch (candidateType) {
        case IICETypes::CandidateTypes::CandidateType_Host:    return RTCIceCandidateType::Host;
        case IICETypes::CandidateTypes::CandidateType_Srflex:  return RTCIceCandidateType::Srflex;
        case IICETypes::CandidateTypes::CandidateType_Prflx:   return RTCIceCandidateType::Prflx;
        case IICETypes::CandidateTypes::CandidateType_Relay:   return RTCIceCandidateType::Relay;
        }
        throw ref new Platform::NotImplementedException();
      }

      IICETransportTypes::States Helper::Convert(RTCIceTransportState state)
      {
        switch (state) {
        case RTCIceTransportState::New:           return IICETransportTypes::State_New;
        case RTCIceTransportState::Checking:      return IICETransportTypes::State_Checking;
        case RTCIceTransportState::Connected:     return IICETransportTypes::State_Connected;
        case RTCIceTransportState::Completed:     return IICETransportTypes::State_Completed;
        case RTCIceTransportState::Disconnected:  return IICETransportTypes::State_Disconnected;
        case RTCIceTransportState::Failed:        return IICETransportTypes::State_Failed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceTransportState Helper::Convert(IICETransportTypes::States state)
      {
        switch (state) {
        case IICETransportTypes::State_New:           return RTCIceTransportState::New;
        case IICETransportTypes::State_Checking:      return RTCIceTransportState::Checking;
        case IICETransportTypes::State_Connected:     return RTCIceTransportState::Connected;
        case IICETransportTypes::State_Completed:     return RTCIceTransportState::Completed;
        case IICETransportTypes::State_Disconnected:  return RTCIceTransportState::Disconnected;
        case IICETransportTypes::State_Failed:        return RTCIceTransportState::Failed;
        }
        throw ref new Platform::NotImplementedException();
      }

      //---------------------------------------------------------------------------
      // RTCIceTcpCandidateType convert methods
      //---------------------------------------------------------------------------
      IICETypes::TCPCandidateTypes Helper::Convert(RTCIceTcpCandidateType candidateType)
      {
        switch (candidateType) {
        case RTCIceTcpCandidateType::Active:   return IICETypes::TCPCandidateTypes::TCPCandidateType_Active;
        case RTCIceTcpCandidateType::Passive:  return IICETypes::TCPCandidateTypes::TCPCandidateType_Passive;
        case RTCIceTcpCandidateType::So:       return IICETypes::TCPCandidateTypes::TCPCandidateType_SO;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCIceTcpCandidateType Helper::Convert(IICETypes::TCPCandidateTypes candidateType)
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
      IMediaStreamTrack::States Helper::Convert(MediaStreamTrackState state)
      {
        switch (state) {
        case MediaStreamTrackState::Live:   return IMediaStreamTrack::States::State_Live;
        case MediaStreamTrackState::Ended:  return IMediaStreamTrack::States::State_Ended;
        }
        throw ref new Platform::NotImplementedException();
      }

      MediaStreamTrackState Helper::Convert(IMediaStreamTrack::States state)
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
      IMediaStreamTrack::Kinds Helper::Convert(MediaStreamTrackKind kind)
      {
        switch (kind) {
        case MediaStreamTrackKind::Audio:   return IMediaStreamTrack::Kinds::Kind_Audio;
        case MediaStreamTrackKind::Video:   return IMediaStreamTrack::Kinds::Kind_Video;
        }
        throw ref new Platform::NotImplementedException();
      }

      MediaStreamTrackKind Helper::Convert(IMediaStreamTrack::Kinds kind)
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
      IRTPTypes::PriorityTypes Helper::Convert(RTCPriorityType priority)
      {
        switch (priority) {
        case RTCPriorityType::Very_Low:   return IRTPTypes::PriorityTypes::PriorityType_VeryLow;
        case RTCPriorityType::Low:        return IRTPTypes::PriorityTypes::PriorityType_Low;
        case RTCPriorityType::Medium:     return IRTPTypes::PriorityTypes::PriorityType_Medium;
        case RTCPriorityType::High:       return IRTPTypes::PriorityTypes::PriorityType_High;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCPriorityType Helper::Convert(IRTPTypes::PriorityTypes priority)
      {
        switch (priority) {
        case IRTPTypes::PriorityTypes::PriorityType_Unknown:  break;
        case IRTPTypes::PriorityTypes::PriorityType_VeryLow:  return RTCPriorityType::Very_Low;
        case IRTPTypes::PriorityTypes::PriorityType_Low:      return RTCPriorityType::Low;
        case IRTPTypes::PriorityTypes::PriorityType_Medium:   return RTCPriorityType::Medium;
        case IRTPTypes::PriorityTypes::PriorityType_High:     return RTCPriorityType::High;
        }
        throw ref new Platform::NotImplementedException();
      }

      IRTPTypes::DegradationPreferences Helper::Convert(RTCDegradationPreference preference)
      {
        switch (preference) {
        case RTCDegradationPreference::MaintainFramerate:    return IRTPTypes::DegradationPreferences::DegradationPreference_MaintainFramerate;
        case RTCDegradationPreference::MaintainResolution:   return IRTPTypes::DegradationPreferences::DegradationPreference_MaintainResolution;
        case RTCDegradationPreference::Balanced:             return IRTPTypes::DegradationPreferences::DegradationPreference_Balanced;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCDegradationPreference Helper::Convert(IRTPTypes::DegradationPreferences preference)
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
  } // namespace ortc
} // namespace org
