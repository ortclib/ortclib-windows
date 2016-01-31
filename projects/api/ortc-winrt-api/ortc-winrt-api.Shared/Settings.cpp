#include "pch.h"
#include "Settings.h"
#include <ortc/types.h>
#include <ortc/ISettings.h>
//#include "helpers.h"


using namespace ortc;
using namespace ortc_winrt_api;

void Settings::ApplyDefaults()
{
  ISettings::applyDefaults();
}
