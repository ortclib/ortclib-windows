#pragma once

namespace org
{
  namespace ortc
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
        /// The TURN services support library for the ORTC library.
        /// </summary>
        ServicesTurn,
        /// <summary>
        /// The HTTP services support library for the ORTC library.
        /// </summary>
        ServicesHttp,
        /// <summary>
        /// The C++ ORTC library.
        /// </summary>
        OrtcLib
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
      /// Listen for an incoming telnet connections and out the logging
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
    };
  }
}
