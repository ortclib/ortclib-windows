#include "pch.h"

#include <org/ortc/RTCStatsProvider.h>
#include <org/ortc/RTCCertificate.h>
#include <org/ortc/RTCDataChannel.h>
#include <org/ortc/RTCDtlsTransport.h>
#include <org/ortc/RTCIceGatherer.h>
#include <org/ortc/RTCIceTransport.h>
#include <org/ortc/RTCIceTransportController.h>
#include <org/ortc/RTCRtpListener.h>
#include <org/ortc/RTCRtpReceiver.h>
#include <org/ortc/RTCRtpSender.h>
#include <org/ortc/RTCSrtpSdesTransport.h>
#include <org/ortc/MediaStreamTrack.h>
#include <org/ortc/RTCSctpTransport.h>
#include <org/ortc/adapter/RTCPeerConnection.h>
#include <org/ortc/adapter/MediaStream.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

#include <ortc/services/IHelper.h>

#include <zsLib/Promise.h>
#include <zsLib/SafeInt.h>

using namespace ortc;

namespace Org
{
  namespace Ortc
  {
    using std::make_shared;
    ZS_DECLARE_USING_PTR(zsLib, Promise);

    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper)

    namespace Internal
    {

#pragma region RTCStateProviderObserver

      class RTCStateProviderObserver : public zsLib::IPromiseResolutionDelegate
      {
      public:
        RTCStateProviderObserver(Concurrency::task_completion_event<RTCStatsReport^> tce) : mTce(tce) {}

        virtual void onPromiseResolved(PromisePtr promise) override
        {
          IStatsProviderTypes::PromiseWithStatsReportPtr reportPromise = ZS_DYNAMIC_PTR_CAST(IStatsProviderTypes::PromiseWithStatsReport, promise);
          auto nativeReport = reportPromise->value();
          auto ret = RTCStatsReport::Convert(nativeReport);
          mTce.set(ret);
        }

        virtual void onPromiseRejected(PromisePtr promise) override
        {
          auto ret = RTCStatsReport::Convert(IStatsReportPtr());
          mTce.set(ret);
        }

      private:
        Concurrency::task_completion_event<RTCStatsReport^> mTce;
      };


#pragma endregion


#pragma region Stats types conversions

      IStatsProviderTypes::StatsTypeSet FromCx(RTCStatsProviderOptions^ input)
      {
        IStatsProviderTypes::StatsTypeSet result;
        if (!input) return result;

        if (input->StatsTypes)
        {
          for (RTCStatsType value : input->StatsTypes)
          {
            result.insert(UseHelper::Convert(value));
          }
        }

        return result;
      }

#pragma endregion

    } // namespace internal

    RTCStatsProvider::RTCStatsProvider(IStatsProviderPtr nativePointer) :
      _nativePointer(nativePointer)
    {
    }

    RTCStatsProvider::RTCStatsProvider(MediaStreamTrack^ track, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = MediaStreamTrack::Convert(track);
    }

    RTCStatsProvider::RTCStatsProvider(RTCCertificate^ cert, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCCertificate::Convert(cert);
    }

    RTCStatsProvider::RTCStatsProvider(RTCDataChannel^ channel, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCDataChannel::Convert(channel);
    }

    RTCStatsProvider::RTCStatsProvider(RTCDtlsTransport^ transport, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCDtlsTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(RTCIceGatherer^ gatherer, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCIceGatherer::Convert(gatherer);
    }

    RTCStatsProvider::RTCStatsProvider(RTCIceTransport^ transport, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCIceTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(RTCIceTransportController^ controller, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCIceTransportController::Convert(controller);
    }

    RTCStatsProvider::RTCStatsProvider(RTCRtpListener^ listener, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCRtpListener::Convert(listener);
    }

    RTCStatsProvider::RTCStatsProvider(RTCRtpReceiver^ receiver, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCRtpReceiver::Convert(receiver);
    }

    RTCStatsProvider::RTCStatsProvider(RTCRtpSender^ sender, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCRtpSender::Convert(sender);
    }

    RTCStatsProvider::RTCStatsProvider(RTCSctpTransport^ transport, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCSctpTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(RTCSrtpSdesTransport^ transport, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = RTCSrtpSdesTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(Adapter::RTCPeerConnection^ connection, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = Adapter::RTCPeerConnection::Convert(connection);
    }

    RTCStatsProvider::RTCStatsProvider(Adapter::MediaStream^ connection, RTCStatsProviderOptions^ options) :
      mStatsTypes(Internal::FromCx(options))
    {
      _nativePointer = Adapter::MediaStream::Convert(connection);
    }


    IAsyncOperation<RTCStatsReport^>^ RTCStatsProvider::GetStats()
    {
      if (!_nativePointer)
      {
        return Concurrency::create_async([]() {
          return RTCStatsReport::Convert(IStatsReportPtr());
        });
      }

      IStatsProviderTypes::PromiseWithStatsReportPtr promise;
      try
      {
        promise = _nativePointer->getStats(mStatsTypes);
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS();
      }
      catch (const InvalidStateError &)
      {
        // return a state report that will contain no reports
        return Concurrency::create_async([]() {
          return RTCStatsReport::Convert(IStatsReportPtr());
        });
      }

      return Concurrency::create_async([promise]() -> RTCStatsReport^ {
        Concurrency::task_completion_event<RTCStatsReport^> tce;

        auto pDelegate(make_shared<Internal::RTCStateProviderObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<RTCStatsReport^>(tce);

        return tceTask.get();
      });
    }

  } // namespace ortc
} // namespace org
