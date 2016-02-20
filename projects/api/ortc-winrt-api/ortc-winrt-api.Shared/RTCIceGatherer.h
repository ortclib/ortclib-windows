#pragma once

#include <ortc/IICEGatherer.h>
#include <collection.h>
#include "RTCIceTypes.h"

using namespace ortc;

using Windows::Foundation::Collections::IVector;
using Platform::Collections::Vector;


namespace ortc_winrt_api
{

  ZS_DECLARE_CLASS_PTR(RTCIceGathererDelegate)

  ref class RTCIceGatherer;
  ref class RTCIceCandidate;

  class RTCIceGathererDelegate : public IICEGathererDelegate
  {
  public:
     virtual void onICEGathererStateChange(
      IICEGathererPtr gatherer,
      IICEGatherer::States state
      );

    virtual void onICEGathererLocalCandidate(
      IICEGathererPtr gatherer,
      CandidatePtr candidate
      );

    virtual void onICEGathererLocalCandidateComplete(
      IICEGathererPtr gatherer,
      CandidateCompletePtr candidate
      );

    virtual void onICEGathererLocalCandidateGone(
      IICEGathererPtr gatherer,
      CandidatePtr candidate
      );

    virtual void onICEGathererError(
      IICEGathererPtr gatherer,
      ErrorCode errorCode,
      zsLib::String errorReason
      );

    RTCIceGatherer^ _gatherer;

    void SetOwnerObject(RTCIceGatherer^ owner) { _gatherer = owner; }

  private:
  };

  public enum class RTCIceGatherPolicy
  {
    None = 0,
    NoIPv4Host = 0x00000001,
    NoIPv4Srflx = 0x00000002,
    NoIPv4Prflx = 0x00000004,
    NoIPv4Relay = 0x00000008,
    NoIPv4Private = 0x00000010,
    NoIPv4 = 0x000000FF,
    NoIPv6Host = 0x00000100,
    NoIPv6Srflx = 0x00000200,
    NoIPv6Prflx = 0x00000400,
    NoIPv6Relay = 0x00000800,
    NoIPv6Private = 0x00001000,
    NoIPv6Tunnel = 0x00002000,
    NoIPv6Permanent = 0x00004000,
    NoIPv6 = 0x0000FF00,
    NoHost = (NoIPv4Host | NoIPv6Host),
    NoSrflx = (NoIPv4Srflx | NoIPv6Srflx),
    NoPrflx = (NoIPv4Prflx | NoIPv6Prflx),
    NoRelay = (NoIPv4Relay | NoIPv6Relay),
    NoPrivate = (NoIPv4Private | NoIPv6Private),
    RelayOnly = (NoIPv4Host | NoSrflx | NoPrflx),
    NoCandidates = (0x7FFFFFFF)
  };

  public enum class RTCIceGathererCredentialType
  {
    Password,
    Token,
  };

  public enum class RTCIceGathererState {
    New,
    Gathering,
    Complete,
    Closed,
  };

  public ref class RTCIceServer sealed
  {
  public:
    property IVector<Platform::String^>^      Urls;
    property Platform::String^                UserName;
    property Platform::String^                Credential;
    property RTCIceGathererCredentialType     CredentialType;
  };

  public ref class RTCIceInterfacePolicy sealed
  {
  public:
    property Platform::String^                InterfaceType;
    property RTCIceGatherPolicy               GatherPolicy;
  };

  public ref class RTCIceGatherOptions sealed
  {
  public:
    property Platform::Boolean                ContinuousGathering;
    property IVector<RTCIceInterfacePolicy^>^ InterfacePolicies;
    property IVector<RTCIceServer^>^          IceServers;

  public:
    RTCIceGatherOptions() { ContinuousGathering = true; }
  };

  public ref class RTCIceGathererError sealed
  {
  public:
    property uint16 ErrorCode;
    property Platform::String^ ErrorReason;
  };


  //------------------------------------------
  // Events and Delegates
  //------------------------------------------
  
  // State change event and delegate
  public ref class RTCIceGathererStateChangeEvent sealed {
  public:
    property RTCIceGathererState State
    {
      RTCIceGathererState  get(){ return _State; }
      void  set(RTCIceGathererState value){ _State = value; }
    }

  private:
    RTCIceGathererState _State;
  };

  public delegate void RTCIceGathererStateChangedDelegate(RTCIceGathererStateChangeEvent^ evt);


  // Candidate event and delegate
  public ref class RTCIceGathererCandidateEvent sealed {
  public:
    property RTCIceCandidate^ Candidate
    {
      RTCIceCandidate^  get(){ return _Candidate; }
      void  set(RTCIceCandidate^ value){ _Candidate = value; }
    }

  private:
    RTCIceCandidate^ _Candidate;
  };

  public delegate void RTCIceGathererLocalCandidateDelegate(RTCIceGathererCandidateEvent^ evt);
  public delegate void RTCIceGathererLocalCandidateGoneDelegate(RTCIceGathererCandidateEvent^ evt);

  // Candidate complete event and delegate

  public ref class RTCIceGathererCandidateCompleteEvent sealed {
  public:
    property RTCIceCandidateComplete^ Completed;
  };

  public delegate void RTCIceGathererCandidateCompleteDelegate(RTCIceGathererCandidateCompleteEvent^ evt);

  // Error event and delegate
  public ref class RTCIceGathererErrorEvent sealed {
  public:
    property RTCIceGathererError^ Error
    {
      RTCIceGathererError^  get(){ return _Error; }
      void  set(RTCIceGathererError^ value){ _Error = value; }
    }

  private:
    RTCIceGathererError^ _Error;
  };

  public delegate void RTCIceGathererErrorDelegate(RTCIceGathererErrorEvent^ evt);

  //------------------------------------------
  // End Events and Delegates
  //------------------------------------------

	public ref class RTCIceGatherer sealed
	{
		friend class RTCIceGathererDelegate;
		friend class FetchNativePointer;
    friend class ConvertObjectToCx;
  private:
    RTCIceGatherer();
	public:
		RTCIceGatherer(RTCIceGatherOptions^ options);
		
		RTCIceParameters^ GetLocalParameters();
		IVector<RTCIceCandidate^>^ GetLocalCandidates();
		RTCIceGatherer^ CreateAssociatedGatherer();

		void Close();

	public:

		property RTCIceComponent Component
		{
			RTCIceComponent get() 
			{ 
				if (_NativePointer)
					return (RTCIceComponent)_NativePointer->component();
  			return RTCIceComponent::Rtp;
			}
		}

		property RTCIceGathererState State
		{
      RTCIceGathererState get();
		}

		event RTCIceGathererStateChangedDelegate^       OnICEGathererStateChanged
    {
      Windows::Foundation::EventRegistrationToken add(RTCIceGathererStateChangedDelegate^ handler)
      {
        RaiseBufferedOnICEGathererStateChangedEvents(handler);
        return _InternalOnICEGathererStateChanged += handler;
      }

      void remove(Windows::Foundation::EventRegistrationToken token)
      {
        _InternalOnICEGathererStateChanged -= token;
      }
    }

    event RTCIceGathererLocalCandidateDelegate^     OnICEGathererLocalCandidate
    {
      Windows::Foundation::EventRegistrationToken add(RTCIceGathererLocalCandidateDelegate^ handler)
      {
        RaiseBufferedOnICEGathererLocalCandidateEvents(handler);
        return _InternalOnICEGathererLocalCandidate += handler;
      }

      void remove(Windows::Foundation::EventRegistrationToken token)
      {
        _InternalOnICEGathererLocalCandidate -= token;
      }
    }

    event RTCIceGathererCandidateCompleteDelegate^  OnICEGathererCandidateComplete
    {
      Windows::Foundation::EventRegistrationToken add(RTCIceGathererCandidateCompleteDelegate^ handler)
      {
        return RaiseBufferedOnICEGathererCandidateCompleteEvents(handler);
      }

      void remove(Windows::Foundation::EventRegistrationToken token)
      {
        _InternalOnICEGathererCandidateComplete -= token;
      }
    }

    event RTCIceGathererLocalCandidateGoneDelegate^ OnICEGathererLocalCandidateGone
    {
      Windows::Foundation::EventRegistrationToken add(RTCIceGathererLocalCandidateGoneDelegate^ handler)
      {
        return RaiseBufferedOnICEGathererLocalCandidateGoneEvents(handler);
      }

      void remove(Windows::Foundation::EventRegistrationToken token)
      {
        _InternalOnICEGathererLocalCandidateGone -= token;
      }
    }

    event RTCIceGathererErrorDelegate^              OnICEGathererError
    {
      Windows::Foundation::EventRegistrationToken add(RTCIceGathererErrorDelegate^ handler)
      {
        return RaiseBufferedOnICEGathererErrorEvents(handler);
      }

      void remove(Windows::Foundation::EventRegistrationToken token)
      {
        _InternalOnICEGathererError -= token;
      }
    }

  public:
    [Windows::Foundation::Metadata::DefaultOverloadAttribute]
    static Platform::String^ ToString();
    [Windows::Foundation::Metadata::OverloadAttribute("IceGatherPolicyToString")]
    static Platform::String^ ToString(RTCIceGatherPolicy value);
    [Windows::Foundation::Metadata::OverloadAttribute("IceGathererStateToString")]
    static Platform::String^ ToString(RTCIceGathererState value);
    [Windows::Foundation::Metadata::OverloadAttribute("IceGathererCredentialTypeToString")]
    static Platform::String^ ToString(RTCIceGathererCredentialType value);

    static RTCIceGatherPolicy ToPolicy(Platform::String^ str);
    static RTCIceGathererState ToState(Platform::String^ str);
    static RTCIceGathererCredentialType ToCredentialType(Platform::String^ str);

  private:
    Windows::Foundation::EventRegistrationToken RaiseBufferedOnICEGathererStateChangedEvents(RTCIceGathererStateChangedDelegate^ handler);
    Windows::Foundation::EventRegistrationToken RaiseBufferedOnICEGathererLocalCandidateEvents(RTCIceGathererLocalCandidateDelegate^ handler);
    Windows::Foundation::EventRegistrationToken RaiseBufferedOnICEGathererCandidateCompleteEvents(RTCIceGathererCandidateCompleteDelegate^ handler);
    Windows::Foundation::EventRegistrationToken RaiseBufferedOnICEGathererLocalCandidateGoneEvents(RTCIceGathererLocalCandidateGoneDelegate^ handler);
    Windows::Foundation::EventRegistrationToken RaiseBufferedOnICEGathererErrorEvents(RTCIceGathererErrorDelegate^ handler);

    void RaiseOnICEGathererStateChangedEvent(RTCIceGathererStateChangeEvent^ evt);
    void RaiseOnICEGathererLocalCandidateEvent(RTCIceGathererCandidateEvent^ evt);
    void RaiseOnICEGathererCandidateCompleteEvent(RTCIceGathererCandidateCompleteEvent^ evt);
    void RaiseOnICEGathererLocalCandidateGoneEvent(RTCIceGathererCandidateEvent^ evt);
    void RaiseOnICEGathererErrorEvent(RTCIceGathererErrorEvent^ evt);

    event RTCIceGathererStateChangedDelegate^       _InternalOnICEGathererStateChanged;
    event RTCIceGathererLocalCandidateDelegate^     _InternalOnICEGathererLocalCandidate;
    event RTCIceGathererCandidateCompleteDelegate^  _InternalOnICEGathererCandidateComplete;
    event RTCIceGathererLocalCandidateGoneDelegate^ _InternalOnICEGathererLocalCandidateGone;
    event RTCIceGathererErrorDelegate^              _InternalOnICEGathererError;

    zsLib::RecursiveLock _Lock;
    IICEGathererPtr _NativePointer;
    RTCIceGathererDelegatePtr _NativeDelegatePointer;

    Vector<RTCIceGathererStateChangeEvent^>^        _RaisedOnICEGathererStateChangedEvents;
    Vector<RTCIceGathererCandidateEvent^>^          _RaisedOnICEGathererLocalCandidateEvents;
    Vector<RTCIceGathererCandidateCompleteEvent^>^  _RaisedOnICEGathererCandidateCompleteEvents;
    Vector<RTCIceGathererCandidateEvent^>^          _RaisedOnICEGathererLocalCandidateGoneEvents;
    Vector<RTCIceGathererErrorEvent^>^              _RaisedOnICEGathererErrorEvents;
  };
}
