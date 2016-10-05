
#pragma once

#include <ortc/types.h>
#include <zsLib/types.h>
#include <zsLib/internal/types.h>

#define ORG_ORTC_THROW_INVALID_PARAMETERS_IF(xExpression) if (xExpression) {throw ref new Platform::InvalidArgumentException();}
#define ORG_ORTC_THROW_INVALID_PARAMETERS() {throw ref new Platform::InvalidArgumentException();}
#define ORG_ORTC_THROW_INVALID_STATE_IF(xExpression) if (xExpression) {throw ref new Platform::COMException(E_ILLEGAL_METHOD_CALL, #xExpression);}
#define ORG_ORTC_THROW_INVALID_STATE_MESSAGE(xStr) {throw ref new Platform::COMException(E_ILLEGAL_METHOD_CALL, #xStr);}
#define ORG_ORTC_THROW_INVALID_STATE(xStr) {throw ref new Platform::COMException(E_ILLEGAL_METHOD_CALL, xStr);}
#define ORG_ORTC_THROW_UNEXPECTED_IF(xExpression) if (xExpression) {throw ref new Platform::COMException(E_UNEXPECTED, #xExpression);}

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(zsLib::Any, Any)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ErrorAny, ErrorAny)

    namespace internal
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

    namespace adapter
    {
      namespace internal
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
      friend class internal::RTCDtlsTransportDelegate;
      friend class internal::MediaDevicesPromiseObserver;
      friend class internal::RTCDataChannelDelegate;
      friend class internal::RTCSrtpSdesTransportDelegate;
      friend class internal::MediaStreamTrackConstraintsPromiseObserver;
      friend class internal::RTCGenerateCertificatePromiseObserver;
      friend class internal::RTCRtpSenderPromiseObserver;
      friend class internal::RTCRtpReceiverPromiseObserver;
      friend class internal::RTCStateProviderObserver;
      friend class adapter::internal::RTCPeerConnectionPromiseWithDescriptionObserver;
      friend class adapter::internal::RTCPeerConnectionPromiseWithSenderObserver;
      friend class adapter::internal::RTCPeerConnectionPromiseWitDataChannelObserver;
      friend class adapter::internal::RTCPeerConnectionPromiseObserver;

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
      friend class internal::MediaStreamTrackDelegate;
      friend class internal::RTCDtlsTransportDelegate;
      friend class internal::RTCDataChannelDelegate;
      friend class internal::RTCSrtpSdesTransportDelegate;

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
