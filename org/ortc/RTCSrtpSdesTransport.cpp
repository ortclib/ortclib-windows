#include "pch.h"

#include <org/ortc/RTCSrtpSdesTransport.h>
#include <org/ortc/RTCIceTransport.h>
#include <org/ortc/helpers.h>
#include <org/ortc/Error.h>

#include <ortc/services/IHelper.h>

#include <zsLib/SafeInt.h>

using Platform::Collections::Vector;
using Platform::Array;
using Platform::Object;

using namespace ortc;

namespace Org
{
  namespace Ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper)

    namespace Internal
    {

#pragma region RTCSrtpSdesTransport conversions

      RTCSrtpSdesCryptoParameters^ ToCx(const ISRTPSDESTransportTypes::CryptoParameters &input)
      {
        auto result = ref new RTCSrtpSdesCryptoParameters();
        result->Tag = SafeInt<decltype(result->Tag)>(input.mTag);
        result->CryptoSuite = Helper::ToCx(input.mCryptoSuite);
        result->KeyParams = ref new Vector<RTCSrtpKeyParam^>();

        for (auto iter = input.mKeyParams.begin(); iter != input.mKeyParams.end(); ++iter)
        {
          auto keyParam = (*iter);
          result->KeyParams->Append(ToCx(keyParam));
        }

        result->SessionParams = ref new Vector<Platform::String^>();
        for (auto iter = input.mSessionParams.begin(); iter != input.mSessionParams.end(); ++iter)
        {
          auto sessionParam = (*iter);
          result->SessionParams->Append(Helper::ToCx(sessionParam));
        }

        return result;
      }

      RTCSrtpSdesCryptoParameters^ ToCx(ISRTPSDESTransportTypes::CryptoParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      ISRTPSDESTransportTypes::CryptoParametersPtr FromCx(RTCSrtpSdesCryptoParameters^ input)
      {
        if (nullptr == input) return ISRTPSDESTransportTypes::CryptoParametersPtr();
        auto result = make_shared<ISRTPSDESTransportTypes::CryptoParameters>();
        result->mTag = SafeInt<decltype(result->mTag)>(input->Tag);
        result->mCryptoSuite = Helper::FromCx(input->CryptoSuite);
        if (nullptr != input->KeyParams)
        {
          for (auto params : input->KeyParams)
          {
            auto convertedParam = FromCx(params);
            if (convertedParam)
            {
              result->mKeyParams.push_back(*convertedParam);
            }
          }
        }
        if (nullptr != input->SessionParams)
        {
          for (auto params : input->SessionParams)
          {
            auto convertedParam = Helper::FromCx(params);
            result->mSessionParams.push_back(convertedParam);
          }
        }

        return result;
      }

      RTCSrtpSdesParameters^ ToCx(const ISRTPSDESTransportTypes::Parameters &input)
      {
        auto result = ref new RTCSrtpSdesParameters();

        for (auto iter = input.mCryptoParams.begin(); iter != input.mCryptoParams.end(); ++iter)
        {
          auto param = (*iter);
          result->CryptoParams->Append(ToCx(param));
        }

        return result;
      }

      RTCSrtpSdesParameters^ ToCx(ISRTPSDESTransportTypes::ParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      ISRTPSDESTransportTypes::ParametersPtr FromCx(RTCSrtpSdesParameters^ input)
      {
        if (nullptr == input) return ISRTPSDESTransportTypes::ParametersPtr();
        auto result = make_shared<ISRTPSDESTransportTypes::Parameters>();
        if (nullptr != input->CryptoParams)
        {
          for (auto params : input->CryptoParams)
          {
            auto convertedParam = FromCx(params);
            if (convertedParam)
            {
              result->mCryptoParams.push_back(*convertedParam);
            }
          }
        }

        return result;
      }

      RTCSrtpKeyParam^ ToCx(const ISRTPSDESTransportTypes::KeyParameters &input)
      {
        auto result = ref new RTCSrtpKeyParam();
        result->KeyMethod = Helper::ToCx(input.mKeyMethod);
        result->KeySalt = Helper::ToCx(input.mKeySalt);
        result->Lifetime = Helper::ToCx(input.mLifetime);
        result->MkiValue = Helper::ToCx(input.mMKIValue);
        result->MkiLength = SafeInt<decltype(result->MkiLength)>(input.mMKILength);
        return result;
      }
      RTCSrtpKeyParam^ ToCx(ISRTPSDESTransportTypes::KeyParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }
      ISRTPSDESTransportTypes::KeyParametersPtr FromCx(RTCSrtpKeyParam^ input)
      {
        if (nullptr == input) return ISRTPSDESTransportTypes::KeyParametersPtr();
        auto result = make_shared<ISRTPSDESTransportTypes::KeyParameters>();
        result->mKeyMethod = Helper::FromCx(input->KeyMethod);
        result->mKeySalt = Helper::FromCx(input->KeySalt);
        result->mMKIValue = Helper::FromCx(input->MkiValue);
        result->mMKILength = SafeInt<decltype(result->mMKILength)>(input->MkiLength);
        return result;
      }

#pragma endregion

#pragma region RTCSrtpSdesTransport delegates

      class RTCSrtpSdesTransportDelegate : public ISRTPSDESTransportDelegate
      {
      public:
        RTCSrtpSdesTransportDelegate(RTCSrtpSdesTransport^ owner) { _owner = owner; }

        virtual void onSRTPSDESTransportLifetimeRemaining(
          ISRTPSDESTransportPtr transport,
          ULONG leastLifetimeRemainingPercentageForAllKeys,
          ULONG overallLifetimeRemainingPercentage
          )
        {
          auto evt = ref new RTCSrtpSdesTransportLifetimeRemainingEvent();
          evt->_leastLifetimeRemainingPercentageForAllKeys = SafeInt<decltype(evt->_leastLifetimeRemainingPercentageForAllKeys)>(leastLifetimeRemainingPercentageForAllKeys);
          evt->_overallLifetimeRemainingPercentage = SafeInt<decltype(evt->_overallLifetimeRemainingPercentage)>(overallLifetimeRemainingPercentage);
          _owner->OnLifetimeRemaining(evt);
        }

        virtual void onSRTPSDESTransportError(
          ISRTPSDESTransportPtr transport,
          ErrorAnyPtr error
          )
        {
          auto evt = ref new ErrorEvent(Error::CreateIfGeneric(error));
          _owner->OnError(evt);
        }

      private:
        RTCSrtpSdesTransport^ _owner;
      };

#pragma endregion

    } // namespace internal

#pragma region RTCSrtpSdesTransport

    RTCSrtpSdesTransport::RTCSrtpSdesTransport(ISRTPSDESTransportPtr transport) :
      _nativeDelegatePointer(make_shared<Internal::RTCSrtpSdesTransportDelegate>(this)),
      _nativePointer(transport)
    {
      if (_nativePointer) {
        _nativeSubscriptionPointer = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    RTCSrtpSdesTransport::RTCSrtpSdesTransport(RTCIceTransport^ transport, RTCSrtpSdesCryptoParameters^ encryptParameters, RTCSrtpSdesCryptoParameters^ decryptParameters) :
      _nativeDelegatePointer(make_shared<Internal::RTCSrtpSdesTransportDelegate>(this))
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == transport)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == encryptParameters)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == decryptParameters)

      auto nativeTransport = RTCIceTransport::Convert(transport);

      try
      {
        _nativePointer = ISRTPSDESTransport::create(_nativeDelegatePointer, nativeTransport, *Internal::FromCx(encryptParameters), *Internal::FromCx(decryptParameters));
      }
      catch (const InvalidParameters &)
      {
        ORG_ORTC_THROW_INVALID_PARAMETERS()
      }
      catch (const InvalidStateError &e)
      {
        ORG_ORTC_THROW_INVALID_STATE(UseHelper::ToCx(e.what()))
      }
    }

    RTCSrtpSdesParameters^ RTCSrtpSdesTransport::GetLocalParameters()
    {
      auto params = ISRTPSDESTransport::getLocalParameters();
      return Internal::ToCx(params);
    }

#pragma endregion

  } // namespace ortc
} // namespace org
