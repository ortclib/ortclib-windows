#pragma once

#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCDtlsTransport.h"
#include "RTCSctpTransport.h"
#include "RTCDataChannel.h"
#include "RTCIceTransportController.h"
#include "RTCRtpSender.h"
#include "RTCRtpReceiver.h"

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
  };
}