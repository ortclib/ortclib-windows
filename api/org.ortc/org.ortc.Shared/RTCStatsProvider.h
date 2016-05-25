#pragma once

#include <ortc/IStatsProvider.h>

#include "RTCStatsReport.h"

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
    /// Gets or sets the inspection options for an RTCStatsProvider.GetStats
    /// method.
    /// </summary>
    public ref struct RTCStatsProviderOptions sealed
    {
      /// <summary>
      /// Gets or sets the desired list of objects to inspect on an object
      /// which provides statistics reports. An empty or null list will cause
      /// all inspectable reports to be generated for the inspected object.
      /// </summary>
      property IVector<RTCStatsType>^         StatsTypes;

      /// <summary>
      /// Constructs an instance of an RTCStatsProviderOptions object that
      /// will configure the options to generate inspectable reports available
      /// for the inspected object.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCStatsProviderOptions() {}

      /// <summary>
      /// Constructs an instance of an RTCStatsProviderOptions object that
      /// will configure the options to generate inspectable reports
      /// restricted to only contain statistic objects whose type(s) are
      /// contained within the statsTypes list.
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
      /// Constructs an instance of an RTCStatsProvider object that will
      /// generate an null statistics report object when the GetStats method
      /// is called. This constructor is only present as a default constructor
      /// as a placeholder for a non-inspectable object.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      RTCStatsProvider() {}

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an MediaStreamTrack object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithMediaStreamTrack")]
      RTCStatsProvider(MediaStreamTrack^ track, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCCertificate object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithCertificate")]
      RTCStatsProvider(RTCCertificate^ cert, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCDataChannel object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithDataChannel")]
      RTCStatsProvider(RTCDataChannel^ channel, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCDtlsTransport object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithDtlsTransport")]
      RTCStatsProvider(RTCDtlsTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCIceGatherer object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithIceGatherer")]
      RTCStatsProvider(RTCIceGatherer^ gatherer, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCIceTransport object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithIceTransport")]
      RTCStatsProvider(RTCIceTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCIceTransportController object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithIceTransportController")]
      RTCStatsProvider(RTCIceTransportController^ controller, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCRtpListener object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtpListener")]
      RTCStatsProvider(RTCRtpListener^ listener, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCRtpReceiver object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtpReceiver")]
      RTCStatsProvider(RTCRtpReceiver^ receiver, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCRtpSender object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithRtpSender")]
      RTCStatsProvider(RTCRtpSender^ sender, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCSrtpSdesTransport object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSrtpSdesTransport")]
      RTCStatsProvider(RTCSrtpSdesTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCSctpTransport object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithSctpTransport")]
      RTCStatsProvider(RTCSctpTransport^ transport, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an RTCPeerConnection object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithPeerConnection")]
      RTCStatsProvider(adapter::RTCPeerConnection^ connection, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Constructs an instance of an RTCStatsProvider object that will
      /// inspect an MediaStream object.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("CreateWithMediaStream")]
      RTCStatsProvider(adapter::MediaStream^ stream, RTCStatsProviderOptions^ options);

      /// <summary>
      /// Gets an awaitable RTCStatsReport object that will contain a set of
      /// statistics objects for the inspected object. A null RTCStatsReport
      /// return is possible if the object declined the request to inspect
      /// statistics at this time. An RTCStatsReport containing no statistics
      /// objects indicates that no inspectable statistics are available at
      /// this time.
      /// </summary>
      IAsyncOperation<RTCStatsReport^>^ GetStats();

    private:
      IStatsProviderPtr _nativePointer;
      IStatsProviderTypes::StatsTypeSet mStatsTypes;
    };

  } // namespace ortc
} // namespace org
