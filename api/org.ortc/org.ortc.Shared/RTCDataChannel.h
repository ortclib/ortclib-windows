#pragma once

#include <collection.h>
#include <ortc/IDataChannel.h>
#include "RTCSctpTransport.h"

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ref class RTCDataChannelParameters;

    namespace internal
    {
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
      property uint16 ErrorCode;
      property Platform::String^ ErrorReason;
    };

    public ref class RTCMessageEventData sealed
    {
    public:
      property Array<byte>^      Binary;
      property Platform::String^ Text;
    };

    //------------------------------------------
    // Events and Delegates
    //------------------------------------------
    // State change event and delegate
    public ref class RTCDataChannelStateChangeEvent sealed {
    public:
      property RTCDataChannelState State
      {
        RTCDataChannelState  get() { return m_state; }
        void  set(RTCDataChannelState value) { m_state = value; }
      }

    private:
      RTCDataChannelState m_state;
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
      friend class FetchNativePointer;
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

    private:
      IDataChannelPtr mNativePointer;
      RTCDataChannelDelegatePtr mNativeDelegatePointer;

    private:
      RTCSctpTransport^ GetSctpTransport();
      RTCDataChannelParameters^ GetParameters();
      Platform::String^ GetBinaryType();
      void SetBinaryType(Platform::String^ binaryType);

    public:
      property RTCSctpTransport^ Transport
      {
        RTCSctpTransport^ get()
        {
          if (mNativePointer) return GetSctpTransport();
          return nullptr;
        }
      }

      property RTCDataChannelParameters^ Parameters
      {
        RTCDataChannelParameters^ get()
        {
          if (mNativePointer) return GetParameters();
          return nullptr;
        }
      }

      property RTCDataChannelState State
      {
        RTCDataChannelState get();
      }

      property uint64 BufferedAmount
      {
        uint64 get()
        {
          if (mNativePointer) return mNativePointer->bufferedAmount();
          return 0;
        }
      }

      property uint64 BufferedAmountLowThreshold
      {
        uint64 get()
        {
          if (mNativePointer) return mNativePointer->bufferedAmountLowThreshold();
          return 0;
        }

        void set(uint64 threshold)
        {
          if (mNativePointer) mNativePointer->bufferedAmountLowThreshold(static_cast<size_t>(threshold));
        }
      }

      property Platform::String^ BinaryType
      {
        Platform::String^ get()
        {
          if (mNativePointer) return GetBinaryType();
          return nullptr;
        }

        void set(Platform::String^ binaryType)
        {
          if (mNativePointer) return SetBinaryType(binaryType);
          return;
        }
      }

    public:

      event RTCDataChannelStateChangedDelegate^            OnDataChannelStateChanged;
      event RTCDataChannelErrorDelegate^                   OnDataChannelError;
      event RTCDataChannelBufferedAmountLowDelegate^       OnDataChannelBufferedAmountLow;
      event RTCDataChannelMessageEventDataDelegate^        OnDataChannelMessage;
    };
  }
}
