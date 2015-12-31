#pragma once

#include <ortc/IDTLSTransport.h>
#include <ortc/ICertificate.h>

#include <collection.h>
#include "RTCIceTransport.h"

using namespace ortc;

using Platform::Array;
using Windows::Foundation::Collections::IVector;
using Windows::Foundation::IAsyncOperation;

namespace ortc_winrt_api
{

  ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)
  ZS_DECLARE_CLASS_PTR(RTCGenerateCertificatePromiseObserver)

  ref class RTCDtlsTransport;
  ref class RTCCertificate;

  class RTCDtlsTransportDelegate : public IDtlsTransportDelegate
  {
  public:
    virtual void onDTLSTransportStateChange(
      IDTLSTransportPtr transport,
      IDTLSTransport::States state
      );

    virtual void onDTLSTransportError(
      IDTLSTransportPtr transport,
      ErrorCode errorCode,
      zsLib::String errorReason
      );

    RTCDtlsTransport^ _transport;

    void SetOwnerObject(RTCDtlsTransport^ owner) { _transport = owner; }
  };

  class RTCGenerateCertificatePromiseObserver : public zsLib::IPromiseResolutionDelegate
  {
  public:
    RTCGenerateCertificatePromiseObserver(Concurrency::task_completion_event<RTCCertificate^> tce);

    virtual void onPromiseResolved(PromisePtr promise);
    virtual void onPromiseRejected(PromisePtr promise);

  private:
    Concurrency::task_completion_event<RTCCertificate^> mTce;
  };

  //--------------------------------------------------------------------
  // Helper classes
  //--------------------------------------------------------------------

  public enum class RTCDtlsTransportState
  {
    State_New,
    State_Connecting,
    State_Connected,
    State_Validated,
    State_Closed,
  };

  public enum class RTCDtlsRole
  {
    Role_Auto,
    Role_Client,
    Role_Server,
  };

  public ref class RTCDtlsFingerprint sealed
  {
  public:
    property Platform::String^ Algorithm;
    property Platform::String^ Value;
  };

  public ref class RTCDtlsParameters sealed
  {
  public:
    property RTCDtlsRole                    Role;
    property IVector<RTCDtlsFingerprint^>^  Fingerprints;
  };

  public ref class RTCDtlsTransportError sealed
  {
  public:
    property uint16 ErrorCode;
    property Platform::String^ ErrorReason;
  };

  //------------------------------------------
  // Events and Delegates
  //------------------------------------------

  // State change event and delegate
  public ref class RTCDtlsTransportStateChangeEvent sealed {
  public:
    property RTCDtlsTransportState State
    {
      RTCDtlsTransportState  get(){ return m_state; }
      void  set(RTCDtlsTransportState value){ m_state = value; }
    }

  private:
    RTCDtlsTransportState m_state;
  };

  public delegate void RTCDtlsTransportStateChangedDelegate(RTCDtlsTransportStateChangeEvent^ evt);

  // Error event and delegate
  public ref class RTCDtlsTransportErrorEvent sealed {
  public:
    property RTCDtlsTransportError^ Error
    {
      RTCDtlsTransportError^  get(){ return m_error; }
      void  set(RTCDtlsTransportError^ value){ m_error = value; }
    }

  private:
    RTCDtlsTransportError^ m_error;
  };

  public delegate void RTCDtlsTransportErrorDelegate(RTCDtlsTransportErrorEvent^ evt);

  //------------------------------------------
  // End Events and Delegates
  //------------------------------------------

  public ref class RTCCertificate sealed
  {
    friend class RTCGenerateCertificatePromiseObserver;
    friend class FetchNativePointer;
  public:
    static IAsyncOperation<RTCCertificate^>^ generateCertificate(Platform::String^ algorithmIdentifier);
  private:

    ICertificatePtr mNativePointer;
    ICertificateTypes::PromiseWithCertificatePtr mCertificatePromise;
  };

  ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)

  public ref class RTCDtlsTransport sealed
  {
    friend class RTCDtlsTransportDelegate;
    friend class FetchNativePointer;
  public:
    RTCDtlsTransport();
    RTCDtlsTransport(RTCIceTransport^ transport, RTCCertificate^ certificate);

    RTCDtlsParameters^      getLocalParameters();
    RTCDtlsParameters^      getRemoteParameters();
    IVector<Object^>^       getRemoteCertificates();
    void                    start(RTCDtlsParameters^ remoteParameters);
    void                    stop();
  private:
    IDtlsTransportPtr mNativePointer;
    RTCDtlsTransportDelegatePtr mNativeDelegatePointer;

  public:

    event RTCDtlsTransportStateChangedDelegate^           OnDtlsTransportStateChanged;
    event RTCDtlsTransportErrorDelegate^                  OnDtlsTransportError;
  };
}