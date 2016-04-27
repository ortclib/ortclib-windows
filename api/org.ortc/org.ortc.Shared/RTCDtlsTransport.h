#pragma once

#include <ortc/ICertificate.h>
#include <ortc/IDTLSTransport.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransportDelegate, IDTLSTransportDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransport, IDTLSTransport)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransportSubscription, IDTLSTransportSubscription)

    using Windows::Foundation::Collections::IVector;

    ref struct RTCDtlsFingerprint;
    ref struct RTCDtlsParameters;

    ref class RTCCertificate;
    ref class RTCIceTransport;
    ref class RTCRtpListener;
    ref class RTCRtpReceiver;
    ref class RTCSctpTransport;
    ref class RTCCertificate;
    ref class RTCDtlsTransport;

    ref struct ErrorEvent;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)

      ZS_DECLARE_TYPEDEF_PTR(::ortc::ICertificateTypes, ICertificateTypes)
      ZS_DECLARE_TYPEDEF_PTR(::ortc::IDTLSTransportTypes, IDTLSTransportTypes)

      RTCDtlsFingerprint^ ToCx(const ICertificateTypes::Fingerprint &input);
      RTCDtlsFingerprint^ ToCx(ICertificateTypes::FingerprintPtr input);
      ICertificateTypes::FingerprintPtr FromCx(RTCDtlsFingerprint^ input);

      RTCDtlsParameters^ ToCx(const IDTLSTransportTypes::Parameters &input);
      RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr parameters);
      IDTLSTransportTypes::ParametersPtr FromCx(RTCDtlsParameters^ parameters);
    }

    /// <summary>
    /// RTCDtlsTransportState indicates the state of the DTLS transport.
    /// </summary>
    public enum class RTCDtlsTransportState
    {
      /// <summary>
      /// The RTCDtlsTransport object has been created and has not started
      /// negotiating yet.
      /// </summary>
      New,
      /// <summary>
      /// DTLS is in the process of negotiating a secure connection and
      /// verifying the remote fingerprint. Once a secure connection is
      /// negotiated (but prior to verification of the remote fingerprint,
      /// enabled by calling start()), incoming data can flow through (and
      /// media, once DTLS-SRTP key derivation is completed).
      /// </summary>
      Connecting,
      /// <summary>
      /// DTLS has completed negotiation of a secure connection but the
      /// fingerprint has not been verified.
      /// </summary>
      Connected,
      /// <summary>
      /// The DTLS connection has been closed intentionally via a call to
      /// Stop() or receipt of a close_notify alert. Calling transport.Stop()
      /// will also result in a transition to the "closed" state.
      /// </summary>
      Closed,
      /// <summary>
      /// The DTLS connection has been closed as the result of an error
      /// (such as receipt of an error alert or a failure to validate the
      /// remote fingerprint).
      /// </summary>
      Failed
    };

    /// <summary>
    /// RTCDtlsRole indicates the role of the DTLS transport.
    /// </summary>
    public enum class RTCDtlsRole
    {
      /// <summary>
      /// The DTLS role is determined based on the resolved ICE role: the
      /// "Controlled" role acts as the DTLS client, the "Controlling" role
      /// acts as the DTLS server. Since RTCDtlsRole is initialized to "Auto"
      /// on construction of an RTCDtlsTransport object,
      /// transport.GetLocalParameters().RTCDtlsRole will have an initial
      /// value of "Auto".
      /// </summary>
      Auto,
      /// <summary>
      /// The DTLS client role. A transition to "Client" will occur if
      /// Start(remoteParameters) is called with remoteParameters.RTCDtlsRole
      /// having a value of "Server". If RTCDtlsRole had previously had a
      /// value of "Server" (e.g. due to the RTCDtlsTransport having
      /// previously received packets from a DTLS client), then the DTLS
      /// session is reset prior to transitioning to the "client" role.
      /// </summary>
      Client,
      /// <summary>
      /// The DTLS server role. If RTCDtlsRole has a value of "Auto" and the
      /// RTCDtlsTransport receives a DTLS client_hello packet, RTCDtlsRole
      /// will transition to "Server", even before start() is called. A
      /// transition from "Auto" to "Server" will also occur if
      /// Start(remoteParameters) is called with remoteParameters.RTCDtlsRole
      /// having a value of "Client".
      /// </summary>
      Server,
    };

    /// <summary>
    /// The RTCDtlsFingerprint object includes the hash function algorithm and
    /// certificate fingerprint as described in [RFC4572].
    /// </summary>
    public ref struct RTCDtlsFingerprint sealed
    {
    public:
      /// <summary>
      /// Gets or sets one of the the hash function algorithms defined in the
      /// 'Hash function Textual Names' registry, initially specified in
      /// [RFC4572] Section 8. As noted in [draft-ietf-rtcweb-jsep] Section
      /// 5.2.1, the digest algorithm used for the fingerprint matches that
      /// used in the certificate signature.
      /// </summary>
      property Platform::String^ Algorithm;
      /// <summary>
      /// The value of the certificate fingerprint in lowercase hex string as
      /// expressed utilizing the syntax of 'fingerprint' in [RFC4572] Section
      /// 5.
      /// </summary>
      property Platform::String^ Value;
    };

    /// <summary>
    /// The RTCDtlsParameters object includes information relating to DTLS
    /// configuration.
    /// </summary>
    public ref struct RTCDtlsParameters sealed
    {
      /// <summary>
      /// Constructs an instance of an RTCDtlsParameters object.
      /// </summary>
      RTCDtlsParameters() { Role = RTCDtlsRole::Auto; }

      /// <summary>
      /// The DTLS role, with a default of Auto.
      /// </summary>
      property RTCDtlsRole                    Role;
      /// <summary>
      /// Sequence of fingerprints, one fingerprint for each certificate.
      /// </summary>
      property IVector<RTCDtlsFingerprint^>^  Fingerprints;

      /// <summary>
      /// Helper method to convert this object to a JSON string.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper method to create an RTCDtlsParameters from a JSON string.
      /// </summary>
      static RTCDtlsParameters^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// The OnStateChange event of the RTCDtlsTransport object uses the
    /// RTCDtlsTransportStateChangedEvent interface.
    /// </summary>
    public ref struct RTCDtlsTransportStateChangedEvent sealed
    {
      friend class internal::RTCDtlsTransportDelegate;

      /// <summary>
      /// Gets the new RTCDtlsTransportState that caused the event.
      /// </summary>
      property RTCDtlsTransportState State
      {
        RTCDtlsTransportState  get() { return _state; }
      }

    private:
      RTCDtlsTransportState _state;
    };


    public delegate void RTCDtlsTransportStateChangedDelegate(RTCDtlsTransportStateChangedEvent^ evt);
    public delegate void RTCDtlsTransportErrorDelegate(ErrorEvent^ evt);

    /// <summary>
    /// The RTCDtlsTransport object includes information relating to Datagram
    /// Transport Layer Security (DTLS) transport.
    /// </summary>
    public ref class RTCDtlsTransport sealed
    {
      friend class internal::RTCDtlsTransportDelegate;

      friend ref class RTCRtpListener;
      friend ref class RTCRtpReceiver;
      friend ref class RTCRtpSender;
      friend ref class RTCSctpTransport;

    private:
      RTCDtlsTransport(IDTLSTransportPtr transport);

      static RTCDtlsTransport^ Convert(IDTLSTransportPtr transport) { if (!transport) return nullptr; return ref new RTCDtlsTransport(transport); }
      static IDTLSTransportPtr Convert(RTCDtlsTransport^ transport) { if (!transport) return nullptr; return transport->_nativePointer; }

    public:
      /// <summary>
      /// A RTCDtlsTransport instance is constructed using an RTCIceTransport
      /// and a sequence of RTCCertificate objects. If certificates is
      /// non-empty, check that the expires attribute of each RTCCertificate
      /// object is in the future. If a certificate has expired, throw an
      /// InvalidParameter exception; otherwise, store the certificates.
      /// </summary>
      RTCDtlsTransport(RTCIceTransport^ transport, IVector<RTCCertificate^>^ certificates);

      /// <summary>
      /// Gets the DTLS parameters of the local RTCDtlsTransport. If multiple
      /// certificates were provided in the constructor, then multiple
      /// fingerprints will be returned, one for each certificate.
      /// </summary>
      RTCDtlsParameters^      GetLocalParameters();
      /// <summary>
      /// Obtain the remote DTLS parameters passed in the Start() method.
      /// Prior to calling start(), null is returned.
      /// </summary>
      RTCDtlsParameters^      GetRemoteParameters();
      /// <summary>
      /// Returns the certificate chain in use by the remote side, with each
      /// certificate encoded in binary Distinguished Encoding Rules (DER)
      /// [X690]. GetRemoteCertificates() returns an empty list prior to
      /// selection of the remote certificate, which is completed once
      /// RTCDtlsTransportState transitions to "Connected".
      /// </summary>
      IVector<Object^>^       GetRemoteCertificates();
      /// <summary>
      /// Start DTLS transport negotiation with the parameters of the remote
      /// DTLS transport, including verification of the remote fingerprint,
      /// then once the DTLS transport session is established, negotiate a
      /// DTLS-SRTP [RFC5764] session to establish keys so as protect media
      /// using SRTP [RFC3711]. Since symmetric RTP [RFC4961] is utilized,
      /// the DTLS-SRTP session is bi-directional.
      /// </summary>
      void                    Start(RTCDtlsParameters^ remoteParameters);
      /// <summary>
      /// Stops and closes the RTCDtlsTransport object. Calling stop() when
      /// state is "closed" has no effect.
      /// </summary>
      void                    Stop();

      /// <summary>
      /// The certificates provided in the constructor.
      /// </summary>
      property IVector<RTCCertificate^>^ Certificates { IVector<RTCCertificate^>^ get(); }
      /// <summary>
      /// The associated RTCIceTransport instance.
      /// </summary>
      property RTCIceTransport^ Transport          { RTCIceTransport^ get(); }
      /// <summary>
      /// The current state of the DTLS transport.
      /// </summary>
      property RTCDtlsTransportState State            { RTCDtlsTransportState get(); }

      /// <summary>
      /// This event handler, uses the RTCDtlsTransportStateChangedEvent
      /// interface. It is called any time the RTCDtlsTransportState changes.
      /// </summary>
      event RTCDtlsTransportStateChangedDelegate^           OnStateChange;
      /// <summary>
      /// This event is be fired on reception of a DTLS error alert.
      /// </summary>
      event RTCDtlsTransportErrorDelegate^                  OnError;

    private:
      IDTLSTransportPtr _nativePointer;
      internal::RTCDtlsTransportDelegatePtr _nativeDelegatePointer;
      IDTLSTransportSubscriptionPtr _nativeSubscriptionPointer;
    };
  }
}
