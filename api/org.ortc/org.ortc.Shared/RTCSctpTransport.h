#pragma once

#include <ortc/ISCTPTransport.h>

#include "RTCDtlsTransport.h"

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDataChannel, IDataChannel)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISCTPTransport, ISCTPTransport)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISCTPTransportDelegate, ISCTPTransportDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISCTPTransportTypes, ISCTPTransportTypes)

    ZS_DECLARE_CLASS_PTR(RTCDataChannelDelegate)
    ZS_DECLARE_CLASS_PTR(RTCSctpTransportDelegate)

    ref class RTCDataChannel;
    ref class RTCDtlsTransport;
    ref class RTCSctpTransport;

    ref struct RTCSctpCapabilities;

    namespace internal
    {

      RTCSctpCapabilities^ ToCx(const ISCTPTransportTypes::Capabilities &input);
      RTCSctpCapabilities^ ToCx(ISCTPTransportTypes::CapabilitiesPtr input);
      ISCTPTransportTypes::CapabilitiesPtr FromCx(RTCSctpCapabilities^ input);
    }


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

    public ref struct RTCSctpCapabilities sealed
    {
      property uint32 MaxMessageSize;
      property uint16 MinPort;
      property uint16 MaxPort;
      property uint16 MaxUsablePorts;
      property uint16 MaxSessionsPerPort;

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
        RTCDataChannel^  get() { return m_channel; }
        void  set(RTCDataChannel^ value) { m_channel = value; }
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
      friend ref class RTCDataChannel;

    private:
      RTCSctpTransport();

      static RTCSctpTransport^ Convert(ISCTPTransportPtr transport);
      static ISCTPTransportPtr Convert(RTCSctpTransport^ transport) { if (!transport) return nullptr; return transport->_nativePointer; }

    public:
      RTCSctpTransport(RTCDtlsTransport^ dtlsTransport, uint16 port);

      static RTCSctpCapabilities^ GetCapabilities();
      void                       Start(RTCSctpCapabilities^ remoteCaps);
      void                       Stop();

    public:
      property RTCDtlsTransport^ Transport  { RTCDtlsTransport^ get(); }
      property uint16 Port                  { uint16 get(); }

    public:
      event RTCSctpTransportDataChannelDelegate^            OnSCTPTransportDataChannel;

    private:
      ISCTPTransportPtr _nativePointer;
      RTCSctpTransportDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
