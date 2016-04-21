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

    public ref struct RTCDTMFToneChangeEvent sealed
    {
      friend class internal::RTCDtmfSenderDelegate;

      property Platform::String^ Tone
      {
        Platform::String^ get() { return _tone; }
      }

    private:
      Platform::String^ _tone;
    };

    public delegate void RTCDtmfSenderOnToneChangeDelegate(RTCDTMFToneChangeEvent^ evt);

    public ref class RTCDtmfSender sealed
    {
      friend class internal::RTCDtmfSenderDelegate;

    private:
      RTCDtmfSender();

    public:
      RTCDtmfSender(RTCRtpSender^ sender);

      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void                        InsertDtmf(Platform::String^ tones);
      [Windows::Foundation::Metadata::OverloadAttribute("InsertDtmfWithDuration")]
      void                        InsertDtmf(Platform::String^ tones, uint64 duration);
      [Windows::Foundation::Metadata::OverloadAttribute("InsertDtmfWithDurationAndGap")]
      void                        InsertDtmf(Platform::String^ tones, uint64 duration, uint64 interToneGap);

      property Platform::Boolean CanInsertDtmf
      {
        Platform::Boolean get();
      }

      property RTCRtpSender^ Sender
      {
        RTCRtpSender^ get();
      }

      property Platform::String^ ToneBuffer
      {
        Platform::String^ get();
      }

      property uint64 Duration
      {
        uint64 get();
      }

      property uint64 InterToneGap
      {
        uint64 get();
      }

      event RTCDtmfSenderOnToneChangeDelegate^       OnToneChange;

    private:
      IDTMFSenderPtr _nativePointer;
      internal::RTCDtmfSenderDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
