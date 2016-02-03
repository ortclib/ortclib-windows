#pragma once
#include "pch.h"
#include "helpers.h"
#include <collection.h>

using namespace ortc;

using Windows::Foundation::Collections::IVector;


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
} // namespace ortc_winrt_api