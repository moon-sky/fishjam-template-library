// WowAHView.cpp : implementation of the CWowAHView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WowAHView.h"

#include <ftlWebInterfaceDetect.h>

_ATL_FUNC_INFO CWowAHView::DocumentComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };

BOOL CWowAHView::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
	   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
		return FALSE;

	// give HTML page a chance to translate this message
	return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

int CWowAHView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    LRESULT lRet = DefWindowProc();// DefWindowProc(uMsg, wParam, lParam);

    // Connect events
    CComPtr<IWebBrowser2> spWebBrowser2;
    HRESULT hr = E_FAIL;
    COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2));
    if(SUCCEEDED(hr))
    {
        COM_VERIFY((DispEventAdvise(spWebBrowser2, &DIID_DWebBrowserEvents2)));
    }

    // Set host flag to indicate that we handle themes
    CComPtr<IAxWinAmbientDispatch> spHost;
    COM_VERIFY(QueryHost(IID_IAxWinAmbientDispatch, (void**)&spHost));
    if(SUCCEEDED(hr))
    {
        COM_VERIFY(spHost->put_DocHostFlags(DOCHOSTUIFLAG_NO3DBORDER
            | DOCHOSTUIFLAG_THEME));
    }

    return lRet;
}

void CWowAHView::OnDestroy()
{
    // Disconnect events
    CComPtr<IWebBrowser2> spWebBrowser2;
    HRESULT hr = E_FAIL;
    COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2));
    if(SUCCEEDED(hr))
    {
        DispEventUnadvise(spWebBrowser2, &DIID_DWebBrowserEvents2);
    }
}


HRESULT CWowAHView::CheckElement()
{
    HRESULT hr = E_FAIL;
    CComPtr<IWebBrowser2> spWebBrowser;
    COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser));
    if(SUCCEEDED(hr) && spWebBrowser != NULL)
    {
        CComPtr<IDispatch> spDocument;
        COM_VERIFY(spWebBrowser->get_Document(&spDocument));
        if(SUCCEEDED(hr) && spDocument != NULL)
        {
            CComQIPtr<IHTMLDocument3> spHtmlDoc = spDocument;
            if(spHtmlDoc != NULL)
            {  
                COM_VERIFY(CheckDocument(spHtmlDoc));
            }
        }
    }

    return hr;
}

HRESULT CWowAHView::CheckDocument(CComQIPtr<IHTMLDocument3>& spDoc)
{
    HRESULT hr = E_FAIL;
    CComPtr<IHTMLElementCollection> spElementColl;
    COM_VERIFY(spDoc->getElementsByTagName(CComBSTR(TEXT("a")), &spElementColl));
    if (spElementColl)
    {
        long nCount = 0;
        COM_VERIFY(spElementColl->get_length(&nCount));
        FTL::CFHTMLElementCollectionDumper dumper(spElementColl, 
            CFOutputWindowInfoOutput::Instance(), 0);
    }

    return hr;
}

void CWowAHView::OnEventDocumentComplete(IDispatch* , VARIANT* URL)
{

}