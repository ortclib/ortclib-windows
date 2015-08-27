//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace ortc_standup;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

Windows::UI::Core::CoreDispatcher^ g_windowDispatcher;

MainPage::MainPage() : mediaEngine_(NULL)
{
	InitializeComponent();
}

ortc_standup::MainPage::~MainPage()
{
  if (mediaEngine_)
    delete mediaEngine_;
}

void ortc_standup::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
  g_windowDispatcher = Window::Current->Dispatcher;
  mediaEngine_ = new MediaEngine();
  mediaEngine_->SetStartStopButton(StartStopButton);
  mediaEngine_->SetLocalMediaElement(RemoteVideoMediaElement);
}

void ortc_standup::MainPage::StartStopButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
  mediaEngine_->StartStopMedia();
}
