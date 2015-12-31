using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class RTCSessionDescription
    {
        public RTCSdpType? Type;
        public String Sdp;

        public RTCSessionDescription(RTCSdpType type, String sdp)
        {
            Type = type;
            Sdp = sdp;
        }
    }
}
