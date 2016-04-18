#pragma once

#include "RTCIceTypes.h"

#include <ortc/IICETransport.h>

//#include <collection.h>
//
//using namespace ortc;
//

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransport, IICETransport)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransportDelegate, IICETransportDelegate)

    ZS_DECLARE_CLASS_PTR(RTCIceTransportDelegate)

    ref class RTCIceGatherer;
    ref class RTCIceTransport;
    ref class RTCIceTransportController;

    ref struct RTCIceCandidate;
    ref struct RTCIceCandidateComplete;
    ref struct RTCIceCandidatePair;
    ref struct RTCIceParameters;
    ref struct RTCIceTransportOptions;

    enum class RTCIceComponent;
    enum class RTCIceRole;

    namespace internal
    {
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IICETransportTypes, IICETransportTypes)

      RTCIceCandidatePair^ ToCx(const IICETransportTypes::CandidatePair &input);
      RTCIceCandidatePair^ ToCx(IICETransportTypes::CandidatePairPtr input);
      IICETransportTypes::CandidatePairPtr FromCx(RTCIceCandidatePair^ input);

      RTCIceTransportOptions^ ToCx(const IICETransportTypes::Options &input);
      RTCIceTransportOptions^ ToCx(IICETransportTypes::OptionsPtr input);
      IICETransportTypes::OptionsPtr FromCx(RTCIceTransportOptions^ input);
    }

    class RTCIceTransportDelegate : public IICETransportDelegate
    {
    public:
      virtual void onICETransportStateChange(
        IICETransportPtr transport,
        IICETransport::States state
        );

      virtual void onICETransportCandidatePairAvailable(
        IICETransportPtr transport,
        CandidatePairPtr candidatePair
        );

      virtual void onICETransportCandidatePairGone(
        IICETransportPtr transport,
        CandidatePairPtr candidatePair
        );

      virtual void onICETransportCandidatePairChanged(
        IICETransportPtr transport,
        CandidatePairPtr candidatePair
        );

      RTCIceTransport^ _transport;

      void SetOwnerObject(RTCIceTransport^ owner) { _transport = owner; }
    };

    //--------------------------------------------------------------------
    // Helper classes
    //--------------------------------------------------------------------

    public enum class RTCIceTransportState
    {
      New,
      Checking,
      Connected,
      Completed,
      Disconnected,
      Failed,
      Closed,
    };

    public ref struct RTCIceTransportOptions sealed
    {
      property Platform::Boolean  AggressiveIce;
      property RTCIceRole         Role;
    };

    public ref struct RTCIceCandidatePair sealed
    {
      property RTCIceCandidate^ Local;
      property RTCIceCandidate^ Remote;
    };

    //------------------------------------------
    // Events and Delegates
    //------------------------------------------

    // State change event and delegate
    public ref class RTCIceTransportStateChangeEvent sealed {
    public:
      property RTCIceTransportState State
      {
        RTCIceTransportState  get() { return m_state; }
        void  set(RTCIceTransportState value) { m_state = value; }
      }

    private:
      RTCIceTransportState m_state;
    };

    public delegate void RTCIceTransportStateChangedDelegate(RTCIceTransportStateChangeEvent^ evt);


    // Candidate pair available event and delegate
    public ref class RTCIceTransportCandidatePairEvent sealed {
    public:
      property RTCIceCandidatePair^ CandidatePair
      {
        RTCIceCandidatePair^  get() { return m_candidatePair; }
        void  set(RTCIceCandidatePair^ value) { m_candidatePair = value; }
      }

    private:
      RTCIceCandidatePair^ m_candidatePair;
    };

    public delegate void RTCIceTransportCandidatePairAvailableDelegate(RTCIceTransportCandidatePairEvent^ evt);
    public delegate void RTCIceTransportCandidatePairGoneDelegate(RTCIceTransportCandidatePairEvent^ evt);
    public delegate void RTCIceTransportCandidatePairChangedDelegate(RTCIceTransportCandidatePairEvent^ evt);

    //------------------------------------------
    // End Events and Delegates
    //------------------------------------------

    public ref class RTCIceTransport sealed
    {
    private:
      struct noop {};
      friend class RTCIceTransportDelegate;
      friend ref class RTCDtlsTransport;
      friend ref class RTCIceTransportController;

    private:
      RTCIceTransport(const noop &);

      static RTCIceTransport^ Convert(IICETransportPtr transport);
      static IICETransportPtr Convert(RTCIceTransport^ gatherer) { return gatherer->_nativePointer; }

    public:
      RTCIceTransport();
      RTCIceTransport(RTCIceGatherer^ Gatherer);

      IVector<RTCIceCandidate^>^ GetRemoteCandidates();
      RTCIceCandidatePair^       GetSelectedCandidatePair();
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters);
      [Windows::Foundation::Metadata::OverloadAttribute("StartWithIceRole")]
      void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role);
      [Windows::Foundation::Metadata::OverloadAttribute("StartWithOptions")]
      void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceTransportOptions^ options);
      void                       Stop();
      RTCIceParameters^          GetRemoteParameters();
      RTCIceTransport^           CreateAssociatedTransport();
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       AddRemoteCandidate(RTCIceCandidate^ remoteCandidate);
      [Windows::Foundation::Metadata::OverloadAttribute("AddRemoteCandidateComplete")]
      void                       AddRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate);
      void                       SetRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates);
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                       KeepWarm(RTCIceCandidatePair^ candidatePair);
      [Windows::Foundation::Metadata::OverloadAttribute("SetKeepWarm")]
      void                       KeepWarm(RTCIceCandidatePair^ candidatePair, Platform::Boolean keepWarm);

      property RTCIceGatherer^ IceGatherer  { RTCIceGatherer^ get(); }
      property RTCIceComponent Component    { RTCIceComponent get(); }
      property RTCIceRole Role              { RTCIceRole get(); }
      property RTCIceTransportState State   { RTCIceTransportState get(); }

    public:
      event RTCIceTransportStateChangedDelegate^            OnICETransportStateChanged;
      event RTCIceTransportCandidatePairAvailableDelegate^  OnICETransportCandidatePairAvailable;
      event RTCIceTransportCandidatePairGoneDelegate^       OnICETransportCandidatePairGone;
      event RTCIceTransportCandidatePairChangedDelegate^    OnICETransportCandidatePairChanged;

    private:
      IICETransportPtr _nativePointer;
      RTCIceTransportDelegatePtr _nativeDelegatePointer;
    };
  }
}
