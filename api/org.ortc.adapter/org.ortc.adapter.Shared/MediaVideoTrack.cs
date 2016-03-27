using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using org.ortc;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            public class MediaVideoTrack : IMediaStreamTrack
            {
                private string _id;
                private bool _enabled;
                internal org.ortc.MediaStreamTrack Track;

                internal MediaVideoTrack(org.ortc.MediaStreamTrack track)
                {
                    _id = track.DeviceId;
                    _enabled = track.Enabled;
                    Track = track;
                    Label = Guid.NewGuid().ToString();
                    SsrcId = (UInt32)Guid.NewGuid().GetHashCode();
                }

                public MediaVideoTrack(string id, bool enabled = true)
                {
                    _id = id;
                    _enabled = enabled;
                }

                public uint SsrcId { get; set; }

                public bool Enabled
                {
                    get { return _enabled; }

                    set { _enabled = value; }
                }

                public string Id
                {
                    get { return _id; }
                }

                public string Cname { get; set; }
                public string Label { get; set; }

                public string Kind
                {
                    get { return "video"; }
                }

                public void Stop()
                {
                    //throw new NotImplementedException();
                }
            }
        }
    }
}