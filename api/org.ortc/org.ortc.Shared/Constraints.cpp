
#include "pch.h"

#include "Constraints.h"
#include "helpers.h"

using Platform::Collections::Vector;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    namespace internal
    {

      ConstrainBooleanParameters^ ToCx(const IConstraints::ConstrainBooleanParameters &input)
      {
        auto result = ref new ConstrainBooleanParameters();
        result->Exact = Helper::ToCx(input.mExact);
        result->Ideal = Helper::ToCx(input.mIdeal);
        return result;
      }

      ConstrainBooleanParameters^ ToCx(const Optional<IConstraints::ConstrainBooleanParameters> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<IConstraints::ConstrainBooleanParameters> FromCx(ConstrainBooleanParameters^ input)
      {
        Optional<IConstraints::ConstrainBooleanParameters> result;
        if (nullptr == input) return result;

        result = IConstraints::ConstrainBooleanParameters();
        result.value().mExact = Helper::FromCx(input->Exact);
        result.value().mIdeal = Helper::FromCx(input->Ideal);
        return result;
      }

      ConstrainBoolean^ ToCx(const IConstraints::ConstrainBoolean &input)
      {
        auto result = ref new ConstrainBoolean();
        result->Value = Helper::ToCx(input.mValue);
        result->Parameters = ToCx(input.mParameters);
        return result;
      }

      IConstraints::ConstrainBoolean FromCx(ConstrainBoolean^ input)
      {
        IConstraints::ConstrainBoolean result;
        if (nullptr == input) return result;
        result.mValue = Helper::FromCx(input->Value);
        result.mParameters = FromCx(input->Parameters);
        return result;
      }

      ConstrainLongRange^ ToCx(const IConstraints::ConstrainLongRange &input)
      {
        auto result = ref new ConstrainLongRange();
        result->Max = Helper::ToCx(input.mMax);
        result->Min = Helper::ToCx(input.mMin);
        result->Exact = Helper::ToCx(input.mExact);
        result->Ideal = Helper::ToCx(input.mIdeal);
        return result;
      }

      ConstrainLongRange^ ToCx(const Optional<IConstraints::ConstrainLongRange> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<IConstraints::ConstrainLongRange> FromCx(ConstrainLongRange^ input)
      {
        Optional<IConstraints::ConstrainLongRange> result;
        if (nullptr == input) return result;

        result = IConstraints::ConstrainLongRange();
        result.value().mMax = Helper::FromCx(input->Max);
        result.value().mMin = Helper::FromCx(input->Min);
        result.value().mExact = Helper::FromCx(input->Exact);
        result.value().mIdeal = Helper::FromCx(input->Ideal);
        return result;
      }

      ConstrainLong^ ToCx(const IConstraints::ConstrainLong &input)
      {
        auto result = ref new ConstrainLong();
        result->Value = Helper::ToCx(input.mValue);
        result->Range = ToCx(input.mRange);
        return result;
      }

      IConstraints::ConstrainLong FromCx(ConstrainLong^ input)
      {
        IConstraints::ConstrainLong result;
        if (nullptr == input) return result;
        result.mValue = Helper::FromCx(input->Value);
        result.mRange = FromCx(input->Range);
        return result;
      }

      ConstrainDoubleRange^ ToCx(const IConstraints::ConstrainDoubleRange &input)
      {
        auto result = ref new ConstrainDoubleRange();
        result->Max = Helper::ToCx(input.mMax);
        result->Min = Helper::ToCx(input.mMin);
        result->Exact = Helper::ToCx(input.mExact);
        result->Ideal = Helper::ToCx(input.mIdeal);
        return result;
      }

      ConstrainDoubleRange^ ToCx(const Optional<IConstraints::ConstrainDoubleRange> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<IConstraints::ConstrainDoubleRange> FromCx(ConstrainDoubleRange^ input)
      {
        Optional<IConstraints::ConstrainDoubleRange> result;
        if (nullptr == input) return result;

        result = IConstraints::ConstrainDoubleRange();
        result.value().mMax = Helper::FromCx(input->Max);
        result.value().mMin = Helper::FromCx(input->Min);
        result.value().mExact = Helper::FromCx(input->Exact);
        result.value().mIdeal = Helper::FromCx(input->Ideal);
        return result;
      }

      ConstrainDouble^ ToCx(const IConstraints::ConstrainDouble &input)
      {
        auto result = ref new ConstrainDouble();
        result->Value = Helper::ToCx(input.mValue);
        result->Range = ToCx(input.mRange);
        return result;
      }

      IConstraints::ConstrainDouble FromCx(ConstrainDouble^ input)
      {
        IConstraints::ConstrainDouble result;
        if (nullptr == input) return result;
        result.mValue = Helper::FromCx(input->Value);
        result.mRange = FromCx(input->Range);
        return result;
      }

      ConstrainString^ ToCx(const IConstraints::ConstrainString &input)
      {
        auto result = ref new ConstrainString();
        result->Value = ToCx(input.mValue);
        result->Parameters = ToCx(input.mParameters);
        return result;
      }

      IConstraints::ConstrainString FromCx(ConstrainString^ input)
      {
        IConstraints::ConstrainString result;
        if (nullptr == input) return result;
        result.mValue = FromCx(input->Value);
        result.mParameters = FromCx(input->Parameters);
        return result;
      }

      StringOrStringList^ ToCx(const IConstraints::StringOrStringList &input)
      {
        auto result = ref new StringOrStringList();
        result->Value = Helper::ToCx(input.mValue);
        result->Values = ToCx(input.mValues);
        return result;
      }

      Optional<IConstraints::StringOrStringList> FromCx(StringOrStringList^ input)
      {
        Optional<IConstraints::StringOrStringList> result;
        if (nullptr == input) return result;

        result = IConstraints::StringOrStringList();
        result.value().mValue = Helper::FromCxToOptional(input->Value);
        result.value().mValues = FromCx(input->Values);
        return result;
      }


      ConstrainStringParameters^ ToCx(const IConstraints::ConstrainStringParameters &input)
      {
        auto result = ref new ConstrainStringParameters();
        result->Exact = ToCx(input.mExact);
        result->Ideal = ToCx(input.mIdeal);
        return result;
      }

      ConstrainStringParameters^ ToCx(const Optional<IConstraints::ConstrainStringParameters> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<IConstraints::ConstrainStringParameters> FromCx(ConstrainStringParameters^ input)
      {
        Optional<IConstraints::ConstrainStringParameters> result;
        if (nullptr == input) return result;

        result = IConstraints::ConstrainStringParameters();
        result.value().mExact = FromCx(input->Exact);
        result.value().mIdeal = FromCx(input->Ideal);
        return result;
      }

      IVector<Platform::String^>^ ToCx(const IConstraints::StringList &input)
      {
        if (input.size() < 1) return nullptr;
        auto result = ref new Vector<Platform::String^>();
        for (auto iter = input.begin(); iter != input.end(); ++iter)
        {
          auto &value = (*iter);
          result->Append(Helper::ToCx(value));
        }
        return result;
      }

      IVector<Platform::String^>^ ToCx(const Optional<IConstraints::StringList> &input)
      {
        if (!input.hasValue()) return nullptr;
        return ToCx(input.value());
      }

      Optional<IConstraints::StringList> FromCx(IVector<Platform::String^>^ input)
      {
        Optional<IConstraints::StringList> result;
        if (nullptr == input) return result;
        result = IConstraints::StringList();
        for (Platform::String^ value : input)
        {
          result.value().push_back(Helper::FromCx(value));
        }
        return result;
      }

    } // namespace internal
  } // namespace ortc
} // namespace org
