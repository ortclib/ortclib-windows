#pragma once
#include <collection.h>

using Windows::Foundation::Collections::IVector;

namespace org
{
  namespace ortc
  {
    public ref struct ConstrainBoolParameters sealed
    {
      property Platform::IBox<Platform::Boolean>^ Exact;
      property Platform::IBox<Platform::Boolean>^ Ideal;
    };

    public ref struct ConstrainBool sealed
    {
      property Platform::IBox<Platform::Boolean>^ Value;
      property ConstrainBoolParameters^           Parameters;
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
