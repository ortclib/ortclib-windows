#include "pch.h"
#include "RTCIceGatherer.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using Platform::Collections::Vector;

namespace ortc_winrt_api
{

  RTCIceGatherer::RTCIceGatherer() :
    mNativeDelegatePointer(nullptr),
    mNativePointer(nullptr)
  {
  }

  RTCIceGatherer::RTCIceGatherer(RTCIceGatherOptions^ options) :
    mNativeDelegatePointer(new RTCIceGathererDelegate())
  {
    mNativeDelegatePointer->SetOwnerObject(this);

    mNativePointer = IICEGatherer::create(mNativeDelegatePointer, FromCx(options));
  }

  RTCIceParameters^ RTCIceGatherer::GetLocalParameters()
  {
    RTCIceParameters^ ret = ref new RTCIceParameters();
    if (mNativePointer)
    {
      IICETypes::ParametersPtr params = mNativePointer->getLocalParameters();
      if (params)
      {
        ret->UsernameFragment = ToCx(params->mUsernameFragment);
        ret->Password = ToCx(params->mPassword);
        PushNativePointer::ToRTCIceParameters(ret, params);
      }
    }

    return ret;
  }

  IVector<RTCIceCandidate^>^ RTCIceGatherer::GetLocalCandidates()
  {
    auto ret = ref new Vector<RTCIceCandidate^>();

    if (mNativePointer)
    {
      auto candidates = mNativePointer->getLocalCandidates();
      for (IICEGatherer::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it) {
        ret->Append(ToCx(make_shared<IICEGatherer::Candidate>(*it)));
      }
    }

    return ret;

  }

  RTCIceGatherer^ RTCIceGatherer::CreateAssociatedGatherer()
  {
    RTCIceGatherer^ ret = ref new RTCIceGatherer();

    if (mNativePointer)
    {
      ret->mNativeDelegatePointer = make_shared<RTCIceGathererDelegate>(RTCIceGathererDelegate());
      ret->mNativePointer = mNativePointer->createAssociatedGatherer(ret->mNativeDelegatePointer);
      ret->mNativeDelegatePointer->SetOwnerObject(ret);
    }
    return ret;
  }

  void RTCIceGatherer::Close()
  {
    if (mNativePointer)
      mNativePointer->close();
  }

  Platform::String^ RTCIceGatherer::ToString()
  {
    throw ref new Platform::NotImplementedException();
  }

  Platform::String^ RTCIceGatherer::ToString(RTCIceGatherPolicy value)
  {
    return ToCx(IIceGatherer::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCIceGatherer::ToString(RTCIceGathererState value)
  {
    return ToCx(IIceGatherer::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCIceGatherer::ToString(RTCIceGathererCredentialType value)
  {
    return ToCx(IIceGatherer::toString(internal::ConvertEnums::convert(value)));
  }

  RTCIceGatherPolicy RTCIceGatherer::ToPolicy(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IIceGatherer::toPolicy(FromCx(str).c_str()));
  }

  RTCIceGathererState RTCIceGatherer::ToState(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IIceGatherer::toState(FromCx(str).c_str()));
  }

  RTCIceGathererCredentialType RTCIceGatherer::ToCredentialType(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IIceGatherer::toCredentialType(FromCx(str).c_str()));
  }

  RTCIceGathererState RTCIceGatherer::State::get()
  {
    if (mNativePointer)
      return internal::ConvertEnums::convert(mNativePointer->state());
    else
      return RTCIceGathererState::Closed;
  }

  //-----------------------------------------------------------------
#pragma mark RTCIceGathererDelegate
//-----------------------------------------------------------------

// Triggered when media is received on a new stream from remote peer.
  void RTCIceGathererDelegate::onICEGathererStateChange(
    IICEGathererPtr gatherer,
    IICEGatherer::States state
    )
  {
    auto evt = ref new RTCIceGathererStateChangeEvent();
    evt->State = internal::ConvertEnums::convert(state);
    _gatherer->OnICEGathererStateChanged(evt);
  }

  void RTCIceGathererDelegate::onICEGathererLocalCandidate(
    IICEGathererPtr gatherer,
    CandidatePtr candidate
    )
  {
    auto evt = ref new RTCIceGathererCandidateEvent();
    evt->Candidate = ToCx(candidate);
    _gatherer->OnICEGathererLocalCandidate(evt);
  }

  void RTCIceGathererDelegate::onICEGathererLocalCandidateComplete(
    IICEGathererPtr gatherer,
    CandidateCompletePtr candidate
    )
  {
    auto evt = ref new RTCIceGathererCandidateCompleteEvent();
    RTCIceCandidateComplete^ temp = ref new RTCIceCandidateComplete();
    temp->Complete = true;
    evt->Completed = temp;
    _gatherer->OnICEGathererCandidateComplete(evt);
  }

  void RTCIceGathererDelegate::onICEGathererLocalCandidateGone(
    IICEGathererPtr gatherer,
    CandidatePtr candidate
    )
  {
    auto evt = ref new RTCIceGathererCandidateEvent();
    evt->Candidate = ToCx(candidate);
    _gatherer->OnICEGathererLocalCandidateGone(evt);
  }

  void RTCIceGathererDelegate::onICEGathererError(
    IICEGathererPtr gatherer,
    ErrorCode errorCode,
    zsLib::String errorReason
    )
  {
    auto evt = ref new RTCIceGathererErrorEvent();
    evt->Error->ErrorCode = errorCode;
    evt->Error->ErrorReason = ToCx(errorReason);
    _gatherer->OnICEGathererError(evt);
  }
} // namespace ortc_winrt_api