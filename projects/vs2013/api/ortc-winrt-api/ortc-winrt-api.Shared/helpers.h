#pragma once

#include <ortc/types.h>
#include "RTCIceGatherer.h"
//#include <collection.h>

using namespace ortc;

//using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
  std::string FromCx(Platform::String^ inObj);
  Platform::String^ ToCx(std::string const& inObj);
  RTCIceCandidate^ ToCx(IICETypes::CandidatePtr candidate);

  class FetchNativePointer
  {
  public:
    static IIceGathererPtr fromIceGatherer(RTCIceGatherer^ gatherer) { return gatherer->mNativePointer; }
  };
}