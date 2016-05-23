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
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISCTPTransportSubscription, ISCTPTransportSubscription)

    ZS_DECLARE_CLASS_PTR(RTCDataChannelDelegate)

    ref class RTCDataChannel;
    ref class RTCDtlsTransport;
    ref class RTCSctpTransport;
    ref class RTCStatsProvider;

    ref struct RTCSctpCapabilities;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCSctpTransportDelegate)
        
      RTCSctpCapabilities^ ToCx(const ISCTPTransportTypes::Capabilities &input);
      RTCSctpCapabilities^ ToCx(ISCTPTransportTypes::CapabilitiesPtr input);
      ISCTPTransportTypes::CapabilitiesPtr FromCx(RTCSctpCapabilities^ input);
    }

    /// <summary>
    /// RTCSctpTransportState indicates the state of the SCTP transport.
    /// </summary>
    public enum class RTCSctpTransportState
    {
      /// <summary>
      /// The RTCSctpTransport object has been created and has not started
      /// negotiating yet.
      /// </summary>
      New,
      /// <summary>
      /// SCTP is in the process of negotiating an association.
      /// </summary>
      Connecting,
      /// <summary>
      /// SCTP has completed negotiation of an association.
      /// </summary>
      Connected,
      /// <summary>
      /// The SCTP association has been closed intentionally via a call to
      /// Stop() or receipt of a SHUTDOWN or ABORT chunk.
      /// </summary>
      Closed
    };

    /// <summary>
    /// The RTCSctpCapabilities represents object represents the capabilities
    /// of SCTP transport.
    /// </summary>
    public ref struct RTCSctpCapabilities sealed
    {
      /// <summary>
      /// Gets or sets the maximum message size.
      /// </summary>
      property uint32 MaxMessageSize;
      /// <summary>
      /// Gets or sets the minimum SCTP port.
      /// </summary>
      property uint16 MinPort;
      /// <summary>
      /// Gets or sets the maximum SCTP port.
      /// </summary>
      property uint16 MaxPort;
      /// <summary>
      /// Gets or sets the maximum SCTP ports that can be used simultaneously.
      /// </summary>
      property uint16 MaxUsablePorts;
      /// <summary>
      /// Gets or sets the maximum sessions allowed per port.
      /// </summary>
      property uint16 MaxSessionsPerPort;

      /// <summary>
      /// Helper routine to convert this object to a JSON blob string.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routine to convert from a JSON blob string to an
      /// RTCSctpCapabilities object.
      /// </summary>
      static RTCSctpCapabilities^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// If the remote peers sets RTCDataChannelParameters.Negotiated to false,
    /// then the event will fire indicating a new RTCDataChannel object has
    /// been constructed to connect with the RTCDataChannel constructed by the
    /// remote peer.
    /// </summary>
    public ref struct RTCDataChannelEvent sealed
    {
      friend class internal::RTCSctpTransportDelegate;

      /// <summary>
      /// Gets the RTCDataChannel that was constructed when upon receiving a
      /// new RTCDataChannel from a remote peer.
      /// </summary>
      property RTCDataChannel^ DataChannel
      {
        RTCDataChannel^  get() { return _channel; }
      }

    private:
      RTCDataChannel^ _channel;
    };

    /// <summary>
    /// RTCSctpTransportStateChangeEvent represents the state event
    /// fired when the RTCSctpTransport state is changed.
    /// </summary>
    public ref struct RTCSctpTransportStateChangeEvent sealed
    {
      friend class internal::RTCSctpTransportDelegate;

      /// <summary>
      /// The RTCSctpTransportState that cased the event to fire.
      /// </summary>
      property RTCSctpTransportState State
      {
        RTCSctpTransportState  get() { return _state; }
      }

    private:
      RTCSctpTransportState _state;
    };


    public delegate void RTCSctpTransportDataChannelDelegate(RTCDataChannelEvent^ evt);
    public delegate void RTCSctpTransportStateChangeDelegate(RTCSctpTransportStateChangeEvent^ evt);

    /// <summary>
    /// The RTCSctpTransport includes information relating to Stream Control
    /// Transmission Protocol (SCTP) transport.
    /// </summary>
    public ref class RTCSctpTransport sealed
    {
      friend class internal::RTCSctpTransportDelegate;
      friend ref class RTCDataChannel;
      friend ref class RTCStatsProvider;

    private:
      RTCSctpTransport(ISCTPTransportPtr transport);

      static RTCSctpTransport^ Convert(ISCTPTransportPtr transport) { if (!transport) return nullptr; return ref new RTCSctpTransport(transport); }
      static ISCTPTransportPtr Convert(RTCSctpTransport^ transport) { if (!transport) return nullptr; return transport->_nativePointer; }

    public:
      /// <summary>
      /// Constructs an instance of RTCSctpTransport from an RTCDtlsTransport
      /// object (with a default of 5000, or the next unused port).
      /// </summary>
      RTCSctpTransport(RTCDtlsTransport^ dtlsTransport);
      /// <summary>
      /// Constructs an instance of RTCSctpTransport from an RTCDtlsTransport
      /// object, and a port number.
      /// </summary>
      RTCSctpTransport(RTCDtlsTransport^ dtlsTransport, uint16 port);

      /// <summary>
      /// Retrieves the RTCSctpCapabilities of the RTCSctpTransport instance.
      /// </summary>
      static RTCSctpCapabilities^ GetCapabilities();
      /// <summary>
      /// Starts the SCTP transport providing the capabilities of remote
      /// SCTP transport.
      /// </summary>
      void                       Start(RTCSctpCapabilities^ remoteCaps);
      /// <summary>
      /// Stops the RTCSctpTransport instance.
      /// </summary>
      void                       Stop();

      /// <summary>
      /// Gets the RTCDtlsTransport instance the RTCSctpTransport object is
      /// sending over.
      /// </summary>
      property RTCDtlsTransport^ Transport  { RTCDtlsTransport^ get(); }
      /// <summary>
      /// Gets the SCTP port number used by the data channel.
      /// </summary>
      property uint16 Port                  { uint16 get(); }

      /// <summary>
      /// If the remote peers sets RTCDataChannelParameters.negotiated to
      /// false, then this event will fire indicating a new RTCDataChannel
      /// object has been constructed to connect with the RTCDataChannel
      /// constructed by the remote peer.
      /// </summary>
      event RTCSctpTransportDataChannelDelegate^            OnDataChannel;

      event RTCSctpTransportStateChangeDelegate^            OnStateChange;

    private:
      ISCTPTransportPtr _nativePointer;
      internal::RTCSctpTransportDelegatePtr _nativeDelegatePointer;
      ISCTPTransportSubscriptionPtr _nativeSubscriptionPointer;
    };

  } // namespace ortc
} // namespace org
