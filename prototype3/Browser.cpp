#include "pch.h"
#include "Browser.h"
#include "string.h"

using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Foundation::Collections;

static Platform::String^ saveData;

Browser::Browser() {
	i = -1;
	currentAddress = "";
	directLoading = true;
	navigationHasFailed = false;
	isReloading = false;
	homeScreenOpen = true;
	histCount = 0;
}
Browser::~Browser() {
	delete this;
}
int Browser::saveFile(Platform::String^ toSave) {
	saveData = toSave;
	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	create_task(storageFolder->CreateFileAsync("history.txt", CreationCollisionOption::OpenIfExists)).then([&](StorageFile^ sampleFile) {
		create_task(FileIO::AppendTextAsync(sampleFile, saveData + "\n"));
	});
	return 0;
}
bool Browser::hasPreviousUrl() {
	return i < 0 ? false : true;
}
Platform::String^ Browser::getPreviousUrl() {
	if (i >= 1) {
		i--;
		return history[i];
	}
	history[0] = "";
	i = -1;
	return "n";
}
void Browser::setHistory(Platform::String^ urlToSave) {
	i++;
	history[i] = urlToSave;
	saveFile(urlToSave);
}
void Browser::loadUrlDirect(Windows::UI::Xaml::Controls::WebView^ webView, Windows::Foundation::Uri^ urlToLoad) {
	setHistory(urlToLoad->ToString());
	currentAddress = urlToLoad->ToString();
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
	currentAddress = prevUrl->ToString();
	try {
		webView->Navigate(prevUrl);
	}
	catch (const std::exception& ex) {
		auto dialog = ref new Windows::UI::Popups::MessageDialog("Problem occured while the page was loading");
		dialog->ShowAsync();
	}
}
void Browser::loadUrlSearch(Windows::UI::Xaml::Controls::WebView^ webView, Platform::String^ toSearch) {
	directLoading = true;
	auto searchUrl = ref new Windows::Foundation::Uri("https://google.com/search?q=" + toSearch);
	setHistory(searchUrl->ToString());
	currentAddress = searchUrl->ToString();
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
	std::string d = "";

	std::string::iterator it1 = fooA.begin();
	
	bool hasProtocol = false;
	int j = 0;
	
	// check if protocol is present
	if (*it1 == 'h' && *(it1 + 1) == 't' && *(it1 + 2) == 't' && *(it1 + 3) == 'p' && *(it1 + 4) == 's' && *(it1 + 5) == ':' && *(it1 + 6) == '/' && *(it1 + 7) == '/') {
		it1 += 8;
		hasProtocol = true;
	} else if (*it1 == 'h' && *(it1 + 1) == 't' && *(it1 + 2) == 't' && *(it1 + 3) == 'p' && *(it1 + 4) == ':' && *(it1 + 5) == '/' && *(it1 + 6) == '/') {
		it1 += 7;
		hasProtocol = true;
	}
	std::string::iterator x = fooA.end();

	// create new string of only domain and extension
	while (it1 < x && *it1 != '/') {
		d.push_back(*it1);
		it1++;
	}

	// check if the new string is a valid url
	it1 = d.begin();
	while (it1 != d.end()) {
		if (*it1 == '.')
			return hasProtocol ? 1 : -1;
		it1++;
	}
	return 0;
}



Windows::UI::Xaml::Controls::TextBlock^ Browser::processHistory(Platform::String^ hDt, Windows::UI::Xaml::Controls::StackPanel^ stkPnl, Windows::UI::Xaml::Controls::WebView^ oB) {
	Windows::UI::Xaml::Controls::TextBlock^ hS[300];
	std::string links[100];
	int i = 0;
	std::wstring fooW(hDt->Begin());
	std::string strData(fooW.begin(), fooW.end());
	std::string::iterator it1 = strData.begin();
	while (it1 != strData.end()) {
		if (*it1 == '\n') {
			i++;
		}
		else {
			links[i].push_back(*it1);
		}
		it1++;
	};
	i--;
	histCount = i;
	int counter = 0;
	while (i > -1 && counter < 16) {
		std::wstring w_str = std::wstring(links[i].begin(), links[i].end());
		const wchar_t* w_chars = w_str.c_str();

		auto link = ref new Platform::String(w_chars);
		auto btnH = ref new Windows::UI::Xaml::Controls::HyperlinkButton;
		btnH->NavigateUri = ref new Windows::Foundation::Uri(link);
		btnH->Content = link;
		stkPnl->Children->Append(btnH);
		counter++;
		i--;
	}
	return hS[0];
}

void Browser::routeHistory(Platform::String^ s, Windows::UI::Xaml::Controls::WebView^ oBox) {
	auto uriHist = ref new Windows::Foundation::Uri(s);
	oBox->Navigate(uriHist);
}