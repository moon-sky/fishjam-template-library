// WowAHView.cpp : implementation of the CWowAHView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "WowAHView.h"
#include "SellerItemPageAnalyze.h"
#include "BitItemPageAnalyze.h"
#include "WowItemManager.h"

#include "ScriptErrHandler.h"

#include <ftlWebInterfaceDetect.h>


_ATL_FUNC_INFO CWowAHView::DocumentComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };
_ATL_FUNC_INFO CWowAHView::NavigateComplete2_Info = { CC_STDCALL, VT_EMPTY, 2, { VT_DISPATCH, VT_BYREF | VT_VARIANT } };
_ATL_FUNC_INFO CWowAHView::DownloadBegin_Info = { CC_STDCALL, VT_EMPTY, 0, { } };

CWowAHView::CWowAHView()
{
	for (INT i = 0; i < (INT)pptCount; i++)
	{
		m_pItemPageAnalyzes[i] = NULL;
	}
	m_pWowItemManager = NULL;
	m_ParsePageType = pptInvalid;
}

CWowAHView::~CWowAHView()
{
	for (INT i = 0; i < (INT)pptCount; i++)
	{
		FTLASSERT(NULL == m_pItemPageAnalyzes[i]);
	}
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

HRESULT CWowAHView::Navigate(const CString& strURL)
{
    HRESULT hr = E_FAIL;
    CComPtr<IWebBrowser2> spWebBrowser2;
    COM_VERIFY(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2));
    if(SUCCEEDED(hr))
    {
        CComVariant varURL(strURL);
        CComVariant varEmpty;
        COM_VERIFY(spWebBrowser2->Navigate2(&varURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty));
    }
    return hr;
}


STDMETHODIMP CWowAHView::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
							DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
							EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr)
{
	if (DISPID_STATUSTEXTCHANGE != dispIdMember
		&& DISPID_COMMANDSTATECHANGE != dispIdMember
		&& DISPID_PROGRESSCHANGE != dispIdMember)
	{
		FTL::CFIExplorerDispidInfo  idspidInfo(dispIdMember, pDispParams);
		FTLTRACE(TEXT("[%d] Invoke, dispIdMember=%d(%s)\n"), GetCurrentThreadId(),
			dispIdMember, idspidInfo.GetConvertedInfo());
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
	m_pWowItemManager = new CWowItemManager();

	m_pItemPageAnalyzes[pptSellerBrowse] = new CSellerItemPageAnalyze(m_pWowItemManager);
	m_pItemPageAnalyzes[pptMyBit] = new CBitItemPageAnalyze(m_pWowItemManager);

    return lRet;
}

void CWowAHView::OnClose()
{
	SetMsgHandled(FALSE);
}

void CWowAHView::OnDestroy()
{
	for (INT i = 0; i < (UINT)pptCount; i++)
	{
		SAFE_DELETE(m_pItemPageAnalyzes[i]);
	}
	SAFE_DELETE(m_pWowItemManager);
	

	// Disconnect events
	CComPtr<IWebBrowser2> spWebBrowser2;
	HRESULT hr = E_FAIL;
	//为什么会多次调用 OnDestroy?
	COM_VERIFY_EXCEPT1(QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser2), E_FAIL);
	if(SUCCEEDED(hr))
	{
		DispEventUnadvise(spWebBrowser2, &DIID_DWebBrowserEvents2);
	}
	//SetMsgHandled(FALSE);
}

void CWowAHView::OnGotoPage(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(Navigate(_T("http://www.battlenet.com.cn/wow/zh/")));
}

void CWowAHView::OnParseSellerBrowse(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	_OpenLocalFileAndParse(pptSellerBrowse);
}

void CWowAHView::OnParseCreateMyAuction(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	_OpenLocalFileAndParse(pptCreateMyAuction);
}

void CWowAHView::OnParseMyBid(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	_OpenLocalFileAndParse(pptMyBit);
}

void CWowAHView::OnParseMyAuction(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	_OpenLocalFileAndParse(pptMyAuction);
}

void CWowAHView::OnSearchSpecialItem(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(SearchSpecialItem(TEXT("中皮")));
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



HRESULT CWowAHView::SearchSpecialItem(const CString& strItemName)
{
	HRESULT hr = E_FAIL;
    FTLTRACE(TEXT("SearchSpecialItem: %s\n"), strItemName);

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

        FTLASSERT(spInputTextItemName);
        FTLASSERT(spSubmitItem);
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
	HRESULT hr = E_FAIL;
	if (m_ParsePageType != pptInvalid)
	{
		CComQIPtr<IHTMLDocument3> spHtmlDoc = _GetDocument();
		if (spHtmlDoc)
		{
			COM_VERIFY(m_pItemPageAnalyzes[m_ParsePageType]->ParseItemPage(spHtmlDoc, _T("")));
		}
	}
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

void CWowAHView::_OpenLocalFileAndParse(ParsePageType pageType)
{
	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;

	TCHAR szCurPath[MAX_PATH] = {0};

	API_VERIFY( GetModuleFileName(NULL, szCurPath, _countof(szCurPath)) > 0);
	API_VERIFY(PathRemoveFileSpec(szCurPath));
	LPTSTR pszLastDirPos = StrRChr(szCurPath, NULL, _T('\\'));
	FTLASSERT(pszLastDirPos != NULL);
	if (pszLastDirPos)
	{
		*pszLastDirPos = NULL;
	}
	static LPCTSTR pszPage[pptCount] = 
	{
		TEXT("\\Page\\SellerBrowse.xht"),
		TEXT("\\Page\\CreateMyAuction.xht"),
		TEXT("\\Page\\MyBid.xht"),
		TEXT("\\Page\\MyAuction.xht"),
	};

	API_VERIFY(PathAppend(szCurPath, pszPage[pageType]));

	CString strUrl;
	strUrl.Format(TEXT("file:///%s"), szCurPath);
	strUrl.Replace(_T('\\'), _T('/'));

	COM_VERIFY(Navigate(strUrl));
	m_ParsePageType = pageType;

}