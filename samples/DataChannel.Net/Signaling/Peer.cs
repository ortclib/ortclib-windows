namespace DataChannel.Net.Signaling
{
    public class Peer
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Message { get; set; }

        public Peer() { }

        public Peer(int id, string name)
        {
            Id = id;
            Name = name.Replace(":",";");
        }

        public Peer(int id, string name, string message)
        {
            Id = id;
            Name = name.Replace(":", ";");
            Message = message;
        }

        public override string ToString()
        {
            return Id + ":" + Name;
        }

        public static Peer CreateFromString(string peerAsString)
        {
            string[] separaingChars = { ":" };
            string[] words = peerAsString.Split(separaingChars, System.StringSplitOptions.RemoveEmptyEntries);

            return new Peer(System.Convert.ToInt32(words[0]), words[1]);
        }

    }
}
