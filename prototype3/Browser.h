#pragma once
class Browser
{
private:
	int i;
	Platform::String^ history[100];
public:
	Platform::String^ currentAddress;

	bool isLoading;
	bool directLoading;
	bool navigationHasFailed;
	bool isReloading;
	bool homeScreenOpen;

	int saveFile(Platform::String^);
	Browser();//done
	bool hasPreviousUrl(); //done
	Platform::String^ getPreviousUrl(); //done
	void setHistory(Platform::String^ urlToSave); //done
	void loadUrlDirect(Windows::UI::Xaml::Controls::WebView^ webView, Windows::Foundation::Uri^ uriToLoad); //done
	void loadPrevUrl(Windows::UI::Xaml::Controls::WebView^ webView, Windows::Foundation::Uri^ prevUrl); //done
	int isValidUrl(Platform::String^ urlString); //done
	void loadUrlSearch(Windows::UI::Xaml::Controls::WebView^ webView, Platform::String^ toSearch);//done

	~Browser();
};

