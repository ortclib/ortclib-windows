#include "pch.h"

#include <org/ortc/RTCIceGatherer.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

#include <zsLib/SafeInt.h>

using namespace ortc;

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

#pragma region RTCIceGatherer conversions

      RTCIceInterfacePolicy^ ToCx(const IICEGathererTypes::InterfacePolicy &input)
      {
        auto result = ref new RTCIceInterfacePolicy();
        result->InterfaceType = Helper::ToCx(input.mInterfaceType);
        result->GatherPolicy = Helper::Convert(input.mGatherPolicy);
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
        result->mGatherPolicy = Helper::Convert(input->GatherPolicy);
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
        result->Username = Helper::ToCx(input.mUserName);
        result->Credential = Helper::ToCx(input.mCredential);
        result->CredentialType = Helper::Convert(input.mCredentialType);
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
        result->mUserName = Helper::FromCx(input->Username);
        result->mCredential = Helper::FromCx(input->Credential);
        result->mCredentialType = Helper::Convert(input->CredentialType);
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

#pragma endregion

#pragma region RTCIceGatherer delegates

      class RTCIceGathererDelegate : public IICEGathererDelegate
      {
      public:
        RTCIceGathererDelegate(RTCIceGatherer^ owner) { _owner = owner; }

        virtual void onICEGathererStateChange(
          IICEGathererPtr gatherer,
          IICEGatherer::States state
          ) override
        {
          auto evt = ref new RTCIceGathererStateChangedEvent();
          evt->_state = UseHelper::Convert(state);
          _owner->OnStateChange(evt);
        }

        virtual void onICEGathererLocalCandidate(
          IICEGathererPtr gatherer,
          CandidatePtr candidate
          ) override
        {
          auto evt = ref new RTCIceGathererCandidateEvent();
          evt->_candidate = internal::ToCx(candidate);
          _owner->OnLocalCandidate(evt);
        }


        virtual void onICEGathererLocalCandidateComplete(
          IICEGathererPtr gatherer,
          CandidateCompletePtr candidate
          ) override
        {
          auto evt = ref new RTCIceGathererCandidateCompleteEvent();
          evt->_candidate = ref new RTCIceCandidateComplete();;
          evt->_candidate->Complete = true;
          _owner->OnLocalCandidateComplete(evt);
        }

        virtual void onICEGathererLocalCandidateGone(
          IICEGathererPtr gatherer,
          CandidatePtr candidate
          ) override
        {
          auto evt = ref new RTCIceGathererCandidateEvent();
          evt->_candidate = internal::ToCx(candidate);
          _owner->OnLocalCandidateGone(evt);
        }

        virtual void onICEGathererError(
          IICEGathererPtr gatherer,
          ErrorEventPtr errorEvent
          ) override
        {
          auto evt = ref new RTCIceGathererIceErrorEvent();
          if (errorEvent)
          {
            evt->_hostCandidate = internal::ToCx(errorEvent->mHostCandidate);
            evt->_url = UseHelper::ToCx(errorEvent->mURL);
            evt->_errorCode = SafeInt<decltype(evt->_errorCode)>(errorEvent->mErrorCode);
            evt->_errorText = UseHelper::ToCx(errorEvent->mErrorText);
          }
          _owner->OnError(evt);
        }

      private:
        RTCIceGatherer^ _owner;
      };

#pragma endregion

    } // namespace internal

    RTCIceGatherer::RTCIceGatherer(const noop &) :
      _nativeDelegatePointer(make_shared<internal::RTCIceGathererDelegate>(this))
    {
    }

    RTCIceGatherer::RTCIceGatherer(IICEGathererPtr gatherer) :
      _nativeDelegatePointer(make_shared<internal::RTCIceGathererDelegate>(this)),
      _nativePointer(gatherer)
    {
    }

    RTCIceGatherer::RTCIceGatherer(RTCIceGatherOptions^ options) :
      _nativeDelegatePointer(make_shared<internal::RTCIceGathererDelegate>(this))
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == options)
      _nativePointer = IICEGatherer::create(_nativeDelegatePointer, *internal::FromCx(options));
    }

    RTCIceParameters^ RTCIceGatherer::GetLocalParameters()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->getLocalParameters());
    }

    IVector<RTCIceCandidate^>^ RTCIceGatherer::GetLocalCandidates()
    {
      auto ret = ref new Vector<RTCIceCandidate^>();
      if (!_nativePointer) return ret;

      auto candidates = _nativePointer->getLocalCandidates();
      for (IICEGatherer::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it) {
        ret->Append(internal::ToCx(*it));
      }

      return ret;
    }

    RTCIceGatherer^ RTCIceGatherer::CreateAssociatedGatherer()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      RTCIceGatherer^ ret = ref new RTCIceGatherer(noop{});

      try
      {
        ret->_nativePointer = _nativePointer->createAssociatedGatherer(ret->_nativeDelegatePointer);
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()));
      }

      return ret;
    }

    void RTCIceGatherer::Gather(RTCIceGatherOptions^ options)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      if (nullptr != options)
      {
        _nativePointer->gather(*internal::FromCx(options));
      }
      else
      {
        _nativePointer->gather();
      }
    }

    void RTCIceGatherer::Close()
    {
      if (!_nativePointer) return;
      _nativePointer->close();
    }
    
    RTCIceComponent RTCIceGatherer::Component::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::Convert(_nativePointer->component());
    }

    RTCIceGathererState RTCIceGatherer::State::get()
    {
      if (!_nativePointer) return RTCIceGathererState::Closed;
      return UseHelper::Convert(_nativePointer->state());
    }


    Platform::String^ RTCIceGatherer::IceGatherPolicyToString(RTCIceGatherFilterPolicy value)
    {
      return UseHelper::ToCx(IICEGatherer::toString(UseHelper::Convert(value)));
    }

    RTCIceGatherFilterPolicy RTCIceGatherer::ToIceGatherPolicy(Platform::String^ str)
    {
      return UseHelper::Convert(IICEGatherer::toPolicy(UseHelper::FromCx(str).c_str()));
    }

  } // namespace ortc
} // namespace org
