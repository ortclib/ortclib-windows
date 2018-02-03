using DataChannel.Net.Signaling;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net.NetworkInformation;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DataChannel.Net
{
    public partial class PeersListForm : Form, IDisposable
    {
        private readonly HttpSignaler _httpSignaler;
        public HttpSignaler HttpSignaler => _httpSignaler;

        Dictionary<int, Tuple<OrtcSignaler, ChatForm> > _chatSessions = new Dictionary<int, Tuple<OrtcSignaler, ChatForm> >();

        static PeersListForm()
        {
        }

        public PeersListForm()
        {
            InitializeComponent();

            lstPeers.SelectedIndex = -1;

            var name = OrtcSignaler.LocalPeer.Name;
            Debug.WriteLine($"Connecting to server from local peer: {name}");

            _httpSignaler =
                new HttpSignaler("peercc-server.ortclib.org", 8888, name);

            _httpSignaler.SignedIn += Signaler_SignedIn;
            _httpSignaler.ServerConnectionFailed += Signaler_ServerConnectionFailed;
            _httpSignaler.PeerConnected += Signaler_PeerConnected;
            _httpSignaler.PeerDisconnected += Signaler_PeerDisconnected;
            _httpSignaler.MessageFromPeer += Signaler_MessageFromPeer;
        }

        private void DisposeChatForms()
        {
            bool multiConnections = (Properties.Settings.Default.MultipleConnections);
            foreach (var item in _chatSessions)
            {
                if (multiConnections)
                    item.Value.Item2.Hide();
                else
                    item.Value.Item2.DialogResult = DialogResult.Cancel;
                item.Value.Item1.Dispose();
                item.Value.Item2.Dispose();
            }
            _chatSessions.Clear();
        }

        private void Signaler_SignedIn(object sender, EventArgs e)
        {
            // The signaler will notify all events from the signaler
            // task thread. To prevent concurrency issues, ensure all
            // notifications from this thread are asynchronously
            // forwarded back to the GUI thread for further processing.
            this.BeginInvoke((Action)(() => { HandleSignedIn(sender, e); }));
        }

        private void HandleSignedIn(object sender, EventArgs e)
        {
            Debug.WriteLine("Peer signed in to server.");
        }

        private void Signaler_ServerConnectionFailed(object sender, EventArgs e)
        {
            // See method Signaler_SignedIn for concurrency comments.
            this.BeginInvoke((Action)(() => { HandleServerConnectionFailed(sender, e); }));
        }

        private void HandleServerConnectionFailed(object sender, EventArgs e)
        {
            Debug.WriteLine("Server connection failure.");
        }

        private void Signaler_PeerConnected(object sender, Peer peer)
        {
            // See method Signaler_SignedIn for concurrency comments.
            this.BeginInvoke((Action)(() => { HandlePeerConnected(sender, peer); }));
        }

        private void HandlePeerConnected(object sender, Peer peer)
        {
            Debug.WriteLine($"Peer connected {peer.Name} / {peer.Id}");

            var found = lstPeers.FindString(peer.ToString());
            if (ListBox.NoMatches != found)
            {
                Debug.WriteLine("Peer already found in list: " + peer.ToString());
                return;
            }

            if (OrtcSignaler.LocalPeer.Name == peer.Name) {
                Debug.WriteLine("Peer is our local peer: " + peer.ToString());
                return;
            }

            lstPeers.Items.Add(peer.ToString());
        }

        private void Signaler_PeerDisconnected(object sender, Peer peer)
        {
            // See method Signaler_SignedIn for concurrency comments.
            this.BeginInvoke((Action)(() => { HandlePeerDisconnected(sender, peer); }));
        }

        private void HandlePeerDisconnected(object sender, Peer peer)
        {
            Debug.WriteLine($"Peer disconnected {peer.Name} / {peer.Id}");

            var found = lstPeers.FindString(peer.ToString());
            if (ListBox.NoMatches != found) {
                Debug.WriteLine("Peer not found in list: " + peer.ToString());
                return;
            }

            lstPeers.Items.RemoveAt(found);
        }

        private void Signaler_MessageFromPeer(object sender, HttpSignalerMessageEvent @event)
        {
            var complete = new ManualResetEvent(false);
            
            // Exactly like the case of Signaler_SignedIn, this event is fired
            // from the signaler task thread and like the other events,
            // the message must be processed on the GUI thread for concurrency
            // reasons. Unlike the connect / disconnect notifications these
            // events must be processed exactly one at a time and the next
            // message from the server should be held back until the current
            // message is fully processed.
            var asyncResult = this.BeginInvoke((Action)(() =>
            {
                // Do not invoke a .Wait() on the task result of
                // HandleMessageFromPeer! While this might seem as a
                // reasonable solution to processing the entire event from the
                // GUI thread before processing the next message in the GUI
                // thread queue, a .Wait() would also block any and all
                // events targeted toward the GUI thread.
                //
                // This would actually create a deadlock in some cases. All
                // events from ORTC are fired at the GUI thread queue and the
                // creation of a RTCCertificate requires an event notification
                // to indicate when the RTCCertificate awaited task is
                // complete. Because .Wait() would block the GUI thread the
                // notification that the RTCCertificate is completed would 
                // never fire and the RTCCertificate generation task result
                // would never complete which the .Wait() is waiting upon
                // to complete.
                //
                // Bottom line, don't block the GUI thread using a .Wait()
                // on a task from the GUI thread - ever!
                HandleMessageFromPeer(sender, @event).ContinueWith((antecedent) =>
                {
                    Debug.WriteLine("Message from peer handled: " + @event.Message);
                    complete.Set();
                });
            }));

            // By waiting on the async result the signaler's task is blocked
            // from processing the next signaler message until the current
            // message is fully processed. The signaler thread is allowed to
            // be blocked because events are never fired on the signaler's
            // tasks dispatchers.
            //
            // While .BeginInvoke() does ensure that each message is processed
            // by the GUI thread in the order the message is sent to the GUI
            // thread, it does not ensure the message is entirely processed
            // before the next message directed to the GUI thread is processed.
            // The moment an async/awaitable task happens on the GUI thread
            // the next message in the GUI queue is allowed to be processed.
            // Tasks and related methods cause the GUI thread to become
            // re-entrant to processing more messages whenever an async
            // related routine is called.
            complete.WaitOne();
        }

        private async Task HandleMessageFromPeer(object sender, HttpSignalerMessageEvent @event)
        {
            var message = @event.Message;
            var peer = @event.Peer;

            if (message.StartsWith("OpenDataChannel"))
            {
                Debug.WriteLine("contains OpenDataChannel");
                SetupPeer(peer, false);
            }

            Tuple<OrtcSignaler, ChatForm> tuple;
            if (!_chatSessions.TryGetValue(peer.Id, out tuple))
            {
                Debug.WriteLine("[WARNING] No peer found to direct remote message: {peer.Id} / " + message);
                return;
            }

            await tuple.Item1.HandleMessageFromPeer(message);
        }

        private async Task SetupPeer(Peer remotePeer, bool isInitiator)
        {
            var found = lstPeers.FindString(remotePeer.ToString());
            if (ListBox.NoMatches != found)
            {
                remotePeer = Peer.CreateFromString(lstPeers.GetItemText(lstPeers.Items[found]));
            }

            Tuple<OrtcSignaler, ChatForm> tuple;
            if (_chatSessions.TryGetValue(remotePeer.Id, out tuple))
            {
                // already have a form created
                tuple.Item2.HandleRemotePeerDisonnected();
                tuple.Item2.BringToFront();
                tuple.Item1.Dispose();
                _chatSessions.Remove(remotePeer.Id);
            }
            else
            {
                if (!Properties.Settings.Default.MultipleConnections)
                {
                    if (_chatSessions.Count > 0)
                    {
                        // Clear out existing chat forms as only one chat form
                        // is allowed.
                        DisposeChatForms();
                    }
                }

                // no chat form created, spawn a new one
                tuple = new Tuple<OrtcSignaler, ChatForm>(null, new ChatForm(OrtcSignaler.LocalPeer, remotePeer));

                tuple.Item2.SendMessageToRemotePeer += ChatForm_SendMessageToRemotePeer;

                // Invoking .ShowDialog() would block the signaler's task
                // being waited upon. Show() block the current task
                // from continuing until the dialog is dismissed but does
                // not block other events from processing on the GUI
                // thread. Not blocking events is insufficient though. The
                // task is waited by the signaler to complete before the
                // next signaler message from the server is allowed to be
                // processed so a dialog cannot be spawned from within the
                // processing of a peer's message task.
                //
                // The solution though to the blocking of the signaler's
                // task when bringing up a dialog is rather simple: invoke
                // the .ShowDialog() method asynchronously on the GUI
                // thread and not from within the current signaler message
                // task. This allows the GUI to be displayed and events are
                // processed as normal including other signaler messages
                // from peers.
                this.BeginInvoke((Action)(() =>
                {
                    if (Properties.Settings.Default.MultipleConnections)
                    {
                        // show the form non model
                        tuple.Item2.Show();
                    }
                    else
                    {
                        // invoke this on the main thread without blocking the
                        // current thread from continuing
                        tuple.Item2.ShowDialog();
                    }
                }));
            }

            // create a new tuple and carry forward the chat form from the previous tuple
            tuple = new Tuple<OrtcSignaler, ChatForm>(new OrtcSignaler(remotePeer, isInitiator), tuple.Item2);
            _chatSessions.Add(remotePeer.Id, tuple);

            tuple.Item1.DataChannelConnected += OrtcSignaler_OnDataChannelConnected;
            tuple.Item1.DataChannelDisonnected += OrtcSignaler_OnDataChannelDisconnected;
            tuple.Item1.SignalMessageToPeer += OrtcSignaler_OnSignalMessageToPeer;
            tuple.Item1.DataChannelMessage += OrtcSignaler_OnDataChannelMessage;

            await tuple.Item1.SetupAsync();
        }

        private void OrtcSignaler_OnDataChannelConnected(object sender, EventArgs e)
        {
            OrtcSignaler signeler = (OrtcSignaler)sender;
            Debug.WriteLine("Remote peer connected: {signeler.RemotePeer.Id}");

            Tuple<OrtcSignaler, ChatForm> tuple;
            if (_chatSessions.TryGetValue(signeler.RemotePeer.Id, out tuple))
            {
                tuple.Item2.HandleRemotePeerConnected();
            }
        }

        private void OrtcSignaler_OnDataChannelDisconnected(object sender, EventArgs e)
        {
            OrtcSignaler signeler = (OrtcSignaler)sender;
            Debug.WriteLine("Remote peer disconnected: {signeler.RemotePeer.Id}");

            Tuple<OrtcSignaler, ChatForm> tuple;
            if (_chatSessions.TryGetValue(signeler.RemotePeer.Id, out tuple))
            {
                tuple.Item2.HandleRemotePeerDisonnected();
            }
        }

        private void OrtcSignaler_OnSignalMessageToPeer(object sender, string message)
        {
            OrtcSignaler signeler = (OrtcSignaler)sender;
            Debug.WriteLine("Send message to remote peer: {signeler.RemotePeer.Id}" + message);
            _httpSignaler.SendToPeer(signeler.RemotePeer.Id, message);
        }

        private void OrtcSignaler_OnDataChannelMessage(object sender, string message)
        {
            OrtcSignaler signeler = (OrtcSignaler)sender;
            Debug.WriteLine("Message from remote peer: {signeler.RemotePeer.Id}" + message);
            _httpSignaler.SendToPeer(signeler.RemotePeer.Id, message);

            Tuple<OrtcSignaler, ChatForm> tuple;
            if (_chatSessions.TryGetValue(signeler.RemotePeer.Id, out tuple))
            {
                tuple.Item2.HandleMessageFromPeer(message);
            }
        }

        private void ChatForm_SendMessageToRemotePeer(object sender, Message message)
        {
            ChatForm form = (ChatForm)sender;
            Debug.WriteLine("Message from remote peer: {message.Recipient.Id}" + message.Text);

            Tuple<OrtcSignaler, ChatForm> tuple;
            if (_chatSessions.TryGetValue(message.Recipient.Id, out tuple))
            {
                tuple.Item1.HandleSendMessageViaDataChannel(message.Text);
            }
        }

        private async void btnConnect_Click(object sender, EventArgs e)
        {
            Debug.WriteLine("Connects to server!");

            await HttpSignaler.Connect();

            btnConnect.Enabled = false;
            btnDisconnect.Enabled = true;
        }

        private async void btnDisconnect_Click(object sender, EventArgs e)
        {
            Debug.WriteLine("Disconnects from server!");

            await _httpSignaler.SignOut();
            lstPeers.Items.Clear();

            btnDisconnect.Enabled = false;
            btnConnect.Enabled = true;
        }

        private async void btnChat_Click(object sender, EventArgs e)
        {
            if (lstPeers.SelectedIndex == -1)
            {
                MessageBox.Show("Please select a peer!");
                return;

            }

            string text = lstPeers.GetItemText(lstPeers.SelectedItem);

            Peer remotePeer = Peer.CreateFromString(text);

            _httpSignaler.SendToPeer(remotePeer.Id, "OpenDataChannel");
            await SetupPeer(remotePeer, true);
        }
    }
}
