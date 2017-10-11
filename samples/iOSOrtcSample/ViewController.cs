using System;
using System.Collections.Generic;
using UIKit;
using Org.Ortc;
using Org.Ortc.Adapter;

namespace iOSOrtcSample
{
    public partial class ViewController : UIViewController
    {
        public ViewController(IntPtr handle) : base(handle)
        {
        }

        public override void ViewDidLoad()
        {
            base.ViewDidLoad();
            // Perform any additional setup after loading the view, typically from a nib.
            this.enumerateDevs();
        }

        public override void DidReceiveMemoryWarning()
        {
            base.DidReceiveMemoryWarning();
            // Release any cached data, images, etc that aren't in use.
        }

        private async void enumerateDevs()
        {
            IReadOnlyList<MediaDeviceInfo> devices = await MediaDevices.EnumerateDevices();

            if (devices.Count > 0)
            {

            }
        }
    }
}