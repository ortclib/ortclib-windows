﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrtcWrapper
{
    public class ResolutionHelper
    {
        public delegate void ResolutionChangedEventHandler(string id, uint width, uint height);
        public static event ResolutionChangedEventHandler ResolutionChanged;
    }
}