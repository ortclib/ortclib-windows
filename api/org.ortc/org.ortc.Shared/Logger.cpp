#include "pch.h"
#include "Logger.h"
#include <ortc/types.h>
#include <openpeer/services/ILogger.h>
#include "helpers.h"


using namespace ortc;
using namespace org::ortc;
using UseServicesLogger = openpeer::services::ILogger;

void Logger::SetLogLevel(Log::Level level)
{
  UseServicesLogger::setLogLevel(internal::ConvertEnums::convert(level));
}

void Logger::SetLogLevel(Log::Component component, Log::Level level)
{
  UseServicesLogger::setLogLevel(internal::ConvertEnums::toComponent(component), internal::ConvertEnums::convert(level));
}

void Logger::SetLogLevel(Platform::String^ component, Log::Level level)
{
  UseServicesLogger::setLogLevel(FromCx(component).c_str(), internal::ConvertEnums::convert(level));
}

void Logger::InstallStdOutLogger(Platform::Boolean colorizeOutput)
{
  UseServicesLogger::installStdOutLogger(colorizeOutput);
}

void Logger::InstallFileLogger(Platform::String ^ fileName, Platform::Boolean colorizeOutput)
{
  UseServicesLogger::installFileLogger(FromCx(fileName).c_str(), colorizeOutput);
}

void Logger::InstallTelnetLogger(uint16 listenPort, uint32 maxSecondsWaitForSocketToBeAvailable, Platform::Boolean colorizeOutput)
{
  UseServicesLogger::installTelnetLogger(listenPort, maxSecondsWaitForSocketToBeAvailable, colorizeOutput);
}

void Logger::InstallOutgoingTelnetLogger(Platform::String ^ serverHostWithPort, Platform::Boolean colorizeOutput, Platform::String ^ sendStringUponConnection)
{
  UseServicesLogger::installOutgoingTelnetLogger(FromCx(serverHostWithPort).c_str(), colorizeOutput, FromCx(sendStringUponConnection).c_str());
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

