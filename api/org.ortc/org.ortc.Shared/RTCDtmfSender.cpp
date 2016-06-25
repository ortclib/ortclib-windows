
#include "pch.h"

#include "RTCDtmfSender.h"
#include "RTCRtpSender.h"
#include "helpers.h"
#include "Error.h"

#include <zsLib/SafeInt.h>

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
#pragma region RTCDtmfSender delegates 

      class RTCDtmfSenderDelegate : public IDTMFSenderDelegate
      {
      public:
        RTCDtmfSenderDelegate(RTCDtmfSender^ owner) { _owner = owner; }

        void onDTMFSenderToneChanged(
          IDTMFSenderPtr sender,
          String tone
          ) override
        {
          auto evt = ref new RTCDTMFToneChangeEvent();
          evt->_tone = UseHelper::ToCx(tone);
          _owner->OnToneChange(evt);
        }

      private:
        RTCDtmfSender^ _owner;
      };

#pragma endregion

    }

#pragma region RTCDtmfSender 

    RTCDtmfSender::RTCDtmfSender(RTCRtpSender^ sender) :
      _nativeDelegatePointer(make_shared<internal::RTCDtmfSenderDelegate>(this))
    {
      auto nativeSender = RTCRtpSender::Convert(sender);

      try
      {
        _nativePointer = IDTMFSender::create(_nativeDelegatePointer, nativeSender);
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCDtmfSender::InsertDtmf(Platform::String^ tones)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      try
      {
        _nativePointer->insertDTMF(UseHelper::FromCx(tones).c_str());
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCDtmfSender::InsertDtmf(Platform::String^ tones, uint64 duration)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      try
      {
        _nativePointer->insertDTMF(UseHelper::FromCx(tones).c_str(), Milliseconds(duration));
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    void RTCDtmfSender::InsertDtmf(Platform::String^ tones, uint64 duration, uint64 interToneGap)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

        try
      {
        _nativePointer->insertDTMF(UseHelper::FromCx(tones).c_str(), Milliseconds(duration), Milliseconds(interToneGap));
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    Platform::Boolean RTCDtmfSender::CanInsertDtmf::get()
    {
      if (!_nativePointer) return false;
      return _nativePointer->canInsertDTMF();
    }

    RTCRtpSender^ RTCDtmfSender::Sender::get()
    {
      if (!_nativePointer) return nullptr;
      auto sender = RTCRtpSender::Convert(_nativePointer->sender());
      return sender;
    }

    Platform::String^ RTCDtmfSender::ToneBuffer::get()
    {
      if (!_nativePointer) return UseHelper::ToCx("");
      return UseHelper::ToCx(_nativePointer->toneBuffer());
    }

    uint64 RTCDtmfSender::Duration::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return SafeInt<uint64>(_nativePointer->duration().count());
    }

    uint64 RTCDtmfSender::InterToneGap::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      return SafeInt<uint64>(_nativePointer->interToneGap().count());
    }

#pragma endregion

  } // namespacr ortc
} // namespace org
