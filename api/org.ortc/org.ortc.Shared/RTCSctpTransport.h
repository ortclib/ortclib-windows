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

    ref class RTCDataChannel;
    ref class RTCDtlsTransport;
    ref class RTCSctpTransport;

    ref struct RTCSctpCapabilities;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCSctpTransportDelegate)
        
      RTCSctpCapabilities^ ToCx(const ISCTPTransportTypes::Capabilities &input);
      RTCSctpCapabilities^ ToCx(ISCTPTransportTypes::CapabilitiesPtr input);
      ISCTPTransportTypes::CapabilitiesPtr FromCx(RTCSctpCapabilities^ input);
    }


    /// <summary>
    /// </summary>
    public ref struct RTCSctpCapabilities sealed
    {
      /// <summary>
      /// </summary>
      property uint32 MaxMessageSize;
      /// <summary>
      /// </summary>
      property uint16 MinPort;
      /// <summary>
      /// </summary>
      property uint16 MaxPort;
      /// <summary>
      /// </summary>
      property uint16 MaxUsablePorts;
      /// <summary>
      /// </summary>
      property uint16 MaxSessionsPerPort;

      /// <summary>
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// </summary>
      static RTCSctpCapabilities^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// </summary>
    public ref struct RTCDataChannelEvent sealed
    {
      /// <summary>
      /// </summary>
      property RTCDataChannel^ DataChannel
      {
        RTCDataChannel^  get() { return m_channel; }
      }

    private:
      RTCDataChannel^ m_channel;
    };

    public delegate void RTCSctpTransportDataChannelDelegate(RTCDataChannelEvent^ evt);

    /// <summary>
    /// </summary>
    public ref class RTCSctpTransport sealed
    {
      friend class internal::RTCSctpTransportDelegate;
      friend ref class RTCDataChannel;

    private:
      RTCSctpTransport();

      static RTCSctpTransport^ Convert(ISCTPTransportPtr transport);
      static ISCTPTransportPtr Convert(RTCSctpTransport^ transport) { if (!transport) return nullptr; return transport->_nativePointer; }

    public:
      /// <summary>
      /// </summary>
      RTCSctpTransport(RTCDtlsTransport^ dtlsTransport, uint16 port);

      /// <summary>
      /// </summary>
      static RTCSctpCapabilities^ GetCapabilities();
      /// <summary>
      /// </summary>
      void                       Start(RTCSctpCapabilities^ remoteCaps);
      /// <summary>
      /// </summary>
      void                       Stop();

      /// <summary>
      /// </summary>
      property RTCDtlsTransport^ Transport  { RTCDtlsTransport^ get(); }
      /// <summary>
      /// </summary>
      property uint16 Port                  { uint16 get(); }

      /// <summary>
      /// </summary>
      event RTCSctpTransportDataChannelDelegate^            OnSCTPTransportDataChannel;

    private:
      ISCTPTransportPtr _nativePointer;
      internal::RTCSctpTransportDelegatePtr _nativeDelegatePointer;
    };

  } // namespace ortc
} // namespace org
