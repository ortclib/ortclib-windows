using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Media.Core;
using ortc_winrt_api;
using System.ComponentModel;
using System.Runtime.CompilerServices;

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

        static public IAsyncOperation<MediaDeviceInfo> EnumerateDevices() //async
        {
            Task<MediaDeviceInfo> t = Task.Run<MediaDeviceInfo>(() =>
            {
                Task<IList<MediaDeviceInfo>> task = MediaDevices.EnumerateDevices().AsTask();
                
                return task.ContinueWith<MediaDeviceInfo>((temp) =>
                {
                    MediaDeviceInfo test = temp.Result[0];

                    return test;
                });
            });

            return t.AsAsyncOperation<MediaDeviceInfo>();
        }

        public IAsyncOperation<MediaStream> GetUserMedia(RTCMediaStreamConstraints mediaStreamConstraints)
        {
            Task<MediaStream> t = Task.Run<MediaStream>(() =>
            {
                Task<IList<MediaStreamTrack>> task = MediaDevices.GetUserMedia(Helper.ToApiConstraints(mediaStreamConstraints)).AsTask();
                
                return task.ContinueWith<MediaStream>((temp) =>
                {
                    MediaStream stream = new MediaStream();
                    List<MediaStreamTrack> test = new List<MediaStreamTrack>(temp.Result);
                    
                    foreach (MediaStreamTrack track in test)
                    {
                        if (track.Kind == MediaStreamTrackKind.TrackKind_Audio)
                        {
                            MediaAudioTrack audio = new MediaAudioTrack(track.Id);
                            stream.AddAudioTrack(audio);
                        }
                        else if (track.Kind == MediaStreamTrackKind.TrackKind_Video)
                        {
                            MediaVideoTrack audio = new MediaVideoTrack(track.Id);
                            stream.AddVideoTrack(audio);
                        }
                    }
                    
                    return stream;
                });
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
        static public void OnAppSuspending()
        {

        }
        public void SelectAudioDevice(MediaDevice device)
        {

        }
        public void SelectVideoDevice(MediaDevice device)
        {

        }
        //public void SetDisplayOrientation(DisplayOrientations display_orientation);
        public bool SelectAudioPlayoutDevice(MediaDevice device)
        {
            return false;
        }

        public IList<MediaDevice> GetAudioPlayoutDevices()
        {
            return null;
        }
    }
}
