
#pragma once

#include <zsLib/types.h>

namespace org
{
  namespace ortc
  {
    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(MediaStreamTrackDelegate)
    } // namespace internal

    public ref struct Error sealed
    {
      /// <summary>
      /// Gets a string representing one of the error type names.
      /// </summary>
      property Platform::String^ Name;
    };

    public ref struct ErrorEvent sealed
    {
    private:
      friend class internal::MediaStreamTrackDelegate;

      ErrorEvent(ref struct Error ^error)
      {
        _error = error;
      }

    public:
      property Error^ Error
      {
        ref struct Error^ get() { return _error; }
      }

    private:
      ref struct Error^ _error;
    };

  } // namespace ortc
} // namespace org
