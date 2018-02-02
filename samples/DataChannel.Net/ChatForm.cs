using DataChannel.Net.Signaling;
using System;
using System.Diagnostics;
using System.Net.NetworkInformation;
using System.Windows.Forms;

namespace DataChannel.Net
{
    public partial class ChatForm : Form
    {
        public event EventHandler RemotePeerConnected;
        public event EventHandler RemotePeerDisconnected;
        public event EventHandler<Message> SendMessageToRemotePeer;

        private event EventHandler<Message> MessageFromRemotePeer;

        public void HandleRemotePeerConnected()
        {
            RemotePeerConnected?.Invoke(this, null);
        }

        public void HandleRemotePeerDisonnected()
        {
            RemotePeerDisconnected?.Invoke(this, null);
        }

        public void HandleMessageFromPeer(string message)
        {
            MessageFromRemotePeer?.Invoke(this, new Message(RemotePeer, LocalPeer, DateTime.Now, message));
        }

        private bool _isSendReady = false;
        public bool IsSendReady
        {
            get { return _isSendReady; }
            set { _isSendReady = value; }
        }

        public Peer LocalPeer { get; set; }
        public Peer RemotePeer { get; set; }

        public ChatForm(Peer localPeer, Peer remotePeer)
        {
            LocalPeer = localPeer;
            RemotePeer = remotePeer;

            InitializeComponent();

            btnSend.Enabled = false;

            this.RemotePeerConnected += Signaler_RemoteConnected;
            this.RemotePeerDisconnected += Signaler_RemoteDisconnected;
            this.MessageFromRemotePeer += Signaler_MessageFromRemotePeer;
        }

        private void Signaler_RemoteConnected(object sender, EventArgs e)
        {
            IsSendReady = true;
            btnSend.Enabled = true;
        }

        private void Signaler_RemoteDisconnected(object sender, EventArgs e)
        {
            IsSendReady = false;
            btnSend.Enabled = false;
        }

        private void Signaler_MessageFromRemotePeer(object sender, Message message)
        {
            lstMessages.Items.Add(message);
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (!IsSendReady)
            {
                MessageBox.Show("Please wait, connecting...");
                return;
            }

            if (txtMessage.Text != string.Empty)
            {
                var message = new Message(LocalPeer, RemotePeer, DateTime.Now, txtMessage.Text);
                lstMessages.Items.Add(message);
                OnSendMessageToRemotePeer(message);
            }

            txtMessage.Text = string.Empty;
        }

        private void OnSendMessageToRemotePeer(Message message)
        {
            SendMessageToRemotePeer?.Invoke(this, message);
        }

    }
}
