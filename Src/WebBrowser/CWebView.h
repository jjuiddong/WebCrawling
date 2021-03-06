#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 

// CWebView html view

class CWebView : public CHtmlView
{
	DECLARE_DYNCREATE(CWebView)

protected:
	CWebView();           // protected constructor used by dynamic creation
	virtual ~CWebView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


