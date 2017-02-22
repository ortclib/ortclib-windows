using System;
using Microsoft.VisualStudio.TestPlatform.UnitTestFramework;
using System.Threading.Tasks;

using Org.Ortc;
using Log = Org.Ortc.Log;
using System.Collections.Generic;

namespace Org.Ortc.Test
{
    [TestClass]
    public class UnitTest_Transports
    {
        UnitTest_Transports()
        {
            Logger.SetDefaultLogLevel(Log.Level.Trace);
            Logger.SetLogLevel(Log.Component.ZsLib, Log.Level.Trace);
            Logger.SetLogLevel(Log.Component.Services, Log.Level.Trace);
            Logger.SetLogLevel(Log.Component.ServicesHttp, Log.Level.Trace);
            Logger.SetLogLevel(Log.Component.OrtcLib, Log.Level.Insane);
            Logger.SetLogLevel("ortc_standup", Log.Level.Insane);


            //openpeer::services::ILogger::installDebuggerLogger();
            Logger.InstallTelnetLogger(59999, TimeSpan.FromSeconds(60), true);

            Settings.ApplyDefaults();
            OrtcLib.Setup();
        }

        [TestMethod]
        public async Task TestMethod1()
        {
            RTCIceGatherOptions options = new RTCIceGatherOptions();
            RTCIceServer server = new RTCIceServer();

            server.Username = "Bojan";
            server.Credential = "12345";
            server.Urls = new List<String>();
            server.Urls.Add("stun:stun.vline.com");
            options.IceServers = new List<RTCIceServer>();
            options.IceServers.Add(server);

            _iceGatherer = new RTCIceGatherer(options);
            _iceGatherer.OnStateChange += this.RTCIceGatherer_onICEGathererStateChanged;
            _iceGatherer.OnLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate;
            _iceGatherer.OnLocalCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete;
            _iceGatherer.OnLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone;
            _iceGatherer.OnError += this.RTCIceGatherer_onICEGathererError;


            _iceGatherer2 = new RTCIceGatherer(options);
            _iceGatherer2.OnStateChange += this.RTCIceGatherer_onICEGathererStateChanged2;
            _iceGatherer2.OnLocalCandidate += this.RTCIceGatherer_onICEGathererLocalCandidate2;
            _iceGatherer2.OnLocalCandidateComplete += this.RTCIceGatherer_onICEGathererCandidateComplete2;
            _iceGatherer2.OnLocalCandidateGone += this.RTCIceGatherer_onICEGathererLocalCandidateGone2;
            _iceGatherer2.OnError += this.RTCIceGatherer_onICEGathererError;

            _iceTransport = new RTCIceTransport(_iceGatherer);
            _iceTransport.OnStateChange += RTCIceTransport_onICETransportStateChanged;
            _iceTransport.OnCandidatePairAvailable += RTCIceTransport_onICETransportCandidatePairAvailable;
            _iceTransport.OnCandidatePairGone += RTCIceTransport_onICETransportCandidatePairGone;
            _iceTransport.OnCandidatePairChange += RTCIceTransport_onICETransportCandidatePairChanged;

            _iceTransport2 = new RTCIceTransport(_iceGatherer);
            _iceTransport2.OnStateChange += RTCIceTransport_onICETransportStateChanged2;
            _iceTransport2.OnCandidatePairAvailable += RTCIceTransport_onICETransportCandidatePairAvailable2;
            _iceTransport2.OnCandidatePairGone += RTCIceTransport_onICETransportCandidatePairGone2;
            _iceTransport2.OnCandidatePairChange += RTCIceTransport_onICETransportCandidatePairChanged2;

            RTCCertificate.GenerateCertificate().AsTask<RTCCertificate>().ContinueWith((cert) =>
            {
                String str = cert.Result.Expires.ToString();
                List<RTCCertificate> certs = new List<RTCCertificate>();
                certs.Add(cert.Result);
                _dtlsTransport = new RTCDtlsTransport(_iceTransport, certs);
                MediaStreamConstraints constraints = new MediaStreamConstraints();

                constraints.Audio = new MediaTrackConstraints();
                constraints.Video = new MediaTrackConstraints();

                MediaDevices.GetUserMedia(constraints).AsTask().ContinueWith<IList<MediaStreamTrack>>((mediaListTask) =>
                {
                    IList<MediaStreamTrack> mediaList = mediaListTask.Result;
                    if (mediaList != null && mediaList.Count > 0)
                    {
                        //List<MediaStreamTrack> ret = new List<MediaStreamTrack>(temp.Result);
                        List<RTCRtpSender> senders = new List<RTCRtpSender>();
                        foreach (MediaStreamTrack track in mediaList)
                        {
                            RTCRtpSender rtpSender = new RTCRtpSender(track, RTCRtpTransport.Cast(_dtlsTransport));
                            senders.Add(rtpSender);
                        }

                        return mediaList;
                    }

                    return null;
                });
            });

            RTCCertificate.GenerateCertificate().AsTask<RTCCertificate>().ContinueWith((cert) =>
            {
                var certs = new List<RTCCertificate>();
                certs.Add(cert.Result);
                _dtlsTransport2 = new RTCDtlsTransport(_iceTransport2, certs);
            });

            MediaDevices.EnumerateDevices().AsTask().ContinueWith<MediaDeviceInfo>((temp) =>
            {
                foreach (MediaDeviceInfo info in temp.Result)
                {
                    if (info.DeviceId != null)
                        System.Diagnostics.Debug.WriteLine("DeviceID: {0}", info.DeviceId);
                }
                return null;
            });

        }


        //---------------------------------------------------------------------
        // ICE GATHERER EVENT HANDLERS
        //---------------------------------------------------------------------

        private void RTCIceGatherer_onICEGathererStateChanged(RTCIceGathererStateChangeEvent evt)
        {
            if (evt.State == RTCIceGathererState.Complete)
            {
                _iceTransport.Start(_iceGatherer, _iceGatherer2.LocalParameters, RTCIceRole.Controlled);
            }
        }

        private void RTCIceGatherer_onICEGathererLocalCandidate(RTCIceGathererCandidateEvent evt)
        {
            //_iceTransport2.AddRemoteCandidate(evt.Candidate);
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

        private void RTCIceGatherer_onICEGathererError(RTCIceGathererIceErrorEvent evt)
        {
            int i = 0;
            i++;
        }

        //***********************************************************************************************

        private void RTCIceGatherer_onICEGathererStateChanged2(RTCIceGathererStateChangeEvent evt)
        {
            if (evt.State == RTCIceGathererState.Complete)
            {
                _iceTransport2.Start(_iceGatherer2, _iceGatherer.LocalParameters, RTCIceRole.Controlling);
            }
        }
        private void RTCIceGatherer_onICEGathererLocalCandidate2(RTCIceGathererCandidateEvent evt)
        {
            //_iceTransport.AddRemoteCandidate(evt.Candidate);

        }

        private void RTCIceGatherer_onICEGathererCandidateComplete2(RTCIceGathererCandidateCompleteEvent evt)
        {
            int i = 0;
            i++;
        }

        private void RTCIceGatherer_onICEGathererLocalCandidateGone2(RTCIceGathererCandidateEvent evt)
        {
            int i = 0;
            i++;
        }

        private void RTCIceGatherer_onICEGathererError2(RTCIceGathererIceErrorEvent evt)
        {
            int i = 0;
            i++;
        }

        //----------------------------------------------------------------------------------
        // ICE TRANSPORT EVENT HANDLERS
        //----------------------------------------------------------------------------------

        private void RTCIceTransport_onICETransportStateChanged(RTCIceTransportStateChangeEvent evt)
        {
            int i = 0;
            i++;
        }

        private void RTCIceTransport_onICETransportCandidatePairAvailable(RTCIceCandidatePairChangeEvent evt)
        {
            int i = 0;
            i++;
        }
        private void RTCIceTransport_onICETransportCandidatePairGone(RTCIceCandidatePairChangeEvent evt)
        {
            int i = 0;
            i++;
        }
        private void RTCIceTransport_onICETransportCandidatePairChanged(RTCIceCandidatePairChangeEvent evt)
        {
            int i = 0;
            i++;
        }

        //****************************************************************************************************

        private void RTCIceTransport_onICETransportStateChanged2(RTCIceTransportStateChangeEvent evt)
        {
            int i = 0;
            i++;
        }

        private void RTCIceTransport_onICETransportCandidatePairAvailable2(RTCIceCandidatePairChangeEvent evt)
        {
            int i = 0;
            i++;
        }
        private void RTCIceTransport_onICETransportCandidatePairGone2(RTCIceCandidatePairChangeEvent evt)
        {
            int i = 0;
            i++;
        }
        private void RTCIceTransport_onICETransportCandidatePairChanged2(RTCIceCandidatePairChangeEvent evt)
        {
            int i = 0;
            i++;
        }

        RTCIceGatherer _iceGatherer;
        RTCIceTransport _iceTransport;
        RTCDtlsTransport _dtlsTransport;

        RTCIceGatherer _iceGatherer2;
        RTCIceTransport _iceTransport2;
        RTCDtlsTransport _dtlsTransport2;
    }
}
