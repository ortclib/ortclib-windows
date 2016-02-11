#include "pch.h"
#include "Ortc.h"
#include <ortc/types.h>
#include <ortc/IORTC.h>
#include <zsLib/helpers.h>
#include <zsLib/MessageQueueThread.h>
//#include "helpers.h"

using namespace ortc;
using namespace ortc_winrt_api;

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

void Ortc::Setup()
{
  IORTC::setup(IMessageQueuePtr());
}

CoreDispatcher ^OrtcWithDispatcher::_dispatcher = nullptr;

void OrtcWithDispatcher::Setup(Windows::UI::Core::CoreDispatcher ^dispatcher)
{
#define TODO_THIS_FEELS_WRONG_USING_GLOBAL 1
#define TODO_THIS_FEELS_WRONG_USING_GLOBAL 2
  g_windowDispatcher = _dispatcher = dispatcher;
  zsLib::setup(dispatcher);
  IORTC::setup(IMessageQueuePtr());
}
