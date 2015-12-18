using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class MediaStream
    {
        IList<MediaAudioTrack> _audioTracks;
        IList<MediaVideoTrack> _videoTracks;

        MediaStream()
        {
            _audioTracks = new List<MediaAudioTrack>();
            _videoTracks = new List<MediaVideoTrack>();
        }
        public IList<MediaAudioTrack> GetAudioTracks()
        {
            return _audioTracks;
        }

        public IList<MediaVideoTrack> GetVideoTracks()
        {
            return _videoTracks;
        }

        public IList<IMediaStreamTrack> GetTracks()
        {
            return null;
        }

        public void RemoveTrack(IMediaStreamTrack track)
        {

        }

        private void AddAudioTrack(MediaAudioTrack track)
        {
            if (track != null)
                _audioTracks.Add(track);
        }

        private void AddVideoTrack(MediaVideoTrack track)
        {
            if (track != null)
                _videoTracks.Add(track);
        }
    }
}
