
// WebBrowserView.cpp : implementation of the CWebBrowserView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WebBrowser.h"
#endif

#include "WebBrowserDoc.h"
#include "WebBrowserView.h"
#include "CHtmlDlgHelper2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebBrowserView

IMPLEMENT_DYNCREATE(CWebBrowserView, CHtmlView)

BEGIN_MESSAGE_MAP(CWebBrowserView, CHtmlView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHtmlView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_NEW, &CWebBrowserView::OnFileNew)
END_MESSAGE_MAP()

// CWebBrowserView construction/destruction

CWebBrowserView::CWebBrowserView() noexcept
{
	// TODO: add construction code here
}

CWebBrowserView::~CWebBrowserView()
{
}

BOOL CWebBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

// CWebBrowserView drawing

void CWebBrowserView::OnDraw(CDC* /*pDC*/)
{
	CWebBrowserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CWebBrowserView printing

BOOL CWebBrowserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWebBrowserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWebBrowserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWebBrowserView diagnostics

#ifdef _DEBUG
void CWebBrowserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebBrowserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CWebBrowserDoc* CWebBrowserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWebBrowserDoc)));
	return (CWebBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CWebBrowserView message handlers


void CWebBrowserView::OnFileNew()
{
	//SetSilent(TRUE);
	//Navigate(L"https://www.investing.com/equities/amazon-com-inc");

	//IDispatch *spDocIDispatch = NULL;
	//m_pBrowserApp->get_Document(&spDocIDispatch);

	//if (spDocIDispatch == NULL) return;
	//IHTMLDocument2* pDoc2 = NULL;
	//spDocIDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*)&pDoc2);
	//if (pDoc2 != NULL)
	//{
	//	CComQIPtr<IHTMLDocument> pDoc(pDoc2);
	//	CComPtr<IDispatch> pDisp;
	//	HRESULT hr = pDoc->get_Script(&pDisp);
	//	if (SUCCEEDED(hr))
	//	{
	//		OLECHAR FAR* szFunction = L"xSet";
	//		DISPID dispid;
	//		hr = pDisp->GetIDsOfNames(IID_NULL, &szFunction, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	//		if (SUCCEEDED(hr))
	//		{
	//			COleVariant vtResult;
	//			static BYTE parms[] = VTS_BSTR;
	//			COleDispatchDriver dispDriver(pDisp, FALSE);
	//			dispDriver.InvokeHelper(dispid, DISPATCH_METHOD, VT_VARIANT,
	//				(void*)&vtResult, parms, (LPCTSTR)strOpt);
	//		}
	//	}
	//	pDoc2->Release();
	//}
	//spDocIDispatch->Release();
}
