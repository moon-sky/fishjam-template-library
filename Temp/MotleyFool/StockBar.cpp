//StockBar.cpp : Implementation of CStockBar

//***************************************************************************//
//                                                                           //
//  This file was created using the DeskBand ATL Object Wizard 2.0           //
//  By Erik Thompson 2001                                                  //
//  Email questions and comments to erikt@radbytes.com						 //
//                                                                           //
//***************************************************************************//

#include "stdafx.h"
#include <wchar.h>
#include "MotleyFool.h"
#include "StockBar.h"

const WCHAR TITLE_CStockBar[] = L"The Motley Fool";

/////////////////////////////////////////////////////////////////////////////
// CStockBar

CStockBar::CStockBar(): 
	m_dwBandID(0), 
	m_dwViewMode(0), 
	m_bShow(FALSE), 
	m_bEnterHelpMode(FALSE), 
	m_hWndParent(NULL), 
	m_pSite(NULL)
{
    FTLTRACE(TEXT("%s\n"), TEXT(__FUNCTION__));
	m_ReflectWnd.GetToolBar().GetEditBox().SetBand(this);
}

BOOL CStockBar::RegisterAndCreateWindow()
{
	RECT rect;
	::GetClientRect(m_hWndParent, &rect);
    FTLTRACE(TEXT("%s, rect=%s\n"), TEXT(__FUNCTION__), CFRectDumpInfo(rect).GetConvertedInfo());
	m_ReflectWnd.Create(m_hWndParent, rect, NULL, WS_CHILD);
	// The toolbar is the window that the host will be using so it is the window that is important.
	return m_ReflectWnd.GetToolBar().IsWindow();
}

// IDeskBand
STDMETHODIMP CStockBar::GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi)
{
    FTLTRACE(TEXT("%s, bandid=%d, viewMode=%d\n"), TEXT(__FUNCTION__), dwBandID, dwViewMode);

	m_dwBandID = dwBandID;
	m_dwViewMode = dwViewMode;

	if (pdbi)
	{
		if (pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = 250;
			pdbi->ptMinSize.y = 22;
		}
		if (pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = 0; // ignored
			pdbi->ptMaxSize.y = -1;	// width
		}
		if (pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 0; // ignored
			pdbi->ptIntegral.y = 0; // not sizeable
		}
		if (pdbi->dwMask & DBIM_ACTUAL)
		{
			pdbi->ptActual.x = 250;
			pdbi->ptActual.y = 22;
		}
		if (pdbi->dwMask & DBIM_TITLE)
		{
			wcscpy(pdbi->wszTitle, L"Fishjam Demo"); //TITLE_CStockBar
		}
		//if (pdbi->dwMask & DBIM_BKCOLOR)
		{
			//Use the default background color by removing this flag.
			//pdbi->dwMask &= ~DBIM_BKCOLOR;
            pdbi->crBkgnd = RGB(255, 0, 0);
            pdbi->dwMask |= DBIM_BKCOLOR;
		}
		if (pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;
		}
	}
	return S_OK;
}

// IOleWindow
STDMETHODIMP CStockBar::GetWindow(HWND* phwnd)
{
    FTLTRACE(TEXT("%s\n"), TEXT(__FUNCTION__));

	HRESULT hr = S_OK;
	if (NULL == phwnd)
	{
		hr = E_INVALIDARG;
	}
	else
	{
		*phwnd = m_ReflectWnd.GetToolBar().m_hWnd;
	}
	return hr;
}

STDMETHODIMP CStockBar::ContextSensitiveHelp(BOOL fEnterMode)
{
    FTLTRACE(TEXT("%s, fEnterMode=%d\n"), TEXT(__FUNCTION__), fEnterMode);

	m_bEnterHelpMode = fEnterMode;
	return S_OK;
}

// IDockingWindow
STDMETHODIMP CStockBar::CloseDW(unsigned long dwReserved)
{
    FTLTRACE(TEXT("%s\n"), TEXT(__FUNCTION__));
	ShowDW(FALSE);
	return S_OK;
}

STDMETHODIMP CStockBar::ResizeBorderDW(const RECT* prcBorder, IUnknown* punkToolbarSite, BOOL fReserved)
{
    FTLTRACE(TEXT("%s\n"), TEXT(__FUNCTION__));

	// Not used by any band object.
	return E_NOTIMPL;
}

STDMETHODIMP CStockBar::ShowDW(BOOL fShow)
{
    FTLTRACE(TEXT("%s, fShow=%d\n"), TEXT(__FUNCTION__), fShow);

	m_bShow = fShow;
	m_ReflectWnd.GetToolBar().ShowWindow(m_bShow ? SW_SHOW : SW_HIDE);
	return S_OK;
}

// IObjectWithSite
STDMETHODIMP CStockBar::SetSite(IUnknown* pUnkSite)
{
    HRESULT hr = E_FAIL;
    BOOL bRet = FALSE;
#if defined _M_IX86
    ::MessageBox(NULL, TEXT("in CStockBar::SetSite"), TEXT("X86"), MB_OK);
#else
    ::MessageBox(NULL, TEXT("in CStockBar::SetSite"), TEXT("X64"), MB_OK);
#endif 

    FTLTRACE(TEXT("%s, pUnkSite=0x%p, m_pSite=0x%p\n"), TEXT(__FUNCTION__), pUnkSite, m_pSite);

//If a site is being held, release it.
	if(m_pSite)
	{
		m_ReflectWnd.GetToolBar().SetBrowser(NULL);
		m_pSite->Release();
		m_pSite = NULL;
	}

	//If punkSite is not NULL, a new site is being set.
	if(pUnkSite)
	{
        COM_DETECT_INTERFACE_FROM_LIST(pUnkSite);
        COM_DETECT_INTERFACE_FROM_REGISTER(pUnkSite);
        
		//Get the parent window.
		IOleWindow  *pOleWindow = NULL;

		m_hWndParent = NULL;

		COM_VERIFY(pUnkSite->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow));
        if(SUCCEEDED(hr))
		{
			COM_VERIFY(pOleWindow->GetWindow(&m_hWndParent));
            FTLTRACE(TEXT("m_hWndParent = 0x%x\n"), m_hWndParent);
			pOleWindow->Release();
		}
        FTLASSERT(::IsWindow(m_hWndParent));

		if(!::IsWindow(m_hWndParent))
			return E_FAIL;

		API_VERIFY(RegisterAndCreateWindow());
        if(!bRet){
			return E_FAIL;
        }
		//Get and keep the IInputObjectSite pointer.
		COM_VERIFY((pUnkSite->QueryInterface(IID_IInputObjectSite, (LPVOID*)&m_pSite)));
		if(FAILED(hr))
        {
			return hr;
		}  

		IWebBrowser2* s_pFrameWB = NULL;
		IOleCommandTarget* pCmdTarget = NULL;
		COM_VERIFY(pUnkSite->QueryInterface(IID_IOleCommandTarget, (LPVOID*)&pCmdTarget));
		if (SUCCEEDED(hr))
		{
			IServiceProvider* pSP;
			hr = pCmdTarget->QueryInterface(IID_IServiceProvider, (LPVOID*)&pSP);
            COM_DETECT_SERVICE_PROVIDER_FROM_LIST(pSP);
            COM_DETECT_SERVICE_PROVIDER_FROM_REGISTER(pSP);

			pCmdTarget->Release();

			if (SUCCEEDED(hr))
			{
				hr = pSP->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (LPVOID*)&s_pFrameWB);
				pSP->Release();
				_ASSERT(s_pFrameWB);
				m_ReflectWnd.GetToolBar().SetBrowser(s_pFrameWB);
				s_pFrameWB->Release();
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CStockBar::GetSite(REFIID riid, void **ppvSite)
{
    FTLTRACE(TEXT("%s\n"), TEXT(__FUNCTION__));

	*ppvSite = NULL;

	if(m_pSite)
	{
	   return m_pSite->QueryInterface(riid, ppvSite);
	}
	return E_FAIL;
}

void CStockBar::FocusChange(BOOL bHaveFocus)
{
    FTLTRACE(TEXT("%s, bHaveFocus=%d\n"), TEXT(__FUNCTION__), bHaveFocus);

    if (m_pSite)
	{
		IUnknown* pUnk = NULL;
		if (SUCCEEDED(QueryInterface(IID_IUnknown, (LPVOID*)&pUnk)) && pUnk != NULL)
		{
			m_pSite->OnFocusChangeIS(pUnk, bHaveFocus);
			pUnk->Release();
			pUnk = NULL;
		}
	}
}

STDMETHODIMP CStockBar::HasFocusIO(void)
{
    FTLTRACE(TEXT("%s\n"), TEXT(__FUNCTION__));

	// if any of the windows in our toolbar have focus then return S_OK else S_FALSE.
	if (m_ReflectWnd.GetToolBar().m_hWnd == ::GetFocus())
		return S_OK;
	if (m_ReflectWnd.GetToolBar().GetEditBox().m_hWnd == ::GetFocus())
		return S_OK;
	return S_FALSE;
}
STDMETHODIMP CStockBar::TranslateAcceleratorIO(LPMSG lpMsg)
{
    DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, lpMsg->message, lpMsg->wParam, lpMsg->lParam);

	// the only window that needs to translate messages is our edit box so forward them.
	return m_ReflectWnd.GetToolBar().GetEditBox().TranslateAcceleratorIO(lpMsg);
}
STDMETHODIMP CStockBar::UIActivateIO(BOOL fActivate, LPMSG lpMsg)
{
    FTLTRACE(TEXT("%s, fActive=%d\n"), TEXT(__FUNCTION__), fActivate);

	// if our deskband is being activated then set focus to the edit box.
	if(fActivate)
	{
		m_ReflectWnd.GetToolBar().GetEditBox().SetFocus();
	}
	return S_OK;
}
