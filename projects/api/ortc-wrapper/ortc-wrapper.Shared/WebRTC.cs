using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime;
using Windows.UI.Core;
using Windows.Storage;
using Windows.Foundation;
using Windows.Media.Capture;
using ortc_winrt_api;

namespace OrtcWrapper
{
    public enum LogLevel
    {
        LOGLVL_SENSITIVE = 0,
        LOGLVL_VERBOSE = 1,
        LOGLVL_INFO = 2,
        LOGLVL_WARNING = 3,
        LOGLVL_ERROR = 4
    }
    public class WebRTC
    {
        public static void DisableLogging()
        {

        }
        public static void EnableLogging(LogLevel level)
        {

        }
        public static IList<CodecInfo> GetAudioCodecs()
        {
            return null;
        }
        //public static double GetCPUUsage();
        //public static long GetMemUsage();
        public static IList<CodecInfo> GetVideoCodecs()
        {
            return null;
        }
        public static void Initialize(CoreDispatcher dispatcher)
        {
            /*OrtcMediaDevices.enumerateDevices().AsTask().ContinueWith<MediaDeviceInfo>((temp) =>
            {
                foreach (MediaDeviceInfo info in temp.Result)
                {
                    if (info.DeviceID != null)
                        System.Diagnostics.Debug.WriteLine("DeviceID: {0}", info.DeviceID);
                }
                return null;
            });*/

            Media.EnumerateDevices().AsTask().ContinueWith<MediaDeviceInfo>((temp) =>
            {
                /*foreach (MediaDeviceInfo info in temp.Result)
                {
                    if (info.DeviceID != null)
                        System.Diagnostics.Debug.WriteLine("DeviceID: {0}", info.DeviceID);
                }*/
                return null;
            });
            /*if (globals::isInitialized)
                return;

            g_windowDispatcher = dispatcher;

            // Create a worker thread
            globals::gThread.SetName("WinRTApiWorker", nullptr);
            globals::gThread.Start();
            globals::RunOnGlobalThread<void>([] {
                rtc::EnsureWinsockInit();
                rtc::InitializeSSL(globals::certificateVerifyCallBack);

                auto encoderFactory = new webrtc::H264WinRTEncoderFactory();
                auto decoderFactory = new webrtc::H264WinRTDecoderFactory();

                LOG(LS_INFO) << "Creating PeerConnectionFactory.";
                globals::gPeerConnectionFactory =
                    webrtc::CreatePeerConnectionFactory(encoderFactory, decoderFactory);

                webrtc::SetupEventTracer(&WebRTC::GetCategoryGroupEnabled,
                  &WebRTC::AddTraceEvent);
            });
            globals::isInitialized = true;*/
        }
        //public static bool IsTracing();
        public static string LogFileName()
        {
            return "";
        }
        public static StorageFolder LogFolder()
        {
            return null;
        }
        private static async Task<bool> RequestAccessForMediaCapturePrivate() //async
        {
            MediaCapture mediaAccessRequester = new MediaCapture();
            MediaCaptureInitializationSettings  mediaSettings = new MediaCaptureInitializationSettings();
            mediaSettings.AudioDeviceId = "";
            mediaSettings.VideoDeviceId = "";
            mediaSettings.StreamingCaptureMode = StreamingCaptureMode.AudioAndVideo;
            mediaSettings.PhotoCaptureSource = PhotoCaptureSource.VideoPreview;

            await mediaAccessRequester.InitializeAsync(mediaSettings);

            if (mediaAccessRequester.MediaCaptureSettings.VideoDeviceId != "" && mediaAccessRequester.MediaCaptureSettings.AudioDeviceId != "")
            {
                return true;
            }

            return false;
        }
        public static IAsyncOperation<bool> RequestAccessForMediaCapture()
        {
            return RequestAccessForMediaCapturePrivate().AsAsyncOperation();
        }
        //[Overload("SaveTrace2")]
        //public static bool SaveTrace(string filename);
        //[Overload("SaveTrace1")]
        public static bool SaveTrace(string host, int port)
        {
            return false;
        }
        public static void SetPreferredVideoCaptureFormat(int frame_width, int frame_height, int fps)
        {

        }
        public static void StartTracing()
        {

        }
        public static void StopTracing()
        {

        }
        public static void SynNTPTime(long current_ntp_time)
        {

        }
        public static void UpdateCPUUsage(double cpu_usage)
        {

        }
        public static void UpdateMemUsage(long mem_usage)
        {

        }
    }
}
