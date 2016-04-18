#pragma once

#include <ortc/IICETransportController.h>
//#include <collection.h>
//#include "RTCIceTransport.h"
//
//using namespace ortc;
//
//
namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransportController, IICETransportController)
    
    ref class RTCIceTransport;

    public ref class RTCIceTransportController sealed
    {

    public:
      RTCIceTransportController();

      IVector<RTCIceTransport^>^ GetTransports();
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       AddTransport(RTCIceTransport^ transport);
      [Windows::Foundation::Metadata::OverloadAttribute("AddTransportAtIndex")]
      void                       AddTransport(RTCIceTransport^ transport, size_t index);

    private:
      IICETransportControllerPtr _nativePointer;
    };
  }
}
