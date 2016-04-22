
#pragma once

#include <ortc/IRTPListener.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPListener, IRTPListener)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPListenerDelegate, IRTPListenerDelegate)

    using Windows::Foundation::Collections::IVector;

    ref class RTCDtlsTransport;
    ref class RTCRtpListener;
    ref struct RTCRtpHeaderExtensionParameters;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCRtpListenerDelegate)
    }

    /// <summary>
    /// This object is fired when the RTCRtpListener detects an RTP stream
    /// that is not configured to be processed by an existing RTCRtpReceiver
    /// object.
    /// </summary>
    public ref struct RTCRtpUnhandledEvent sealed
    {
      friend class internal::RTCRtpListenerDelegate;

      /// <summary>
      /// The SSRC in the RTP stream triggering the OnUnhandledRtp event.
      /// </summary>
      property uint64 Ssrc
      {
        uint64 get() { return _ssrc; }
      }

      /// <summary>
      /// The Payload Type value in the RTP stream triggering the
      /// OnUnhandledRtp event.
      /// </summary>
      property uint8 PayloadType
      {
        uint8 get() { return _payloadType; }
      }

      /// <summary>
      /// If present, the value of the MID RTP header extension [BUNDLE] in
      /// the RTP stream triggering the OnUnhandledRtp event.
      /// </summary>
      property Platform::String^ MuxId
      {
        Platform::String^ get() { return _muxId; }
      }

      /// <summary>
      /// If present, the value of the RID RTP header extension [RID] in the
      /// RTP stream triggering the OnUnhandledRtp event.
      /// </summary>
      property Platform::String^ Rid
      {
        Platform::String^ get() { return _rid; }
      }

    private:
      uint64            _ssrc {};
      uint8             _payloadType {};
      Platform::String^ _muxId;
      Platform::String^ _rid;
    };

    public delegate void RTCRtpListenerUnhandledRtpDelegate(RTCRtpUnhandledEvent^ evt);

    /// <summary>
    /// The RTCRtpListener listens to RTP packets received from the
    /// RTCDtlsTransport, determining whether an incoming RTP stream is
    /// configured to be processed by an existing RTCRtpReceiver object. If no
    /// match is found, the unhandledrtp event is fired. This can be due to
    /// packets having an unknown SSRC, payload type or any other error that
    /// makes it impossible to attribute an RTP packet to a specific
    /// RTCRtpReceiver object. The event is not fired once for each arriving
    /// packet; multiple discarded packets for the same SSRC should result in
    /// a single event.
    /// </summary>
    public ref class RTCRtpListener sealed
    {
      friend class internal::RTCRtpListenerDelegate;

    public:
      /// <summary>
      /// Construct an instance of RTCRtpListener from an RTCDtlsTransport
      /// object.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCRtpListener(RTCDtlsTransport^ transport);
      /// <summary>
      /// Construct an instance of RTCRtpListener from an RTCDtlsTransport
      /// object and the mapped header extensions to properly intepret
      /// RTP header extensions.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithHeaderExtensions")]
      RTCRtpListener(RTCDtlsTransport^ transport, IVector<RTCRtpHeaderExtensionParameters^>^ headerExtensions);

      /// <summary>
      /// Gets the RTP RTCDtlsTransport instance.
      /// </summary>
      property RTCDtlsTransport^ Transport { RTCDtlsTransport^ get(); }

      /// <summary>
      /// The event handler emits the RTCRtpUnhandledEvent, which is fired
      /// when the RTCRtpListener detects an RTP stream that is not configured
      /// to be processed by an existing RTCRtpReceiver object.
      /// </summary>
      event RTCRtpListenerUnhandledRtpDelegate^  OnUnhandledRtp;

    private:
      IRTPListenerPtr _nativePointer;
      internal::RTCRtpListenerDelegatePtr _nativeDelegatePointer;
    };
  }
}
