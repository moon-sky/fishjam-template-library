// WowAHView.cpp : implementation of the CWowAHView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WowAHView.h"
#include "ItemPageAnalyzeBase.h"
#include "ScriptErrHandler.h"

#include <ftlWebInterfaceDetect.h>


_ATL_FUNC_INFO CWowAHView::DocumentComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };
_ATL_FUNC_INFO CWowAHView::NavigateComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };
_ATL_FUNC_INFO CWowAHView::DownloadBegin_Info = { CC_STDCALL, VT_EMPTY, 0, { } };

CWowAHView::CWowAHView()
{
	m_pItemPageAnalyze = NULL;
}

CWowAHView::~CWowAHView()
{
	FTLASSERT(NULL == m_pItemPageAnalyze);
}

BOOL CWowAHView::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
	   (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
		return FALSE;

	// give HTML page a chance to translate this message
	return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
}
//void CWowAHView::OnFinalMessage(HWND /*hWnd*/)
//{
//	delete this;
//}


STDMETHODIMP CWowAHView::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
							DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
							EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr)
{
	if (DISPID_STATUSTEXTCHANGE != dispIdMember
		&& DISPID_COMMANDSTATECHANGE != dispIdMember
		&& DISPID_PROGRESSCHANGE != dispIdMember)
	{
		FTL::CFIExplorerDispidInfo  idspidInfo(dispIdMember, pDispParams);
		FTLTRACE(TEXT("Invoke, dispIdMember=%d(%s)\n"), dispIdMember, idspidInfo.GetConvertedInfo());
	}

	HRESULT hr = __super::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult,
		pExcepInfo, puArgErr);

	return hr;
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
		COM_VERIFY(spHost->put_AllowShowUI(VARIANT_FALSE));

    }
	m_pItemPageAnalyze = new CItemPageAnalyzeBase();

    return lRet;
}

void CWowAHView::OnClose()
{
}
void CWowAHView::OnDestroy()
{
	SAFE_DELETE(m_pItemPageAnalyze);

	// Disconnect events
	CComPtr<IWebBrowser2> spWebBrowser2;
	HRESULT hr = E_FAIL;
	COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2));
	if(SUCCEEDED(hr))
	{
		DispEventUnadvise(spWebBrowser2, &DIID_DWebBrowserEvents2);
	}
}

CComPtr<IHTMLDocument3>	CWowAHView::_GetDocument()
{
	HRESULT hr = E_FAIL;
	CComQIPtr<IHTMLDocument3> spHtmlDoc;

	CComPtr<IWebBrowser2> spWebBrowser;
	COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser));
	if(spWebBrowser)
	{
		CComPtr<IDispatch> spDispDocument;
		COM_VERIFY(spWebBrowser->get_Document(&spDispDocument));
		if(spDispDocument)
		{
			spHtmlDoc = spDispDocument;
		}
	}

	return spHtmlDoc;
}


HRESULT CWowAHView::CheckElement()
{
    HRESULT hr = E_FAIL;
	CComQIPtr<IHTMLDocument3> spHtmlDoc = _GetDocument();
	if (spHtmlDoc)
	{
		COM_VERIFY(m_pItemPageAnalyze->ParseItemPage(spHtmlDoc));
	}

    return hr;
}

HRESULT CWowAHView::SearchSpecialItem(const CString& strItemName)
{
	HRESULT hr = E_FAIL;
	CComQIPtr<IHTMLDocument3> spHtmlDoc = _GetDocument();
	if (spHtmlDoc)
	{
		CComPtr<IHTMLElement>	spInputItemName;
		CComPtr<IHTMLElement>   spSubmitItem;
		COM_VERIFY(spHtmlDoc->getElementById(CComBSTR("itemName"), &spInputItemName));
		COM_VERIFY(spHtmlDoc->getElementById(CComBSTR("button-submit"), &spSubmitItem));

		CComQIPtr<IHTMLInputTextElement> spInputTextItemName = spInputItemName;
		//COM_DETECT_INTERFACE_FROM_REGISTER(spSubmitItem);
		//CComQIPtr<IHTMLButtonElement>	spSubmitButtonItem = spSubmitItem;
		if (spInputTextItemName && spSubmitItem) // && spSubmitButtonItem)
		{
			COM_VERIFY(spInputTextItemName->put_value(CComBSTR(strItemName)));
			COM_VERIFY(spSubmitItem->click());
		}
	}

	return hr;
}

void CWowAHView::OnEventDocumentComplete(IDispatch* , VARIANT* URL)
{

}

void CWowAHView::OnNavigateComplete2(IDispatch* /*pDisp*/, VARIANT* URL)
{
	HRESULT hr = E_FAIL;
	CComPtr<IWebBrowser2> spWebBrowser;
	COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser));
	if (spWebBrowser)
	{
		//COM_DETECT_INTERFACE_FROM_REGISTER(spWebBrowser);

		COM_VERIFY(spWebBrowser->put_Silent(VARIANT_TRUE));

#pragma TODO(为什么这个时候得不到 IHTMLDocument2)
		CComQIPtr<IHTMLDocument2> spDoc = spWebBrowser;
		if (spDoc)
		{
			CScriptErrHandler scriptHandler;
			scriptHandler.ShieldCurrPage(spDoc);
			scriptHandler.ShieldAllChildPages(spDoc);
		}
	}
}

void CWowAHView::OnDownloadBegin()
{
	FTLTRACE(TEXT("On Download Begin\n"));
}