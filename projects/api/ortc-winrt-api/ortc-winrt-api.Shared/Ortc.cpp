#include "pch.h"
#include "Ortc.h"
#include <ortc/types.h>
#include <ortc/IORTC.h>
//#include "helpers.h"


using namespace ortc;
using namespace ortc_winrt_api;

void Ortc::Setup()
{
  IORTC::setup(IMessageQueuePtr());
}
