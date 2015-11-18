#pragma once

#include <ortc/types.h>
#include <ortc/IICEGatherer.h>

using namespace ortc;

namespace ortc_winrt_api
{

  //public class IIceGathererDelegateWrapper
  //{
  //public:

  //  virtual void onICEGathererStateChanged(
  //    IICEGathererPtr gatherer,
  //    IICEGatherer::States state
  //    );

  //  //virtual void onICEGathererLocalCandidate(
  //  //  IICEGathererPtr gatherer,
  //  //  CandidatePtr candidate
  //  //  );

  //  //virtual void onICEGathererLocalCandidateComplete(
  //  //  IICEGathererPtr gatherer,
  //  //  CandidateCompletePtr candidate
  //  //  );

  //  //virtual void onICEGathererLocalCandidateGone(
  //  //  IICEGathererPtr gatherer,
  //  //  CandidatePtr candidate
  //  //  );

  //  //virtual void onICEGathererError(
  //  //  IICEGathererPtr gatherer,
  //  //  ErrorCode errorCode,
  //  //  String errorReason
  //  //  );

  //};
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

  public ref class RTCIceGatherOptions sealed
  {
    RTCIceGatherPolicy     gatherPolicy;
    std::vector<RTCIceServer^> iceServers;
  };

	public ref class RTCIceGatherer sealed
	{

    //friend class RTCIceGathererDelegateWrapper;
	public:
    RTCIceGatherer(RTCIceGatherOptions^ options);
    RTCIceGatherer^ createAssociatedGatherer();

  private:
    IICEGathererPtr* mNativePointer;
    IICEGathererDelegatePtr* mNativeDelegatePointer;

  public:
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
