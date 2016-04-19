
#pragma once

#include <ortc/types.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(zsLib::Any, Any)

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackDelegate)
      ZS_DECLARE_CLASS_PTR(MediaDevicesPromiseObserver)
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackConstraintsPromiseObserver)
      ZS_DECLARE_CLASS_PTR(RTCGenerateCertificatePromiseObserver)
      ZS_DECLARE_CLASS_PTR(RTCSenderSetTrackPromiseObserver)
    } // namespace internal

    /// <summary>
    /// This object represents generic error information.
    /// </summary>
    public ref struct Error sealed
    {
    private:
      friend class internal::MediaDevicesPromiseObserver;
      friend class internal::MediaStreamTrackConstraintsPromiseObserver;
      friend class internal::RTCGenerateCertificatePromiseObserver;
      friend class internal::RTCSenderSetTrackPromiseObserver;

      static Error^ CreateIfGeneric(AnyPtr any);

    public:
      /// <summary>
      /// Gets or sets a string representing one of the error typename.
      /// </summary>
      property Platform::String^ Name;
    };

    /// <summary>
    /// This object represents the error event for event delegates.
    /// </summary>
    public ref struct ErrorEvent sealed
    {
    private:
      friend class internal::MediaStreamTrackDelegate;

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
