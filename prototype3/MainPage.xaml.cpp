﻿//
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
using namespace Windows::Storage;
using namespace concurrency;
using namespace Windows::Web;

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
		b.loadPrevUrl(outputBox, newUrl);
	}
}


void prototype3::MainPage::ReloadButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!b.isLoading) {
		auto currUrl = ref new Windows::Foundation::Uri(b.currentAddress);
		b.isReloading = true;
		outputBox->Navigate(currUrl);
	}
	else if (b.isLoading) {
		outputBox->Stop();
		loaderRing->IsActive = false;
		loaderRing->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		b.isLoading = false;
		reloadButton->Content = "R";
	}
}


void prototype3::MainPage::OutputBox_NavigationStarting(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationStartingEventArgs^ args) {

	b.isLoading = true;
	if (args != nullptr && args->Uri != nullptr && !b.navigationHasFailed) {
		urlContainer->Text = args->Uri->ToString();
	}
	outputBox->Focus(Windows::UI::Xaml::FocusState::Keyboard);
	loaderRing->IsActive = true;
	loaderRing->Visibility = Windows::UI::Xaml::Visibility::Visible;
	reloadButton->Content = "X";
}

void prototype3::MainPage::OutputBox_ContentLoading(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewContentLoadingEventArgs^ args)
{
	if (args != nullptr && args->Uri != nullptr && !b.navigationHasFailed) {
		urlContainer->Text = args->Uri->ToString();
	}
	if (!b.directLoading) {
		if (args != nullptr && args->Uri != nullptr && !b.navigationHasFailed && !b.isReloading) {
			b.setHistory(args->Uri->ToString());
		}
	}
}

void prototype3::MainPage::OutputBox_NavigationCompleted(Windows::UI::Xaml::Controls::WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ args)
{
	loaderRing->IsActive = false;
	loaderRing->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	b.directLoading = false;
	b.navigationHasFailed = false;
	b.isReloading = false;
	b.isLoading = false;
	reloadButton->Content = "R";
}


void prototype3::MainPage::OutputBox_NavigationFailed(Platform::Object^ sender, Windows::UI::Xaml::Controls::WebViewNavigationFailedEventArgs^ e)
{
	b.isLoading = false;
	reloadButton->Content = "R";
	b.navigationHasFailed = true;
	loaderRing->IsActive = false;
	loaderRing->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	if (e->Uri) {
		if (e->WebErrorStatus == WebErrorStatus::CannotConnect || e->WebErrorStatus == WebErrorStatus::NotFound || e->WebErrorStatus == WebErrorStatus::UnexpectedStatusCode || e->WebErrorStatus == WebErrorStatus::ConnectionAborted || e->WebErrorStatus == WebErrorStatus::ConnectionReset || e->WebErrorStatus == WebErrorStatus::Disconnected || e->WebErrorStatus == WebErrorStatus::Forbidden) {
			auto error404 = ref new Windows::Foundation::Uri("ms-appx-web:///Assets/404.html");
			outputBox->Navigate(error404);
		}
		else {
			auto d = ref new Windows::UI::Popups::MessageDialog(e->WebErrorStatus.ToString());
			d->ShowAsync();
		}
	}
	b.directLoading = false;
	b.isReloading = false;
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
			auto urlToGo = ref new Windows::Foundation::Uri("http://" + urlContainer->Text);
			b.loadUrlDirect(outputBox, urlToGo);
		}
	}
}