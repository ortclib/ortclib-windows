using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            public class MediaStream
            {
                private SemaphoreSlim _lock = new SemaphoreSlim(1, 1);

                IList<MediaAudioTrack> _audioTracks;
                IList<MediaVideoTrack> _videoTracks;

                public string Id { get; set; }

                public MediaStream()
                {
                    _audioTracks = new List<MediaAudioTrack>();
                    _videoTracks = new List<MediaVideoTrack>();
                    Id = Guid.NewGuid().ToString();
                }

                public MediaStream(List<MediaAudioTrack> audioTracks, List<MediaVideoTrack> videoTracks)
                {
                    Id = Guid.NewGuid().ToString();
                    _audioTracks = audioTracks;
                    _videoTracks = videoTracks;
                }

                public IList<MediaAudioTrack> GetAudioTracks()
                {
                    IList<MediaAudioTrack> result = null;
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        result = _audioTracks;
                    }
                    return result;
                }

                public IList<MediaVideoTrack> GetVideoTracks()
                {
                    IList<MediaVideoTrack> result = null;
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        result = _videoTracks;
                    }
                    return result;
                }

                public IList<IMediaStreamTrack> GetTracks()
                {
                    List<IMediaStreamTrack> ret = new List<IMediaStreamTrack>();
                    using (var @lock = new AutoLock(_lock))
                    {
                        @lock.WaitAsync().Wait();
                        if (null != _audioTracks)
                            ret.AddRange(_audioTracks);
                        if (null != _videoTracks)
                            ret.AddRange(_videoTracks);
                    }
                    return ret;
                }

                public void RemoveTrack(IMediaStreamTrack track)
                {
                    if (track != null)
                    {
                        using (var @lock = new AutoLock(_lock))
                        {
                            @lock.WaitAsync().Wait();
                            if (track.Kind.Equals("audio"))
                                _audioTracks.Remove((MediaAudioTrack) track);
                            else
                                _videoTracks.Remove((MediaVideoTrack) track);
                        }
                    }
                }

                public void AddAudioTrack(MediaAudioTrack track)
                {
                    if (track != null)
                    {
                        using (var @lock = new AutoLock(_lock))
                        {
                            @lock.WaitAsync().Wait();
                            _audioTracks.Add(track);
                        }
                    }
                }

                public void AddVideoTrack(MediaVideoTrack track)
                {
                    if (track != null)
                    {
                        using (var @lock = new AutoLock(_lock))
                        {
                            @lock.WaitAsync().Wait();
                            _videoTracks.Add(track);
                        }
                    }
                }

                internal uint GetAudioTrackSsrc()
                {
                    if (_audioTracks.Count > 0)
                    {
                        MediaAudioTrack mat = _audioTracks[0];
                        return mat.SsrcId;
                    }
                    return 0;
                }

                internal uint GetVideoTrackSsrc()
                {
                    if (_videoTracks.Count > 0)
                    {
                        MediaVideoTrack mat = _videoTracks[0];
                        return mat.SsrcId;
                    }
                    return 0;
                }
            }
        }
    }
}