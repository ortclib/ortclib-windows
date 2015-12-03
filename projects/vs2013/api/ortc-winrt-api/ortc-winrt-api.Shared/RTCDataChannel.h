#pragma once

#include <collection.h>
#include <ortc/IDataChannel.h>
#include "RTCSctpTransport.h"

using namespace ortc;

namespace ortc_winrt_api
{
  ZS_DECLARE_CLASS_PTR(RTCDataChannelDelegate)
  ref class RTCDataChannel;
  ref class RTCSctpTransport;

  class RTCDataChannelDelegate : public IDataChannelDelegate
  {
  public:
    virtual void onDataChannelStateChanged(
      IDataChannelPtr channel,
      States state
      );

    virtual void onDataChannelError(
      IDataChannelPtr channel,
      ErrorCode errorCode,
      String errorReason
      );

    virtual void onDataChannelMessage(
      IDataChannelPtr channel,
      MessageEventDataPtr data
      );

    RTCDataChannel^ _channel;

    void SetOwnerObject(RTCDataChannel^ owner) { _channel = owner; }
  };

  //--------------------------------------------------------------------
  // Helper classes
  //--------------------------------------------------------------------
  public ref class RTCDataChannelParameters sealed
  {
  public:
    property Platform::String^   Label;
    property boolean             Ordered;
    property uint64              MaxPacketLifetime;
    property uint64              MaxRetransmits;
    property Platform::String^   Protocol;
    property boolean             Negotiated;
    property uint16              Id;
  };

  public enum class RTCDataChannelState
  {
    State_Connecting,
    State_Open,
    State_Closing,
    State_Closed,
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
      RTCDataChannelState  get(){ return m_state; }
      void  set(RTCDataChannelState value){ m_state = value; }
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
      RTCDataChannelError^  get(){ return m_error; }
      void  set(RTCDataChannelError^ value){ m_error = value; }
    }

  private:
    RTCDataChannelError^ m_error;
  };

  public delegate void RTCDataChannelErrorDelegate(RTCDataChannelErrorEvent^ evt);

  // Message event data event and delegate
  public ref class RTCMessageEventDataEvent sealed {
  public:
    property RTCMessageEventData^ MessageData
    {
      RTCMessageEventData^  get(){ return m_dataEvent; }
      void  set(RTCMessageEventData^ value){ m_dataEvent = value; }
    }

  private:
    RTCMessageEventData^ m_dataEvent;
  };

  public delegate void RTCDataChannelMessageEventDataDelegate(RTCMessageEventDataEvent^ evt);
  //------------------------------------------
  // End Events and Delegates
  //------------------------------------------


  public ref class RTCDataChannel sealed
  {
    friend class RTCDataChannelDelegate;
    friend class RTCSctpTransportDelegate;
    friend class FetchNativePointer;
  public:
    RTCDataChannel();
    RTCDataChannel(RTCSctpTransport^ dataTransport, RTCDataChannelParameters^ params);

    void close();

    void send(Platform::String^ data);
    void send(const Platform::Array<byte>^ data);
    void send(const Platform::Array<byte>^ data, uint16 bufferSizeInBytes);

  private:
    IDataChannelPtr mNativePointer;
    RTCDataChannelDelegatePtr mNativeDelegatePointer;

  public:

    event RTCDataChannelStateChangedDelegate^            OnDataChannelStateChanged;
    event RTCDataChannelErrorDelegate^                   OnDataChannelError;
    event RTCDataChannelMessageEventDataDelegate^        OnDataChannelMessage;
  };

}