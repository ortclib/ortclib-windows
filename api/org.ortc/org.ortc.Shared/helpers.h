#pragma once

#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCDtlsTransport.h"
#include "RTCSctpTransport.h"
#include "RTCDataChannel.h"
#include "RTCIceTransportController.h"
#include "RTCRtpSender.h"
#include "RTPTypes.h"
#include "Contraints.h"
#include "Capabilities.h"
#include "MediaStreamTrack.h"
#include "MediaDevices.h"
#include "Logger.h"

#include <zsLib/zsLib.h>

namespace org
{
  namespace ortc
  {
    namespace internal
    {
      class Helper
      {
      public:
        static std::string FromCx(Platform::String^ inObj);
        static Platform::String^ ToCx(const std::string & inObj);

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
      };
    }

    ISCTPTransport::CapabilitiesPtr FromCx(RTCSctpCapabilities^ input);

    RTCDtlsFingerprint^ ToCx(ICertificateTypes::FingerprintPtr input);
    IDTLSTransportTypes::ParametersPtr FromCx(RTCDtlsParameters^ parameters);
    RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr parameters);

    MediaDeviceInfo^ ToCx(const IMediaDevices::Device &input);
    MediaDeviceInfo^ ToCx(IMediaDevices::DevicePtr input);

    IMediaStreamTrackTypes::ConstraintsPtr FromCx(Constraints ^input);
    MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr input);
    MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr input);
    IMediaStreamTrackTypes::TrackConstraintsPtr FromCx(MediaTrackConstraints^ input);
    MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr input);
    IMediaStreamTrackTypes::ConstraintSetPtr FromCx(MediaTrackConstraintSet^ input);

    SupportedConstraints^ ToCx(IMediaDevices::SupportedConstraintsPtr input);
    Constraints^ ToCx(IMediaStreamTrackTypes::ConstraintsPtr input);

    RTCIceCandidate^ ToCx(IICETypes::CandidatePtr candidate);
    RTCIceCandidate^ ToCx(const IICETypes::Candidate &candidate);
    IICETypes::CandidatePtr FromCx(RTCIceCandidate^ candidate);
    IICETypes::ParametersPtr FromCx(RTCIceParameters^ params);
    RTCIceParameters^ ToCx(IICETypes::ParametersPtr input);
    IICETypes::ParametersPtr FromCx(RTCIceParameters^ input);

    IICEGatherer::OptionsPtr FromCx(RTCIceGatherOptions^ options);

    RTCIceCandidatePair^ ToCx(IICETransport::CandidatePairPtr input);
    IICETransport::CandidatePairPtr FromCx(RTCIceCandidatePair^ input);

    IRTPTypes::HeaderExtensionParametersPtr FromCx(RTCRtpHeaderExtensionParameters^ input);
    RTCRtpParameters ^ToCx(IRTPTypes::ParametersPtr input);
    IRTPTypes::ParametersPtr FromCx(RTCRtpParameters^ input);
    RTCRtpCapabilities ^ToCx(IRTPTypes::CapabilitiesPtr input);
    IRTPTypes::CapabilitiesPtr FromCx(RTCRtpCapabilities^ input);
    RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr input);
    IRTPTypes::CodecCapabilityPtr FromCx(RTCRtpCodecCapability^ input);
    IRTPTypes::RTCPFeedback FromCx(RTCRtcpFeedback^ input);
    IRTPTypes::HeaderExtensionPtr FromCx(RTCRtpHeaderExtension^ input);

    class FetchNativePointer
    {
    public:
      static IIceGathererPtr FromIceGatherer(RTCIceGatherer^ gatherer) { return gatherer->_NativePointer; }
      static IIceTransportPtr FromIceTransport(RTCIceTransport^ transport) { return transport->mNativePointer; }
      static IDtlsTransportPtr FromDtlsTransport(RTCDtlsTransport^ transport) { return transport->mNativePointer; }
      static ICertificatePtr FromCertificate(RTCCertificate^ certificate) { return certificate->mNativePointer; }
      static ISctpTransportPtr FromSctpTransport(RTCSctpTransport^ transport) { return transport->mNativePointer; }
      static IMediaStreamTrackPtr FromMediaTrack(MediaStreamTrack^ track) { return track->mNativePointer; }
    };

    class ConvertObjectToCx
    {
    public:
      static RTCIceGatherer^ ToIceGatherer(IIceGathererPtr iceGatherer);
      static RTCIceTransport^ ToIceTransport(IIceTransportPtr iceTransport);
      static RTCCertificate^ ToCertificate(ICertificatePtr certificate);
      static RTCDtlsTransport^ ToDtlsTransport(IDtlsTransportPtr dtlsTransport);
      static RTCSctpTransport^ ToSctpTransport(ISctpTransportPtr sctpTransport);

      static MediaStreamTrack^ ToMediaStreamTrack(IMediaStreamTrackPtr mediaStreamTrackPtr);
    };

    class CreateEmptyCxObject
    {
    public:
      static RTCIceTransport^ IceTransport() { return ref new RTCIceTransport(true); }
    };

    class CallPrivateMethod
    {
    public:
      static void SetMediaElement(MediaStreamTrack^ track, void* element) { track->SetMediaElement(element); }
    };

    namespace internal
    {
      class ConvertEnums
      {
      public:
        // RTCIceGatherer convertors
        static IICEGathererTypes::FilterPolicies convert(RTCIceGatherPolicy policy);
        static RTCIceGatherPolicy convert(IICEGathererTypes::FilterPolicies policy);

        static IICEGathererTypes::CredentialTypes convert(RTCIceGathererCredentialType credentialType);
        static RTCIceGathererCredentialType convert(IICEGathererTypes::CredentialTypes credentialType);

        static IICEGathererTypes::States convert(RTCIceGathererState state);
        static RTCIceGathererState convert(IICEGathererTypes::States state);

        // RTCDataChannel convertors
        static IDataChannel::States convert(RTCDataChannelState state);
        static RTCDataChannelState ConvertEnums::convert(IDataChannel::States state);

        // RTCDtlsTransport convertors
        static IDtlsTransport::States convert(RTCDtlsTransportState state);
        static RTCDtlsTransportState convert(IDtlsTransport::States state);

        static IDtlsTransport::Roles convert(RTCDtlsRole role);
        static RTCDtlsRole convert(IDtlsTransport::Roles role);

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

        // MediaStreamTrack convertors
        static IMediaStreamTrack::States convert(MediaStreamTrackState state);
        static MediaStreamTrackState convert(IMediaStreamTrack::States state);

        static IMediaStreamTrack::Kinds convert(MediaStreamTrackKind kind);
        static MediaStreamTrackKind convert(IMediaStreamTrack::Kinds kind);

        // RtpTypes convertors
        static IRTPTypes::PriorityTypes convert(RTCPriorityType priority);
        static RTCPriorityType convert(IRTPTypes::PriorityTypes priority);

        static IRTPTypes::DegradationPreferences convert(RTCDegradationPreference preference);
        static RTCDegradationPreference convert(IRTPTypes::DegradationPreferences preference);

        // Logger convertors
        static zsLib::Log::Level convert(Log::Level level);
        static const char *toComponent(Log::Component  component);
      };
    }
  }
}
