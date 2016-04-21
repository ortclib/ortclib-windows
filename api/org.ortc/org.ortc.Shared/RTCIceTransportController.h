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

    /// <summmary>
    /// The RTCIceTransportController object assists in the managing of ICE
    /// freezing and bandwidth estimation.
    /// </summmary>
    public ref class RTCIceTransportController sealed
    {
    public:
      /// <summmary>
      /// Constructs an instance of the RTCIceTransportController.
      /// </summmary>
      RTCIceTransportController();

      /// <summary>
      /// Returns the RTCIceTransport objects with a component of "RTP". If
      /// addTransport() has not been called, an empty list is returned.
      /// </summary>
      IVector<RTCIceTransport^>^ GetTransports();

      /// <summary>
      /// Adds transport to the RTCIceTransportController object for the
      /// purposes of managing ICE freezing and sharing bandwidth estimation.
      /// Since addTransport manages ICE freezing, candidate pairs that are
      /// not in the frozen state maintain their state when
      /// AddTransport(transport) is called. RTCIceTransport objects will be
      /// unfrozen according to their index. The added transport is inserted
      /// as the final index.
      ///
      ///  If transport.state is "closed", throw an InvalidStateError
      /// exception. If transport has already been added to another
      /// RTCIceTransportController object, or if transport.component is
      /// "RTCP", throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       AddTransport(RTCIceTransport^ transport);
      /// <summary>
      /// The transport is inserted at index. If index is greater than the
      /// current number of RTCIceTransports with a component of "RTP",
      /// throw an InvalidParameters exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("AddTransportAtIndex")]
      void                       AddTransport(RTCIceTransport^ transport, size_t index);

    private:
      IICETransportControllerPtr _nativePointer;
    };
  }
}
