using DataChannel.Net.Signaling;
using System;

namespace DataChannel.Net
{
    public class Message
    {
        public Peer Author { get; set; }
        public Peer Recipient { get; set; }
        public Peer SendingPeer { get; set; }

        public DateTime Time { get; set; }
        public string Text { get; set; }

        public Message() { }

        public Message(Peer author, Peer recipient, DateTime date, string text)
        {
            Author = author;
            Recipient = recipient;
            Time = date;
            Text = text;
        }

        public override string ToString()
        {
            return "[" + Author.Name + "] " + Time.ToString("h:mm") + ":  " + Text;
        }
    }
}
