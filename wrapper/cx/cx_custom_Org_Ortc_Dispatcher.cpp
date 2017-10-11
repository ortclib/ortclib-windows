
#include <wrapper/generated/cx/cx_Helpers.h>

#include "cx_custom_Org_Ortc_Dispatcher.h"

typedef zsLib::AnyHolder< Windows::UI::Core::CoreDispatcher^ > AnyDispatchHolder;

Windows::UI::Core::CoreDispatcher ^Org::Ortc::Dispatcher::dispatcher_ = nullptr;

//------------------------------------------------------------------------------
::Org::Ortc::Dispatcher^ Org::Ortc::Dispatcher::ToCx(wrapper::org::ortc::DispatcherPtr value)
{
  if (!value) return nullptr;
  auto result = ref new Dispatcher(WrapperCreate{});
  result->native_ = value;
  return result;
}

//------------------------------------------------------------------------------
wrapper::org::ortc::DispatcherPtr Org::Ortc::Dispatcher::FromCx(::Org::Ortc::Dispatcher^ value)
{
  if (nullptr == value) return wrapper::org::ortc::DispatcherPtr();
  return value->native_;
}

//------------------------------------------------------------------------------
Org::Ortc::Dispatcher::Dispatcher(Windows::UI::Core::CoreDispatcher^ dispatcher)
 : native_(wrapper::org::ortc::Dispatcher::wrapper_create())
{
  dispatcher_ = dispatcher;

  if (!native_) {throw ref new Platform::NullReferenceException();}

  auto holder = make_shared<AnyDispatchHolder>();
  holder->value_ = dispatcher;
  native_->wrapper_init_org_ortc_Dispatcher(holder);
}

//------------------------------------------------------------------------------
Windows::UI::Core::CoreDispatcher^ Org::Ortc::Dispatcher::Source::get()
{
  return dispatcher_;
}
