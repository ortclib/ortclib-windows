#pragma once

#include <ortc/ICertificate.h>
#include <ortc/IDTLSTransport.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(zsLib::Promise, Promise)

    ZS_DECLARE_TYPEDEF_PTR(::ortc::ICertificate, ICertificate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ICertificateTypes, ICertificateTypes)

    ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate)

    using Windows::Foundation::IAsyncOperation;

    ref class RTCCertificate;
    ref struct RTCDtlsFingerprint;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCGenerateCertificatePromiseObserver)
    } // namespace internal

    /// <summary>
    /// The RTCCertificate interface enables the certificates used by an
    /// RTCDtlsTransport in its constructor. 
    /// </summary>
    public ref class RTCCertificate sealed
    {
    private:
      friend class internal::RTCGenerateCertificatePromiseObserver;
      friend ref class RTCDtlsTransport;

    private:
      RTCCertificate(ICertificatePtr certificate);

      static RTCCertificate^ Convert(ICertificatePtr certificate);
      static ICertificatePtr Convert(RTCCertificate^ certificate) { if (!certificate) return nullptr; return certificate->_nativePointer; }

    public:
      /// <summary>
      /// The GenerateCertificate method causes the user agent to create and
      /// store an X.509 certificate [ISO/IEC 9594-8:1997] and corresponding
      /// private key. 
      /// </summary>
      /// <returns>An awaitable newly generated certificate.</returns>
      static IAsyncOperation<RTCCertificate^>^ GenerateCertificate();
      /// <summary>
      /// The GenerateCertificate method causes the user agent to create and
      /// store an X.509 certificate [ISO/IEC 9594-8:1997] and corresponding
      /// private key. 
      /// </summary>
      /// <param name='keygenAlgorithm'>
      //// This argument is used to control how the private key associated
      /// with the certificate is generated.
      /// </param>
      /// <returns>An awaitable newly generated certificate.</returns>
      static IAsyncOperation<RTCCertificate^>^ GenerateCertificate(Platform::String^ algorithmIdentifier);

      /// <summary>
      /// Gets the expiry after which the certificate will be considered
      /// invalid by the browser. After this time, attempts to construct an
      /// RTCDtlsTransport object using this certificate will fail.
      /// </summary>
      property Windows::Foundation::DateTime Expires
      {
        Windows::Foundation::DateTime get();
      }

      /// <summary>
      /// Gets the fingerprint of the certificate. As noted in
      /// [draft-ietf-rtcweb-jsep] Section 5.2.1, the digest algorithm used
      /// for the fingerprint matches that used in the certificate signature.
      /// </summary>
      property RTCDtlsFingerprint^ Fingerprint
      {
        RTCDtlsFingerprint^ get();
      }

    private:
      ICertificatePtr _nativePointer;
    };

  }
}
