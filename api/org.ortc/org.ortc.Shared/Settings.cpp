#include "pch.h"
#include "Settings.h"
#include <ortc/types.h>
#include <ortc/ISettings.h>
//#include "helpers.h"


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
