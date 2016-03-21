using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class CaptureCapability
    {
        public uint Width { get; set; }
        public uint Height { get; set; }
        public uint FrameRate { get; set; }
        public String FullDescription { get; set; }
        public String ResolutionDescription { get; set; }
        public String FrameRateDescription { get; set; }
        //public Windows.Media.MediaProperties.MediaRatio PixelAspectRatio;
    }
}
