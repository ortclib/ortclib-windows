using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class FrameCounterHelper
    {
        public delegate void FramesPerSecondChangedEventHandler(string id, string fps);

        public static event FramesPerSecondChangedEventHandler FramesPerSecondChanged;
    }
}
