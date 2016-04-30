#include "pch.h"

#include "Settings.h"

#include <ortc/ISettings.h>


using namespace ortc;

namespace org
{
  namespace ortc
  {
    void Settings::ApplyDefaults()
    {
      ISettings::applyDefaults();
    }
  }
}
