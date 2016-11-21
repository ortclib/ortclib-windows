#include "pch.h"

#include <org/ortc/RTCDataChannel.h>
#include <org/ortc/RTCSctpTransport.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

#include <ortc/services/IHelper.h>

#include <zsLib/SafeInt.h>

using namespace ortc;

namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::services::IHelper, UseServicesHelper);

    namespace Internal
    {
      RTCSctpCapabilities^ ToCx(const ISCTPTransportTypes::Capabilities &input)
      {
        auto result = ref new RTCSctpCapabilities();
        result->MaxMessageSize = SafeInt<uint32>(input.mMaxMessageSize);
        result->MinPort = SafeInt<uint16>(input.mMinPort);
        result->MaxPort = SafeInt<uint16>(input.mMaxPort);
        result->MaxUsablePorts = SafeInt<uint16>(input.mMaxUsablePorts);
        result->MaxSessionsPerPort = SafeInt<uint16>(input.mMaxSessionsPerPort);
        return result;
      }

      RTCSctpCapabilities^ ToCx(ISCTPTransportTypes::CapabilitiesPtr input)
      {
        if (!input) return nullptr;
        return ToCx(input);
      }

      ISCTPTransportTypes::CapabilitiesPtr FromCx(RTCSctpCapabilities^ input)
      {
        if (nullptr == input) return ISCTPTransport::CapabilitiesPtr();
        auto result = std::make_shared<ISCTPTransport::Capabilities>();

        result->mMaxMessageSize = SafeInt<uint32>(input->MaxMessageSize);
        result->mMinPort = SafeInt<uint16>(input->MinPort);
        result->mMaxPort = SafeInt<uint16>(input->MaxPort);
        result->mMaxUsablePorts = SafeInt<uint16>(input->MaxUsablePorts);
        result->mMaxSessionsPerPort = SafeInt<uint16>(input->MaxSessionsPerPort);
        return result;
      }

      class RTCSctpTransportDelegate : public ISCTPTransportDelegate
      {
      public:
        RTCSctpTransportDelegate(RTCSctpTransport^ owner) { _owner = owner; }

        virtual void onSCTPTransportDataChannel(
          ISCTPTransportPtr transport,
          IDataChannelPtr channel
          ) override
        {
          auto evt = ref new RTCDataChannelEvent();
          evt->_channel = RTCDataChannel::Convert(channel);
          _owner->OnDataChannel(evt);
        }

        virtual void onSCTPTransportStateChange(
          ISCTPTransportPtr transport,
          ISCTPTransportTypes::States state
          ) override
        {
          auto evt = ref new RTCSctpTransportStateChangeEvent();
          evt->_state = UseHelper::Convert(state);
          _owner->OnStateChange(evt);
        }

      private:
        RTCSctpTransport^ _owner;
      };


    } //namespace internal

    RTCSctpTransport::RTCSctpTransport(ISCTPTransportPtr transport) :
      _nativeDelegatePointer(make_shared<Internal::RTCSctpTransportDelegate>(this)),
      _nativePointer(transport)
    {
      if (_nativePointer)
      {
        _nativeSubscriptionPointer = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    RTCSctpTransport::RTCSctpTransport(RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(make_shared<Internal::RTCSctpTransportDelegate>(this))
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);

      try
      {
        _nativePointer = ISCTPTransport::create(_nativeDelegatePointer, nativeTransport);
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

    RTCSctpTransport::RTCSctpTransport(RTCDtlsTransport^ transport, uint16 port) :
      _nativeDelegatePointer(make_shared<Internal::RTCSctpTransportDelegate>(this))
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);

      try
      {
        _nativePointer = ISCTPTransport::create(_nativeDelegatePointer, nativeTransport, port);
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

    RTCSctpCapabilities^ RTCSctpTransport::GetCapabilities()
    {
      RTCSctpCapabilities^ ret = ref new RTCSctpCapabilities();

      ISCTPTransportTypes::CapabilitiesPtr caps = ISCTPTransport::getCapabilities();
      ret->MaxMessageSize = SafeInt<decltype(ret->MaxMessageSize)>(caps->mMaxMessageSize);

      return ret;
    }

    void RTCSctpTransport::Start(RTCSctpCapabilities^ remoteCaps)
    {
      if (_nativePointer)
      {
        ISCTPTransportTypes::Capabilities caps;
        caps.mMaxMessageSize = remoteCaps->MaxMessageSize;
        _nativePointer->start(caps);
      }
    }

    void RTCSctpTransport::Stop()
    {
      if (_nativePointer)
      {
        _nativePointer->stop();
      }
    }

    RTCDtlsTransport^ RTCSctpTransport::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(_nativePointer->transport());
    }

    uint16 RTCSctpTransport::Port::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return _nativePointer->port();
    }

    Platform::String^ RTCSctpCapabilities::ToJsonString()
    {
      auto caps = Internal::FromCx(this);
      return UseHelper::ToCx(UseServicesHelper::toString(caps->createElement("SctpCapabilities")));
    }

    RTCSctpCapabilities^ RTCSctpCapabilities::FromJsonString(Platform::String^ jsonString)
    {
      auto caps = make_shared<ISCTPTransport::Capabilities>(ISCTPTransport::Capabilities::Capabilities(UseServicesHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return Internal::ToCx(caps);
    }
  } // namespace ortc
} // namespace org
