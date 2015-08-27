#pragma once

#include <zsLib/Promise.h>
#include <ortc/IMediaDevices.h>
#include <ortc/IMediaStreamTrack.h>
#include <ortc/IRTPSender.h>
#include <ortc/IRTPReceiver.h>

ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::PromiseWithDeviceList, PromiseWithDeviceList)
ZS_DECLARE_TYPEDEF_PTR(ortc::IMediaDevicesTypes::PromiseWithMediaStreamTrackList, PromiseWithMediaStreamTrackList)

namespace ortc_standup
{
  class MediaElementWrapper;
  ZS_DECLARE_CLASS_PTR(PromiseWithMediaStreamTrackListCallback)
  ZS_DECLARE_CLASS_PTR(PromiseWithDeviceListCallback)

  class MediaEngine : public ortc::IRTPSenderDelegate,
                      public ortc::IRTPReceiverDelegate
  {
    friend PromiseWithDeviceListCallback;
    friend PromiseWithMediaStreamTrackListCallback;

  public:
    MediaEngine();
    ~MediaEngine();
    void SetLocalMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetRemoteMediaElement(Windows::UI::Xaml::Controls::MediaElement^ element);
    void SetStartStopButton(Windows::UI::Xaml::Controls::Button^ button);
    void StartStopMedia();

  private:
    virtual void onRTPSenderError(
                                  ortc::IRTPSenderPtr sender,
                                  IRTPSenderDelegate::ErrorCode errorCode,
                                  zsLib::String errorReason
                                  );

    virtual void onRTPSenderSSRCConflict(
                                         ortc::IRTPSenderPtr sender,
                                         SSRCType ssrc
                                         );

    virtual void onRTPReceiverError(
                                    ortc::IRTPReceiverPtr sender,
                                    IRTPReceiverDelegate::ErrorCode errorCode,
                                    zsLib::String errorReason
                                    );


    Windows::UI::Core::CoreDispatcher^ mDispatcher;
    bool mStarted;

    Windows::UI::Xaml::Controls::Button^ mStartStopButton;
    MediaElementWrapper* mLocalMediaWrapper;
    MediaElementWrapper* mRemoteMediaWrapper;
    PromiseWithDeviceListPtr mPromiseWithDeviceList;
    PromiseWithMediaStreamTrackListPtr mPromiseWithMediaStreamTrackList;
    ortc::IMediaStreamTrackPtr mLocalVideoMediaStreamTrack;
    ortc::IMediaStreamTrackPtr mRemoteVideoMediaStreamTrack;
  };
}
