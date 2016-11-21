#include "pch.h"

#include <org/ortc/RTCCertificate.h>
#include <org/ortc/RTCDtlsTransport.h>
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
    ZS_DECLARE_TYPEDEF_PTR(Internal::Helper, UseHelper);
    ZS_DECLARE_TYPEDEF_PTR(::ortc::services::IHelper, UseServicesHelper);


    namespace Internal
    {

#pragma region RTCDtlsTransport conversions

      RTCDtlsFingerprint^ ToCx(const ICertificateTypes::Fingerprint &input)
      {
        auto result = ref new RTCDtlsFingerprint();
        result->Algorithm = Helper::ToCx(input.mAlgorithm);
        result->Value = Helper::ToCx(input.mValue);
        return result;
      }

      RTCDtlsFingerprint^ ToCx(ICertificateTypes::FingerprintPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      ICertificateTypes::FingerprintPtr FromCx(RTCDtlsFingerprint^ input)
      {
        if (nullptr == input) return ICertificateTypes::FingerprintPtr();
        auto result = make_shared<ICertificateTypes::Fingerprint>();
        result->mAlgorithm = Helper::FromCx(input->Algorithm);
        result->mValue = Helper::FromCx(input->Value);
        return result;
      }

      RTCDtlsParameters^ ToCx(const IDTLSTransportTypes::Parameters &input)
      {
        auto result = ref new RTCDtlsParameters();
        result->Role = Helper::Convert(input.mRole);
        if (input.mFingerprints.size() > 0)
        {
          result->Fingerprints = ref new Vector<RTCDtlsFingerprint^>();
          for (auto iter = input.mFingerprints.begin(); iter != input.mFingerprints.end(); ++iter)
          {
            auto &value = (*iter);
            result->Fingerprints->Append(ToCx(value));
          }
        }
        return result;
      }

      RTCDtlsParameters^ ToCx(IDTLSTransportTypes::ParametersPtr input)
      {
        if (!input) return nullptr;
        return ToCx(*input);
      }

      IDTLSTransportTypes::ParametersPtr FromCx(RTCDtlsParameters^ input)
      {
        if (nullptr == input) return IDTLSTransportTypes::ParametersPtr();
        auto result = make_shared<IDTLSTransportTypes::Parameters>();
        result->mRole = Helper::Convert(input->Role);
        if (input->Fingerprints)
        {
          for (RTCDtlsFingerprint^ value : input->Fingerprints)
          {
            if (nullptr == value) continue;
            result->mFingerprints.push_front(*FromCx(value));
          }
        }
        return result;
      }

#pragma endregion

#pragma region RTCDtlsTransport delegates

      class RTCDtlsTransportDelegate : public IDTLSTransportDelegate
      {
      public:
        RTCDtlsTransportDelegate(RTCDtlsTransport^ owner) { _owner = owner; }

        virtual void onDTLSTransportStateChange(
          IDTLSTransportPtr transport,
          IDTLSTransport::States state
          ) override
        {
          auto evt = ref new RTCDtlsTransportStateChangedEvent();
          evt->_state = UseHelper::Convert(state);
          _owner->OnStateChange(evt);
        }

        virtual void onDTLSTransportError(
          IDTLSTransportPtr transport,
          ErrorAnyPtr error
          ) override
        {
          auto evt = ref new ErrorEvent(Error::CreateIfGeneric(error));
          _owner->OnError(evt);
        }

      private:
        RTCDtlsTransport^ _owner;
      };

#pragma endregion

    } // namespace internal

#pragma region RTCDtlsTransport

    RTCDtlsTransport::RTCDtlsTransport(IDTLSTransportPtr transport) :
      _nativeDelegatePointer(make_shared<Internal::RTCDtlsTransportDelegate>(this)),
      _nativePointer(transport)
    {
      if (_nativePointer) {
        _nativeSubscriptionPointer = _nativePointer->subscribe(_nativeDelegatePointer);
      }
    }

    RTCDtlsTransport::RTCDtlsTransport(RTCIceTransport^ transport, IVector<RTCCertificate^>^ certificates) :
      _nativeDelegatePointer(make_shared<Internal::RTCDtlsTransportDelegate>(this))
    {
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(nullptr == certificates)

      auto nativeTransport = RTCIceTransport::Convert(transport);

      IDTLSTransport::CertificateList coreCertificates;
      for (RTCCertificate^ cert : certificates)
      {
        ICertificatePtr nativeCert = RTCCertificate::Convert(cert);
        ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!nativeCert)
        coreCertificates.push_back(nativeCert);
      }

      try
      {
        _nativePointer = IDTLSTransport::create(_nativeDelegatePointer, nativeTransport, coreCertificates);
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

    RTCDtlsParameters^ RTCDtlsTransport::GetLocalParameters()
    {
      if (!_nativePointer) return nullptr;
      return Internal::ToCx(_nativePointer->getLocalParameters());
    }

    RTCDtlsParameters^ RTCDtlsTransport::GetRemoteParameters()
    {
      if (!_nativePointer) return nullptr;
      return Internal::ToCx(_nativePointer->getRemoteParameters());
    }

    IVector<Object^>^ RTCDtlsTransport::GetRemoteCertificates()
    {
      auto ret = ref new Vector<Object^>();
      if (!_nativePointer) return ret;

      IDTLSTransportTypes::SecureByteBlockListPtr certificates = _nativePointer->getRemoteCertificates();

      if (certificates->size() > 0)
      {
        for (IDTLSTransportTypes::SecureByteBlockList::iterator it = certificates->begin(); it != certificates->end(); ++it)
        {
          Array<byte>^ arr = ref new Array<byte>((*it).BytePtr(), SafeInt<unsigned int>((*it).SizeInBytes()));
          ret->Append(arr);
        }
      }

      return ret;
    }

    void RTCDtlsTransport::Start(RTCDtlsParameters^ remoteParameters)
    {
      ORG_ORTC_THROW_INVALID_STATE_IF(!_nativePointer)
      ORG_ORTC_THROW_INVALID_PARAMETERS_IF(!remoteParameters)

      try
      {
        _nativePointer->start(*Internal::FromCx(remoteParameters));
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

    void RTCDtlsTransport::Stop()
    {
      if (!_nativePointer) return;
      _nativePointer->stop();
    }

    RTCDtlsTransportState RTCDtlsTransport::State::get()
    {
      if (!_nativePointer) return RTCDtlsTransportState::Closed;
      return UseHelper::Convert(_nativePointer->state());
    }

    IVector<RTCCertificate^>^ RTCDtlsTransport::Certificates::get()
    {
      auto ret = ref new Vector<RTCCertificate^>();
      if (!_nativePointer) return ret;

      IDTLSTransport::CertificateListPtr certificates = _nativePointer->certificates();
      for (IDTLSTransport::CertificateList::iterator it = certificates->begin(); it != certificates->end(); ++it)
      {
        ret->Append(RTCCertificate::Convert(*it));
      }
    
      return ret;
    }

    RTCIceTransport^ RTCDtlsTransport::Transport::get()
    {
      if (!_nativePointer) return nullptr;
      return RTCIceTransport::Convert(_nativePointer->transport());
    }


    Platform::String^ RTCDtlsParameters::ToJsonString()
    {
      auto params = Internal::FromCx(this);
      return UseHelper::ToCx(UseServicesHelper::toString(params->createElement("DtlsParameters")));
    }

    RTCDtlsParameters^ RTCDtlsParameters::FromJsonString(Platform::String^ jsonString)
    {
      auto params = make_shared<IDTLSTransport::Parameters>(IDTLSTransport::Parameters::Parameters(UseServicesHelper::toJSON(UseHelper::FromCx(jsonString).c_str())));
      return Internal::ToCx(params);
    }

#pragma endregion

  } // namespace ortc
} // namespace org
