#pragma once

#include <ortc/IStatsProvider.h>

#include "RTCStatsReport.h"

//#include <ortc/IICEGatherer.h>
//#include <collection.h>
//
//using namespace ortc;
//
//using Windows::Foundation::Collections::IVector;


namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IStatsReportTypes, IStatsReportTypes);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IStatsReport, IStatsReport);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IStatsProviderTypes, IStatsProviderTypes);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IStatsProvider, IStatsProvider);

    using Windows::Foundation::Collections::IVector;
    using Windows::Foundation::IAsyncOperation;

    ref struct RTCStatsProviderOptions;

    ref class RTCStatsProvider;

    ref class RTCCertificate;
    ref class RTCDataChannel;
    ref class RTCDtlsTransport;
    ref class RTCIceGatherer;
    ref class RTCIceTransport;
    ref class RTCIceTransportController;
    ref class RTCRtpListener;
    ref class RTCRtpReceiver;
    ref class RTCRtpSender;
    ref class RTCSrtpSdesTransport;
    ref class MediaStreamTrack;
    ref class RTCSctpTransport;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCStateProviderObserver);

      IStatsProviderTypes::StatsTypeSet FromCx(RTCStatsProviderOptions^ input);

    } // namespace internal

    namespace adapter
    {
      ref class RTCPeerConnection;
      ref class MediaStream;
    } // namespace adapter


    /// <summary>
    /// </summary>
    public ref struct RTCStatsProviderOptions sealed
    {
      /// <summary>
      /// </summary>
      property IVector<RTCStatsType>^         StatsTypes;

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCStatsProviderOptions() {}

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithStatsTypes")]
      RTCStatsProviderOptions(IVector<RTCStatsType>^ statsTypes) { StatsTypes = statsTypes; }
    };

    /// <summary>
    /// </summary>
    public ref class RTCStatsProvider sealed
    {
      friend class internal::RTCStateProviderObserver;

    private:
      RTCStatsProvider(IStatsProviderPtr nativePointer);

      static RTCStatsProvider^ Convert(IStatsProviderPtr nativePointer) { if (!nativePointer) return nullptr; return ref new RTCStatsProvider(nativePointer); }
      static IStatsProviderPtr Convert(RTCStatsProvider^ channel) { if (!channel) return nullptr; return channel->_nativePointer; }

    public:
      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCStatsProvider() {}

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithMediaStreamTrack")]
      RTCStatsProvider(MediaStreamTrack^ track, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithCertificate")]
      RTCStatsProvider(RTCCertificate^ cert, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithDataChannel")]
      RTCStatsProvider(RTCDataChannel^ channel, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithDtlsTransport")]
      RTCStatsProvider(RTCDtlsTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithIceGatherer")]
      RTCStatsProvider(RTCIceGatherer^ gatherer, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithIceTransport")]
      RTCStatsProvider(RTCIceTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithIceTransportController")]
      RTCStatsProvider(RTCIceTransportController^ controller, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtpListener")]
      RTCStatsProvider(RTCRtpListener^ listener, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtpReceiver")]
      RTCStatsProvider(RTCRtpReceiver^ receiver, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtpSender")]
      RTCStatsProvider(RTCRtpSender^ sender, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSrtpSdesTransport")]
      RTCStatsProvider(RTCSrtpSdesTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSctpTransport")]
      RTCStatsProvider(RTCSctpTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithPeerConnection")]
      RTCStatsProvider(adapter::RTCPeerConnection^ connection, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithMediaStream")]
      RTCStatsProvider(adapter::MediaStream^ stream, RTCStatsProviderOptions^ options);

      /// <summary>
      /// </summary>
      IAsyncOperation<RTCStatsReport^>^ GetStats();

    private:
      IStatsProviderPtr _nativePointer;
      IStatsProviderTypes::StatsTypeSet mStatsTypes;
    };

  } // namespace ortc
} // namespace org
