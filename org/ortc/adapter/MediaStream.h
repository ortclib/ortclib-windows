#pragma once

#include <ortc/adapter/IMediaStream.h>
#include "../MediaStreamTrack.h"

namespace Org
{
  namespace Ortc
  {
    ref class RTCStatsProvider;

    namespace Adapter
    {
      using Windows::Foundation::Collections::IVector;

      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IMediaStream, IMediaStream);
      ZS_DECLARE_TYPEDEF_PTR(::ortc::adapter::IMediaStreamSubscription, IMediaStreamSubscription);

      ref class RTCPeerConnection;

      namespace Internal
      {
        ZS_DECLARE_CLASS_PTR(MediaStreamDelegate);
        ZS_DECLARE_CLASS_PTR(RTCPeerConnectionDelegate);
      }

      public delegate void MediaStreamAddTrackDelegate(MediaStreamTrack^ track);
      public delegate void MediaStreamRemoveTrackDelegate(MediaStreamTrack^ track);

      /// <summary>
      /// The two main components in the MediaStream API are the
      /// MediaStreamTrack and MediaStream interfaces. The MediaStreamTrack
      /// object represents media of a single type that originates from one
      /// media source in the User Agent, e.g. video produced by a web camera.
      /// A MediaStream is used to group several MediaStreamTrack objects into
      /// one unit that can be recorded or rendered in a media element.
      /// </summary>
      public ref class MediaStream sealed
      {
      private:
        friend class Internal::MediaStreamDelegate;
        friend class Internal::RTCPeerConnectionDelegate;
        friend ref class RTCPeerConnection;
        friend ref class RTCStatsProvider;

      private:
        MediaStream(IMediaStreamPtr stream);

        static MediaStream^ Convert(IMediaStreamPtr stream) { if (!stream) return nullptr; return ref new MediaStream(stream); }
        static IMediaStreamPtr Convert(MediaStream^ stream) { if (!stream) return nullptr; return stream->_nativePointer; }

      public:

        /// <summary>
        /// Constructs an empty MediaStream object.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute]
        MediaStream();

        /// <summary>
        /// Constructs an instance of an MediaStream and clones the tracks
        /// from the original MediaStream.
        /// </summary>
        [Windows::Foundation::Metadata::OverloadAttribute("CreateWithStream")]
        MediaStream(MediaStream^ stream);

        /// <summary>
        /// Constructs an instance of an MediaStream containing a list of
        /// MediaStreamTracks supplied.
        /// </summary>
        [Windows::Foundation::Metadata::DefaultOverloadAttribute(), Windows::Foundation::Metadata::OverloadAttribute("CreateWithTracks")]
        MediaStream(IVector<MediaStreamTrack^>^ tracks);

        /// <summary>
        /// Gets the media stream identifier. When a MediaStream object is
        /// created, the engine must generate an identifier string, and must
        /// initialize the object's id attribute to that string. A good
        /// practice is to use a UUID [RFC4122], which is 36 characters long
        /// in its canonical form. To avoid fingerprinting, implementations
        /// should use the forms in section 4.4 or 4.5 of [RFC4122] when
        /// generating UUIDs.
        /// </summary>
        property Platform::String^ Id
        {
          Platform::String^ get();
        }

        /// <summary>
        /// Gets an indicator if this MediaStream is active.
        /// </summary>
        property Platform::Boolean Active
        {
          Platform::Boolean get();
        }

        /// <summary>
        /// Returns a sequence of MediaStreamTrack objects representing the
        /// audio tracks in this stream.
        /// </summary>
        IVector<MediaStreamTrack^>^ GetAudioTracks();
        /// <summary>
        /// Returns a sequence of MediaStreamTrack objects representing the
        /// video tracks in this stream.
        /// </summary>
        IVector<MediaStreamTrack^>^ GetVideoTracks();
        /// <summary>
        /// Returns a sequence of MediaStreamTrack objects representing all
        /// the tracks in this stream.
        /// </summary>
        IVector<MediaStreamTrack^>^ GetTracks();
        /// <summary>
        /// The GetTrackById() method must return either a MediaStreamTrack
        /// object from this stream's track set whose id is equal to trackId,
        /// or null, if no such track exists.
        /// </summary>
        MediaStreamTrack^           GetTrackById(Platform::String^ trackId);


        /// <summary>
        /// Adds the given MediaStreamTrack to this MediaStream.
        /// </summary>
        void                        AddTrack(MediaStreamTrack^ track);
        /// <summary>
        /// Removes the given MediaStreamTrack object from this MediaStream.
        /// </summary>
        void                        RemoveTrack(MediaStreamTrack^ track);

        /// <summary>
        /// Clones the given MediaStream and all its tracks.
        /// </summary>
        MediaStream^                Clone();

        /// <summary>
        /// This event fires when a new MediaStreamTrack has been added to
        /// this stream. Note that this event is not fired when the script
        /// directly modifies the tracks of a MediaStream.
        /// </summary>
        event MediaStreamAddTrackDelegate^    OnAddTrack;
        /// <summary>
        /// This event fires when a MediaStreamTrack has been removed from
        /// this stream. Note that this event is not fired when the script
        /// directly modifies the tracks of a MediaStream.
        /// </summary>
        event MediaStreamRemoveTrackDelegate^ OnRemoveTrack;

      private:
        IMediaStreamPtr _nativePointer;
        Internal::MediaStreamDelegatePtr _nativeDelegatePointer;
        IMediaStreamSubscriptionPtr _nativeSubscriptionPointer;
      };

    }

  } // namespace ortc
} // namespace org
