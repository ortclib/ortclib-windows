#include "pch.h"
#include "RTCSctpTransport.h"
#include "helpers.h"

namespace ortc_winrt_api
{

  RTCSctpTransport::RTCSctpTransport() :
    mNativeDelegatePointer(nullptr),
    mNativePointer(nullptr)
  {
  }

  RTCSctpTransport::RTCSctpTransport(RTCDtlsTransport^ dtlsTransport, uint16 port) :
    mNativeDelegatePointer(new RTCSctpTransportDelegate())
  {

    if (!dtlsTransport)
    {
      return;
    }

    if (FetchNativePointer::FromDtlsTransport(dtlsTransport))
    {
      mNativeDelegatePointer->SetOwnerObject(this);
      mNativePointer = ISCTPTransport::create(mNativeDelegatePointer, FetchNativePointer::FromDtlsTransport(dtlsTransport), port);
    }
  }

  RTCSctpCapabilities^ RTCSctpTransport::GetCapabilities()
  {
    RTCSctpCapabilities^ ret = ref new RTCSctpCapabilities();

    ISCTPTransportTypes::CapabilitiesPtr caps = ISCTPTransport::getCapabilities();
    ret->MaxMessageSize = caps->mMaxMessageSize;
    PushNativePointer::ToRTCSctpCapabilities(ret, caps);
    return ret;
  }

  void RTCSctpTransport::Start(RTCSctpCapabilities^ remoteCaps)
  {
    if (mNativePointer)
    {
      ISCTPTransportTypes::Capabilities caps;
      caps.mMaxMessageSize = remoteCaps->MaxMessageSize;
      mNativePointer->start(caps);
    }
  }

  void RTCSctpTransport::Stop()
  {
    if (mNativePointer)
    {
      mNativePointer->stop();
    }
  }

  RTCDtlsTransport^ RTCSctpTransport::GetDtlsTransport()
  {
    return ConvertObjectToCx::ToDtlsTransport(mNativePointer->transport());
  }

  void RTCSctpTransportDelegate::onSCTPTransportDataChannel(
    ISCTPTransportPtr transport,
    IDataChannelPtr channel
    )
  {
    auto evt = ref new RTCDataChannelEvent();
    RTCDataChannelDelegatePtr delegate(new RTCDataChannelDelegate());
    RTCDataChannel^ dataChannel = ref new RTCDataChannel();
    delegate->SetOwnerObject(dataChannel);
    dataChannel->mNativeDelegatePointer = delegate;
    dataChannel->mNativePointer = channel;

    evt->DataChannel = dataChannel;
    _transport->OnSCTPTransportDataChannel(evt);
  }

  //---------------------------------------------------------------------------
  // RTCSctpCapabilities methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCSctpCapabilities::ToJsonString()
  {
    if (mNativePointer)
    {
      return ToCx(openpeer::services::IHelper::toString(mNativePointer->createElement("SctpCapabilities")));
    }

  }
  RTCSctpCapabilities^ RTCSctpCapabilities::FromJsonString(Platform::String^ jsonString)
  {
    auto ret = ref new RTCSctpCapabilities();

    auto caps = make_shared<ISCTPTransport::Capabilities>(ISCTPTransport::Capabilities::Capabilities(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    ret->MaxMessageSize = caps->mMaxMessageSize;
    PushNativePointer::ToRTCSctpCapabilities(ret, caps);
    return ret;
  }
} // namespace ortc_winrt_api