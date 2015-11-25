#include "pch.h"
#include "RTCIceTransport.h"
#include "helpers.h"
#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"

using namespace ortc_winrt_api;
using namespace Platform;

using namespace zsLib;

using Platform::Collections::Vector;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

RTCIceTransport::RTCIceTransport() :
mNativeDelegatePointer(nullptr),
mNativePointer(nullptr)
{
}

RTCIceTransport::RTCIceTransport(RTCIceGatherer^ gatherer) :
mNativeDelegatePointer(new RTCIceTransportDelegate())
{

  if (!gatherer)
  {
    return;
  }

  if (FetchNativePointer::fromIceGatherer(gatherer))
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IICETransport::create(mNativeDelegatePointer, FetchNativePointer::fromIceGatherer(gatherer));
  }
}

IVector<RTCIceCandidate^>^ RTCIceTransport::getRemoteCandidates()
{
  auto ret = ref new Vector<RTCIceCandidate^>();
  if (mNativePointer)
  {
    auto candidates = mNativePointer->getRemoteCandidates();
    for (IICETransport::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it) {
      ret->Append(ToCx(make_shared<IICETransport::Candidate>(*it)));
    }
  }
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