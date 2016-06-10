#include "pch.h"

#include "Logger.h"
#include "helpers.h"

#include <ortc/IORTC.h>

#include <openpeer/services/ILogger.h>

using namespace ortc;
using UseServicesLogger = openpeer::services::ILogger;

namespace org
{
  namespace ortc
  {
    ZS_DECLARE_TYPEDEF_PTR(internal::Helper, UseHelper)

    void Logger::SetLogLevel(Log::Level level)
    {
      IORTC::setDefaultLogLevel(UseHelper::Convert(level));
    }

    void Logger::SetLogLevel(Log::Component component, Log::Level level)
    {
      IORTC::setLogLevel(UseHelper::ToComponent(component), UseHelper::Convert(level));
    }

    void Logger::SetLogLevel(Platform::String^ component, Log::Level level)
    {
      IORTC::setLogLevel(UseHelper::FromCx(component).c_str(), UseHelper::Convert(level));
    }

    void Logger::InstallStdOutLogger(Platform::Boolean colorizeOutput)
    {
      UseServicesLogger::installStdOutLogger(colorizeOutput);
    }

    void Logger::InstallFileLogger(Platform::String ^ fileName, Platform::Boolean colorizeOutput)
    {
      UseServicesLogger::installFileLogger(UseHelper::FromCx(fileName).c_str(), colorizeOutput);
    }

    void Logger::InstallTelnetLogger(uint16 listenPort, uint32 maxSecondsWaitForSocketToBeAvailable, Platform::Boolean colorizeOutput)
    {
      UseServicesLogger::installTelnetLogger(listenPort, maxSecondsWaitForSocketToBeAvailable, colorizeOutput);
    }

    void Logger::InstallOutgoingTelnetLogger(Platform::String ^ serverHostWithPort, Platform::Boolean colorizeOutput, Platform::String ^ sendStringUponConnection)
    {
      UseServicesLogger::installOutgoingTelnetLogger(UseHelper::FromCx(serverHostWithPort).c_str(), colorizeOutput, UseHelper::FromCx(sendStringUponConnection).c_str());
    }

    void Logger::InstallDebuggerLogger()
    {
      UseServicesLogger::installDebuggerLogger();
    }

    Platform::Boolean Logger::IsTelnetLoggerListening()
    {
      return UseServicesLogger::isTelnetLoggerListening();
    }

    Platform::Boolean Logger::IsTelnetLoggerConnected()
    {
      return UseServicesLogger::isTelnetLoggerConnected();
    }

    Platform::Boolean Logger::IsOutgoingTelnetLoggerConnected()
    {
      return UseServicesLogger::isOutgoingTelnetLoggerConnected();
    }

    void Logger::UninstallStdOutLogger()
    {
      UseServicesLogger::uninstallStdOutLogger();
    }

    void Logger::UninstallFileLogger()
    {
      UseServicesLogger::uninstallFileLogger();
    }

    void Logger::UninstallTelnetLogger()
    {
      UseServicesLogger::uninstallTelnetLogger();
    }

    void Logger::UninstallOutgoingTelnetLogger()
    {
      UseServicesLogger::uninstallOutgoingTelnetLogger();
    }

    void Logger::UninstallDebuggerLogger()
    {
      UseServicesLogger::uninstallDebuggerLogger();
    }

  } // namespace ortc
  
} // namespace org

