
#include "cx_custom_Zs_MessageQueue.h"

#include <wrapper/generated/cx/cx_Helpers.h>

//------------------------------------------------------------------------------
::Zs::MessageQueue^ Zs::MessageQueue::ToCx(wrapper::zs::MessageQueuePtr value)
{
  if (!value) return nullptr;
  auto result = ref new MessageQueue(WrapperCreate{});
  result->native_ = value;
  return result;
}

//------------------------------------------------------------------------------
wrapper::zs::MessageQueuePtr Zs::MessageQueue::FromCx(::Zs::MessageQueue^ value)
{
  if (nullptr == value) return wrapper::zs::MessageQueuePtr();
  return value->native_;
}

//------------------------------------------------------------------------------
::Zs::MessageQueue^ Zs::MessageQueue::GetDefaultForUi()
{
  if (!native_) {throw ref new Platform::NullReferenceException();}
  ::Zs::MessageQueue^ result {};
  result = ::Internal::Helper::ToCx_Zs_MessageQueue(native_->getDefaultForUi());
  return result;
}
