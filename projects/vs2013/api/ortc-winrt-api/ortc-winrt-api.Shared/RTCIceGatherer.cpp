#include "pch.h"
#include "RTCIceGatherer.h"

#include <openpeer/services/ILogger.h>

using namespace ortc_winrt_api;
using namespace Platform;

using namespace zsLib;

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;


RTCIceGatherer::RTCIceGatherer(RTCIceGatherOptions^ options) :
mNativeDelegatePointer(new RTCIceGathererDelegate())
{
  openpeer::services::ILogger::setLogLevel(zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("zsLib", zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("openpeer_services", zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("openpeer_services_http", zsLib::Log::Trace);
  openpeer::services::ILogger::setLogLevel("ortclib", zsLib::Log::Insane);
  openpeer::services::ILogger::setLogLevel("ortc_standup", zsLib::Log::Insane);

  //openpeer::services::ILogger::installDebuggerLogger();
  openpeer::services::ILogger::installTelnetLogger(59999, 60, true);
  zsLib::String url = zsLib::String("stun:") + "stun.vline.com";

  ortc::IICEGatherer::Server server;
  server.mURLs.push_back(url);
  ortc::IICEGatherer::Options optionsNative;
  optionsNative.mICEServers.push_back(server);

  mNativeDelegatePointer->SetOwnerObject(this);

  mNativePointer = IICEGatherer::create(mNativeDelegatePointer, optionsNative);
}

RTCIceGatherer^ RTCIceGatherer::createAssociatedGatherer()
{
  RTCIceGatherer^ ret = nullptr;
  return ret;
}

// Triggered when media is received on a new stream from remote peer.
void RTCIceGathererDelegate::onICEGathererStateChanged(
  IICEGathererPtr gatherer,
  IICEGatherer::States state
  ) {
  _gatherer->OnICEGathererStateChanged();
}

void RTCIceGathererDelegate::onICEGathererLocalCandidate(
  IICEGathererPtr gatherer,
  CandidatePtr candidate
  ) 
{
  int i = 0;
  i++;
}

void RTCIceGathererDelegate::onICEGathererLocalCandidateComplete(
  IICEGathererPtr gatherer,
  CandidateCompletePtr candidate
  )
{
  int i = 0;
  i++;
}

void RTCIceGathererDelegate::onICEGathererLocalCandidateGone(
  IICEGathererPtr gatherer,
  CandidatePtr candidate
  )
{
  int i = 0;
  i++;
}

void RTCIceGathererDelegate::onICEGathererError(
  IICEGathererPtr gatherer,
  ErrorCode errorCode,
  zsLib::String errorReason
  )
{
  int i = 0;
  i++;
}