#pragma once

#include <ortc/IDTMFSender.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTMFSender, IDTMFSender)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTMFSenderDelegate, IDTMFSenderDelegate)

    ref class RTCDtmfSender;
    ref class RTCRtpSender;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCDtmfSenderDelegate)
    } // namespace internal

    /// <summary>
    /// The OnToneChange event uses the RTCDTMFToneChangeEvent interface.
    /// </summary>
    public ref struct RTCDTMFToneChangeEvent sealed
    {
      friend class internal::RTCDtmfSenderDelegate;

      /// <summary>
      /// Gets the tone attribute contains the character for the tone that has
      /// just begun playout (see InsertDTMF()). If the value is the empty
      /// string, it indicates that the previous tone has completed playback.
      /// </summary>
      property Platform::String^ Tone
      {
        Platform::String^ get() { return _tone; }
      }

    private:
      Platform::String^ _tone;
    };

    public delegate void RTCDtmfSenderOnToneChangeDelegate(RTCDTMFToneChangeEvent^ evt);

    /// <summary>
    /// An RTCDtmfSender instance allows sending DTMF tones to/from the remote
    /// peer, as per [RFC4733].
    /// </summary>
    public ref class RTCDtmfSender sealed
    {
      friend class internal::RTCDtmfSenderDelegate;

    private:
      RTCDtmfSender();

    public:
      /// <summary>
      /// Construct an instance of the RTCDtmfSender associated to an
      /// RTCRtpSender.
      /// </summary>
      RTCDtmfSender(RTCRtpSender^ sender);

      /// <summary>
      /// The InsertDTMF() method is used to send DTMF tones. Since DTMF tones
      /// cannot be sent without configuring the DTMF codec, if InsertDTMF()
      /// is called prior to sender.Send(parameters), or if
      /// sender.Send(parameters) was called but parameters did not include
      /// the DTMF codec, throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                        InsertDtmf(Platform::String^ tones);
      /// <summary>
      /// The InsertDTMF() method is used to send DTMF tones. Since DTMF tones
      /// cannot be sent without configuring the DTMF codec, if InsertDTMF()
      /// is called prior to sender.Send(parameters), or if
      /// sender.Send(parameters) was called but parameters did not include
      /// the DTMF codec, throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("InsertDtmfWithDuration")]
      void                        InsertDtmf(Platform::String^ tones, uint64 duration);
      /// <summary>
      /// The InsertDTMF() method is used to send DTMF tones. Since DTMF tones
      /// cannot be sent without configuring the DTMF codec, if InsertDTMF()
      /// is called prior to sender.Send(parameters), or if
      /// sender.Send(parameters) was called but parameters did not include
      /// the DTMF codec, throw an InvalidStateError exception.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("InsertDtmfWithDurationAndGap")]
      void                        InsertDtmf(Platform::String^ tones, uint64 duration, uint64 interToneGap);

      /// <summary>
      /// Gets whether the RTCDtmfSender is capable of sending DTMF.
      /// </summary>
      property Platform::Boolean CanInsertDtmf
      {
        Platform::Boolean get();
      }

      /// <summary>
      /// Gets the RTCRtpSender instance.
      /// </summary>
      property RTCRtpSender^ Sender
      {
        RTCRtpSender^ get();
      }

      /// <summary>
      /// Gets the toneBuffer attribute returns a list of the tones remaining
      /// to be played out.
      /// </summary>
      property Platform::String^ ToneBuffer
      {
        Platform::String^ get();
      }

      /// <summary>
      /// Get the duration attribute returns the current tone duration value
      /// in milliseconds. This value will be the value last set via the
      /// InsertDTMF() method, or the default value of 100 ms if
      /// InsertDTMF() was called without specifying the duration.
      /// </summary>
      property uint64 Duration
      {
        uint64 get();
      }

      /// <summary>
      /// Gets the interToneGap attribute returns the current value of the
      /// between-tone gap. This value will be the value last set via the
      /// InsertDTMF() method, or the default value of 70 ms if InsertDTMF()
      /// was called without specifying the interToneGap.
      /// </summary>
      property uint64 InterToneGap
      {
        uint64 get();
      }

      /// <summary>
      /// The OnToneChange event handler uses the RTCDTMFToneChangeEvent
      /// interface to return the character for each tone as it is played out.
      /// </summary>
      event RTCDtmfSenderOnToneChangeDelegate^       OnToneChange;

    private:
      IDTMFSenderPtr _nativePointer;
      internal::RTCDtmfSenderDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
