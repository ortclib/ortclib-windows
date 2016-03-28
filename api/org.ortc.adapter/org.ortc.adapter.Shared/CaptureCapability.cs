using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            public class CaptureCapability
            {
                public uint Width { get; set; }
                public uint Height { get; set; }
                public uint FrameRate { get; set; }
                public String FullDescription { get; set; }
                public String ResolutionDescription { get; set; }
                public String FrameRateDescription { get; set; }
                public MediaRatio PixelAspectRatio { get; set; }
            }
        }
    }
}
