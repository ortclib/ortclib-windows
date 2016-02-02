#include "pch.h"
#include "RTCIceGatherer.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using Platform::Collections::Vector;

namespace ortc_winrt_api
{
  namespace internal
  {
    //---------------------------------------------------------------------------
    // RTCIceGatherPolicy convert methods
    //---------------------------------------------------------------------------
    static IICEGathererTypes::FilterPolicies convert(RTCIceGatherPolicy policy)
    {
      switch (policy) {
      case RTCIceGatherPolicy::None:              return IICEGathererTypes::FilterPolicies::FilterPolicy_None;
      case RTCIceGatherPolicy::NoIPv4Srflx:       return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Srflx;
      case RTCIceGatherPolicy::NoIPv4Prflx:       return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Prflx;
      case RTCIceGatherPolicy::NoIPv4Relay:       return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Relay;
      case RTCIceGatherPolicy::NoIPv4Private:     return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Private;
      case RTCIceGatherPolicy::NoIPv4:            return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Host;
      case RTCIceGatherPolicy::NoIPv6Host:        return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Host;
      case RTCIceGatherPolicy::NoIPv6Srflx:       return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Srflx;
      case RTCIceGatherPolicy::NoIPv6Prflx:       return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Prflx;
      case RTCIceGatherPolicy::NoIPv6Relay:       return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Relay;
      case RTCIceGatherPolicy::NoIPv6Private:     return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Private;
      case RTCIceGatherPolicy::NoIPv6Tunnel:      return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Tunnel;
      case RTCIceGatherPolicy::NoIPv6Permanent:   return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Permanent;
      case RTCIceGatherPolicy::NoIPv6:            return IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6;
      case RTCIceGatherPolicy::NoHost:            return IICEGathererTypes::FilterPolicies::FilterPolicy_NoHost;
      case RTCIceGatherPolicy::NoSrflx:           return IICEGathererTypes::FilterPolicies::FilterPolicy_NoSrflx;
      case RTCIceGatherPolicy::NoPrflx:           return IICEGathererTypes::FilterPolicies::FilterPolicy_NoPrflx;
      case RTCIceGatherPolicy::NoRelay:           return IICEGathererTypes::FilterPolicies::FilterPolicy_NoRelay;
      case RTCIceGatherPolicy::NoPrivate:         return IICEGathererTypes::FilterPolicies::FilterPolicy_NoPrivate;
      case RTCIceGatherPolicy::RelayOnly:         return IICEGathererTypes::FilterPolicies::FilterPolicy_RelayOnly;
      case RTCIceGatherPolicy::NoCandidates:      return IICEGathererTypes::FilterPolicies::FilterPolicy_NoCandidates;
      }
      throw ref new Platform::NotImplementedException();
    }

    static RTCIceGatherPolicy convert(IICEGathererTypes::FilterPolicies policy)
    {
      switch (policy) {
      case IICEGathererTypes::FilterPolicies::FilterPolicy_None:              return RTCIceGatherPolicy::None;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Srflx:       return RTCIceGatherPolicy::NoIPv4Srflx;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Prflx:       return RTCIceGatherPolicy::NoIPv4Prflx;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Relay:       return RTCIceGatherPolicy::NoIPv4Relay;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Private:     return RTCIceGatherPolicy::NoIPv4Private;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv4Host:        return RTCIceGatherPolicy::NoIPv4;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Host:        return RTCIceGatherPolicy::NoIPv6Host;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Srflx:       return RTCIceGatherPolicy::NoIPv6Srflx;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Prflx:       return RTCIceGatherPolicy::NoIPv6Prflx;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Relay:       return RTCIceGatherPolicy::NoIPv6Relay;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Private:     return RTCIceGatherPolicy::NoIPv6Private;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Tunnel:      return RTCIceGatherPolicy::NoIPv6Tunnel;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6Permanent:   return RTCIceGatherPolicy::NoIPv6Permanent;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoIPv6:            return RTCIceGatherPolicy::NoIPv6;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoHost:            return RTCIceGatherPolicy::NoHost;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoSrflx:           return RTCIceGatherPolicy::NoSrflx;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoPrflx:           return RTCIceGatherPolicy::NoPrflx;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoRelay:           return RTCIceGatherPolicy::NoRelay;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoPrivate:         return RTCIceGatherPolicy::NoPrivate;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_RelayOnly:         return RTCIceGatherPolicy::RelayOnly;
      case IICEGathererTypes::FilterPolicies::FilterPolicy_NoCandidates:      return RTCIceGatherPolicy::NoCandidates;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // RTCIceGathererCredentialType convert methods
    //---------------------------------------------------------------------------
    static IICEGathererTypes::CredentialTypes convert(RTCIceGathererCredentialType credentialType)
    {
      switch (credentialType) {
      case RTCIceGathererCredentialType::Password:    return IICEGathererTypes::CredentialTypes::CredentialType_Password;
      case RTCIceGathererCredentialType::Token:       return IICEGathererTypes::CredentialTypes::CredentialType_Token;
      }
      throw ref new Platform::NotImplementedException();
    }

    static RTCIceGathererCredentialType convert(IICEGathererTypes::CredentialTypes credentialType)
    {
      switch (credentialType) {
      case IICEGathererTypes::CredentialTypes::CredentialType_Password:    return RTCIceGathererCredentialType::Password;
      case IICEGathererTypes::CredentialTypes::CredentialType_Token:       return RTCIceGathererCredentialType::Token;
      }
      throw ref new Platform::NotImplementedException();
    }

    //---------------------------------------------------------------------------
    // RTCIceGathererState convert methods
    //---------------------------------------------------------------------------
    static IICEGathererTypes::States convert(RTCIceGathererState state)
    {
      switch (state) {
      case RTCIceGathererState::New:          return IICEGathererTypes::States::State_New;
      case RTCIceGathererState::Gathering:    return IICEGathererTypes::States::State_Gathering;
      case RTCIceGathererState::Complete:     return IICEGathererTypes::States::State_Complete;
      case RTCIceGathererState::Closed:       return IICEGathererTypes::States::State_Closed;
      }
      throw ref new Platform::NotImplementedException();
    }

    static RTCIceGathererState convert(IICEGathererTypes::States state)
    {
      switch (state) {
      case IICEGathererTypes::States::State_New:          return RTCIceGathererState::New;
      case IICEGathererTypes::States::State_Gathering:    return RTCIceGathererState::Gathering;
      case IICEGathererTypes::States::State_Complete:     return RTCIceGathererState::Complete;
      case IICEGathererTypes::States::State_Closed:       return RTCIceGathererState::Closed;
      }
      throw ref new Platform::NotImplementedException();
    }
  } // namespace internal

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
    return ToCx(IIceGatherer::toString(internal::convert(value)));
  }

  Platform::String^ RTCIceGatherer::ToString(RTCIceGathererState value)
  {
    return ToCx(IIceGatherer::toString(internal::convert(value)));
  }

  Platform::String^ RTCIceGatherer::ToString(RTCIceGathererCredentialType value)
  {
    return ToCx(IIceGatherer::toString(internal::convert(value)));
  }

  RTCIceGatherPolicy RTCIceGatherer::ToPolicy(Platform::String^ str)
  {
    return internal::convert(IIceGatherer::toPolicy(FromCx(str).c_str()));
  }

  RTCIceGathererState RTCIceGatherer::ToState(Platform::String^ str)
  {
    return internal::convert(IIceGatherer::toState(FromCx(str).c_str()));
  }

  RTCIceGathererCredentialType RTCIceGatherer::ToCredentialType(Platform::String^ str)
  {
    return internal::convert(IIceGatherer::toCredentialType(FromCx(str).c_str()));
  }

  RTCIceGathererState RTCIceGatherer::State::get()
  {
    if (mNativePointer)
      return internal::convert(mNativePointer->state());
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
    evt->State = internal::convert(state);
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