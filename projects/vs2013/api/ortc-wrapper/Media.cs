using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Media.Core;
using ortc_winrt_api;
using System.ComponentModel;

namespace OrtcWrapper
{
    public class Media
    {
        public delegate void OnMediaCaptureDeviceFoundDelegate(MediaDevice __param0);
        public event OnMediaCaptureDeviceFoundDelegate OnAudioCaptureDeviceFound;
        public event OnMediaCaptureDeviceFoundDelegate OnVideoCaptureDeviceFound;

        static public Media CreateMedia()
        {
            var ret = new Media();

            return ret;
        }
        static public IAsyncOperation<Media> CreateMediaAsync() //async
        {
            return null;
        }

        public IAsyncOperation<MediaStream> GetUserMedia(RTCMediaStreamConstraints mediaStreamConstraints) //async
        {

            Task<MediaStream> t = Task.Run<MediaStream>(() => 
            {
                IAsyncOperation<IList<MediaStreamTrack>> async = OrtcMediaDevices.getUserMedia(Helper.ToApiConstraints(mediaStreamConstraints));
                MediaStream stream = new MediaStream();

                if (mediaStreamConstraints.audioEnabled)
                {
                    MediaAudioTrack track = new MediaAudioTrack();
                    stream.AddAudioTrack(track);
                }

                if (mediaStreamConstraints.videoEnabled)
                {
                    MediaVideoTrack track = new MediaVideoTrack();
                    stream.AddVideoTrack(track);
                }

                var constraint = new Constraints();
                var tracks = OrtcMediaDevices.getUserMedia(constraint);

                return stream;
            });

            return t.AsAsyncOperation<MediaStream>();
        }




        public IMediaSource CreateMediaStreamSource(MediaVideoTrack track, uint framerate, string id)
        {
            return null;
        }
        public IAsyncOperation<bool> EnumerateAudioVideoCaptureDevices()
        {
            return null;
        }
        //public IList<MediaDevice> GetAudioCaptureDevices();
        //public IAsyncOperation<MediaStream> GetUserMedia(RTCMediaStreamConstraints mediaStreamConstraints);
        //public IList<MediaDevice> GetVideoCaptureDevices();
        public void OnAppSuspending()
        {

        }
        public void SelectAudioDevice(MediaDevice device)
        {

        }
        public void SelectVideoDevice(MediaDevice device)
        {

        }
        //public void SetDisplayOrientation(DisplayOrientations display_orientation);
    }
}
