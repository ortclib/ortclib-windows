#include "pch.h"
#include "Logger.h"
#include <ortc/types.h>
#include <openpeer/services/ILogger.h>
#include "helpers.h"


using namespace ortc;
using namespace ortc_winrt_api;
using UseServicesLogger = openpeer::services::ILogger;

namespace internal
{

  static zsLib::Log::Level convert(Log::Level level)
  {
    switch (level) {
    case Log::Level::Basic:   return zsLib::Log::Basic;
    case Log::Level::Detail:  return zsLib::Log::Detail;
    case Log::Level::Debug:   return zsLib::Log::Debug;
    case Log::Level::Trace:   return zsLib::Log::Trace;
    case Log::Level::Insane:  return zsLib::Log::Insane;
    }
    throw ref new Platform::NotImplementedException();
  }

  static const char *toComponent(Log::Component  component)
  {
    switch (component) {
    case Log::Component::ZsLib:         return "zsLib";
    case Log::Component::ZsLibSocket:   return "zsLib_socket";
    case Log::Component::Services:      return "openpeer_services";
    case Log::Component::ServicesTurn:  return "openpeer_services_turn";
    case Log::Component::ServicesHttp:  return "openpeer_services_http";
    case Log::Component::OrtcLib:       return "ortclib";
    }
    throw ref new Platform::NotImplementedException();
  }
}

void Logger::SetLogLevel(Log::Level level)
{
  UseServicesLogger::setLogLevel(internal::convert(level));
}

void Logger::SetLogLevel(Log::Component component, Log::Level level)
{
  UseServicesLogger::setLogLevel(internal::toComponent(component), internal::convert(level));
}

void Logger::SetLogLevel(Platform::String^ component, Log::Level level)
{
  UseServicesLogger::setLogLevel(FromCx(component).c_str(), internal::convert(level));
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

