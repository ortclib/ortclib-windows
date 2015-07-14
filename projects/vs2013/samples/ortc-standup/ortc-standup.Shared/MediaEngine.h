#pragma once

#include "webrtc/base/thread.h"
#include "webrtc/system_wrappers/interface/trace.h"

#include "webrtc/voice_engine/include/voe_base.h"
#include "webrtc/voice_engine/include/voe_codec.h"
#include "webrtc/voice_engine/include/voe_network.h"
#include "webrtc/voice_engine/include/voe_rtp_rtcp.h"
#include "webrtc/voice_engine/include/voe_audio_processing.h"
#include "webrtc/voice_engine/include/voe_volume_control.h"
#include "webrtc/voice_engine/include/voe_hardware.h"
#include "webrtc/voice_engine/include/voe_file.h"

#include "webrtc/video_engine/include/vie_base.h"
#include "webrtc/video_engine/include/vie_network.h"
#include "webrtc/video_engine/include/vie_render.h"
#include "webrtc/video_engine/include/vie_capture.h"
#include "webrtc/video_engine/include/vie_codec.h"
#include "webrtc/video_engine/include/vie_rtp_rtcp.h"

#include "webrtc/test/channel_transport/include/channel_transport.h"
#include "webrtc/test/field_trial.h"

namespace ortc_standup
{
  class MediaElementWrapper;

  class MediaEngine
  {
  public:
    MediaEngine();
    ~MediaEngine();
    void SetLocalMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetRemoteMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetStartStopButton(Windows::UI::Xaml::Controls::Button^ button);
    void StartStopMedia(Platform::String^ remoteIPAddress);

  private:
    std::string GetRawVideoFormatString(webrtc::RawVideoType videoType);

    webrtc::VideoRender* vrm_;
    webrtc::TraceCallback* traceCallback_;
    Windows::UI::Core::CoreDispatcher^ dispatcher_;
    bool started_;
    bool startedVideo_;

    int audioPort_;
    int videoPort_;
    std::string remoteIpAddress_;

    Windows::UI::Xaml::Controls::Button^ startStopButton_;
    MediaElementWrapper* localMediaWrapper_;
    MediaElementWrapper* remoteMediaWrapper_;

    int voiceChannel_;
    webrtc::test::VoiceChannelTransport* voiceTransport_;
    webrtc::VoiceEngine* voiceEngine_;
    webrtc::VoEBase* voiceBase_;
    webrtc::VoECodec* voiceCodec_;
    webrtc::VoENetwork* voiceNetwork_;
    webrtc::VoERTP_RTCP* voiceRtpRtcp_;
    webrtc::VoEAudioProcessing* voiceAudioProcessing_;
    webrtc::VoEVolumeControl* voiceVolumeControl_;
    webrtc::VoEHardware* voiceHardware_;
    webrtc::VoEFile* voiceFile_;

    int videoChannel_;
    webrtc::test::VideoChannelTransport* videoTransport_;
    int captureId_;
    char deviceUniqueId_[512];
    Concurrency::event stopEvent_;
    webrtc::VideoCaptureModule* vcpm_;
    webrtc::VideoEngine* videoEngine_;
    webrtc::ViEBase* videoBase_;
    webrtc::ViENetwork* videoNetwork_;
    webrtc::ViERender* videoRender_;
    webrtc::ViECapture* videoCapture_;
    webrtc::ViERTP_RTCP* videoRtpRtcp_;
    webrtc::ViECodec* videoCodec_;

    rtc::Thread workerThread_;
  };
}