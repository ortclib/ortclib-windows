#pragma once

#include <ortc/ICapabilities.h>

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ref struct CapabilityBoolean;
    ref struct CapabilityLong;
    ref struct CapabilityDouble;
    ref struct CapabilityString;

    namespace internal
    {
      using zsLib::Optional;

      ZS_DECLARE_TYPEDEF_PTR(::ortc::ICapabilities, ICapabilities)

      CapabilityBoolean^ ToCx(const ICapabilities::CapabilityBoolean &input);
      CapabilityBoolean^ ToCx(const Optional<ICapabilities::CapabilityBoolean> &input);
      Optional<ICapabilities::CapabilityBoolean> FromCx(CapabilityBoolean^ input);

      CapabilityLong^ ToCx(const ICapabilities::CapabilityLong &input);
      CapabilityLong^ ToCx(const Optional<ICapabilities::CapabilityLong> &input);
      Optional<ICapabilities::CapabilityLong> FromCx(CapabilityLong^ input);

      CapabilityDouble^ ToCx(const ICapabilities::CapabilityDouble &input);
      CapabilityDouble^ ToCx(const Optional<ICapabilities::CapabilityDouble> &input);
      Optional<ICapabilities::CapabilityDouble> FromCx(CapabilityDouble^ input);

      CapabilityString^ ToCx(const ICapabilities::CapabilityString &input);
      CapabilityString^ ToCx(const Optional<ICapabilities::CapabilityString> &input);
      Optional<ICapabilities::CapabilityString> FromCx(CapabilityString^ input);

    } // namespace internal

    public ref struct CapabilityBoolean sealed
    {
      property IVector<Platform::Boolean>^ Values;
    };

    public ref struct CapabilityLong sealed
    {
      property int32    Min;
      property int32    Max;
    };

    public ref struct CapabilityDouble sealed
    {
      property float64  Min;
      property float64  Max;
    };

    public ref struct CapabilityString sealed
    {
      property IVector<Platform::String ^>^ Values;
    };
  }
}
