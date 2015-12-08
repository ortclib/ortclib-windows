#pragma once

#include <ortc/IICETransport.h>
#include <collection.h>
#include "RTCIceGatherer.h"

using namespace ortc;

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{

  ZS_DECLARE_CLASS_PTR(RTCIceTransportDelegate)

  ref class RTCIceTransport;
  ref class RTCIceCandidate;

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
    State_New,
    State_Checking,
    State_Connected,
    State_Completed,
    State_Disconnected,
    State_Failed,
    State_Closed,
  };

  public enum class RTCIceRole
  {
    Role_Controlling,
    Role_Controlled,
  };

  public ref class RTCIceCandidatePair sealed
  {
  public:
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
      RTCIceTransportState  get(){ return m_state; }
      void  set(RTCIceTransportState value){ m_state = value; }
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
      RTCIceCandidatePair^  get(){ return m_candidatePair; }
      void  set(RTCIceCandidatePair^ value){ m_candidatePair = value; }
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
    friend class RTCIceTransportDelegate;
    friend class FetchNativePointer;
  public:
    RTCIceTransport();
    RTCIceTransport(RTCIceGatherer^ gatherer);

    IVector<RTCIceCandidate^>^ getRemoteCandidates();
    RTCIceCandidatePair^       getSelectedCandidatePair();
    void                       start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role);
    void                       stop();
    RTCIceParameters^          getRemoteParameters();
    RTCIceTransport^           createAssociatedTransport();
    void                       addRemoteCandidate(RTCIceCandidate^ remoteCandidate);
    void                       addRemoteCandidate(RTCIceCandidateComplete^ remoteCandidate);
    void                       setRemoteCandidates(IVector<RTCIceCandidate^>^ remoteCandidates);
  private:
    IICETransportPtr mNativePointer;
    RTCIceTransportDelegatePtr mNativeDelegatePointer;

  public:

    event RTCIceTransportStateChangedDelegate^            OnICETransportStateChanged;
    event RTCIceTransportCandidatePairAvailableDelegate^  OnICETransportCandidatePairAvailable;
    event RTCIceTransportCandidatePairGoneDelegate^       OnICETransportCandidatePairGone;
    event RTCIceTransportCandidatePairChangedDelegate^    OnICETransportCandidatePairChanged;
  };
}