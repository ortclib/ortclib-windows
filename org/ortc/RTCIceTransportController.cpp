#include "pch.h"

#include <org/ortc/RTCIceTransportController.h>
#include <org/ortc/RTCIceTransport.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

using namespace ortc;

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper);
    
    RTCIceTransportController::RTCIceTransportController(IICETransportControllerPtr nativePointer) :
      _nativePointer(nativePointer)
    {
    }

    RTCIceTransportController::RTCIceTransportController()
    {
      _nativePointer = IICETransportController::create();
    }

    IVector<RTCIceTransport^>^ RTCIceTransportController::GetTransports()
    {
      auto ret = ref new Vector<RTCIceTransport^>();
      if (!_nativePointer) return ret;
      auto candidates = _nativePointer->getTransports();
      for (IICETransportControllerTypes::ICETransportList::iterator it = candidates.begin(); it != candidates.end(); ++it) {
        ret->Append(RTCIceTransport::Convert(*it));
      }
      return ret;
    }

    void RTCIceTransportController::AddTransport(RTCIceTransport^ transport)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      try
      {
        _nativePointer->addTransport(RTCIceTransport::Convert(transport));
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

    void RTCIceTransportController::AddTransport(RTCIceTransport^ transport, size_t index)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      try
      {
        _nativePointer->addTransport(RTCIceTransport::Convert(transport), index);
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

  } // namespace ortc
} // namespace org
