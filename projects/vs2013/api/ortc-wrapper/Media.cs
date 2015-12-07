using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class Media
    {
        static public Media CreateMedia()
        {
            return null;
        }
        static Task<Media> CreateMediaAsync() //async
        {
            return null;
        }

        public Task<MediaStream> GetUserMedia(RTCMediaStreamConstraints mediaStreamConstraints) //async
        {
            return null; 
        }
    }
}
