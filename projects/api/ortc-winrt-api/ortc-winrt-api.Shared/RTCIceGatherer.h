#pragma once

#include <ortc/IICEGatherer.h>
#include <collection.h>
#include "RTCIceTypes.h"

using namespace ortc;

using Windows::Foundation::Collections::IVector;


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
    property IVector<Platform::String^>^      URLs;
    property Platform::String^                UserName;
    property Platform::String^                Credential;
    property RTCIceGathererCredentialType     CredentialType;
  };

  public ref class RTCIceGatherOptions sealed
  {
  public:
    property RTCIceGatherPolicy      GatherPolicy;
    property IVector<RTCIceServer^>^ IceServers;
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
      RTCIceGathererState  get(){ return m_state; }
      void  set(RTCIceGathererState value){ m_state = value; }
    }

  private:
    RTCIceGathererState m_state;
  };

  public delegate void RTCIceGathererStateChangedDelegate(RTCIceGathererStateChangeEvent^ evt);


  // Candidate event and delegate
  public ref class RTCIceGathererCandidateEvent sealed {
  public:
    property RTCIceCandidate^ Candidate
    {
      RTCIceCandidate^  get(){ return m_candidate; }
      void  set(RTCIceCandidate^ value){ m_candidate = value; }
    }

  private:
    RTCIceCandidate^ m_candidate;
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
      RTCIceGathererError^  get(){ return m_error; }
      void  set(RTCIceGathererError^ value){ m_error = value; }
    }

  private:
    RTCIceGathererError^ m_error;
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

	private:
		IICEGathererPtr mNativePointer;
		RTCIceGathererDelegatePtr mNativeDelegatePointer;

	public:

		property RTCIceComponent Component
		{
			RTCIceComponent get() 
			{ 
				if (mNativePointer)
					return (RTCIceComponent)mNativePointer->component();
				else
					return RTCIceComponent::Rtp;
			}
		}

		property RTCIceGathererState State
		{
      RTCIceGathererState get();
		}

		event RTCIceGathererStateChangedDelegate^       OnICEGathererStateChanged;
		event RTCIceGathererLocalCandidateDelegate^     OnICEGathererLocalCandidate;
		event RTCIceGathererCandidateCompleteDelegate^  OnICEGathererCandidateComplete;
		event RTCIceGathererLocalCandidateGoneDelegate^ OnICEGathererLocalCandidateGone;
		event RTCIceGathererErrorDelegate^              OnICEGathererError;

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
	};
}
