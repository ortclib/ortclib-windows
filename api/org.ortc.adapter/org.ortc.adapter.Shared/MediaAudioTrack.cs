using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using org.ortc;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            public class MediaAudioTrack : IMediaStreamTrack
            {
                internal org.ortc.MediaStreamTrack Track { get; set; }

                internal MediaAudioTrack(org.ortc.MediaStreamTrack track)
                {
                    Id = track.DeviceId;
                    Enabled = track.Enabled;
                    Track = track;
                    Label = Guid.NewGuid().ToString();
                    SsrcId = (UInt32)Guid.NewGuid().GetHashCode();
                }

                public MediaAudioTrack(string id, bool enabled = true)
                {
                    Id = id;
                    Enabled = enabled;
                }

                public uint SsrcId { get; set; }
                public bool Enabled { get; set; }

                public string Id { get; }
                public string Cname { get; set; }
                public string Label { get; set; }

                public string Kind => "audio";

                public void Stop()
                {
                    Track.Stop();
                }

            }
        }
    }
}