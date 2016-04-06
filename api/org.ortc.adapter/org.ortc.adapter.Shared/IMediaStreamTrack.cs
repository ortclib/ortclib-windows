using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace org
{
    namespace ortc
    {
        namespace adapter
        {
            /// <summary>
            /// An IMediaStreamTrack object represents media of a single type that
            /// originates from one media source, e.g. video produced by a web camera.
            /// </summary>
            /// <remarks>
            /// http://www.w3.org/TR/mediacapture-streams
            /// </remarks>
            public interface IMediaStreamTrack
            {
                /// <summary>
                /// Type of media, "audio" or "video".
                /// </summary>
                String Kind { get; }

                /// <summary>
                /// Identifier.
                /// </summary>
                String Id { get; }

                //String Cname { get; set; }

                String Label { get; set; }

                uint SsrcId { get; set; }

                /// <summary>
                /// Controls the enabled state for the track.
                /// </summary>
                Boolean Enabled { get; set; }

                /// <summary>
                /// TODO(WINRT): check implementations, they may not be
                /// conforming to the spec:
                /// http://www.w3.org/TR/mediacapture-streams/#widl-MediaStreamTrack-stop-void
                /// </summary>
                void Stop();
            };
        }
    }
}