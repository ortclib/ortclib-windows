using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ortc_winrt_api;

namespace OrtcWrapper
{
    class Helper
    {
        public static RTCIceCandidate toWrapperIceCandidate(ortc_winrt_api.RTCIceCandidate iceCandidate)
        {
            StringBuilder sb = new StringBuilder();


            var ret = new RTCIceCandidate("","",0);

            return ret; 
        }
    }
}
