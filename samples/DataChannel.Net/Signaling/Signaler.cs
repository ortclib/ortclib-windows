using System;
using System.Threading.Tasks;

namespace DataChannel.Net.Signaling
{
    public abstract class Signaler
    {
        // Connection events
        public event EventHandler SignedIn;
        public event EventHandler Disconnected;
        public event EventHandler ServerConnectionFailed;

        public event EventHandler<Peer> PeerConnected;
        public event EventHandler<Peer> PeerDisconnected;
        public event EventHandler<Peer> PeerHangup;
        public event EventHandler<Peer> MessageFromPeer;

        public abstract void SendToPeer(int id, string message);

        protected void OnSignedIn()
        {
            if (SignedIn != null)
                SignedIn(this, null);
        }
        protected void OnDisconnected()
        {
            if (Disconnected != null)
                Disconnected(this, null);
        }
        protected void OnPeerConnected(Peer peer)
        {
            if (PeerConnected != null)
                PeerConnected(this, (peer));
        }
        protected void OnPeerDisconnected(Peer peer)
        {
            if (PeerDisconnected != null)
                PeerDisconnected(this, peer);
        }
        protected void OnPeerHangup(Peer peer)
        {
            if (PeerHangup != null)
                PeerHangup(this, peer);
        }
        protected void OnMessageFromPeer(Peer peer)
        {
            if (MessageFromPeer != null)
                MessageFromPeer(this, peer);
        }
        protected void OnServerConnectionFailure()
        {
            if (ServerConnectionFailed != null)
                ServerConnectionFailed(this, null);
        }
    }
}
