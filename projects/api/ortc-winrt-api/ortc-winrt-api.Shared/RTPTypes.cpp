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
  Platform::String^ RTCRtpCapabilities::ToJsonString()
  {
    if (mNativePointer)
    {
      return ToCx(openpeer::services::IHelper::toString(mNativePointer->createElement("RtpCapabilities")));
    }

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
    PushNativePointer::ToRTCRtpCapabilities(ret, capabilitiesPtr);
    return ret;
  }

  //---------------------------------------------------------------------------
  // RTCRtpCodecCapability methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtpCodecCapability::ToJsonString()
  {
    if (mNativePointer)
    {
      return ToCx(openpeer::services::IHelper::toString(mNativePointer->createElement("CodecCapability")));
    }

  }
  RTCRtpCodecCapability^ RTCRtpCodecCapability::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtpCodecCapability();

    auto capability = make_shared<IRTPTypes::CodecCapability>(IRTPTypes::CodecCapability::CodecCapability(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret = ToCx(capability);
    PushNativePointer::ToRTCRtpCodecCapability(ret, capability);
    return ret;
  }

  //---------------------------------------------------------------------------
  // RTCRtcpFeedback methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtcpFeedback::ToJsonString()
  {
    if (mNativePointer)
    {
      return ToCx(openpeer::services::IHelper::toString(mNativePointer->createElement("RtcpFeedback")));
    }

  }
  RTCRtcpFeedback^ RTCRtcpFeedback::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtcpFeedback();

    auto feedback = make_shared<IRTPTypes::RTCPFeedback>(IRTPTypes::RTCPFeedback::RTCPFeedback(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret->Parameter = ToCx(feedback->mParameter);
    ret->Type = ToCx(feedback->mType);
    PushNativePointer::ToRTCRtcpFeedback(ret, feedback);
    return ret;
  }

  //---------------------------------------------------------------------------
  // RTCRtpHeaderExtension methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCRtpHeaderExtension::ToJsonString()
  {
    if (mNativePointer)
    {
      return ToCx(openpeer::services::IHelper::toString(mNativePointer->createElement("RtpHeaderExtension")));
    }

  }
  RTCRtpHeaderExtension^ RTCRtpHeaderExtension::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCRtpHeaderExtension();

    auto headerExtension = make_shared<IRTPTypes::HeaderExtension>(IRTPTypes::HeaderExtension::HeaderExtension(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret->Kind = ToCx(headerExtension->mKind);
    ret->Uri = ToCx(headerExtension->mURI);
    ret->PreferredEncrypt = headerExtension->mPreferredEncrypt;
    ret->PreferredId = headerExtension->mPreferredID;
    PushNativePointer::ToRTCRtpHeaderExtension(ret, headerExtension);
    return ret;
  }
} // namespace ortc_winrt_api