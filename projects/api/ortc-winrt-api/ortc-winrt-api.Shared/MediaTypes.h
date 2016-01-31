#pragma once
#include <collection.h>

using Windows::Foundation::Collections::IVector;

namespace ortc_winrt_api
{
  public ref struct LongRange sealed
  {
    property uint32 Max;
    property uint32 Min;
  };

  public ref struct ConstrainLongRange sealed //: LongRange
  {
    property uint32 Max;
    property uint32 Min;

    property uint32 Exact;
    property uint32 Ideal;
  };

  public ref struct ConstrainLong sealed //: ConstrainLongRange
  {
    property uint32 Max;
    property uint32 Min;

    property uint32 Exact;
    property uint32 Ideal;

    property uint32 Value;
  };

  public ref struct DoubleRange sealed
  {
    property double  Max;
    property double  Min;
  };

  public ref struct ConstrainDoubleRange sealed //: DoubleRange
  {
    property double  Max;
    property double  Min;

    property double  Exact;
    property double  Ideal;
  };

  public ref struct ConstrainDouble sealed //: ConstrainDoubleRange
  {
    property double  Max;
    property double  Min;

    property double  Exact;
    property double  Ideal;

    property double  Value;
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

  public ref struct ConstrainBoolParameters sealed
  {
    property bool Exact;
    property bool Ideal;
  };

  public ref struct ConstrainBool sealed
  {
    property bool      Value;
    property ConstrainBoolParameters^  Parameters;
  };

  public ref struct SupportedConstraints sealed
  {
    property bool Width;
    property bool Height;
    property bool AspectRatio;
    property bool FrameRate;
    property bool FacingMode;
    property bool Volume;
    property bool SampleRate;
    property bool SampleSize;
    property bool EchoCancellation;
    property bool Latency;
    property bool DeviceID;
    property bool GroupID;
  };

  public enum class MediaDeviceKinds
  {
    AudioInput,
    AudioOutput,
    Video,
  };

  public ref struct MediaDeviceInfo sealed 
  {
    property MediaDeviceKinds Kind;

    property Platform::String^ Label;
    property Platform::String^ DeviceID;
    property Platform::String^ GroupID;

    property SupportedConstraints^ SupportedConstraints;
  };
}