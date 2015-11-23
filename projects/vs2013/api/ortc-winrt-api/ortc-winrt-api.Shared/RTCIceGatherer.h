#pragma once

#include <ortc/types.h>
#include <ortc/IICEGatherer.h>

using namespace ortc;

namespace ortc_winrt_api
{

  public delegate void RTCIceGathererStateChangedDelegate();
    //RTCIceGatherer^, RTCIceGathererState);

  ZS_DECLARE_CLASS_PTR(RTCIceGathererDelegate)

  ref class RTCIceGatherer;

  class RTCIceGathererDelegate : public IICEGathererDelegate
  {
  public:
     virtual void onICEGathererStateChanged(
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
  public ref class RTCIceServer sealed
  {
    std::vector<Platform::String^> mURLs;
    Platform::String^     mUserName;
    Platform::String^     mCredential;
  };

  public enum class RTCIceGatherPolicy
  {
    IceGatherPolicy_All,
    IceGatherPolicy_Nohost,
    IceGatherPolicy_Relay
  };

  public enum class RTCIceGathererState {
    State_New,
    State_Gathering,
    State_Complete,
    State_Closed,
  };

  public ref class RTCIceGatherOptions sealed
  {
    RTCIceGatherPolicy     gatherPolicy;
    std::vector<RTCIceServer^> iceServers;
  };

	public ref class RTCIceGatherer sealed
	{
    friend class RTCIceGathererDelegate;
	public:
    RTCIceGatherer(RTCIceGatherOptions^ options);
    RTCIceGatherer^ createAssociatedGatherer();

  private:
    IICEGathererPtr mNativePointer;
    RTCIceGathererDelegatePtr mNativeDelegatePointer;

  public:

    event RTCIceGathererStateChangedDelegate^ OnICEGathererStateChanged;
    ///// <summary>
    ///// A remote peer has opened a data channel.
    ///// </summary>
    //event onICEGathererStateChanged^ onICEGathererStateChanged;

    ///// <summary>
    ///// New connection health stats are available.
    ///// </summary>
    //event RTCPeerConnectionHealthStatsDelegate^ OnConnectionHealthStats;

    ///// <summary>
    ///// Webrtc statistics report is ready <see cref="RTCStatsReports"/>.
    ///// </summary>
    //event RTCStatsReportsReadyEventDelegate^  OnRTCStatsReportsReady;
	};
}
