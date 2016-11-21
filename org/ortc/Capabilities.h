#pragma once

#include <ortc/ICapabilities.h>

namespace Org
{
  namespace Ortc
  {
    using Windows::Foundation::Collections::IVector;

    ref struct CapabilityBoolean;
    ref struct CapabilityLong;
    ref struct CapabilityDouble;
    ref struct CapabilityString;

    namespace Internal
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

    /// <summary>
    /// The allowed legal boolean values.
    /// </summary>
    public ref struct CapabilityBoolean sealed
    {
      /// <summary>
      /// Gets or sets the allowed boolean values.
      /// </summary>
      property IVector<Platform::Boolean>^ Values;
    };

    /// <summary>
    /// The minimum and maximum legal values for an interger value.
    /// </summary>
    public ref struct CapabilityLong sealed
    {
      /// <summary>
      /// Gets or sets the minimum value of this property.
      /// </summary>
      property int32    Min;
      /// <summary>
      /// Gets or sets the maximum legal value of this property.
      /// </summary>
      property int32    Max;
    };

    /// <summary>
    /// The minimum and maximum legal values for an real value.
    /// </summary>
    public ref struct CapabilityDouble sealed
    {
      /// <summary>
      /// Gets or sets the minimum value of this property.
      /// </summary>
      property float64  Min;
      /// <summary>
      /// Gets or sets the maximum legal value of this property.
      /// </summary>
      property float64  Max;
    };

    /// <summary>
    /// The allowed legal string values.
    /// </summary>
    public ref struct CapabilityString sealed
    {
      /// <summary>
      /// Gets or sets the allowed values.
      /// </summary>
      property IVector<Platform::String ^>^ Values;
    };
  }
}
