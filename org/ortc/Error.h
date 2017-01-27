
#pragma once

#include <ortc/types.h>
#include <zsLib/types.h>
#include <zsLib/internal/types.h>

#define ORG_ORTC_THROW_INVALID_PARAMETERS() {throw ref new Platform::InvalidArgumentException();}
#define ORG_ORTC_THROW_INVALID_PARAMETERS_IF(xExpression) if (xExpression) {throw ref new Platform::InvalidArgumentException();}
#define ORG_ORTC_THROW_INVALID_STATE_IF(xExpression) if (xExpression) {throw ref new Platform::COMException(E_NOT_VALID_STATE, #xExpression);}
#define ORG_ORTC_THROW_INVALID_STATE(xStr) {throw ref new Platform::COMException(E_NOT_VALID_STATE, xStr);}
#define ORG_ORTC_THROW_INVALID_STATE_MESSAGE(xStr) {throw ref new Platform::COMException(E_NOT_VALID_STATE, #xStr);}
#define ORG_ORTC_THROW_NOT_SUPPORTED(xStr) {throw ref new Platform::COMException(CO_E_NOT_SUPPORTED, xStr);}
#define ORG_ORTC_THROW_NOT_SUPPORTED_IF(xExpression) if (xExpression) {throw ref new Platform::COMException(CO_E_NOT_SUPPORTED, #xExpression);}
#define ORG_ORTC_THROW_UNEXPECTED_IF(xExpression) if (xExpression) {throw ref new Platform::COMException(E_UNEXPECTED, #xExpression);}

namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(zsLib::Any, Any)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ErrorAny, ErrorAny)

    namespace Internal
    {
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackDelegate);
      ZS_DECLARE_CLASS_PTR(RTCDtlsTransportDelegate);
      ZS_DECLARE_CLASS_PTR(RTCDataChannelDelegate);
      ZS_DECLARE_CLASS_PTR(RTCSrtpSdesTransportDelegate);
      ZS_DECLARE_CLASS_PTR(MediaDevicesPromiseObserver);
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackConstraintsPromiseObserver);
      ZS_DECLARE_CLASS_PTR(RTCGenerateCertificatePromiseObserver);
      ZS_DECLARE_CLASS_PTR(RTCRtpSenderPromiseObserver);
      ZS_DECLARE_CLASS_PTR(RTCRtpReceiverPromiseObserver);
      ZS_DECLARE_CLASS_PTR(RTCStateProviderObserver);
    } // namespace internal

    namespace Adapter
    {
      namespace Internal
      {
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionPromiseWithDescriptionObserver);
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionPromiseWithSenderObserver);
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionPromiseWitDataChannelObserver);
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionPromiseObserver);
      } // namespace internal
    } // namespace adapter

    /// <summary>
    /// This object represents generic error information.
    /// </summary>
    public ref struct Error sealed
    {
    private:
      friend class Internal::RTCDtlsTransportDelegate;
      friend class Internal::MediaDevicesPromiseObserver;
      friend class Internal::RTCDataChannelDelegate;
      friend class Internal::RTCSrtpSdesTransportDelegate;
      friend class Internal::MediaStreamTrackConstraintsPromiseObserver;
      friend class Internal::RTCGenerateCertificatePromiseObserver;
      friend class Internal::RTCRtpSenderPromiseObserver;
      friend class Internal::RTCRtpReceiverPromiseObserver;
      friend class Internal::RTCStateProviderObserver;
      friend class Adapter::Internal::RTCPeerConnectionPromiseWithDescriptionObserver;
      friend class Adapter::Internal::RTCPeerConnectionPromiseWithSenderObserver;
      friend class Adapter::Internal::RTCPeerConnectionPromiseWitDataChannelObserver;
      friend class Adapter::Internal::RTCPeerConnectionPromiseObserver;

      static Error^ CreateIfGeneric(AnyPtr any);
      static Error^ CreateIfGeneric(ErrorAnyPtr error);

    public:
      /// <summary>
      /// Gets or sets a string representing the error code (see HTTP
      /// status codes).
      /// </summary>
      property uint16            ErrorCode;
      /// <summary>
      /// Gets or sets a string representing one of the error typename.
      /// </summary>
      property Platform::String^ Name;
      /// <summary>
      /// Gets or sets a string representing one of the error reason.
      /// </summary>
      property Platform::String^ Reason;
    };

    /// <summary>
    /// This object represents the error event for event delegates.
    /// </summary>
    public ref struct ErrorEvent sealed
    {
    private:
      friend class Internal::MediaStreamTrackDelegate;
      friend class Internal::RTCDtlsTransportDelegate;
      friend class Internal::RTCDataChannelDelegate;
      friend class Internal::RTCSrtpSdesTransportDelegate;

      ErrorEvent(ref struct Error ^error)
      {
        _error = error;
      }

    public:
      /// <summary>
      /// Gets the error information associated with this event (if any).
      /// </summary>
      property Error^ Error
      {
        ref struct Error^ get() { return _error; }
      }

    private:
      ref struct Error^ _error;
    };

  } // namespace ortc
} // namespace org
