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

  RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr codecCapabilityPtr)
  {
    auto ret = ref new RTCRtpCodecCapability();

    ret->name = ToCx(codecCapabilityPtr->mName);
    ret->kind = ToCx(codecCapabilityPtr->mKind);
    ret->clockRate = codecCapabilityPtr->mClockRate;
    //ret->preferredPayloadType = codecCapabilityPtr->mPreferredPayloadType;
    ret->maxptime = codecCapabilityPtr->mMaxPTime;
    ret->numChannels = codecCapabilityPtr->mNumChannels;

    //ret->rtcpFeedback = ref new Vector<RTCRtcpFeedback^>();

    for (IRTPTypes::RTCPFeedbackList::iterator it = codecCapabilityPtr->mFeedback.begin(); it != codecCapabilityPtr->mFeedback.end(); ++it)
    {
      auto feedback = ref new RTCRtcpFeedback();
      feedback->parameter = ToCx(it->mParameter);
      feedback->type = ToCx(it->mType);
      //ret->rtcpFeedback->Append(feedback);
    }

    //ret->parameters = codecCapabilityPtr->mParameters;
    //ret->options = codecCapabilityPtr->mOptions;
    ret->maxTemporalLayers = codecCapabilityPtr->mMaxTemporalLayers; //default = 0;
    ret->maxSpatialLayers = codecCapabilityPtr->mMaxSpatialLayers; //default = 0;
    ret->svcMultiStreamSupport = codecCapabilityPtr->mSVCMultiStreamSupport;

    return ret;
  }

  RTCRtpHeaderExtensions^ ToCx(IRTPTypes::HeaderExtensionsPtr headerExtensions)
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

  RTCIceTransport^ ConvertObjectToCx::ToIceTransport(IIceTransportPtr iceTransport)
  {
    RTCIceTransport^ ret = ref new RTCIceTransport();
    ret->mNativePointer = iceTransport;
    return ret;
  }

  MediaStreamTrack^ ConvertObjectToCx::ToMediaStreamTrack(IMediaStreamTrackPtr mediaStreamTrackPtr)
  {
	  auto ret = ref new MediaStreamTrack();

	  ret->_kind = (MediaStreamTrackKind)mediaStreamTrackPtr->kind();
	  ret->_id = ToCx(mediaStreamTrackPtr->id());
	  ret->_label = ToCx(mediaStreamTrackPtr->label());
	  ret->_enabled = mediaStreamTrackPtr->enabled();
	  ret->_muted = mediaStreamTrackPtr->muted();
	  ret->_readonly = mediaStreamTrackPtr->readOnly();
	  ret->_remote = mediaStreamTrackPtr->remote();
	  ret->_readyState = (MediaStreamTrackState)mediaStreamTrackPtr->readyState();

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

	  ret->max = input.mMax;
	  ret->min = input.mMin;

	  return ret;
  }


  template <typename  From, typename To> To^ convertConstrainToCx(From from)
  {
	  auto ret = ref new To();

	  ret->max = from.mRange.value().mMax;
	  ret->min = from.mRange.value().mMin;
	  ret->exact = from.mRange.value().mExact;
	  ret->ideal = from.mRange.value().mIdeal;
	  ret->value = from.mValue;

	  return ret;
  }

  StringOrStringList^ ToCx(ortc::IConstraints::StringOrStringList from)
  {
	  auto ret = ref new StringOrStringList();

	  if (from.mValue.hasValue())
		  ret->value = ToCx(from.mValue.value());

	  if (from.mValues.hasValue())
	  {
		  ret->values = ref new Vector<Platform::String^>();

		  for (IConstraints::StringList::iterator it = from.mValues.value().begin(); it != from.mValues.value().end(); ++it)
			  ret->values->Append(ToCx(*it));
	  }
	  return ret;
  }
  ConstrainString^ convertConstrainToCx(ortc::IConstraints::ConstrainString from)
  {
	  auto ret = ref new ConstrainString();
	  ret->value = ToCx(from.mValue);
	  ret->parameters = ref new ConstrainStringParameters();
	  if (from.mParameters.hasValue())
	  {
		  ret->parameters->exact = ToCx(from.mParameters.value().mExact);
		  ret->parameters->ideal = ToCx(from.mParameters.value().mIdeal);
	  }
	  return ret;
  }

  ConstrainBool^ convertConstrainToCx(ortc::IConstraints::ConstrainBool from)
  {
	  auto ret = ref new ConstrainBool();
	  ret->value = from.mValue;
	  ret->parameters = ref new ConstrainBoolParameters();
	  if (from.mParameters.hasValue())
	  {
		  ret->parameters->exact = from.mParameters.value().mExact;
		  ret->parameters->ideal = from.mParameters.value().mIdeal;
	  }
	  return ret;
  }

  MediaTrackConstraintSet^ ToCx(IMediaStreamTrackTypes::ConstraintSetPtr constraintSetPtr)
  {
	  auto ret = ref new MediaTrackConstraintSet();

	  ret->width = convertConstrainToCx<ortc::IConstraints::ConstrainLong,ConstrainLong>(constraintSetPtr->mWidth);
	  ret->height = convertConstrainToCx<ortc::IConstraints::ConstrainLong, ConstrainLong>(constraintSetPtr->mHeight);
	  ret->aspectRatio = convertConstrainToCx<ortc::IConstraints::ConstrainDouble, ConstrainDouble>(constraintSetPtr->mAspectRatio);
	  ret->frameRate = convertConstrainToCx<ortc::IConstraints::ConstrainDouble, ConstrainDouble>(constraintSetPtr->mFrameRate);

	  ret->facingMode = convertConstrainToCx(constraintSetPtr->mFacingMode);
	  ret->volume = convertConstrainToCx<ortc::IConstraints::ConstrainDouble, ConstrainDouble>(constraintSetPtr->mVolume);
	  ret->sampleRate = convertConstrainToCx<ortc::IConstraints::ConstrainLong, ConstrainLong>(constraintSetPtr->mSampleRate);
	  ret->sampleSize = convertConstrainToCx<ortc::IConstraints::ConstrainLong, ConstrainLong>(constraintSetPtr->mSampleSize);

	  ret->echoCancellation = convertConstrainToCx(constraintSetPtr->mEchoCancellation);
	  ret->deviceId = convertConstrainToCx(constraintSetPtr->mDeviceID);
	  ret->groupId = convertConstrainToCx(constraintSetPtr->mGroupID);

	  return ret;
  }

  MediaTrackConstraints^ ToCx(IMediaStreamTrackTypes::TrackConstraintsPtr trackConstraintsPtr)
  {
	  auto ret = ref new MediaTrackConstraints();

	  for (IMediaStreamTrackTypes::ConstraintSetList::iterator it = trackConstraintsPtr->mAdvanced.begin(); it != trackConstraintsPtr->mAdvanced.end(); ++it)
	  {
		  MediaTrackConstraintSet^ mediaTrackConstraintSet = ToCx(*it);
		  ret->advanced->Append(mediaTrackConstraintSet);
	  }

	  return ret;
  }

  MediaTrackSettings^ ToCx(IMediaStreamTrackTypes::SettingsPtr settingsPtr)
  {
	  auto ret = ref new MediaTrackSettings();

	  ret->width = settingsPtr->mWidth.value();
	  ret->height = settingsPtr->mHeight.value();
	  ret->aspectRatio = settingsPtr->mAspectRatio.value();
	  ret->frameRate = settingsPtr->mFrameRate.value();
	  ret->facingMode = ToCx(settingsPtr->mFacingMode.value());
	  ret->volume = settingsPtr->mVolume.value();
	  ret->sampleRate = settingsPtr->mSampleRate.value();
	  ret->sampleSize = settingsPtr->mSampleSize.value();
	  ret->echoCancellation = settingsPtr->mEchoCancellation.value();
	  ret->deviceId = ToCx(settingsPtr->mDeviceID.value());
	  ret->groupId = ToCx(settingsPtr->mGroupID.value());
	  
	  return ret;
  }
  IRTPTypes::Parameters FromCx(RTCRtpParameters^ parameters)
  {
	  IRTPTypes::Parameters ret;

	  ret.mMuxID = FromCx(parameters->muxId);

	  for (RTCRtpCodecParameters^ codec : parameters->codecs)
	  {
		  IRTPTypes::CodecParameters codecCore;
		  codecCore.mName = FromCx(codec->name);
		  codecCore.mPayloadType = codec->payloadType;
		  codecCore.mClockRate = codec->clockRate;
		  codecCore.mMaxPTime = codec->maxptime;
		  codecCore.mNumChannels = codec->numChannels;

		  for (RTCRtcpFeedback^ feedback : codec->rtcpFeedback)
		  {
			  IRTPTypes::RTCPFeedback feedbackCore;
			  feedbackCore.mParameter = FromCx(feedback->parameter);
			  feedbackCore.mType = FromCx(feedback->type);
			  codecCore.mRTCPFeedback.push_back(feedbackCore);
		  }

		  //TODO: AnyPtr            mParameters;

		  ret.mCodecs.push_back(codecCore);
	  }

	  for (RTCRtpHeaderExtensionParameters^ headerExtension : parameters->headerExtensions)
	  {
		  IRTPTypes::HeaderExtensionParameters extensionCore;
		  extensionCore.mEncrypt = headerExtension->encrypt;
		  extensionCore.mID = headerExtension->id;
		  extensionCore.mURI = FromCx(headerExtension->uri);

		  ret.mHeaderExtensions.push_back(extensionCore);
	  }

	  for (RTCRtpEncodingParameters^ encodingParameters : parameters->encodings)
	  {
		  IRTPTypes::EncodingParameters encodingParametersCore;

		  encodingParametersCore.mSSRC = encodingParameters->ssrc;
		  encodingParametersCore.mCodecPayloadType = encodingParameters->codecPayloadType;
		  encodingParametersCore.mFEC.value().mSSRC = encodingParameters->fec->ssrc;
		  encodingParametersCore.mFEC.value().mMechanism = FromCx(encodingParameters->fec->mechanism);
		  encodingParametersCore.mRTX.value().mSSRC = encodingParameters->rtx->ssrc;
		  encodingParametersCore.mRTX.value().mPayloadType = encodingParameters->rtx->payloadType;
		  encodingParametersCore.mPriority = (IRTPTypes::PriorityTypes)encodingParameters->priority;
		  encodingParametersCore.mMaxBitrate = encodingParameters->maxBitrate;
		  encodingParametersCore.mMinQuality = encodingParameters->minQuality;
		  encodingParametersCore.mActive = encodingParameters->active;
		  encodingParametersCore.mEncodingID = FromCx(encodingParameters->encodingId);

		  for (Platform::String^ dependencyEncodingID : encodingParameters->dependencyEncodingIds)
		  {
			  encodingParametersCore.mDependencyEncodingIDs.push_back(FromCx(dependencyEncodingID));
		  }
		  ret.mEncodingParameters.push_back(encodingParametersCore);
	  }

	  ret.mRTCP.mCName = FromCx(parameters->rtcp->cname);
	  ret.mRTCP.mMux = parameters->rtcp->mux;
	  ret.mRTCP.mReducedSize = parameters->rtcp->reducedSize;
	  ret.mRTCP.mSSRC = parameters->rtcp->ssrc;

	  ret.mDegredationPreference = (IRTPTypes::DegradationPreferences)parameters->degradationPreference;

	  return ret;
  }

} // namespace ortc_winrt_api