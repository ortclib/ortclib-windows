
#pragma once

namespace org
{
  namespace ortc
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
