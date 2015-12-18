#pragma once

#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCDtlsTransport.h"
#include "RTCSctpTransport.h"
#include "RTCDataChannel.h"
#include "RTCIceTransportController.h"
#include "RTCRtpSender.h"
#include "RTCRtpReceiver.h"
#include "MediaStreamTrack.h"

using namespace ortc;

namespace ortc_winrt_api
{
  public ref class ConfigureOrtcEngine sealed
  {
  public:
    ConfigureOrtcEngine();
  };

  std::string FromCx(Platform::String^ inObj);
  Platform::String^ ToCx(std::string const& inObj);

  IICETypes::Candidate FromCx(RTCIceCandidate^ candidate);
  RTCIceCandidate^ ToCx(IICETypes::CandidatePtr candidate);

  IDTLSTransportTypes::Parameters FromCx(RTCDtlsParameters^ parameters);
  RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr parameters);

  IDataChannelTypes::Parameters FromCx(RTCDataChannelParameters^ parameters);
  RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr parameters);

  IICEGatherer::Options FromCx(RTCIceGatherOptions^ options);

  RTCRtpCodecCapability^ toCx(IRTPTypes::CodecCapabilityPtr codecCapabilityPtr);
  RTCRtpHeaderExtensions^ toCx(IRTPTypes::HeaderExtensionsPtr headerExtensions);

  static MediaTrackCapabilities^ toCx(IMediaStreamTrackTypes::CapabilitiesPtr capabilitiesPtr);
  static MediaTrackConstraintSet^ toCx(IMediaStreamTrackTypes::ConstraintSetPtr constraintSetPtr);
  static MediaTrackConstraints^ toCx(IMediaStreamTrackTypes::TrackConstraintsPtr trackConstraintsPtr);
  class FetchNativePointer
  {
  public:
    static IIceGathererPtr fromIceGatherer(RTCIceGatherer^ gatherer) { return gatherer->mNativePointer; }
    static IIceTransportPtr fromIceTransport(RTCIceTransport^ transport) { return transport->mNativePointer; }
    static IDtlsTransportPtr fromDtlsTransport(RTCDtlsTransport^ transport) { return transport->mNativePointer; }
    static ICertificatePtr fromCertificate(RTCCertificate^ certificate) { return certificate->mNativePointer; }
    static ISctpTransportPtr fromSctpTransport(RTCSctpTransport^ transport) { return transport->mNativePointer; }
  };

  class ConvertObjectToCx
  {
  public:
    static RTCIceTransport^ iceTransport(IIceTransportPtr iceTransport);

	static MediaStreamTrack^ mediaStreamTrack(IMediaStreamTrackPtr mediaStreamTrackPtr);
	
	static MediaTrackConstraints^ mediaTrackConstraints(IMediaStreamTrackTypes::TrackConstraintsPtr trackConstraintsPtr);
	static MediaTrackSettings^ mediaTrackSettings(IMediaStreamTrackTypes::SettingsPtr settingsPtr);
  };

}