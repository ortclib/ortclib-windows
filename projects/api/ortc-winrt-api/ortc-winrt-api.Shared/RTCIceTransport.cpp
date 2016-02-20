#include "pch.h"
#include "RTCIceTransport.h"
#include "helpers.h"

using namespace ortc_winrt_api;

using Platform::Collections::Vector;

RTCIceTransport::RTCIceTransport(Platform::Boolean noop) :
  mNativeDelegatePointer(nullptr),
  mNativePointer(nullptr)
{
}

RTCIceTransport::RTCIceTransport() :
  mNativeDelegatePointer(new RTCIceTransportDelegate())
{
  mNativeDelegatePointer->SetOwnerObject(this);
  mNativePointer = IICETransport::create(mNativeDelegatePointer);
}

RTCIceTransport::RTCIceTransport(RTCIceGatherer^ gatherer) :
mNativeDelegatePointer(new RTCIceTransportDelegate())
{

  if (!gatherer)
  {
    return;
  }

  if (FetchNativePointer::FromIceGatherer(gatherer))
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IICETransport::create(mNativeDelegatePointer, FetchNativePointer::FromIceGatherer(gatherer));
  }
}

IVector<RTCIceCandidate^>^ RTCIceTransport::GetRemoteCandidates()
{
  auto ret = ref new Vector<RTCIceCandidate^>();
  if (mNativePointer)
  {
    auto candidates = mNativePointer->getRemoteCandidates();
    for (IICETransport::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it) {
      ret->Append(ToCx(*it));
    }
  }
  return ret;
}

RTCIceCandidatePair^ RTCIceTransport::GetSelectedCandidatePair()
{
  if (mNativePointer)
  {
    return ToCx(mNativePointer->getSelectedCandidatePair()); // should it be getSelectedCandidatePair???
  }

  return nullptr;
}

void RTCIceTransport::Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters)
{
  if (mNativePointer && FetchNativePointer::FromIceGatherer(gatherer))
  {
    assert(nullptr != remoteParameters);
    mNativePointer->start(FetchNativePointer::FromIceGatherer(gatherer), *FromCx(remoteParameters));
  }
}

void RTCIceTransport::Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role)
{
  if (mNativePointer && FetchNativePointer::FromIceGatherer(gatherer))
  {
    assert(nullptr != remoteParameters);

    IIceTransport::Options options;
    options.mRole = internal::ConvertEnums::convert(role);

    mNativePointer->start(FetchNativePointer::FromIceGatherer(gatherer), *FromCx(remoteParameters), options);
  }
}

void RTCIceTransport::Stop()
{
  if (mNativePointer)
  {
    mNativePointer->stop();
  }
}

RTCIceParameters^ RTCIceTransport::GetRemoteParameters()
{
  if (mNativePointer)
  {
    return ToCx(mNativePointer->getRemoteParameters());
  }
  return nullptr;
}

RTCIceTransport^ RTCIceTransport::CreateAssociatedTransport()
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

void RTCIceTransport::AddRemoteCandidate(RTCIceCandidate^ remoteCandidate)
{
  if (mNativePointer)
  {
    assert(nullptr != remoteCandidate);
    mNativePointer->addRemoteCandidate(*FromCx(remoteCandidate));
  }
}

void RTCIceTransport::AddRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate)
{
  if (mNativePointer)
  {
    IICETypes::CandidateComplete complete;
    mNativePointer->addRemoteCandidate(complete);
  }
}

void RTCIceTransport::SetRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates)
{
  if (mNativePointer)
  {
    assert(nullptr != remoteCandidates);

    IICETypes::CandidateList list;
    for (RTCIceCandidate^ candidate : remoteCandidates)
    {
      if (nullptr == candidate) continue;
      list.push_back(*FromCx(candidate));
    }

    mNativePointer->setRemoteCandidates(list);
  }
}

void RTCIceTransport::KeepWarm(RTCIceCandidatePair^ candidatePair)
{
  if (mNativePointer)
  {
    assert(nullptr != candidatePair);
    mNativePointer->keepWarm(*FromCx(candidatePair));
  }
}

void RTCIceTransport::KeepWarm(RTCIceCandidatePair^ candidatePair, Platform::Boolean keepWarm)
{
  if (mNativePointer)
  {
    assert(nullptr != candidatePair);
    mNativePointer->keepWarm(*FromCx(candidatePair), keepWarm);
  }
}

RTCIceGatherer^ RTCIceTransport::GetIceGatherer()
{
  return ConvertObjectToCx::ToIceGatherer(mNativePointer->iceGatherer());
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
  evt->CandidatePair = ToCx(candidatePair);
  _transport->OnICETransportCandidatePairAvailable(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairGone(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  auto evt = ref new RTCIceTransportCandidatePairEvent();
  evt->CandidatePair = ToCx(candidatePair);
  _transport->OnICETransportCandidatePairGone(evt);
}

void RTCIceTransportDelegate::onICETransportCandidatePairChanged(
  IICETransportPtr transport,
  CandidatePairPtr candidatePair
  )
{
  auto evt = ref new RTCIceTransportCandidatePairEvent();
  evt->CandidatePair = ToCx(candidatePair);
  _transport->OnICETransportCandidatePairChanged(evt);
}
