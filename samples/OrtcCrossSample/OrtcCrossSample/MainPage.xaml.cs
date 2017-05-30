using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;
using Org.Ortc;
using Org.Ortc.Adapter;
using System.Collections.ObjectModel;

namespace OrtcCrossSample
{
	public partial class MainPage : ContentPage
	{
        public static ObservableCollection<MediaDeviceInfo> Devices = new ObservableCollection<MediaDeviceInfo>();

        public MainPage()
		{
            InitializeComponent();
		}

	    private void EnumerateDevices()
	    {
	        try
	        {
	            var asyncOp = MediaDevices.EnumerateDevices();

	            asyncOp.ContinueWith(op =>
	            {
	                try
	                {
	                    var devices = op.Result;
	                    foreach (var deviceInfo in devices)
	                    {
	                        Devices.Add(deviceInfo);
                        }
	                }
	                catch (Exception e)
	                {
	                    //string str = "Obtaining results of the enumeration has failed.";
	                    DisplayAlert("Error", "Obtaining results of the enumeration has failed.", "OK");
                    }
                });
	        }
	        catch (Exception e)
	        {
	            //string str = "Device enumeration has failed.";
	            DisplayAlert("Error", "Device enumeration has failed.", "OK");
            }
	        
	    }

        private void Button_Click_EnumerateDevices(object sender, EventArgs e)
        {
            DisplayAlert("Alert", "Enumeration started!", "OK");
            Devices.Clear();
            this.EnumerateDevices();
        }
    }

    

}
