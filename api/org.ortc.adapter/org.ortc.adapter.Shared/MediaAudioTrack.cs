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
            public class MediaAudioTrack : IMediaStreamTrack, IDisposable
            {
                internal MediaStreamTrack Track { get; set; }

                internal MediaAudioTrack(MediaStreamTrack track)
                {
                    Track = track;
                    Label = Guid.NewGuid().ToString();
                    SsrcId = (uint)Guid.NewGuid().GetHashCode();
                }

                public uint SsrcId { get; set; }
                public bool Enabled
                {
                    get { return Track.Enabled; }
                    set { Track.Enabled = value; }
                }

                public string Id => Track.Id;
                public string Cname { get; set; }
                public string Label { get; set; }

                public string Kind => MediaStreamTrack.ToString(Track.Kind);

                public void Stop()
                {
                    Track.Stop();
                }

                public void Dispose()
                {
                    Track.Stop();
                }
            }
        }
    }
}