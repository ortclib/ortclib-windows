#include "pch.h"
#include "RTCDataChannel.h"
#include "helpers.h"
#include <op-services-cpp\openpeer\services\IHelper.h>

using namespace ortc_winrt_api;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

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

void RTCDataChannel::close()
{
  if (mNativePointer)
  {
    mNativePointer->close();
  }
}

void RTCDataChannel::send(Platform::String^ data)
{
  if (mNativePointer)
  {
    mNativePointer->send(FromCx(data));
    ++sentPackets;
  }
}

void RTCDataChannel::send(const Platform::Array<byte>^ data)
{
  if (mNativePointer)
  {
    SecureByteBlockPtr bytes = openpeer::services::IHelper::convertToBuffer(data->Data, data->Length);
    mNativePointer->send(*bytes.get());
    ++sentPackets;
  }
}

void RTCDataChannel::send(const Platform::Array<byte>^ data, uint16 bufferSizeInBytes)
{
  if (mNativePointer)
  {
    mNativePointer->send(data->Data, bufferSizeInBytes);
    ++sentPackets;
  }
}

void RTCDataChannelDelegate::onDataChannelStateChanged(
  IDataChannelPtr channel,
  States state
  )
{
  auto evt = ref new RTCDataChannelStateChangeEvent();
  evt->State = (RTCDataChannelState)state;
  _channel->OnDataChannelStateChanged(evt);
}

void RTCDataChannelDelegate::onDataChannelError(
  IDataChannelPtr channel,
  ErrorCode errorCode,
  String errorReason
  )
{
  auto evt = ref new RTCDataChannelErrorEvent();
  evt->Error = ref new RTCDataChannelError();
  evt->Error->ErrorCode = errorCode;
  evt->Error->ErrorReason = ToCx(errorReason);
  _channel->OnDataChannelError(evt);
}

void RTCDataChannelDelegate::onDataChannelMessage(
  IDataChannelPtr channel,
  MessageEventDataPtr data
  )
{
  ++numberOfPackets;
  auto evt = ref new RTCMessageEventDataEvent();
  evt->MessageData = ref new RTCMessageEventData();
  if (data->mBinary)
  {
    evt->MessageData->Binary = ref new Array<byte>(data->mBinary->BytePtr(), data->mBinary->SizeInBytes());
  }
  if (data->mText)
  {
    evt->MessageData->Text = ToCx(data->mText);
  }
  _channel->OnDataChannelMessage(evt);
}
