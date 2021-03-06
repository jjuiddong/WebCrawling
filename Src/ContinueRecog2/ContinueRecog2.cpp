//
// chrome tab moving capture and text recognition
//
#include "pch.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <opencv2/opencv.hpp>
#include "../WebCrawl/webcrawl.h"

#ifdef _DEBUG
	#pragma comment(lib, "opencv_core340d.lib")
	#pragma comment(lib, "opencv_imgproc340d.lib")
	#pragma comment(lib, "opencv_imgcodecs340d.lib")
#else
	#pragma comment(lib, "opencv_core340.lib")
	#pragma comment(lib, "opencv_imgproc340.lib")
	#pragma comment(lib, "opencv_imgcodecs340.lib")
#endif


using namespace std;
using namespace cv;

webcrawl::cCapture g_capture;
webcrawl::cTesseract g_tess;

struct sCrawlData {
	string name;
	string data;
};

void DisplayConfig(webcrawl::cConfig &config);
HWND GetChromeWindowHandle();
void ChangeChromeUrl(const HWND hwnd, const string &url);
bool FocusChromeWindow(const HWND hwnd);
std::pair<string, string> RecogText(const HWND hwnd, const RECT &nameR, const RECT &stockR
	, const int offsetY);
std::pair<string, string> RecogTextByTemplateMatch(const HWND hwnd
	, const webcrawl::cConfig::sSite &site, const RECT &nameR, const RECT &stockR);
bool Filtering(INOUT string &name, INOUT string &stock);
bool WriteExcel(const char *fileName, const map<string, sCrawlData> &datas);


int main()
{
	CoInitialize(NULL);

	webcrawl::cConfig config;
	if (!config.Read("config.json"))
	{
		cout << "Error Read config.json file" << endl;
		cout << config.m_err << endl;
		return 0;
	}

	DisplayConfig(config);

	if (!g_tess.Create())
	{
		cout << "Error Read Tesseract Resource" << endl;
		return 0;
	}

	cout << "Configuration..." << endl;
	Sleep(100);
	Sleep(100);
	Sleep(3000);

	const RECT nameR = config.m_nameRect;
	const RECT stockR = config.m_stockRect;
	const int offsetY = config.m_offsetY;
	
	map<string, sCrawlData> crawlDatas;

	int cnt = 0;
	for (int i=0; i < (int)config.m_sites.size(); ++i)
	{
		auto &site = config.m_sites[i];
		HWND hwnd = GetChromeWindowHandle();

		// Goto URL
		cout << "Goto " << site.url << endl;
		string url = webcrawl::GetCurrentChromeTabURL(hwnd);
		while (url != site.url)
		{
			ChangeChromeUrl(hwnd, site.url);
			url = webcrawl::GetCurrentChromeTabURL(hwnd);
			if (url.empty())
			{
				hwnd = GetChromeWindowHandle();
				url = webcrawl::GetCurrentChromeTabURL(hwnd);
			}
		}
		Sleep(config.m_waitSiteLoadingSeconds * 1000);
		//

		//auto result = RecogText(hwnd, nameR, stockR, 0);
		//if (result.first.find(site.name) == string::npos)
		//{
		//	cout << "\t ** Offset Recognition " << endl;
		//	result = RecogText(hwnd, nameR, stockR, config.m_offsetY);
		//	
		//	// templatematching 으로 위치를 찾아서 인식한다.
		//	if (result.first.find(site.name) == string::npos)
		//	{
		//		cout << "\t *** TemplateMatch Recognition " << endl;
		//		result = RecogTextByTemplateMatch(hwnd, site, nameR, stockR);
		//	}
		//}

		// templatematching 으로 위치를 찾아서 인식한다.
		auto result = RecogTextByTemplateMatch(hwnd, site, nameR, stockR);

		++cnt;
		if (Filtering(result.first, result.second))
		{
			sCrawlData data;
			data.name = result.first;
			data.data = result.second;
			crawlDatas[result.first] = data;

			cout << "Recognition (" << cnt << "/" << config.m_sites.size() << ")" 
				<< " >>" << endl;
			cout << "\t URL = " << url << endl;
			cout << "\t Name = " << result.first << endl;
			cout << "\t Stock = " << result.second << endl;
			cout << endl;

			// 정보를 가져오는데 성공했으면, 회사이름 이미지 레이블을 파일로 저장한다.
			// 광고 배너 크기에 따라 위치가 동적으로 바뀔수 있기 때문에, 미리 레이블을
			// 저장해 놓고, 회사이름을 못찾았을 때, 레이블을 이용해서 위치를 찾기 위함이다.
			// 레이블 이름은 회사이름으로 정한다.
			string fileName = "label/";
			fileName += site.name + ".bmp";
			if (_access_s(fileName.c_str(), 0) != 0) // Is Exist Label File?
			{
				rename("name.bmp", fileName.c_str());
			}
		}
		else
		{
			cout << "Error!! Crawling Name = " << site.name << ", URL = " << site.url << endl;
		}

		//if ((config.m_sites.size() - 1) == i)
		//	i = -1; // infinity loop
	}

	const string fileName = webcrawl::GetCurrentDateTime() + ".txt";
	if (!WriteExcel(fileName.c_str(), crawlDatas))
	{
		cout << "Error Write Excel File" << endl;
	}

	CoUninitialize();
	return 0;
}


void DisplayConfig(webcrawl::cConfig &config)
{
	// display configuration 
	cout << "Configuration Information >>" << endl;
	cout << "\t* crawl-interval-seconds = " << config.m_crawlInteralSeconds << endl;
	cout << "\t* name-rect = " << config.m_nameRect.left << " " << config.m_nameRect.top << " "
		<< config.m_nameRect.right << " " << config.m_nameRect.bottom << endl;
	cout << "\t* stock-rect = " << config.m_stockRect.left << " " << config.m_stockRect.top << " "
		<< config.m_stockRect.right << " " << config.m_stockRect.bottom << endl;
	cout << "\t* offset-y = " << config.m_offsetY << endl;
	cout << "\t* wait-site-loading-seconds = " << config.m_waitSiteLoadingSeconds << endl;
	cout << endl << endl;
}


HWND GetChromeWindowHandle()
{
	HWND hwnd = NULL;
	while (!hwnd)
	{
		while (1)
		{
			HWND fhwnd = FindWindowEx(0, hwnd, L"Chrome_WidgetWin_1", NULL);
			if (!fhwnd)
				break;
			hwnd = fhwnd;
		}
		if (!hwnd)
		{
			ShellExecute(0, 0, L"www.google.com", 0, 0, SW_SHOW);
			Sleep(1000);
			Sleep(1000);
			Sleep(3000);
		}
	}

	return hwnd;
}


void ChangeChromeUrl(const HWND hwnd, const string &url)
{
	webcrawl::cKeyBot bot;
	const int delayTime = 100;

	if (!FocusChromeWindow(hwnd))
		return;

	// Already Focus Chrome Address EditBox
	// Copyt URL string to ClipBoard
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, url.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), url.c_str(), url.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
	//

	// Paste URL String to Chrome Address Input Window
	bot.KeyDown(VK_CONTROL);
	Sleep(delayTime);
	bot.KeyClick('V');
	Sleep(delayTime);
	bot.KeyUp(VK_CONTROL);
	Sleep(delayTime);

	// Goto URL Link
	bot.KeyClick(VK_RETURN);
}


// Focus window to send key event
bool FocusChromeWindow(const HWND hwnd)
{
	const int oldT = GetTickCount();
	while (GetForegroundWindow() != hwnd)
	{
		BringWindowToTop(hwnd);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);
		Sleep(300);

		// too much waste time in this code, return
		if (abs((int)GetTickCount() - oldT) > 10000)
			return false;
	}
	return true;
}


// return Text Recognition Result name, stock text
std::pair<string, string> RecogText(const HWND hwnd, const RECT &nameR, const RECT &stockR
	, const int offsetY)
{
	RECT rc;
	GetWindowRect(hwnd, &rc);

	const int iDpi = GetDpiForWindow(hwnd);
	const string namePath = "name.bmp";
	{
		const int x = MulDiv(rc.left, iDpi, 96) + nameR.left;
		const int y = MulDiv(rc.top, iDpi, 96) + nameR.top + offsetY;
		const int w = nameR.right - nameR.left;
		const int h = nameR.bottom - nameR.top;
		if (!g_capture.Capture(x, y, w, h, namePath.c_str()))
			return {};
	}

	const string stockPath = "stock.bmp";
	{
		const int x = MulDiv(rc.left, iDpi, 96) + stockR.left;
		const int y = MulDiv(rc.top, iDpi, 96) + stockR.top + offsetY;
		const int w = stockR.right - stockR.left;
		const int h = stockR.bottom - stockR.top;
		if (!g_capture.Capture(x, y, w, h, stockPath.c_str()))
			return {};
	}

	Mat imgName = cv::imread(namePath, IMREAD_COLOR);
	const string recogName = g_tess.GetText(imgName.data, imgName.cols, imgName.rows, 3, imgName.step);
	
	Mat imgStock = cv::imread(stockPath, IMREAD_COLOR);
	const string recogStock = g_tess.GetText(imgStock.data, imgStock.cols, imgStock.rows, 3, imgStock.step);

	return { recogName, recogStock };
}


// 회사명 Label로 위치를 찾은 후, 문자를 인식한다.
std::pair<string, string> RecogTextByTemplateMatch(const HWND hwnd
	, const webcrawl::cConfig::sSite &site
	, const RECT &nameR, const RECT &stockR)
{
	string labelFileName = "label/";
	labelFileName += site.name + ".bmp";
	if (_access_s(labelFileName.c_str(), 0) != 0) // Is Exist Label File?
		return {}; // not found label file

	RECT rc;
	GetWindowRect(hwnd, &rc);

	const int iDpi = GetDpiForWindow(hwnd);
	const string bgPath = "bg.bmp";
	{
		const int x = MulDiv(rc.left, iDpi, 96);
		const int y = MulDiv(rc.top, iDpi, 96);
		const int w = rc.right - rc.left;
		const int h = rc.bottom - rc.top;
		g_capture.Capture(x, y, w, h, bgPath.c_str());
	}

	Mat bg = cv::imread(bgPath.c_str());
	Mat label = cv::imread(labelFileName.c_str());
	cvtColor(label, label, CV_RGB2GRAY);
	cvtColor(bg, bg, CV_RGB2GRAY);

	// 위치를 찾는다.
	Mat result;
	matchTemplate(bg, label, result, CV_TM_SQDIFF);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
	matchLoc = minLoc;

	// debugging code
	//Mat img_display = bg.clone();
	//rectangle(img_display, matchLoc, Point(matchLoc.x + label.cols, matchLoc.y + label.rows)
	//	, Scalar(0, 0, 255), 2, 8, 0);

	if (maxVal < 0.9f)
		return {};

	const int offsetY = stockR.top - nameR.top;
	const int nh = nameR.bottom - nameR.top;
	const int sh = stockR.bottom - stockR.top;
	const RECT nR = { nameR.left, matchLoc.y, nameR.right, matchLoc.y + nh };
	RECT sR = { stockR.left, matchLoc.y, stockR.right, matchLoc.y + sh };
	OffsetRect(&sR, 0, offsetY);

	return RecogText(hwnd, nR, sR, 0);
}


// stock 문자열에서 (1.0%) 안의 1.0 만 가져온다.
bool Filtering(INOUT string &name, INOUT string &stock)
{
	string out;
	int i = 0;
	int state = 0;
	while (i < (int)stock.size())
	{
		if (0 == state)
		{
			if ('(' == stock[i])
				state = 1;
		}
		else if (1 == state)
		{
			if ((')' == stock[i]) || ('%' == stock[i]))
			{
				break;
			}
			else
			{
				out += stock[i];
			}
		}
		++i;
	}

	if (out.empty())
		return false;

	webcrawl::trim(name);
	webcrawl::trim(out);

	stock = out;
	return true;
}


// Write Excel Format file
bool WriteExcel(const char *fileName, const map<string, sCrawlData> &datas)
{
	ofstream ofs(fileName);
	if (!ofs.is_open())
		return false;

	for (auto &kv : datas)
		ofs << kv.second.name << "\t" << kv.second.data << endl;

	return true;
}
