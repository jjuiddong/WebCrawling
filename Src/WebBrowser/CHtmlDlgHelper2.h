// CHtmlDlgHelper2.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CHtmlDlgHelper2

class CHtmlDlgHelper2 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CHtmlDlgHelper2)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x3050F4E1, 0x98B5, 0x11CF, { 0xBB, 0x82, 0x0, 0xAA, 0x0, 0xBD, 0xCE, 0xB } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID,
						CCreateContext* pContext = nullptr)
	{
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
				BSTR bstrLicKey = nullptr)
	{
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey);
	}

// Attributes
public:

// Operations
public:

	LPDISPATCH get_document()
	{
		LPDISPATCH result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	CString openfiledlg(VARIANT initFile, VARIANT initDir, VARIANT filter, VARIANT title)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &initFile, &initDir, &filter, &title);
		return result;
	}
	CString savefiledlg(VARIANT initFile, VARIANT initDir, VARIANT filter, VARIANT title)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &initFile, &initDir, &filter, &title);
		return result;
	}
	long choosecolordlg(VARIANT initColor)
	{
		long result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, parms, &initColor);
		return result;
	}
	long getCharset(LPCTSTR fontName)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_I4, (void*)&result, parms, fontName);
		return result;
	}
	LPDISPATCH get_fonts()
	{
		LPDISPATCH result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, nullptr);
		return result;
	}
	CString openfiledlgex(VARIANT initFile, VARIANT initDir, VARIANT filter, VARIANT initFilterIndex, VARIANT title)
	{
		CString result;
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, &initFile, &initDir, &filter, &initFilterIndex, &title);
		return result;
	}


};
