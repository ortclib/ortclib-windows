
#pragma once

namespace Org
{
  namespace Ortc
  {
    using Windows::UI::Core::CoreDispatcher;

    public ref class Ortc sealed
    {
    public:
      /// <summary>
      /// Initialize the ORTC stack.
      /// </summary>
      static void Setup();

      /// <summary>
      /// Gets or sets the NTP server time discovered in milliseconds since
      /// NTP epoch. This property must only be set at the actual moment when
      /// the server time was discovered to ensure the NTP clock is as closed
      /// to accurate as possible.
      /// </summary>
      static property int64 NtpServerTime
      {
        int64 get();
        void set(int64 value);
      };

      /// <summary>
      /// Starts media engine trancing system. The collected traces can be
      /// saved to file or sent to TCP listener. Traces can be visualised
      /// further by importing the data into Chrome browser tool that can be
      /// accessed using following option - chrome://tracing.
      /// This method starts the tracing engine.
      /// </summary>
      static void StartMediaTracing();
      /// <summary>
      /// Stops media tracing process.
      /// </summary>
      static void StopMediaTracing();
      /// <summary>
      /// Indiceates whether tracing system is activated.
      /// </summary>
      static bool IsMediaTracing();
      /// <summary>
      /// Saves media trace data to the file specified by filename.
      /// </summary>
      static bool SaveMediaTrace(Platform::String ^filename);
      /// <summary>
      /// Sends all traced data to TCP listener specified by host name and port.
      /// </summary>
      static bool SaveMediaTrace(Platform::String ^host, int port);
      /// <summary>
      /// Checks whether Media Resource Pack is installed on the system.
      /// </summary>
      static bool IsMRPInstalled();
    };

    [Windows::Foundation::Metadata::WebHostHiddenAttribute]
    public ref class OrtcWithDispatcher sealed
    {
    private:
      static CoreDispatcher ^_dispatcher;

    public:
      static property CoreDispatcher ^Dispatcher
      {
        CoreDispatcher ^get() { return _dispatcher; }
      }

      /// <summary>
      /// Initialize the ORTC stack and fire events through the specified core
      /// dispatcher.
      /// </summary>
      static void Setup(CoreDispatcher ^dispatcher);
    };

  }
}
