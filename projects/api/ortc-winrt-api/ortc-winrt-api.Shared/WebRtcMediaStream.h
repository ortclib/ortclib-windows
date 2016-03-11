
// Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#pragma once
#include "pch.h"
#include <wrl.h>
#include <mfapi.h>
#include <vector>
#include "webrtc/system_wrappers/include/critical_section_wrapper.h"
#include "webrtc/base/scoped_ptr.h"
#include "MediaSourceHelper.h"
#include "WebRtcMediaSource.h"
#include "webrtc/modules/video_render/include/video_render.h"

using Microsoft::WRL::ComPtr;
using Microsoft::WRL::RuntimeClass;
using Microsoft::WRL::RuntimeClassFlags;
using Microsoft::WRL::RuntimeClassType;
using Windows::System::Threading::ThreadPoolTimer;

namespace org
{
  namespace ortc
  {
    class WebRtcMediaSource;
    ref class MediaStreamTrack;

    class WebRtcMediaStream :
      public RuntimeClass<RuntimeClassFlags<RuntimeClassType::WinRtClassicComMix>,
      IMFMediaStream, IMFMediaEventGenerator,
      IMFGetService> {
      InspectableClass(L"WebRtcMediaStream", BaseTrust)
    public:
      WebRtcMediaStream();
      virtual ~WebRtcMediaStream();
      HRESULT RuntimeClassInitialize(
        WebRtcMediaSource* source,
        MediaStreamTrack^ track,
        Platform::String^ id);

      // IMFMediaEventGenerator
      IFACEMETHOD(GetEvent)(DWORD dwFlags, IMFMediaEvent **ppEvent);
      IFACEMETHOD(BeginGetEvent)(IMFAsyncCallback *pCallback, IUnknown *punkState);
      IFACEMETHOD(EndGetEvent)(IMFAsyncResult *pResult, IMFMediaEvent **ppEvent);
      IFACEMETHOD(QueueEvent)(MediaEventType met, const GUID& guidExtendedType,
        HRESULT hrStatus, const PROPVARIANT *pvValue);
      // IMFMediaStream
      IFACEMETHOD(GetMediaSource)(IMFMediaSource **ppMediaSource);
      IFACEMETHOD(GetStreamDescriptor)(IMFStreamDescriptor **ppStreamDescriptor);
      IFACEMETHOD(RequestSample)(IUnknown *pToken);
      // IMFGetService
      IFACEMETHOD(GetService)(REFGUID guidService, REFIID riid, LPVOID *ppvObject);

      STDMETHOD(Start)(IMFPresentationDescriptor *pPresentationDescriptor,
        const GUID *pguidTimeFormat, const PROPVARIANT *pvarStartPosition);
      STDMETHOD(Stop)();
      STDMETHOD(Shutdown)();
      STDMETHOD(SetD3DManager)(ComPtr<IMFDXGIDeviceManager> manager);

      rtc::scoped_ptr<webrtc::CriticalSectionWrapper> _lock;

    private:
      class RTCRenderer : public webrtc::VideoRenderCallback {
      public:
        explicit RTCRenderer(WebRtcMediaStream* streamSource);
        virtual ~RTCRenderer();
        virtual void SetSize(uint32 width, uint32 height, uint32 reserved);
        virtual int32_t RenderFrame(const uint32_t streamId,
          const webrtc::VideoFrame& videoFrame);
        virtual bool CanApplyRotation() { return true; }
      private:
        // This object is owned by RTMediaStreamSource
        // so _streamSource must be a weak reference
        WebRtcMediaStream* _streamSource;
      };

    private:
      ComPtr<IMFMediaEventQueue> _eventQueue;

      WebRtcMediaSource* _source;
      MediaStreamTrack^ _track;
      Platform::String^ _id;

      static HRESULT CreateMediaType(unsigned int width, unsigned int height,
        unsigned int rotation, IMFMediaType** ppType, bool isH264);
      HRESULT MakeSampleCallback(webrtc::VideoFrame* frame, IMFSample** sample);
      void FpsCallback(int fps);

      HRESULT ReplyToSampleRequest();

      rtc::scoped_ptr<MediaSourceHelper> _helper;
      rtc::scoped_ptr<RTCRenderer> _rtcRenderer;

      ComPtr<IMFMediaType> _mediaType;
      ComPtr<IMFDXGIDeviceManager> _deviceManager;
      ComPtr<IMFStreamDescriptor> _streamDescriptor;
      ULONGLONG _startTickCount;
      ULONGLONG _frameCount;
      int _index;
      ULONG _frameReady;

      // From the sample manager.
      HRESULT ResetMediaBuffers();
      static const int BufferCount = 3;
      std::vector<ComPtr<IMFMediaBuffer>> _mediaBuffers;
      int _frameBufferIndex;

      // Used to smooth out frame samples to prevent sample being
      // rendered faster than a certain rate.
      ThreadPoolTimer^ _fpsTimer;
      void FPSTimerElapsedExecute(ThreadPoolTimer^ source);
      bool _frameSentThisTime;

      bool _gpuVideoBuffer;
      bool _isH264;
    };
  } // namespace ortc
}  // namespace org
