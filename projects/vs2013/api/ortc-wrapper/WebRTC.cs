using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Core;
using Windows.Storage;
using Windows.Foundation;

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
        public static IAsyncOperation<bool> RequestAccessForMediaCapture() //async
        {
            return null;
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
