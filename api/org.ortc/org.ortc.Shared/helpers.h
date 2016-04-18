#pragma once

#include "Logger.h"

#include <ortc/IDataChannel.h>
#include <ortc/IDTLSTransport.h>
#include <ortc/IICETypes.h>
#include <ortc/IICEGatherer.h>
#include <ortc/IICETransport.h>
#include <ortc/IMediaDevices.h>
#include <ortc/IRTPTypes.h>

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
    enum class RTCIceGatherPolicy;
    enum class RTCIceGathererCredentialType;
    enum class RTCIceGathererState;
    enum class RTCIceProtocol;
    enum class RTCIceRole;
    enum class RTCIceTransportState;
    enum class RTCIceTcpCandidateType;
    enum class RTCPriorityType;

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

      public:
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
        static IICEGathererTypes::FilterPolicies convert(RTCIceGatherPolicy policy);
        static RTCIceGatherPolicy convert(IICEGathererTypes::FilterPolicies policy);

        static IICEGathererTypes::CredentialTypes convert(RTCIceGathererCredentialType credentialType);
        static RTCIceGathererCredentialType convert(IICEGathererTypes::CredentialTypes credentialType);

        static IICEGathererTypes::States convert(RTCIceGathererState state);
        static RTCIceGathererState convert(IICEGathererTypes::States state);

        // RTCDataChannel convertors
        static IDataChannelTypes::States convert(RTCDataChannelState state);
        static RTCDataChannelState convert(IDataChannelTypes::States state);

        // RTCDtlsTransport convertors
        static IDTLSTransportTypes::States convert(RTCDtlsTransportState state);
        static RTCDtlsTransportState convert(IDTLSTransportTypes::States state);

        static IDTLSTransportTypes::Roles convert(RTCDtlsRole role);
        static RTCDtlsRole convert(IDTLSTransportTypes::Roles role);

        // RTCIceTypes convertors
        static IICETypes::Roles convert(RTCIceRole role);
        static RTCIceRole convert(IICETypes::Roles role);

        static IICETypes::Components convert(RTCIceComponent component);
        static RTCIceComponent convert(IICETypes::Components component);

        static IICETypes::Protocols convert(RTCIceProtocol protocol);
        static RTCIceProtocol convert(IICETypes::Protocols protocol);

        static IICETypes::CandidateTypes convert(RTCIceCandidateType candidateType);
        static RTCIceCandidateType convert(IICETypes::CandidateTypes candidateType);

        static IICETypes::TCPCandidateTypes convert(RTCIceTcpCandidateType candidateType);
        static RTCIceTcpCandidateType convert(IICETypes::TCPCandidateTypes candidateType);

        // RTCIceTransport converters
        static IICETransportTypes::States convert(RTCIceTransportState state);
        static RTCIceTransportState convert(IICETransportTypes::States state);

        // MediaDevice
        static IMediaDevicesTypes::DeviceKinds convert(MediaDeviceKind kind);
        static MediaDeviceKind convert(IMediaDevicesTypes::DeviceKinds kind);

        // MediaStreamTrack convertors
        static IMediaStreamTrackTypes::States convert(MediaStreamTrackState state);
        static MediaStreamTrackState convert(IMediaStreamTrackTypes::States state);

        static IMediaStreamTrackTypes::Kinds convert(MediaStreamTrackKind kind);
        static MediaStreamTrackKind convert(IMediaStreamTrackTypes::Kinds kind);

        // RtpTypes convertors
        static IRTPTypes::PriorityTypes convert(RTCPriorityType priority);
        static RTCPriorityType convert(IRTPTypes::PriorityTypes priority);

        static IRTPTypes::DegradationPreferences convert(RTCDegradationPreference preference);
        static RTCDegradationPreference convert(IRTPTypes::DegradationPreferences preference);

        // Logger convertors
        static zsLib::Log::Level convert(Log::Level level);
        static const char *toComponent(Log::Component  component);
      };

    } // namespace internal
  } // namespace ortc
} // namespace org
