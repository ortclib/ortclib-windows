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
        IList<IMediaStreamTrack> _mediaTracks;

        public string Id { get; set; }

        public MediaStream()
        {
            _audioTracks = new List<MediaAudioTrack>();
            _videoTracks = new List<MediaVideoTrack>();
            _mediaTracks = new List<IMediaStreamTrack>();
            Id = Guid.NewGuid().ToString();
        }

        public MediaStream(List<MediaAudioTrack> audioTracks, List<MediaVideoTrack> videoTracks)
        {
            Id = Guid.NewGuid().ToString();
            _audioTracks = audioTracks;
            _videoTracks = videoTracks;
            _mediaTracks = new List<IMediaStreamTrack>();
            if (null != audioTracks) { foreach (var track in audioTracks) { _mediaTracks.Add(track); } }
            if (null != videoTracks) { foreach (var track in videoTracks) { _mediaTracks.Add(track); } }
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
            return _mediaTracks;
        }

        public void RemoveTrack(IMediaStreamTrack track)
        {
            if (track != null)
            {
                _mediaTracks.Remove(track);
                if (track.Kind.Equals("audio"))
                    _audioTracks.Remove((MediaAudioTrack)track);
                else
                    _videoTracks.Remove((MediaVideoTrack)track);
            }
        }

        public void AddAudioTrack(MediaAudioTrack track)
        {
            if (track != null)
            {
                _audioTracks.Add(track);
                _mediaTracks.Add(track);
            }
        }

        public void AddVideoTrack(MediaVideoTrack track)
        {
            if (track != null)
            {
                _videoTracks.Add(track);
                _mediaTracks.Add(track);
            }
        }
    }
}
