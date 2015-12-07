using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public enum RTCBundlePolicy
    {
        Balanced = 0,
        MaxBundle = 1,
        MaxCompat = 2
    }
    public enum RTCIceTransportPolicy
    {
        None = 0,
        Relay = 1,
        NoHost = 2,
        All = 3
    }
    public class RTCConfiguration
    {
        public IList<RTCIceServer> IceServers;
        public RTCIceTransportPolicy? IceTransportPolicy;
        public RTCBundlePolicy? BundlePolicy;
    }
}
