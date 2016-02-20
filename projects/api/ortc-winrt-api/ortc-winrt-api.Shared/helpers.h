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

namespace ortc_winrt_api
{
  std::string FromCx(Platform::String^ inObj);
  Platform::String^ ToCx(const std::string & inObj);

  RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr input);
  IDataChannelTypes::ParametersPtr FromCx(RTCDataChannelParameters^ input);
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
    static IIceGathererPtr FromIceGatherer(RTCIceGatherer^ gatherer) { return gatherer->mNativePointer; }
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