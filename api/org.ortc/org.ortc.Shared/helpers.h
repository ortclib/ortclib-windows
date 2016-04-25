#pragma once

#include "Logger.h"

#include <ortc/IDataChannel.h>
#include <ortc/IDTLSTransport.h>
#include <ortc/IICETypes.h>
#include <ortc/IICEGatherer.h>
#include <ortc/IICETransport.h>
#include <ortc/IMediaDevices.h>
#include <ortc/IRTPTypes.h>
#include <ortc/ISCTPTransport.h>

#include <zsLib/types.h>

namespace org
{
  namespace ortc
  {
    enum class MediaDeviceKind;
    enum class MediaStreamTrackState;
    enum class MediaStreamTrackKind;

    enum class RTCDataChannelState;
    enum class RTCDegradationPreference;
    enum class RTCDtlsTransportState;
    enum class RTCDtlsRole;
    enum class RTCIceCandidateType;
    enum class RTCIceComponent;
    enum class RTCIceGatherFilterPolicy;
    enum class RTCIceGathererCredentialType;
    enum class RTCIceGathererState;
    enum class RTCIceProtocol;
    enum class RTCIceRole;
    enum class RTCIceTransportState;
    enum class RTCIceTcpCandidateType;
    enum class RTCPriorityType;
    enum class RTCSctpTransportState;

    ref class MediaStreamTrack;
    ref class RTCCertificate;
    ref class RTCDtlsTransport;
    ref class RTCIceGatherer;
    ref class RTCIceTransport;
    ref class RTCSctpTransport;

    namespace internal
    {
      using zsLib::Optional;

      class Helper
      {
        typedef ::ortc::IDataChannelTypes IDataChannelTypes;
        typedef ::ortc::IDTLSTransportTypes IDTLSTransportTypes;
        typedef ::ortc::IICETypes IICETypes;
        typedef ::ortc::IICEGathererTypes IICEGathererTypes;
        typedef ::ortc::IICETransportTypes IICETransportTypes;
        typedef ::ortc::IMediaDevicesTypes IMediaDevicesTypes;
        typedef ::ortc::IMediaStreamTrackTypes IMediaStreamTrackTypes;
        typedef ::ortc::IRTPTypes IRTPTypes;
        typedef ::ortc::ISCTPTransportTypes ISCTPTransportTypes;

      public:
        static bool IsNullOrEmpty(Platform::String ^input);
        static std::string FromCx(Platform::String^ inObj);
        static Platform::String^ ToCx(const std::string & inObj);
        static Platform::String^ ToCx(const char *str);

        static Platform::IBox<Platform::Boolean>^ ToCx(const Optional<bool> &input);
        static Optional<bool> FromCx(Platform::IBox<Platform::Boolean>^ input);

        static Platform::IBox<uint8>^ ToCx(const Optional<BYTE> &input);
        static Optional<BYTE> FromCx(Platform::IBox<uint8>^ input);

        static Platform::IBox<int16>^ ToCx(const Optional<SHORT> &input);
        static Optional<SHORT> FromCx(Platform::IBox<int16>^ input);

        static Platform::IBox<uint16>^ ToCx(const Optional<USHORT> &input);
        static Optional<USHORT> FromCx(Platform::IBox<uint16>^ input);

        static Platform::IBox<int32>^ ToCx(const Optional<LONG> &input);
        static Optional<LONG> FromCx(Platform::IBox<int32>^ input);

        static Platform::IBox<uint32>^ ToCx(const Optional<ULONG> &input);
        static Optional<ULONG> FromCx(Platform::IBox<uint32>^ input);

        static Platform::IBox<uint64>^ ToCx(const Optional<ULONGLONG> &input);
        static Optional<ULONGLONG> FromCx(Platform::IBox<uint64>^ input);

        static Platform::IBox<float64>^ ToCx(const Optional<double> &input);
        static Optional<double> FromCx(Platform::IBox<float64>^ input);

        static Platform::String^ ToCx(const Optional<zsLib::String> &input);
        static Optional<zsLib::String> FromCxToOptional(Platform::String^ input);

        // RTCIceGatherer convertors
        static IICEGathererTypes::FilterPolicies Convert(RTCIceGatherFilterPolicy policy);
        static RTCIceGatherFilterPolicy Convert(IICEGathererTypes::FilterPolicies policy);

        static IICEGathererTypes::CredentialTypes Convert(RTCIceGathererCredentialType credentialType);
        static RTCIceGathererCredentialType Convert(IICEGathererTypes::CredentialTypes credentialType);

        static IICEGathererTypes::States Convert(RTCIceGathererState state);
        static RTCIceGathererState Convert(IICEGathererTypes::States state);

        // RTCDataChannel convertors
        static IDataChannelTypes::States Convert(RTCDataChannelState state);
        static RTCDataChannelState Convert(IDataChannelTypes::States state);

        // RTCDtlsTransport convertors
        static IDTLSTransportTypes::States Convert(RTCDtlsTransportState state);
        static RTCDtlsTransportState Convert(IDTLSTransportTypes::States state);

        static IDTLSTransportTypes::Roles Convert(RTCDtlsRole role);
        static RTCDtlsRole Convert(IDTLSTransportTypes::Roles role);

        // RTCIceTypes convertors
        static IICETypes::Roles Convert(RTCIceRole role);
        static RTCIceRole Convert(IICETypes::Roles role);

        static IICETypes::Components Convert(RTCIceComponent component);
        static RTCIceComponent Convert(IICETypes::Components component);

        static IICETypes::Protocols Convert(RTCIceProtocol protocol);
        static RTCIceProtocol Convert(IICETypes::Protocols protocol);

        static IICETypes::CandidateTypes Convert(RTCIceCandidateType candidateType);
        static RTCIceCandidateType Convert(IICETypes::CandidateTypes candidateType);

        static IICETypes::TCPCandidateTypes Convert(RTCIceTcpCandidateType candidateType);
        static RTCIceTcpCandidateType Convert(IICETypes::TCPCandidateTypes candidateType);

        // RTCIceTransport converters
        static IICETransportTypes::States Convert(RTCIceTransportState state);
        static RTCIceTransportState Convert(IICETransportTypes::States state);

        // MediaDevice
        static IMediaDevicesTypes::DeviceKinds Convert(MediaDeviceKind kind);
        static MediaDeviceKind Convert(IMediaDevicesTypes::DeviceKinds kind);

        // MediaStreamTrack convertors
        static IMediaStreamTrackTypes::States Convert(MediaStreamTrackState state);
        static MediaStreamTrackState Convert(IMediaStreamTrackTypes::States state);

        static IMediaStreamTrackTypes::Kinds Convert(MediaStreamTrackKind kind);
        static MediaStreamTrackKind Convert(IMediaStreamTrackTypes::Kinds kind);

        // RtpTypes convertors
        static IRTPTypes::PriorityTypes Convert(RTCPriorityType priority);
        static RTCPriorityType Convert(IRTPTypes::PriorityTypes priority);

        static IRTPTypes::DegradationPreferences Convert(RTCDegradationPreference preference);
        static RTCDegradationPreference Convert(IRTPTypes::DegradationPreferences preference);

        // SctpTransport converters
        static ISCTPTransportTypes::States Convert(RTCSctpTransportState state);
        static RTCSctpTransportState Convert(ISCTPTransportTypes::States state);

        // Logger convertors
        static zsLib::Log::Level Convert(Log::Level level);
        static const char *ToComponent(Log::Component  component);
      };

    } // namespace internal
  } // namespace ortc
} // namespace org
