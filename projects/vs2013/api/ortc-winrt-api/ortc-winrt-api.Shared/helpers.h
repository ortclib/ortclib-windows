#pragma once

#include <ortc/types.h>
#include "RTCIceGatherer.h"
//#include <collection.h>

using namespace ortc;

//using Windows::Foundation::Collections::IVector;

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

  IICEGatherer::Options FromCx(RTCIceGatherOptions^ options);

  class FetchNativePointer
  {
  public:
    static IIceGathererPtr fromIceGatherer(RTCIceGatherer^ gatherer) { return gatherer->mNativePointer; }
  };
}