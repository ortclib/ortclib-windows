#include "pch.h"

#include "RTCCertificate.h"
#include "RTCDtlsTransport.h"
#include "helpers.h"
#include "Error.h"

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

    Windows::Foundation::DateTime RTCCertificate::GetExpires()
    {
      Windows::Foundation::DateTime ret;

      if (_nativePointer)
      {
        auto coreTime = _nativePointer->expires();
        time_t time = std::chrono::system_clock::to_time_t(coreTime);
        tm dateTime;
        gmtime_s(&dateTime, &time);

        auto cal = ref new Windows::Globalization::Calendar();

        cal->Year = dateTime.tm_year + 1900;
        cal->Month = dateTime.tm_mon + 1;
        cal->Day = dateTime.tm_mday;

        // Microsoft magic number: 1 for AM, 2 for PM. 
        // https://msdn.microsoft.com/en-us/library/windows/apps/windows.globalization.calendar.period
        cal->Period = dateTime.tm_hour > 12 ? 2 /*PM*/ : 1 /*AM*/;

        cal->Hour = dateTime.tm_hour > 12 ? dateTime.tm_hour - 12 : dateTime.tm_hour;
        cal->Minute = dateTime.tm_min;
        cal->Second = dateTime.tm_sec;

        ret = cal->GetDateTime();
      }

      return ret;
    }

    RTCDtlsFingerprint^ RTCCertificate::GetFingerprint()
    {
      if (_nativePointer)
      {
        return internal::ToCx(_nativePointer->fingerprint());
      }
      return nullptr;
    }

  } // namespace ortc
} // namespace org
