#include "pch.h"

#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using namespace Platform;
using Platform::Collections::Vector;

namespace ortc_winrt_api
{
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
      ret.mInterfacePolicies.push_back(interfacePolicy);

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

    ret->Name = ToCx(codecCapabilityPtr->mName);
    ret->Kind = ToCx(codecCapabilityPtr->mKind);
    ret->ClockRate = codecCapabilityPtr->mClockRate;
    //ret->preferredPayloadType = codecCapabilityPtr->mPreferredPayloadType;
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

  RTCRtpHeaderExtensions^ ToCx(IRTPTypes::HeaderExtensionPtr headerExtensions)
  {
    auto ret = ref new RTCRtpHeaderExtensions();

    ret->Kind = ToCx(headerExtensions->mKind);
    ret->Uri = ToCx(headerExtensions->mURI);
    ret->PreferredEncrypt = headerExtensions->mPreferredEncrypt;
    ret->PreferredId = headerExtensions->mPreferredID;

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

	  ret->Kind = (MediaDeviceKinds)device.mKind;

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

} // namespace ortc_winrt_api
