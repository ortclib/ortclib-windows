#include "pch.h"
#include "RTCDtlsTransport.h"
#include "helpers.h"

#include <openpeer/services/IHelper.h>

using Platform::Collections::Vector;
using Platform::Array;
using Platform::Object;

namespace Concurrency
{
  using ::LONG;
}

namespace ortc_winrt_api
{

  RTCDtlsTransport::RTCDtlsTransport() :
    mNativeDelegatePointer(nullptr),
    mNativePointer(nullptr)
  {
  }

  RTCDtlsTransport::RTCDtlsTransport(RTCIceTransport^ transport, IVector<RTCCertificate^>^ certificates) :
    mNativeDelegatePointer(new RTCDtlsTransportDelegate())
  {

    if (!transport || !certificates)
    {
      return;
    }

    if (FetchNativePointer::FromIceTransport(transport))
    {
      mNativeDelegatePointer->SetOwnerObject(this);

      IDTLSTransport::CertificateList coreCertificates;
      for (RTCCertificate^ cert : certificates)
      {
        ICertificatePtr coreCert = FetchNativePointer::FromCertificate(cert);
        coreCertificates.push_back(coreCert);
      }
      mNativePointer = IDTLSTransport::create(mNativeDelegatePointer, FetchNativePointer::FromIceTransport(transport), coreCertificates);
    }
  }

  RTCDtlsParameters^ RTCDtlsTransport::GetLocalParameters()
  {
    if (mNativePointer)
    {
      return ToCx(mNativePointer->getLocalParameters());
    }
    return nullptr;
  }

  RTCDtlsParameters^ RTCDtlsTransport::GetRemoteParameters()
  {
    if (mNativePointer)
    {
      return ToCx(mNativePointer->getRemoteParameters());
    }
    return nullptr;
  }

  IVector<Object^>^ RTCDtlsTransport::GetRemoteCertificates()
  {
    auto ret = ref new Vector<Object^>();

    if (mNativePointer)
    {
      IDTLSTransportTypes::SecureByteBlockListPtr certificates = mNativePointer->getRemoteCertificates();

      if (certificates->size() > 0)
      {
        for (IDTLSTransportTypes::SecureByteBlockList::iterator it = certificates->begin(); it != certificates->end(); ++it)
        {
          Array<byte>^ arr = ref new Array<byte>((*it).BytePtr(), (*it).SizeInBytes());
          ret->Append(arr);
        }
      }
    }

    return ret;
  }

  void RTCDtlsTransport::Start(RTCDtlsParameters^ remoteParameters)
  {
    if (mNativePointer)
    {
      assert(nullptr != remoteParameters);
      mNativePointer->start(*FromCx(remoteParameters));
    }
  }

  void RTCDtlsTransport::Stop()
  {
    if (mNativePointer)
    {
      mNativePointer->stop();
    }
  }

  RTCIceTransport^ RTCDtlsTransport::GetIceTransport()
  {
    return ConvertObjectToCx::ToIceTransport(mNativePointer->transport());
  }

  IVector<RTCCertificate^>^ RTCDtlsTransport::GetCertificates()
  {
    auto ret = ref new Vector<RTCCertificate^>();

    if (mNativePointer)
    {
      IDTLSTransport::CertificateListPtr certificates = mNativePointer->certificates();
      for (IDTLSTransport::CertificateList::iterator it = certificates->begin(); it != certificates->end(); ++it)
      {
        ret->Append(ConvertObjectToCx::ToCertificate(*it));
      }
    }

    return ret;
  }

  Platform::String^ RTCDtlsTransport::ToString()
  {
    throw ref new Platform::NotImplementedException();
  }

  Platform::String^ RTCDtlsTransport::ToString(RTCDtlsTransportState value)
  {
    return ToCx(IDtlsTransport::toString(internal::ConvertEnums::convert(value)));
  }

  Platform::String^ RTCDtlsTransport::ToString(RTCDtlsRole value)
  {
    return ToCx(IDtlsTransport::toString(internal::ConvertEnums::convert(value)));
  }

  RTCDtlsTransportState RTCDtlsTransport::ToState(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IDtlsTransport::toState(FromCx(str).c_str()));
  }

  RTCDtlsRole RTCDtlsTransport::ToRole(Platform::String^ str)
  {
    return internal::ConvertEnums::convert(IDtlsTransport::toRole(FromCx(str).c_str()));
  }

  RTCDtlsTransportState RTCDtlsTransport::State::get()
  {
    if (mNativePointer)
      return (RTCDtlsTransportState)mNativePointer->state();
    else
      return RTCDtlsTransportState::Closed;
  }

  void RTCDtlsTransportDelegate::onDTLSTransportStateChange(
    IDTLSTransportPtr transport,
    IDTLSTransport::States state
    )
  {
    auto evt = ref new RTCDtlsTransportStateChangeEvent();
    evt->State = internal::ConvertEnums::convert(state);
    _transport->OnDtlsTransportStateChanged(evt);
  }

  void RTCDtlsTransportDelegate::onDTLSTransportError(
    IDTLSTransportPtr transport,
    ErrorCode errorCode,
    zsLib::String errorReason
    )
  {
    auto evt = ref new RTCDtlsTransportErrorEvent();
    evt->Error->ErrorCode = errorCode;
    evt->Error->ErrorReason = ToCx(errorReason);
    _transport->OnDtlsTransportError(evt);
  }

  IAsyncOperation<RTCCertificate^>^ RTCCertificate::GenerateCertificate() {

    return Concurrency::create_async([]() -> RTCCertificate^ {
      Concurrency::task_completion_event<RTCCertificate^> tce;

      ICertificate::PromiseWithCertificatePtr promise = ICertificate::generateCertificate();
      RTCGenerateCertificatePromiseObserverPtr pDelegate(make_shared<RTCGenerateCertificatePromiseObserver>(tce));

      promise->then(pDelegate);
      promise->background();
      auto tceTask = Concurrency::task<RTCCertificate^>(tce);

      return tceTask.get();
    });
  }

  IAsyncOperation<RTCCertificate^>^ RTCCertificate::GenerateCertificate(Platform::String^ algorithmIdentifier) {

    return Concurrency::create_async([algorithmIdentifier]() -> RTCCertificate^ {
      Concurrency::task_completion_event<RTCCertificate^> tce;

      ICertificate::PromiseWithCertificatePtr promise = ICertificate::generateCertificate(FromCx(algorithmIdentifier).c_str());
      RTCGenerateCertificatePromiseObserverPtr pDelegate(make_shared<RTCGenerateCertificatePromiseObserver>(tce));

      promise->then(pDelegate);
      promise->background();
      auto tceTask = Concurrency::task<RTCCertificate^>(tce);

      return tceTask.get();
    });
  }

  Windows::Foundation::DateTime RTCCertificate::GetExpires()
  {
    Windows::Foundation::DateTime ret;

    if (mNativePointer)
    {
      auto coreTime = mNativePointer->expires();
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
    if (mNativePointer)
    {
      return ToCx(mNativePointer->fingerprint());
    }
    return nullptr;
  }

  RTCGenerateCertificatePromiseObserver::RTCGenerateCertificatePromiseObserver(Concurrency::task_completion_event<RTCCertificate^> tce) :
    mTce(tce)
  {
  }

  void RTCGenerateCertificatePromiseObserver::onPromiseResolved(PromisePtr promise)
  {
    auto ret = ref new RTCCertificate();

    ICertificate::PromiseWithCertificatePtr certificatePromise = ZS_DYNAMIC_PTR_CAST(ICertificate::PromiseWithCertificate, promise);
    ret->mNativePointer = certificatePromise->value();
    mTce.set(ret);
  }
  void RTCGenerateCertificatePromiseObserver::onPromiseRejected(PromisePtr promise)
  {
    //mTce.set_exception(promise->reason());
  }

  //---------------------------------------------------------------------------
  // RTCDtlsParameters methods
  //---------------------------------------------------------------------------
  Platform::String^ RTCDtlsParameters::ToJsonString()
  {
    IDtlsTransport::ParametersPtr params = FromCx(this);
    return ToCx(openpeer::services::IHelper::toString(params->createElement("DtlsParameters")));
  }

  RTCDtlsParameters^ RTCDtlsParameters::FromJsonString(Platform::String^ jsonString)
  {
    auto params = make_shared<IDtlsTransport::Parameters>(IDtlsTransport::Parameters::Parameters(openpeer::services::IHelper::toJSON(FromCx(jsonString).c_str())));
    return ToCx(params);
  }

} // namespace ortc_winrt_api
