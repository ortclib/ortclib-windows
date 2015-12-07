#pragma once

#include <ortc/ISCTPTransport.h>
#include <collection.h>

#include "RTCDtlsTransport.h"
#include "RTCDataChannel.h"

using namespace ortc;

namespace ortc_winrt_api
{
  ZS_DECLARE_CLASS_PTR(RTCSctpTransportDelegate)

  ref class RTCSctpTransport;
  ref class RTCDataChannel;

  class RTCSctpTransportDelegate : public ISCTPTransportDelegate
  {
  public:
    virtual void onSCTPTransportDataChannel(
      ISCTPTransportPtr transport,
      IDataChannelPtr channel
      );

    RTCSctpTransport^ _transport;

    void SetOwnerObject(RTCSctpTransport^ owner) { _transport = owner; }
  };

  //--------------------------------------------------------------------
  // Helper classes
  //--------------------------------------------------------------------

  public ref class RTCSctpCapabilities sealed
  {
  public:
    property uint16 MaxMessageSize;
  };

  //------------------------------------------
  // Events and Delegates
  //------------------------------------------
  // Data channel event and delegate
  public ref class RTCDataChannelEvent sealed {
  public:
    property RTCDataChannel^ DataChannel
    {
      RTCDataChannel^  get(){ return m_channel; }
      void  set(RTCDataChannel^ value){ m_channel = value; }
    }

  private:
    RTCDataChannel^ m_channel;
  };

  public delegate void RTCSctpTransportDataChannelDelegate(RTCDataChannelEvent^ evt);
  //------------------------------------------
  // End Events and Delegates
  //------------------------------------------

  public ref class RTCSctpTransport sealed
  {
    friend class RTCSctpTransportDelegate;
    friend class FetchNativePointer;
  public:
    RTCSctpTransport();
    RTCSctpTransport(RTCDtlsTransport^ dtlsTransport, uint16 port);

    static RTCSctpCapabilities^ getCapabilities();
    void                       start(RTCSctpCapabilities^ remoteCaps);
    void                       stop();
  private:
    ISCTPTransportPtr mNativePointer;
    RTCSctpTransportDelegatePtr mNativeDelegatePointer;

  public:

    event RTCSctpTransportDataChannelDelegate^            OnSCTPTransportDataChannel;
  };
}