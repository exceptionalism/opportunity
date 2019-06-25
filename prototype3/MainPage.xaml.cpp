//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Browser.h"

using namespace prototype3;

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

//Browser b(outputBox, urlContainer, prevButton, reloadButton, loaderRing);
Browser b;
// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409
MainPage::MainPage()
{
	InitializeComponent();
}


void prototype3::MainPage::PrevButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (b.hasPreviousUrl()) {
		auto newUrl = ref new  Windows::Foundation::Uri(b.getPreviousUrl());
		//outputBox->Navigate(newUrl);
		b.loadPrevUrl(outputBox, newUrl);
	}
}


void prototype3::MainPage::ReloadButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	outputBox->Refresh();
}


void prototype3::MainPage::OutputBox_NavigationStarting(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args)
{
	urlContainer->Text = args->Uri->ToString();
	outputBox->Focus(Windows::UI::Xaml::FocusState::Keyboard);
	loaderRing->IsActive = true;
	loaderRing->Visibility = Windows::UI::Xaml::Visibility::Visible;
	if (!b.directLoading) {
		b.setHistory(args->Uri->ToString());
	}
}

void prototype3::MainPage::OutputBox_ContentLoading(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewContentLoadingEventArgs^ args)
{
	urlContainer->Text = args->Uri->ToString();
}

void prototype3::MainPage::OutputBox_NavigationCompleted(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ args)
{
	loaderRing->IsActive = false;
	loaderRing->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	/*auto dialog = ref new Windows::UI::Popups::MessageDialog(outputBox->DocumentTitle);
	dialog->ShowAsync();*/
	b.directLoading = false;
}


void prototype3::MainPage::OutputBox_NavigationFailed(Platform::Object^ sender, Windows::UI::Xaml::Controls::WebViewNavigationFailedEventArgs^ e)
{
	if (e->Uri) {
		auto dialog = ref new Windows::UI::Popups::MessageDialog("Page cannot be loaded.");
		dialog->ShowAsync();
	}
	b.directLoading = false;
}


void prototype3::MainPage::UrlContainer_KeyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e)
{
	if (e->Key == Windows::System::VirtualKey::Enter) {
		int i = 1;
 		i = b.isValidUrl(urlContainer->Text);
		if (i == 1) {
			auto urlToLoad = ref new Windows::Foundation::Uri(urlContainer->Text);
			b.loadUrlDirect(outputBox, urlToLoad);
		}
		else if (i == 0) {
			b.loadUrlSearch(outputBox, urlContainer->Text);
		}
		else if (i == -1) {
			auto urlToSearch = ref new Windows::Foundation::Uri("http://" + urlContainer->Text);
			b.loadUrlDirect(outputBox, urlToSearch);
		}
	}
}


void prototype3::MainPage::OutputBox_ContainsFullScreenElementChanged(Windows::UI::Xaml::Controls::WebView^ sender, Platform::Object^ args)
{
}
