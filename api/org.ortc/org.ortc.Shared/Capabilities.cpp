#include "pch.h"

#include "Capabilities.h"
#include "helpers.h"

#include <zsLib/SafeInt.h>

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

      CapabilityBoolean^ ToCx(const ICapabilities::CapabilityBoolean &input)
      {
        auto result = ref new CapabilityBoolean();
        if (input.size() < 1) return result;

        result->Values = ref new Vector<Platform::Boolean>();

        for (auto iter = input.begin(); iter != input.end(); ++iter)
        {
          auto &value = *iter;
          result->Values->Append(value);
        }
        return result;
      }

      CapabilityBoolean^ ToCx(const Optional<ICapabilities::CapabilityBoolean> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<ICapabilities::CapabilityBoolean> FromCx(CapabilityBoolean^ input)
      {
        Optional<ICapabilities::CapabilityBoolean> result;
        if (nullptr == input) return result;

        result = ICapabilities::CapabilityBoolean();
        if (input->Values->Size < 1) return result;

        for (Platform::Boolean value : input->Values)
        {
          result.value().insert(value);
        }
        return result;
      }


      CapabilityLong^ ToCx(const ICapabilities::CapabilityLong &input)
      {
        auto result = ref new CapabilityLong();
        result->Min = SafeInt<int32>(input.mMin);
        result->Max = SafeInt<int32>(input.mMax);
        return result;
      }

      CapabilityLong^ ToCx(const Optional<ICapabilities::CapabilityLong> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<ICapabilities::CapabilityLong> FromCx(CapabilityLong^ input)
      {
        Optional<ICapabilities::CapabilityLong> result;
        if (nullptr == input) return result;

        result = ICapabilities::CapabilityLong();
        result.value().mMin = SafeInt<decltype(ICapabilities::CapabilityLong::mMin)>(input->Min);
        result.value().mMax = SafeInt<decltype(ICapabilities::CapabilityLong::mMax)>(input->Max);
        return result;
      }

      CapabilityDouble^ ToCx(const ICapabilities::CapabilityDouble &input)
      {
        auto result = ref new CapabilityDouble();
        result->Min = static_cast<float64>(input.mMin);
        result->Max = static_cast<float64>(input.mMax);
        return result;
      }

      CapabilityDouble^ ToCx(const Optional<ICapabilities::CapabilityDouble> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<ICapabilities::CapabilityDouble> FromCx(CapabilityDouble^ input)
      {
        Optional<ICapabilities::CapabilityDouble> result;
        if (nullptr == input) return result;

        result = ICapabilities::CapabilityDouble();
        result.value().mMin = static_cast<decltype(ICapabilities::CapabilityDouble::mMin)>(input->Min);
        result.value().mMax = static_cast<decltype(ICapabilities::CapabilityDouble::mMax)>(input->Max);
        return result;
      }

      CapabilityString^ ToCx(const ICapabilities::CapabilityString &input)
      {
        auto result = ref new CapabilityString();
        if (input.size() < 1) return result;

        result->Values = ref new Vector<Platform::String^>();

        for (auto iter = input.begin(); iter != input.end(); ++iter)
        {
          auto &value = *iter;
          result->Values->Append(Helper::ToCx(value));
        }
        return result;
      }

      CapabilityString^ ToCx(const Optional<ICapabilities::CapabilityString> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<ICapabilities::CapabilityString> FromCx(CapabilityString^ input)
      {
        Optional<ICapabilities::CapabilityString> result;
        if (nullptr == input) return result;

        result = ICapabilities::CapabilityString();
        if (input->Values->Size < 1) return result;

        for (Platform::String^ value : input->Values)
        {
          result.value().insert(Helper::FromCx(value));
        }
        return result;
      }

    } // namespace internal

  } // namespace ortc
} // namespace org
