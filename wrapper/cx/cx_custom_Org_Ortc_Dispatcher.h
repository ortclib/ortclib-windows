
#pragma once

#include <wrapper/generated/cx/types.h>

#include <wrapper/generated/org_ortc_Dispatcher.h>

namespace Org {
  namespace Ortc {

    [Windows::Foundation::Metadata::WebHostHiddenAttribute]
    public ref class Dispatcher sealed
    {
    internal:
      static Windows::UI::Core::CoreDispatcher ^dispatcher_;

      wrapper::org::ortc::DispatcherPtr native_;

      struct WrapperCreate {};
      Dispatcher(const WrapperCreate &) {}

      static Dispatcher^ ToCx(wrapper::org::ortc::DispatcherPtr value);
      static wrapper::org::ortc::DispatcherPtr FromCx(Dispatcher^ value);

    public:
      // ::org::ortc::Dispatcher
      Dispatcher(Windows::UI::Core::CoreDispatcher^ dispatcher);

      static property Windows::UI::Core::CoreDispatcher^ Source { Windows::UI::Core::CoreDispatcher^ get(); }
    };

  } // namespace Ortc
} // namespace Org
