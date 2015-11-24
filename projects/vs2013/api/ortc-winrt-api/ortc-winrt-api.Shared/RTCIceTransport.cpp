#include "pch.h"
#include "RTCIceTransport.h"

#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"

using namespace ortc_winrt_api;
using namespace Platform;

using namespace zsLib;

using Platform::Collections::Vector;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

//std::string FromCx(Platform::String^ inObj) {
//  return rtc::ToUtf8(inObj->Data());
//}
//
//Platform::String^ ToCx(std::string const& inObj) {
//  return ref new Platform::String(rtc::ToUtf16(inObj).c_str());
//}
//
//RTCIceCandidate^ ToCx(ortc::IICETypes::CandidatePtr candidate)
//{
//  RTCIceCandidate^ ret = ref new RTCIceCandidate();
//
//  ret->CandidateType = (RTCIceCandidateType)candidate->mCandidateType;
//  ret->Foundation = ToCx(candidate->mFoundation);
//  ret->InterfaceType = ToCx(candidate->mInterfaceType);
//  ret->IP = ToCx(candidate->mIP);
//  ret->Port = candidate->mPort;
//  ret->Priority = candidate->mPriority;
//  ret->Protocol = (RTCIceProtocol)candidate->mProtocol;
//  ret->RelatedAddress = ToCx(candidate->mRelatedAddress);
//  ret->RelatedPort = candidate->mRelatedPort;
//  ret->TCPType = (RTCIceTcpCandidateType)candidate->mTCPType;
//  ret->UnfreezePriority = candidate->mUnfreezePriority;
//
//  return ret;
//}

RTCIceTransport::RTCIceTransport() :
mNativeDelegatePointer(nullptr),
mNativePointer(nullptr)
{
}

RTCIceTransport::RTCIceTransport(RTCIceGatherer^ gatherer) :
mNativeDelegatePointer(new RTCIceTransportDelegate())
{
  //openpeer::services::ILogger::setLogLevel(zsLib::Log::Trace);
  //openpeer::services::ILogger::setLogLevel("zsLib", zsLib::Log::Trace);
  //openpeer::services::ILogger::setLogLevel("openpeer_services", zsLib::Log::Trace);
  //openpeer::services::ILogger::setLogLevel("openpeer_services_http", zsLib::Log::Trace);
  //openpeer::services::ILogger::setLogLevel("ortclib", zsLib::Log::Insane);
  //openpeer::services::ILogger::setLogLevel("ortc_standup", zsLib::Log::Insane);

  ////openpeer::services::ILogger::installDebuggerLogger();
  //openpeer::services::ILogger::installTelnetLogger(59999, 60, true);
  //zsLib::String url = zsLib::String("stun:") + "stun.vline.com";

  //ortc::IICEGatherer::Server server;
  //server.mURLs.push_back(url);
  //ortc::IICEGatherer::Options optionsNative;
  //optionsNative.mICEServers.push_back(server);

  //mNativeDelegatePointer->SetOwnerObject(this);

  //mNativePointer = IICEGatherer::create(mNativeDelegatePointer, optionsNative);
}

IVector<RTCIceCandidate^>^ RTCIceTransport::getRemoteCandidates()
{
  auto ret = ref new Vector<RTCIceCandidate^>();

  return ret;
}

RTCIceCandidatePair^ RTCIceTransport::getSelectedCandidatePair()
{
  auto ret = ref new RTCIceCandidatePair();

  return ret;
}

void RTCIceTransport::start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role)
{

}

void RTCIceTransport::stop()
{

}

RTCIceParameters^ RTCIceTransport::getRemoteParameters()
{
  auto ret = ref new RTCIceParameters();

  return ret;
}

RTCIceTransport^ RTCIceTransport::createAssociatedTransport()
{
  auto ret = ref new RTCIceTransport();

  return ret;
}

void RTCIceTransport::addRemoteCandidate(RTCIceCandidate^ remoteCandidate)
{

}

void RTCIceTransport::addRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate)
{

}

void RTCIceTransport::setRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates)
{

}

//-----------------------------------------------------------------
#pragma mark RTCIceGathererDelegate
//-----------------------------------------------------------------

// Triggered when media is received on a new stream from remote peer.
void RTCIceTransportDelegate::onICETransportStateChanged(
  IICETransportPtr transport,
  IICETransport::States state
  )
{
  //auto evt = ref new RTCIceGathererStateChangeEvent();
  //evt->State = (RTCIceGathererState)state;
  //_gatherer->OnICEGathererStateChanged(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairAvailable(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  //auto evt = ref new RTCIceGathererCandidateEvent();
  //evt->Candidate = ToCx(candidate);
  //_gatherer->OnICEGathererLocalCandidate(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairGone(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  //auto evt = ref new RTCIceGathererCandidateCompleteEvent();
  //evt->Complete->Complete = true;
  //_gatherer->OnICEGathererCandidateComplete(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairChanged(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  //auto evt = ref new RTCIceGathererCandidateEvent();
  //evt->Candidate = ToCx(candidate);
  //_gatherer->OnICEGathererLocalCandidateGone(evt);
}