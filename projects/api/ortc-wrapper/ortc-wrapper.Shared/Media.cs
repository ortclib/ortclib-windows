using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Media.Core;
using org.ortc;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace OrtcWrapper
{
    public class Media
    {
        IList<MediaDeviceInfo> _audioCaptureDevices = new List<MediaDeviceInfo>();
        IList<MediaDeviceInfo> _audioPlaybackDevices = new List<MediaDeviceInfo>();
        IList<MediaDeviceInfo> _videoDevices = new List<MediaDeviceInfo>();

        private MediaDevice _audioCaptureDevice;
        private MediaDevice _audioPlaybackDevice;
        private MediaDevice _videoDevice;

        public delegate void OnMediaCaptureDeviceFoundDelegate(MediaDevice __param0);
        public event OnMediaCaptureDeviceFoundDelegate OnAudioCaptureDeviceFound;
        public event OnMediaCaptureDeviceFoundDelegate OnVideoCaptureDeviceFound;

        static public Media CreateMedia()
        {
            var ret = new Media();

            return ret;
        }
        static public IAsyncOperation<Media> CreateMediaAsync()
        {
            return Task.Run<Media>(() =>
            {
                return CreateMedia();
             }).AsAsyncOperation<Media>();
        }

        static public IAsyncOperation<MediaDeviceInfo> EnumerateDevices()
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
                var constraints = Helper.MakeConstraints(mediaStreamConstraints.audioEnabled, null, MediaDeviceKind.AudioInput, _audioCaptureDevice);
                constraints = Helper.MakeConstraints(mediaStreamConstraints.videoEnabled, constraints, MediaDeviceKind.Video, _videoDevice);

                Task<IList<MediaStreamTrack>> task = MediaDevices.GetUserMedia(constraints).AsTask();
                return task.ContinueWith<MediaStream>((temp) =>
                {
                    var audioTracks = Helper.InsertAudioIfValid(mediaStreamConstraints.audioEnabled, null, temp.Result, _audioCaptureDevice);
                    var videoTracks = Helper.InsertVideoIfValid(mediaStreamConstraints.videoEnabled, null, temp.Result, _videoDevice);
                    return new MediaStream(audioTracks, videoTracks);
                });
            });

            return t.AsAsyncOperation<MediaStream>();
        }


        public IMediaSource CreateMediaStreamSource(MediaVideoTrack track, uint framerate, string id)
        {
            var useTrack = track.Track;
            if (null == useTrack) return null;

            return useTrack.CreateMediaSource();
        }
        public IAsyncOperation<bool> EnumerateAudioVideoCaptureDevices()
        {
            return Task.Run<bool>(async () =>
            {
                var devices = await MediaDevices.EnumerateDevices();

                var audioCaptureList = Helper.Filter(MediaDeviceKind.AudioInput, devices);
                //var audioPlaybackList = Helper.Filter(MediaDeviceKind.AudioOutput, devices);
                var videoList = Helper.Filter(MediaDeviceKind.Video, devices);

                _audioCaptureDevices = audioCaptureList;
                _videoDevices = videoList;

                await Task.Run(() =>
                {
                    foreach (var info in audioCaptureList)
                    {
                        OnAudioCaptureDeviceFound(new MediaDevice(info.DeviceId, info.Label));
                    }
                    foreach (var info in videoList)
                    {
                        OnVideoCaptureDeviceFound(new MediaDevice(info.DeviceId, info.Label));
                    }
                });

                return devices.Count > 0;
            }).AsAsyncOperation();
        }
        //public IList<MediaDevice> GetAudioCaptureDevices();
        //public IAsyncOperation<MediaStream> GetUserMedia(RTCMediaStreamConstraints mediaStreamConstraints);
        //public IList<MediaDevice> GetVideoCaptureDevices();
        static public void OnAppSuspending()
        {

        }
        public void SelectAudioDevice(MediaDevice device)
        {
            _audioCaptureDevice = device;
        }
        public void SelectVideoDevice(MediaDevice device)
        {
            _videoDevice = device;
        }
        //public void SetDisplayOrientation(DisplayOrientations display_orientation);
        public bool SelectAudioPlayoutDevice(MediaDevice device)
        {
            _audioPlaybackDevice = device;
            return true;
        }

        public IList<MediaDevice> GetAudioPlayoutDevices()
        {
            var contentAsync = MediaDevices.EnumerateDevices();
            contentAsync.AsTask().Wait();
            var devices = contentAsync.GetResults();

            var audioPlaybackList = Helper.Filter(MediaDeviceKind.AudioOutput, devices);
            
            _audioPlaybackDevices = audioPlaybackList;

            return Helper.ToMediaDevices(audioPlaybackList);
        }
    }
}
