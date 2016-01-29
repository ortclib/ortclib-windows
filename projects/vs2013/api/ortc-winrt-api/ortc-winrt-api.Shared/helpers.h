#pragma once

#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCDtlsTransport.h"
#include "RTCSctpTransport.h"
#include "RTCDataChannel.h"
#include "RTCIceTransportController.h"
#include "RTCRtpSender.h"
#include "RTPTypes.h"
#include "MediaStreamTrack.h"
#include <ortc/IMediaStreamTrack.h>
#include <ortc/IMediaDevices.h>

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

  RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr codecCapabilityPtr);
  RTCRtpHeaderExtensions^ ToCx(IRTPTypes::HeaderExtensionPtr headerExtensions);

  MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr capabilitiesPtr);
  MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr constraintSetPtr);
  MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr trackConstraintsPtr);
  MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr settingsPtr);
  SupportedConstraints^ ToCx(IMediaDevicesTypes::SupportedConstraints constraints);

  MediaDeviceInfo^ ToCx(IMediaDevicesTypes::Device device);
  MediaDeviceInfo^ ToCx(IMediaDevicesTypes::Device device);

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

  IRTPTypes::Parameters FromCx(RTCRtpParameters^ parameters);

}