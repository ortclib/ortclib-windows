namespace DataChannel.Net
{
    public class Message
    {
        public string Time { get; set; }
        public string Text { get; set; }

        public Message() { }

        public Message(string date, string text)
        {
            Time = date;
            Text = text;
        }

        public override string ToString()
        {
            return Time + ":  " + Text;
        }
    }
}
