#include "pch.h"

#include <org/ortc/Settings.h>
#include <org/ortc/helpers.h>

#include <ortc/ISettings.h>

#include <zsLib/SafeInt.h>

using namespace ortc;

namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper);

    void Settings::ApplyDefaults()
    {
      ISettings::applyDefaults();
    }

    Platform::Boolean Settings::Apply(Platform::String ^jsonSettings)
    {
      return ISettings::apply(UseHelper::FromCx(jsonSettings).c_str());
    }

    void Settings::SetString(Platform::String ^key, Platform::String ^value)
    {
      ISettings::setString(UseHelper::FromCx(key).c_str(), UseHelper::FromCx(value).c_str());
    }
    void Settings::SetInt(Platform::String ^key, int64 value)
    {
      ISettings::setInt(UseHelper::FromCx(key).c_str(), SafeInt<LONG>(value));
    }
    void Settings::SetUInt(Platform::String ^key, uint64 value)
    {
      ISettings::setInt(UseHelper::FromCx(key).c_str(), SafeInt<ULONG>(value));
    }
    void Settings::SetBool(Platform::String ^key, Platform::Boolean value)
    {
      ISettings::setBool(UseHelper::FromCx(key).c_str(), value);
    }
    void Settings::SetFloat(Platform::String ^key, float32 value)
    {
      ISettings::setFloat(UseHelper::FromCx(key).c_str(), value);
    }
    void Settings::SetDouble(Platform::String ^key, float64 value)
    {
      ISettings::setDouble(UseHelper::FromCx(key).c_str(), value);
    }

    void Settings::Clear(Platform::String ^key)
    {
      return ISettings::clear(UseHelper::FromCx(key).c_str());
    }

    void Settings::ClearAll()
    {
      ISettings::clearAll();
    }

  }
}
