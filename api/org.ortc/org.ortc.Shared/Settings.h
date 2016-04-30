#pragma once

namespace org
{
  namespace ortc
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
    };
  }
}
