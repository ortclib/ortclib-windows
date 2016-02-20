#pragma once
#include <collection.h>

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
  public ref struct CapabilityBool sealed
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