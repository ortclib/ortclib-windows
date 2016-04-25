#pragma once

#include <ortc/IDataChannel.h>

#include <collection.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDataChannelTypes, IDataChannelTypes)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDataChannelDelegate, IDataChannelDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDataChannel, IDataChannel)

    ref class RTCDataChannel;
    ref class RTCSctpTransport;
    ref class RTCDataChannelParameters;
    ref struct ErrorEvent;

    enum class RTCDataChannelState;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCDataChannelDelegate)

      RTCDataChannelParameters^ ToCx(const IDataChannelTypes::Parameters &input);
      RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr input);
      IDataChannelTypes::ParametersPtr FromCx(RTCDataChannelParameters^ input);
    }

    /// <summary>
    /// An RTCDataChannel can be configured to operate in different
    /// reliability modes. A reliable channel ensures that the data is
    /// delivered at the other peer through retransmissions. An unreliable
    /// channel is configured to either limit the number of retransmissions
    /// (maxRetransmits ) or set a time during which transmissions (including
    /// retransmissions) are allowed (maxPacketLifeTime). These properties
    /// can not be used simultaneously and an attempt to do so will result in
    /// an error. Not setting any of these properties results in a reliable
    /// channel.
    /// </summary>
    public ref struct RTCDataChannelParameters sealed
    {
      /// <summary>
      /// Gets or sets a label that can be used to distinguish this
      /// RTCDataChannel object from other RTCDataChannel objects.
      /// </summary>
      property Platform::String^        Label;
      /// <summary>
      /// Gets or sets the if the RTCDataChannel is ordered, and false if out
      /// of order delivery is allowed.
      /// </summary>
      property Platform::Boolean        Ordered;
      /// <summary>
      /// Gets or sets the maxPacketLifetime attribute represents the length
      /// of the time window (in milliseconds) during which retransmissions
      /// may occur in unreliable mode. A value of 0 means not used.
      /// </summary>
      property uint64                   MaxPacketLifetime;
      /// <summary>
      /// Gets or sets the maxRetransmits attribute returns the maximum number
      /// of retransmissions that are attempted in unreliable mode if set.
      /// </summary>
      property Platform::IBox<uint32>^  MaxRetransmits;
      /// <summary>
      /// </summary>
      property Platform::String^        Protocol;
      /// <summary>
      /// Gets or sets an indicator if the this RTCDataChannel was negotiated
      /// by the application. If true, the application negotiated the
      /// RTCDataChannel, otherwise the value is false.
      /// </summary>
      property Platform::Boolean        Negotiated;
      /// <summary>
      /// The id attribute returns the id for this RTCDataChannel if set.
      /// </summary>
      property Platform::IBox<uint16>^  Id;

      /// <summary>
      /// Helper routine to convert this object to a JSON blob string.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routine to convert from a JSON blob string to an 
      /// RTCDataChannelParameters instance.
      /// </summary>
      static RTCDataChannelParameters^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// RTCDataChannelState represents the state of the RTCDataChannel object. 
    /// </summary>
    public enum class RTCDataChannelState
    {
      /// <summary>
      /// The user agent is attempting to establish the underlying data
      /// transport. This is the initial state of an RTCDataChannel object.
      /// </summary>
      Connecting,
      /// <summary>
      /// The underlying data transport is established and communication is
      /// possible. This is the initial state of an RTCDataChannel object
      /// dispatched as a part of an RTCDataChannelEvent.
      /// </summary>
      Open,
      /// <summary>
      /// The procedure to close down the underlying data transport has
      /// started.
      /// </summary>
      Closing,
      /// <summary>
      /// The underlying data transport has been closed or could not be
      /// established.
      /// </summary>
      Closed,
    };

    /// <summary>
    /// RTCMessageEventData represents data arriving from the RTCDataChannel.
    /// </summary>
    public ref struct RTCMessageEventData sealed
    {
      friend class internal::RTCDataChannelDelegate;

      /// <summary>
      /// Gets the binary message being sent. Only set if the data was binary.
      /// </summary>
      property Platform::Array<byte>^ Binary
      {
        Platform::Array<byte>^ get() { return _binary; }
      }
      /// <summary>
      /// Gets the binary message being sent. Only set if the data was text.
      /// </summary>
      property Platform::String^      Text
      {
        Platform::String^ get() { return _text; }
      }

    private:
      Platform::Array<byte>^ _binary;
      Platform::String^ _text;
    };

    /// <summary>
    /// RTCDataChannelStateChangeEvent represents the ready state event
    /// fired when the RTCDataChannel state is changed.
    /// </summary>
    public ref struct RTCDataChannelStateChangeEvent sealed
    {
      friend class internal::RTCDataChannelDelegate;

      /// <summary>
      /// The RTCDataChannelState that cased the event to fire.
      /// </summary>
      property RTCDataChannelState State
      {
        RTCDataChannelState  get() { return _state; }
      }

    private:
      RTCDataChannelState _state;
    };

    /// <summary>
    /// RTCMessageEvent represents the event data when a message from the
    /// RTCDataChannel is fired.
    /// </summary>
    public ref struct RTCMessageEvent sealed
    {
      friend class internal::RTCDataChannelDelegate;

      /// <summary>
      /// Gets the binary message being sent.
      /// </summary>
      property RTCMessageEventData^ Data
      {
        RTCMessageEventData^  get() { return _data; }
      }

    private:
      RTCMessageEventData^ _data;
    };

    public delegate void RTCDataChannelErrorDelegate(ErrorEvent^ evt);
    public delegate void RTCDataChannelStateChangedDelegate(RTCDataChannelStateChangeEvent^ evt);
    public delegate void RTCDataChannelBufferedAmountLowDelegate();
    public delegate void RTCDataChannelMessageEventDataDelegate(RTCMessageEvent^ evt);


    /// <summary>
    /// An RTCDataChannel class instance allows sending data messages to/from
    /// the remote peer.
    /// </summary>
    public ref class RTCDataChannel sealed
    {
      friend class internal::RTCDataChannelDelegate;
      friend class RTCSctpTransportDelegate;

    private:
      RTCDataChannel();

    public:
      /// <summary>
      /// Constructs an instance of RTCDataChannel object from a
      /// RTCDataTransport object and an RTCDataChannelParameters object.
      /// </summary>
      RTCDataChannel(RTCSctpTransport^ dataTransport, RTCDataChannelParameters^ params);

      /// <summary>
      /// Closes the RTCDataChannel.
      /// </summary>
      void Close();

      /// <summary>
      /// The Send() method is overloaded to handle different data argument
      /// types. This Send() method transmits a string to the remote peer.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SendString")]
      void Send(Platform::String^ data);
      /// <summary>
      /// The Send() method is overloaded to handle different data argument
      /// types. This Send() method transmits a binary blob to the remote
      /// peer.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void Send(const Platform::Array<byte>^ data);
      /// <summary>
      /// The Send() method is overloaded to handle different data argument
      /// types. This Send() method transmits a binary array of a set length
      /// to the remote
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SendWithSize")]
      void Send(const Platform::Array<byte>^ data, uint16 bufferSizeInBytes);

      /// <summary>
      /// Gets or sets the related transport object.
      /// </summary>
      property RTCSctpTransport^ Transport { RTCSctpTransport^ get(); }
      /// <summary>
      /// Gets or sets the parameters applying to this data channel.
      /// </summary>
      property RTCDataChannelParameters^ Parameters { RTCDataChannelParameters^ get(); }
      /// <summary>
      /// Gets or sets the state of the RTCDataChannel object. It must return
      /// the value to which the user agent last set it (as defined by the
      /// processing model algorithms).
      /// </summary>
      property RTCDataChannelState State { RTCDataChannelState get(); }
      /// <summary>
      /// Gets the number of bytes of application data (UTF-8 text and binary
      /// data) that have been queued using send() but that, as of the last
      /// time the event loop started executing a task, had not yet been
      /// transmitted to the network.
      /// </summary>
      property uint64 BufferedAmount { uint64 get(); }
      /// <summary>
      /// Gets or sets the threshold at which the BufferedAmount is considered
      /// to be low. When the bufferedAmount decreases from above this
      /// threshold to equal or below it, the bufferedamountlow event fires.
      /// The bufferedAmountLowThreshold is initially zero on each new
      /// RTCDataChannel, but the application may change its value at any
      /// time.
      /// </summary>
      property uint64 BufferedAmountLowThreshold
      {
        uint64 get();
        void set(uint64 threshold);
      }
      /// <summary>
      /// Gets or sets the binaryType attribute. On getting, return the value
      /// to which it was last set. On setting, the user agent must set the
      /// IDL attribute to the new value. When an RTCDataChannel object is
      /// constructed, the binaryType attribute must be initialized to the
      /// string 'blob'. This attribute controls how binary data is exposed to
      /// scripts.
      /// </summary>
      property Platform::String^ BinaryType
      {
        Platform::String^ get();
        void set(Platform::String^ binaryType);
      }

      /// <summary>
      /// The event handler when the state of the RTCDataChannel object
      //// changes.
      /// </summary>
      event RTCDataChannelStateChangedDelegate^            OnStateChange;
      /// <summary>
      /// The error event handler for the RTCDataChannel.
      /// </summary>
      event RTCDataChannelErrorDelegate^                   OnError;
      /// <summary>
      /// The event handler when the BufferedAmount falls below the
      /// BufferedAmountLowThreshold.
      /// </summary>
      event RTCDataChannelBufferedAmountLowDelegate^       OnBufferedAmountLow;
      /// <summary>
      /// The event handler when data from the remote peer arrives.
      /// </summary>
      event RTCDataChannelMessageEventDataDelegate^        OnMessage;

    private:
      IDataChannelPtr _nativePointer;
      internal::RTCDataChannelDelegatePtr _nativeDelegatePointer;
    };
  }
}
