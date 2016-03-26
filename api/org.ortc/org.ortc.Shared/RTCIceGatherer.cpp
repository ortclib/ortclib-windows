#include "pch.h"
#include "RTCIceGatherer.h"
#include "helpers.h"
#include <ortc/ISettings.h>

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {

    RTCIceGatherer::RTCIceGatherer() :
      _NativeDelegatePointer(nullptr),
      _NativePointer(nullptr)
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

    void RTCIceGatherer::Gather(RTCIceGatherOptions^ options)
    {
      if (_NativePointer)
      {
        if (nullptr != options)
        {
          _NativePointer->gather(*FromCx(options));
        }
        else
        {
          _NativePointer->gather();
        }
      }
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
  } // namespace ortc
} // namespace org
