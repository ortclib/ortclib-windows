using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using org.ortc;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace ortc_device_enumerator.Windows.Universal
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            
        }

        private void EnumerateDevices()
        {
            try
            {
                var asyncOp = MediaDevices.EnumerateDevices();
                
                asyncOp.Completed = (op, state) =>
                {
                    try
                    {
                        var devices = op.GetResults();
                        int i = 0;
                        foreach (var deviceInfo in devices)
                        {
                            i++;
                            string str = "Device " + i + ":" + deviceInfo.Label;
                            listView.Items.Add(str);
                        }
                    }
                    catch (Exception e)
                    {
                        string str = "Obtaining results of the enumeration has failed.";
                        listView.Items.Add(str);
                    }
                };
            }
            catch (Exception e)
            {
                string str = "Device enumeration has failed.";
                listView.Items.Add(str);
            }
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            EnumerateDevices();
        }
    }
}
