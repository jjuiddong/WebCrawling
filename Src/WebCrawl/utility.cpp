
#include "stdafx.h"
#include "utility.h"
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>
#include <algorithm>
#include <sstream>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"


using namespace webcrawl;


// return Current Active Tab URL string in Chrome
std::string webcrawl::GetCurrentChromeTabURL(const HWND hwnd)
{
	CComQIPtr<IUIAutomation> uia;
	if (FAILED(uia.CoCreateInstance(CLSID_CUIAutomation)) || !uia)
		return {};

	CComPtr<IUIAutomationElement> root;
	if (FAILED(uia->ElementFromHandle(hwnd, &root)) || !root)
		return {};

	CComPtr<IUIAutomationCondition> condition;

	//URL's id is 0xC354, or use UIA_EditControlTypeId for 1st edit box
	uia->CreatePropertyCondition(UIA_ControlTypePropertyId,
		CComVariant(0xC354), &condition);

	//or use edit control's name instead
	//uia->CreatePropertyCondition(UIA_NamePropertyId, 
	//      CComVariant(L"Address and search bar"), &condition);

	CComPtr<IUIAutomationElement> edit;
	if (FAILED(root->FindFirst(TreeScope_Descendants, condition, &edit))
		|| !edit)
		return {}; //maybe we don't have the right tab, continue...

	CComVariant url;
	edit->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &url);
	edit->SetFocus();

	char buff[128];
	ZeroMemory(buff, sizeof(buff));
	const int slength = (int)ARRAYSIZE(buff);
	int len = ::WideCharToMultiByte(CP_ACP, 0, url.bstrVal, slength, 0, 0, NULL, FALSE);
	len = std::min((int)ARRAYSIZE(buff) - 1, len);
	::WideCharToMultiByte(CP_ACP, 0, url.bstrVal, slength, buff, len, NULL, FALSE);
	return buff;
}


// 2017-01-08-11-05-010
// year-month-day-hour-minutes-seconds-millseconds
string webcrawl::GetCurrentDateTime()
{
	// http://stackoverflow.com/questions/22975077/how-to-convert-a-boostptime-to-string

	using namespace boost::gregorian;
	using namespace boost::posix_time;

	boost::gregorian::date dayte(boost::gregorian::day_clock::local_day());
	boost::posix_time::ptime midnight(dayte);
	boost::posix_time::ptime
		now(boost::posix_time::microsec_clock::local_time());
	boost::posix_time::time_duration td = now - midnight;

	date::ymd_type ymd = second_clock::local_time().date().year_month_day();

	std::stringstream ss;
	ss << ymd.year
		<< "-" << std::setfill('0') << std::setw(2) << ymd.month.as_number()
		<< "-" << std::setfill('0') << std::setw(2) << ymd.day
		<< "-" << std::setfill('0') << std::setw(2) << td.hours()
		<< "-" << std::setfill('0') << std::setw(2) << td.minutes()
		<< "-" << std::setfill('0') << std::setw(2) << td.seconds()
		<< "-" << std::setfill('0') << std::setw(3) << td.total_milliseconds() % 1000;

	return ss.str();
}


template <class Seq>
void rotatepopvector(Seq &seq, const unsigned int idx)
{
	if ((seq.size() - 1) > idx)
		std::rotate(seq.begin() + idx, seq.begin() + idx + 1, seq.end());
	seq.pop_back();
}


// "  skdfjskdjf  "
// "$$$skdfjskdjf$$$"
// "skdfjskdjf"
string& webcrawl::trim(string &str)
{
	// 앞에서부터 검색
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if ((str[i] == '\n') || (str[i] == '\t') || (str[i] == '\r') || (str[i] == ' '))
		{
			rotatepopvector(str, i);
			--i;
		}
		else
			break;
	}

	// 뒤에서부터 검색
	for (int i = (int)str.length() - 1; i >= 0; --i)
	{
		if ((str[i] == '\n') || (str[i] == '\t') || (str[i] == '\r') || (str[i] == ' '))
		{
			rotatepopvector(str, i);
		}
		else
			break;
	}

	return str;
}
