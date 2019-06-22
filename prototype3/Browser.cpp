#include "pch.h"
#include "Browser.h"
#include "string.h"

Browser::Browser() {
	i = 0;
	history[i] = "https://google.com";
	directLoading = true;
}
Browser::~Browser()
{
}
bool Browser::hasPreviousUrl() {
	return i == 0 ? false : true;
}
Platform::String^ Browser::getPreviousUrl() {
	i--;
	return history[i];
}
void Browser::setHistory(Platform::String^ urlToSave) {
	i++;
	history[i] = urlToSave;
}
void Browser::loadUrlDirect(Windows::UI::Xaml::Controls::WebView^ webView, Windows::Foundation::Uri^ urlToLoad) {
	setHistory(urlToLoad->ToString());
	directLoading = true;
	try {
		webView->Navigate(urlToLoad);
	}
	catch (const std::exception& ex) {
		auto dialog = ref new Windows::UI::Popups::MessageDialog("Problem occured while the page was loading");
		dialog->ShowAsync();
	}
}
void Browser::loadPrevUrl(Windows::UI::Xaml::Controls::WebView^ webView, Windows::Foundation::Uri^ prevUrl) {
	directLoading = true;
	try {
		webView->Navigate(prevUrl);
	}
	catch (const std::exception& ex) {
		auto dialog = ref new Windows::UI::Popups::MessageDialog("Problem occured while the page was loading");
		dialog->ShowAsync();
	}
}
void Browser::loadUrlSearch(Windows::UI::Xaml::Controls::WebView^ webView, Platform::String^ toSearch) {
	setHistory(toSearch);
	directLoading = true;
	auto searchUrl = ref new Windows::Foundation::Uri("https://google.com/search?q=" + toSearch);
	try {
		webView->Navigate(searchUrl);
	}
	catch (const std::exception& ex) {
		auto dialog = ref new Windows::UI::Popups::MessageDialog("Problem occured while the page was loading");
		dialog->ShowAsync();
	}
}
int Browser::isValidUrl(Platform::String^ urlString) {
	std::wstring fooW(urlString->Begin());
	std::string fooA(fooW.begin(), fooW.end());
	std::string d = "fg";
	const char* charStr = fooA.c_str();
	char* toCheck = (char*)d.c_str();
	bool hasProtocol = false;
	int i = 0;
	if (charStr[i + 0] == 'h' && charStr[i + 1] == 't' && charStr[i + 2] == 't' && charStr[i + 3] == 'p' && charStr[i + 4] == 's' && charStr[i + 5] == ':' && charStr[i + 6] == '/' && charStr[i + 7] == '/') {
		i = 8;
		hasProtocol = true;
	}
	else if (charStr[i + 0] == 'h' && charStr[i + 1] == 't' && charStr[i + 2] == 't' && charStr[i + 3] == 'p' && charStr[i + 4] == ':' && charStr[i + 5] == '/' && charStr[i + 6] == '/') {
		i = 7;
		hasProtocol = true;
	}
	int j = 0;
	while (charStr[i] != '/' && i < strlen(charStr)) {
		toCheck[j] = charStr[i];
		i++;
		j++;
	}
	i = 1;
	while (i < j) {
		if (toCheck[i] == '.') {
			return hasProtocol ? 1 : 55;
		}
		i++;
	}
	return 0;
}