#include "pch.h"

#include <org/ortc/RTCIceGatherer.h>
#include <org/ortc/RTCIceTransport.h>
#include <org/ortc/RTCIceTypes.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

using namespace ortc;

using Platform::Collections::Vector;

namespace Org
{
  namespace Ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper)

    namespace Internal
    {
#pragma region RTCIceTransport conversions

      RTCIceCandidatePair^ ToCx(const IICETransportTypes::CandidatePair &input)
      {
        auto result = ref new RTCIceCandidatePair();
        result->Local = ToCx(input.mLocal);
        result->Remote = ToCx(input.mRemote);
        return result;
      }

      RTCIceCandidatePair^ ToCx(IICETransportTypes::CandidatePairPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICETransportTypes::CandidatePairPtr FromCx(RTCIceCandidatePair^ input)
      {
        if (nullptr == input) return IICETransport::CandidatePairPtr();
        auto result = make_shared<IICETransport::CandidatePair>();
        result->mLocal = FromCx(input->Local);
        result->mRemote = FromCx(input->Remote);
        return result;
      }

      RTCIceTransportOptions^ ToCx(const IICETransportTypes::Options &input)
      {
        auto result = ref new RTCIceTransportOptions();
        result->AggressiveIce = input.mAggressiveICE;
        result->Role = Helper::Convert(input.mRole);
        return result;
      }

      RTCIceTransportOptions^ ToCx(IICETransportTypes::OptionsPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IICETransportTypes::OptionsPtr FromCx(RTCIceTransportOptions^ input)
      {
        if (nullptr == input) return IICETransport::OptionsPtr();
        auto result = make_shared<IICETransport::Options>();
        result->mAggressiveICE = input->AggressiveIce;
        result->mRole = Helper::Convert(input->Role);
        return result;
      }

#pragma endregion

#pragma region RTCIceTransport delegates

      class RTCIceTransportDelegate : public IICETransportDelegate
      {
      public:
        RTCIceTransportDelegate(RTCIceTransport^ owner) { _owner = owner; }

        virtual void onICETransportStateChange(
          IICETransportPtr transport,
          IICETransport::States state
          ) override
        {
          auto evt = ref new RTCIceTransportStateChangedEvent();
          evt->_state = UseHelper::Convert(state);
          _owner->OnStateChange(evt);
        }

        virtual void onICETransportCandidatePairAvailable(
          IICETransportPtr transport,
          CandidatePairPtr candidatePair
          ) override
        {
          auto evt = ref new RTCIceCandidatePairChangedEvent();
          evt->_candidatePair = Internal::ToCx(candidatePair);
          _owner->OnCandidatePairAvailable(evt);
        }

        virtual void onICETransportCandidatePairGone(
          IICETransportPtr transport,
          CandidatePairPtr candidatePair
          )
        {
          auto evt = ref new RTCIceCandidatePairChangedEvent();
          evt->_candidatePair = Internal::ToCx(candidatePair);
          _owner->OnCandidatePairGone(evt);
        }

        virtual void onICETransportCandidatePairChanged(
          IICETransportPtr transport,
          CandidatePairPtr candidatePair
          ) override
        {
          auto evt = ref new RTCIceCandidatePairChangedEvent();
          evt->_candidatePair = Internal::ToCx(candidatePair);
          _owner->OnCandidatePairChange(evt);
        }

      private:
        RTCIceTransport^ _owner;
      };

#pragma endregion

    } // namespace internal

#pragma region RTCIceTransport

    RTCIceTransport::RTCIceTransport(const noop &) :
      _nativeDelegatePointer(make_shared<Internal::RTCIceTransportDelegate>(this)),
      _nativePointer(nullptr)
    {
    }

    RTCIceTransport::RTCIceTransport(IICETransportPtr transport) :
      _nativeDelegatePointer(make_shared<Internal::RTCIceTransportDelegate>(this)),
      _nativePointer(transport)
    {
      if (_nativePointer) {
        _nativeSubscriptionPointer = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    RTCIceTransport::RTCIceTransport() :
      _nativeDelegatePointer(make_shared<Internal::RTCIceTransportDelegate>(this))
    {
      try
      {
        _nativePointer = IICETransport::create(_nativeDelegatePointer);
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    RTCIceTransport::RTCIceTransport(RTCIceGatherer^ gatherer) :
      _nativeDelegatePointer(make_shared<Internal::RTCIceTransportDelegate>(this))
    {
      auto nativeGatherer = RTCIceGatherer::Convert(gatherer);
      _nativePointer = IICETransport::create(_nativeDelegatePointer, nativeGatherer);
    }

    IVector<RTCIceCandidate^>^ RTCIceTransport::GetRemoteCandidates()
    {
      auto ret = ref new Vector<RTCIceCandidate^>();

      if (_nativePointer)
      {
        auto candidates = _nativePointer->getRemoteCandidates();
        for (IICETransport::CandidateList::iterator it = candidates->begin(); it != candidates->end(); ++it)
        {
          ret->Append(Internal::ToCx(*it));
        }
      }
      return ret;
    }

    RTCIceCandidatePair^ RTCIceTransport::GetSelectedCandidatePair()
    {
      if (!_nativePointer) return nullptr;
      return Internal::ToCx(_nativePointer->getSelectedCandidatePair()); // should it be getSelectedCandidatePair???
    }

    void RTCIceTransport::Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteParameters)

      auto nativeGatherer = RTCIceGatherer::Convert(gatherer);
      _nativePointer->start(nativeGatherer, *Internal::FromCx(remoteParameters));
    }

    void RTCIceTransport::Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role)
    {
      RTCIceTransportOptions^ options = ref new RTCIceTransportOptions;
      options->AggressiveIce = true;
      options->Role = role;

      Start(gatherer, remoteParameters, options);
    }

    void RTCIceTransport::Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceTransportOptions^ options)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteParameters)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == options)

      auto nativeGatherer = RTCIceGatherer::Convert(gatherer);

      try
      {
        _nativePointer->start(nativeGatherer, *Internal::FromCx(remoteParameters), *Internal::FromCx(options));
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCIceTransport::Stop()
    {
      if (!_nativePointer) return;
      _nativePointer->stop();
    }

    RTCIceParameters^ RTCIceTransport::GetRemoteParameters()
    {
      if (!_nativePointer) return nullptr;
      return Internal::ToCx(_nativePointer->getRemoteParameters());
    }

    RTCIceTransport^ RTCIceTransport::CreateAssociatedTransport()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      RTCIceTransport^ ret = nullptr;

      try
      {
        auto nativePointer = _nativePointer->createAssociatedTransport(IICETransportDelegatePtr());
        ORG_ORTC_THROW_UNEXPECTED_IF(!nativePointer);
        ret = RTCIceTransport::Convert(nativePointer);
      } 
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }

      return ret;
    }

    void RTCIceTransport::AddRemoteCandidate(RTCIceCandidate^ remoteCandidate)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteCandidate)

      try
      {
        _nativePointer->addRemoteCandidate(*Internal::FromCx(remoteCandidate));
      }     
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS();
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCIceTransport::AddRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteCandidate)
      try
      {
        _nativePointer->addRemoteCandidate(*Internal::FromCx(remoteCandidate));
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS();
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCIceTransport::SetRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteCandidates)

      IICETypes::CandidateList list;

      for (RTCIceCandidate^ candidate : remoteCandidates)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == candidate)
        list.push_back(*Internal::FromCx(candidate));
      }

      try
      {
        _nativePointer->setRemoteCandidates(list);
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS();
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCIceTransport::KeepWarm(RTCIceCandidatePair^ candidatePair)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == candidatePair)
      _nativePointer->keepWarm(*Internal::FromCx(candidatePair));
    }

    void RTCIceTransport::KeepWarm(RTCIceCandidatePair^ candidatePair, Platform::Boolean keepWarm)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == candidatePair)
      _nativePointer->keepWarm(*Internal::FromCx(candidatePair), keepWarm);
    }

    RTCIceGatherer^ RTCIceTransport::IceGatherer::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCIceGatherer::Convert(_nativePointer->iceGatherer());
    }

    RTCIceComponent RTCIceTransport::Component::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::Convert(_nativePointer->component());
    }

    RTCIceRole RTCIceTransport::Role::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::Convert(_nativePointer->role());
    }

    RTCIceTransportState RTCIceTransport::State::get()
    {
      if (!_nativePointer) return RTCIceTransportState::Closed;
      return UseHelper::Convert(_nativePointer->state());
    }

#pragma endregion

  } // namespace ortc
} // namespace org

