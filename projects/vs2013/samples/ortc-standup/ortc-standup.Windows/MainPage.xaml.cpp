//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

#include <ppltasks.h>

#include "webrtc/modules/video_capture/include/video_capture_factory.h"

using namespace ortc_standup;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

#if (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#define PREFERRED_FRAME_WIDTH 640
#define PREFERRED_FRAME_HEIGHT 480
#define PREFERRED_MAX_FPS 30
#define CAPTURE_DEVICE_INDEX 0
#define MAX_BITRATE 500
#else
#define PREFERRED_FRAME_WIDTH 800
#define PREFERRED_FRAME_HEIGHT 600
#define PREFERRED_MAX_FPS 30
#define CAPTURE_DEVICE_INDEX 0
#define MAX_BITRATE 1000
#endif

#define VOICE
#define VIDEO

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

class TestTraceCallback : public webrtc::TraceCallback
{
public:

  virtual void Print(webrtc::TraceLevel level, const char* message, int length)
  {
    WCHAR szTextBuf[1024];
    int cTextBufSize = MultiByteToWideChar(CP_UTF8, 0, message, length + 2, NULL, 0);
    MultiByteToWideChar(CP_UTF8, 0, message, length + 2, szTextBuf, cTextBufSize);
    szTextBuf[cTextBufSize - 3] = L'\r';
    szTextBuf[cTextBufSize - 2] = L'\n';
    szTextBuf[cTextBufSize - 1] = 0;
    OutputDebugString(szTextBuf);
  }
};

MainPage::MainPage() :
  traceCallback_(new TestTraceCallback()),
  started_(false),
  startedVideo_(false),
  voiceTransport_(NULL),
  videoTransport_(NULL),
  voiceChannel_(-1),
  captureId_(-1),
  videoChannel_(-1)
{
	InitializeComponent();

  int error;

  webrtc::test::InitFieldTrialsFromString("");
  webrtc::Trace::CreateTrace();
  webrtc::Trace::SetTraceCallback(traceCallback_);
  webrtc::Trace::set_level_filter(webrtc::kTraceAll);

#ifdef VOICE
  voiceEngine_ = webrtc::VoiceEngine::Create();
  if (voiceEngine_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to create voice engine.");
    return;
  }
  voiceBase_ = webrtc::VoEBase::GetInterface(voiceEngine_);
  if (voiceBase_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for voice base.");
    return;
  }
  voiceCodec_ = webrtc::VoECodec::GetInterface(voiceEngine_);
  if (voiceCodec_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for voice codec.");
    return;
  }
  voiceNetwork_ = webrtc::VoENetwork::GetInterface(voiceEngine_);
  if (voiceNetwork_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for voice network.");
    return;
  }
  voiceRtpRtcp_ = webrtc::VoERTP_RTCP::GetInterface(voiceEngine_);
  if (voiceRtpRtcp_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for voice RTP/RTCP.");
    return;
  }
  voiceAudioProcessing_ = webrtc::VoEAudioProcessing::GetInterface(voiceEngine_);
  if (voiceAudioProcessing_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for audio processing.");
    return;
  }
  voiceVolumeControl_ = webrtc::VoEVolumeControl::GetInterface(voiceEngine_);
  if (voiceVolumeControl_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for volume control.");
    return;
  }
  voiceHardware_ = webrtc::VoEHardware::GetInterface(voiceEngine_);
  if (voiceHardware_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for audio hardware.");
    return;
  }
  voiceFile_ = webrtc::VoEFile::GetInterface(voiceEngine_);
  if (voiceFile_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to get interface for voice file.");
    return;
  }

  error = voiceHardware_->SetAudioDeviceLayer(webrtc::kAudioWindowsWasapi);
  if (error < 0) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to set audio device layer type. Error: %d", voiceBase_->LastError());
    return;
  }

  error = voiceBase_->Init();
  if (error < 0) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to initialize voice base. Error: %d", voiceBase_->LastError());
    return;
  }
  else if (voiceBase_->LastError() > 0) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "An error has occured during voice base init. Error: %d", voiceBase_->LastError());
  }
#endif
#ifdef VIDEO
  videoEngine_ = webrtc::VideoEngine::Create();
  if (videoEngine_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to create video engine.");
    return;
  }

  videoBase_ = webrtc::ViEBase::GetInterface(videoEngine_);
  if (videoBase_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to get interface for video base.");
    return;
  }
  videoCapture_ = webrtc::ViECapture::GetInterface(videoEngine_);
  if (videoCapture_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed get interface for video capture.");
    return;
  }
  videoRtpRtcp_ = webrtc::ViERTP_RTCP::GetInterface(videoEngine_);
  if (videoRtpRtcp_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to get interface for video RTP/RTCP.");
    return;
  }
  videoNetwork_ = webrtc::ViENetwork::GetInterface(videoEngine_);
  if (videoNetwork_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to get interface for video network.");
    return;
  }
  videoRender_ = webrtc::ViERender::GetInterface(videoEngine_);
  if (videoRender_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to get interface for video render.");
    return;
  }
  videoCodec_ = webrtc::ViECodec::GetInterface(videoEngine_);
  if (videoCodec_ == NULL) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to get interface for video codec.");
    return;
  }

  error = videoBase_->Init();
  if (error < 0) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to initialize video base. Error: %d", videoBase_->LastError());
    return;
  }
  else if (videoBase_->LastError() > 0) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "An error has occured during video base init. Error: %d", videoBase_->LastError());
  }
#ifdef VOICE
  error = videoBase_->SetVoiceEngine(voiceEngine_);
  if (error < 0) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to set voice engine for video base. Error: %d", videoBase_->LastError());
    return;
  }
#endif
#endif
}

ortc_standup::MainPage::~MainPage()
{
  int error;

#ifdef VOICE
  if (voiceBase_) {
    error = voiceBase_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release voice base. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceCodec_) {
    error = voiceCodec_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release voice codec. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceNetwork_) {
    error = voiceNetwork_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release voice network. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceRtpRtcp_) {
    error = voiceRtpRtcp_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release voice RTP/RTCP. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceAudioProcessing_) {
    error = voiceAudioProcessing_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release audio processing. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceVolumeControl_) {
    error = voiceVolumeControl_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release volume control. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceHardware_) {
    error = voiceHardware_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release audio hardware. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (voiceFile_) {
    error = voiceFile_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
        "Failed to release voice file. Error: %d", voiceBase_->LastError());
      return;
    }
  }

  if (!webrtc::VoiceEngine::Delete(voiceEngine_)) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
      "Failed to delete voice engine.");
    return;
  }
#endif
#ifdef VIDEO
  if (videoBase_) {
    error = videoBase_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
        "Failed to release video base. Error: %d", videoBase_->LastError());
      return;
    }
  }

  if (videoNetwork_) {
    error = videoNetwork_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
        "Failed to release video network. Error: %d", videoBase_->LastError());
      return;
    }
  }

  if (videoRender_) {
    error = videoRender_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
        "Failed to release video render. Error: %d", videoBase_->LastError());
      return;
    }
  }

  if (videoCapture_) {
    error = videoCapture_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
        "Failed to release video capture. Error: %d", videoBase_->LastError());
      return;
    }
  }

  if (videoRtpRtcp_) {
    error = videoRtpRtcp_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
        "Failed to release video RTP/RTCP. Error: %d", videoBase_->LastError());
      return;
    }
  }

  if (videoCodec_) {
    error = videoCodec_->Release();
    if (error < 0) {
      webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
        "Failed to release video codec. Error: %d", videoBase_->LastError());
      return;
    }
  }

  if (!webrtc::VideoEngine::Delete(videoEngine_)) {
    webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
      "Failed to delete video engine.");
    return;
  }
#endif
}

void ortc_standup::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
  dispatcher_ = Window::Current->Dispatcher;
}

void ortc_standup::MainPage::StartStopButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
  if (started_) {
    stopEvent_.set();
    started_ = false;
  }
  else {
    stopEvent_.reset();

    Concurrency::create_task([this]() {
      int error;
#ifdef VOICE
      voiceChannel_ = voiceBase_->CreateChannel();
      if (voiceChannel_ < 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Could not create voice channel. Error: %d", voiceBase_->LastError());
        voiceChannel_ = -1;
        return Concurrency::create_task([](){});
      }

      voiceTransport_ = new webrtc::test::VoiceChannelTransport(voiceNetwork_, voiceChannel_);
      if (voiceTransport_ == NULL) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Could not create voice channel transport.");
        return Concurrency::create_task([](){});
      }

      webrtc::EcModes ecMode = webrtc::kEcAec;
      error = voiceAudioProcessing_->SetEcStatus(true, ecMode);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to set acoustic echo canceller status. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }
      if (ecMode == webrtc::kEcAecm) {
        error = voiceAudioProcessing_->SetAecmMode(webrtc::kAecmSpeakerphone);
        if (error != 0) {
          webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
            "Failed to set acoustic echo canceller mobile mode. Error: %d", voiceBase_->LastError());
          return Concurrency::create_task([](){});
        }
      }

      error = voiceAudioProcessing_->SetAgcStatus(true, webrtc::kAgcAdaptiveDigital);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to set automatic gain control status. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = voiceAudioProcessing_->SetNsStatus(true, webrtc::kNsLowSuppression);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to set noise suppression status. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = voiceVolumeControl_->SetInputMute(-1, false);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to set microphone mute. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

      webrtc::CodecInst cinst;
      memset(&cinst, 0, sizeof(webrtc::CodecInst));
      for (int idx = 0; idx < voiceCodec_->NumOfCodecs(); idx++) {
        error = voiceCodec_->GetCodec(idx, cinst);
        if (error != 0) {
          webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
            "Failed to get voice codec. Error: %d", voiceBase_->LastError());
          return Concurrency::create_task([](){});
        }
        if (strcmp(cinst.plname, "OPUS") == 0) {
          strcpy_s(cinst.plname, "OPUS");
          cinst.pltype = 110;
          cinst.rate = 20000;
          cinst.pacsize = 320; // 20ms
          cinst.plfreq = 16000;
          cinst.channels = 1;
          error = voiceCodec_->SetSendCodec(voiceChannel_, cinst);
          if (error != 0) {
            webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
              "Failed to set send voice codec. Error: %d", voiceBase_->LastError());
            return Concurrency::create_task([](){});
          }
          break;
        }
      }

      error = voiceTransport_->SetSendDestination(remoteIpAddress_.c_str(), audioPort_);

      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to set send destination for voice channel.");
        return Concurrency::create_task([](){});
      }

      error = voiceTransport_->SetLocalReceiver(audioPort_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to set local receiver for voice channel.");
        return Concurrency::create_task([](){});
      }

      error = voiceBase_->StartSend(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to start sending voice. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = voiceBase_->StartReceive(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to start receiving voice. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = voiceBase_->StartPlayout(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to start playout. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

#endif
#ifdef VIDEO
      const unsigned int KMaxDeviceNameLength = 128;
      const unsigned int KMaxUniqueIdLength = 256;
      char deviceName[KMaxDeviceNameLength];
      memset(deviceName, 0, KMaxDeviceNameLength);
      char uniqueId[KMaxUniqueIdLength];
      memset(uniqueId, 0, KMaxUniqueIdLength);
      uint32_t captureIdx = CAPTURE_DEVICE_INDEX;

      int devicesNumber = videoCapture_->NumberOfCaptureDevices();

      for (int i = 0; i < devicesNumber; i++) {

        error = videoCapture_->GetCaptureDevice(i, deviceName,
          KMaxDeviceNameLength, uniqueId,
          KMaxUniqueIdLength);
        if (error != 0) {
          webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
            "Failed to get video device name.");
          return Concurrency::create_task([](){});
        }

        webrtc::WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideo, -1,
          "Capture device - index: %d, name: %s, unique ID: %s", i, deviceName, uniqueId);
      }

      error = videoCapture_->GetCaptureDevice(captureIdx, deviceName,
        KMaxDeviceNameLength, uniqueId,
        KMaxUniqueIdLength);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to get video device name.");
        return Concurrency::create_task([](){});
      }

      strcpy_s(deviceUniqueId_, uniqueId);

      webrtc::WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideo, -1,
        "Selected capture device - index: %d, name: %s, unique ID: %s", captureIdx, deviceName, uniqueId);

      vcpm_ = webrtc::VideoCaptureFactory::Create(1, uniqueId);
      if (vcpm_ == NULL) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to create video capture module.");
        return Concurrency::create_task([](){});
      }

      error = videoCapture_->AllocateCaptureDevice(*vcpm_, captureId_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to allocate video capture device. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }
      vcpm_->AddRef();

      int capabilitiesNumber = videoCapture_->NumberOfCapabilities(uniqueId, KMaxUniqueIdLength);

      webrtc::CaptureCapability capability;
      int minWidthDiff = INT_MAX;
      int minHeightDiff = INT_MAX;
      int minFpsDiff = INT_MAX;

      for (int i = 0; i < capabilitiesNumber; i++) {

        webrtc::CaptureCapability deviceCapability;

        error = videoCapture_->GetCaptureCapability(uniqueId, KMaxUniqueIdLength, i, deviceCapability);
        if (error != 0) {
          webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
            "Failed to get capture capability.");
          return Concurrency::create_task([](){});
        }

        std::string deviceRawVideoFormat = GetRawVideoFormatString(deviceCapability.rawType);
        webrtc::WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideo, -1,
          "Capture capability - index: %d, width: %d, height: %d, max fps: %d, video format: %s",
          i, deviceCapability.width, deviceCapability.height, deviceCapability.maxFPS, deviceRawVideoFormat.c_str());

        if (deviceCapability.rawType == webrtc::kVideoMJPEG || deviceCapability.rawType == webrtc::kVideoUnknown)
          continue;

        int widthDiff = abs((int)(deviceCapability.width - PREFERRED_FRAME_WIDTH));
        if (widthDiff < minWidthDiff) {
          capability = deviceCapability;
          minWidthDiff = widthDiff;
        }
        else if (widthDiff == minWidthDiff) {
          int heightDiff = abs((int)(deviceCapability.height - PREFERRED_FRAME_HEIGHT));
          if (heightDiff < minHeightDiff) {
            capability = deviceCapability;
            minHeightDiff = heightDiff;
          }
          else if (heightDiff == minHeightDiff) {
            int fpsDiff = abs((int)(deviceCapability.maxFPS - PREFERRED_MAX_FPS));
            if (fpsDiff < minFpsDiff) {
              capability = deviceCapability;
              minFpsDiff = fpsDiff;
            }
          }
        }
      }

      std::string rawVideoFormat = GetRawVideoFormatString(capability.rawType);
      webrtc::WEBRTC_TRACE(webrtc::kTraceInfo, webrtc::kTraceVideo, -1,
        "Selected capture capability - width: %d, height: %d, max fps: %d, video format: %s",
        capability.width, capability.height, capability.maxFPS, rawVideoFormat.c_str());

      error = videoCapture_->StartCapture(captureId_, capability);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to start capturing. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      IInspectable* captureRendererPtr = reinterpret_cast<IInspectable*>(LocalVideoMediaElement);

      error = videoRender_->AddRenderer(captureId_, captureRendererPtr, 0, 0.0F, 0.0F, 1.0F, 1.0F);
      if (0 != error) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to add renderer for video capture. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoRender_->StartRender(captureId_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to start rendering video capture. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoBase_->CreateChannel(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Could not create video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      videoTransport_ = new webrtc::test::VideoChannelTransport(videoNetwork_, videoChannel_);
      if (videoTransport_ == NULL) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Could not create video channel transport.");
        return Concurrency::create_task([](){});
      }

      error = videoCapture_->ConnectCaptureDevice(captureId_, videoChannel_);
      if (0 != error) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to connect capture device to video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoRtpRtcp_->SetRTCPStatus(videoChannel_, webrtc::kRtcpCompound_RFC4585);
      if (0 != error) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to set video RTCP status. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoRtpRtcp_->SetKeyFrameRequestMethod(videoChannel_, webrtc::kViEKeyFrameRequestPliRtcp);
      if (0 != error) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to set key frame request method. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoRtpRtcp_->SetTMMBRStatus(videoChannel_, true);
      if (0 != error) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to set temporary max media bit rate status. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      webrtc::VideoCodec videoCodec;
      memset(&videoCodec, 0, sizeof(webrtc::VideoCodec));
      for (int idx = 0; idx < videoCodec_->NumberOfCodecs(); idx++) {
        error = videoCodec_->GetCodec(idx, videoCodec);
        if (error != 0) {
          webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
            "Failed to get video codec. Error: %d", videoBase_->LastError());
          return Concurrency::create_task([](){});
        }
        if (videoCodec.codecType == webrtc::kVideoCodecVP8) {
          videoCodec.width = PREFERRED_FRAME_WIDTH;
          videoCodec.height = PREFERRED_FRAME_HEIGHT;
          videoCodec.maxFramerate = PREFERRED_MAX_FPS;
          videoCodec.maxBitrate = MAX_BITRATE;
          error = videoCodec_->SetSendCodec(videoChannel_, videoCodec);
          if (error != 0) {
            webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
              "Failed to set send video codec. Error: %d", videoBase_->LastError());
            return Concurrency::create_task([](){});
          }
          break;
        }
      }

      error = videoTransport_->SetSendDestination(remoteIpAddress_.c_str(), videoPort_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to set send destination for video channel.");
        return Concurrency::create_task([](){});
      }

      error = videoTransport_->SetLocalReceiver(videoPort_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to set local receiver for video channel.");
        return Concurrency::create_task([](){});
      }

      error = videoBase_->StartSend(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to start sending video. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoBase_->StartReceive(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to start receiving video. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      IInspectable* channelRendererPtr = reinterpret_cast<IInspectable*>(RemoteVideoMediaElement);

      error = videoRender_->AddRenderer(videoChannel_, channelRendererPtr, 0, 0.0F, 0.0F, 1.0F, 1.0F);
      if (0 != error) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to add renderer for video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoRender_->StartRender(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to start rendering video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }
#endif

      Concurrency::create_task(dispatcher_->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
        StartStopButton->Content = "Stop";
      })));


      started_ = true;
      stopEvent_.wait();

#ifdef VOICE

      error = voiceBase_->StopSend(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to stop sending voice. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }
      error = voiceBase_->StopPlayout(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to stop playout. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }
      error = voiceBase_->StopReceive(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to stop receiving voice. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }
      if (voiceTransport_)
        delete voiceTransport_;
      error = voiceBase_->DeleteChannel(voiceChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVoice, -1,
          "Failed to delete voice channel. Error: %d", voiceBase_->LastError());
        return Concurrency::create_task([](){});
      }

      voiceChannel_ = -1;
#endif
#ifdef VIDEO
      error = videoRender_->StopRender(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to stop rendering video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoRender_->RemoveRenderer(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to remove renderer for video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoBase_->StopSend(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to stop sending video. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }
      error = videoBase_->StopReceive(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to stop receiving video. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }
      error = videoCapture_->DisconnectCaptureDevice(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to disconnect capture device from video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }
      if (videoTransport_)
        delete videoTransport_;
      error = videoBase_->DeleteChannel(videoChannel_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to delete video channel. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      videoChannel_ = -1;

      error = videoRender_->StopRender(captureId_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to stop rendering video capture. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }
      error = videoRender_->RemoveRenderer(captureId_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to remove renderer for video capture. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoCapture_->StopCapture(captureId_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to stop video capturing. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      error = videoCapture_->ReleaseCaptureDevice(captureId_);
      if (error != 0) {
        webrtc::WEBRTC_TRACE(webrtc::kTraceError, webrtc::kTraceVideo, -1,
          "Failed to release video capture device. Error: %d", videoBase_->LastError());
        return Concurrency::create_task([](){});
      }

      if (vcpm_ != NULL)
        vcpm_->Release();

      vcpm_ = NULL;

      captureId_ = -1;
#endif

      started_ = false;

      return Concurrency::create_task(dispatcher_->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]() {
        StartStopButton->Content = "Start";
      })));
    });
  }
}

std::string ortc_standup::MainPage::GetRawVideoFormatString(webrtc::RawVideoType videoType) {

  std::string videoFormatString;
  switch (videoType)
  {
  case webrtc::kVideoYV12:
    videoFormatString = "YV12";
    break;
  case webrtc::kVideoYUY2:
    videoFormatString = "YUY2";
    break;
  case webrtc::kVideoI420:
    videoFormatString = "I420";
    break;
  case webrtc::kVideoIYUV:
    videoFormatString = "IYUV";
    break;
  case webrtc::kVideoRGB24:
    videoFormatString = "RGB24";
    break;
  case webrtc::kVideoARGB:
    videoFormatString = "ARGB";
    break;
  case webrtc::kVideoMJPEG:
    videoFormatString = "MJPEG";
    break;
  case webrtc::kVideoNV12:
    videoFormatString = "NV12";
    break;
  default:
    videoFormatString = "Not supported";
    break;
  }
  return videoFormatString;
}