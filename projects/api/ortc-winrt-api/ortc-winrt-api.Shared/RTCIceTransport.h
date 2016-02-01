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
    New,
    Checking,
    Connected,
    Completed,
    Disconnected,
    Failed,
    Closed,
  };

  public enum class RTCIceRole
  {
    Controlling,
    Controlled
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
    friend class ConvertObjectToCx;
    friend class CreateEmptyCxObject;
  private:
    RTCIceTransport(Platform::Boolean noop);
  public:
    RTCIceTransport();
    RTCIceTransport(RTCIceGatherer^ Gatherer);

    IVector<RTCIceCandidate^>^ GetRemoteCandidates();
    RTCIceCandidatePair^       GetSelectedCandidatePair();
    [Windows::Foundation::Metadata::DefaultOverloadAttribute]
    void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters);
    [Windows::Foundation::Metadata::OverloadAttribute("StartWithIceRole")]
    void                       Start(RTCIceGatherer^ gatherer, RTCIceParameters^ remoteParameters, RTCIceRole role);
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
    [Windows::Foundation::Metadata::OverloadAttribute("KeepWarm1")]
    void                       KeepWarm(RTCIceCandidatePair^ candidatePair, Platform::Boolean keepWarm);
  private:
    IICETransportPtr mNativePointer;
    RTCIceTransportDelegatePtr mNativeDelegatePointer;

  private:
    RTCIceGatherer^ GetIceGatherer();
    
  public:

    property RTCIceGatherer^ IceGatherer
    {
      RTCIceGatherer^ get()
      {
        if (mNativePointer)
          return GetIceGatherer();
        else
          return nullptr;
      }
    }

    property RTCIceComponent Component
    {
      RTCIceComponent get()
      {
        if (mNativePointer)
          return (RTCIceComponent)mNativePointer->component();
        else
          return RTCIceComponent::RTP;
      }
    }

    property RTCIceRole Role
    {
      RTCIceRole get()
      {
        if (mNativePointer)
          return (RTCIceRole)mNativePointer->role();
        else
          return RTCIceRole::Controlled;
      }
    }

    property RTCIceTransportState State
    {
      RTCIceTransportState get()
      {
        if (mNativePointer)
          return (RTCIceTransportState)mNativePointer->state();
        else
          return RTCIceTransportState::Closed;
      }
    }

  public:

    event RTCIceTransportStateChangedDelegate^            OnICETransportStateChanged;
    event RTCIceTransportCandidatePairAvailableDelegate^  OnICETransportCandidatePairAvailable;
    event RTCIceTransportCandidatePairGoneDelegate^       OnICETransportCandidatePairGone;
    event RTCIceTransportCandidatePairChangedDelegate^    OnICETransportCandidatePairChanged;
  };
}