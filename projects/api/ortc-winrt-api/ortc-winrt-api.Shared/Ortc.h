
#pragma once

namespace ortc_winrt_api
{
  using Windows::UI::Core::CoreDispatcher;

	public ref class Ortc sealed
	{
	public:
		static void Setup();
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
    
		static void Setup(CoreDispatcher ^dispatcher);
	};

}
