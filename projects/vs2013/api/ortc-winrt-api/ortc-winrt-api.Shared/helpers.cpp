#include "pch.h"

#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using namespace Platform;
using Platform::Collections::Vector;

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

namespace ortc_winrt_api
{

  ConfigureOrtcEngine::ConfigureOrtcEngine()
  {
    openpeer::services::ILogger::setLogLevel(zsLib::Log::Trace);
    openpeer::services::ILogger::setLogLevel("zsLib", zsLib::Log::Trace);
    openpeer::services::ILogger::setLogLevel("openpeer_services", zsLib::Log::Trace);
    openpeer::services::ILogger::setLogLevel("openpeer_services_http", zsLib::Log::Trace);
    openpeer::services::ILogger::setLogLevel("ortclib", zsLib::Log::Insane);
    openpeer::services::ILogger::setLogLevel("ortc_standup", zsLib::Log::Insane);

    //openpeer::services::ILogger::installDebuggerLogger();
    openpeer::services::ILogger::installTelnetLogger(59999, 60, true);

    ortc::ISettings::applyDefaults();
  }


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
    ret.mIP = FromCx(candidate->IP);
    ret.mPort = candidate->Port;
    ret.mPriority = candidate->Priority;
    ret.mProtocol = (IICETypes::Protocols)candidate->Protocol;
    ret.mRelatedAddress = FromCx(candidate->RelatedAddress);
    ret.mRelatedPort = candidate->RelatedPort;
    ret.mTCPType = (IICETypes::TCPCandidateTypes)candidate->TCPType;
    ret.mUnfreezePriority = candidate->UnfreezePriority;

    return ret;
  }

  RTCIceCandidate^ ToCx(ortc::IICETypes::CandidatePtr candidate)
  {
    RTCIceCandidate^ ret = ref new RTCIceCandidate();

    ret->CandidateType = (RTCIceCandidateType)candidate->mCandidateType;
    ret->Foundation = ToCx(candidate->mFoundation);
    ret->InterfaceType = ToCx(candidate->mInterfaceType);
    ret->IP = ToCx(candidate->mIP);
    ret->Port = candidate->mPort;
    ret->Priority = candidate->mPriority;
    ret->Protocol = (RTCIceProtocol)candidate->mProtocol;
    ret->RelatedAddress = ToCx(candidate->mRelatedAddress);
    ret->RelatedPort = candidate->mRelatedPort;
    ret->TCPType = (RTCIceTcpCandidateType)candidate->mTCPType;
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
      ret.mInterfacePolicy.push_back(interfacePolicy);

      if (options->IceServers->Size > 0)
      {
        for (RTCIceServer^ srv : options->IceServers)
        {
          ortc::IICEGatherer::Server server;
          server.mUserName = FromCx(srv->UserName);
          server.mCredential = FromCx(srv->Credential);
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

  RTCRtpCodecCapability^ toCx(IRTPTypes::CodecCapabilityPtr codecCapabilityPtr)
  {
    auto ret = ref new RTCRtpCodecCapability();

    ret->name = ToCx(codecCapabilityPtr->mName);
    ret->kind = ToCx(codecCapabilityPtr->mKind);
    ret->clockRate = codecCapabilityPtr->mClockRate;
    //ret->preferredPayloadType = codecCapabilityPtr->mPreferredPayloadType;
    ret->maxptime = codecCapabilityPtr->mMaxPTime;
    ret->numChannels = codecCapabilityPtr->mNumChannels;

    ret->rtcpFeedback = ref new Vector<RTCRtcpFeedback^>();

    for (IRTPTypes::RTCPFeedbackList::iterator it = codecCapabilityPtr->mFeedback.begin(); it != codecCapabilityPtr->mFeedback.end(); ++it)
    {
      auto feedback = ref new RTCRtcpFeedback();
      feedback->parameter = ToCx(it->mParameter);
      feedback->type = ToCx(it->mType);
      ret->rtcpFeedback->Append(feedback);
    }

    //ret->parameters = codecCapabilityPtr->mParameters;
    //ret->options = codecCapabilityPtr->mOptions;
    ret->maxTemporalLayers = codecCapabilityPtr->mMaxTemporalLayers; //default = 0;
    ret->maxSpatialLayers = codecCapabilityPtr->mMaxSpatialLayers; //default = 0;
    ret->svcMultiStreamSupport = codecCapabilityPtr->mSVCMultiStreamSupport;

    return ret;
  }

  RTCRtpHeaderExtensions^ toCx(IRTPTypes::HeaderExtensionsPtr headerExtensions)
  {
    auto ret = ref new RTCRtpHeaderExtensions();

    ret->kind = ToCx(headerExtensions->mKind);
    ret->uri = ToCx(headerExtensions->mURI);
    ret->preferredEncrypt = headerExtensions->mPreferredEncrypt;
    ret->preferredId = headerExtensions->mPreferredID;

    return ret;
  }
  //***********************************************************************
  // ConvertObjectToCx class methods
  //***********************************************************************

  RTCIceTransport^ ConvertObjectToCx::iceTransport(IIceTransportPtr iceTransport)
  {
    RTCIceTransport^ ret = ref new RTCIceTransport();
    ret->mNativePointer = iceTransport;
    return ret;
  }


} // namespace ortc_winrt_api