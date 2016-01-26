using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OrtcWrapper
{
    public class CodecInfo
    {
        public CodecInfo(int id, int clockrate, string name)
        {
            Id = id;
            Clockrate = clockrate;
            Name = name;
        }

        public int Clockrate { get; set; }
        public int Id { get; set; }
        public string Name { get; set; }
    }
}
