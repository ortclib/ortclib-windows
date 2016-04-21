#include "pch.h"

#include "RTCIceTransportController.h"
#include "RTCIceTransport.h"
#include "Error.h"

using namespace ortc;

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    
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
    }

    void RTCIceTransportController::AddTransport(RTCIceTransport^ transport, size_t index)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      _nativePointer->addTransport(RTCIceTransport::Convert(transport), index);
    }

  } // namespace ortc
} // namespace org
