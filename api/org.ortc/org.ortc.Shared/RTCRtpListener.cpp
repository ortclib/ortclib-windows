#include "pch.h"

#include "RTCDtlsTransport.h"
#include "RTCRtpListener.h"
#include "RtpTypes.h"
#include "helpers.h"

#include <zsLib/SafeInt.h>

using std::make_shared;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
      class RTCRtpListenerDelegate : public IRTPListenerDelegate
      {
      public:
        RTCRtpListenerDelegate(RTCRtpListener^ owner) { _owner = owner; }

        virtual void onRTPListenerUnhandledRTP(
          IRTPListenerPtr listener,
          SSRCType ssrc,
          PayloadType payloadType,
          const char *mid,
          const char *rid
          ) override
        {
          auto evt = ref new RTCRtpUnhandledEvent();
          evt->_ssrc = SafeInt<decltype(evt->_ssrc)>(ssrc);
          evt->_payloadType = SafeInt<decltype(evt->_payloadType)>(payloadType);
          evt->_muxId = UseHelper::ToCx(mid);
          evt->_rid = UseHelper::ToCx(rid);
          _owner->OnUnhandledRtp(evt);
        }

      private:
        RTCRtpListener^ _owner;
      };
    }

    RTCRtpListener::RTCRtpListener(RTCDtlsTransport^ transport) :
      _nativeDelegatePointer(make_shared<internal::RTCRtpListenerDelegate>(this))
    {
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer = IRTPListener::create(_nativeDelegatePointer, nativeTransport);
    }

    RTCRtpListener::RTCRtpListener(RTCDtlsTransport^ transport, IVector<RTCRtpHeaderExtensionParameters^>^ headerExtensions) :
      _nativeDelegatePointer(make_shared<internal::RTCRtpListenerDelegate>(this))
    {
      IRTPListener::HeaderExtensionParametersList list;

      if (headerExtensions)
      {
        for (RTCRtpHeaderExtensionParameters^ ext : headerExtensions)
        {
          if (nullptr == ext) continue;
          list.push_back(*internal::FromCx(ext));
        }
      }
      auto nativeTransport = RTCDtlsTransport::Convert(transport);
      _nativePointer = IRTPListener::create(_nativeDelegatePointer, nativeTransport, list);
    }

    RTCDtlsTransport^ RTCRtpListener::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCDtlsTransport::Convert(IDTLSTransport::convert(_nativePointer->transport()));
    }

  } // namespace ortc
} // namespace org
