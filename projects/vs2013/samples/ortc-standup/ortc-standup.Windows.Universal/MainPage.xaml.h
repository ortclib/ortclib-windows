//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

#include "MediaEngine.h"

namespace ortc_standup
{
  ZS_DECLARE_CLASS_PTR(MediaEngine)

  /// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
    virtual ~MainPage();

  private:
    void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
    void StartStopButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

    MediaEnginePtr mMediaEngine;
  };
}
