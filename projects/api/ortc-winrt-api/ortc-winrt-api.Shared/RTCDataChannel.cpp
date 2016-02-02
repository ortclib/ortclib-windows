#include "pch.h"
#include "RTCDataChannel.h"
#include "helpers.h"
#include <op-services-cpp\openpeer\services\IHelper.h>

namespace ortc_winrt_api
{
  namespace internal
  {
    //---------------------------------------------------------------------------
    // RTCDataChannelState convert methods
    //---------------------------------------------------------------------------
    static IDataChannel::States convert(RTCDataChannelState state)
    {
      switch (state) {
      case RTCDataChannelState::Connecting:   return IDataChannel::States::State_Connecting;
      case RTCDataChannelState::Open:         return IDataChannel::States::State_Open;
      case RTCDataChannelState::Closing:      return IDataChannel::States::State_Closing;
      case RTCDataChannelState::Closed:       return IDataChannel::States::State_Closed;
      }
      throw ref new Platform::NotImplementedException();
    }

    static RTCDataChannelState convert(IDataChannel::States state)
    {
      switch (state) {
      case IDataChannel::States::State_Connecting:   return RTCDataChannelState::Connecting;
      case IDataChannel::States::State_Open:         return RTCDataChannelState::Open;
      case IDataChannel::States::State_Closing:      return RTCDataChannelState::Closing;
      case IDataChannel::States::State_Closed:       return RTCDataChannelState::Closed;
      }
      throw ref new Platform::NotImplementedException();
    }
  } // namespace internal

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
      return internal::convert(mNativePointer->readyState());
    else
      return RTCDataChannelState::Closed;
  }

  void RTCDataChannelDelegate::onDataChannelStateChanged(
    IDataChannelPtr channel,
    States state
    )
  {
    auto evt = ref new RTCDataChannelStateChangeEvent();
    evt->State = internal::convert(state);
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
} // namespace ortc_winrt_api
