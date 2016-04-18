#include "pch.h"

#include "RTCIceGatherer.h"
#include "helpers.h"

using namespace ortc;

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
      RTCIceInterfacePolicy^ ToCx(const IICEGathererTypes::InterfacePolicy &input)
      {
        auto result = ref new RTCIceInterfacePolicy();
        result->InterfaceType = Helper::ToCx(input.mInterfaceType);
        result->GatherPolicy = Helper::convert(input.mGatherPolicy);
        return result;
      }

      RTCIceInterfacePolicy^ ToCx(IICEGathererTypes::InterfacePolicyPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICEGathererTypes::InterfacePolicyPtr FromCx(RTCIceInterfacePolicy^ input)
      {
        if (nullptr == input) return IICEGatherer::InterfacePolicyPtr();
        auto result = make_shared<IICEGatherer::InterfacePolicy>();
        result->mInterfaceType = Helper::FromCx(input->InterfaceType);
        result->mGatherPolicy = Helper::convert(input->GatherPolicy);
        return result;
      }

      RTCIceServer^ ToCx(const IICEGathererTypes::Server &input)
      {
        auto result = ref new RTCIceServer();

        if (input.mURLs.size() > 0)
        {
          result->Urls = ref new Vector<Platform::String^>();
          for (auto iter = input.mURLs.begin(); iter != input.mURLs.end(); ++iter)
          {
            auto &value = (*iter);
            result->Urls->Append(Helper::ToCx(value));
          }
        }
        result->UserName = Helper::ToCx(input.mUserName);
        result->Credential = Helper::ToCx(input.mCredential);
        result->CredentialType = Helper::convert(input.mCredentialType);
        return result;
      }

      RTCIceServer^ ToCx(IICEGathererTypes::ServerPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICEGathererTypes::ServerPtr FromCx(RTCIceServer^ input)
      {
        if (nullptr == input) return IICEGatherer::ServerPtr();
        auto result = make_shared<IICEGatherer::Server>();
        if (input->Urls)
        {
          for (Platform::String^ value : input->Urls)
          {
            result->mURLs.push_front(Helper::FromCx(value));
          }
        }
        result->mUserName = Helper::FromCx(input->UserName);
        result->mCredential = Helper::FromCx(input->Credential);
        result->mCredentialType = Helper::convert(input->CredentialType);
        return result;
      }

      RTCIceGatherOptions^ ToCx(const IICEGathererTypes::Options &input)
      {
        auto result = ref new RTCIceGatherOptions();
        result->ContinuousGathering = input.mContinuousGathering;
        if (input.mInterfacePolicies.size() > 0)
        {
          result->InterfacePolicies = ref new Vector<RTCIceInterfacePolicy^>();
          for (auto iter = input.mInterfacePolicies.begin(); iter != input.mInterfacePolicies.end(); ++iter)
          {
            auto &value = (*iter);
            result->InterfacePolicies->Append(ToCx(value));
          }
        }
        if (input.mICEServers.size() > 0)
        {
          result->IceServers = ref new Vector<RTCIceServer^>();
          for (auto iter = input.mICEServers.begin(); iter != input.mICEServers.end(); ++iter)
          {
            auto &value = (*iter);
            result->IceServers->Append(ToCx(value));
          }
        }
        return result;
      }

      RTCIceGatherOptions^ ToCx(IICEGathererTypes::OptionsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICEGatherer::OptionsPtr FromCx(RTCIceGatherOptions^ input)
      {
        if (nullptr == input) return IICEGatherer::OptionsPtr();
        auto result = make_shared<IICEGatherer::Options>();
        result->mContinuousGathering = input->ContinuousGathering;
        if (input->InterfacePolicies)
        {
          for (RTCIceInterfacePolicy^ value : input->InterfacePolicies)
          {
            if (nullptr == value) continue;
            result->mInterfacePolicies.push_front(*FromCx(value));
          }
        }
        if (input->IceServers)
        {
          for (RTCIceServer^ value : input->IceServers)
          {
            if (nullptr == value) continue;
            result->mICEServers.push_front(*FromCx(value));
          }
        }
        return result;
      }

    } // namespace internal

    RTCIceGatherer::RTCIceGatherer() :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCIceGatherer^ RTCIceGatherer::Convert(IICEGathererPtr gatherer)
    {
      RTCIceGatherer^ result = ref new RTCIceGatherer();
      result->_nativePointer = gatherer;
      return result;
    }

    RTCIceGatherer::RTCIceGatherer(RTCIceGatherOptions^ options) :
      _nativeDelegatePointer(new RTCIceGathererDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);

      assert(nullptr != options);
      _nativePointer = IICEGatherer::create(_nativeDelegatePointer, *internal::FromCx(options));
    }

    RTCIceParameters^ RTCIceGatherer::GetLocalParameters()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->getLocalParameters());
      }

      return nullptr;
    }

    IVector<RTCIceCandidate^>^ RTCIceGatherer::GetLocalCandidates()
    {
      auto ret = ref new Vector<RTCIceCandidate^>();

      if (_nativePointer)
      {
        auto candidates = _nativePointer->getLocalCandidates();
        for (IICEGatherer::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it) {
          ret->Append(internal::ToCx(*it));
        }
      }

      return ret;

    }

    RTCIceGatherer^ RTCIceGatherer::CreateAssociatedGatherer()
    {
      RTCIceGatherer^ ret = ref new RTCIceGatherer();

      if (_nativePointer)
      {
        ret->_nativeDelegatePointer = make_shared<RTCIceGathererDelegate>(RTCIceGathererDelegate());
        ret->_nativePointer = _nativePointer->createAssociatedGatherer(ret->_nativeDelegatePointer);
        ret->_nativeDelegatePointer->SetOwnerObject(ret);
      }
      return ret;
    }

    void RTCIceGatherer::Gather(RTCIceGatherOptions^ options)
    {
      if (_nativePointer)
      {
        if (nullptr != options)
        {
          _nativePointer->gather(*internal::FromCx(options));
        }
        else
        {
          _nativePointer->gather();
        }
      }
    }

    void RTCIceGatherer::Close()
    {
      if (_nativePointer)
        _nativePointer->close();
    }

    Platform::String^ RTCIceGatherer::ToString()
    {
      throw ref new Platform::NotImplementedException();
    }

    Platform::String^ RTCIceGatherer::ToString(RTCIceGatherPolicy value)
    {
      return UseHelper::ToCx(IICEGatherer::toString(UseHelper::convert(value)));
    }

    Platform::String^ RTCIceGatherer::ToString(RTCIceGathererState value)
    {
      return UseHelper::ToCx(IICEGatherer::toString(UseHelper::convert(value)));
    }

    Platform::String^ RTCIceGatherer::ToString(RTCIceGathererCredentialType value)
    {
      return UseHelper::ToCx(IICEGatherer::toString(UseHelper::convert(value)));
    }

    RTCIceGatherPolicy RTCIceGatherer::ToPolicy(Platform::String^ str)
    {
      return UseHelper::convert(IICEGatherer::toPolicy(UseHelper::FromCx(str).c_str()));
    }

    RTCIceGathererState RTCIceGatherer::ToState(Platform::String^ str)
    {
      return UseHelper::convert(IICEGatherer::toState(UseHelper::FromCx(str).c_str()));
    }

    RTCIceGathererCredentialType RTCIceGatherer::ToCredentialType(Platform::String^ str)
    {
      return UseHelper::convert(IICEGatherer::toCredentialType(UseHelper::FromCx(str).c_str()));
    }

    RTCIceGathererState RTCIceGatherer::State::get()
    {
      if (_nativePointer)
        return UseHelper::convert(_nativePointer->state());
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
      evt->State = UseHelper::convert(state);
      _gatherer->OnICEGathererStateChanged(evt);
    }

    void RTCIceGathererDelegate::onICEGathererLocalCandidate(
      IICEGathererPtr gatherer,
      CandidatePtr candidate
      )
    {
      auto evt = ref new RTCIceGathererCandidateEvent();
      evt->Candidate = internal::ToCx(candidate);
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
      evt->Candidate = internal::ToCx(candidate);
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
      evt->Error->ErrorReason = UseHelper::ToCx(errorReason);
      _gatherer->OnICEGathererError(evt);
    }
  } // namespace ortc
} // namespace org
