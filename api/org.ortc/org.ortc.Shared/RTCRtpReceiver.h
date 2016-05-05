
#pragma once

#include "MediaStreamTrack.h"

#include <ortc/IRTPReceiver.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiverDelegate, IRTPReceiverDelegate);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiver, IRTPReceiver);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPReceiverSubscription, IRTPReceiverSubscription);

    using Windows::Foundation::Collections::IVector;

    ref class MediaStreamTrack;
    ref class RTCDtlsTransport;
    ref class RTCSrtpSdesTransport;
    ref class RTCIceTransport;
    ref class RTCRtpReceiver;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpParameters;

    enum class MediaStreamTrackKind;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCRtpReceiverDelegate);
      ZS_DECLARE_CLASS_PTR(RTCRtpReceiverPromiseObserver);
    } // namespace internal

    namespace adapter
    {
      ref class RTCPeerConnection;

      namespace internal
      {
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionDelegate);
      } // namespace internal
    } // namespace adpater

    /// <summary>
    /// The RTCRtpContributingSource object contains information about a
    /// contributing source. Each time an RTP packet is received, the
    /// RTCRtpContributingSource objects are updated. If the RTP packet
    /// contains CSRCs, then the RTCRtpContributingSource objects
    /// corresponding to those CSRCs are updated, and the level values for
    /// those CSRCs are updated based on the mixer-client header extension
    /// [RFC6464] if present. If the RTP packet contains no CSRCs, then the
    /// RTCRtpContributingSource object corresponding to the SSRC is updated,
    /// and the level value for the SSRC is updated based on the client-mixer
    /// header extension [RFC6464] if present.
    /// </summary>
    public ref class RTCRtpContributingSource sealed
    {
      friend RTCRtpReceiver;

    public:
      /// <summary>
      /// Gets the timestamp indicating the time of reception of the most
      /// recent RTP packet containing the source. The timestamp corresponds
      /// to a local clock.
      /// </summary>
      property Windows::Foundation::DateTime TimeStamp
      {
        Windows::Foundation::DateTime get();
      }
      /// <summary>
      /// The CSRC or SSRC value of the contributing source.
      /// </summary>
      property uint32 Source
      {
        uint32  get() { return _source; }
      }
      /// <summary>
      /// The audio level contained in the last RTP packet received from this
      /// source. If the source was set from an SSRC, this will be the level
      /// value in [RFC6464]. If the source was set from a CSRC, this will be
      /// the level value in [RFC6465]. Both [RFC6464] and [RFC6465] define
      /// the level as a integral value from 0 to 127 representing the audio
      /// level in negative decibels relative to the loudest signal that they
      /// system could possibly encode. Thus, 0 represents the loudest signal
      /// the system could possibly encode, and 127 represents silence.
      /// </summary>
      property uint8 AudioLevel
      {
        uint8 get() { return _audioLevel; }
      }

      /// <summary>
      /// Gets whether the last RTP packet received from this source contains
      /// voice activity ("true") or not ("false"). Since the "V" bit is
      /// supported in [RFC6464] but not [RFC6465], the voiceActivityFlag
      /// attribute will only be set when receivers enable the client-mixer
      /// header extension (setting the vad attribute to "true"), and when RTP
      /// packets are received from senders enabling the client-mixer header
      /// extension (setting the vad attribute to "true").
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ VoiceActivityFlag
      {
        Platform::IBox<Platform::Boolean>^ get();
      }

    private:
      std::chrono::system_clock::time_point _timeStamp {};
      uint32                                _source {};
      uint8		                              _audioLevel {};
      zsLib::Optional<bool>                 _voiceActivityFlag {};
    };


    /// <summary>
    /// The RTCRtpReceiver includes information relating to the RTP receiver.
    /// </summary>
    public ref class RTCRtpReceiver sealed
    {
      friend class internal::RTCRtpReceiverDelegate;
      friend class adapter::internal::RTCPeerConnectionDelegate;
      friend ref class adapter::RTCPeerConnection;

    private:
      RTCRtpReceiver(IRTPReceiverPtr receiver);

      static RTCRtpReceiver^ Convert(IRTPReceiverPtr receiver) { if (!receiver) return nullptr; return ref new RTCRtpReceiver(receiver); }
      static IRTPReceiverPtr Convert(RTCRtpReceiver^ receiver) { if (!receiver) return nullptr; return receiver->_nativePointer; }

    public:
      /// <summary>
      /// Constructs an instance of RTCRtpReceiver from a value of kind and an
      /// RTCDtlsTransport object. If an attempt is made to construct an
      /// RTCRtpReceiver object with transport.State  "closed", throw an
      /// InvalidStateError exception. Upon construction, Track is set, and
      /// the value of track.Kind is determined based on the value of kind
      /// passed in the constructor.      
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute()]
      RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport^ transport);
      /// <summary>
      /// Constructs an instance of RTCRtpReceiver from a value of kind and an
      /// RTCSrtpSdesTransport object. Upon construction, Track is set, and
      /// the value of track.Kind is determined based on the value of kind
      /// passed in the constructor.      
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSrtpSdesTransport")]
      RTCRtpReceiver(MediaStreamTrackKind kind, RTCSrtpSdesTransport^ transport);
      /// <summary>
      /// Constructs an instance of RTCRtpReceiver from a value of kind and an
      /// RTCDtlsTransport object. If an attempt is made to construct an
      /// RTCRtpReceiver object with transport.State or rtcpTransport.State
      /// "closed", throw an InvalidStateError exception. Upon construction,
      /// Track is set, and the value of track.Kind is determined based on
      /// the value of kind passed in the constructor.      
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtcpTransport")]
      RTCRtpReceiver(MediaStreamTrackKind kind, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      /// <summary>
      /// Constructs an instance of RTCRtpReceiver from a value of kind and an
      /// RTCSrtpSdesTransport and an RTCIceTransport object. If an attempt is
      /// made to construct an RTCRtpReceiver object with
      /// rtcpTransport.State "closed", throw an InvalidStateError exception.
      /// Upon construction, Track is set, and the value of track.Kind is determined based on
      /// the value of kind passed in the constructor.      
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSdesSrtpAndIceRtcpTransport")]
      RTCRtpReceiver(MediaStreamTrackKind kind, RTCSrtpSdesTransport^ transport, RTCIceTransport^ rtcpTransport);

      /// <summary>
      /// Obtain the receiver capabilities, based on kind. Capabilities such
      /// as retransmission [RFC4588], redundancy [RFC2198], and Forward Error
      /// Correction that do not have an associated value of kind are always
      /// included, regardless of the value of kind passed to
      /// GetCapabilities().
      /// </summary>
      static RTCRtpCapabilities^          GetCapabilities(Platform::String^ kind);

      /// <summary>
      /// Set the RTP RTCDtlsTransport. If the last call to
      /// receive(parameters) had parameters.Rtcp.Mux set to false, throw an
      /// InvalidParameters exception. If SetTransport() is called and
      /// transport.State is "closed", throw an InvalidParameters exception.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                                SetTransport(RTCDtlsTransport^ transport);
      /// <summary>
      /// Set the RTP RTCSrtpSdesTransport. If the last call to
      /// receive(parameters) had parameters.Rtcp.Mux set to false, throw an
      /// InvalidParameters exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetSrtpSdesTransport")]
      void                                SetTransport(RTCSrtpSdesTransport^ transport);
      /// <summary>
      /// Set the RTP RTCDtlsTransport and RTCP RTCDtlsTransport. If
      /// SetTransport() is called and transport.State or rtcpTransport.State
      /// is "closed", throw an InvalidParameters exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetTransportWithRtcpTransport")]
      void                                SetTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport);
      /// <summary>
      /// Set the RTP RTCSrtpSdesTransport and RTCP RTCIceTransport. If
      /// SetTransport() is called and rtcpTransport.State is "closed"
      /// throw an InvalidParameters exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetSrtpSdesTransportWithRtcpTransport")]
      void                                SetTransport(RTCSrtpSdesTransport^ transport, RTCIceTransport^ rtcpTransport);

      /// <summary>
      /// Media to be received is controlled by parameters. If
      /// Receive(parameters) is called with invalid parameters, throw an
      /// InvalidParameters exception. If rtcpTransport is not set and
      /// Receive(parameters) is called with parameters.rtcp.mux set to false,
      /// throw an InvalidParameters exception. The Receive() method does not
      /// update parameters based on what is currently being received, so that
      /// the value of parameters remains that last passed to the Receive()
      /// method. The RTCRtpReceiver object starts receiving when Receive() is
      /// called for the first time, and changes the receiving parameters when
      /// Receive() is called again. The RTCRtpReceiver object stops receiving
      /// when Stop() is called. For each value of i from 0 to the number of
      /// codecs, check that each value of parameters.codecs[i].name not equal
      /// to "red", "rtx" or a forward error correction codec is included in
      /// GetCapabilities(kind).codecs[j].name, where j goes from 0 to the
      /// number of codecs, and kind takes the value passed in the
      /// RTCRtpReceiver constructor. If no match is found for any value of i,
      /// throw an InvalidParameters exception. SSRC misusage also results in
      /// an InvalidParameters exception.
      /// </summary>
      IAsyncAction^                       Receive(RTCRtpParameters^ parameters);
      /// <summary>
      /// Returns an RTCRtpContributingSource object for each unique CSRC or
      /// SSRC received by this RTCRtpReceiver. The browser must keep
      /// information from RTP packets received in the last 10 seconds. If no
      /// contributing sources are available, an empty list is returned.
      /// </summary>
      IVector<RTCRtpContributingSource^>^ GetContributingSource();
      /// <summary>
      /// Stops receiving the track on the wire. Stop is final like
      /// MediaStreamTrack.
      /// </summary>
      void                                Stop();

      /// <summary>
      /// The associated MediaStreamTrack instance. Upon construction, Track
      /// is set. The value of track.kind is determined based on the value of
      /// kind passed in the constructor.
      /// </summary>
      property MediaStreamTrack^ Track
      {
        MediaStreamTrack^ get();
      }
      /// <summary>
      /// The associated RTP RTCDtlsTransport instance.
      /// </summary>
      property Platform::Object^ Transport
      {
        Platform::Object^ get();
      }
      /// <summary>
      /// Gets the RTCDtlsTransport instance over which RTCP is sent and
      /// received. When BUNDLE is used, many RTCRtpReceiver objects will
      /// share one rtcpTransport and will all send and receive RTCP over the
      /// same RTCDtlsTransport. When RTCP mux is used, rtcpTransport will be
      /// null, and both RTP and RTCP traffic will flow over the
      /// RTCDtlsTransport transport.
      /// </summary>
      property Platform::Object^ RtcpTransport
      {
        Platform::Object^ get();
      }
      
    private:
      IRTPReceiverPtr _nativePointer;
      internal::RTCRtpReceiverDelegatePtr _nativeDelegatePointer;
      IRTPReceiverSubscriptionPtr _nativeSubscriptionPointer;
    };
  }
}
