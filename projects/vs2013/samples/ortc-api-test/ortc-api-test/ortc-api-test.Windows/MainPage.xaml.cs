using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

using ortc_winrt_api;

namespace ortc_api_test
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
          RTCIceGatherOptions options = new RTCIceGatherOptions();
          
          _iceGatherer = new RTCIceGatherer(options);

          _iceGatherer.OnICEGathererStateChanged += this.RTCIceGatherer_onICEGathererStateChanged;
          _iceGatherer.OnICEGathererLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate;
          _iceGatherer.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
          _iceGatherer.OnICEGathererLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone;
          _iceGatherer.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;
        }

        private void RTCIceGatherer_onICEGathererStateChanged(RTCIceGathererStateChangeEvent evt)
        {
          int i = 0;
          i++;
        }

        private void RTCIceGatherer_onICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
        {
          int i = 0;
          i++;
        }

        private void RTCIceGatherer_onICEGathererCandidateComplete(RTCIceGathererCandidateCompleteEvent evt)
        {
          int i = 0;
          i++;
        }

        private void RTCIceGatherer_onICEGathererLocalCandidateGone(RTCIceGathererCandidateEvent evt)
        {
          int i = 0;
          i++;
        }

        private void RTCIceGatherer_onICEGathererError(RTCIceGathererErrorEvent evt)
        {
          int i = 0;
          i++;
        }

        RTCIceGatherer _iceGatherer;
    }
}
