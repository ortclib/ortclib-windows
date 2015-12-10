using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class RTCIceCandidate
    {
        public String Candidate;
        public String SdpMid;
        public UInt16 SdpMLineIndex;

        public RTCIceCandidate(String candidate, String sdpMid, UInt16 sdpMLineIndex)
        {
            Candidate = candidate;
            SdpMid = sdpMid;
            SdpMLineIndex = sdpMLineIndex;
        }
    }
}
