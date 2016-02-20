#include "pch.h"
#include "RTCIceGatherer.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using Platform::Collections::Vector;

namespace ortc_winrt_api
{

  RTCIceGatherer::RTCIceGatherer() :
    _NativeDelegatePointer(nullptr),
    _NativePointer(nullptr),
    _RaisedOnICEGathererStateChangedEvents(ref new Vector<RTCIceGathererStateChangeEvent^>()),
    _RaisedOnICEGathererLocalCandidateEvents(ref new Vector<RTCIceGathererCandidateEvent^>()),
    _RaisedOnICEGathererCandidateCompleteEvents(ref new Vector<RTCIceGathererCandidateCompleteEvent^>()),
    _RaisedOnICEGathererLocalCandidateGoneEvents(ref new Vector<RTCIceGathererCandidateEvent^>()),
    _RaisedOnICEGathererErrorEvents(ref new Vector<RTCIceGathererErrorEvent^>())
  {
  }

  RTCIceGatherer::RTCIceGatherer(RTCIceGatherOptions^ options) :
    _NativeDelegatePointer(new RTCIceGathererDelegate())
  {
    _NativeDelegatePointer->SetOwnerObject(this);

    assert(nullptr != options);
    _NativePointer = IICEGatherer::create(_NativeDelegatePointer, *FromCx(options));
  }

  RTCIceParameters^ RTCIceGatherer::GetLocalParameters()
  {
    if (_NativePointer)
    {
      return ToCx(_NativePointer->getLocalParameters());
    }

    return nullptr;
  }

  IVector<RTCIceCandidate^>^ RTCIceGatherer::GetLocalCandidates()
  {
    auto ret = ref new Vector<RTCIceCandidate^>();

    if (_NativePointer)
    {
      auto candidates = _NativePointer->getLocalCandidates();
      for (IICEGatherer::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it) {
        ret->Append(ToCx(*it));
      }
    }

    return ret;

  }

  RTCIceGatherer^ RTCIceGatherer::CreateAssociatedGatherer()
  {
    RTCIceGatherer^ ret = ref new RTCIceGatherer();

    if (_NativePointer)
    {
      ret->_NativeDelegatePointer = make_shared<RTCIceGathererDelegate>(RTCIceGathererDelegate());
      ret->_NativePointer = _NativePointer->createAssociatedGatherer(ret->_NativeDelegatePointer);
      ret->_NativeDelegatePointer->SetOwnerObject(ret);
    }
    return ret;
  }

  void RTCIceGatherer::Close()
  {
    if (_NativePointer)
      _NativePointer->close();
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
    if (_NativePointer)
      return internal::ConvertEnums::convert(_NativePointer->state());
    else
      return RTCIceGathererState::Closed;
  }

  Windows::Foundation::EventRegistrationToken RTCIceGatherer::RaiseBufferedOnICEGathererStateChangedEvents(RTCIceGathererStateChangedDelegate^ handler)
  {
    Vector<RTCIceGathererStateChangeEvent^>^ fired = nullptr;
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      fired = _RaisedOnICEGathererStateChangedEvents;
      _RaisedOnICEGathererStateChangedEvents = nullptr;
    }

    if (nullptr != fired) {for (auto evt : fired) {handler(evt);}}
    return _InternalOnICEGathererStateChanged += handler;
  }

  Windows::Foundation::EventRegistrationToken RTCIceGatherer::RaiseBufferedOnICEGathererLocalCandidateEvents(RTCIceGathererLocalCandidateDelegate^ handler)
  {
    Vector<RTCIceGathererCandidateEvent^>^ fired = nullptr;
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      fired = _RaisedOnICEGathererLocalCandidateEvents;
      _RaisedOnICEGathererLocalCandidateEvents = nullptr;
    }

    if (nullptr != fired) { for (auto evt : fired) { handler(evt); } }
    return _InternalOnICEGathererLocalCandidate += handler;
  }

  Windows::Foundation::EventRegistrationToken RTCIceGatherer::RaiseBufferedOnICEGathererCandidateCompleteEvents(RTCIceGathererCandidateCompleteDelegate^ handler)
  {
    Vector<RTCIceGathererCandidateCompleteEvent^>^ fired = nullptr;
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      fired = _RaisedOnICEGathererCandidateCompleteEvents;
      _RaisedOnICEGathererCandidateCompleteEvents = nullptr;
    }

    if (nullptr != fired) { for (auto evt : fired) { handler(evt); } }
    return _InternalOnICEGathererCandidateComplete += handler;
  }

  Windows::Foundation::EventRegistrationToken RTCIceGatherer::RaiseBufferedOnICEGathererLocalCandidateGoneEvents(RTCIceGathererLocalCandidateGoneDelegate^ handler)
  {
    Vector<RTCIceGathererCandidateEvent^>^ fired = nullptr;
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      fired = _RaisedOnICEGathererLocalCandidateGoneEvents;
      _RaisedOnICEGathererLocalCandidateGoneEvents = nullptr;
    }

    if (nullptr != fired) { for (auto evt : fired) { handler(evt); } }
    return _InternalOnICEGathererLocalCandidateGone += handler;
  }

  Windows::Foundation::EventRegistrationToken RTCIceGatherer::RaiseBufferedOnICEGathererErrorEvents(RTCIceGathererErrorDelegate^ handler)
  {
    Vector<RTCIceGathererErrorEvent^>^ fired = nullptr;
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      fired = _RaisedOnICEGathererErrorEvents;
      _RaisedOnICEGathererErrorEvents = nullptr;
    }

    if (nullptr != fired) { for (auto evt : fired) { handler(evt); } }
    return _InternalOnICEGathererError += handler;
  }

  void RTCIceGatherer::RaiseOnICEGathererStateChangedEvent(RTCIceGathererStateChangeEvent^ evt)
  {
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      if (nullptr != _RaisedOnICEGathererStateChangedEvents)
        _RaisedOnICEGathererStateChangedEvents->Append(evt);
    }
    _InternalOnICEGathererStateChanged(evt);
  }

  void RTCIceGatherer::RaiseOnICEGathererLocalCandidateEvent(RTCIceGathererCandidateEvent^ evt)
  {
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      if (nullptr != _RaisedOnICEGathererLocalCandidateEvents)
        _RaisedOnICEGathererLocalCandidateEvents->Append(evt);
    }
    _InternalOnICEGathererLocalCandidate(evt);
  }

  void RTCIceGatherer::RaiseOnICEGathererCandidateCompleteEvent(RTCIceGathererCandidateCompleteEvent^ evt)
  {
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      if (nullptr != _RaisedOnICEGathererCandidateCompleteEvents)
        _RaisedOnICEGathererCandidateCompleteEvents->Append(evt);
    }
    _InternalOnICEGathererCandidateComplete(evt);
  }

  void RTCIceGatherer::RaiseOnICEGathererLocalCandidateGoneEvent(RTCIceGathererCandidateEvent^ evt)
  {
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      if (nullptr != _RaisedOnICEGathererLocalCandidateGoneEvents)
        _RaisedOnICEGathererLocalCandidateGoneEvents->Append(evt);
    }
    _InternalOnICEGathererLocalCandidateGone(evt);
  }

  void RTCIceGatherer::RaiseOnICEGathererErrorEvent(RTCIceGathererErrorEvent^ evt)
  {
    {
      zsLib::AutoRecursiveLock lock(_Lock);
      if (nullptr != _RaisedOnICEGathererErrorEvents)
        _RaisedOnICEGathererErrorEvents->Append(evt);
    }
    _InternalOnICEGathererError(evt);
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
    _gatherer->RaiseOnICEGathererStateChangedEvent(evt);
  }

  void RTCIceGathererDelegate::onICEGathererLocalCandidate(
    IICEGathererPtr gatherer,
    CandidatePtr candidate
    )
  {
    auto evt = ref new RTCIceGathererCandidateEvent();
    evt->Candidate = ToCx(candidate);
    _gatherer->RaiseOnICEGathererLocalCandidateEvent(evt);
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
    _gatherer->RaiseOnICEGathererCandidateCompleteEvent(evt);
  }

  void RTCIceGathererDelegate::onICEGathererLocalCandidateGone(
    IICEGathererPtr gatherer,
    CandidatePtr candidate
    )
  {
    auto evt = ref new RTCIceGathererCandidateEvent();
    evt->Candidate = ToCx(candidate);
    _gatherer->RaiseOnICEGathererLocalCandidateGoneEvent(evt);
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
    _gatherer->RaiseOnICEGathererErrorEvent(evt);
  }

} // namespace ortc_winrt_api