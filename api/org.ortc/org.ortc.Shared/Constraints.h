#pragma once

#include <ortc/IConstraints.h>

namespace org
{
  namespace ortc
  {
    using Windows::Foundation::Collections::IVector;

    ref struct ConstrainBooleanParameters;
    ref struct ConstrainBoolean;
    ref struct ConstrainLong;
    ref struct ConstrainLongRange;
    ref struct ConstrainDouble;
    ref struct ConstrainDoubleRange;
    ref struct ConstrainString;
    ref struct ConstrainStringParameters;
    ref struct StringOrStringList;

    namespace internal
    {
      using zsLib::Optional;

      ZS_DECLARE_TYPEDEF_PTR(::ortc::IConstraints, IConstraints)

      ConstrainBooleanParameters^ ToCx(const IConstraints::ConstrainBooleanParameters &input);
      ConstrainBooleanParameters^ ToCx(const Optional<IConstraints::ConstrainBooleanParameters> &input);
      Optional<IConstraints::ConstrainBooleanParameters> FromCx(ConstrainBooleanParameters^ input);

      ConstrainBoolean^ ToCx(const IConstraints::ConstrainBoolean &input);
      IConstraints::ConstrainBoolean FromCx(ConstrainBoolean^ input);

      ConstrainLongRange^ ToCx(const IConstraints::ConstrainLongRange &input);
      ConstrainLongRange^ ToCx(const Optional<IConstraints::ConstrainLongRange> &input);
      Optional<IConstraints::ConstrainLongRange> FromCx(ConstrainLongRange^ input);

      ConstrainLong^ ToCx(const IConstraints::ConstrainLong &input);
      IConstraints::ConstrainLong FromCx(ConstrainLong^ input);

      ConstrainDoubleRange^ ToCx(const IConstraints::ConstrainDoubleRange &input);
      ConstrainDoubleRange^ ToCx(const Optional<IConstraints::ConstrainDoubleRange> &input);
      Optional<IConstraints::ConstrainDoubleRange> FromCx(ConstrainDoubleRange^ input);

      ConstrainDouble^ ToCx(const IConstraints::ConstrainDouble &input);
      IConstraints::ConstrainDouble FromCx(ConstrainDouble^ input);

      ConstrainString^ ToCx(const IConstraints::ConstrainString &input);
      IConstraints::ConstrainString FromCx(ConstrainString^ input);

      StringOrStringList^ ToCx(const IConstraints::StringOrStringList &input);
      Optional<IConstraints::StringOrStringList> FromCx(StringOrStringList^ input);

      ConstrainStringParameters^ ToCx(const IConstraints::ConstrainStringParameters &input);
      ConstrainStringParameters^ ToCx(const Optional<IConstraints::ConstrainStringParameters> &input);
      Optional<IConstraints::ConstrainStringParameters> FromCx(ConstrainStringParameters^ input);

      IVector<Platform::String^>^ ToCx(const IConstraints::StringList &input);
      IVector<Platform::String^>^ ToCx(const Optional<IConstraints::StringList> &input);
      Optional<IConstraints::StringList> FromCx(IVector<Platform::String^>^ input);
    }

    /// <summary>
    /// This object represents the constrainable boolean parameters.
    /// </summary>
    public ref struct ConstrainBooleanParameters sealed
    {
      /// <summary>
      /// Gets or sets the exact required value for this property.
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Exact;
      /// <summary>
      /// Gets or sets the ideal (target) value for this property.
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Ideal;
    };

    /// <summary>
    /// This object represents the constrainable boolean values.
    /// </summary>
    public ref struct ConstrainBoolean sealed
    {
      /// <summary>
      /// Gets or sets the exact boolean value requried. Use this value must
      /// be mutually exclusively set from Parameters.
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Value;
      /// <summary>
      /// Gets or sets the boolean constrainable value parameters.
      /// </summary>
      property ConstrainBooleanParameters^        Parameters;
    };

    /// <summary>
    /// This object represents the constrainable minimum, maximum, exact, or
    /// ideal integer values.
    /// </summary>
    public ref struct ConstrainLongRange sealed
    {
      /// <summary>
      /// Gets or sets the maximum legal value of this property.
      /// </summary>
      property Platform::IBox<int32>^ Max;
      /// <summary>
      /// Gets or sets the minimum value of this property.
      /// </summary>
      property Platform::IBox<int32>^ Min;

      /// <summary>
      /// Gets or sets the exact required value for this property.
      /// </summary>
      property Platform::IBox<int32>^ Exact;
      /// <summary>
      /// Gets or sets the ideal (target) value for this property.
      /// </summary>
      property Platform::IBox<int32>^ Ideal;
    };

    /// <summary>
    /// This object represents the constrainable integer values.
    /// </summary>
    public ref struct ConstrainLong sealed
    {
      /// <summary>
      /// Gets or sets the exact integer value requried. This value must be
      /// mutually exclusively set from Range.
      /// </summary>
      property Platform::IBox<int32>^ Value;
      /// <summary>
      /// Gets or sets the constrainable integer range values.
      /// </summary>
      property ConstrainLongRange^    Range;
    };

    /// <summary>
    /// This object represents the constrainable minimum, maximum, exact, or
    /// ideal real values.
    /// </summary>
    public ref struct ConstrainDoubleRange sealed
    {
      /// <summary>
      /// Gets or sets the maximum legal value of this property.
      /// </summary>
      property Platform::IBox<float64>^ Max;
      /// <summary>
      /// Gets or sets the minimum value of this property.
      /// </summary>
      property Platform::IBox<float64>^ Min;

      /// <summary>
      /// Gets or sets the exact required value for this property.
      /// </summary>
      property Platform::IBox<float64>^ Exact;
      /// <summary>
      /// Gets or sets the ideal (target) value for this property.
      /// </summary>
      property Platform::IBox<float64>^ Ideal;
    };

    /// <summary>
    /// This object represents the constrainable real values.
    /// </summary>
    public ref struct ConstrainDouble sealed
    {
      /// <summary>
      /// Gets or sets the exact real value requried. This value must be
      /// mutually exclusively set from Range.
      /// </summary>
      property Platform::IBox<float64>^ Value;
      /// <summary>
      /// Gets or sets the constrainable real range values.
      /// </summary>
      property ConstrainDoubleRange^    Range;
    };

    /// <summary>
    /// This object represents a single string value or a list of string
    /// values.
    /// </summary>
    public ref struct StringOrStringList sealed
    {
      /// <summary>
      /// Gets or sets a specific constrainable string value. This value must
      /// be mutally exclusively set from Values;
      /// </summary>
      property Platform::String^            Value;
      /// <summary>
      /// Gets or sets the constrainable string values.
      /// </summary>
      property IVector<Platform::String^>^  Values;
    };

    /// <summary>
    /// This object represents the constrainable string parameters.
    /// </summary>
    public ref struct ConstrainStringParameters sealed
    {
      /// <summary>
      /// Gets or sets the exact required value for this property.
      /// </summary>
      property StringOrStringList^ Exact;
      /// <summary>
      /// Gets or sets the ideal (target) value for this property.
      /// </summary>
      property StringOrStringList^ Ideal;
    };

    /// <summary>
    /// This object represents the constrainable string values.
    /// </summary>
    public ref struct ConstrainString sealed
    {
      /// <summary>
      /// Gets or sets the exact string value(s) requried. This value must be
      /// mutually exclusively set from Parameters.
      /// </summary>
      property StringOrStringList^        Value;
      /// <summary>
      /// Gets or sets the string constrainable value parameters.
      /// </summary>
      property ConstrainStringParameters^ Parameters;
    };
  }
}
