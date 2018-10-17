//
// 2018-10-16, jjuiddong
// utility
//
#pragma once


namespace webcrawl
{

	std::string GetCurrentChromeTabURL(const HWND hwnd);

	// year-month-day-hour-minutes-seconds-millseconds
	string GetCurrentDateTime();

	string& trim(string &str);
}
