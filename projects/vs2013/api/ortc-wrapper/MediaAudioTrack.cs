using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class MediaAudioTrack : IMediaStreamTrack
    {
        private string _id;
        private bool _enabled;
        public MediaAudioTrack(string id, bool enabled = true)
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
                return "audio";
            }
        }

        public void Stop()
        {
            //throw new NotImplementedException();
        }
    }
}
