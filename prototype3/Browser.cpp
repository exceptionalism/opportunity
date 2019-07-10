#include "pch.h"
#include "Browser.h"
#include "string.h"

using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::Foundation::Collections;

Browser::Browser() {
	i = 0;
	history[i] = "https://google.com";
	currentAddress = history[0];
	directLoading = true;
	navigationHasFailed = false;
	isReloading = false;
}
Browser::~Browser()
{
}
int Browser::saveFile(Platform::String^ toSave) {
	/*auto createFileTask = create_task(DownloadsFolder::CreateFileAsync(L"file.txt", CreationCollisionOption::OpenIfExists));
	createFileTask.then([&](StorageFile^ newFile)
		{
			create_task(FileIO::WriteTextAsync(newFile, toSave));
		});*/


	//StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	//concurrency::create_task(storageFolder->CreateFileAsync("sample.txt", CreationCollisionOption::ReplaceExisting));
	//create_task(storageFolder->GetFileAsync("sample.txt")).then([&](StorageFile^ sampleFile) {
		//int j = 0;
		// Process file
		/*while (j < i) {
			create_task(FileIO::AppendTextAsync(sampleFile, history[j]));
			j++;
		}*/
		//create_task(FileIO::WriteTextAsync(sampleFile, toSave));
	//});
	return 0;
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

int Browser::handleLocalErrroFiles() {
	StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
	bool folExists = false;
	create_task(storageFolder->GetFoldersAsync()).then([&](IVectorView<StorageFolder^>^ itemsInFolder) {
		for (auto it = itemsInFolder->First(); it->HasCurrent; it->MoveNext())
		{
			if (it->Current->Name == "html") {
				folExists = true;
				break;
			}
		}
		if (!folExists) {
			create_task(storageFolder->CreateFolderAsync("html"));
		}
	});
	create_task(storageFolder->GetFolderAsync("html")).then([=](StorageFolder^ folder1) {

		create_task(folder1->GetFilesAsync()).then([=](IVectorView<StorageFile^>^ filesInFolder) {
			//first
			bool exists = false;
			auto iter = filesInFolder->First();
			while (iter->HasCurrent) {
				if (iter->Current->Name == "404.html") {
					exists = true;
					break;
				}
				iter->MoveNext();
			}
			if (!exists) {
				create_task(folder1->CreateFileAsync("404.html", CreationCollisionOption::FailIfExists));
				create_task(folder1->GetFileAsync("404.html")).then([&](StorageFile^ sampleFile) {
					create_task(FileIO::WriteTextAsync(sampleFile, "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF - 8\"> <meta name=\"viewport\" content=\"width = device - width, initial - scale = 1.0\"> <meta http-equiv=\"X - UA - Compatible\" content=\"ie = edge\"> <title>Orient College : A Levels</title> <link href=\"https://fonts.googleapis.com/css?family=Quicksand\" rel=\"stylesheet\"> <link rel=\"stylesheet\" href=\"./styles.css\"></head><body> <div class=\"container quicksand\"> <h1>Oops!</h1> <p>The address you entered could not be accessed.</p><p>It's possible that the content in the specified location was moved to another location, or you might have entered incorrectly the address. Please, make sure the address you're entering exists.</p></div></body></html>"));
				});
			}
			//second
			exists = false;
			iter = filesInFolder->First();
			Platform::String^ s;
			while (iter->HasCurrent) {
				if (iter->Current->Name == "styles.css") {
					exists = true;
					break;
				}
				iter->MoveNext();
			}
			if (!exists) {
				create_task(folder1->CreateFileAsync("styles.css", CreationCollisionOption::FailIfExists));
				create_task(folder1->GetFileAsync("styles.css")).then([&](StorageFile^ sampleFile) {
					create_task(FileIO::WriteTextAsync(sampleFile, "body{margin:0;padding:0;background:#f3f3f3}.container{width:80%;height:calc(100vh - 60px);margin:0 auto;padding:0 20px;display:flex;flex-direction:column;justify-content:center;align-items:center}.quicksand{font-family:Quicksand,sans-serif}h1{font-size:3rem;margin-bottom:0}p{line-height:1.5;margin:0;margin-top:10px;text-align:center}"));
				});
			}
			//third
			exists = false;
			iter = filesInFolder->First();
			while (iter->HasCurrent) {
				if (iter->Current->Name == "cannotConnect.html") {
					exists = true;
					break;
				}
				iter->MoveNext();
			}
			if (!exists) {
				create_task(folder1->CreateFileAsync("cannotConnect.html", CreationCollisionOption::FailIfExists));
				create_task(folder1->GetFileAsync("cannotConnect.html")).then([&](StorageFile^ sampleFile) {
					create_task(FileIO::WriteTextAsync(sampleFile, "<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF - 8\"> <meta name=\"viewport\" content=\"width = device - width, initial - scale = 1.0\"> <meta http-equiv=\"X - UA - Compatible\" content=\"ie = edge\"> <title>Orient College : A Levels</title> <link href=\"https://fonts.googleapis.com/css?family=Quicksand\" rel=\"stylesheet\"> <link rel=\"stylesheet\" href=\"./styles.css\"></head><body> <div class=\"container quicksand\"> <h1>Oops!</h1> <p>It looks like you don't have internet connection.</p><p>Please, make sure you're connected to the internet and try again.</p></div></body></html>"));
				});
			}
		});
	});
	return 0;
}