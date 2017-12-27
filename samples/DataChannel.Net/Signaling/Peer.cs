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
            Name = name;
        }

        public Peer(int id, string name, string message)
        {
            Id = id;
            Name = name;
            Message = message;
        }

        public override string ToString()
        {
            return Id + ":" + Name;
        }
    }
}
