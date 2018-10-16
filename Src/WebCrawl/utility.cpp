
#include "stdafx.h"
#include "utility.h"
#include <AtlBase.h>
#include <AtlCom.h>
#include <UIAutomation.h>
#include <algorithm>

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

	char buff[128];
	ZeroMemory(buff, sizeof(buff));
	const int slength = (int)ARRAYSIZE(buff);
	int len = ::WideCharToMultiByte(CP_ACP, 0, url.bstrVal, slength, 0, 0, NULL, FALSE);
	len = std::min((int)ARRAYSIZE(buff) - 1, len);
	::WideCharToMultiByte(CP_ACP, 0, url.bstrVal, slength, buff, len, NULL, FALSE);
	return buff;
}
