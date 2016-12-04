#pragma once

namespace Org
{
  namespace Ortc
  {
    namespace Log
    {
      /// <summary>
      /// The level represents the detail level used when logging.
      /// </summary>
      public enum class Level
      {
        /// <summary>
        /// Do not log any log messages.
        /// </summary>
        None,
        /// <summary>
        /// Log only the most important logger information.
        /// </summary>
        Basic,
        /// <summary>
        /// Log a few more details from the logger information.
        /// </summary>
        Detail,
        /// <summary>
        /// Log a level appropriate to debug the engine.
        /// </summary>
        Debug,
        /// <summary>
        /// Log a level to trace the code path within the engine.
        /// </summary>
        Trace,
        /// <summary>
        /// Log every possible mundane detail possible.
        /// </summary>
        Insane
      };

      /// <summary>
      /// The component represents the area of code that is capable of
      /// logging.
      /// </summary>
      public enum class Component
      {
        /// <summary>
        /// The low level socket, aynchronous eventing, and OS abstraction
        /// library.
        /// </summary>
        ZsLib,
        /// <summary>
        /// The socket monitor within the low level socket abstraction
        /// library.
        /// </summary>
        ZsLibSocket,
        /// <summary>
        /// The servers support library for the ORTC library.
        /// </summary>
        Services,
        /// <summary>
        /// The DNS services support library for the ORTC library.
        /// </summary>
        ServicesDns,
        /// <summary>
        /// The TURN services support library for the ORTC library.
        /// </summary>
        ServicesTurn,
        /// <summary>
        /// The HTTP services support library for the ORTC library.
        /// </summary>
        ServicesHttp,
        /// <summary>
        /// The wire level tracing of services support for the ORTC library.
        /// This component also includes the details of the STUN packet(s).
        /// </summary>
        ServicesWire,
        /// <summary>
        /// All STUN related request processing (minus the STUN on the wire).
        /// </summary>
        ServicesStun,
        /// <summary>
        /// The C++ ORTC library (for all objects who don't have their own
        /// trace level).
        /// </summary>
        OrtcLib,
        /// <summary>
        /// The C++ ORTC media area.
        /// </summary>
        OrtcLibWebrtc,
        /// <summary>
        /// The C++ ORTC DTLS transport object.
        /// </summary>
        OrtcLibDtlsTransport,
        /// <summary>
        /// The C++ ORTC ICE gatherer object.
        /// </summary>
        OrtcLibIceGatherer,
        /// <summary>
        /// The C++ ORTC ICE gatherer router object.
        /// </summary>
        OrtcLibIceGathererRouter,
        /// <summary>
        /// The C++ ORTC ICE transport object.
        /// </summary>
        OrtcLibIceTransport,
        /// <summary>
        /// The C++ ORTC ICE transport controller object.
        /// </summary>
        OrtcLibIceTransportController,
        /// <summary>
        /// The C++ ORTC media devices object.
        /// </summary>
        OrtcLibMediaDevices,
        /// <summary>
        /// The C++ ORTC media stream track object.
        /// </summary>
        OrtcLibMediaStreamTrack,
        /// <summary>
        /// The C++ ORTC rtp / rtcp packet related traces.
        /// </summary>
        OrtcLibRtpRtcpPacket,
        /// <summary>
        /// The C++ ORTC rtp listener object.
        /// </summary>
        OrtcLibRtpListener,
        /// <summary>
        /// The C++ ORTC rtp listener object.
        /// </summary>
        OrtcLibRtpMediaEngine,
        /// <summary>
        /// The C++ ORTC rtp receiver object.
        /// </summary>
        OrtcLibRtpReceiver,
        /// <summary>
        /// The C++ ORTC rtp sender object.
        /// </summary>
        OrtcLibRtpSender,
        /// <summary>
        /// The C++ ORTC rtp type structure tracing (parsers, helpers and
        /// other).
        /// </summary>
        OrtcLibRtpTypes,
        /// <summary>
        /// The C++ SCTP and data channel related objects.
        /// </summary>
        OrtcLibSctpDataChannel,
        /// <summary>
        /// The C++ SRTP related objects.
        /// </summary>
        OrtcLibSrtp,
        /// <summary>
        /// The C++ SRTP related objects.
        /// </summary>
        OrtcLibStats,
        /// <summary>
        /// The C++ ORTC Adapter library.
        /// </summary>
        OrtcLibAdapter,
      };
    }

    /// <summary>
    /// An API used to control the logging engine within the ORTC library.
    /// </summary>
    public ref class Logger sealed
    {
    public:
      /// <summary>
      /// Set the default log level for all componenets.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetDefaultLogLevel")]
      static void SetLogLevel(Log::Level level);
      /// <summary>
      /// Set the log level for a specific component.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      static void SetLogLevel(Log::Component component, Log::Level level);
      /// <summary>
      /// Sets the log level for a component by its component string name.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetLogLevelByName")]
      static void SetLogLevel(Platform::String^ component, Log::Level level);

      /// <summary>
      /// Output log information to the standard out and optionally colorize
      /// the output using ANSI color codes.
      /// </summary>
      static void InstallStdOutLogger(Platform::Boolean colorizeOutput);
      /// <summary>
      /// Output the log information to a file and optional colorize the
      /// file output using ANSI color codes.
      /// </summary>
      static void InstallFileLogger(Platform::String^ fileName, Platform::Boolean colorizeOutput);
      /// <summary>
      /// Listen for an incoming telnet connections and output the logging
      /// information to the incoming telnet connection.
      /// </summary>
      static void InstallTelnetLogger(
        uint16 listenPort,
        uint32 maxSecondsWaitForSocketToBeAvailable,
        Platform::Boolean colorizeOutput
        );
      /// <summary>
      /// Create an outgoing telnet connection to connect to a telnet server
      /// and output the logging information to the telnet server.
      /// </summary>
      static void InstallOutgoingTelnetLogger(
        Platform::String^ serverHostWithPort,
        Platform::Boolean colorizeOutput,
        Platform::String^ sendStringUponConnection
        );
      /// <summary>
      /// Log to the connected debugger logger instance.
      /// </summary>
      static void InstallDebuggerLogger();

      /// <summary>
      /// Returns true if the telnet logger is listening.
      /// </summary>
      static Platform::Boolean IsTelnetLoggerListening();
      /// <summary>
      /// Returns true if a telnet logger client is connected.
      /// </summary>
      static Platform::Boolean IsTelnetLoggerConnected();
      /// <summary>
      /// Returns true if the telnet logger has an outgoing client connected
      /// telnet logger session.
      /// </summary>
      static Platform::Boolean IsOutgoingTelnetLoggerConnected();

      /// <summary>
      /// Uninstall the logger outputing to standard out.
      /// </summary>
      static void UninstallStdOutLogger();
      /// <summary>
      /// Uninstall the logger outputing to a file.
      /// </summary>
      static void UninstallFileLogger();
      /// <summary>
      /// Uninstall the telnet listening logger.
      /// </summary>
      static void UninstallTelnetLogger();
      /// <summary>
      /// Uninstall the outgoing client telnet logger.
      /// </summary>
      static void UninstallOutgoingTelnetLogger();
      /// <summary>
      /// Uninstall the logger outputting to a connected debugger.
      /// </summary>
      static void UninstallDebuggerLogger();

      /// <summary>
      /// Set the default eventing level for all componenets.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetDefaultEventingLevel")]
      static void SetEventingLevel(Log::Level level);
      /// <summary>
      /// Set the eventing level for a specific component.
      /// </summary>
      [Windows::Foundation::Metadata::DefaultOverloadAttribute]
      static void SetEventingLevel(Log::Component component, Log::Level level);
      /// <summary>
      /// Sets the log eventing for a component by its component string name.
      /// </summary>
      [Windows::Foundation::Metadata::OverloadAttribute("SetEventingLevelByName")]
      static void SetEventingLevel(Platform::String^ component, Log::Level level);

      /// <summary>
      /// Listen for an incoming eventing connection and output the eventing
      /// information to the incoming eventing connection. Both the eventing
      /// listener and connecting to an eventing server cannot be enabled
      /// at the same time.
      /// </summary>
      static void InstallEventingListener(
        Platform::String^ sharedSecret,
        uint16 listenPort,
        uint32 maxSecondsWaitForSocketToBeAvailable
      );
      /// <summary>
      /// Create an outgoing eventing connection to an eventing server and
      /// output the eventing information to the server. Both the eventing
      /// listener and connecting to an eventing server cannot be enabled
      /// at the same time.
      /// </summary>
      static void ConnectToEventingServer(
        Platform::String^ sharedSecret,
        Platform::String^ serverHostWithPort
      );

      /// <summary>
      /// Uninstall the eventing listener.
      /// </summary>
      static void UninstallEventingListener();
      /// <summary>
      /// Uninstall the outgoing client telnet logger.
      /// </summary>
      static void DisconnectEventingServer();
    };
  }
}
