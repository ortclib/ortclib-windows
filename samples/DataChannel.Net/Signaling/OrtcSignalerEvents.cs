using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataChannel.Net.Signaling
{
    public abstract class OrtcSignalerEvents
    {
        public event EventHandler DataChannelConnected;
        public event EventHandler DataChannelDisonnected;
        public event EventHandler<string> SignalMessageToPeer;
        public event EventHandler<string> DataChannelMessage;

        protected void OnDataChannelConnected()
        {
            DataChannelConnected?.Invoke(this, null);
        }

        protected void OnDataChannelDisonnected()
        {
            DataChannelDisonnected?.Invoke(this, null);
        }

        protected void OnSignalMessageToPeer(string message)
        {
            SignalMessageToPeer?.Invoke(this, message);
        }

        protected void OnDataChannelMessage(string message)
        {
            DataChannelMessage?.Invoke(this, message);
        }
    }
}


