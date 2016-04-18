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

    public ref struct ConstrainBooleanParameters sealed
    {
      property Platform::IBox<Platform::Boolean>^ Exact;
      property Platform::IBox<Platform::Boolean>^ Ideal;
    };

    public ref struct ConstrainBoolean sealed
    {
      property Platform::IBox<Platform::Boolean>^ Value;
      property ConstrainBooleanParameters^           Parameters;
    };

    public ref struct ConstrainLongRange sealed
    {
      property Platform::IBox<int32>^ Max;
      property Platform::IBox<int32>^ Min;

      property Platform::IBox<int32>^ Exact;
      property Platform::IBox<int32>^ Ideal;
    };

    public ref struct ConstrainLong sealed
    {
      property Platform::IBox<int32>^ Value;
      property ConstrainLongRange^    Range;
    };

    public ref struct ConstrainDoubleRange sealed
    {
      property Platform::IBox<float64>^ Max;
      property Platform::IBox<float64>^ Min;

      property Platform::IBox<float64>^ Exact;
      property Platform::IBox<float64>^ Ideal;
    };

    public ref struct ConstrainDouble sealed
    {
      property Platform::IBox<float64>^ Value;
      property ConstrainDoubleRange^    Range;
    };

    public ref struct StringOrStringList sealed
    {
      property Platform::String^            Value;
      property IVector<Platform::String^>^  Values;
    };
    public ref struct ConstrainStringParameters sealed
    {
      property StringOrStringList^ Exact;
      property StringOrStringList^ Ideal;
    };

    public ref struct ConstrainString sealed
    {
      property StringOrStringList^        Value;
      property ConstrainStringParameters^ Parameters;
    };
  }
}
