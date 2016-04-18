#include "pch.h"

#include "Error.h"
#include "helpers.h"

using namespace ortc;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    Error^ Error::CreateIfGeneric(AnyPtr any)
    {
      if (!any) return nullptr;

      auto genericError = ZS_DYNAMIC_PTR_CAST(ErrorAny, any);
      if (!genericError) return nullptr;

      Error ^ error = ref new Error();
      error->Name = UseHelper::ToCx(genericError->mName);

      return error;
    }

  } // namespace ortc
} // namespace org
