#pragma once

namespace ortc_winrt_api
{
  namespace Log
  {
    public enum class Level
    {
      None,
      Basic,
      Detail,
      Debug,
      Trace,
      Insane
    };

    public enum class Component
    {
      ZsLib,
      ZsLibSocket,
      Services,
      ServicesTurn,
      ServicesHttp,
      OrtcLib
    };
  }

	public ref class Logger sealed
	{
	public:
    static void SetLogLevel(Log::Level level);
    [Windows::Foundation::Metadata::DefaultOverloadAttribute]
    static void SetLogLevel(Log::Component component, Log::Level level);
    static void SetLogLevel(Platform::String^ component, Log::Level level);

    static void InstallStdOutLogger(Platform::Boolean colorizeOutput);
    static void InstallFileLogger(Platform::String^ fileName, Platform::Boolean colorizeOutput);
    static void InstallTelnetLogger(
      uint16 listenPort,
      uint32 maxSecondsWaitForSocketToBeAvailable,
      Platform::Boolean colorizeOutput
      );
    static void InstallOutgoingTelnetLogger(
      Platform::String^ serverHostWithPort,
      Platform::Boolean colorizeOutput,
      Platform::String^ sendStringUponConnection
      );
    static void InstallDebuggerLogger();

    static Platform::Boolean IsTelnetLoggerListening();
    static Platform::Boolean IsTelnetLoggerConnected();
    static Platform::Boolean IsOutgoingTelnetLoggerConnected();

    static void UninstallStdOutLogger();
    static void UninstallFileLogger();
    static void UninstallTelnetLogger();
    static void UninstallOutgoingTelnetLogger();
    static void UninstallDebuggerLogger();
  };

}
