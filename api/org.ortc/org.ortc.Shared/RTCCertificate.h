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
    ZS_DECLARE_CLASS_PTR(RTCGenerateCertificatePromiseObserver)

    using Windows::Foundation::IAsyncOperation;

    ref class RTCCertificate;
    ref struct RTCDtlsFingerprint;

    class RTCGenerateCertificatePromiseObserver : public zsLib::IPromiseSettledDelegate
    {
    public:
      RTCGenerateCertificatePromiseObserver(Concurrency::task_completion_event<RTCCertificate^> tce);

      virtual void onPromiseSettled(PromisePtr promise);

    private:
      Concurrency::task_completion_event<RTCCertificate^> mTce;
    };

    //--------------------------------------------------------------------
    // Helper classes
    //--------------------------------------------------------------------

    //------------------------------------------
    // Events and Delegates
    //------------------------------------------

    //------------------------------------------
    // End Events and Delegates
    //------------------------------------------

    public ref class RTCCertificate sealed
    {
    private:
      friend class RTCGenerateCertificatePromiseObserver;
      friend ref class RTCDtlsTransport;

    private:
      static RTCCertificate^ Convert(ICertificatePtr certificate);
      static ICertificatePtr Convert(RTCCertificate^ certificate) { if (!certificate) return nullptr; return certificate->_nativePointer; }

    public:
      static IAsyncOperation<RTCCertificate^>^ GenerateCertificate();
      static IAsyncOperation<RTCCertificate^>^ GenerateCertificate(Platform::String^ algorithmIdentifier);

    private:
      Windows::Foundation::DateTime GetExpires();
      RTCDtlsFingerprint^ GetFingerprint();

    public:
      property Windows::Foundation::DateTime Expires
      {
        Windows::Foundation::DateTime get()
        {
          if (_nativePointer)
            return GetExpires();
          else
            return Windows::Foundation::DateTime();
        }
      }

      property RTCDtlsFingerprint^ Fingerprint
      {
        RTCDtlsFingerprint^ get()
        {
          if (_nativePointer)
            return GetFingerprint();
          else
            return nullptr;
        }
      }

    private:
      ICertificatePtr _nativePointer;
      ICertificateTypes::PromiseWithCertificatePtr _certificatePromise;
    };

  }
}
