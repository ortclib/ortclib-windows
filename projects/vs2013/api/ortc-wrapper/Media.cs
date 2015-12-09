using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Media.Core;

namespace OrtcWrapper
{

    public class Media
    {
        public delegate void OnMediaCaptureDeviceFoundDelegate(MediaDevice __param0);
        public event OnMediaCaptureDeviceFoundDelegate OnAudioCaptureDeviceFound;
        public event OnMediaCaptureDeviceFoundDelegate OnVideoCaptureDeviceFound;

        static public Media CreateMedia()
        {
            return null;
        }
        static public IAsyncOperation<Media> CreateMediaAsync() //async
        {
            return null;
        }

        public IAsyncOperation<MediaStream> GetUserMedia(RTCMediaStreamConstraints mediaStreamConstraints) //async
        {
            return null; 
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
