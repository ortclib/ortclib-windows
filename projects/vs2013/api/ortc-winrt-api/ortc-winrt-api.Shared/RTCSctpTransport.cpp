#include "pch.h"
#include "RTCSctpTransport.h"
#include "helpers.h"

using namespace ortc_winrt_api;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

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

  if (FetchNativePointer::fromDtlsTransport(dtlsTransport))
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = ISCTPTransport::create(mNativeDelegatePointer, FetchNativePointer::fromDtlsTransport(dtlsTransport), port);
  }
}

RTCSctpCapabilities^ RTCSctpTransport::getCapabilities()
{
  RTCSctpCapabilities^ ret = ref new RTCSctpCapabilities();

  ISCTPTransportTypes::CapabilitiesPtr caps = ISCTPTransport::getCapabilities();
  ret->MaxMessageSize = caps->mMaxMessageSize;

  return ret;
}

void RTCSctpTransport::start(RTCSctpCapabilities^ remoteCaps)
{
  if (mNativePointer)
  {
    ISCTPTransportTypes::Capabilities caps;
    caps.mMaxMessageSize = remoteCaps->MaxMessageSize;
    mNativePointer->start(caps);
  }
}

void RTCSctpTransport::stop()
{
  if (mNativePointer)
  {
    mNativePointer->stop();
  }
}

void RTCSctpTransportDelegate::onSCTPTransportDataChannel(
  ISCTPTransportPtr transport,
  IDataChannelPtr channel
  )
{
  auto evt = ref new RTCDataChannelEvent();
  //evt->DataChannel = ToCx(channel);
  _transport->OnSCTPTransportDataChannel(evt);
}