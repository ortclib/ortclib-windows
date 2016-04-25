#include "pch.h"

#include "RTCCertificate.h"
#include "RTCDtlsTransport.h"
#include "helpers.h"
#include "Error.h"

using namespace ortc;

namespace org
{
  namespace ortc
  {
    using std::make_shared;

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {
      class RTCGenerateCertificatePromiseObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        RTCGenerateCertificatePromiseObserver(Concurrency::task_completion_event<RTCCertificate^> tce);

        virtual void onPromiseResolved(PromisePtr promise);
        virtual void onPromiseRejected(PromisePtr promise);

      private:
        Concurrency::task_completion_event<RTCCertificate^> mTce;
      };

      RTCGenerateCertificatePromiseObserver::RTCGenerateCertificatePromiseObserver(Concurrency::task_completion_event<RTCCertificate^> tce) :
        mTce(tce)
      {
      }

      void RTCGenerateCertificatePromiseObserver::onPromiseResolved(PromisePtr promise)
      {
        ICertificate::PromiseWithCertificatePtr certificatePromise = ZS_DYNAMIC_PTR_CAST(ICertificate::PromiseWithCertificate, promise);
        auto nativeCert = certificatePromise->value();
        if (!nativeCert)
        {
          Error ^error = nullptr;
          mTce.set_exception(error);
          return;
        }
        auto ret = ref new RTCCertificate();
        ret->_nativePointer = nativeCert;
        mTce.set(ret);
      }

      void RTCGenerateCertificatePromiseObserver::onPromiseRejected(PromisePtr promise)
      {
        auto reason = promise->reason<Any>();
        auto error = Error::CreateIfGeneric(reason);
        mTce.set_exception(error);
      }

    } // namespace internal

    RTCCertificate^ RTCCertificate::Convert(ICertificatePtr certificate)
    {
      RTCCertificate^ result = ref new RTCCertificate();
      result->_nativePointer = certificate;
      return result;
    }

    IAsyncOperation<RTCCertificate^>^ RTCCertificate::GenerateCertificate() {

      return Concurrency::create_async([]() -> RTCCertificate^ {
        Concurrency::task_completion_event<RTCCertificate^> tce;

        ICertificate::PromiseWithCertificatePtr promise = ICertificate::generateCertificate();
        auto pDelegate(make_shared<internal::RTCGenerateCertificatePromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<RTCCertificate^>(tce);

        return tceTask.get();
      });
    }

    IAsyncOperation<RTCCertificate^>^ RTCCertificate::GenerateCertificate(Platform::String^ algorithmIdentifier) {

      return Concurrency::create_async([algorithmIdentifier]() -> RTCCertificate^ {
        Concurrency::task_completion_event<RTCCertificate^> tce;

        ICertificate::PromiseWithCertificatePtr promise = ICertificate::generateCertificate(UseHelper::FromCx(algorithmIdentifier).c_str());
        auto pDelegate(make_shared<internal::RTCGenerateCertificatePromiseObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<RTCCertificate^>(tce);

        return tceTask.get();
      });
    }

    Windows::Foundation::DateTime RTCCertificate::Expires::get()
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)

      auto expires = _nativePointer->expires();
      return UseHelper::ToCx(expires);
    }

    RTCDtlsFingerprint^ RTCCertificate::Fingerprint::get()
    {
      if (!_nativePointer) return nullptr;
      return internal::ToCx(_nativePointer->fingerprint());
    }

  } // namespace ortc
} // namespace org
