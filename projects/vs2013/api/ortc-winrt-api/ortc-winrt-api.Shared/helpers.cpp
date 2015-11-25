#include "pch.h"

#include <openpeer/services/ILogger.h>
#include "webrtc/base/win32.h"
#include "helpers.h"

using namespace Platform;

using namespace zsLib;

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

namespace ortc_winrt_api
{

  std::string FromCx(Platform::String^ inObj) {
    return rtc::ToUtf8(inObj->Data());
  }

  Platform::String^ ToCx(std::string const& inObj) {
    return ref new Platform::String(rtc::ToUtf16(inObj).c_str());
  }

  IICETypes::Candidate FromCx(RTCIceCandidate^ candidate)
  {
    IICETypes::Candidate ret;

    ret.mCandidateType = (IICETypes::CandidateTypes)candidate->CandidateType;
    ret.mFoundation = FromCx(candidate->Foundation);
    ret.mInterfaceType = FromCx(candidate->InterfaceType);
    ret.mIP = FromCx(candidate->IP);
    ret.mPort = candidate->Port;
    ret.mPriority = candidate->Priority;
    ret.mProtocol = (IICETypes::Protocols)candidate->Protocol;
    ret.mRelatedAddress = FromCx(candidate->RelatedAddress);
    ret.mRelatedPort = candidate->RelatedPort;
    ret.mTCPType = (IICETypes::TCPCandidateTypes)candidate->TCPType;
    ret.mUnfreezePriority = candidate->UnfreezePriority;

    return ret;
  }

  RTCIceCandidate^ ToCx(ortc::IICETypes::CandidatePtr candidate)
  {
    RTCIceCandidate^ ret = ref new RTCIceCandidate();

    ret->CandidateType = (RTCIceCandidateType)candidate->mCandidateType;
    ret->Foundation = ToCx(candidate->mFoundation);
    ret->InterfaceType = ToCx(candidate->mInterfaceType);
    ret->IP = ToCx(candidate->mIP);
    ret->Port = candidate->mPort;
    ret->Priority = candidate->mPriority;
    ret->Protocol = (RTCIceProtocol)candidate->mProtocol;
    ret->RelatedAddress = ToCx(candidate->mRelatedAddress);
    ret->RelatedPort = candidate->mRelatedPort;
    ret->TCPType = (RTCIceTcpCandidateType)candidate->mTCPType;
    ret->UnfreezePriority = candidate->mUnfreezePriority;

    return ret;
  }
}