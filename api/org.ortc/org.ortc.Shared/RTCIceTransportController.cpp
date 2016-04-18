#include "pch.h"

#include "RTCIceTransportController.h"
#include "RTCIceTransport.h"

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
      if (_nativePointer)
      {
        auto candidates = _nativePointer->getTransports();
        for (IICETransportControllerTypes::ICETransportList::iterator it = candidates.begin(); it != candidates.end(); ++it) {
          ret->Append(RTCIceTransport::Convert(*it));
        }
      }
      return ret;
    }

    void RTCIceTransportController::AddTransport(RTCIceTransport^ transport)
    {
      if (_nativePointer)
      {
        _nativePointer->addTransport(RTCIceTransport::Convert(transport));
      }
    }

    void RTCIceTransportController::AddTransport(RTCIceTransport^ transport, size_t index)
    {
      if (_nativePointer)
      {
        _nativePointer->addTransport(RTCIceTransport::Convert(transport), index);
      }
    }

  } // namespace ortc
} // namespace org
