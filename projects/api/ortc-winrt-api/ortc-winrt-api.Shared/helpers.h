#pragma once

#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCDtlsTransport.h"
#include "RTCSctpTransport.h"
#include "RTCDataChannel.h"
#include "RTCIceTransportController.h"
#include "RTCRtpSender.h"
#include "RTPTypes.h"
#include "MediaTypes.h"
#include "MediaStreamTrack.h"
#include "MediaDevices.h"
#include "Logger.h"
#include <ortc/IMediaStreamTrack.h>
#include <ortc/IMediaDevices.h>
#include <op-services-cpp\openpeer\services\IHelper.h>

using namespace ortc;

namespace ortc_winrt_api
{
  std::string FromCx(Platform::String^ inObj);
  Platform::String^ ToCx(std::string const& inObj);

  IICETypes::Candidate FromCx(RTCIceCandidate^ candidate);
  RTCIceCandidate^ ToCx(IICETypes::CandidatePtr candidate);

  IDTLSTransportTypes::Parameters FromCx(RTCDtlsParameters^ parameters);
  RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr parameters);

  IDataChannelTypes::Parameters FromCx(RTCDataChannelParameters^ parameters);
  RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr parameters);

  IICEGatherer::Options FromCx(RTCIceGatherOptions^ options);

  RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr codecCapabilityPtr);
  RTCRtpHeaderExtension^ ToCx(IRTPTypes::HeaderExtensionPtr headerExtensions);

  IRTPTypes::HeaderExtensionParameters FromCx(RTCRtpHeaderExtensionParameters^ headerExtensions);

  
  MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr constraintSetPtr);
  IMediaStreamTrackTypes::ConstraintSetPtr FromCx(MediaTrackConstraintSet^ constraintSet);

  //Constraints^ ToCx(IMediaStreamTrackTypes::ConstraintsPtr constraintPtr);
  IMediaStreamTrackTypes::ConstraintsPtr FromCx(Constraints^ constraint);

  MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr capabilitiesPtr);
  MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr trackConstraintsPtr);
  MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr settingsPtr);
  SupportedConstraints^ ToCx(IMediaDevicesTypes::SupportedConstraints constraints);

  MediaDeviceInfo^ ToCx(IMediaDevicesTypes::Device device);
  MediaDeviceInfo^ ToCx(IMediaDevicesTypes::Device device);

  IRTPTypes::Parameters FromCx(RTCRtpParameters^ parameters);


  IICETypes::Parameters         FromCx(RTCIceParameters^ params);
  ISCTPTransport::Capabilities  FromCx(RTCSctpCapabilities^ params);
  IRTPTypes::Capabilities       FromCx(RTCRtpCapabilities^ params);
  IRTPTypes::CodecCapability    FromCx(RTCRtpCodecCapability^ params);
  IRTPTypes::RTCPFeedback       FromCx(RTCRtcpFeedback^ params);
  IRTPTypes::HeaderExtension    FromCx(RTCRtpHeaderExtension^ params);

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
	

	//static MediaTrackSettings^ mediaTrackSettings(IMediaStreamTrackTypes::SettingsPtr settingsPtr);
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

      // Logger convertors
      static zsLib::Log::Level convert(Log::Level level);
      static const char *toComponent(Log::Component  component);
    };
  }

}