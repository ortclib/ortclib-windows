#include "pch.h"

#include "helpers.h"
#include "MediaDevices.h"
#include "MediaStreamTrack.h"
#include "RTCDataChannel.h"
#include "RTCDtlsTransport.h"
#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RtpTypes.h"
#include "RTCSctpTransport.h"
#include "RTCStatsReport.h"
#include "adapter/RTCSessionDescription.h"
#include "adapter/RTCPeerConnection.h"

#include "webrtc/base/win32.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/date.h>
#include <zsLib/SafeInt.h>

using namespace Platform;
using Platform::Collections::Vector;
using zsLib::Optional;
using namespace ortc;
using namespace ortc::adapter;
using namespace date;

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

    ZS_DECLARE_TYPEDEF_PTR(openpeer::services::IHelper, UseServicesHelper);

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

      Windows::Foundation::DateTime Helper::ToCx(const zsLib::Time &value)
      {
        Windows::Foundation::DateTime result {};
        auto t = day_point(jan / 1 / 1601);

        auto diff = value - t;
        auto nano = zsLib::toNanoseconds(diff);

        result.UniversalTime = SafeInt<decltype(result.UniversalTime)>(nano.count() / static_cast<zsLib::Nanoseconds::rep>(100));
        return result;
      }

      zsLib::Time Helper::FromCx(Windows::Foundation::DateTime value)
      {
        zsLib::Time t = day_point(jan / 1 / 1601);

        auto nano = zsLib::toMilliseconds(zsLib::Nanoseconds(static_cast<zsLib::Nanoseconds::rep>(value.UniversalTime) * static_cast<zsLib::Nanoseconds::rep>(100)));

        auto result = t + nano;

        return zsLib::timeSinceEpoch(result);
      }

      Platform::String^ Helper::ToCx(ElementPtr rootEl)
      {
        if (!rootEl) return nullptr;
        return ToCx(UseServicesHelper::toString(rootEl));
      }

      ElementPtr Helper::FromJsonCx(Platform::String^ rootStr)
      {
        if (!rootStr) return nullptr;
        return UseServicesHelper::toJSON(FromCx(rootStr->ToString()).c_str());
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

      //---------------------------------------------------------------------------
      // RTCSctpTransport convert methods
      //---------------------------------------------------------------------------
      ISCTPTransportTypes::States Helper::Convert(RTCSctpTransportState state)
      {
        switch (state) {
        case RTCSctpTransportState::New:        return ISCTPTransportTypes::States::State_New;
        case RTCSctpTransportState::Connecting: return ISCTPTransportTypes::States::State_Connecting;
        case RTCSctpTransportState::Connected:  return ISCTPTransportTypes::States::State_Connected;
        case RTCSctpTransportState::Closed:     return ISCTPTransportTypes::States::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCSctpTransportState Helper::Convert(ISCTPTransportTypes::States state)
      {
        switch (state) {
        case ISCTPTransportTypes::States::State_New:        return RTCSctpTransportState::New;
        case ISCTPTransportTypes::States::State_Connecting: return RTCSctpTransportState::Connecting;
        case ISCTPTransportTypes::States::State_Connected:  return RTCSctpTransportState::Connected;
        case ISCTPTransportTypes::States::State_Closed:     return RTCSctpTransportState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      //-----------------------------------------------------------------------
      // Logger convert methods
      //-----------------------------------------------------------------------
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
        case Log::Component::ZsLib:                         return "zsLib";
        case Log::Component::ZsLibSocket:                   return "zsLib_socket";
        case Log::Component::Services:                      return "openpeer_services";
        case Log::Component::ServicesDns:                   return "openpeer_services_dns";
        case Log::Component::ServicesHttp:                  return "openpeer_services_http";
        case Log::Component::ServicesWire:                  return "openpeer_services_wire";
        case Log::Component::ServicesStun:                  return "openpeer_services_stun";
        case Log::Component::ServicesTurn:                  return "openpeer_services_turn";
        case Log::Component::OrtcLib:                       return "ortclib";
        case Log::Component::OrtcLibWebrtc:                 return "ortclib_webrtc";
        case Log::Component::OrtcLibDtlsTransport:          return "ortclib_dtlstransport";
        case Log::Component::OrtcLibIceGatherer:            return "ortclib_icegatherer";
        case Log::Component::OrtcLibIceGathererRouter:      return "ortclib_icegatherer_router";
        case Log::Component::OrtcLibIceTransport:           return "ortclib_icetransport";
        case Log::Component::OrtcLibIceTransportController: return "ortclib_icetransport_controller";
        case Log::Component::OrtcLibMediaDevices:           return "ortclib_mediadevices";
        case Log::Component::OrtcLibMediaStreamTrack:       return "ortclib_mediastreamtrack";
        case Log::Component::OrtcLibRtpRtcpPacket:          return "ortclib_rtp_rtcp_packet";
        case Log::Component::OrtcLibRtpListener:            return "ortclib_rtplistener";
        case Log::Component::OrtcLibRtpMediaEngine:         return "ortclib_rtpmediaengine";
        case Log::Component::OrtcLibRtpReceiver:            return "ortclib_rtpreceiver";
        case Log::Component::OrtcLibRtpSender:              return "ortclib_rtpsender";
        case Log::Component::OrtcLibRtpTypes:               return "ortclib_rtptypes";
        case Log::Component::OrtcLibSctpDataChannel:        return "ortclib_sctp_datachannel";
        case Log::Component::OrtcLibSrtp:                   return "ortclib_srtp";
        case Log::Component::OrtcLibStats:                  return "ortclib_stats";
        case Log::Component::OrtcLibAdapter:                return "ortclib_adapter";
        }
        throw ref new Platform::NotImplementedException();
      }
      
      
      //-----------------------------------------------------------------------
      // Stats convert methods
      //-----------------------------------------------------------------------

      RTCStatsType Helper::Convert(const Optional<IStatsReportTypes::StatsTypes> &type)
      {
        if (!type.hasValue()) return RTCStatsType::Unknown;
        return Convert(type.value());
      }

      RTCStatsType Helper::Convert(IStatsReportTypes::StatsTypes type)
      {
        switch (type) {
        case IStatsReportTypes::StatsType_InboundRTP:         return RTCStatsType::InboundRtp;
        case IStatsReportTypes::StatsType_OutboundRTP:        return RTCStatsType::OutboundRtp;
        case IStatsReportTypes::StatsType_Codec:              return RTCStatsType::Codec;
        case IStatsReportTypes::StatsType_SCTPTransport:      return RTCStatsType::SctpTransport;
        case IStatsReportTypes::StatsType_DataChannel:        return RTCStatsType::DataChannel;
        case IStatsReportTypes::StatsType_Stream:             return RTCStatsType::Stream;
        case IStatsReportTypes::StatsType_Track:              return RTCStatsType::Track;
        case IStatsReportTypes::StatsType_ICEGatherer:        return RTCStatsType::IceGatherer;
        case IStatsReportTypes::StatsType_ICETransport:       return RTCStatsType::IceTransport;
        case IStatsReportTypes::StatsType_DTLSTransport:      return RTCStatsType::DtlsTransport;
        case IStatsReportTypes::StatsType_SRTPTransport:      return RTCStatsType::SrtpTransport;
        case IStatsReportTypes::StatsType_Certificate:        return RTCStatsType::Certificate;
        case IStatsReportTypes::StatsType_Candidate:          return RTCStatsType::Candidate;
        case IStatsReportTypes::StatsType_CandidatePair:      return RTCStatsType::CandidatePair;
        case IStatsReportTypes::StatsType_LocalCandidate:     return RTCStatsType::LocalCandidate;
        case IStatsReportTypes::StatsType_RemoteCandidate:    return RTCStatsType::RemoteCandidate;
        }
        throw ref new Platform::NotImplementedException();
      }

      Optional<IStatsReportTypes::StatsTypes> Helper::Convert(RTCStatsType type)
      {
        switch (type) {
        case RTCStatsType::Unknown:           return Optional<IStatsReportTypes::StatsTypes>();
        case RTCStatsType::InboundRtp:        return IStatsReportTypes::StatsType_InboundRTP;
        case RTCStatsType::OutboundRtp:       return IStatsReportTypes::StatsType_OutboundRTP;
        case RTCStatsType::Codec:             return IStatsReportTypes::StatsType_Codec;
        case RTCStatsType::SctpTransport:     return IStatsReportTypes::StatsType_SCTPTransport;
        case RTCStatsType::DataChannel:       return IStatsReportTypes::StatsType_DataChannel;
        case RTCStatsType::Stream:            return IStatsReportTypes::StatsType_Stream;
        case RTCStatsType::Track:             return IStatsReportTypes::StatsType_Track;
        case RTCStatsType::IceGatherer:       return IStatsReportTypes::StatsType_ICEGatherer;
        case RTCStatsType::IceTransport:      return IStatsReportTypes::StatsType_ICETransport;
        case RTCStatsType::DtlsTransport:     return IStatsReportTypes::StatsType_DTLSTransport;
        case RTCStatsType::SrtpTransport:     return IStatsReportTypes::StatsType_SRTPTransport;
        case RTCStatsType::Certificate:       return IStatsReportTypes::StatsType_Certificate;
        case RTCStatsType::Candidate:         return IStatsReportTypes::StatsType_Candidate;
        case RTCStatsType::CandidatePair:     return IStatsReportTypes::StatsType_CandidatePair;
        case RTCStatsType::LocalCandidate:    return IStatsReportTypes::StatsType_LocalCandidate;
        case RTCStatsType::RemoteCandidate:   return IStatsReportTypes::StatsType_RemoteCandidate;
        }
        throw ref new Platform::NotImplementedException();
      }

      RTCStatsIceCandidatePairState Helper::Convert(IStatsReportTypes::StatsICECandidatePairStates state)
      {
        switch (state) {
        case IStatsReportTypes::StatsICECandidatePairState_Frozen:      return RTCStatsIceCandidatePairState::Frozen;
        case IStatsReportTypes::StatsICECandidatePairState_Waiting:     return RTCStatsIceCandidatePairState::Waiting;
        case IStatsReportTypes::StatsICECandidatePairState_InProgress:  return RTCStatsIceCandidatePairState::InProgress;
        case IStatsReportTypes::StatsICECandidatePairState_Failed:      return RTCStatsIceCandidatePairState::Failed;
        case IStatsReportTypes::StatsICECandidatePairState_Succeeded:   return RTCStatsIceCandidatePairState::Succeeded;
        case IStatsReportTypes::StatsICECandidatePairState_Cancelled:   return RTCStatsIceCandidatePairState::Cancelled;
        }
        throw ref new Platform::NotImplementedException();
      }

      IStatsReportTypes::StatsICECandidatePairStates Helper::Convert(RTCStatsIceCandidatePairState state)
      {
        switch (state) {
        case RTCStatsIceCandidatePairState::Frozen:       return IStatsReportTypes::StatsICECandidatePairState_Frozen;
        case RTCStatsIceCandidatePairState::Waiting:      return IStatsReportTypes::StatsICECandidatePairState_Waiting;
        case RTCStatsIceCandidatePairState::InProgress:   return IStatsReportTypes::StatsICECandidatePairState_InProgress;
        case RTCStatsIceCandidatePairState::Failed:       return IStatsReportTypes::StatsICECandidatePairState_Failed;
        case RTCStatsIceCandidatePairState::Succeeded:    return IStatsReportTypes::StatsICECandidatePairState_Succeeded;
        case RTCStatsIceCandidatePairState::Cancelled:    return IStatsReportTypes::StatsICECandidatePairState_Cancelled;
        }
        throw ref new Platform::NotImplementedException();
      }

      //-----------------------------------------------------------------------
      // SessionDescription convert methods
      //-----------------------------------------------------------------------

      ISessionDescriptionTypes::SignalingTypes Helper::Convert(adapter::RTCSessionDescriptionSignalingType type)
      {
        switch (type)
        {
        case adapter::RTCSessionDescriptionSignalingType::Json:         return ISessionDescription::SignalingType_JSON;
        case adapter::RTCSessionDescriptionSignalingType::SdpOffer:     return ISessionDescription::SignalingType_SDPOffer;
        case adapter::RTCSessionDescriptionSignalingType::SdpPranswer:  return ISessionDescription::SignalingType_SDPPranswer;
        case adapter::RTCSessionDescriptionSignalingType::SdpAnswer:    return ISessionDescription::SignalingType_SDPAnswer;
        case adapter::RTCSessionDescriptionSignalingType::SdpRollback:  return ISessionDescription::SignalingType_SDPRollback;
        }
        throw ref new Platform::NotImplementedException();
      }
      adapter::RTCSessionDescriptionSignalingType Helper::Convert(ISessionDescription::SignalingTypes type)
      {
        switch (type)
        {
        case ISessionDescription::SignalingType_JSON:           return adapter::RTCSessionDescriptionSignalingType::Json;
        case ISessionDescription::SignalingType_SDPOffer:       return adapter::RTCSessionDescriptionSignalingType::SdpOffer;
        case ISessionDescription::SignalingType_SDPPranswer:    return adapter::RTCSessionDescriptionSignalingType::SdpPranswer;
        case ISessionDescription::SignalingType_SDPAnswer:      return adapter::RTCSessionDescriptionSignalingType::SdpAnswer;
        case ISessionDescription::SignalingType_SDPRollback:    return adapter::RTCSessionDescriptionSignalingType::SdpRollback;
        }
        throw ref new Platform::NotImplementedException();
      }

      ISessionDescriptionTypes::MediaTypes Helper::Convert(adapter::RTCSessionDescriptionMediaType type)
      {
        switch (type)
        {
        case adapter::RTCSessionDescriptionMediaType::Unknown:      return ISessionDescriptionTypes::MediaType_Unknown;
        case adapter::RTCSessionDescriptionMediaType::Audio:        return ISessionDescriptionTypes::MediaType_Audio;
        case adapter::RTCSessionDescriptionMediaType::Video:        return ISessionDescriptionTypes::MediaType_Video;
        case adapter::RTCSessionDescriptionMediaType::Text:         return ISessionDescriptionTypes::MediaType_Text;
        case adapter::RTCSessionDescriptionMediaType::Application:  return ISessionDescriptionTypes::MediaType_Application;
        }
        throw ref new Platform::NotImplementedException();
      }

      adapter::RTCSessionDescriptionMediaType Helper::Convert(ISessionDescriptionTypes::MediaTypes type)
      {
        switch (type)
        {
        case ISessionDescriptionTypes::MediaType_Unknown:      return adapter::RTCSessionDescriptionMediaType::Unknown;
        case ISessionDescriptionTypes::MediaType_Audio:        return adapter::RTCSessionDescriptionMediaType::Audio;
        case ISessionDescriptionTypes::MediaType_Video:        return adapter::RTCSessionDescriptionMediaType::Video;
        case ISessionDescriptionTypes::MediaType_Text:         return adapter::RTCSessionDescriptionMediaType::Text;
        case ISessionDescriptionTypes::MediaType_Application:  return adapter::RTCSessionDescriptionMediaType::Application;
        }
        throw ref new Platform::NotImplementedException();
      }

      ISessionDescriptionTypes::MediaDirections Helper::Convert(adapter::RTCSessionDescriptionMediaDirection type)
      {
        switch (type)
        {
        case adapter::RTCSessionDescriptionMediaDirection::SendReceive:     return ISessionDescriptionTypes::MediaDirection_SendReceive;
        case adapter::RTCSessionDescriptionMediaDirection::SendOnly:        return ISessionDescriptionTypes::MediaDirection_SendOnly;
        case adapter::RTCSessionDescriptionMediaDirection::ReceiveOnly:     return ISessionDescriptionTypes::MediaDirection_ReceiveOnly;
        case adapter::RTCSessionDescriptionMediaDirection::Inactive:        return ISessionDescriptionTypes::MediaDirection_Inactive;
        }
        throw ref new Platform::NotImplementedException();
      }
      adapter::RTCSessionDescriptionMediaDirection Helper::Convert(ISessionDescriptionTypes::MediaDirections type)
      {
        switch (type)
        {
        case ISessionDescriptionTypes::MediaDirection_SendReceive:    return adapter::RTCSessionDescriptionMediaDirection::SendReceive;
        case ISessionDescriptionTypes::MediaDirection_SendOnly:       return adapter::RTCSessionDescriptionMediaDirection::SendOnly;
        case ISessionDescriptionTypes::MediaDirection_ReceiveOnly:    return adapter::RTCSessionDescriptionMediaDirection::ReceiveOnly;
        case ISessionDescriptionTypes::MediaDirection_Inactive:       return adapter::RTCSessionDescriptionMediaDirection::Inactive;
        }
        throw ref new Platform::NotImplementedException();
      }

      //-----------------------------------------------------------------------
      // PeerSessionDescriptionManager convert methods
      //-----------------------------------------------------------------------
      IPeerConnectionTypes::BundlePolicies Helper::Convert(adapter::RTCBundlePolicy policy)
      {
        switch (policy)
        {
        case adapter::RTCBundlePolicy::Balanced:        return IPeerConnectionTypes::BundlePolicy_Balanced;
        case adapter::RTCBundlePolicy::MaxBundle:       return IPeerConnectionTypes::BundlePolicy_MaxBundle;
        case adapter::RTCBundlePolicy::MaxCompat:       return IPeerConnectionTypes::BundlePolicy_MaxCompat;
        }
        throw ref new Platform::NotImplementedException();
      }
      adapter::RTCBundlePolicy Helper::Convert(IPeerConnectionTypes::BundlePolicies policy)
      {
        switch (policy)
        {
        case IPeerConnectionTypes::BundlePolicy_Balanced:        return adapter::RTCBundlePolicy::Balanced;
        case IPeerConnectionTypes::BundlePolicy_MaxBundle:       return adapter::RTCBundlePolicy::MaxBundle;
        case IPeerConnectionTypes::BundlePolicy_MaxCompat:       return adapter::RTCBundlePolicy::MaxCompat;
        }
        throw ref new Platform::NotImplementedException();
      }

      IPeerConnectionTypes::RTCPMuxPolicies Helper::Convert(adapter::RTCRtcpMuxPolicy policy)
      {
        switch (policy)
        {
        case adapter::RTCRtcpMuxPolicy::Negotiated:    return IPeerConnectionTypes::RTCPMuxPolicy_Negotiated;
        case adapter::RTCRtcpMuxPolicy::Require:       return IPeerConnectionTypes::RTCPMuxPolicy_Require;
        }
        throw ref new Platform::NotImplementedException();
      }

      adapter::RTCRtcpMuxPolicy Helper::Convert(IPeerConnectionTypes::RTCPMuxPolicies policy)
      {
        switch (policy)
        {
        case IPeerConnectionTypes::RTCPMuxPolicy_Negotiated:    return adapter::RTCRtcpMuxPolicy::Negotiated;
        case IPeerConnectionTypes::RTCPMuxPolicy_Require:       return adapter::RTCRtcpMuxPolicy::Require;
        }
        throw ref new Platform::NotImplementedException();
      }

      IPeerConnectionTypes::SignalingModes Helper::Convert(adapter::RTCPeerConnectionSignalingMode mode)
      {
        switch (mode)
        {
        case adapter::RTCPeerConnectionSignalingMode::Json:    return IPeerConnectionTypes::SignalingMode_JSON;
        case adapter::RTCPeerConnectionSignalingMode::Sdp:       return IPeerConnectionTypes::SignalingMode_SDP;
        }
        throw ref new Platform::NotImplementedException();
      }

      adapter::RTCPeerConnectionSignalingMode Helper::Convert(IPeerConnectionTypes::SignalingModes mode)
      {
        switch (mode)
        {
        case IPeerConnectionTypes::SignalingMode_JSON:    return adapter::RTCPeerConnectionSignalingMode::Json;
        case IPeerConnectionTypes::SignalingMode_SDP:     return adapter::RTCPeerConnectionSignalingMode::Sdp;
        }
        throw ref new Platform::NotImplementedException();
      }

      IPeerConnectionTypes::SignalingStates Helper::Convert(adapter::RTCSignalingState state)
      {
        switch (state)
        {
        case adapter::RTCSignalingState::Stable:              return IPeerConnectionTypes::SignalingState_Stable;
        case adapter::RTCSignalingState::HaveLocalOffer:      return IPeerConnectionTypes::SignalingState_HaveLocalOffer;
        case adapter::RTCSignalingState::HaveRemoteOffer:     return IPeerConnectionTypes::SignalingState_HaveRemoteOffer;
        case adapter::RTCSignalingState::HaveLocalPranswer:   return IPeerConnectionTypes::SignalingState_HaveLocalPranswer;
        case adapter::RTCSignalingState::HaveRemotePranswer:  return IPeerConnectionTypes::SignalingState_HaveRemotePranswer;
        case adapter::RTCSignalingState::Closed:              return IPeerConnectionTypes::SignalingState_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }
      adapter::RTCSignalingState Helper::Convert(IPeerConnectionTypes::SignalingStates state)
      {
        switch (state)
        {
        case IPeerConnectionTypes::SignalingState_Stable:               return adapter::RTCSignalingState::Stable;
        case IPeerConnectionTypes::SignalingState_HaveLocalOffer:       return adapter::RTCSignalingState::HaveLocalOffer;
        case IPeerConnectionTypes::SignalingState_HaveRemoteOffer:      return adapter::RTCSignalingState::HaveRemoteOffer;
        case IPeerConnectionTypes::SignalingState_HaveLocalPranswer:    return adapter::RTCSignalingState::HaveLocalPranswer;
        case IPeerConnectionTypes::SignalingState_HaveRemotePranswer:   return adapter::RTCSignalingState::HaveRemotePranswer;
        case IPeerConnectionTypes::SignalingState_Closed:               return adapter::RTCSignalingState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      IPeerConnectionTypes::ICEGatheringStates Helper::Convert(adapter::RTCIceGatheringState state)
      {
        switch (state)
        {
        case adapter::RTCIceGatheringState::New:         return IICEGathererTypes::State_New;
        case adapter::RTCIceGatheringState::Gathering:   return IICEGathererTypes::State_Gathering;
        case adapter::RTCIceGatheringState::Complete:    return IICEGathererTypes::State_Complete;
        }
        throw ref new Platform::NotImplementedException();
      }

      adapter::RTCIceGatheringState Helper::ConvertToGatheringState(IPeerConnectionTypes::ICEGatheringStates state)
      {
        switch (state)
        {
        case IICEGathererTypes::State_New:          return adapter::RTCIceGatheringState::New;
        case IICEGathererTypes::State_Gathering:    return adapter::RTCIceGatheringState::Gathering;
        case IICEGathererTypes::State_Complete:     return adapter::RTCIceGatheringState::Complete;
        case IICEGathererTypes::State_Closed:       return adapter::RTCIceGatheringState::Complete;
        }
        throw ref new Platform::NotImplementedException();
      }

      IPeerConnectionTypes::ICEConnectionStates Helper::Convert(adapter::RTCIceConnectionState state)
      {
        switch (state)
        {
        case adapter::RTCIceConnectionState::New:           return IICETransportTypes::State_New;
        case adapter::RTCIceConnectionState::Checking:      return IICETransportTypes::State_Checking;
        case adapter::RTCIceConnectionState::Connected:     return IICETransportTypes::State_Connected;
        case adapter::RTCIceConnectionState::Completed:     return IICETransportTypes::State_Completed;
        case adapter::RTCIceConnectionState::Failed:        return IICETransportTypes::State_Failed;
        case adapter::RTCIceConnectionState::Disconnected:  return IICETransportTypes::State_Disconnected;
        case adapter::RTCIceConnectionState::Closed:        return IICETransportTypes::State_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      adapter::RTCIceConnectionState Helper::ConvertToConnectionState(IPeerConnectionTypes::ICEConnectionStates state)
      {
        switch (state)
        {
        case IICETransportTypes::State_New:           return adapter::RTCIceConnectionState::New;
        case IICETransportTypes::State_Checking:      return adapter::RTCIceConnectionState::Checking;
        case IICETransportTypes::State_Connected:     return adapter::RTCIceConnectionState::Connected;
        case IICETransportTypes::State_Completed:     return adapter::RTCIceConnectionState::Completed;
        case IICETransportTypes::State_Failed:        return adapter::RTCIceConnectionState::Failed;
        case IICETransportTypes::State_Disconnected:  return adapter::RTCIceConnectionState::Disconnected;
        case IICETransportTypes::State_Closed:        return adapter::RTCIceConnectionState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

      IPeerConnectionTypes::PeerConnectionStates Helper::Convert(adapter::RTCPeerConnectionState state)
      {
        switch (state)
        {
        case adapter::RTCPeerConnectionState::New:          return IPeerConnectionTypes::PeerConnectionState_New;
        case adapter::RTCPeerConnectionState::Connecting:   return IPeerConnectionTypes::PeerConnectionState_Connecting;
        case adapter::RTCPeerConnectionState::Connected:    return IPeerConnectionTypes::PeerConnectionState_Connected;
        case adapter::RTCPeerConnectionState::Disconnected: return IPeerConnectionTypes::PeerConnectionState_Disconnected;
        case adapter::RTCPeerConnectionState::Failed:       return IPeerConnectionTypes::PeerConnectionState_Failed;
        case adapter::RTCPeerConnectionState::Closed:       return IPeerConnectionTypes::PeerConnectionState_Closed;
        }
        throw ref new Platform::NotImplementedException();
      }
      adapter::RTCPeerConnectionState Helper::Convert(IPeerConnectionTypes::PeerConnectionStates state)
      {
        switch (state)
        {
        case IPeerConnectionTypes::PeerConnectionState_New:          return adapter::RTCPeerConnectionState::New;
        case IPeerConnectionTypes::PeerConnectionState_Connecting:   return adapter::RTCPeerConnectionState::Connecting;
        case IPeerConnectionTypes::PeerConnectionState_Connected:    return adapter::RTCPeerConnectionState::Connected;
        case IPeerConnectionTypes::PeerConnectionState_Disconnected: return adapter::RTCPeerConnectionState::Disconnected;
        case IPeerConnectionTypes::PeerConnectionState_Failed:       return adapter::RTCPeerConnectionState::Failed;
        case IPeerConnectionTypes::PeerConnectionState_Closed:       return adapter::RTCPeerConnectionState::Closed;
        }
        throw ref new Platform::NotImplementedException();
      }

#pragma endregion

    } // namespace internal
  } // namespace ortc
} // namespace org
