#include "pch.h"

#include "RTCDataChannel.h"
#include "RTCSctpTransport.h"
#include "helpers.h"
#include "Error.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/SafeInt.h>

using Platform::Array;

using namespace ortc;

namespace org
{
  namespace ortc
  {
    using zsLib::Milliseconds;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
      RTCDataChannelParameters^ ToCx(const IDataChannelTypes::Parameters &input)
      {
        auto result = ref new RTCDataChannelParameters();

        result->Label = Helper::ToCx(input.mLabel);
        result->Ordered = input.mOrdered;
        if (Milliseconds() != input.mMaxPacketLifetime)
        {
          result->MaxPacketLifetime = input.mMaxPacketLifetime.count();
        }
        else
        {
          result->MaxPacketLifetime = 0;
        }
        result->MaxRetransmits = Helper::ToCx(input.mMaxRetransmits);
        result->Protocol = Helper::ToCx(input.mProtocol);
        result->Negotiated = input.mNegotiated;
        result->Id = Helper::ToCx(input.mID);

        return result;
      }

      RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(input);
      }

      IDataChannelTypes::ParametersPtr FromCx(RTCDataChannelParameters^ input)
      {
        if (nullptr == input) return IDataChannelTypes::ParametersPtr();
        auto result = std::make_shared<IDataChannelTypes::Parameters>();

        result->mLabel = Helper::FromCx(input->Label);
        result->mOrdered = input->Ordered;
        if (0 != input->MaxPacketLifetime) result->mMaxPacketLifetime = Milliseconds(SafeInt<Milliseconds::rep>(input->MaxPacketLifetime));
        result->mMaxRetransmits = Helper::FromCx(input->MaxRetransmits);
        result->mProtocol = Helper::FromCx(input->Protocol);
        result->mNegotiated = input->Negotiated;
        result->mID = Helper::FromCx(input->Id);

        return result;
      }
    }

    RTCDataChannel::RTCDataChannel() :
      _nativeDelegatePointer(nullptr),
      _nativePointer(nullptr)
    {
    }

    RTCDataChannel::RTCDataChannel(RTCSctpTransport^ transport, RTCDataChannelParameters^ params) :
      _nativeDelegatePointer(new RTCDataChannelDelegate())
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == params)

      _nativeDelegatePointer->SetOwnerObject(this);
      auto nativeTransport = RTCSctpTransport::Convert(transport);

      try
      {
        _nativePointer = IDataChannel::create(_nativeDelegatePointer, nativeTransport, *internal::FromCx(params));
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

    void RTCDataChannel::Close()
    {
      if (!_nativePointer) return;
      _nativePointer->close();
    }

    void RTCDataChannel::Send(Platform::String^ data)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
        _nativePointer->send(UseHelper::FromCx(data));
    }

    void RTCDataChannel::Send(const Platform::Array<byte>^ data)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      try
      {
        SecureByteBlockPtr bytes = openpeer::services::IHelper::convertToBuffer(data->Data, data->Length);
        _nativePointer->send(*bytes.get());
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

    void RTCDataChannel::Send(const Platform::Array<byte>^ data, uint16 bufferSizeInBytes)
    {
      try
      {
        ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
        _nativePointer->send(data->Data, data->Length);
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

    RTCSctpTransport^ RTCDataChannel::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCSctpTransport::Convert(ISCTPTransport::convert(_nativePointer->transport()));
    }

    RTCDataChannelParameters^ RTCDataChannel::Parameters::get()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->parameters());
    }

    RTCDataChannelState RTCDataChannel::State::get()
    {
      if (!_nativePointer) return RTCDataChannelState::Closed;
      return UseHelper::Convert(_nativePointer->readyState());
    }

    uint64 RTCDataChannel::BufferedAmount::get()
    {
      if (_nativePointer) return _nativePointer->bufferedAmount();
      return 0;
    }

    uint64 RTCDataChannel::BufferedAmountLowThreshold::get()
    {
      if (_nativePointer) return _nativePointer->bufferedAmountLowThreshold();
      return 0;
    }

    void RTCDataChannel::BufferedAmountLowThreshold::set(uint64 threshold)
    {
      if (_nativePointer) _nativePointer->bufferedAmountLowThreshold(static_cast<size_t>(threshold));
    }

    Platform::String^ RTCDataChannel::BinaryType::get()
    {
      if (!_nativePointer) return nullptr;
      return UseHelper::ToCx(_nativePointer->binaryType());
    }

    void RTCDataChannel::BinaryType::set(Platform::String^ binaryType)
    {
      if (!_nativePointer) return;
      _nativePointer->binaryType(UseHelper::FromCx(binaryType).c_str());
    }

    void RTCDataChannelDelegate::onDataChannelStateChanged(
      IDataChannelPtr channel,
      States state
      )
    {
      auto evt = ref new RTCDataChannelStateChangeEvent();
      evt->State = UseHelper::Convert(state);
      _channel->OnDataChannelStateChanged(evt);
    }

    void RTCDataChannelDelegate::onDataChannelError(
      IDataChannelPtr channel,
      ErrorCode errorCode,
      String errorReason
      )
    {
      auto evt = ref new RTCDataChannelErrorEvent();
      evt->Error->ErrorCode = errorCode;
      evt->Error->ErrorReason = UseHelper::ToCx(errorReason);
      _channel->OnDataChannelError(evt);
    }

    void RTCDataChannelDelegate::onDataChannelBufferedAmountLow(
      IDataChannelPtr channel
      )
    {
      auto evt = ref new RTCDataChannelBufferedAmountLowEvent();
      _channel->OnDataChannelBufferedAmountLow(evt);
    }

    void RTCDataChannelDelegate::onDataChannelMessage(
      IDataChannelPtr channel,
      MessageEventDataPtr data
      )
    {
      auto evt = ref new RTCMessageEvent();
      evt->Data->Binary = ref new Array<byte>(data->mBinary->BytePtr(), SafeInt<unsigned int>(data->mBinary->SizeInBytes()));
      evt->Data->Text = UseHelper::ToCx(data->mText);
      _channel->OnDataChannelMessage(evt);
    }

    //---------------------------------------------------------------------------
    // RTCIceParameters methods
    //---------------------------------------------------------------------------
    Platform::String^ RTCDataChannelParameters::ToJsonString()
    {
      auto params = internal::FromCx(this);
      return UseHelper::ToCx(openpeer::services::IHelper::toString(params->createElement("DataChannelParameters")));
    }

    RTCDataChannelParameters^ RTCDataChannelParameters::FromJsonString(Platform::String^ jsonString)
    {
      auto params = make_shared<IDataChannel::Parameters>(IDataChannel::Parameters::Parameters(openpeer::services::IHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return internal::ToCx(params);
    }
    
  } // namespace ortc
} // namespace ortc
