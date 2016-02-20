#pragma once
#include "pch.h"
#include "helpers.h"
#include <collection.h>

#include <openpeer/services/IHelper.h>

using namespace ortc;

namespace ortc_winrt_api
{
  //---------------------------------------------------------------------------
  // RTCRtpCapabilities methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtpCapabilities::ToJsonString()
  {
    auto caps = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(caps->createElement("RtpCapabilities")));
  }

  RTCRtpCapabilities^ RTCRtpCapabilities::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtpCapabilities();
	
    auto capabilitiesPtr = make_shared<IRTPTypes::Capabilities>(IRTPTypes::Capabilities::Capabilities(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));

    return ToCx(capabilitiesPtr);
  }

  //---------------------------------------------------------------------------
  // RTCRtpCodecCapability methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtpCodecCapability::ToJsonString()
  {
    auto capability = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(capability->createElement("CodecCapability")));
  }

  RTCRtpCodecCapability^ RTCRtpCodecCapability::FromJsonString(Platform::String^ jsonString)
  {
    auto capability = make_shared<IRTPTypes::CodecCapability>(IRTPTypes::CodecCapability::CodecCapability(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    return ToCx(capability);
  }

  //---------------------------------------------------------------------------
  // RTCRtcpFeedback methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtcpFeedback::ToJsonString()
  {
    IRTPTypes::RTCPFeedback feedback = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(feedback.createElement("RtcpFeedback")));
  }

  RTCRtcpFeedback^ RTCRtcpFeedback::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtcpFeedback();

    auto feedback = make_shared<IRTPTypes::RTCPFeedback>(IRTPTypes::RTCPFeedback::RTCPFeedback(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret->Parameter = ToCx(feedback->mParameter);
    ret->Type = ToCx(feedback->mType);

    return ret;
  }

  //---------------------------------------------------------------------------
  // RTCRtpHeaderExtension methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtpHeaderExtension::ToJsonString()
  {
    auto ext = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(ext->createElement("RtpHeaderExtension")));
  }

  RTCRtpHeaderExtension^ RTCRtpHeaderExtension::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtpHeaderExtension();

    auto headerExtension = make_shared<IRTPTypes::HeaderExtension>(IRTPTypes::HeaderExtension::HeaderExtension(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret->Kind = ToCx(headerExtension->mKind);
    ret->Uri = ToCx(headerExtension->mURI);
    ret->PreferredEncrypt = headerExtension->mPreferredEncrypt;
    ret->PreferredId = headerExtension->mPreferredID;

    return ret;
  }
} // namespace ortc_winrt_api