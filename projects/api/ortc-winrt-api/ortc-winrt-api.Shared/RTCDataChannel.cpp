#include "pch.h"
#include "RTCDataChannel.h"
#include "helpers.h"

namespace ortc_winrt_api
{

  RTCDataChannel::RTCDataChannel() :
    mNativeDelegatePointer(nullptr),
    mNativePointer(nullptr)
  {
  }

  RTCDataChannel::RTCDataChannel(RTCSctpTransport^ dataTransport, RTCDataChannelParameters^ params) :
    mNativeDelegatePointer(new RTCDataChannelDelegate())
  {

    if (!dataTransport)
    {
      return;
    }

    if (FetchNativePointer::FromSctpTransport(dataTransport))
    {
      mNativeDelegatePointer->SetOwnerObject(this);
      mNativePointer = IDataChannel::create(mNativeDelegatePointer, FetchNativePointer::FromSctpTransport(dataTransport), FromCx(params));
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
      mNativePointer->send(FromCx(data));
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
    return ToCx(mNativePointer->parameters());
  }

  Platform::String^ RTCDataChannel::GetBinaryType()
  {
    return ToCx(mNativePointer->binaryType());
  }

  void RTCDataChannel::SetBinaryType(Platform::String^ binaryType)
  {
    mNativePointer->binaryType(FromCx(binaryType).c_str());
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
    evt->Error->ErrorReason = ToCx(errorReason);
    _channel->OnDataChannelError(evt);
  }

  void RTCDataChannelDelegate::onDataChannelMessage(
    IDataChannelPtr channel,
    MessageEventDataPtr data
    )
  {
    auto evt = ref new RTCMessageEventDataEvent();
    evt->MessageData->Binary = ref new Array<byte>(data->mBinary->BytePtr(), data->mBinary->SizeInBytes());
    evt->MessageData->Text = ToCx(data->mText);
    _channel->OnDataChannelMessage(evt);
  }

  //---------------------------------------------------------------------------
  // RTCIceParameters methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCDataChannelParameters::ToJsonString()
  {
    if (mNativePointer)
    {
      return ToCx(openpeer::services::IHelper::toString(mNativePointer->createElement("DataChannelParameters")));
    }

  }
  RTCDataChannelParameters^ RTCDataChannelParameters::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCDataChannelParameters();

    auto params = make_shared<IDataChannel::Parameters>(IDataChannel::Parameters::Parameters(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret = ToCx(params);
    PushNativePointer::ToRTCDataChannelParameters(ret, params);
    return ret;
  }
} // namespace ortc_winrt_api
