#include "pch.h"
#include "RTCDtlsTransport.h"
#include "helpers.h"

using namespace ortc_winrt_api;

using Platform::Collections::Vector;
using Platform::Array;
using Platform::Object;

namespace Concurrency
{
  using ::LONG;
}

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

RTCDtlsTransport::RTCDtlsTransport() :
mNativeDelegatePointer(nullptr),
mNativePointer(nullptr)
{
}

RTCDtlsTransport::RTCDtlsTransport(RTCIceTransport^ transport, RTCCertificate^ certificate) :
mNativeDelegatePointer(new RTCDtlsTransportDelegate())
{

  if (!transport || !certificate)
  {
    return;
  }

  if (FetchNativePointer::FromIceTransport(transport))
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IDTLSTransport::create(mNativeDelegatePointer, FetchNativePointer::FromIceTransport(transport), FetchNativePointer::FromCertificate(certificate));
  }
}

RTCDtlsParameters^ RTCDtlsTransport::getLocalParameters()
{
  auto ret = ref new RTCDtlsParameters();

  if (mNativePointer)
  {
    auto parameters = mNativePointer->getLocalParameters();
    ret = ToCx(parameters);
  }

  return ret;
}

RTCDtlsParameters^ RTCDtlsTransport::getRemoteParameters()
{
  auto ret = ref new RTCDtlsParameters();

  if (mNativePointer)
  {
    auto parameters = mNativePointer->getRemoteParameters();
    ret = ToCx(parameters);
  }

  return ret;
}

IVector<Object^>^ RTCDtlsTransport::getRemoteCertificates()
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

void RTCDtlsTransport::start(RTCDtlsParameters^ remoteParameters)
{
  if (mNativePointer)
  {
    mNativePointer->start(FromCx(remoteParameters));
  }
}

void RTCDtlsTransport::stop()
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

#define WRAP_ENTIRE_LIST_OF_CERTIFICATES_ONCE_SUPPORTED_FROM_CORE_IMPLEMENTATION 1
#define WRAP_ENTIRE_LIST_OF_CERTIFICATES_ONCE_SUPPORTED_FROM_CORE_IMPLEMENTATION 2

  auto ret = ref new Vector<RTCCertificate^>();

  if (mNativePointer)
  {
    ret->Append(ConvertObjectToCx::ToCertificate(mNativePointer->certificate()));
  }

  return ret;
}

void RTCDtlsTransportDelegate::onDTLSTransportStateChange(
  IDTLSTransportPtr transport,
  IDTLSTransport::States state
  )
{
  auto evt = ref new RTCDtlsTransportStateChangeEvent();
  evt->State = (RTCDtlsTransportState)state;
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

IAsyncOperation<RTCCertificate^>^ RTCCertificate::generateCertificate(Platform::String^ algorithmIdentifier) {

  return Concurrency::create_async([algorithmIdentifier]() -> RTCCertificate^ {
    Concurrency::task_completion_event<RTCCertificate^> tce;

    ICertificate::PromiseWithCertificatePtr promise = ICertificate::generateCertificate();
    RTCGenerateCertificatePromiseObserverPtr pDelegate(make_shared<RTCGenerateCertificatePromiseObserver>(tce));

    promise->then(pDelegate);
    promise->background();
    auto tceTask = Concurrency::task<RTCCertificate^>(tce);

    return tceTask.get();
  });
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