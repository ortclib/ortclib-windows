#pragma once
#include "pch.h"
#include "helpers.h"
#include <collection.h>

using namespace ortc;

namespace ortc_winrt_api
{
  //---------------------------------------------------------------------------
  // RTCRtpCapabilities methods
  //---------------------------------------------------------------------------
	RTCRtpCapabilities::RTCRtpCapabilities()
	{
		Codecs = ref new Platform::Collections::Vector<RTCRtpCodecCapability^>();
		HeaderExtensions = ref new Platform::Collections::Vector<RTCRtpHeaderExtension^>();
		FecMechanisms = ref new Platform::Collections::Vector<Platform::String^>();
	}
	

  Platform::String^ RTCRtpCapabilities::ToJsonString()
  {
    IRTPTypes::Capabilities caps = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(caps.createElement("RtpCapabilities")));
  }
  RTCRtpCapabilities^ RTCRtpCapabilities::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtpCapabilities();
	
    auto capabilitiesPtr = make_shared<IRTPTypes::Capabilities>(IRTPTypes::Capabilities::Capabilities(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));

    for (IRTPTypes::CodecCapabilitiesList::iterator it = capabilitiesPtr->mCodecs.begin(); it != capabilitiesPtr->mCodecs.end(); ++it)
    {
      auto codec = ToCx((make_shared<IRTPTypes::CodecCapability>(*it)));
      ret->Codecs->Append(codec);
    }

    for (IRTPTypes::HeaderExtensionsList::iterator it = capabilitiesPtr->mHeaderExtensions.begin(); it != capabilitiesPtr->mHeaderExtensions.end(); ++it)
    {
      auto codec = ToCx((make_shared<IRTPTypes::HeaderExtension>(*it)));
      ret->HeaderExtensions->Append(codec);
    }

    for (std::list<zsLib::String>::iterator it = capabilitiesPtr->mFECMechanisms.begin(); it != capabilitiesPtr->mFECMechanisms.end(); ++it)
    {
      ret->FecMechanisms->Append(ToCx(*it));
    }
    return ret;
  }

  //---------------------------------------------------------------------------
  // RTCRtpCodecCapability methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtpCodecCapability::ToJsonString()
  {
    IRTPTypes::CodecCapability capability = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(capability.createElement("CodecCapability")));
  }
  RTCRtpCodecCapability^ RTCRtpCodecCapability::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtpCodecCapability();

    auto capability = make_shared<IRTPTypes::CodecCapability>(IRTPTypes::CodecCapability::CodecCapability(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret = ToCx(capability);
    return ret;
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
    IRTPTypes::HeaderExtension ext = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(ext.createElement("RtpHeaderExtension")));
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