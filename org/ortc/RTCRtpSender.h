#pragma once

#include <ortc/IRTPSender.h>

#include <ppltasks.h>

namespace Org
{
  namespace Ortc
  {
    using Windows::Foundation::IAsyncAction;

    ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)

    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPSender, IRTPSender)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPSenderSubscription, IRTPSenderSubscription)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPSenderDelegate, IRTPSenderDelegate)

    ref class MediaStreamTrack;
    ref class RTCDtlsTransport;
    ref class RTCRtpSender;
    ref class RTCDtmfSender;
    ref class RTCSrtpSdesTransport;
    ref class RTCIceTransport;
    ref class RTCStatsProvider;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;

    namespace Internal
    {
      ZS_DECLARE_CLASS_PTR(RTCRtpSenderDelegate)
      ZS_DECLARE_CLASS_PTR(RTCRtpSenderPromiseObserver)
    } // namespace internal

    namespace Adapter
    {
      ref class RTCPeerConnection;

      namespace Internal
      {
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionPromiseWithSenderObserver);
      }
    } // namespace adapter

    /// <summary>
    /// This object is fired if an SSRC conflict is detected within the RTP
    /// session or an SSRC misconfiguration is detected after Send() or
    /// Receive() returns or when SetTransport() is called.
    /// </summary>
    public ref struct RTCSsrcConflictEvent sealed
    {
      friend class Internal::RTCRtpSenderDelegate;

      /// <summary>
      /// Gets the ssrc attribute represents the conflicting SSRC that caused
      /// the event.
      /// </summary>
      property uint32 Ssrc
      {
        uint32 get() { return _ssrc; }
      }

    private:
      uint32 _ssrc;
    };

    public delegate void RTCRtpSenderSSRCConflictDelegate(RTCSsrcConflictEvent^ evt);

    /// <summary>
    /// The RTCRtpSender includes information relating to the RTP sender.
    /// </summary>
    public ref class RTCRtpSender sealed
    {
      friend class Internal::RTCRtpSenderDelegate;
      friend class Adapter::Internal::RTCPeerConnectionPromiseWithSenderObserver;
      friend ref class RTCDtmfSender;
      friend ref class Adapter::RTCPeerConnection;
      friend ref class RTCStatsProvider;

    private:
      RTCRtpSender(IRTPSenderPtr sender);

      static RTCRtpSender^ Convert(IRTPSenderPtr sender) { if (!sender) return nullptr; return ref new RTCRtpSender(sender); }
      static IRTPSenderPtr Convert(RTCRtpSender^ sender) { if (!sender) return nullptr; return sender->_nativePointer; }

    public:
      /// <summary>
      /// Constructs an instance of an RTCRtpSender from an MediaStreamTrack
      /// object and associated to an RTCDtlsTransport. If an attempt is made
      /// to construct an RTCRtpSender object with transport.State 
      /// "closed", or if track.readyState is "ended", throw an
      /// InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport);
      /// <summary>
      /// Constructs an instance of an RTCRtpSender from an MediaStreamTrack
      /// object and associated to an RTCSrtpSdesTransport. If an attempt is
      /// made to construct an RTCRtpSender object with track.readyState is
      /// "ended", throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSrtpSdesTransport")]
      RTCRtpSender(MediaStreamTrack^ track, RTCSrtpSdesTransport^ transport);
      /// <summary>
      /// Constructs an instance of an RTCRtpSender from an MediaStreamTrack
      /// object and associated to an RTCDtlsTransport. If an attempt is made
      /// to construct an RTCRtpSender object with transport.State or
      /// rtcpTransport.State "closed", or if track.readyState is "ended",
      /// throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtcpTransport")]
      RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      /// <summary>
      /// Constructs an instance of an RTCRtpSender from an MediaStreamTrack
      /// object and associated to an RTCSrtpSdesTransport and
      /// RTCIceTransport. If an attempt is made to construct an RTCRtpSender
      /// object with track.readyState is "ended", throw an
      /// InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSdesSrtpAndIceRtcpTransport")]
      RTCRtpSender(MediaStreamTrack^ track, RTCSrtpSdesTransport^ transport, RTCIceTransport^ rtcpTransport);

      /// <summary>
      /// Obtain the sender capabilities, based on kind. Capabilities such as
      /// retransmission [RFC4588], redundancy [RFC2198], and Forward Error
      /// Correction that do not have an associated value of kind are always
      /// included, regardless of the value of kind passed to
      /// GetCapabilities().
      /// </summary>
      static RTCRtpCapabilities^  GetCapabilities(Platform::String^ kind);

      /// <summary>
      /// Set the RTP RTCDtlsTransport. If the last call to
      /// sender.Send(parameters) had parameters.rtcp.mux set to false,
      /// throw an InvalidParameters exception. If SetTransport() is called
      /// when transport.State is "closed", throw an InvalidStateError
      /// exception.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                        SetTransport(RTCDtlsTransport^ transport);
      /// <summary>
      /// Set the RTP RTCSrtpSdesTransport. If the last call to
      /// sender.Send(parameters) had parameters.rtcp.mux set to false,
      /// throw an InvalidParameters exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetSrtpSdesTransport")]
      void                        SetTransport(RTCSrtpSdesTransport^ transport);
      /// <summary>
      /// Set the RTP RTCDtlsTransport and RTCP RTCDtlsTransport. If
      /// SetTransport() is called when transport.State or rtcpTransport.State
      /// is "closed", throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetTransportWithRtcpTransport")]
      void                        SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      /// <summary>
      /// Set the RTP RTCSrtpSdesTransport and RTCP RTCIceTransport. If
      /// SetTransport() is called when rtcpTransport.State is "closed",
      /// throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetSrtpSdesTransportWithRtcpTransport")]
      void                        SetTransport(RTCSrtpSdesTransport^ transport, RTCIceTransport^ rtcpTransport);
      /// <summary>
      /// Attempts to replace the track being sent with another track provided.
      /// </summary>
      IAsyncAction^               SetTrack(MediaStreamTrack^ track);
      /// <summary>
      /// Media to be sent is controlled by parameters. If Send() is called
      /// with invalid parameters, throw an InvalidParameters exception. If
      /// rtcpTransport is not set and Send(parameters) is called with
      /// parameters.rtcp.mux set to "false", throw an InvalidParameters
      /// exception. For each value of i from 0 to the number of encodings,
      /// check whether parameters.encodings[i].codecPayloadType corresponds
      /// to a value of parameters.codecs[j].payloadType where j goes from 0
      /// to the number of codecs. If parameters.codecs[j].name is equal to
      /// "red", "cn", "dtmf", "rtx" or a forward error correction codec,
      /// throw an InvalidParameters exception. For each value of i from 0 to
      /// the number of codecs, check that each value of
      /// parameters.codecs[i].name not equal to "red", "rtx" or a forward
      /// error correction codec is included in
      /// GetCapabilities(track.kind).codecs[j].name, where j goes from 0 to
      /// the number of codecs. If a match is not found for any value of i,
      /// throw an InvalidParameters exception. If parameters.encodings is
      /// unset, the browser behaves as though a single encodings[0] entry was
      /// provided, with encodings[0].ssrc set to a browser-determined value,
      /// encodings[0].active set to "true", encodings[0].codecPayloadType set
      /// to codecs[j].payloadType where j is the index of the first codec
      /// that is not "cn", "dtmf", "red", "rtx" or a forward error correction
      /// codec, and all the other parameters.encodings[0] attributes (e.g.
      /// fec, rtx, priority, maxBitrate, minQuality, resolutionScale,etc.)
      /// unset. Calling Send(parameters) does not update parameters based on
      /// what is currently being sent. The RTCRtpSender object starts sending
      /// when Send() is called for the first time, and changes the sending
      /// parameters when send() is called again. The RTCRtpSender object
      /// stops sending when stop() is called. SSRC misusage results in an
      /// InvalidParameters exception.
      /// </summary>
      IAsyncAction^               Send(RTCRtpParameters^ parameters);
      /// <summary>
      /// Stops sending the track on the wire, and sends an RTCP BYE. Stop is
      /// final as in
      /// </summary>
      void                        Stop();

      /// <summary>
      /// The associated MediaStreamTrack instance.
      /// </summary>
      property MediaStreamTrack^ Track
      {
        MediaStreamTrack^ get();
      }

      /// <summary>
      /// Gets the RTCDtlsTransport instance over which RTCP is sent and
      /// received. When BUNDLE is used, many RTCRtpSender objects will share
      /// one rtcpTransport and will all send and receive RTCP over the same
      // RTCDtlsTransport. When RTCP mux is used, rtcpTransport will be null,
      /// and both RTP and RTCP traffic will flow over the RTCDtlsTransport
      /// transport.
      /// </summary>
      property Platform::Object^ Transport
      {
        Platform::Object^ get();
      }

      /// <summary>
      /// Gets the associated RTCP RTCDtlsTransport instance if one was
      /// provided in the constructor. When RTCP mux is used, rtcpTransport
      /// will be null, and both RTP and RTCP traffic will flow over the
      /// RTCDtlsTransport transport.
      /// </summary>
      property Platform::Object^ RtcpTransport
      {
        Platform::Object^ get();
      }

      /// <summary>
      /// The OnSsrcConflict event handler, of event handler type
      /// RTCSsrcConflictEvent, is fired if an SSRC conflict is detected
      /// within the RTP session or an SSRC misconfiguration is detected
      /// after Send() or Receive() returns or when SetTransport() is called.
      /// In this situation, the RTCRtpSender automatically sends an RTCP BYE
      /// on the conflicted SSRC, if RTP packets were sent using that SSRC.
      /// </summary>
      event RTCRtpSenderSSRCConflictDelegate^       OnSsrcConflict;

    private:
      IRTPSenderPtr _nativePointer;
      Internal::RTCRtpSenderDelegatePtr _nativeDelegatePointer;
      IRTPSenderSubscriptionPtr _nativeSubscriptionPointer;
    };

  } // namespace ortc
} // namespace org
