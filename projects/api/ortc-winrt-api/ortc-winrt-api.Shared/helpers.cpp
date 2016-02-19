#include "pch.h"

#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using namespace Platform;
using Platform::Collections::Vector;

namespace ortc_winrt_api
{
  namespace internal
  {
    //---------------------------------------------------------------------------
    // MediaDeviceKind convert methods
    //---------------------------------------------------------------------------
    static IMediaDevicesTypes::DeviceKinds convert(MediaDeviceKind kind)
    {
      switch (kind) {
      case MediaDeviceKind::AudioInput:   return IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioInput;
      case MediaDeviceKind::AudioOutput:  return IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioOutput;
      case MediaDeviceKind::Video:        return IMediaDevicesTypes::DeviceKinds::DeviceKind_Video;
      }
      throw ref new Platform::NotImplementedException();
    }

    static MediaDeviceKind convert(IMediaDevicesTypes::DeviceKinds kind)
    {
      switch (kind) {
      case IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioInput:   return MediaDeviceKind::AudioInput;
      case IMediaDevicesTypes::DeviceKinds::DeviceKind_AudioOutput:  return MediaDeviceKind::AudioOutput;
      case IMediaDevicesTypes::DeviceKinds::DeviceKind_Video:        return MediaDeviceKind::Video;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // RTCIceGatherPolicy convert methods
    //---------------------------------------------------------------------------
    IICEGathererTypes::FilterPolicies ConvertEnums::convert(RTCIceGatherPolicy policy)
    {
      return static_cast<IICEGathererTypes::FilterPolicies>(zsLib::to_underlying(policy));
    }

    RTCIceGatherPolicy ConvertEnums::convert(IICEGathererTypes::FilterPolicies policy)
    {
      return static_cast<RTCIceGatherPolicy>(zsLib::to_underlying(policy));
    }

    //---------------------------------------------------------------------------
    // RTCIceGathererCredentialType convert methods
    //---------------------------------------------------------------------------
    IICEGathererTypes::CredentialTypes ConvertEnums::convert(RTCIceGathererCredentialType credentialType)
    {
      switch (credentialType) {
      case RTCIceGathererCredentialType::Password:    return IICEGathererTypes::CredentialTypes::CredentialType_Password;
      case RTCIceGathererCredentialType::Token:       return IICEGathererTypes::CredentialTypes::CredentialType_Token;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceGathererCredentialType ConvertEnums::convert(IICEGathererTypes::CredentialTypes credentialType)
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
    IICEGathererTypes::States ConvertEnums::convert(RTCIceGathererState state)
    {
      switch (state) {
      case RTCIceGathererState::New:          return IICEGathererTypes::States::State_New;
      case RTCIceGathererState::Gathering:    return IICEGathererTypes::States::State_Gathering;
      case RTCIceGathererState::Complete:     return IICEGathererTypes::States::State_Complete;
      case RTCIceGathererState::Closed:       return IICEGathererTypes::States::State_Closed;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceGathererState ConvertEnums::convert(IICEGathererTypes::States state)
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
    IDataChannel::States ConvertEnums::convert(RTCDataChannelState state)
    {
      switch (state) {
      case RTCDataChannelState::Connecting:   return IDataChannel::States::State_Connecting;
      case RTCDataChannelState::Open:         return IDataChannel::States::State_Open;
      case RTCDataChannelState::Closing:      return IDataChannel::States::State_Closing;
      case RTCDataChannelState::Closed:       return IDataChannel::States::State_Closed;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCDataChannelState ConvertEnums::convert(IDataChannel::States state)
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
    zsLib::Log::Level ConvertEnums::convert(Log::Level level)
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

    const char * ConvertEnums::toComponent(Log::Component  component)
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
    IDtlsTransport::States ConvertEnums::convert(RTCDtlsTransportState state)
    {
      switch (state) {
      case RTCDtlsTransportState::New:            return IDtlsTransport::States::State_New;
      case RTCDtlsTransportState::Connecting:     return IDtlsTransport::States::State_Connecting;
      case RTCDtlsTransportState::Connected:      return IDtlsTransport::States::State_Connected;
      case RTCDtlsTransportState::Validated:      return IDtlsTransport::States::State_Validated;
      case RTCDtlsTransportState::Closed:         return IDtlsTransport::States::State_Closed;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCDtlsTransportState ConvertEnums::convert(IDtlsTransport::States state)
    {
      switch (state) {
      case IDtlsTransport::States::State_New:           return RTCDtlsTransportState::New;
      case IDtlsTransport::States::State_Connecting:    return RTCDtlsTransportState::Connecting;
      case IDtlsTransport::States::State_Connected:     return RTCDtlsTransportState::Connected;
      case IDtlsTransport::States::State_Validated:     return RTCDtlsTransportState::Validated;
      case IDtlsTransport::States::State_Closed:        return RTCDtlsTransportState::Closed;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // RTCDtlsRole convert methods
    //---------------------------------------------------------------------------
    IDtlsTransport::Roles ConvertEnums::convert(RTCDtlsRole role)
    {
      switch (role) {
      case RTCDtlsRole::Auto:       return IDtlsTransport::Roles::Role_Auto;
      case RTCDtlsRole::Client:     return IDtlsTransport::Roles::Role_Client;
      case RTCDtlsRole::Server:     return IDtlsTransport::Roles::Role_Server;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCDtlsRole ConvertEnums::convert(IDtlsTransport::Roles role)
    {
      switch (role) {
      case IDtlsTransport::Roles::Role_Auto:      return RTCDtlsRole::Auto;
      case IDtlsTransport::Roles::Role_Client:    return RTCDtlsRole::Client;
      case IDtlsTransport::Roles::Role_Server:    return RTCDtlsRole::Server;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // RTCIceRole convert methods
    //---------------------------------------------------------------------------
    IICETypes::Roles ConvertEnums::convert(RTCIceRole role)
    {
      switch (role) {
      case RTCIceRole::Controlling:   return IICETypes::Roles::Role_Controlling;
      case RTCIceRole::Controlled:    return IICETypes::Roles::Role_Controlled;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceRole ConvertEnums::convert(IICETypes::Roles role)
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
    IICETypes::Components ConvertEnums::convert(RTCIceComponent component)
    {
      switch (component) {
      case RTCIceComponent::Rtp:   return IICETypes::Components::Component_RTP;
      case RTCIceComponent::Rtcp:  return IICETypes::Components::Component_RTCP;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceComponent ConvertEnums::convert(IICETypes::Components component)
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
    IICETypes::Protocols ConvertEnums::convert(RTCIceProtocol protocol)
    {
      switch (protocol) {
      case RTCIceProtocol::Udp:   return IICETypes::Protocols::Protocol_UDP;
      case RTCIceProtocol::Tcp:  return IICETypes::Protocols::Protocol_TCP;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceProtocol ConvertEnums::convert(IICETypes::Protocols protocol)
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
    IICETypes::CandidateTypes ConvertEnums::convert(RTCIceCandidateType candidateType)
    {
      switch (candidateType) {
      case RTCIceCandidateType::Host:   return IICETypes::CandidateTypes::CandidateType_Host;
      case RTCIceCandidateType::Srflex: return IICETypes::CandidateTypes::CandidateType_Srflex;
      case RTCIceCandidateType::Prflx:  return IICETypes::CandidateTypes::CandidateType_Prflx;
      case RTCIceCandidateType::Relay:  return IICETypes::CandidateTypes::CandidateType_Relay;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceCandidateType ConvertEnums::convert(IICETypes::CandidateTypes candidateType)
    {
      switch (candidateType) {
      case IICETypes::CandidateTypes::CandidateType_Host:    return RTCIceCandidateType::Host;
      case IICETypes::CandidateTypes::CandidateType_Srflex:  return RTCIceCandidateType::Srflex;
      case IICETypes::CandidateTypes::CandidateType_Prflx:   return RTCIceCandidateType::Prflx;
      case IICETypes::CandidateTypes::CandidateType_Relay:   return RTCIceCandidateType::Relay;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // RTCIceTcpCandidateType convert methods
    //---------------------------------------------------------------------------
    IICETypes::TCPCandidateTypes ConvertEnums::convert(RTCIceTcpCandidateType candidateType)
    {
      switch (candidateType) {
      case RTCIceTcpCandidateType::Active:   return IICETypes::TCPCandidateTypes::TCPCandidateType_Active;
      case RTCIceTcpCandidateType::Passive:  return IICETypes::TCPCandidateTypes::TCPCandidateType_Passive;
      case RTCIceTcpCandidateType::So:       return IICETypes::TCPCandidateTypes::TCPCandidateType_SO;
      }
      throw ref new Platform::NotImplementedException();
    }

    RTCIceTcpCandidateType ConvertEnums::convert(IICETypes::TCPCandidateTypes candidateType)
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
    IMediaStreamTrack::States ConvertEnums::convert(MediaStreamTrackState state)
    {
      switch (state) {
      case MediaStreamTrackState::Live:   return IMediaStreamTrack::States::State_Live;
      case MediaStreamTrackState::Ended:  return IMediaStreamTrack::States::State_Ended;
      }
      throw ref new Platform::NotImplementedException();
    }

    MediaStreamTrackState ConvertEnums::convert(IMediaStreamTrack::States state)
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
    IMediaStreamTrack::Kinds ConvertEnums::convert(MediaStreamTrackKind kind)
    {
      switch (kind) {
      case MediaStreamTrackKind::Audio:   return IMediaStreamTrack::Kinds::Kind_Audio;
      case MediaStreamTrackKind::Video:   return IMediaStreamTrack::Kinds::Kind_Video;
      }
      throw ref new Platform::NotImplementedException();
    }

    MediaStreamTrackKind ConvertEnums::convert(IMediaStreamTrack::Kinds kind)
    {
      switch (kind) {
      case IMediaStreamTrack::Kinds::Kind_Audio:   return MediaStreamTrackKind::Audio;
      case IMediaStreamTrack::Kinds::Kind_Video:   return MediaStreamTrackKind::Video;
      }
      throw ref new Platform::NotImplementedException();
    }
  } // namespace internal

  std::string FromCx(Platform::String^ inObj) {
    return rtc::ToUtf8(inObj->Data());
  }

  Platform::String^ ToCx(std::string const& inObj) {
    return ref new Platform::String(rtc::ToUtf16(inObj).c_str());
  }

  IICETypes::Candidate FromCx(RTCIceCandidate^ candidate)
  {
    IICETypes::Candidate ret;

    ret.mCandidateType = (IICETypes::CandidateTypes)candidate->CandidateType;
    ret.mFoundation = FromCx(candidate->Foundation);
    ret.mInterfaceType = FromCx(candidate->InterfaceType);
    ret.mIP = FromCx(candidate->Ip);
    ret.mPort = candidate->Port;
    ret.mPriority = candidate->Priority;
    ret.mProtocol = (IICETypes::Protocols)candidate->Protocol;
    ret.mRelatedAddress = FromCx(candidate->RelatedAddress);
    ret.mRelatedPort = candidate->RelatedPort;
    ret.mTCPType = (IICETypes::TCPCandidateTypes)candidate->TcpType;
    ret.mUnfreezePriority = candidate->UnfreezePriority;

    return ret;
  }

  RTCIceCandidate^ ToCx(ortc::IICETypes::CandidatePtr candidate)
  {
    RTCIceCandidate^ ret = ref new RTCIceCandidate();

    ret->CandidateType = (RTCIceCandidateType)candidate->mCandidateType;
    ret->Foundation = ToCx(candidate->mFoundation);
    ret->InterfaceType = ToCx(candidate->mInterfaceType);
    ret->Ip = ToCx(candidate->mIP);
    ret->Port = candidate->mPort;
    ret->Priority = candidate->mPriority;
    ret->Protocol = (RTCIceProtocol)candidate->mProtocol;
    ret->RelatedAddress = ToCx(candidate->mRelatedAddress);
    ret->RelatedPort = candidate->mRelatedPort;
    ret->TcpType = (RTCIceTcpCandidateType)candidate->mTCPType;
    ret->UnfreezePriority = candidate->mUnfreezePriority;

    return ret;
  }

  IICEGatherer::Options FromCx(RTCIceGatherOptions^ options)
  {
    IICEGatherer::Options ret;

    if (options)
    {
      IICEGatherer::InterfacePolicy interfacePolicy;
      interfacePolicy.mGatherPolicy = (IICEGatherer::FilterPolicies)options->GatherPolicy;
      ret.mInterfacePolicies.push_back(interfacePolicy);

      if (options->IceServers->Size > 0)
      {
        for (RTCIceServer^ srv : options->IceServers)
        {
          ortc::IICEGatherer::Server server;
          server.mUserName = FromCx(srv->UserName);
          server.mCredential = FromCx(srv->Credential);
          server.mCredentialType = internal::ConvertEnums::convert(srv->CredentialType);
          if (srv->URLs->Size > 0)
          {
            for (Platform::String^ url : srv->URLs)
            {
              server.mURLs.push_back(FromCx(url));
            }
          }
          ret.mICEServers.push_back(server);
        }

      }
    }
    return ret;
  }

  IDTLSTransportTypes::Parameters FromCx(RTCDtlsParameters^ parameters)
  {
    IDTLSTransportTypes::Parameters ret;

    if (parameters)
    {
      if (parameters->Fingerprints->Size > 0)
      {
        for (RTCDtlsFingerprint^ fingerprint : parameters->Fingerprints)
        {
          ICertificate::Fingerprint fing;
          fing.mAlgorithm = FromCx(fingerprint->Algorithm);
          fing.mValue = FromCx(fingerprint->Value);
          ret.mFingerprints.push_back(fing);
        }

        ret.mRole = (IDTLSTransportTypes::Roles)parameters->Role;
      }
    }
    return ret;
  }

  RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr parameters)
  {
    auto ret = ref new RTCDtlsParameters();

    if (parameters)
    {
      RTCDtlsFingerprint^ fingerprint = ref new RTCDtlsFingerprint();
      ret->Fingerprints = ref new Vector<RTCDtlsFingerprint^>();
      for (ICertificateTypes::FingerprintList::iterator it = parameters->mFingerprints.begin(); it != parameters->mFingerprints.end(); ++it)
      {
        fingerprint->Algorithm = ToCx((*it).mAlgorithm);
        fingerprint->Value = ToCx((*it).mValue);
        ret->Fingerprints->Append(fingerprint);
      }

      ret->Role = (RTCDtlsRole)parameters->mRole;
    }

    return ret;
  }

  IDataChannelTypes::Parameters FromCx(RTCDataChannelParameters^ parameters)
  {
    IDataChannelTypes::Parameters ret;

    ret.mLabel = FromCx(parameters->Label);
    ret.mOrdered = parameters->Ordered;
    ret.mMaxPacketLifetime = Milliseconds(parameters->MaxPacketLifetime);
    ret.mMaxRetransmits = parameters->MaxRetransmits;
    ret.mProtocol = FromCx(parameters->Protocol);
    ret.mNegotiated = parameters->Negotiated;
    ret.mID = parameters->Id;

    return ret;
  }

  RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr parameters)
  {
    RTCDataChannelParameters^ ret = ref new RTCDataChannelParameters();

    ret->Label = ToCx(parameters->mLabel);
    ret->Ordered = parameters->mOrdered;
    ret->MaxPacketLifetime = parameters->mMaxPacketLifetime.count();
    ret->MaxRetransmits = parameters->mMaxRetransmits;
    ret->Protocol = ToCx(parameters->mProtocol);
    ret->Negotiated = parameters->mNegotiated;
    ret->Id = parameters->mID;

    return ret;
  }

  RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr codecCapabilityPtr)
  {
    auto ret = ref new RTCRtpCodecCapability();

    ret->Name = ToCx(codecCapabilityPtr->mName);
    ret->Kind = ToCx(codecCapabilityPtr->mKind);
    ret->ClockRate = codecCapabilityPtr->mClockRate;
    ret->PreferredPayloadType = codecCapabilityPtr->mPreferredPayloadType;
    ret->Maxptime = codecCapabilityPtr->mMaxPTime;
    ret->NumChannels = codecCapabilityPtr->mNumChannels;

    //ret->rtcpFeedback = ref new Vector<RTCRtcpFeedback^>();

    for (IRTPTypes::RTCPFeedbackList::iterator it = codecCapabilityPtr->mRTCPFeedback.begin(); it != codecCapabilityPtr->mRTCPFeedback.end(); ++it)
    {
      auto feedback = ref new RTCRtcpFeedback();
      feedback->Parameter = ToCx(it->mParameter);
      feedback->Type = ToCx(it->mType);
      //ret->rtcpFeedback->Append(feedback);
    }

    //ret->parameters = codecCapabilityPtr->mParameters;
    //ret->options = codecCapabilityPtr->mOptions;
    ret->MaxTemporalLayers = codecCapabilityPtr->mMaxTemporalLayers; //default = 0;
    ret->MaxSpatialLayers = codecCapabilityPtr->mMaxSpatialLayers; //default = 0;
    ret->SvcMultiStreamSupport = codecCapabilityPtr->mSVCMultiStreamSupport;

    return ret;
  }

  RTCRtpHeaderExtension^ ToCx(IRTPTypes::HeaderExtensionPtr headerExtension)
  {
    auto ret = ref new RTCRtpHeaderExtension();

    ret->Kind = ToCx(headerExtension->mKind);
    ret->Uri = ToCx(headerExtension->mURI);
    ret->PreferredEncrypt = headerExtension->mPreferredEncrypt;
    ret->PreferredId = headerExtension->mPreferredID;
 
    return ret;
  }

  IRTPTypes::HeaderExtensionParameters FromCx(RTCRtpHeaderExtensionParameters^ headerExtensions)
  {
    IRTPTypes::HeaderExtensionParameters ret;

    ret.mURI = FromCx(headerExtensions->Uri);
    ret.mEncrypt = headerExtensions->Encrypt;
    ret.mID = headerExtensions->Id;

    return ret;
  }
 
  //***********************************************************************
  // ConvertObjectToCx class methods
  //***********************************************************************

  RTCIceGatherer^ ConvertObjectToCx::ToIceGatherer(IIceGathererPtr iceGatherer)
  {
    RTCIceGatherer^ ret = ref new RTCIceGatherer();
    ret->mNativePointer = iceGatherer;
    return ret;
  }

  RTCIceTransport^ ConvertObjectToCx::ToIceTransport(IIceTransportPtr iceTransport)
  {
    RTCIceTransport^ ret = ref new RTCIceTransport();
    ret->mNativePointer = iceTransport;
    return ret;
  }

  RTCCertificate^ ConvertObjectToCx::ToCertificate(ICertificatePtr certificate)
  {
    RTCCertificate^ ret = ref new RTCCertificate();
    ret->mNativePointer = certificate;
    return ret;
  }

  RTCDtlsTransport^ ConvertObjectToCx::ToDtlsTransport(IDtlsTransportPtr dtlsTransport)
  {
    RTCDtlsTransport^ ret = ref new RTCDtlsTransport();
    ret->mNativePointer = dtlsTransport;
    return ret;
  }

  RTCSctpTransport^ ConvertObjectToCx::ToSctpTransport(ISctpTransportPtr sctpTransport)
  {
    RTCSctpTransport^ ret = ref new RTCSctpTransport();
    ret->mNativePointer = sctpTransport;
    return ret;
  }

  MediaStreamTrack^ ConvertObjectToCx::ToMediaStreamTrack(IMediaStreamTrackPtr mediaStreamTrackPtr)
  {
	  auto ret = ref new MediaStreamTrack();
    ret->mNativePointer = mediaStreamTrackPtr;

	  return ret;
  }

  MediaTrackCapabilities^ ToCx(IMediaStreamTrackTypes::CapabilitiesPtr capabilitiesPtr)
  {
	  auto ret = ref new MediaTrackCapabilities();

	  /*ret->width = capabilitiesPtr->mWidth;
	  ret->height = capabilitiesPtr->mHeight;
	  ret->aspectRatio = capabilitiesPtr->mAspectRatio;
	  ret->frameRate = capabilitiesPtr->mFrameRate;
	  ret->facingMode = ToCx(capabilitiesPtr->mFacingMode);
	  ret->volume = capabilitiesPtr->mVolume;
	  ret->sampleRate = capabilitiesPtr->mSampleRate;
	  ret->sampleSize = capabilitiesPtr->mSampleSize;
	  ret->echoCancellation = capabilitiesPtr->mEchoCancellation;
	  ret->deviceId = ToCx(capabilitiesPtr->mDeviceID);
	  ret->groupId = ToCx(capabilitiesPtr->mGroupID);
	  */
	  return ret;
  }

  LongRange^ ToCx(IMediaStreamTrackTypes::CapabilityLong input)
  {
	  auto ret = ref new LongRange();

	  ret->Max = input.mMax;
	  ret->Min = input.mMin;

	  return ret;
  }


  template <typename  From, typename To> To^ convertConstrainToCx(From from)
  {
	  auto ret = ref new To();

	  ret->Max = from.mRange.value().mMax;
	  ret->Min = from.mRange.value().mMin;
	  ret->Exact = from.mRange.value().mExact;
	  ret->Ideal = from.mRange.value().mIdeal;
	  ret->Value = from.mValue;

	  return ret;
  }

  template <typename  From, typename To> To convertConstrainFromCx(From^ from)
  {
    To ret;

    ret.mRange.value().mMax = from->Max;
    ret.mRange.value().mMin = from->Min;
    ret.mRange.value().mExact = from->Exact;
    ret.mRange.value().mIdeal = from->Ideal;

    return ret;
  }

  StringOrStringList^ ToCx(ortc::IConstraints::StringOrStringList from)
  {
	  auto ret = ref new StringOrStringList();

	  if (from.mValue.hasValue())
		  ret->Value = ToCx(from.mValue.value());

	  if (from.mValues.hasValue())
	  {
		  ret->Values = ref new Vector<Platform::String^>();

		  for (IConstraints::StringList::iterator it = from.mValues.value().begin(); it != from.mValues.value().end(); ++it)
			  ret->Values->Append(ToCx(*it));
	  }
	  return ret;
  }

  ortc::IConstraints::StringOrStringList FromCx(StringOrStringList^ from)
  {
    ortc::IConstraints::StringOrStringList ret;

    if (!from->Value->IsEmpty())
      ret.mValue = FromCx(from->Value);

    if (from->Values->Size > 0)
    {
      for (Platform::String^ value : from->Values)
      {
        ret.mValues.value().push_back(FromCx(value));
      }
    }
    return ret;
  }

  ConstrainString^ convertConstrainToCx(ortc::IConstraints::ConstrainString from)
  {
	  auto ret = ref new ConstrainString();
	  ret->Value = ToCx(from.mValue);
	  ret->Parameters = ref new ConstrainStringParameters();
	  if (from.mParameters.hasValue())
	  {
		  ret->Parameters->Exact = ToCx(from.mParameters.value().mExact);
		  ret->Parameters->Ideal = ToCx(from.mParameters.value().mIdeal);
	  }
	  return ret;
  }

  ortc::IConstraints::ConstrainString convertConstrainFromCx(ConstrainString^ from)
  {
    ortc::IConstraints::ConstrainString ret;
    ret.mValue = FromCx(from->Value);
    if (from->Parameters)
    {
      ret.mParameters.value().mExact = FromCx(from->Parameters->Exact);
      ret.mParameters.value().mIdeal = FromCx(from->Parameters->Ideal);
    }
    return ret;
  }

  ConstrainBool^ convertConstrainToCx(ortc::IConstraints::ConstrainBool from)
  {
	  auto ret = ref new ConstrainBool();
	  ret->Value = from.mValue;
	  ret->Parameters = ref new ConstrainBoolParameters();
	  if (from.mParameters.hasValue())
	  {
		  ret->Parameters->Exact = from.mParameters.value().mExact;
		  ret->Parameters->Ideal = from.mParameters.value().mIdeal;
	  }
	  return ret;
  }

  ortc::IConstraints::ConstrainBool convertConstrainFromCx(ConstrainBool^ from)
  {
    ortc::IConstraints::ConstrainBool ret;
    ret.mValue = from->Value;
    if (from->Parameters)
    {
      ret.mParameters.value().mExact = from->Parameters->Exact;
      ret.mParameters.value().mIdeal = from->Parameters->Ideal;
    }
    return ret;
  }

  MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr constraintSetPtr)
  {
	  auto ret = ref new MediaTrackConstraintSet();

	  ret->Width = convertConstrainToCx<ortc::IConstraints::ConstrainLong,ConstrainLong>(constraintSetPtr->mWidth);
	  ret->Height = convertConstrainToCx<ortc::IConstraints::ConstrainLong, ConstrainLong>(constraintSetPtr->mHeight);
	  ret->AspectRatio = convertConstrainToCx<ortc::IConstraints::ConstrainDouble, ConstrainDouble>(constraintSetPtr->mAspectRatio);
	  ret->FrameRate = convertConstrainToCx<ortc::IConstraints::ConstrainDouble, ConstrainDouble>(constraintSetPtr->mFrameRate);

	  ret->FacingMode = convertConstrainToCx(constraintSetPtr->mFacingMode);
	  ret->Volume = convertConstrainToCx<ortc::IConstraints::ConstrainDouble, ConstrainDouble>(constraintSetPtr->mVolume);
	  ret->SampleRate = convertConstrainToCx<ortc::IConstraints::ConstrainLong, ConstrainLong>(constraintSetPtr->mSampleRate);
	  ret->SampleSize = convertConstrainToCx<ortc::IConstraints::ConstrainLong, ConstrainLong>(constraintSetPtr->mSampleSize);

	  ret->EchoCancellation = convertConstrainToCx(constraintSetPtr->mEchoCancellation);
	  ret->DeviceId = convertConstrainToCx(constraintSetPtr->mDeviceID);
	  ret->GroupId = convertConstrainToCx(constraintSetPtr->mGroupID);

	  return ret;
  }

  IMediaStreamTrackTypes::ConstraintSetPtr FromCx(MediaTrackConstraintSet^ constraintSet)
  {
    IMediaStreamTrackTypes::ConstraintSetPtr ret;

    ret->mWidth = convertConstrainFromCx<ConstrainLong, ortc::IConstraints::ConstrainLong>(constraintSet->Width);
    ret->mHeight = convertConstrainFromCx<ConstrainLong, ortc::IConstraints::ConstrainLong>(constraintSet->Height);
    ret->mAspectRatio = convertConstrainFromCx<ConstrainDouble, ortc::IConstraints::ConstrainDouble>(constraintSet->AspectRatio);
    ret->mFrameRate = convertConstrainFromCx<ConstrainDouble, ortc::IConstraints::ConstrainDouble>(constraintSet->FrameRate);

    ret->mFacingMode = convertConstrainFromCx(constraintSet->FacingMode);
    ret->mVolume = convertConstrainFromCx<ConstrainDouble, ortc::IConstraints::ConstrainDouble>(constraintSet->Volume);
    ret->mSampleRate = convertConstrainFromCx<ConstrainLong, ortc::IConstraints::ConstrainLong>(constraintSet->SampleRate);
    ret->mSampleSize = convertConstrainFromCx<ConstrainLong, ortc::IConstraints::ConstrainLong>(constraintSet->SampleSize);

    ret->mEchoCancellation = convertConstrainFromCx(constraintSet->EchoCancellation);
    ret->mDeviceID = convertConstrainFromCx(constraintSet->DeviceId);
    ret->mGroupID = convertConstrainFromCx(constraintSet->GroupId);

    return ret;
  }

  Constraints^ ToCx(IMediaStreamTrackTypes::ConstraintsPtr constraintPtr)
  {
	  return nullptr;
  }


  IMediaStreamTrackTypes::ConstraintsPtr FromCx(Constraints^ constraint)
  {
	  IMediaStreamTrackTypes::ConstraintsPtr ret = IMediaStreamTrackTypes::Constraints::create();

	  if (constraint->Audio != nullptr)
	  {
		  for (MediaTrackConstraintSet^ trackConstraint : constraint->Audio->Advanced)
		  {
			  ortc::IMediaDevicesTypes::ConstraintSetPtr constraintSet = ortc::IMediaDevicesTypes::ConstraintSet::create();
			  constraintSet->mDeviceID.mValue.value().mValue.value() = FromCx(trackConstraint->DeviceId->Value->Value);
			  //constraintSet->mWidth.mValue.value() = trackConstraint->Width->Value;
			  //constraintSet->mHeight.mValue.value() = trackConstraint->Height->Value;
			  //constraintSet->mFrameRate.mValue.value() = trackConstraint->FrameRate->Value;
			  //constraintSet->mAspectRatio.mValue.value() = trackConstraint->AspectRatio->Value;
			  
			  if (trackConstraint->Volume)
				  constraintSet->mVolume.mValue.value() = trackConstraint->Volume->Value;
			  if (trackConstraint->SampleRate)
				constraintSet->mSampleRate.mValue.value() = trackConstraint->SampleRate->Value;
			  if(trackConstraint->SampleSize)
				constraintSet->mSampleSize.mValue.value() = trackConstraint->SampleSize->Value;
			  if (trackConstraint->EchoCancellation)
				constraintSet->mEchoCancellation.mValue.value() = trackConstraint->EchoCancellation->Value;

			  if (trackConstraint->GroupId)
				constraintSet->mGroupID.mValue.value().mValue.value() = FromCx(trackConstraint->GroupId->Value->Value);

			  if (trackConstraint->FacingMode)
				constraintSet->mFacingMode.mValue.value().mValue.value() = FromCx(trackConstraint->FacingMode->Value->Value);

			  ret->mAudio = IMediaStreamTrackTypes::TrackConstraints::create();
			  ret->mAudio->mAdvanced.push_back(constraintSet);
		  }
	  }

	  if (constraint->Video != nullptr)
	  {
		  for (MediaTrackConstraintSet^ trackConstraint : constraint->Video->Advanced)
		  {
			  ortc::IMediaDevicesTypes::ConstraintSetPtr constraintSet = ortc::IMediaDevicesTypes::ConstraintSet::create();
			  constraintSet->mDeviceID.mValue.value().mValue.value() = FromCx(trackConstraint->DeviceId->Value->Value);
			  if (trackConstraint->Width)
				constraintSet->mWidth.mValue.value() = trackConstraint->Width->Value;

			  if (trackConstraint->Height)
				constraintSet->mHeight.mValue.value() = trackConstraint->Height->Value;

			  if (trackConstraint->FrameRate)
				constraintSet->mFrameRate.mValue.value() = trackConstraint->FrameRate->Value;

			  if (trackConstraint->AspectRatio)
				constraintSet->mAspectRatio.mValue.value() = trackConstraint->AspectRatio->Value;
			  //constraintSet->mVolume.mValue.value() = trackConstraint->Volume->Value;
			  //constraintSet->mSampleRate.mValue.value() = trackConstraint->SampleRate->Value;
			  //constraintSet->mSampleSize.mValue.value() = trackConstraint->SampleSize->Value;
			  //constraintSet->mEchoCancellation.mValue.value() = trackConstraint->EchoCancellation->Value;

			  if (trackConstraint->GroupId)
				constraintSet->mGroupID.mValue.value().mValue.value() = FromCx(trackConstraint->GroupId->Value->Value);

			  if (trackConstraint->FacingMode)
				constraintSet->mFacingMode.mValue.value().mValue.value() = FromCx(trackConstraint->FacingMode->Value->Value);

			  ret->mVideo = IMediaStreamTrackTypes::TrackConstraints::create();
			  ret->mVideo->mAdvanced.push_back(constraintSet);
		  }
	  }

	  return ret;
  }
  MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr trackConstraintsPtr)
  {
	  auto ret = ref new MediaTrackConstraints();

	  for (IMediaStreamTrackTypes::ConstraintSetList::iterator it = trackConstraintsPtr->mAdvanced.begin(); it != trackConstraintsPtr->mAdvanced.end(); ++it)
	  {
		  MediaTrackConstraintSet^ mediaTrackConstraintSet = ToCx(*it);
		  ret->Advanced->Append(mediaTrackConstraintSet);
	  }

	  return ret;
  }

  MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr settingsPtr)
  {
	  auto ret = ref new MediaTrackSettings();

	  ret->Width = settingsPtr->mWidth.value();
	  ret->Height = settingsPtr->mHeight.value();
	  ret->AspectRatio = settingsPtr->mAspectRatio.value();
	  ret->FrameRate = settingsPtr->mFrameRate.value();
	  ret->FacingMode = ToCx(settingsPtr->mFacingMode.value());
	  ret->Volume = settingsPtr->mVolume.value();
	  ret->SampleRate = settingsPtr->mSampleRate.value();
	  ret->SampleSize = settingsPtr->mSampleSize.value();
	  ret->EchoCancellation = settingsPtr->mEchoCancellation.value();
	  ret->DeviceId = ToCx(settingsPtr->mDeviceID.value());
	  ret->GroupId = ToCx(settingsPtr->mGroupID.value());
	  
	  return ret;
  }

  SupportedConstraints^ ToCx(IMediaDevicesTypes::SupportedConstraints constraints)
  {
	  auto ret = ref new SupportedConstraints();

	  ret->Width = constraints.mWidth;
	  ret->Height = constraints.mHeight;
	  ret->AspectRatio = constraints.mAspectRatio;
	  ret->FrameRate = constraints.mFrameRate;
	  ret->FacingMode = constraints.mFacingMode;
	  ret->Volume = constraints.mVolume;
	  ret->SampleRate = constraints.mSampleRate;
	  ret->SampleSize = constraints.mSampleSize;
	  ret->EchoCancellation = constraints.mEchoCancellation;
	  ret->Latency = constraints.mLatency;
	  ret->DeviceID = constraints.mDeviceID;
	  ret->GroupID = constraints.mGroupID;

	  return ret;
  }

  MediaDeviceInfo^ ToCx(IMediaDevicesTypes::Device device)
  {
	  auto ret = ref new MediaDeviceInfo();

	  ret->Kind = internal::convert(device.mKind);

	  ret->Label = ToCx(device.mLabel);
	  ret->DeviceID = ToCx(device.mDeviceID);
	  ret->GroupID = ToCx(device.mGroupID);

	  ret->SupportedConstraints = ToCx(device.mSupportedConstraints);

	  return ret;
  }

  IRTPTypes::Parameters FromCx(RTCRtpParameters^ parameters)
  {
	  IRTPTypes::Parameters ret;

	  ret.mMuxID = FromCx(parameters->MuxId);

	  for (RTCRtpCodecParameters^ codec : parameters->Codecs)
	  {
		  IRTPTypes::CodecParameters codecCore;
		  codecCore.mName = FromCx(codec->Name);
		  codecCore.mPayloadType = codec->PayloadType;
		  codecCore.mClockRate = codec->ClockRate;
		  codecCore.mMaxPTime = codec->Maxptime;
		  codecCore.mNumChannels = codec->NumChannels;

		  for (RTCRtcpFeedback^ feedback : codec->RtcpFeedback)
		  {
			  IRTPTypes::RTCPFeedback feedbackCore;
			  feedbackCore.mParameter = FromCx(feedback->Parameter);
			  feedbackCore.mType = FromCx(feedback->Type);
			  codecCore.mRTCPFeedback.push_back(feedbackCore);
		  }

		  //TODO: AnyPtr            mParameters;

		  ret.mCodecs.push_back(codecCore);
	  }

	  for (RTCRtpHeaderExtensionParameters^ headerExtension : parameters->HeaderExtensions)
	  {
		  IRTPTypes::HeaderExtensionParameters extensionCore;
		  extensionCore.mEncrypt = headerExtension->Encrypt;
		  extensionCore.mID = headerExtension->Id;
		  extensionCore.mURI = FromCx(headerExtension->Uri);

		  ret.mHeaderExtensions.push_back(extensionCore);
	  }

	  for (RTCRtpEncodingParameters^ encodingParameters : parameters->Encodings)
	  {
		  IRTPTypes::EncodingParameters encodingParametersCore;

		  encodingParametersCore.mSSRC = encodingParameters->Ssrc;
		  encodingParametersCore.mCodecPayloadType = encodingParameters->CodecPayloadType;
		  encodingParametersCore.mFEC.value().mSSRC = encodingParameters->Fec->Ssrc;
		  encodingParametersCore.mFEC.value().mMechanism = FromCx(encodingParameters->Fec->Mechanism);
		  encodingParametersCore.mRTX.value().mSSRC = encodingParameters->Rtx->Ssrc;
		  encodingParametersCore.mRTX.value().mPayloadType = encodingParameters->Rtx->PayloadType;
		  encodingParametersCore.mPriority = (IRTPTypes::PriorityTypes)encodingParameters->Priority;
		  encodingParametersCore.mMaxBitrate = encodingParameters->MaxBitrate;
		  encodingParametersCore.mMinQuality = encodingParameters->MinQuality;
		  encodingParametersCore.mActive = encodingParameters->Active;
		  encodingParametersCore.mEncodingID = FromCx(encodingParameters->EncodingId);

		  for (Platform::String^ dependencyEncodingID : encodingParameters->DependencyEncodingIds)
		  {
			  encodingParametersCore.mDependencyEncodingIDs.push_back(FromCx(dependencyEncodingID));
		  }
		  ret.mEncodingParameters.push_back(encodingParametersCore);
	  }

	  ret.mRTCP.mCName = FromCx(parameters->Rtcp->CName);
	  ret.mRTCP.mMux = parameters->Rtcp->Mux;
	  ret.mRTCP.mReducedSize = parameters->Rtcp->ReducedSize;
	  ret.mRTCP.mSSRC = parameters->Rtcp->Ssrc;

	  ret.mDegredationPreference = (IRTPTypes::DegradationPreferences)parameters->DegradationPreference;

	  return ret;
  }

  IICETypes::Parameters FromCx(RTCIceParameters^ params)
  {
    IICETypes::Parameters ret;
    ret.mUsernameFragment = FromCx(params->UsernameFragment);
    ret.mPassword = FromCx(params->Password);
    return ret;
  }

  ISCTPTransport::Capabilities FromCx(RTCSctpCapabilities^ caps)
  {
    ISCTPTransport::Capabilities ret;
    ret.mMaxMessageSize = caps->MaxMessageSize;
    return ret;
  }

  IRTPTypes::Capabilities FromCx(RTCRtpCapabilities^ caps)
  {
    IRTPTypes::Capabilities ret;
    for (RTCRtpCodecCapability^ c: caps->Codecs)
    {
      auto codec = FromCx(c);
      ret.mCodecs.push_back(codec);
    }

    for (RTCRtpHeaderExtension^ ext : caps->HeaderExtensions)
    {
      auto extension = FromCx(ext);
      ret.mHeaderExtensions.push_back(extension);
    }

    for (Platform::String^ fec : caps->FecMechanisms)
    {
      ret.mFECMechanisms.push_back(FromCx(fec));
    }
    return ret;
  }

  IRTPTypes::CodecCapability FromCx(RTCRtpCodecCapability^ cap)
  {
    IRTPTypes::CodecCapability ret;
    ret.mName = FromCx(cap->Name);
    ret.mKind = FromCx(cap->Kind);
    ret.mClockRate = cap->ClockRate;
    //ret->preferredPayloadType = codecCapabilityPtr->mPreferredPayloadType;
    ret.mMaxPTime = cap->Maxptime;
    ret.mNumChannels = cap->NumChannels;

    //ret->rtcpFeedback = ref new Vector<RTCRtcpFeedback^>();

    if (cap->RtcpFeedback)
    {
      for (RTCRtcpFeedback^ fb : cap->RtcpFeedback)
      {
        ret.mRTCPFeedback.push_back(FromCx(fb));
      }
    }

    //ret->parameters = codecCapabilityPtr->mParameters;
    //ret->options = codecCapabilityPtr->mOptions;
    ret.mMaxTemporalLayers = cap->MaxTemporalLayers; //default = 0;
    ret.mMaxSpatialLayers = cap->MaxSpatialLayers; //default = 0;
    ret.mSVCMultiStreamSupport = cap->SvcMultiStreamSupport;
    return ret;
  }

  IRTPTypes::RTCPFeedback FromCx(RTCRtcpFeedback^ fb)
  {
    IRTPTypes::RTCPFeedback ret;
    ret.mParameter = FromCx(fb->Parameter);
    ret.mType = FromCx(fb->Type);
    return ret;
  }

  IRTPTypes::HeaderExtension FromCx(RTCRtpHeaderExtension^ ext)
  {
    IRTPTypes::HeaderExtension ret;
    ret.mKind = FromCx(ext->Kind);
    ret.mURI = FromCx(ext->Uri);
    ret.mPreferredEncrypt = ext->PreferredEncrypt;
    ret.mPreferredID = ext->PreferredId;
    return ret;
  }
} // namespace ortc_winrt_api
