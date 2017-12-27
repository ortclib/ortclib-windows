using DataChannel.Net.Signaling;
using System;
using System.Diagnostics;
using System.Net.NetworkInformation;
using System.Windows.Forms;

namespace DataChannel.Net
{
    public partial class Form2 : Form
    {
        public Peer p;
        public Form2(Peer peer)
        {
            p = peer;
            InitializeComponent();
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            Debug.WriteLine("Remote peer id:  " + p.Id + ", Remote peer name:  " + p.Name);
            lstMessages.Items.Add(new Message(DateTime.Now.ToString("h:mm"), GetLocalPeerName() + ":  " + txtMessage.Text));
            txtMessage.Text = "";
        }

        private string GetLocalPeerName()
        {
            string hostname = IPGlobalProperties.GetIPGlobalProperties().HostName;
            return (hostname != null ? hostname : "<unknown host>") + "-dual";
        }
    }
}
