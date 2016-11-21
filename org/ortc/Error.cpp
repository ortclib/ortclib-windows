#include "pch.h"

#include <org/ortc/Error.h>
#include <org/ortc/helpers.h>

#include <zsLib/SafeInt.h>

using namespace ortc;

namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper)

    Error^ Error::CreateIfGeneric(AnyPtr any)
    {
      if (!any) return nullptr;

      auto genericError = ZS_DYNAMIC_PTR_CAST(ErrorAny, any);
      return CreateIfGeneric(genericError);
    }

    Error^ Error::CreateIfGeneric(ErrorAnyPtr genericError)
    {
      if (!genericError) return nullptr;

      Error ^ error = ref new Error();
      error->Name = UseHelper::ToCx(genericError->mName);
      error->Reason = UseHelper::ToCx(genericError->mReason);
      error->ErrorCode = SafeInt<decltype(error->ErrorCode)>(genericError->mErrorCode);

      return error;
    }


  } // namespace ortc
} // namespace org
