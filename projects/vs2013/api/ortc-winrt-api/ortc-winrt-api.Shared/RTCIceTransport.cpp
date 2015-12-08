#include "pch.h"
#include "RTCIceTransport.h"
#include "helpers.h"

using namespace ortc_winrt_api;

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
  if (mNativePointer)
  {
    auto candidatePair = mNativePointer->getSelectedCandidatePair(); // should it be getSelectedCandidatePair???
    ret->Local = ToCx(candidatePair->mLocal);
    ret->Remote = ToCx(candidatePair->mRemote);
  }

  return ret;
}

void RTCIceTransport::start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role)
{
  if (mNativePointer && FetchNativePointer::fromIceGatherer(gatherer))
  {

    IIceTransport::Parameters params;
    params.mUsernameFragment = FromCx(remoteParameters->UsernameFragment);
    params.mPassword = FromCx(remoteParameters->Password);

    IIceTransport::Options options;
    options.mRole = (IICETypes::Roles)role;

    mNativePointer->start(FetchNativePointer::fromIceGatherer(gatherer), params, options);
  }
}

void RTCIceTransport::stop()
{
  if (mNativePointer)
  {
    mNativePointer->stop();
  }
}

RTCIceParameters^ RTCIceTransport::getRemoteParameters()
{
  auto ret = ref new RTCIceParameters();
  if (mNativePointer)
  {
    auto params = mNativePointer->getRemoteParameters();
    ret->UsernameFragment = ToCx(params->mUsernameFragment);
    ret->Password = ToCx(params->mPassword);
  }
  return ret;
}

RTCIceTransport^ RTCIceTransport::createAssociatedTransport()
{
  auto ret = ref new RTCIceTransport();

  if (mNativePointer)
  {
    ret->mNativeDelegatePointer = make_shared<RTCIceTransportDelegate>(RTCIceTransportDelegate());
    ret->mNativePointer = mNativePointer->createAssociatedTransport();
    ret->mNativeDelegatePointer->SetOwnerObject(ret);
  }

  return ret;
}

void RTCIceTransport::addRemoteCandidate(RTCIceCandidate^ remoteCandidate)
{
  if (mNativePointer)
  {
    mNativePointer->addRemoteCandidate(FromCx(remoteCandidate));
  }
}

void RTCIceTransport::addRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate)
{
  if (mNativePointer)
  {
    IICETypes::CandidateComplete complete;
    mNativePointer->addRemoteCandidate(complete);
  }
}

void RTCIceTransport::setRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates)
{
  if (mNativePointer)
  {
    IICETypes::CandidateList list;
    for (RTCIceCandidate^ candidate : remoteCandidates)
    {
      list.push_back(FromCx(candidate));
    }

    mNativePointer->setRemoteCandidates(list);
  }
}

//-----------------------------------------------------------------
#pragma mark RTCIceTransportDelegate
//-----------------------------------------------------------------

// Triggered when media is received on a new stream from remote peer.
void RTCIceTransportDelegate::onICETransportStateChange(
  IICETransportPtr transport,
  IICETransport::States state
  )
{
  auto evt = ref new RTCIceTransportStateChangeEvent();
  evt->State = (RTCIceTransportState)state;
  _transport->OnICETransportStateChanged(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairAvailable(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  auto evt = ref new RTCIceTransportCandidatePairEvent();
  RTCIceCandidatePair^ pair = ref new RTCIceCandidatePair();
  pair->Local = ToCx(candidatePair->mLocal);
  pair->Remote = ToCx(candidatePair->mRemote);
  evt->CandidatePair = pair;
  _transport->OnICETransportCandidatePairAvailable(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairGone(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  auto evt = ref new RTCIceTransportCandidatePairEvent();
  RTCIceCandidatePair^ pair = ref new RTCIceCandidatePair();
  pair->Local = ToCx(candidatePair->mLocal);
  pair->Remote = ToCx(candidatePair->mRemote);
  evt->CandidatePair = pair;
  _transport->OnICETransportCandidatePairGone(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairChanged(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  auto evt = ref new RTCIceTransportCandidatePairEvent();
  RTCIceCandidatePair^ pair = ref new RTCIceCandidatePair();
  pair->Local = ToCx(candidatePair->mLocal);
  pair->Remote = ToCx(candidatePair->mRemote);
  evt->CandidatePair = pair;
  _transport->OnICETransportCandidatePairChanged(evt);
}