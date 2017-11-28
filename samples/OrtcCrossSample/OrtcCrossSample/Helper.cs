using System;
using System.Collections.Generic;
using System.Text;
using Org.Ortc;
using Org.Ortc.Adapter;

namespace OrtcCrossSample
{
    public class Helper
    {
        static public async void EnumerateDevs()
        {
            IReadOnlyList<MediaDeviceInfo> devices = await MediaDevices.EnumerateDevices();

            if (devices.Count > 0)
            {

            }
        }
    }
}
