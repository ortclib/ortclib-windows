#include "pch.h"

#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCIceTypes.h"
#include "helpers.h"

using namespace ortc;

using Platform::Collections::Vector;

namespace ortc { ZS_DECLARE_SUBSYSTEM(ortclib) }

namespace org
{
  namespace ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
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
        result->Role = Helper::convert(input.mRole);
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
        result->mRole = Helper::convert(input->Role);
        return result;
      }
    } // namespace internal

    RTCIceTransport::RTCIceTransport(const noop &) :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCIceTransport^ RTCIceTransport::Convert(IICETransportPtr transport)
    {
      RTCIceTransport^ result = ref new RTCIceTransport();
      result->_nativePointer = transport;
      return result;
    }

    RTCIceTransport::RTCIceTransport() :
      _nativeDelegatePointer(new RTCIceTransportDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);
      _nativePointer = IICETransport::create(_nativeDelegatePointer);
    }

    RTCIceTransport::RTCIceTransport(RTCIceGatherer^ gatherer) :
      _nativeDelegatePointer(new RTCIceTransportDelegate())
    {
      _nativeDelegatePointer->SetOwnerObject(this);

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
          ret->Append(internal::ToCx(*it));
        }
      }
      return ret;
    }

    RTCIceCandidatePair^ RTCIceTransport::GetSelectedCandidatePair()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->getSelectedCandidatePair()); // should it be getSelectedCandidatePair???
    }

    void RTCIceTransport::Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteParameters)

      auto nativeGatherer = RTCIceGatherer::Convert(gatherer);
      _nativePointer->start(nativeGatherer, *internal::FromCx(remoteParameters));
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
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteParameters)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == options)

      auto nativeGatherer = RTCIceGatherer::Convert(gatherer);
      _nativePointer->start(nativeGatherer, *internal::FromCx(remoteParameters), *internal::FromCx(options));
    }

    void RTCIceTransport::Stop()
    {
      if (!_nativePointer) return;
      _nativePointer->stop();
    }

    RTCIceParameters^ RTCIceTransport::GetRemoteParameters()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->getRemoteParameters());
    }

    RTCIceTransport^ RTCIceTransport::CreateAssociatedTransport()
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      auto ret = ref new RTCIceTransport(noop{});

      ret->_nativeDelegatePointer = make_shared<RTCIceTransportDelegate>(RTCIceTransportDelegate());
      ret->_nativePointer = _nativePointer->createAssociatedTransport();
      ret->_nativeDelegatePointer->SetOwnerObject(ret);
    
      return ret;
    }

    void RTCIceTransport::AddRemoteCandidate(RTCIceCandidate^ remoteCandidate)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteCandidate)
      _nativePointer->addRemoteCandidate(*internal::FromCx(remoteCandidate));
    }

    void RTCIceTransport::AddRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteCandidate)
      _nativePointer->addRemoteCandidate(*internal::FromCx(remoteCandidate));
    }

    void RTCIceTransport::SetRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == remoteCandidates)

      IICETypes::CandidateList list;

      for (RTCIceCandidate^ candidate : remoteCandidates)
      {
        ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == candidate)
        list.push_back(*internal::FromCx(candidate));
      }

      _nativePointer->setRemoteCandidates(list);
    }

    void RTCIceTransport::KeepWarm(RTCIceCandidatePair^ candidatePair)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == candidatePair)
      _nativePointer->keepWarm(*internal::FromCx(candidatePair));
    }

    void RTCIceTransport::KeepWarm(RTCIceCandidatePair^ candidatePair, Platform::Boolean keepWarm)
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == candidatePair)
      _nativePointer->keepWarm(*internal::FromCx(candidatePair), keepWarm);
    }

    RTCIceGatherer^ RTCIceTransport::IceGatherer::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCIceGatherer::Convert(_nativePointer->iceGatherer());
    }

    RTCIceComponent RTCIceTransport::Component::get()
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::convert(_nativePointer->component());
    }

    RTCIceRole RTCIceTransport::Role::get()
    {
      ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return UseHelper::convert(_nativePointer->role());
    }

    RTCIceTransportState RTCIceTransport::State::get()
    {
      if (!_nativePointer) return RTCIceTransportState::Closed;
      return UseHelper::convert(_nativePointer->state());
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
      evt->CandidatePair = internal::ToCx(candidatePair);
      _transport->OnICETransportCandidatePairAvailable(evt);
    }

    void RTCIceTransportDelegate::onICETransportCandidatePairGone(
      IICETransportPtr transport,
      CandidatePairPtr candidatePair
      )
    {
      auto evt = ref new RTCIceTransportCandidatePairEvent();
      evt->CandidatePair = internal::ToCx(candidatePair);
      _transport->OnICETransportCandidatePairGone(evt);
    }

    void RTCIceTransportDelegate::onICETransportCandidatePairChanged(
      IICETransportPtr transport,
      CandidatePairPtr candidatePair
      )
    {
      auto evt = ref new RTCIceTransportCandidatePairEvent();
      evt->CandidatePair = internal::ToCx(candidatePair);
      _transport->OnICETransportCandidatePairChanged(evt);
    }

  } // namespace ortc
} // namespace org

