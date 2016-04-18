#pragma once

#include <ortc/ICertificate.h>
#include <ortc/IDTLSTransport.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransportDelegate, IDTLSTransportDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransport, IDTLSTransport)

    using Windows::Foundation::Collections::IVector;

    ref struct RTCDtlsFingerprint;
    ref struct RTCDtlsParameters;
    ref class RTCCertificate;
    ref class RTCIceTransport;
    ref class RTCRtpListener;
    ref class RTCRtpReceiver;
    ref class RTCSctpTransport;

    namespace internal
    {
      ZS_DECLARE_TYPEDEF_PTR(::ortc::ICertificateTypes, ICertificateTypes)
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransportTypes, IDTLSTransportTypes)

      RTCDtlsFingerprint^ ToCx(const ICertificateTypes::Fingerprint &input);
      RTCDtlsFingerprint^ ToCx(ICertificateTypes::FingerprintPtr input);
      ICertificateTypes::FingerprintPtr FromCx(RTCDtlsFingerprint^ input);

      RTCDtlsParameters^ ToCx(const IDTLSTransportTypes::Parameters &input);
      RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr parameters);
      IDTLSTransportTypes::ParametersPtr FromCx(RTCDtlsParameters^ parameters);
    }

    ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)

    ref class RTCCertificate;
    ref class RTCDtlsTransport;

    class RTCDtlsTransportDelegate : public IDTLSTransportDelegate
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

    public ref struct RTCDtlsFingerprint sealed
    {
    public:
      property Platform::String^ Algorithm;
      property Platform::String^ Value;
    };

    public ref struct RTCDtlsParameters sealed
    {
    public:
      property RTCDtlsRole                    Role;
      property IVector<RTCDtlsFingerprint^>^  Fingerprints;

    public:
      Platform::String^ ToJsonString();
      static RTCDtlsParameters^ FromJsonString(Platform::String^ jsonString);
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
        RTCDtlsTransportState  get() { return m_state; }
        void  set(RTCDtlsTransportState value) { m_state = value; }
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
        RTCDtlsTransportError^  get() { return m_error; }
        void  set(RTCDtlsTransportError^ value) { m_error = value; }
      }

    private:
      RTCDtlsTransportError^ m_error;
    };

    public delegate void RTCDtlsTransportErrorDelegate(RTCDtlsTransportErrorEvent^ evt);

    //------------------------------------------
    // End Events and Delegates
    //------------------------------------------

    ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)

    public ref class RTCDtlsTransport sealed
    {
      friend class RTCDtlsTransportDelegate;
      friend ref class RTCRtpListener;
      friend ref class RTCRtpReceiver;
      friend ref class RTCRtpSender;
      friend ref class RTCSctpTransport;

    private:
      RTCDtlsTransport();

      static RTCDtlsTransport^ Convert(IDTLSTransportPtr transport);
      static IDTLSTransportPtr Convert(RTCDtlsTransport^ transport) { if (!transport) return nullptr; return transport->_nativePointer; }

    public:
      RTCDtlsTransport(RTCIceTransport^ transport, IVector<RTCCertificate^>^ certificates);

      RTCDtlsParameters^      GetLocalParameters();
      RTCDtlsParameters^      GetRemoteParameters();
      IVector<Object^>^       GetRemoteCertificates();
      void                    Start(RTCDtlsParameters^ remoteParameters);
      void                    Stop();

    public:
      property IVector<RTCCertificate^>^ Certificates { IVector<RTCCertificate^>^ get(); }
      property RTCIceTransport^ IceTransport          { RTCIceTransport^ get(); }
      property RTCDtlsTransportState State            { RTCDtlsTransportState get(); }

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

    private:
      IDTLSTransportPtr _nativePointer;
      RTCDtlsTransportDelegatePtr _nativeDelegatePointer;
    };
  }
}
