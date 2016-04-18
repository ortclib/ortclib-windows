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

    ref class RTCDataChannelParameters;
    enum class RTCDataChannelState;

    namespace internal
    {
      RTCDataChannelParameters^ ToCx(const IDataChannelTypes::Parameters &input);
      RTCDataChannelParameters^ ToCx(IDataChannelTypes::ParametersPtr input);
      IDataChannelTypes::ParametersPtr FromCx(RTCDataChannelParameters^ input);
    }

    ZS_DECLARE_CLASS_PTR(RTCDataChannelDelegate)
    ref class RTCDataChannel;
    ref class RTCSctpTransport;

    class RTCDataChannelDelegate : public IDataChannelDelegate
    {
    public:
      virtual void onDataChannelStateChanged(
        IDataChannelPtr channel,
        States state
        ) override;

      virtual void onDataChannelError(
        IDataChannelPtr channel,
        ErrorCode errorCode,
        zsLib::String errorReason
        ) override;

      virtual void onDataChannelBufferedAmountLow(
        IDataChannelPtr channel
        ) override;

      virtual void onDataChannelMessage(
        IDataChannelPtr channel,
        MessageEventDataPtr data
        ) override;

      RTCDataChannel^ _channel;

      void SetOwnerObject(RTCDataChannel^ owner) { _channel = owner; }
    };

    //--------------------------------------------------------------------
    // Helper classes
    //--------------------------------------------------------------------
    public ref class RTCDataChannelParameters sealed
    {
    public:
      property Platform::String^        Label;
      property Platform::Boolean        Ordered;
      property uint64                   MaxPacketLifetime;
      property Platform::IBox<uint32>^  MaxRetransmits;
      property Platform::String^        Protocol;
      property Platform::Boolean        Negotiated;
      property Platform::IBox<uint16>^  Id;
    public:
      Platform::String^ ToJsonString();
      static RTCDataChannelParameters^ FromJsonString(Platform::String^ jsonString);
    };

    public enum class RTCDataChannelState
    {
      Connecting,
      Open,
      Closing,
      Closed,
    };

    public ref class RTCDataChannelError sealed
    {
    public:
      property uint16             ErrorCode;
      property Platform::String^  ErrorReason;
    };

    public ref class RTCMessageEventData sealed
    {
    public:
      property Platform::Array<byte>^ Binary;
      property Platform::String^      Text;
    };

    //------------------------------------------
    // Events and Delegates
    //------------------------------------------
    // State change event and delegate
    public ref class RTCDataChannelStateChangeEvent sealed {
    public:
      property RTCDataChannelState State
      {
        RTCDataChannelState  get() { return _state; }
        void  set(RTCDataChannelState value) { _state = value; }
      }

    private:
      RTCDataChannelState _state;
    };

    public delegate void RTCDataChannelStateChangedDelegate(RTCDataChannelStateChangeEvent^ evt);

    // Error event and delegate
    public ref class RTCDataChannelErrorEvent sealed {
    public:
      property RTCDataChannelError^ Error
      {
        RTCDataChannelError^  get() { return m_error; }
        void  set(RTCDataChannelError^ value) { m_error = value; }
      }

    private:
      RTCDataChannelError^ m_error;
    };

    public delegate void RTCDataChannelErrorDelegate(RTCDataChannelErrorEvent^ evt);

    // Buffered amount low event data event and delegate
    public ref class RTCDataChannelBufferedAmountLowEvent sealed {
    public:
    private:
    };

    public delegate void RTCDataChannelBufferedAmountLowDelegate(RTCDataChannelBufferedAmountLowEvent^ evt);

    // Message event data event and delegate
    public ref class RTCMessageEvent sealed {
    public:
      property RTCMessageEventData^ Data
      {
        RTCMessageEventData^  get() { return m_data; }
        void  set(RTCMessageEventData^ value) { m_data = value; }
      }

    private:
      RTCMessageEventData^ m_data;
    };

    public delegate void RTCDataChannelMessageEventDataDelegate(RTCMessageEvent^ evt);
    //------------------------------------------
    // End Events and Delegates
    //------------------------------------------


    public ref class RTCDataChannel sealed
    {
      friend class RTCDataChannelDelegate;
      friend class RTCSctpTransportDelegate;

    private:
      RTCDataChannel();
    public:
      RTCDataChannel(RTCSctpTransport^ dataTransport, RTCDataChannelParameters^ params);

      void Close();

      [Windows::Foundation::Metadata::OverloadAttribute("SendString")]
      void Send(Platform::String^ data);
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      void Send(const Platform::Array<byte>^ data);
      [Windows::Foundation::Metadata::OverloadAttribute("SendWithSize")]
      void Send(const Platform::Array<byte>^ data, uint16 bufferSizeInBytes);

    public:
      property RTCSctpTransport^ Transport { RTCSctpTransport^ get(); }
      property RTCDataChannelParameters^ Parameters { RTCDataChannelParameters^ get(); }
      property RTCDataChannelState State { RTCDataChannelState get(); }
      property uint64 BufferedAmount { uint64 get(); }
      property uint64 BufferedAmountLowThreshold
      {
        uint64 get();
        void set(uint64 threshold);
      }
      property Platform::String^ BinaryType
      {
        Platform::String^ get();
        void set(Platform::String^ binaryType);
      }

    public:

      event RTCDataChannelStateChangedDelegate^            OnDataChannelStateChanged;
      event RTCDataChannelErrorDelegate^                   OnDataChannelError;
      event RTCDataChannelBufferedAmountLowDelegate^       OnDataChannelBufferedAmountLow;
      event RTCDataChannelMessageEventDataDelegate^        OnDataChannelMessage;

    private:
      IDataChannelPtr _nativePointer;
      RTCDataChannelDelegatePtr _nativeDelegatePointer;
    };
  }
}
