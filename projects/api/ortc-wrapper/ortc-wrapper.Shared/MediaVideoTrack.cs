using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class MediaVideoTrack : IMediaStreamTrack
    {
        private string _id;
        private bool _enabled;
        public MediaVideoTrack(string id, bool enabled = true)
        {
            _id = id;
            _enabled = enabled;
        }
        public bool Enabled
        {
            get
            {
                return _enabled;
            }

            set
            {
                _enabled = value;
            }
        }

        public string Id
        {
            get
            {
                return _id;
            }
        }

        public string Kind
        {
            get
            {
                return "video";
            }
        }

        public void Stop()
        {
            //throw new NotImplementedException();
        }
    }
}
