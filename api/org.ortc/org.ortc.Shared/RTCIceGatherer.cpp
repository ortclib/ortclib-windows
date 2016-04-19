#include "pch.h"

#include "RTCIceGatherer.h"
#include "helpers.h"

#include <zsLib/SafeInt.h>

using namespace ortc;

using Platform::Collections::Vector;

namespace ortc { ZS_DECLARE_SUBSYSTEM(ortclib) }

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
        result->UserName = Helper::ToCx(input.mUserName);
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
        result->mUserName = Helper::FromCx(input->UserName);
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
        virtual void onICEGathererStateChange(
          IICEGathererPtr gatherer,
          IICEGatherer::States state
          ) override;

        virtual void onICEGathererLocalCandidate(
          IICEGathererPtr gatherer,
          CandidatePtr candidate
          ) override;

        virtual void onICEGathererLocalCandidateComplete(
          IICEGathererPtr gatherer,
          CandidateCompletePtr candidate
          ) override;

        virtual void onICEGathererLocalCandidateGone(
          IICEGathererPtr gatherer,
          CandidatePtr candidate
          ) override;

        virtual void onICEGathererError(
          IICEGathererPtr gatherer,
          ErrorEventPtr errorEvent
          ) override;

        void SetOwnerObject(RTCIceGatherer^ owner) { _gatherer = owner; }

      private:
        RTCIceGatherer^ _gatherer;
      };

      void RTCIceGathererDelegate::onICEGathererStateChange(
        IICEGathererPtr gatherer,
        IICEGatherer::States state
        )
      {
        auto evt = ref new RTCIceGathererStateChangedEvent();
        evt->_state = UseHelper::Convert(state);
        _gatherer->OnStateChange(evt);
      }

      void RTCIceGathererDelegate::onICEGathererLocalCandidate(
        IICEGathererPtr gatherer,
        CandidatePtr candidate
        )
      {
        auto evt = ref new RTCIceGathererCandidateEvent();
        evt->_candidate = internal::ToCx(candidate);
        _gatherer->OnLocalCandidate(evt);
      }

      void RTCIceGathererDelegate::onICEGathererLocalCandidateComplete(
        IICEGathererPtr gatherer,
        CandidateCompletePtr candidate
        )
      {
        auto evt = ref new RTCIceGathererCandidateCompleteEvent();
        evt->_candidate = ref new RTCIceCandidateComplete();;
        evt->_candidate->Complete = true;
        _gatherer->OnLocalCandidateComplete(evt);
      }

      void RTCIceGathererDelegate::onICEGathererLocalCandidateGone(
        IICEGathererPtr gatherer,
        CandidatePtr candidate
        )
      {
        auto evt = ref new RTCIceGathererCandidateEvent();
        evt->_candidate = internal::ToCx(candidate);
        _gatherer->OnLocalCandidateGone(evt);
      }

      void RTCIceGathererDelegate::onICEGathererError(
        IICEGathererPtr gatherer,
        ErrorEventPtr errorEvent
        )
      {
        auto evt = ref new RTCIceGathererIceErrorEvent();
        if (errorEvent)
        {
          evt->_hostCandidate = internal::ToCx(errorEvent->mHostCandidate);
          evt->_url = UseHelper::ToCx(errorEvent->mURL);
          evt->_errorCode = SafeInt<decltype(evt->_errorCode)>(errorEvent->mErrorCode);
          evt->_errorText = UseHelper::ToCx(errorEvent->mErrorText);
        }
        _gatherer->OnError(evt);
      }
#pragma endregion

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
      _nativeDelegatePointer(make_shared<internal::RTCIceGathererDelegate>())
    {
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == options)
      _nativeDelegatePointer->SetOwnerObject(this);
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
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      RTCIceGatherer^ ret = ref new RTCIceGatherer();

      ret->_nativeDelegatePointer = make_shared<internal::RTCIceGathererDelegate>(internal::RTCIceGathererDelegate());
      ret->_nativeDelegatePointer->SetOwnerObject(ret);
      ret->_nativePointer = _nativePointer->createAssociatedGatherer(ret->_nativeDelegatePointer);

      return ret;
    }

    void RTCIceGatherer::Gather(RTCIceGatherOptions^ options)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
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
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
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
