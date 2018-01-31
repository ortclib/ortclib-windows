using DataChannel.Net.Signaling;
using System;
using System.Diagnostics;
using System.Net.NetworkInformation;
using System.Windows.Forms;

namespace DataChannel.Net
{
    public partial class ChatForm : Form
    {
        public event EventHandler<Message> MessageAdded;

        public void OnMessageAdded(Message message)
        {
            if (MessageAdded != null)
                MessageAdded(this, (message));
        }

        public Peer p;
        public ChatForm(Peer peer)
        {
            p = peer;

            InitializeComponent();

            this.MessageAdded += Chat_MessageAdded;
        }

        private void Chat_MessageAdded(object sender, Message message)
        {
            lstMessages.Items.Add(message);
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (PeersListForm._IsSendEnabled != false)
            {
                if (txtMessage.Text != string.Empty)
                {
                    lstMessages.Items.Add(new Message(DateTime.Now.ToString("h:mm"), GetLocalPeerName() + ":  " + txtMessage.Text));
                    PeersListForm._dataChannel.Send(txtMessage.Text);
                }
                txtMessage.Text = string.Empty;
            }
            else
            {
                MessageBox.Show("Please wait, connecting...");
            }
        }

        private string GetLocalPeerName()
        {
            string hostname = IPGlobalProperties.GetIPGlobalProperties().HostName;
            return (hostname != null ? hostname : "<unknown host>") + "-dual";
        }
    }
}
