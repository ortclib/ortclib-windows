using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;

namespace OrtcWrapper
{
    public class MediaDevice
    {
        public MediaDevice(string id, string name)
        {
            Id = id;
            Name = name;
        }

        public string Id { get; set; }
        public string Name { get; set; }

        public IAsyncOperation<IList<CaptureCapability>> GetVideoCaptureCapabilities() //async
        {
            return null;
        }
    }
}
