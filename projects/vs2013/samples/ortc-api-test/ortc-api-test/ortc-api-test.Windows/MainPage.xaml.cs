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
using Windows.Storage.Pickers;
using Windows.Storage;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

using ortc_winrt_api;
using System.Threading.Tasks;
using Windows.Storage.Streams;
using System.Threading;

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
            ConfigureOrtcEngine conf = new ConfigureOrtcEngine();
            this.btnStart.Visibility = Windows.UI.Xaml.Visibility.Visible;
            this.btnSendFile.Visibility = Windows.UI.Xaml.Visibility.Collapsed;
            this.tbLog.Text = "Please Start ORTC Engine !\n\n";

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
          this.tbLog.Text += "Initialization in progress, please wait...\n";
          this.tbLog.Text += "Starting ORTC Engine...\n";
          RTCIceGatherOptions options = new RTCIceGatherOptions();
          RTCIceServer server = new RTCIceServer();

          server.UserName = "Bojan";
          server.Credential = "12345";
          server.URLs = new List<String>();
          server.URLs.Add("stun:stun.vline.com");
          options.IceServers = new List<RTCIceServer>();
          options.IceServers.Add(server);

          this.tbLog.Text += "Creating IceGatherer 1 Object\n";
          _iceGatherer = new RTCIceGatherer(options);
          _iceGatherer.OnICEGathererStateChanged += this.RTCIceGatherer_onICEGathererStateChanged;
          _iceGatherer.OnICEGathererLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate;
          _iceGatherer.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
          _iceGatherer.OnICEGathererLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone;
          _iceGatherer.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;

          this.tbLog.Text += "Creating IceGatherer 2 Object\n";
          _iceGatherer2 = new RTCIceGatherer(options);
          _iceGatherer2.OnICEGathererStateChanged += this.RTCIceGatherer_onICEGathererStateChanged2;
          _iceGatherer2.OnICEGathererLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate2;
          _iceGatherer2.OnICEGathererCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete2;
          _iceGatherer2.OnICEGathererLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone2;
          _iceGatherer2.OnICEGathererError += this.RTCIceGatherer_onICEGathererError;

          this.tbLog.Text += "Creating IceTransport 1 Object\n";
          _iceTransport = new RTCIceTransport(_iceGatherer);
          _iceTransport.OnICETransportStateChanged += RTCIceTransport_onICETransportStateChanged;
          _iceTransport.OnICETransportCandidatePairAvailable += RTCIceTransport_onICETransportCandidatePairAvailable;
          _iceTransport.OnICETransportCandidatePairGone += RTCIceTransport_onICETransportCandidatePairGone;
          _iceTransport.OnICETransportCandidatePairChanged += RTCIceTransport_onICETransportCandidatePairChanged;

          this.tbLog.Text += "Creating IceTransport 2 Object\n";
          _iceTransport2 = new RTCIceTransport(_iceGatherer);
          _iceTransport2.OnICETransportStateChanged += RTCIceTransport_onICETransportStateChanged2;
          _iceTransport2.OnICETransportCandidatePairAvailable += RTCIceTransport_onICETransportCandidatePairAvailable2;
          _iceTransport2.OnICETransportCandidatePairGone += RTCIceTransport_onICETransportCandidatePairGone2;
          _iceTransport2.OnICETransportCandidatePairChanged += RTCIceTransport_onICETransportCandidatePairChanged2;

          this.tbLog.Text += "Generating certificate for DTLSTransport 1 Object \n";
          RTCCertificate.generateCertificate("").AsTask<RTCCertificate>().ContinueWith((cert) => {
            _dtlsTransport = new RTCDtlsTransport(_iceTransport, cert.Result);
            _dtlsTransport.OnDtlsTransportStateChanged += RTCDtlsTransport_onDTLSTransportStateChanged;
            _dtlsTransport.OnDtlsTransportError += RTCDtlsTransport_onDTLSTransportError;
            isReceiveDtlsConstructed = true;

            _sctpTransport = new RTCSctpTransport(_dtlsTransport, 0);
            _sctpTransport.OnSCTPTransportDataChannel += RTCSctpTransport_onSCTPTransportDataChannel;
            
            _dataChannelList = new List<RTCDataChannel>();
            
          });

          this.tbLog.Text += "Generating certificate for DTLSTransport 2 Object \n";
          RTCCertificate.generateCertificate("").AsTask<RTCCertificate>().ContinueWith((cert) =>
          {
            _dtlsTransport2 = new RTCDtlsTransport(_iceTransport2, cert.Result);
            _dtlsTransport2.OnDtlsTransportStateChanged += RTCDtlsTransport_onDTLSTransportStateChanged2;
            _dtlsTransport2.OnDtlsTransportError += RTCDtlsTransport_onDTLSTransportError2;
            isSendDtlsConstructed = true;
            _sctpTransport2 = new RTCSctpTransport(_dtlsTransport2, 0);
            _sctpTransport2.OnSCTPTransportDataChannel += RTCSctpTransport_onSCTPTransportDataChannel2;
            //_sctpTransport2.start(RTCSctpTransport.getCapabilities());

            _dataChannelList2 = new List<RTCDataChannel>();

            //dataChannel.send("fuck you");
            
          });

        }

        //----------------------------------------------------------------------------------
        // ICE GATHERER EVENT HANDLERS
        //----------------------------------------------------------------------------------

        private void RTCIceGatherer_onICEGathererStateChanged(RTCIceGathererStateChangeEvent evt)
        {
          if(evt.State == RTCIceGathererState.State_Complete)
          {
            this.tbLog.Text += "IceGatherer 1 Object state=Completed\n";
            this.tbLog.Text += "Starting IceTransport 1 Object\n";
            _iceTransport.start(_iceGatherer, _iceGatherer2.getLocalParameters(), RTCIceRole.Role_Controlled);
          }
        }

        private void RTCIceGatherer_onICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
        {
          this.tbLog.Text += "Local candidate from IceTransport 1 Object ready, sending it to IceTransport 2 Object\n";
          _iceTransport2.addRemoteCandidate(evt.Candidate);
        }

        private void RTCIceGatherer_onICEGathererCandidateComplete(RTCIceGathererCandidateCompleteEvent evt)
        {
        }

        private void RTCIceGatherer_onICEGathererLocalCandidateGone(RTCIceGathererCandidateEvent evt)
        {
        }

        private void RTCIceGatherer_onICEGathererError(RTCIceGathererErrorEvent evt)
        {
        }

      //***********************************************************************************************

        private void RTCIceGatherer_onICEGathererStateChanged2(RTCIceGathererStateChangeEvent evt)
        {
          if (evt.State == RTCIceGathererState.State_Complete)
          {
            this.tbLog.Text += "IceGatherer 2 Object state=Completed\n";
            this.tbLog.Text += "Starting IceTransport 2 Object\n";
            _iceTransport2.start(_iceGatherer2, _iceGatherer.getLocalParameters(), RTCIceRole.Role_Controlling);
          }
        }
        private void RTCIceGatherer_onICEGathererLocalCandidate2(RTCIceGathererCandidateEvent evt)
        {
          this.tbLog.Text += "Local candidate from IceTransport 2 Object ready, sending it to IceTransport 1 Object\n";
          _iceTransport.addRemoteCandidate(evt.Candidate);

        }

        private void RTCIceGatherer_onICEGathererCandidateComplete2(RTCIceGathererCandidateCompleteEvent evt)
        {
        }

        private void RTCIceGatherer_onICEGathererLocalCandidateGone2(RTCIceGathererCandidateEvent evt)
        {
        }

        private void RTCIceGatherer_onICEGathererError2(RTCIceGathererErrorEvent evt)
        {
        }

      //----------------------------------------------------------------------------------
      // ICE TRANSPORT EVENT HANDLERS
      //----------------------------------------------------------------------------------

        private void RTCIceTransport_onICETransportStateChanged(RTCIceTransportStateChangeEvent evt)
        {
          if (evt.State == RTCIceTransportState.State_Connected)
          {
            this.tbLog.Text += "IceTransport 1 Object state=Connected\n";
            this.tbLog.Text += "Starting DtlsTransport 1 Object\n";
            _dtlsTransport.start(_dtlsTransport2.getLocalParameters());
          }
        }

        private void RTCIceTransport_onICETransportCandidatePairAvailable(RTCIceTransportCandidatePairEvent evt)
        {
        }
        private void RTCIceTransport_onICETransportCandidatePairGone(RTCIceTransportCandidatePairEvent evt)
        {
        }
        private void RTCIceTransport_onICETransportCandidatePairChanged(RTCIceTransportCandidatePairEvent evt)
        {
        }

      //****************************************************************************************************

        private void RTCIceTransport_onICETransportStateChanged2(RTCIceTransportStateChangeEvent evt)
        {
          if (evt.State == RTCIceTransportState.State_Connected)
          {
            this.tbLog.Text += "IceTransport 2 Object state=Connected\n";
            this.tbLog.Text += "Starting DtlsTransport 2 Object\n";
            _dtlsTransport2.start(_dtlsTransport.getLocalParameters());
          }
        }

        private void RTCIceTransport_onICETransportCandidatePairAvailable2(RTCIceTransportCandidatePairEvent evt)
        {
        }
        private void RTCIceTransport_onICETransportCandidatePairGone2(RTCIceTransportCandidatePairEvent evt)
        {
        }
        private void RTCIceTransport_onICETransportCandidatePairChanged2(RTCIceTransportCandidatePairEvent evt)
        {
        }

        //----------------------------------------------------------------------------------
        // DTLS TRANSPORT EVENT HANDLERS
        //----------------------------------------------------------------------------------
        private void RTCDtlsTransport_onDTLSTransportStateChanged(RTCDtlsTransportStateChangeEvent evt)
        {
          if (evt.State == RTCDtlsTransportState.State_Validated)
          {
            this.tbLog.Text += "DtlsTransport 1 Object state=Validated\n";
            isReceiveDtlsReadyAndValidated = true;
            startSctp();
            if(isDtlsReady())
            {
              this.btnSendFile.Visibility = Windows.UI.Xaml.Visibility.Visible;
            }
          }
        }
        private void RTCDtlsTransport_onDTLSTransportError(RTCDtlsTransportErrorEvent evt)
        {
        }

        //******************************************************************************************************
        private void RTCDtlsTransport_onDTLSTransportStateChanged2(RTCDtlsTransportStateChangeEvent evt)
        {
          if (evt.State == RTCDtlsTransportState.State_Validated)
          {
            this.tbLog.Text += "DtlsTransport 2 Object state=Validated\n";
            isSendDtlsReadyAndValidated = true;
            startSctp();
            if (isDtlsReady())
            {
              this.btnSendFile.Visibility = Windows.UI.Xaml.Visibility.Visible;
            }
          }
        }
        private void RTCDtlsTransport_onDTLSTransportError2(RTCDtlsTransportErrorEvent evt)
        {
        }

        //----------------------------------------------------------------------------------
        // SCTP TRANSPORT EVENT HANDLERS
        //----------------------------------------------------------------------------------
        private void RTCSctpTransport_onSCTPTransportDataChannel(RTCDataChannelEvent evt)
        {
          this.tbLog.Text += "DataChannel Object received from remote side\n";
          _dataChannelList.Add(evt.DataChannel);
          sendFile(_dataChannelList.ElementAt(0));
        }

        //******************************************************************************************************
        private void RTCSctpTransport_onSCTPTransportDataChannel2(RTCDataChannelEvent evt)
        {
          this.tbLog.Text += "DataChannel Object received from remote side\n";
          _dataChannelList2.Add(evt.DataChannel);
        }

        //----------------------------------------------------------------------------------
        // DATA CHANNEL EVENT HANDLERS
        //----------------------------------------------------------------------------------
        private void RTCDataChannel_onDataChannelStateChanged(RTCDataChannelStateChangeEvent evt)
        {
          if (evt.State == RTCDataChannelState.State_Open)
          {
            this.tbLog.Text += "DataChannel Object 1 state = Open\n";
            isReceiveChannelOpen = true;
          }
        }
        private void RTCDataChannel_onDataChannelError(RTCDataChannelErrorEvent evt)
        {
        }

        private void RTCDataChannel_onDataChannelMessage(RTCMessageEventDataEvent evt)
        {
          this.tbLog.Text += "DataChannel Object 1 received packet\n";
          parseMessageData(evt.MessageData);
        }

        //******************************************************************************************************

        private void RTCDataChannel_onDataChannelStateChanged2(RTCDataChannelStateChangeEvent evt)
        {
          if (evt.State == RTCDataChannelState.State_Open)
          {
            this.tbLog.Text += "DataChannel Object 1 state = Open\n";
            isSendChannelOpen = true;
          }
        }
        private void RTCDataChannel_onDataChannelError2(RTCDataChannelErrorEvent evt)
        {
        }

        private void RTCDataChannel_onDataChannelMessage2(RTCMessageEventDataEvent evt)
        {
          this.tbLog.Text += "DataChannel Object 2 received packet\n";
          parseMessageData(evt.MessageData);
        }

        //******************************************************************************************************
        RTCIceGatherer _iceGatherer;
        RTCIceTransport _iceTransport;
        RTCDtlsTransport _dtlsTransport;
        RTCSctpTransport _sctpTransport;
        List<RTCDataChannel> _dataChannelList;

        RTCIceGatherer _iceGatherer2;
        RTCIceTransport _iceTransport2;
        RTCDtlsTransport _dtlsTransport2;
        RTCSctpTransport _sctpTransport2;
        List<RTCDataChannel> _dataChannelList2;

        String _filePath;
        String _sendFileName;
        String _receivedFileName;
        byte[] _sentFileBytes;

        int _expectedFileSize;
        int _bytesReceived = 0;
        int _receivedBinaryPackets = 0;
        int _sentBinaryPackets = 0;

        bool isSendDtlsConstructed = false;
        bool isReceiveDtlsConstructed = false;
        bool isDtlsStarted = false;

        bool isSendDtlsReadyAndValidated = false;
        bool isReceiveDtlsReadyAndValidated = false;

        bool isSendChannelOpen = true;
        bool isReceiveChannelOpen = true;

        private async void Button_Click_1(object sender, RoutedEventArgs e)
        {

          _filePath = string.Empty;
          FileOpenPicker filePicker = new Windows.Storage.Pickers.FileOpenPicker();
          filePicker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
          filePicker.ViewMode = PickerViewMode.Thumbnail;

          // Filter to include a sample subset of file types
          filePicker.FileTypeFilter.Clear();
          filePicker.FileTypeFilter.Add(".bmp");
          filePicker.FileTypeFilter.Add(".png");
          filePicker.FileTypeFilter.Add(".jpeg");
          filePicker.FileTypeFilter.Add(".jpg");

          StorageFile file = await filePicker.PickSingleFileAsync();
          if (file != null)
          {
            _sendFileName = file.DisplayName + file.FileType;

            _sentFileBytes = await ReadFile(file);
            RTCDataChannelParameters parameters = new RTCDataChannelParameters();
            parameters.Ordered = true;
            RTCDataChannel dataChannel = new RTCDataChannel(_sctpTransport2, parameters);
            dataChannel.OnDataChannelStateChanged += RTCDataChannel_onDataChannelStateChanged2;
            dataChannel.OnDataChannelError += RTCDataChannel_onDataChannelError2;
            dataChannel.OnDataChannelMessage += RTCDataChannel_onDataChannelMessage2;
            _dataChannelList2.Add(dataChannel);
          }
        }

        /// <summary>
        /// Loads the byte data from a StorageFile
        /// </summary>
        /// <param name="file">The file to read</param>
        public async Task<byte[]> ReadFile(StorageFile file)
        {
          byte[] fileBytes = null;
          using (IRandomAccessStreamWithContentType stream = await file.OpenReadAsync())
          {
            fileBytes = new byte[stream.Size];
            using (DataReader reader = new DataReader(stream))
            {
              await reader.LoadAsync((uint)stream.Size);
              reader.ReadBytes(fileBytes);
            }
          }

          return fileBytes;
        }

      private void sendFile(RTCDataChannel channel)
      {
        int incomingOffset = 0;
        int chunkSize = RTCSctpTransport.getCapabilities().MaxMessageSize;
        byte[] outboundBuffer = new byte[chunkSize];

        this.tbLog.Text += "Sending file=" + _sendFileName + ", file size = " + _sentFileBytes.Length.ToString() + "\n";
        try
        {
          channel.send(_sendFileName + "|" + _sentFileBytes.Length.ToString());

          while (incomingOffset < _sentFileBytes.Length)
          {
            int length =
                Math.Min(outboundBuffer.Length, _sentFileBytes.Length - incomingOffset);

            System.Buffer.BlockCopy(_sentFileBytes, incomingOffset,
                              outboundBuffer, 0,
                              length);

            incomingOffset += length;

            // Transmit outbound buffer
            channel.send(outboundBuffer, (ushort)length);
            _sentBinaryPackets++;
          }

          channel.send("eof");
        }
        finally
        {
        }
        
      }

      private async void parseMessageData(RTCMessageEventData data)
      {
        if (data.Text.Length > 0)
        {
          if (!data.Text.Equals("eof"))
          {
            String[] str = data.Text.Split('|');
            await createFile(str[0], Int32.Parse(str[1]));
            this.tbLog.Text += "Received file info: file=" + str[0] + ", file size = " + str[1] + "\n";
          }
          else if (data.Text.Equals("eof"))
          {
            this.tbLog.Text += "Received End of file, closing DataChannel objects\n";
            _dataChannelList.ElementAt(0).close();
            _dataChannelList.Clear();

            _dataChannelList2.ElementAt(0).close();
            _dataChannelList2.Clear();
            
          }
        }
        else if (data.Binary.Length > 0)
        {
          this.tbLog.Text += "Received binary package size = " + data.Binary.Length.ToString() + "\n";
          await saveBytes(data.Binary);
        }
      }

      private static readonly SemaphoreSlim _sl = new SemaphoreSlim(initialCount: 1);

      private async Task createFile(String filename, int sizeInBytes)
      {
        await _sl.WaitAsync();

        try
        {
          // Do async work here
          StorageFolder picturesFolder = KnownFolders.PicturesLibrary;
          StorageFile newFile = await picturesFolder.CreateFileAsync(filename, CreationCollisionOption.GenerateUniqueName);
          _receivedFileName = newFile.DisplayName + newFile.FileType;
          _expectedFileSize = sizeInBytes;

        }
        finally
        {
          // Release the thread
          _sl.Release();
        }
      }
      private async Task saveBytes(byte[] buffer)
      {
        await _sl.WaitAsync();
        try
        {
          // Do async work here
          StorageFolder picturesFolder = KnownFolders.PicturesLibrary;
          StorageFile file = await picturesFolder.GetFileAsync(_receivedFileName);

          using (BinaryWriter writer = new BinaryWriter(await file.OpenStreamForWriteAsync()))
          {
            writer.Seek(0, SeekOrigin.End);
            writer.Write(buffer);
            _bytesReceived += buffer.Length;
          }


        }
        finally
        {
          // Release the thread
          _sl.Release();
        }
      }

      private bool isDtlsReady()
      {
        if (isSendDtlsReadyAndValidated && isReceiveDtlsReadyAndValidated)
        {
          this.tbLog.Text += "DTLS Channel is now ready !!!\n";
          return true;
        }
        else
        {
          return false;
        }
      }

      private void startSctp()
      {
        if(!isDtlsStarted && isSendDtlsConstructed && isReceiveDtlsConstructed)
        {
          
          isDtlsStarted = true;

          this.tbLog.Text += "Starting SctpTransport 1 Object\n";
          _sctpTransport.start(RTCSctpTransport.getCapabilities());
          this.tbLog.Text += "Starting SctpTransport 2 Object\n";
          _sctpTransport2.start(RTCSctpTransport.getCapabilities());
        }
      }
    }
}
