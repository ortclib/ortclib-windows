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
    New,
    Connecting,
    Connected,
    Validated,
    Closed,
  };

  public enum class RTCDtlsRole
  {
    Auto,
    Client,
    Server,
  };

  public ref class RTCDtlsFingerprint sealed
  {
  public:
    property Platform::String^ Algorithm;
    property Platform::String^ Value;
  };

  public ref class RTCDtlsParameters sealed
  {
    friend class PushNativePointer;
  public:
    property RTCDtlsRole                    Role;
    property IVector<RTCDtlsFingerprint^>^  Fingerprints;

  public:
    Platform::String^ ToJsonString();
    static RTCDtlsParameters^ FromJsonString(Platform::String^ jsonString);

  private:
    IDtlsTransport::ParametersPtr mNativePointer;
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
    friend class ConvertObjectToCx;
  public:
    static IAsyncOperation<RTCCertificate^>^ GenerateCertificate(Platform::String^ algorithmIdentifier);
  private:

    ICertificatePtr mNativePointer;
    ICertificateTypes::PromiseWithCertificatePtr mCertificatePromise;
  };

  ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)

  public ref class RTCDtlsTransport sealed
  {
    friend class RTCDtlsTransportDelegate;
    friend class FetchNativePointer;
    friend class ConvertObjectToCx;
  private:
    RTCDtlsTransport();
  public:
    RTCDtlsTransport(RTCIceTransport^ transport, RTCCertificate^ certificate);

    RTCDtlsParameters^      GetLocalParameters();
    RTCDtlsParameters^      GetRemoteParameters();
    IVector<Object^>^       GetRemoteCertificates();
    void                    Start(RTCDtlsParameters^ remoteParameters);
    void                    Stop();

  private:
    IDtlsTransportPtr mNativePointer;
    RTCDtlsTransportDelegatePtr mNativeDelegatePointer;

  private:
    RTCIceTransport^ GetIceTransport();
    IVector<RTCCertificate^>^ GetCertificates();

  public:
    property IVector<RTCCertificate^>^ Certificates
    {
      IVector<RTCCertificate^>^ get()
      {
        if (mNativePointer)
          return GetCertificates();
        else
          return nullptr;
      }
    }

    property RTCIceTransport^ IceTransport
    {
      RTCIceTransport^ get()
      {
        if (mNativePointer)
          return GetIceTransport();
        else
          return nullptr;
      }
    }

    property RTCDtlsTransportState State
    {
      RTCDtlsTransportState get();
    }
  public:
    event RTCDtlsTransportStateChangedDelegate^           OnDtlsTransportStateChanged;
    event RTCDtlsTransportErrorDelegate^                  OnDtlsTransportError;

  public:
    [Windows::Foundation::Metadata::DefaultOverloadAttribute]
    static Platform::String^ ToString();
    [Windows::Foundation::Metadata::OverloadAttribute("DtlsTransportStateToString")]
    static Platform::String^ ToString(RTCDtlsTransportState value);
    [Windows::Foundation::Metadata::OverloadAttribute("DtlsTransportRoleToString")]
    static Platform::String^ ToString(RTCDtlsRole value);

    static RTCDtlsTransportState ToState(Platform::String^ str);
    static RTCDtlsRole ToRole(Platform::String^ str);
  };
}