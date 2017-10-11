using Org.Ortc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OrtcSample.Net
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.listView.Items.Clear();
            this.EnumerateDevices();
        }

        private async void EnumerateDevices()
        {
            try
            {
                IReadOnlyList<MediaDeviceInfo> devices = await MediaDevices.EnumerateDevices();

                if (devices.Count > 0)
                {
                    int i = 0;

                    foreach (var deviceInfo in devices)
                    {
                        i++;
                        string str = "Device " + i + ":" + deviceInfo.Label;
                        listView.Items.Add(str);
                    }
                }
            }
            catch (Exception e)
            {
                string str = "Device enumeration has failed.";
                listView.Items.Add(str);
            }
        }
    }
}
