#pragma once

#include <ortc/IICETransportController.h>
#include <collection.h>
#include "RTCIceTransport.h"

using namespace ortc;

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{

  public ref class RTCIceTransportController sealed
  {
    friend class FetchNativePointer;
  public:
    RTCIceTransportController();

    IVector<RTCIceTransport^>^ GetTransports();
    void                       AddTransport(RTCIceTransport^ transport, size_t index);
  private:
    IICETransportControllerPtr mNativePointer;
  };
}