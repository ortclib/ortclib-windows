#pragma once

//#include <ortc/ICertificate.h>
#include <ortc/ISRTPSDESTransport.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISRTPSDESTransportDelegate, ISRTPSDESTransportDelegate)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISRTPSDESTransport, ISRTPSDESTransport)
    ZS_DECLARE_TYPEDEF_PTR(::ortc::ISRTPSDESTransportSubscription, ISRTPSDESTransportSubscription)

    using Windows::Foundation::Collections::IVector;

    ref class RTCIceTransport;
    ref class RTCStatsProvider;

    ref struct ErrorEvent;

    ref struct RTCSrtpSdesCryptoParameters;
    ref struct RTCSrtpSdesParameters;
    ref struct RTCSrtpKeyParam;

    namespace internal
    {
      ZS_DECLARE_CLASS_PTR(RTCSrtpSdesTransportDelegate)

      ZS_DECLARE_TYPEDEF_PTR(::ortc::ISRTPSDESTransportTypes, ISRTPSDESTransportTypes)

      RTCSrtpSdesCryptoParameters^ ToCx(const ISRTPSDESTransportTypes::CryptoParameters &input);
      RTCSrtpSdesCryptoParameters^ ToCx(ISRTPSDESTransportTypes::CryptoParametersPtr input);
      ISRTPSDESTransportTypes::CryptoParametersPtr FromCx(RTCSrtpSdesCryptoParameters^ input);

      RTCSrtpSdesParameters^ ToCx(const ISRTPSDESTransportTypes::Parameters &input);
      RTCSrtpSdesParameters^ ToCx(ISRTPSDESTransportTypes::ParametersPtr input);
      ISRTPSDESTransportTypes::ParametersPtr FromCx(RTCSrtpSdesParameters^ input);

      RTCSrtpKeyParam^ ToCx(const ISRTPSDESTransportTypes::KeyParameters &input);
      RTCSrtpKeyParam^ ToCx(ISRTPSDESTransportTypes::KeyParametersPtr input);
      ISRTPSDESTransportTypes::KeyParametersPtr FromCx(RTCSrtpKeyParam^ input);
    }


    /// <summary>
    /// The RTCSrtpSdesCryptoParameters object includes information relating
    /// to SRTP/SDES cryptographic configuration.
    /// </summary>
    public ref struct RTCSrtpSdesCryptoParameters sealed
    {
    public:
      /// <summary>
      /// Gets or sets the tag identifying the parameters set, in the syntax
      /// defined for 'tag' in [RFC4568] Section 9.1.
      /// </summary>
      property uint16                       Tag;

      /// <summary>
      /// Gets or sets the the ciphersuite to be utilized, in the syntax
      /// defined for 'srtp-crypto-suite' in [RFC4568] Section 9.2;
      /// </summary>
      property Platform::String^            CryptoSuite;

      /// <summary>
      /// Gets or sets the key parameters to be utilized, as described in
      /// [RFC4568] Section 6.1. Calls to
      /// RTCSrtpSdesTransport.GetLocalParameters should return only a single
      /// RTCSrtpKeyParam dictionary entry.
      /// </summary>
      property IVector<RTCSrtpKeyParam^>^   KeyParams;

      /// <summary>
      /// Gets or sets a sequence of session parameters to be utilized, each
      /// in the syntax defined for 'session-param' in [RFC4568] Section 9.1;
      /// </summary>
      property IVector<Platform::String^>^  SessionParams;
    };

    /// <summary>
    /// The RTCSrtpSdesParameters object includes information relating to
    /// SRTP/SDES configuration.
    /// </summary>
    public ref struct RTCSrtpSdesParameters sealed
    {
    public:
      /// <summary>
      /// Gets or sets a sequence of cryptographic parameters.
      /// </summary>
      property IVector<RTCSrtpSdesCryptoParameters^>^   CryptoParams;
    };


    /// <summary>
    /// The RTCSrtpKeyParam objects defines the keying material needed for a
    /// SRTP SDES session.
    /// </summary>
    public ref struct RTCSrtpKeyParam sealed
    {
    public:
      /// <summary>
      /// Gets or sets the key method to be utilized, in the syntax defined
      /// for 'srtp-key-method' in [RFC4568] Section 9.2.
      /// </summary>
      property Platform::String^            KeyMethod;

      /// <summary>
      /// Gets or sets the key and salt to be utilized, in the syntax defined
      /// for 'key-salt' in [RFC4568] Sections 9.2.
      /// </summary>
      property Platform::String^            KeySalt;

      /// <summary>
      /// Gets or sets the key lifetime to be utilized, in the syntax defined
      /// for 'lifetime' in [RFC4568] Section 9.2.
      /// </summary>
      property Platform::String^            Lifetime;

      /// <summary>
      /// Gets or sets the master key index to be utilized, in the syntax
      /// defined for 'mki-value' in [RFC4568] Section 9.2.
      /// </summary>
      property Platform::String^            MkiValue;

      /// <summary>
      /// Gets or sets the master key index length to be utilized, in the
      /// syntax defined for 'mki-length' in [RFC4568] Section 9.2.
      /// </summary>
      property uint16                       MkiLength;
    };


    /// <summary>
    /// This is the event data associated with the
    /// transport.OnLifetimeRemaining event handler.
    /// </summary>
    public ref struct RTCSrtpSdesTransportLifetimeRemainingEvent sealed
    {
      friend class internal::RTCSrtpSdesTransportDelegate;

      /// <summary>
      /// Gets the least percentage remaining of all the keys in the
      /// SDES/SRTP transport.
      /// </summary>
      property uint32 LeastLifetimeRemainingPercentageForAllKeys
      {
        uint32 get() { return _leastLifetimeRemainingPercentageForAllKeys; }
      }

      /// <summary>
      /// Gets the overall percentage of life remaining of all the keys in the
      /// SDES/SRTP transport.
      /// </summary>
      property uint32 OverallLifetimeRemainingPercentage
      {
        uint32 get() { return _overallLifetimeRemainingPercentage; }
      }

    private:
      uint32 _leastLifetimeRemainingPercentageForAllKeys;
      uint32 _overallLifetimeRemainingPercentage;
    };


    public delegate void RTCSrtpSdesTransportLifetimeRemainingDelegate(RTCSrtpSdesTransportLifetimeRemainingEvent^ evt);
    public delegate void RTCSrtpSdesTransportErrorDelegate(ErrorEvent^ evt);

    /// <summary>
    /// The RTCSrtpSdesTransport includes information relating to SRTP/SDES
    /// transport.
    /// </summary>
    public ref class RTCSrtpSdesTransport sealed
    {
      friend class internal::RTCSrtpSdesTransportDelegate;
      friend ref class RTCRtpReceiver;
      friend ref class RTCRtpSender;
      friend ref class RTCRtpListener;
      friend ref class RTCStatsProvider;

    private:
      RTCSrtpSdesTransport(ISRTPSDESTransportPtr transport);

      static RTCSrtpSdesTransport^ Convert(ISRTPSDESTransportPtr transport) { if (!transport) return nullptr; return ref new RTCSrtpSdesTransport(transport); }
      static ISRTPSDESTransportPtr Convert(RTCSrtpSdesTransport^ transport) { if (!transport) return nullptr; return transport->_nativePointer; }

    public:
      /// <summary>
      /// </summary>
      RTCSrtpSdesTransport(RTCIceTransport^ transport, RTCSrtpSdesCryptoParameters^ encryptParameters, RTCSrtpSdesCryptoParameters^ decryptParameters);

      /// <summary>
      /// Obtain the local SRTP/SDES parameter sets.
      /// </summary>
      static RTCSrtpSdesParameters^ GetLocalParameters();


      /// <summary>
      /// This event is fired when the lifetime of the keying material
      /// percentage remaining has changed.
      /// </summary>
      event RTCSrtpSdesTransportLifetimeRemainingDelegate^  OnLifetimeRemaining;

      /// <summary>
      /// This event is fired on reception of an error.
      /// </summary>
      event RTCSrtpSdesTransportErrorDelegate^              OnError;

    private:
      ISRTPSDESTransportPtr _nativePointer;
      internal::RTCSrtpSdesTransportDelegatePtr _nativeDelegatePointer;
      ISRTPSDESTransportSubscriptionPtr _nativeSubscriptionPointer;
    };
  }
}
