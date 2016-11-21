#pragma once

namespace Org
{
  namespace Ortc
  {
    /// <summary>
    /// Controls the detailed settings of how the ORTC engine operates.
    /// </summary>
    public ref class Settings sealed
    {
    public:
      /// <summary>
      /// Apply all the default settings for how the ORTC engine should
      /// behave.
      /// </summary>
      static void ApplyDefaults();

      /// <summary>
      /// Apply a collection of json based settings.
      /// </summary>
      static Platform::Boolean Apply(Platform::String ^jsonSettings);

      /// <summary>
      /// Apply a specific string setting with a key and value.
      /// </summary>
      static void SetString(Platform::String ^key, Platform::String ^value);

      /// <summary>
      /// Apply a specific integer setting with a key and value.
      /// </summary>
      static void SetInt(
        Platform::String ^key,
        int64 value
      );
      /// <summary>
      /// Apply a specific unsigned integer setting with a key and value.
      /// </summary>
      static void SetUInt(
        Platform::String ^key,
        uint64 value
      );
      /// <summary>
      /// Apply a specific boolean setting with a key and value.
      /// </summary>
      static void SetBool(
        Platform::String ^key,
        Platform::Boolean value
      );
      /// <summary>
      /// Apply a specific floating point setting with a key and value.
      /// </summary>
      static void SetFloat(
        Platform::String ^key,
        float32 value
      );
      /// <summary>
      /// Apply a specific double floating point setting with a key and value.
      /// </summary>
      static void SetDouble(
        Platform::String ^key,
        float64 value
      );

      /// <summary>
      /// Clear a specific setting by its key.
      /// </summary>
      static void Clear(Platform::String ^key);

      /// <summary>
      /// Clear all known settings.
      /// </summary>
      static void ClearAll();
    };
  }
}
