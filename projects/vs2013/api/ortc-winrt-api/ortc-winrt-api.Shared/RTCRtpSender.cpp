#include "pch.h"
#include "RTCRtpSender.h"
#include "helpers.h"

using namespace ortc_winrt_api;

using Platform::Collections::Vector;

extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

RTCRtpSender::RTCRtpSender() :
mNativeDelegatePointer(nullptr),
mNativePointer(nullptr)
{
}

RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport) :
mNativeDelegatePointer(new RTCRtpSenderDelegate())
{
  if (!track && !transport)
  {
    return;
  }

  if (FetchNativePointer::fromDtlsTransport(transport)) // add mediaStreamTrack too
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IRTPSender::create(mNativeDelegatePointer, IMediaStreamTrackPtr(), FetchNativePointer::fromDtlsTransport(transport));
  }
}

RTCRtpSender::RTCRtpSender(MediaStreamTrack^ track, RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport) :
mNativeDelegatePointer(new RTCRtpSenderDelegate())
{
  if (!track && !transport && !rtcpTransport)
  {
    return;
  }

  if (FetchNativePointer::fromDtlsTransport(transport) && FetchNativePointer::fromDtlsTransport(rtcpTransport)) // add mediaStreamTrack too
  {
    mNativeDelegatePointer->SetOwnerObject(this);
    mNativePointer = IRTPSender::create(mNativeDelegatePointer, IMediaStreamTrackPtr(), FetchNativePointer::fromDtlsTransport(transport), FetchNativePointer::fromDtlsTransport(rtcpTransport));
  }
}

void RTCRtpSender::setTransport(RTCDtlsTransport^ transport, RTCDtlsTransport^ rtcpTransport)
{
  if (mNativePointer)
  {
    mNativePointer->setTransport(FetchNativePointer::fromDtlsTransport(transport), FetchNativePointer::fromDtlsTransport(rtcpTransport));
  }
}

/*IAsyncAction^ RTCRtpSender::setTrack(MediaStreamTrack^ track)
{
  return Concurrency::create_async([]() -> RTCCertificate^ {
    Concurrency::task_completion_event<RTCCertificate^> tce;

    ICertificate::PromiseWithCertificatePtr promise = ICertificate::generateCertificate();
    RTCGenerateCertificatePromiseObserverPtr pDelegate(make_shared<RTCGenerateCertificatePromiseObserver>(tce));

    promise->then(pDelegate);
    promise->background();
    auto tceTask = Concurrency::task<RTCCertificate^>(tce);

    return tceTask.get();
  });
}*/

RTCRtpCapabilities^ RTCRtpSender::getCapabilities(Platform::String^ kind)
{
  auto ret = ref new RTCRtpCapabilities();
  IRTPTypes::CapabilitiesPtr capabilitiesPtr;
  if (kind != nullptr)
  {
    if (Platform::String::CompareOrdinal(kind, "audio") == 0)
      capabilitiesPtr = IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Audio);
    else if (Platform::String::CompareOrdinal(kind, "video") == 0)
      capabilitiesPtr = IRtpReceiver::getCapabilities(IRTPReceiverTypes::Kinds::Kind_Video);
    else
      capabilitiesPtr = IRtpReceiver::getCapabilities();
  }

  if (capabilitiesPtr)
  {
    for (IRTPTypes::CodecCapabilitiesList::iterator it = capabilitiesPtr->mCodecs.begin(); it != capabilitiesPtr->mCodecs.end(); ++it)
    {
      auto codec = toCx((make_shared<IRTPTypes::CodecCapability>(*it)));
      ret->codecs->Append(codec);
    }

    for (IRTPTypes::HeaderExtensionsList::iterator it = capabilitiesPtr->mHeaderExtensions.begin(); it != capabilitiesPtr->mHeaderExtensions.end(); ++it)
    {
      auto codec = toCx((make_shared<IRTPTypes::HeaderExtensions>(*it)));
      ret->headerExtensions->Append(codec);
    }

    for (std::list<zsLib::String>::iterator it = capabilitiesPtr->mFECMechanisms.begin(); it != capabilitiesPtr->mFECMechanisms.end(); ++it)
    {
      ret->fecMechanisms->Append(ToCx(*it));
    }

  }

  return ret;
}

void RTCRtpSender::send(RTCRtpParameters^ parameters)
{

}

void RTCRtpSender::stop()
{
  if (mNativePointer)
  {
    mNativePointer->stop();
  }
}
