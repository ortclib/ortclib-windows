#include "pch.h"
#include "RTCIceGatherer.h"

using namespace ortc_winrt_api;
using namespace Platform;

using namespace zsLib;

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

RTCIceGatherer::RTCIceGatherer(RTCIceGatherOptions^ options)
{
  zsLib::String url = zsLib::String("stun:") + "stun.vline.com";

  ortc::IICEGatherer::Server server;
  server.mURLs.push_back(url);
  ortc::IICEGatherer::Options optionsNative;
  optionsNative.mICEServers.push_back(server);
  mNativePointer = &IICEGatherer::create(NULL, optionsNative);
}

RTCIceGatherer^ RTCIceGatherer::createAssociatedGatherer()
{
  RTCIceGatherer^ ret = nullptr;
  return ret;
}
