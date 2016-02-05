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
    property size_t MaxMessageSize;
  public:
    Platform::String^ ToJsonString();
    static RTCSctpCapabilities^ FromJsonString(Platform::String^ jsonString);
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
    friend class ConvertObjectToCx;
  private:
    RTCSctpTransport();
  public:
    RTCSctpTransport(RTCDtlsTransport^ dtlsTransport, uint16 port);

    static RTCSctpCapabilities^ GetCapabilities();
    void                       Start(RTCSctpCapabilities^ remoteCaps);
    void                       Stop();
  private:
    ISCTPTransportPtr mNativePointer;
    RTCSctpTransportDelegatePtr mNativeDelegatePointer;

  private:
    RTCDtlsTransport^ GetDtlsTransport();
  public:
    property RTCDtlsTransport^ Transport
    {
      RTCDtlsTransport^ get()
      {
        if (mNativePointer)
          return GetDtlsTransport();
        else
          return nullptr;
      }
    }

    property uint16 Port
    {
      uint16 get()
      {
        if (mNativePointer)
          return mNativePointer->port();
        else
          return -1;
      }
    }

  public:

    event RTCSctpTransportDataChannelDelegate^            OnSCTPTransportDataChannel;
  };
}