#include "pch.h"

#include "RTCStatsProvider.h"
#include "RTCCertificate.h"
#include "RTCDataChannel.h"
#include "RTCDtlsTransport.h"
#include "RTCIceGatherer.h"
#include "RTCIceTransport.h"
#include "RTCIceTransportController.h"
#include "RTCRtpListener.h"
#include "RTCRtpReceiver.h"
#include "RTCRtpSender.h"
#include "RTCSrtpSdesTransport.h"
#include "MediaStreamTrack.h"
#include "RTCSctpTransport.h"
#include "adapter/RTCPeerConnection.h"
#include "adapter/MediaStream.h"
#include "helpers.h"
#include "Error.h"

#include <openpeer/services/IHelper.h>

#include <zsLib/Promise.h>
#include <zsLib/SafeInt.h>

using namespace ortc;

namespace org
{
  namespace ortc
  {
    using std::make_shared;
    ZS_DECLARE_USING_PTR(zsLib, Promise);

    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
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

    RTCStatsProvider::RTCStatsProvider(MediaStreamTrack^ track, RTCStatsProviderOptions^ options)
    {
      _nativePointer = MediaStreamTrack::Convert(track);
    }

    RTCStatsProvider::RTCStatsProvider(RTCCertificate^ cert, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCCertificate::Convert(cert);
    }

    RTCStatsProvider::RTCStatsProvider(RTCDataChannel^ channel, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCDataChannel::Convert(channel);
    }

    RTCStatsProvider::RTCStatsProvider(RTCDtlsTransport^ transport, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCDtlsTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(RTCIceGatherer^ gatherer, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCIceGatherer::Convert(gatherer);
    }

    RTCStatsProvider::RTCStatsProvider(RTCIceTransport^ transport, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCIceTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(RTCIceTransportController^ controller, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCIceTransportController::Convert(controller);
    }

    RTCStatsProvider::RTCStatsProvider(RTCRtpListener^ listener, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCRtpListener::Convert(listener);
    }

    RTCStatsProvider::RTCStatsProvider(RTCRtpReceiver^ receiver, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCRtpReceiver::Convert(receiver);
    }

    RTCStatsProvider::RTCStatsProvider(RTCRtpSender^ sender, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCRtpSender::Convert(sender);
    }

    RTCStatsProvider::RTCStatsProvider(RTCSctpTransport^ transport, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCSctpTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(RTCSrtpSdesTransport^ transport, RTCStatsProviderOptions^ options)
    {
      _nativePointer = RTCSrtpSdesTransport::Convert(transport);
    }

    RTCStatsProvider::RTCStatsProvider(adapter::RTCPeerConnection^ connection, RTCStatsProviderOptions^ options)
    {
      _nativePointer = adapter::RTCPeerConnection::Convert(connection);
    }

    RTCStatsProvider::RTCStatsProvider(adapter::MediaStream^ connection, RTCStatsProviderOptions^ options)
    {
      _nativePointer = adapter::MediaStream::Convert(connection);
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

        auto pDelegate(make_shared<internal::RTCStateProviderObserver>(tce));

        promise->then(pDelegate);
        promise->background();
        auto tceTask = Concurrency::task<RTCStatsReport^>(tce);

        return tceTask.get();
      });
    }

  } // namespace ortc
} // namespace org
