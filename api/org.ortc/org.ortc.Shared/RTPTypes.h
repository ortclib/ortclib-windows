
#pragma once

#include <ortc\IRTPTypes.h>

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(::ortc::IRTPTypes, IRTPTypes)
      
    using Windows::Foundation::Collections::IVector;

    ref struct RTCRtpCapabilities;
    ref struct RTCRtpCodecCapability;
    ref struct RTCRtpOpusCodecCapabilityOptions;
    ref struct RTCRtpOpusCodecCapabilityParameters;
    ref struct RTCRtpVp8CodecCapabilityParameters;
    ref struct RTCRtpH264CodecCapabilityParameters;
    ref struct RTCRtpRtxCodecCapabilityParameters;
    ref struct RTCRtpFlexFecCodecCapabilityParameters;

    ref struct RTCRtpParameters;
    ref struct RTCRtcpParameters;
    ref struct RTCRtpCodecParameters;
    ref struct RTCRtpOpusCodecParameters;
    ref struct RTCRtpVp8CodecParameters;
    ref struct RTCRtpH264CodecParameters;
    ref struct RTCRtpRtxCodecParameters;
    ref struct RTCRtpRedCodecParameters;
    ref struct RTCRtpFlexFecCodecParameters;
    ref struct RTCRtpEncodingParameters;
    ref struct RTCRtpRtxParameters;
    ref struct RTCRtpFecParameters;
    ref struct RTCRtpHeaderExtensionParameters;

    ref struct RTCRtcpFeedback;
    ref struct RTCRtpHeaderExtension;

    namespace internal
    {
      RTCRtpCapabilities^ ToCx(const IRTPTypes::Capabilities &input);
      RTCRtpCapabilities^ ToCx(IRTPTypes::CapabilitiesPtr input);
      IRTPTypes::CapabilitiesPtr FromCx(RTCRtpCapabilities^ input);

      RTCRtpCodecCapability^ ToCx(const IRTPTypes::CodecCapability &input);
      RTCRtpCodecCapability^ ToCx(IRTPTypes::CodecCapabilityPtr input);
      IRTPTypes::CodecCapabilityPtr FromCx(RTCRtpCodecCapability^ input);

      RTCRtpOpusCodecCapabilityOptions^ ToCx(const IRTPTypes::OpusCodecCapabilityOptions &input);
      RTCRtpOpusCodecCapabilityOptions^ ToCx(IRTPTypes::OpusCodecCapabilityOptionsPtr input);
      IRTPTypes::OpusCodecCapabilityOptionsPtr FromCx(RTCRtpOpusCodecCapabilityOptions^ input);

      RTCRtpOpusCodecCapabilityParameters^ ToCx(const IRTPTypes::OpusCodecCapabilityParameters &input);
      RTCRtpOpusCodecCapabilityParameters^ ToCx(IRTPTypes::OpusCodecCapabilityParametersPtr input);
      IRTPTypes::OpusCodecCapabilityParametersPtr FromCx(RTCRtpOpusCodecCapabilityParameters^ input);

      RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::VP8CodecCapabilityParameters &input);
      RTCRtpVp8CodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::VP8CodecCapabilityParametersPtr input);
      IRTPTypes::VP8CodecCapabilityParametersPtr FromCx(RTCRtpVp8CodecCapabilityParameters^ input);

      RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::H264CodecCapabilityParameters &input);
      RTCRtpH264CodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::H264CodecCapabilityParametersPtr input);
      IRTPTypes::H264CodecCapabilityParametersPtr FromCx(RTCRtpH264CodecCapabilityParameters^ input);

      RTCRtpRtxCodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::RTXCodecCapabilityParameters &input);
      RTCRtpRtxCodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::RTXCodecCapabilityParametersPtr input);
      IRTPTypes::RTXCodecCapabilityParametersPtr FromCx(RTCRtpRtxCodecCapabilityParameters^ input);

      RTCRtpFlexFecCodecCapabilityParameters^ ToCxCapabilityParameters(const IRTPTypes::FlexFECCodecCapabilityParameters &input);
      RTCRtpFlexFecCodecCapabilityParameters^ ToCxCapabilityParameters(IRTPTypes::FlexFECCodecCapabilityParametersPtr input);
      IRTPTypes::FlexFECCodecCapabilityParametersPtr FromCx(RTCRtpFlexFecCodecCapabilityParameters^ input);


      RTCRtpParameters^ ToCx(const IRTPTypes::Parameters &input);
      RTCRtpParameters^ ToCx(IRTPTypes::ParametersPtr input);
      IRTPTypes::ParametersPtr FromCx(RTCRtpParameters^ input);

      RTCRtcpParameters^ ToCx(const IRTPTypes::RTCPParameters &input);
      RTCRtcpParameters^ ToCx(IRTPTypes::RTCPParametersPtr input);
      IRTPTypes::RTCPParametersPtr FromCx(RTCRtcpParameters^ input);

      RTCRtpCodecParameters^ ToCx(const IRTPTypes::CodecParameters &input);
      RTCRtpCodecParameters^ ToCx(IRTPTypes::CodecParametersPtr input);
      IRTPTypes::CodecParametersPtr FromCx(RTCRtpCodecParameters^ input);

      RTCRtpOpusCodecParameters^ ToCx(const IRTPTypes::OpusCodecParameters &input);
      RTCRtpOpusCodecParameters^ ToCx(IRTPTypes::OpusCodecParametersPtr input);
      IRTPTypes::OpusCodecParametersPtr FromCx(RTCRtpOpusCodecParameters^ input);

      RTCRtpVp8CodecParameters^ ToCxParameters(const IRTPTypes::VP8CodecParameters &input);
      RTCRtpVp8CodecParameters^ ToCxParameters(IRTPTypes::VP8CodecParametersPtr input);
      IRTPTypes::VP8CodecCapabilityParametersPtr FromCx(RTCRtpVp8CodecParameters^ input);

      RTCRtpH264CodecParameters^ ToCxParameters(const IRTPTypes::H264CodecParameters &input);
      RTCRtpH264CodecParameters^ ToCxParameters(IRTPTypes::H264CodecParametersPtr input);
      IRTPTypes::H264CodecParametersPtr FromCx(RTCRtpH264CodecParameters^ input);

      RTCRtpRtxCodecParameters^ ToCx(const IRTPTypes::RTXCodecParameters &input);
      RTCRtpRtxCodecParameters^ ToCx(IRTPTypes::RTXCodecParametersPtr input);
      IRTPTypes::RTXCodecParametersPtr FromCx(RTCRtpRtxCodecParameters^ input);

      RTCRtpRedCodecParameters^ ToCx(const IRTPTypes::REDCodecParameters &input);
      RTCRtpRedCodecParameters^ ToCx(IRTPTypes::REDCodecParametersPtr input);
      IRTPTypes::REDCodecParametersPtr FromCx(RTCRtpRedCodecParameters^ input);

      RTCRtpFlexFecCodecParameters^ ToCx(const IRTPTypes::FlexFECCodecParameters &input);
      RTCRtpFlexFecCodecParameters^ ToCx(IRTPTypes::FlexFECCodecParametersPtr input);
      IRTPTypes::FlexFECCodecParametersPtr FromCx(RTCRtpFlexFecCodecParameters^ input);

      RTCRtpEncodingParameters^ ToCx(const IRTPTypes::EncodingParameters &input);
      RTCRtpEncodingParameters^ ToCx(IRTPTypes::EncodingParametersPtr input);
      IRTPTypes::EncodingParametersPtr FromCx(RTCRtpEncodingParameters^ input);

      RTCRtpRtxParameters^ ToCx(const IRTPTypes::RTXParameters &input);
      RTCRtpRtxParameters^ ToCx(IRTPTypes::RTXParametersPtr input);
      IRTPTypes::RTXParametersPtr FromCx(RTCRtpRtxParameters^ input);

      RTCRtpFecParameters^ ToCx(const IRTPTypes::FECParameters &input);
      RTCRtpFecParameters^ ToCx(IRTPTypes::FECParametersPtr input);
      IRTPTypes::FECParametersPtr FromCx(RTCRtpFecParameters^ input);

      RTCRtpHeaderExtensionParameters^ ToCx(const IRTPTypes::HeaderExtensionParameters &input);
      RTCRtpHeaderExtensionParameters^ ToCx(IRTPTypes::HeaderExtensionParametersPtr input);
      IRTPTypes::HeaderExtensionParametersPtr FromCx(RTCRtpHeaderExtensionParameters^ input);


      RTCRtcpFeedback^ ToCx(const IRTPTypes::RTCPFeedback &input);
      RTCRtcpFeedback^ ToCx(IRTPTypes::RTCPFeedbackPtr input);
      IRTPTypes::RTCPFeedback FromCx(RTCRtcpFeedback^ input);

      RTCRtpHeaderExtension^ ToCx(const IRTPTypes::HeaderExtension &input);
      RTCRtpHeaderExtension^ ToCx(IRTPTypes::HeaderExtensionPtr input);
      IRTPTypes::HeaderExtensionPtr FromCx(RTCRtpHeaderExtension^ input);

    } // namespace internal

    /// <summary>
    /// RTCDegradationPreference can be used to indicate the desired choice
    /// between degrading resolution and degrading framerate when bandwidth is
    /// constrained.
    /// </summary>
    public enum class RTCDegradationPreference
    {
      /// <summary>
      /// Degrade resolution in order to maintain framerate.
      /// </summary>
      MaintainFramerate,
      /// <summary>
      /// Degrade framerate in order to maintain resolution.
      /// </summary>
      MaintainResolution,
      /// <summary>
      /// Degrade a balance of framerate and resolution.
      /// </summary>
      Balanced
    };

    /// <summary>
    /// RTCRtcpFeedback provides information on RTCP feedback messages.
    /// </summary>
    public ref struct RTCRtcpFeedback sealed
    {
      /// <summary>
      /// Gets or sets valid values for type are the "RTCP Feedback" Attribute
      /// Values enumerated in [iana.org/assignments/sdp-parameters/sdp-parameters.xhtml#sdp-parameters-14]
      /// ("ack", "ccm", "nack", etc.), as well as "goog-remb"
      /// [draft-alvestrand-rmcat-remb] and "transport-cc"
      /// [draft-holmer-rmcat-transport-wide-cc-extensions].
      /// </summary>
      property Platform::String^ Type;
      /// <summary>
      /// Gets or sets the parameters associated with the Type. For a type of
      /// "ack" or "nack", valid values for parameter are the "ack" and "nack"
      /// Attribute Values enumerated in
      /// [iana.org/assignments/sdp-parameters/sdp-parameters.xhtml#sdp-parameters-15]
      /// ("sli", "rpsi", etc.). For the Generic NACK feedback message defined
      /// in [RFC4585] Section 6.2.1, the type attribute is set to "nack" and
      /// the parameter attribute is unset. For a type of "ccm", valid values
      /// for parameter are the "Codec Control Messages" enumerated in
      /// [iana.org/assignments/sdp-parameters/sdp-parameters.xhtml#sdp-parameters-19]
      /// ("fir", "tmmbr" (includes "tmmbn"), etc.).
      /// </summary>
      property Platform::String^ Parameter;

      /// <summary>
      /// Helper routine to convert this object to a JSON blob.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routine to convert a JSON blob to an RTCRtcpFeedback object.
      /// </summary>
      static RTCRtcpFeedback^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// RTCRtpCodecCapability provides information on the capabilities of a
    /// codec.
    /// </summary>
    public ref struct RTCRtpCodecCapability sealed
    {
      /// <summary>
      /// Constructs an instance of an RTCRtpCodecCapability object.
      /// </summary>
      RTCRtpCodecCapability()
      {
        Ptime = 0;
        Maxptime = 0;
        MaxTemporalLayers = 0;
        MaxSpatialLayers = 0;
        SvcMultiStreamSupport = false;
      }

      /// <summary>
      /// Gets or sets the MIME media type. Valid types are listed in
      /// [iana.org/assignments/rtp-parameters/rtp-parameters.xhtml#rtp-parameters-2].
      /// </summary>
      property Platform::String^          Name;
      /// <summary>
      /// Gets or sets the media supported by the codec: "audio", "video" or
      /// "" for both.
      /// </summary>
      property Platform::String^          Kind;
      /// <summary>
      /// Gets or sets the codec clock rate expressed in Hertz. If unset, the
      /// codec is applicable to any clock rate.
      /// </summary>
      property Platform::IBox<uint32>^    ClockRate;
      /// <summary>
      /// Gets or sets the preferred RTP payload type for the codec denoted by
      /// RTCRtpCodecCapability.name. This attribute was added to make it
      /// possible for the sender and receiver to pick a matching payload type
      /// when creating sender and receiver parameters. When returned by
      /// RTCRtpSender.GetCapabilities(),
      /// RTCRtpCapabilities.Codecs.PreferredPayloadtype represents the
      /// preferred RTP payload type for sending. When returned by
      /// RTCRtpReceiver.GetCapabilities(),
      /// RTCRtpCapabilities.Codecs.PreferredPayloadtype represents the
      /// preferred RTP payload type for receiving. To avoid payload type
      /// conflicts, each value of preferredPayloadType must be unique.
      /// </summary>
      property uint8                      PreferredPayloadType;
      /// <summary>
      /// Gets or sets the preferred duration of media represented by a packet
      /// in milliseconds for the RTCRtpSender or RTCRtpReceiver.
      /// </summary>
      property uint32                     Ptime;
      /// <summary>
      /// Gets or sets the maximum packetization time supported by the
      /// RTCRtpReceiver.
      /// </summary>
      property uint32                     Maxptime;
      /// <summary>
      /// Gets or sets the number of channels supported (e.g. two for stereo).
      /// For video, this attribute is unset.
      /// </summary>
      property Platform::IBox<uint32>^    NumChannels;
      /// <summary>
      /// Gets or sets the transport layer and codec-specific feedback
      /// messages for this codec.
      /// </summary>
      property IVector<RTCRtcpFeedback^>^ RtcpFeedback;
      /// <summary>
      /// Gets or sets the codec-specific parameters that must be signaled to
      /// the remote party.
      /// </summary>
      property Object^                    Parameters;
      /// <summary>
      /// Gets or sets the codec-specific parameters available for signaling.
      /// </summary>
      property Object^                    Options;
      /// <summary>
      /// Gets or sets the maximum number of temporal layer extensions
      /// supported by this codec (e.g. a value of 1 indicates support for up
      /// to 2 temporal layers). A value of 0 indicates no support for
      /// temporal scalability.
      /// </summary>
      property uint16                     MaxTemporalLayers;
      /// <summary>
      /// Gets or sets the maximum number of spatial layer extensions
      /// supported by this codec (e.g. a value of 1 indicates support for up
      /// to 2 spatial layers). A value of 0 indicates no support for spatial
      /// scalability.
      /// </summary>
      property uint16                     MaxSpatialLayers;
      /// <summary>
      /// Gets or sets whether the implementation can send SVC layers
      /// utilizing distinct SSRCs. Unset for audio codecs. For video codecs,
      /// only set if the codec supports scalable video coding with multiple
      /// streams.
      /// </summary>
      property Platform::Boolean          SvcMultiStreamSupport;

      /// <summary>
      /// Helper routine to convert to a JSON string blob.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routine to convert a JSON string blob to a codec capabilitity.
      /// </summary>
      static RTCRtpCodecCapability^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// Configures the type of signal being encoded.
    /// </summary>
    enum class OpusCodecSignal
    {
      /// <summary>
      /// (default)
      /// </summary>
      Auto,
      /// <summary>
      /// Bias thresholds towards choosing MDCT modes.
      /// </summary>
      Music,
      /// <summary>
      /// Bias thresholds towards choosing LPC or Hybrid modes.
      /// </summary>
      Voice
    };

    /// <summary>
    /// Opus coding mode.
    /// </summary>
    enum class OpusCodecApplication
    {
      /// <summary>
      /// Gives best quality at a given bitrate for voice signals.
      /// </summary>
      VoIP,
      /// <summary>
      /// Gives best quality at a given bitrate for most non-voice signals
      /// like music. 
      /// </summary>
      Audio,
      /// <summary>
      /// Configures low-delay mode that disables the speech-optimized mode in
      /// exchange for slightly reduced delay. 
      /// </summary>
      LowDelay
    };

    /// <summary>
    /// The following capability options are defined for Opus:
    /// </summary>
    public ref struct RTCRtpOpusCodecCapabilityOptions sealed
    {
      /// <summary>
      /// Gets or sets the default value for the encoder's computational
      /// complexity. The supported range is 0-10 with 10 representing the
      /// highest complexity.
      /// </summary>
      property uint32 Complexity;
      /// <summary>
      /// Gets or sets the default value for the type of signal being encoded.
      /// Possible values are string version of enum OpusCodecSignal.
      /// </summary>
      property Platform::String^ Signal;
      /// <summary>
      /// Gets or sets the default value for the encoder's intended
      /// application. Possible values are string version of enum
      /// OpusCodecApplication.
      /// </summary>
      property Platform::String^ Application;
      /// <summary>
      /// Gets or sets the default value for the encoder's expected packet
      /// loss percentage. Possible values are 0-100.
      /// </summary>
      property uint32 PacketLossPerc;
      /// <summary>
      /// Gets or sets the default value for whether prediction is disabled,
      /// making frames almost complete independent (if true) or not (if false).
      /// </summary>
      property Platform::Boolean PredictionDisabled;
    };

    /// <summary>
    /// The following optional capability parameters are defined for "opus",
    /// as noted in [RFC7587] Section 6.1:
    /// </summary>
    public ref struct RTCRtpOpusCodecCapabilityParameters sealed
    {
      /// <summary>
      /// Gets or sets a hint about the maximum output sampling rate that the
      /// receiver is capable of rendering in Hz.
      /// </summary>
      property Platform::IBox<uint32>^            MaxPlaybackRate;
      /// <summary>
      /// Gets or sets the maximum average receive bitrate of a session in
      /// bits per second (bits/s).
      /// </summary>
      property Platform::IBox<uint32>^            MaxAverageBitrate;
      /// <summary>
      /// Gets or sets of whether the decoder prefers receiving stereo or
      /// mono signals.
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Stereo;
      /// <summary>
      /// Gets or sets if the decoder prefers the use of constant bitrate (if
      /// true) or variable bitrate (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Cbr;
      /// <summary>
      /// Gets or sets if the decoder has the capability to take advantage of
      /// Opus in-band fec (if true) or not (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ UseInbandFec;
      /// <summary>
      /// Gets or sets if the decoder prefers the use of DTX (if true) or not
      /// (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ UseDtx;

      /// <summary>
      /// A hint about the maximum input sampling rate that the sender is
      /// likely to produce.
      /// </summary>
      property Platform::IBox<uint32>^            SpropMaxCaptureRate;
      /// <summary>
      /// Gets or sets whether the sender is likely to produce stereo audio.
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ SpropStereo;
    };

    /// <summary>
    /// The following receiver capability parameters are defined for "vp8", as
    /// noted in [RFC7741] Section 6.1:
    /// </summary>
    public ref struct RTCRtpVp8CodecCapabilityParameters sealed
    {
      /// <summary>
      /// Gets or sets the maximum frame rate in frames per second that the
      /// decoder is capable of decoding.
      /// </summary>
      property Platform::IBox<uint32>^  MaxFr;
      /// <summary>
      /// Gets or sets the maximum frame size in macroblocks that the decoder
      /// is capable of decoding.
      /// </summary>
      property Platform::IBox<uint64>^  MaxFs;
    };

    /// <summary>
    /// The following capability parameters are defined for "h264", as noted
    /// in [RFC6184] Section 8.1, and [draft-ietf-rtcweb-video] Section 6.2.
    /// </summary>
    public ref struct RTCRtpH264CodecCapabilityParameters sealed
    {
      /// <summary>
      /// Gets or sets the maximum capability of the decoder (for an
      /// RTCRtpReceiver) or the encoder (for an RTCRtpSender). It must be
      /// supported, as noted in [draft-ietf-rtcweb-video] Section 6.2.
      /// </summary>
      property Platform::IBox<uint32>^ ProfileLevelId;
      /// <summary>
      /// A sequence of unsigned shorts, each ranging from 0 to 2, indicating
      /// supported packetizationMode values. As noted in
      /// [draft-ietf-rtcweb-video] Section 6.2, support for packetization
      /// mode 1 is mandatory.
      /// </summary>
      property IVector<uint16>^        PacketizationModes;

      /// <summary>
      /// Gets or sets an integer indicating the maximum macroblock processing
      /// rate in units of macroblocks per second.
      /// </summary>
      property Platform::IBox<uint64>^ MaxMbps;
      /// <summary>
      /// Gets or sets an integer indicating the maximum static macroblock
      /// processing rate in units of static macroblocks per second, under the
      /// hypothetical assumption that all macroblocks are static macroblocks.
      /// </summary>
      property Platform::IBox<uint64>^ MaxSmbps;
      /// <summary>
      /// Gets or sets an integer indicating the maximum frame size in units
      /// of macroblocks.
      /// </summary>
      property Platform::IBox<uint64>^ MaxFs;
      /// <summary>
      /// Gets or sets an integer indicating the maximum coded picture buffer
      /// size in units of 1000 bits for the VCL HRD parameters and in units
      /// of 1200 bits for the NAL HRD parameters.
      /// </summary>
      property Platform::IBox<uint64>^ MaxCpb;
      /// <summary>
      /// Gets or sets an integer indicating the maximum decoded picture
      /// buffer size in units of 8 / 3 macroblocks.
      /// </summary>
      property Platform::IBox<uint64>^ MaxDpb;
      /// <summary>
      /// Gets or sets an integer indicating the maximum video bitrate in
      /// units of 1000 bits per second for the VCL HRD parameters and in
      /// units of 1200 bits per second for the NAL HRD parameters.
      /// </summary>
      property Platform::IBox<uint64>^ MaxBr;
    };

    /// <summary>
    /// The following capability is defined for "rtx", as noted in [RFC4588]
    /// Section 8.6:
    /// </summary>
    public ref struct RTCRtpRtxCodecCapabilityParameters sealed
    {
      /// <summary>
      /// Gets or sets the default time in milliseconds (measured from the
      /// time a packet was first sent) that the sender keeps an RTP packet
      /// in its buffers available for retransmission, as defined in
      /// [RFC4588].
      /// </summary>
      property uint32                   RtxTime;
      /// <summary>
      /// Gets or sets the associated payload type of the original stream
      /// being retransmitted, as defined in [RFC4588].
      /// </summary>
      property uint8                    Apt;
    };

    /// <summary>
    /// The following capabilities are defined for "flexfec", as noted in
    /// [draft-ietf-rtcweb-fec] Section 5.1.1:
    /// </summary>
    public ref struct RTCRtpFlexFecCodecCapabilityParameters sealed
    {
      /// <summary>
      /// Gets or sets the default time that spans the source packets and the
      /// corresponding repair packets, in microseconds.
      /// </summary>
      property uint64                   RepairWindow;

      /// <summary>
      /// Gets or sets the default number of columns of the source block that
      /// are protected by this FEC block.
      /// </summary>
      property Platform::IBox<uint32>^  L;
      /// <summary>
      /// Gets or sets the default number of rows of the source block that are
      /// protected by this FEC block.
      /// </summary>
      property Platform::IBox<uint32>^  D;
      /// <summary>
      /// Gets or sets the default type of protection for the sender: 0 for
      /// 1-D interleaved FEC protection, 1 for 1-D non-interleaved FEC
      /// protection, and 2 for 2-D parity FEC protection. The value of 3 is
      /// reserved for future use.
      /// </summary>
      property Platform::IBox<uint16>^  ToP;
    };

    /// <summary>
    /// The RTCRtpHeaderExtension dictionary enables a header extension to be
    /// configured for use within an RTCRtpSender or RTCRtpReceiver.
    /// </summary>
    public ref struct RTCRtpHeaderExtension sealed
    {
      /// <summary>
      /// Constructs an instance of an RTCRtpHeaderExtension object.
      /// </summary>
      RTCRtpHeaderExtension()
      {
        PreferredEncrypt = false;
      }

      /// <summary>
      /// Gets or sets the media supported by the header extension: "audio"
      /// for an audio codec, "video" for a video codec, and "" for both.
      /// </summary>
      property Platform::String^    Kind;
      /// <summary>
      /// Gets or sets the URI of the RTP header extension, as defined in
      /// [RFC5285].
      /// </summary>
      property Platform::String^    Uri;
      /// <summary>
      /// Gets or sets the preferred ID value that goes in the packet.
      /// </summary>
      property uint16               PreferredId;
      /// <summary>
      /// Gets or sets if it is preferred that the value in the header be
      /// encrypted as per [RFC6904] if true. Default is to prefer
      /// unencrypted.
      /// </summary>
      property Platform::Boolean    PreferredEncrypt;

      /// <summary>
      /// Helper routing to conver this object into a JSON string blob.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routing to convert a JOSN string blob into an
      /// RTCRtpHeaderExtension object.
      /// </summary>
      static RTCRtpHeaderExtension^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// The RTCRtpCapabilities object expresses the capabilities of
    /// RTCRtpSender and RTCRtpReceiver objects. 
    /// </summary>
    public ref struct RTCRtpCapabilities sealed
    {
      /// <summary>
      /// Gets or sets the supported codecs.
      /// </summary>
      property IVector<RTCRtpCodecCapability^>^   Codecs;
      /// <summary>
      /// </summary>
      property IVector<RTCRtpHeaderExtension^>^   HeaderExtensions;
      /// <summary>
      /// Gets or sets the supported Forward Error Correction (FEC)
      /// mechanisms. Values include "red", "red+ulpfec" and "flexfec".
      /// [draft-ietf-rtcweb-fec] summarizes requirements relating to FEC
      /// mechanisms.
      /// </summary>
      property IVector<Platform::String^>^        FecMechanisms;

      /// <summary>
      /// Returns a copy of the RTCRtpCapabilities object.
      /// </summary>
      RTCRtpCapabilities^                         Clone();

      /// <summary>
      /// Helper routing to conver this object into a JSON string blob.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routing to convert a JOSN string blob into an
      /// RTCRtpCapabilities object.
      /// </summary>
      static RTCRtpCapabilities^ FromJsonString(Platform::String^ jsonString);
    };

    /// <summary>
    /// RTCRtcpParameters provides information on RTCP settings.
    /// </summary>
    public ref struct RTCRtcpParameters sealed
    {
      /// <summary>
      /// Creates an instance of an RTCRtcpParameters object.
      /// </summary>
      RTCRtcpParameters()
      {
        ReducedSize = false;
        Mux = true;
      }

      /// <summary>
      /// Gets or sets the SSRC to be used in the "SSRC of packet sender"
      /// field defined in [RFC3550] Section 6.4.2 (Receiver Report) and
      /// [RFC4585] Section 6.1 (Feedback Messages), as well as the "SSRC"
      /// field defined in [RFC3611] Section 2 (Extended Reports). It can only
      /// be set for an RTCRtpReceiver.
      /// </summary>
      property uint32               Ssrc;
      /// <summary>
      /// Gets or sets the Canonical Name (CNAME) used by RTCP (e.g. in SDES
      /// messages). Guidelines for CNAME generation are provided in
      /// [draft-ietf-rtcweb-rtp-usage] Section 4.9 and [RFC7022].
      /// </summary>
      property Platform::String^    Cname;
      /// <summary>
      /// Gets or sets whether reduced size RTCP [RFC5506] is configured
      /// (if true) or compound RTCP as specified in [RFC3550] (if false).
      /// </summary>
      property Platform::Boolean    ReducedSize;
      /// <summary>
      /// Gets or sets whether RTP and RTCP are multiplexed, as specified in
      /// [RFC5761]. The default is true. If set to false, the RTCIceTransport
      /// must have an associated RTCIceTransport object with a component of
      /// "RTCP", in which case RTCP will be sent on the associated
      /// RTCIceTransport.
      /// </summary>
      property Platform::Boolean    Mux;
    };

    /// <summary>
    /// RTCRtpCodecParameters provides information on codec settings.
    /// </summary>
    public ref struct RTCRtpCodecParameters sealed
    {
      /// <summary>
      /// Gets or sets the MIME media type. Valid types are listed in
      /// [www.iana.org/assignments/rtp-parameters/rtp-parameters.xhtml#rtp-parameters-2].
      /// The name must always be provided.
      /// </summary>
      property Platform::String^          Name;
      /// <summary>
      /// Gets or sets the value that goes in the RTP Payload Type Field
      /// [RFC3550]. The payloadType must always be provided, and must be
      /// unique.
      /// </summary>
      property uint8                      PayloadType;
      /// <summary>
      /// Gets or sets the codec clock rate expressed in Hertz, null if unset.
      /// </summary>
      property Platform::IBox<uint32>^    ClockRate;
      /// <summary>
      /// Gets or sets the duration of media represented by a packet in
      /// milliseconds for the RTCRtpSender. If unset, the RTCRtpSender may
      /// select any value up to maxptime.
      /// </summary>
      property uint32                     Ptime;
      /// <summary>
      /// Gets or sets the maximum packetization time set on the RTCRtpSender.
      /// Not specified if unset. If Ptime is also set, Maxptime is ignored.
      /// </summary>
      property uint32                     Maxptime;
      /// <summary>
      /// Gets or sets the number of channels supported (e.g. two for stereo).
      /// If unset for audio, use the codec default. For video, this can be
      /// left unset.
      /// </summary>
      property Platform::IBox<uint32>^    NumChannels;
      /// <summary>
      /// </summary>
      property IVector<RTCRtcpFeedback^>^ RtcpFeedback;
      /// <summary>
      /// Gets or sets the codec-specific parameters available for signaling.
      /// </summary>
      property Object^                    Parameters;
    };

    /// <summary>
    /// The following settings are defined for "opus".
    /// </summary>
    public ref struct RTCRtpOpusCodecParameters sealed
    {
      /// <summary>
      /// Gets or sets the maximum output sampling rate of the encoder in Hz.
      /// </summary>
      property Platform::IBox<uint32>^            MaxPlaybackRate;
      /// <summary>
      /// Gets or sets the maximum average receive bitrate of a session in bits
      /// per second (bits/s).
      /// </summary>
      property Platform::IBox<uint32>^            MaxAverageBitrate;
      /// <summary>
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Stereo;
      /// <summary>
      /// Gets or sets if the encoder is configured to generate constant
      /// bitrate (if true) or variable bitrate (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ Cbr;
      /// <summary>
      /// Gets or sets if the encoder is configured to generate Opus in-band
      /// fec (if true) or not (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ UseInbandFec;
      /// <summary>
      /// Gets or sets if the encoder is configured to use DTX (if true) or
      /// not (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ UseDtx;

      /// <summary>
      /// Gets or sets the encoder's computational complexity. The supported
      // range is 0-10 with 10 representing the highest complexity.
      /// </summary>
      property Platform::IBox<uint32>^            Complexity;
      /// <summary>
      /// Gets or sets the type of signal being encoded. Possible values are
      /// the string form of the enum OpusCodecSignal.
      /// </summary>
      property Platform::String^                  Signal;
      /// <summary>
      /// Gets or sets the encoder's intended application. Possible values are
      /// the string form of the enum OpusCodecApplication.
      /// </summary>
      property Platform::String^                  Application;
      /// <summary>
      /// Gets or sets the encoder's expected packet loss percentage. Possible
      /// values are 0-100.
      /// </summary>
      property Platform::IBox<uint32>^            PacketLossPerc;
      /// <summary>
      /// Gets or sests whether prediction is disabled, making frames almost
      /// complete independent (if true) or not (if false).
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ PredictionDisabled;

      /// <summary>
      /// Gets or sets the maximum input sampling rate produced by the sender.
      /// </summary>
      property Platform::IBox<uint32>^            SpropMaxCaptureRate;
      /// <summary>
      /// Gets or sets whether the sender is likely to produce stereo audio.
      /// </summary>
      property Platform::IBox<Platform::Boolean>^ SpropStereo;
    };

    /// <summary>
    /// The following settings are defined for "vp8".
    /// </summary>
    public ref struct RTCRtpVp8CodecParameters sealed
    {
      /// <summary>
      /// Gets or sets the maximum frame rate in frames per second that the
      /// decoder is capable of decoding.
      /// </summary>
      property Platform::IBox<uint32>^  MaxFr;
      /// <summary>
      /// Gets or sets the maximum frame size in macroblocks that the decoder
      /// is capable of decoding.
      /// </summary>
      property Platform::IBox<uint64>^  MaxFs;
    };

    /// <summary>
    /// The following settings are defined for "h264".
    /// </summary>
    public ref struct RTCRtpH264CodecParameters sealed
    {
      /// <summary>
      /// Gets or sets the configuration of the stream to be sent, as noted in
      /// [RFC6184] Section 8.2.2. It must be supported, as noted in
      /// [draft-ietf-rtcweb-video] Section 6.2.
      /// </summary>
      property Platform::IBox<uint32>^  ProfileLevelId;
      /// <summary>
      /// Gets or sets an unsigned short ranging from 0 to 2, indicating the
      /// packetizationMode value to be used by the sender. This setting must
      /// be supported, as noted in [draft-ietf-rtcweb-video] Section 6.2.
      /// </summary>
      property IVector<uint16>^         PacketizationModes;

      /// <summary>
      /// Gets or sets an integer indicating the maximum macroblock processing
      /// rate in units of macroblocks per second.
      /// </summary>
      property Platform::IBox<uint64>^  MaxMbps;
      /// <summary>
      /// Gets or sets an integer indicating the maximum static macroblock
      /// processing rate in units of static macroblocks per second, under the
      /// hypothetical assumption that all macroblocks are static macroblocks.
      /// </summary>
      property Platform::IBox<uint64>^  MaxSmbps;
      /// <summary>
      /// Gets or sets an integer indicating the maximum frame size in units
      /// of macroblocks.
      /// </summary>
      property Platform::IBox<uint64>^  MaxFs;
      /// <summary>
      /// Gets or sets an integer indicating the maximum coded picture buffer
      /// size in units of 1000 bits for the VCL HRD parameters and in units
      /// of 1200 bits for the NAL HRD parameters.
      /// </summary>
      property Platform::IBox<uint64>^  MaxCpb;
      /// <summary>
      /// Gets or sets an integer indicating the maximum decoded picture
      /// buffer size in units of 8 / 3 macroblocks.
      /// </summary>
      property Platform::IBox<uint64>^  MaxDpb;
      /// <summary>
      /// Gets or sets an integer indicating the maximum video bitrate in
      /// units of 1000 bits per second for the VCL HRD parameters and in
      /// units of 1200 bits per second for the NAL HRD parameters.
      /// </summary>
      property Platform::IBox<uint64>^  MaxBr;
    };

    /// <summary>
    /// The following settings are defined for "rtx", as noted in [RFC4588]
    /// Section 8.6.
    /// </summary>
    public ref struct RTCRtpRtxCodecParameters sealed
    {
      /// <summary>
      /// Gets or sets the time in milliseconds (measured from the time a
      /// packet was first sent) that the sender keeps an RTP packet in its
      /// buffers available for retransmission, as defined in [RFC4588]
      /// </summary>
      property uint32                   RtxTime;
      /// <summary>
      /// Gets or sets the associated payload type of the original stream
      /// being retransmitted, as defined in [RFC4588].
      /// </summary>
      property uint8                    Apt;
    };

    /// <summary>
    /// The following setting is defined for "red", as noted in [RFC2198]
    /// Section 5.
    /// </summary>
    public ref struct RTCRtpRedCodecParameters sealed
    {
      /// <summary>
      /// Gets or sets sequence of payload types to be encapsulated in RED,
      /// each of which must be unique. If payloadTypes is unset, this means
      /// that any codec other than "red" or "rtx" can be encapsulsated in
      /// RED.
      /// </summary>
      property IVector<uint8>^          PayloadTypes;
    };

    /// <summary>
    /// The following settings are defined for "flexfec", as noted in
    /// [draft-ietf-payload-flexible-fec-scheme] Section 5.1.1:
    /// </summary>
    public ref struct RTCRtpFlexFecCodecParameters sealed
    {
      /// <summary>
      /// Gets or sets the time that spans the source packets and the
      /// corresponding repair packets, in microseconds.
      /// </summary>
      property uint64                   RepairWindow;

      /// <summary>
      /// Gets or sets the number of columns of the source block that are
      /// protected by this FEC block.
      /// </summary>
      property Platform::IBox<uint32>^  L;
      /// <summary>
      /// Gets or sets the number of rows of the source block that are
      /// protected by this FEC block.
      /// </summary>
      property Platform::IBox<uint32>^  D;
      /// <summary>
      /// Gets or sets the type of protection applied by the sender: 0 for
      /// 1-D interleaved FEC protection, 1 for 1-D non-interleaved FEC
      /// protection, and 2 for 2-D parity FEC protection. The value of 3 is
      /// reserved for future use.
      /// </summary>
      property Platform::IBox<uint16>^  ToP;
    };

    /// <summary>
    /// The RTCRtpHeaderExtension dictionary configers the header extensions
    /// for use within an RTCRtpSender or RTCRtpReceiver.
    /// </summary>
    public ref struct  RTCRtpHeaderExtensionParameters sealed
    {
      /// <summary>
      /// Constructs an instance of an RTCRtpHeaderExtensionParameters object.
      /// </summary>
      RTCRtpHeaderExtensionParameters()
      {
        Encrypt = false;
      }

      /// <summary>
      /// Gets or sets the URI of the RTP header extension, as defined in
      /// [RFC5285].
      /// </summary>
      property Platform::String^        Uri;
      /// <summary>
      /// Gets or sets the value that goes in the packet.
      /// </summary>
      property uint16                   Id;
      /// <summary>
      /// Gets or sets the value in the header is encrypted as per [RFC6904],
      /// if true. Default is unencrypted.
      /// </summary>
      property Platform::Boolean        Encrypt;
    };

    /// <summary>
    /// The RTCRtpFecParameters represents the FEC sets for a particular
    /// encoding.
    /// </summary>
    public ref struct RTCRtpFecParameters sealed
    {
      /// <summary>
      /// Gets or sets the SSRC to use for FEC. If unset in an RTCRtpSender
      /// object, the ORTC engine will choose.
      /// </summary>
      property Platform::IBox<uint32>^  Ssrc;
      /// <summary>
      /// Gets or sets the Forward Error Correction (FEC) mechanism to use:
      /// "red", "red+ulpfec" or "flexfec".
      /// </summary>
      property Platform::String^        Mechanism;
    };

    /// <summary>
    /// The RTCRtpRtxParameters represents the RTX sets for a particular
    /// encoding.
    /// </summary>
    public ref struct RTCRtpRtxParameters sealed
    {
      /// <summary>
      /// Gets or sets the SSRC to use for retransmission, as specified in
      /// [RFC4588]. If unset when passed to RTCRtpSender.Send(), the
      /// ORTC engine will choose.
      /// </summary>
      property Platform::IBox<uint32>^  Ssrc;
    };

    /// <summary>
    /// RTCPriorityType can be used to indicate the relative priority of
    /// various flows.
    /// </summary>
    public enum class  RTCPriorityType
    {
      /// <summary>
      /// See [draft-ietf-rtcweb-transports], Section 4.
      /// </summary>
      Very_Low,
      /// <summary>
      /// See [draft-ietf-rtcweb-transports], Section 4.
      /// </summary>
      Low,
      /// <summary>
      /// See [draft-ietf-rtcweb-transports], Section 4.
      /// </summary>
      Medium,
      /// <summary>
      /// See [draft-ietf-rtcweb-transports], Section 4.
      /// </summary>
      High
    };

    /// <summary>
    /// RTCRtpEncodingParameters provides information relating to the
    /// encoding.
    /// </summary>
    public ref struct RTCRtpEncodingParameters sealed
    {
      /// <summary>
      /// Constructs an instance of the RTCRtpEncodingParameters.
      /// </summary>
      RTCRtpEncodingParameters()
      {
        Active = true;
        Priority = RTCPriorityType::Medium;
      }

      /// <summary>
      /// Gets or sets the SSRC for this layering/encoding.
      /// </summary>
      property Platform::IBox<uint32>^      Ssrc;
      /// <summary>
      /// Gets or sets the codec payload type per-encoding. If unset, the
      /// ORTC engine will choose the first codec in parameters.Codecs[] of
      /// the appropriate kind.
      /// </summary>
      property Platform::IBox<uint8>^       CodecPayloadType;
      /// <summary>
      /// Gets or sets the FEC mechanism.
      /// </summary>
      property RTCRtpFecParameters^         Fec;
      /// <summary>
      /// Gets or sets the RTX [RFC4588] parameters.
      /// </summary>
      property RTCRtpRtxParameters^         Rtx;
      /// <summary>
      /// Gets or sets the priority of this encoding.
      /// </summary>
      property RTCPriorityType              Priority;
      /// <summary>
      /// Gets or sets the maximum bitrate ramp up for resolution/quality/
      /// framerate, if set.
      /// </summary>
      property Platform::IBox<uint64>^      MaxBitrate;
      /// <summary>
      /// Gets or sets the minimum quality to never send less than.
      /// </summary>
      property Platform::IBox<float64>^     MinQuality;
      /// <summary>
      /// Gets or sets the video's resolution will be scaled down in each
      /// dimension by the given value before sending, if the sender's kind is
      /// "video".
      /// </summary>
      property Platform::IBox<float64>^     ResolutionScale;
      /// <summary>
      /// Gets or sets the inverse of the input framerate fraction to be
      /// encoded.
      /// </summary>
      property Platform::IBox<float64>^     FramerateScale;
      /// <summary>
      /// Gets or sets an indicates whether this encoding is actively being
      /// sent for the RTCRtpSender. Setting it to false causes this
      /// encoding to no longer be sent.
      /// </summary>
      property Platform::Boolean            Active;
      /// <summary>
      /// Gets or sets an identifier for the encoding object.
      /// </summary>
      property Platform::String^            EncodingId;
      /// <summary>
      /// Gets or sets the EncodingIds on which this layer depends.
      /// </summary>
      property IVector<Platform::String^>^  DependencyEncodingIds;
    };

    /// <summary>
    /// RTCRtpParameters contains the RTP stack settings.
    /// </summary>
    public ref struct RTCRtpParameters sealed
    {
      /// <summary>
      /// Constructs an instance of RTCRtpParameters.
      /// </summary>
      RTCRtpParameters() { DegradationPreference = RTCDegradationPreference::Balanced; }

      /// <summary>
      /// Gets or sets the muxId assigned to the RTP stream, if any.
      /// </summary>
      property Platform::String^                          MuxId;
      /// <summary>
      /// Gets or sets the codecs to send or receive (could include RED
      /// [RFC2198], RTX [RFC4588] and CN [RFC3389]). codecs must be set for
      /// an RTCRtpParameters object to be a valid argument passed to Send()
      /// or Receive().
      /// </summary>
      property IVector<RTCRtpCodecParameters^>^           Codecs;
      /// <summary>
      /// Gets or sets the header extensions. If unset, no header extensions
      /// are configured.
      /// </summary>
      property IVector<RTCRtpHeaderExtensionParameters^>^ HeaderExtensions;
      /// <summary>
      /// Gets or sets the "encodings" or "layers" to be used for things like
      /// simulcast, Scalable Video Coding, RTX, FEC, etc. 
      /// </summary>
      property IVector<RTCRtpEncodingParameters^>^        Encodings;
      /// <summary>
      /// Gets or sets the parameters to configure RTCP. 
      /// </summary>
      property RTCRtcpParameters^                         Rtcp;
      /// <summary>
      /// Gets or sets the degradation preference when bandwidth is
      /// constrained and the RTCRtpSender needs to choose between degrading
      /// resolution or degrading framerate.
      /// </summary>
      property RTCDegradationPreference                   DegradationPreference;

      /// <summary>
      /// Returns a cloned copy of the RTCRtpParameters object.
      /// </summary>
      RTCRtpParameters^                                   Clone();

      /// <summary>
      /// Helper routing to conver this object into a JSON string blob.
      /// </summary>
      Platform::String^ ToJsonString();
      /// <summary>
      /// Helper routing to convert a JOSN string blob into an
      /// RTCRtpParameters object.
      /// </summary>
      static RTCRtpParameters^ FromJsonString(Platform::String^ jsonString);
    };
  }
}
