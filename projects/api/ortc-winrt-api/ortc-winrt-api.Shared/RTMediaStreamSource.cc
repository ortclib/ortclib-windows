// Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.
#include "pch.h"
#include "RTMediaStreamSource.h"
#include <mfapi.h>
#include <ppltasks.h>
#include <mfidl.h>
#include "libyuv/convert.h"
#include "webrtc/system_wrappers/include/critical_section_wrapper.h"
#include "Ortc.h"
#include "helpers.h"

using Microsoft::WRL::ComPtr;
using Platform::Collections::Vector;
using Windows::Media::Core::VideoStreamDescriptor;
using Windows::Media::Core::MediaStreamSourceSampleRequestedEventArgs;
using Windows::Media::Core::MediaStreamSourceSampleRequest;
using Windows::Media::Core::MediaStreamSourceStartingEventArgs;
using Windows::Media::Core::MediaStreamSource;
using Windows::Media::MediaProperties::VideoEncodingProperties;
using Windows::Media::MediaProperties::MediaEncodingSubtypes;
using Windows::System::Threading::TimerElapsedHandler;
using Windows::System::Threading::ThreadPoolTimer;

namespace Microsoft
{
  namespace WRL
  {
    using ::ULONG;
    using ::DWORD;
    using ::UINT;
    using ::LONG;
    using ::BYTE;
  }
}

namespace Concurrency
{
  using ::LONG;
}

namespace ortc_winrt_api {

MediaStreamSource^ RTMediaStreamSource::CreateMediaSource(
  MediaStreamTrack^ track, uint32 frameRate, Platform::String^ id) {

  bool isH264 = false; // track->GetImpl()->GetSource()->IsH264Source();

  auto streamState = ref new RTMediaStreamSource(track, isH264);
  streamState->_id = id;
  streamState->_rtcRenderer = rtc::scoped_ptr<RTCRenderer>(
    new RTCRenderer(streamState));
  CallPrivateMethod::SetMediaElement(track, (void*)streamState->_rtcRenderer.get());
  VideoEncodingProperties^ videoProperties;
  if (isH264) {
    videoProperties = VideoEncodingProperties::CreateH264();
    //videoProperties->ProfileId = Windows::Media::MediaProperties::H264ProfileIds::Baseline;
  } else {
    videoProperties =
      VideoEncodingProperties::CreateUncompressed(
        MediaEncodingSubtypes::Nv12, 10, 10);
  }
  streamState->_videoDesc = ref new VideoStreamDescriptor(videoProperties);

  // initial value, this will be override by incoming frame from webrtc.
  // this is needed since the UI element might request sample before webrtc has
  // incoming frame ready(ex.: remote stream), in this case, this initial value
  // will make sure we will at least create a small dummy frame.
  streamState->_videoDesc->EncodingProperties->Width = 720;
  streamState->_videoDesc->EncodingProperties->Height = 1280;

  ResolutionHelper::FireEvent(id,
    streamState->_videoDesc->EncodingProperties->Width,
    streamState->_videoDesc->EncodingProperties->Height);

  streamState->_videoDesc->EncodingProperties->FrameRate->Numerator =
                                                                    frameRate;
  streamState->_videoDesc->EncodingProperties->FrameRate->Denominator = 1;
  auto streamSource = ref new MediaStreamSource(streamState->_videoDesc);

  auto startingCookie = streamSource->Starting +=
    ref new Windows::Foundation::TypedEventHandler<
    MediaStreamSource ^,
    MediaStreamSourceStartingEventArgs ^>([streamState](
      MediaStreamSource^ sender,
      MediaStreamSourceStartingEventArgs^ args) {
    // Get a deferall on the starting event so we can trigger it
    // when the first frame arrives.
    streamState->_startingDeferral = args->Request->GetDeferral();
  });

  streamState->_mediaStreamSource = streamSource;

  // Use a lambda to capture a strong reference to RTMediaStreamSource.
  // This is the only way to tie the lifetime of the RTMediaStreamSource
  // to that of the MediaStreamSource.
  auto sampleRequestedCookie = streamSource->SampleRequested +=
    ref new Windows::Foundation::TypedEventHandler<
    MediaStreamSource^,
    MediaStreamSourceSampleRequestedEventArgs^>([streamState](
        MediaStreamSource^ sender,
        MediaStreamSourceSampleRequestedEventArgs^ args) {
    streamState->OnSampleRequested(sender, args);
  });
  streamSource->Closed +=
    ref new Windows::Foundation::TypedEventHandler<
      Windows::Media::Core::MediaStreamSource^,
      Windows::Media::Core::MediaStreamSourceClosedEventArgs ^>(
        [streamState, startingCookie, sampleRequestedCookie](
        Windows::Media::Core::MediaStreamSource^ sender,
        Windows::Media::Core::MediaStreamSourceClosedEventArgs^ args) {
    //LOG(LS_INFO) << "RTMediaStreamSource::OnClosed";
    streamState->Teardown();
    sender->Starting -= startingCookie;
    sender->SampleRequested -= sampleRequestedCookie;
  });

  // Create a timer which sends request progress periodically.
  {
    auto handler = ref new TimerElapsedHandler(streamState,
      &RTMediaStreamSource::ProgressTimerElapsedExecute);
    auto timespan = Windows::Foundation::TimeSpan();
    timespan.Duration = 500 * 1000 * 10;  // 500 ms in hns
    streamState->_progressTimer = ThreadPoolTimer::CreatePeriodicTimer(
      handler, timespan);
  }

  // Create a timer which ensures we don't display frames faster that expected.
  // Required because Media Foundation sometimes requests samples in burst mode
  // but we use the wall clock to drive timestamps.
  {
    auto handler = ref new TimerElapsedHandler(streamState,
      &RTMediaStreamSource::FPSTimerElapsedExecute);
    auto timespan = Windows::Foundation::TimeSpan();
    timespan.Duration = 15 * 1000 * 10;
    streamState->_fpsTimer = ThreadPoolTimer::CreatePeriodicTimer(handler,
      timespan);
  }

  return streamSource;
}

RTMediaStreamSource::RTMediaStreamSource(MediaStreamTrack^ videoTrack,
                                         bool isH264) :
    _videoTrack(videoTrack),
    _lock(webrtc::CriticalSectionWrapper::CreateCriticalSection()),
    _frameSentThisTime(false) {
  //LOG(LS_INFO) << "RTMediaStreamSource::RTMediaStreamSource";

  // Create the helper with the callback functions.
  _helper.reset(new MediaSourceHelper(isH264,
    [this](webrtc::VideoFrame* frame, IMFSample** sample) -> HRESULT {
    return MakeSampleCallback(frame, sample);
  },
    [this](int fps) {
    return FpsCallback(fps);
  }));
}

RTMediaStreamSource::~RTMediaStreamSource() {
  //LOG(LS_INFO) << "RTMediaStreamSource::~RTMediaStreamSource : "
  //             << rtc::ToUtf8(_id->Data()).c_str();
  Teardown();
}

void RTMediaStreamSource::Teardown() {
  //LOG(LS_INFO) << "RTMediaStreamSource::Teardown()";
  webrtc::CriticalSectionScoped csLock(_lock.get());
  if (_progressTimer != nullptr) {
    _progressTimer->Cancel();
    _progressTimer = nullptr;
  }
  if (_fpsTimer != nullptr) {
    _fpsTimer->Cancel();
    _fpsTimer = nullptr;
  }
  if (_rtcRenderer != nullptr && _videoTrack != nullptr) {
    //_videoTrack->UnsetRenderer(_rtcRenderer.get());
  }
  _videoTrack = nullptr;
  _rtcRenderer.reset();

  _request = nullptr;
  if (_deferral != nullptr) {
    _deferral->Complete();
    _deferral = nullptr;
  }
  if (_startingDeferral != nullptr) {
    _startingDeferral->Complete();
    _startingDeferral = nullptr;
  }
  _helper.reset();
}

RTMediaStreamSource::RTCRenderer::RTCRenderer(
  RTMediaStreamSource^ streamSource) : _streamSource(streamSource) {
}

RTMediaStreamSource::RTCRenderer::~RTCRenderer() {
  //LOG(LS_INFO) << "RTMediaStreamSource::RTCRenderer::~RTCRenderer";
}

void RTMediaStreamSource::RTCRenderer::SetSize(
  uint32 width, uint32 height, uint32 reserved) {
  auto stream = _streamSource.Resolve<RTMediaStreamSource>();
  if (stream != nullptr) {
    stream->ResizeSource(width, height);
  }
}

int32_t RTMediaStreamSource::RTCRenderer::RenderFrame(const uint32_t streamId,
  const webrtc::VideoFrame& frame) {
  webrtc::VideoFrame* frameCopy = new webrtc::VideoFrame();
  frameCopy->CopyFrame(frame);
  // Do the processing async because there's a risk of a deadlock otherwise.
  Concurrency::create_async([this, frameCopy] {
    auto stream = _streamSource.Resolve<RTMediaStreamSource>();
    if (stream != nullptr) {
      stream->ProcessReceivedFrame(frameCopy);
    }
  });
  return 0;
}

void RTMediaStreamSource::ProgressTimerElapsedExecute(ThreadPoolTimer^ source) {
  webrtc::CriticalSectionScoped csLock(_lock.get());
  if (_request != nullptr) {
    _request->ReportSampleProgress(1);
  }
}

void RTMediaStreamSource::FPSTimerElapsedExecute(ThreadPoolTimer^ source) {
  webrtc::CriticalSectionScoped csLock(_lock.get());
  _frameSentThisTime = false;
  if (_request != nullptr) {
    ReplyToSampleRequest();
  }
}

void RTMediaStreamSource::ReplyToSampleRequest() {
  auto sampleData = _helper->DequeueFrame();
  if (sampleData == nullptr) {
    return;
  }

  // Update rotation property
  if (sampleData->rotationHasChanged)
  {
    auto props = _videoDesc->EncodingProperties->Properties;
    OutputDebugString((L"Video rotation changed: " + sampleData->rotation + "\r\n")->Data());
    props->Insert(MF_MT_VIDEO_ROTATION, sampleData->rotation);
  }

  // Frame size in EncodingProperties needs to be updated before completing
  // deferral, otherwise the MediaElement will receive a frame having different
  // size and application may crash.
  if (sampleData->sizeHasChanged) {
    auto props = _videoDesc->EncodingProperties;
    props->Width = (unsigned int)sampleData->size.cx;
    props->Height = (unsigned int)sampleData->size.cy;
      ResolutionHelper::FireEvent(
        _id, props->Width, props->Height);
      OutputDebugString((L"Video frame size changed for " + _id +
        L" W=" + props->Width +
        L" H=" + props->Height + L"\r\n")->Data());
  }

  Microsoft::WRL::ComPtr<IMFMediaStreamSourceSampleRequest> spRequest;
  HRESULT hr = reinterpret_cast<IInspectable*>(_request)->QueryInterface(
    spRequest.ReleaseAndGetAddressOf());

  //Fixme: it appears that MSS works well if we hardcode the sample duration with 33ms,
  // Don' know why, override the value set by the mediahelper
  LONGLONG duration = (LONGLONG)((1.0 / 30) * 1000 * 1000 * 10);
  sampleData->sample.Get()->SetSampleDuration(duration);

  hr = spRequest->SetSample(sampleData->sample.Get());

  if (_deferral != nullptr) {
    _deferral->Complete();
  }

  _frameSentThisTime = true;

  _request = nullptr;
  _deferral = nullptr;
}

HRESULT RTMediaStreamSource::MakeSampleCallback(
  webrtc::VideoFrame* frame, IMFSample** sample) {
  ComPtr<IMFSample> spSample;
  HRESULT hr = MFCreateSample(spSample.GetAddressOf());
  if (FAILED(hr)) {
    return E_FAIL;
  }
  ComPtr<IMFMediaBuffer> mediaBuffer;
  hr = MFCreate2DMediaBuffer(
    (::DWORD)frame->width(), (::DWORD)frame->height(),
    MFVideoFormat_NV12.Data1, FALSE,
    mediaBuffer.GetAddressOf());
  if (FAILED(hr)) {
    return E_FAIL;
  }

  spSample->AddBuffer(mediaBuffer.Get());

  ComPtr<IMF2DBuffer2> imageBuffer;
  if (FAILED(mediaBuffer.As(&imageBuffer))) {
    return E_FAIL;
  }

  ::BYTE* destRawData;
  ::BYTE* buffer;
  ::LONG pitch;
  ::DWORD destMediaBufferSize;

  if (FAILED(imageBuffer->Lock2DSize(MF2DBuffer_LockFlags_Write,
    &destRawData, &pitch, &buffer, &destMediaBufferSize))) {
    return E_FAIL;
  }
  try {
    //frame->MakeExclusive();
    // Convert to NV12
    uint8* uvDest = destRawData + (pitch * frame->height());
   /* libyuv::I420ToNV12(frame->GetYPlane(), frame->GetYPitch(),
      frame->GetUPlane(), frame->GetUPitch(),
      frame->GetVPlane(), frame->GetVPitch(),
      reinterpret_cast<uint8*>(destRawData), pitch,
      uvDest, pitch,
      static_cast<int>(frame->GetWidth()),
      static_cast<int>(frame->GetHeight()));*/

    libyuv::I420ToNV12(frame->video_frame_buffer()->data(webrtc::PlaneType::kYPlane), frame->video_frame_buffer()->stride(webrtc::PlaneType::kYPlane),
      frame->video_frame_buffer()->data(webrtc::PlaneType::kUPlane), frame->video_frame_buffer()->stride(webrtc::PlaneType::kUPlane),
      frame->video_frame_buffer()->data(webrtc::PlaneType::kVPlane), frame->video_frame_buffer()->stride(webrtc::PlaneType::kVPlane),
      reinterpret_cast<uint8*>(destRawData), pitch,
      uvDest, pitch,
      static_cast<int>(frame->width()),
      static_cast<int>(frame->height()));
  }
  catch (...) {
    //LOG(LS_ERROR) << "Exception caught in RTMediaStreamSource::ConvertFrame()";
  }
  imageBuffer->Unlock2D();

  *sample = spSample.Detach();
  return S_OK;
}

void RTMediaStreamSource::FpsCallback(int fps) {
  FrameCounterHelper::FireEvent(
    _id, fps.ToString());
}

void RTMediaStreamSource::OnSampleRequested(
  MediaStreamSource ^sender, MediaStreamSourceSampleRequestedEventArgs ^args) {
  try {
    // Check to detect cases where samples are still being requested
    // but the source has ended.
    //auto trackState = _videoTrack->GetImpl()->GetSource()->state();
    //if (trackState == webrtc::MediaSourceInterface::kEnded) {
    //  return;
    //}
    if (_mediaStreamSource == nullptr)
      return;

    webrtc::CriticalSectionScoped csLock(_lock.get());

    _request = args->Request;
    if (_request == nullptr) {
      return;
    }

    if (!_frameSentThisTime && _helper->HasFrames()) {
      ReplyToSampleRequest();
      return;
    }
    else {
      // Save the request and referral for when a sample comes in.
      if (_deferral != nullptr) {
        //LOG(LS_ERROR) << "Got deferral when another hasn't completed.";
      }
      _deferral = _request->GetDeferral();
      return;
    }
  }
  catch (...) {
    //LOG(LS_ERROR) << "Exception in RTMediaStreamSource::OnSampleRequested.";
  }
}

void RTMediaStreamSource::ProcessReceivedFrame(
  webrtc::VideoFrame *frame) {
  webrtc::CriticalSectionScoped csLock(_lock.get());

  if (_startingDeferral != nullptr) {
    _startingDeferral->Complete();
    _startingDeferral = nullptr;
  }

  _helper->QueueFrame(frame);

  // If we have a pending request, reply to it now.
  if (_deferral != nullptr && _request != nullptr && !_frameSentThisTime) {
    ReplyToSampleRequest();
  }
}

void RTMediaStreamSource::ResizeSource(uint32 width, uint32 height) {
}

void FrameCounterHelper::FireEvent(Platform::String^ id,
  Platform::String^ str) {
  if (OrtcWithDispatcher::Dispatcher != nullptr) {
    OrtcWithDispatcher::Dispatcher->RunAsync(
      Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([id, str] {
      FramesPerSecondChanged(id, str);
    }));
  }
  else {
    FramesPerSecondChanged(id, str);
  }
}

void ResolutionHelper::FireEvent(Platform::String^ id,
  unsigned int width, unsigned int heigth) {
  if (OrtcWithDispatcher::Dispatcher != nullptr) {
    OrtcWithDispatcher::Dispatcher->RunAsync(
      Windows::UI::Core::CoreDispatcherPriority::Normal,
      ref new Windows::UI::Core::DispatchedHandler([id, width, heigth] {
      ResolutionChanged(id, width, heigth);
    }));
  }
  else {
    ResolutionChanged(id, width, heigth);
  }
}

}  // namespace ortc_winrt_api

//extern Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;
