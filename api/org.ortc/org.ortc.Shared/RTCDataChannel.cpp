#include "pch.h"
#include <openpeer/services/IHelper.h>
#include "RTCDataChannel.h"
#include "helpers.h"

using zsLib::DWORD;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
#pragma region DataChannel conversion
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
#pragma endregion
    }

    RTCDataChannel::RTCDataChannel() :
      mNativeDelegatePointer(nullptr),
      mNativePointer(nullptr)
    {
    }

    RTCDataChannel::RTCDataChannel(RTCSctpTransport^ dataTransport, RTCDataChannelParameters^ params) :
      mNativeDelegatePointer(new RTCDataChannelDelegate())
    {

      if (!dataTransport) return;

      assert(nullptr != params);

      if (FetchNativePointer::FromSctpTransport(dataTransport))
      {
        mNativeDelegatePointer->SetOwnerObject(this);
        mNativePointer = IDataChannel::create(mNativeDelegatePointer, FetchNativePointer::FromSctpTransport(dataTransport), *internal::FromCx(params));
      }
    }

    void RTCDataChannel::Close()
    {
      if (mNativePointer)
      {
        mNativePointer->close();
      }
    }

    void RTCDataChannel::Send(Platform::String^ data)
    {
      if (mNativePointer)
      {
        mNativePointer->send(UseHelper::FromCx(data));
      }
    }

    void RTCDataChannel::Send(const Platform::Array<byte>^ data)
    {
      if (mNativePointer)
      {
        SecureByteBlockPtr bytes = openpeer::services::IHelper::convertToBuffer(data->Data, data->Length);
        mNativePointer->send(*bytes.get());
      }
    }

    void RTCDataChannel::Send(const Platform::Array<byte>^ data, uint16 bufferSizeInBytes)
    {
      if (mNativePointer)
      {
        mNativePointer->send(data->Data, data->Length);
      }
    }

    RTCSctpTransport^ RTCDataChannel::GetSctpTransport()
    {
      return ConvertObjectToCx::ToSctpTransport(ISctpTransport::convert(mNativePointer->transport()));
    }

    RTCDataChannelParameters^ RTCDataChannel::GetParameters()
    {
      return internal::ToCx(mNativePointer->parameters());
    }

    Platform::String^ RTCDataChannel::GetBinaryType()
    {
      return UseHelper::ToCx(mNativePointer->binaryType());
    }

    void RTCDataChannel::SetBinaryType(Platform::String^ binaryType)
    {
      mNativePointer->binaryType(UseHelper::FromCx(binaryType).c_str());
    }

    RTCDataChannelState RTCDataChannel::State::get()
    {
      if (mNativePointer)
        return internal::ConvertEnums::convert(mNativePointer->readyState());
      else
        return RTCDataChannelState::Closed;
    }

    void RTCDataChannelDelegate::onDataChannelStateChanged(
      IDataChannelPtr channel,
      States state
      )
    {
      auto evt = ref new RTCDataChannelStateChangeEvent();
      evt->State = internal::ConvertEnums::convert(state);
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
