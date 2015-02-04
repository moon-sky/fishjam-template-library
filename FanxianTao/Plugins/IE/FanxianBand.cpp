// FanxianBand.cpp : Implementation of CFanxianBand

#include "stdafx.h"
#include "FanxianBand.h"

#include <ShlGuid.h>

#define TB_MIN_SIZE_X   10
#define TB_MIN_SIZE_Y   10
#define TB_MAX_SIZE_Y   25

void InitWinSock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 2 );
	WSAStartup( wVersionRequested, &wsaData );
}

void UninitWinSock()
{
	WSACleanup( );
}

// CFanxianBand
CFanxianBand::CFanxianBand()
{
	//m_hReBar = NULL;
	m_hWndParent = NULL;
	m_dwBandID = 0;
	m_dwViewMode = 0;
	m_bFocus = FALSE;
	m_bAdvised = FALSE;
	//m_ReflectWnd.GetToolBar().GetEditBox().SetBand(this);

}

CFanxianBand::~CFanxianBand()
{

}

HRESULT CFanxianBand::FinalConstruct()
{
	return S_OK;
}

void CFanxianBand::FinalRelease()
{

}

STDMETHODIMP CFanxianBand::GetWindow(HWND *phWnd)
{
	// Get safe window handle
	*phWnd = m_Toolbar.m_hWnd;

	return S_OK;
}

STDMETHODIMP CFanxianBand::ContextSensitiveHelp(BOOL fEnterMode)
{
	ATLTRACENOTIMPL("IOleWindow::ContextSensitiveHelp");
}

STDMETHODIMP CFanxianBand::ShowDW(BOOL bShow)
{
	// Do we have a valid toolbar?
	if (m_Toolbar.m_hWnd)
	{
		// Should we show or hide our window?
		if (bShow)
		{
			// Show our window
			m_Toolbar.ShowWindow(SW_SHOW);
		}
		else
		{
			// Hide our window
			m_Toolbar.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		MessageBox(m_hWndParent, TEXT("CFanxianBand::ShowDW with None m_hWnd"), NULL, MB_OK);
	}

	// Succeeded
	return S_OK;
}

STDMETHODIMP CFanxianBand::CloseDW(DWORD /*dwReserved*/)
{
	// Hide
	ShowDW(FALSE);
	return S_OK;
}

STDMETHODIMP CFanxianBand::ResizeBorderDW(const RECT* /*pRC*/, IUnknown* /*pUnkSite*/, BOOL /*bReserved*/)
{
	ATLTRACENOTIMPL("IDockingWindow::ResizeBorderDW");
}

STDMETHODIMP CFanxianBand::UIActivateIO(BOOL bActivate, LPMSG /*pMsg*/)
{
	//if ( bActivate && m_Toolbar.IsLoadThemeDone() )
	//{
	//	::SetFocus(m_Toolbar.GetSearchCombo()->GetEditCtrl());
	//}

	return S_OK;
}

HRESULT CFanxianBand::GetBandInfo( 
								  /* [in] */ DWORD dwBandID,
								  /* [in] */ DWORD dwViewMode,
								  /* [out][in] */ __RPC__inout DESKBANDINFO *pdbi)
{
	if (pdbi)
	{
		m_dwBandID = dwBandID;
		m_dwViewMode = dwViewMode;

		if (pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = TB_MIN_SIZE_X;
			pdbi->ptMinSize.y = TB_MIN_SIZE_Y;
		}

		if (pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = -1;
			pdbi->ptMaxSize.y = TB_MAX_SIZE_Y;
		}

		if(pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 1;
			pdbi->ptIntegral.y = 1;
		}

		if (pdbi->dwMask & DBIM_ACTUAL)
		{
			pdbi->ptActual.x = 200;
			pdbi->ptActual.y = TB_MAX_SIZE_Y;
		}

		if (pdbi->dwMask & DBIM_TITLE)
		{
			StringCchCopy(pdbi->wszTitle, _countof(pdbi->wszTitle), TEXT("FanxianTao"));
		}

		if (pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_NORMAL;

			pdbi->dwModeFlags |= DBIMF_VARIABLEHEIGHT;
		}

		if (pdbi->dwMask & DBIM_BKCOLOR)
		{
			// Use the default background color by removing this flag
			pdbi->dwMask &= ~DBIM_BKCOLOR;
		}

		// Succeeded
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

STDMETHODIMP CFanxianBand::HasFocusIO(void)
{
	// Does the window has a focus?
	if (m_bFocus)
	{	
		// Yes, return S_OK
		return S_OK;
	}
	else
	{
		// No, return S_FALSE
		return S_FALSE;
	}
}

STDMETHODIMP CFanxianBand::TranslateAcceleratorIO(LPMSG pMsg)
{
	// Needed for MFC calls
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// Let toolbar translate
	return m_Toolbar.TranslateAcceleratorIO(pMsg);
}


STDMETHODIMP CFanxianBand::RegisterEventHandler(BOOL inAdvise)
{
	HRESULT hr = E_FAIL;
	if (inAdvise)
	{
		COM_VERIFY(DispEventAdvise(m_spWebBrowser2));
		if (SUCCEEDED(hr))
		{
			m_bAdvised = TRUE;
		}
	}
	else
	{
		if (m_bAdvised)
		{
			COM_VERIFY(DispEventUnadvise(m_spWebBrowser2));
			m_bAdvised = FALSE;
		}
		
	}
	return hr;
}
HRESULT CFanxianBand::CreateToolWindow()
{
	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;

	CWindow wndParent(m_hWndParent);

	CRect rcClientParent;
	rcClientParent.SetRectEmpty();
	API_VERIFY(wndParent.GetClientRect(&rcClientParent));

	API_VERIFY(NULL != m_Toolbar.Create(m_hWndParent, rcClientParent));
	
	return S_OK;
}

BOOL CFanxianBand::GetWebBrowserPtr( IUnknown* pUnkSite )
{
	CComQIPtr<IOleWindow> spOleWindow(pUnkSite);
	CComQIPtr<IServiceProvider> spServiceProvider(pUnkSite);
	FTLASSERT(spOleWindow);
	FTLASSERT(spServiceProvider);

	if (spOleWindow == NULL)
	{
		return FALSE;
	}

	if (spServiceProvider == NULL)
	{
		return FALSE;
	}

	//spOleWindow->GetWindow(&m_hReBar);
	m_spWebBrowser2.Release();


	spServiceProvider->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, reinterpret_cast<void**>(&m_spWebBrowser2));
	if ( m_spWebBrowser2 == NULL )
	{
		return FALSE;
	}

	return TRUE;
}

STDMETHODIMP CFanxianBand::SetSite(IUnknown *pUnkSite)
{
	HRESULT hr = E_POINTER;
	BOOL bRet = FALSE;
	
	//MessageBox(NULL, TEXT("SetSite"), TEXT("SetSite"), MB_OK);

	FTLTRACE(TEXT("CFanxianBand::SetSite, pUnkSite=0x%p\n"), pUnkSite);

	if (pUnkSite)
	{
		InitWinSock();

#ifdef DEBUG
		FormatMessageBox(NULL, TEXT("SetSite"), MB_OK, TEXT("SetSite for 0x%p"), pUnkSite);
        FTLASSERT(CONNECT_E_NOCONNECTION == 0x80040200)
#endif 
		COM_DETECT_INTERFACE_FROM_REGISTER(pUnkSite);

		//API_VERIFY(GetWebBrowserPtr(pUnkSite));

		m_spWebBrowser2 = pUnkSite;
		if (m_spWebBrowser2)
		{
			COM_VERIFY(RegisterEventHandler(TRUE));
		}
		
		FTLASSERT(NULL == m_hWndParent);
		CComQIPtr<IWebBrowserApp> spWebBrowserApp(pUnkSite);
		if (spWebBrowserApp)
		//CComQIPtr<IOleWindow> spOleWindow(m_spWebBrowser2);
		//if (spOleWindow)
		{
			//COM_VERIFY(spOleWindow->GetWindow(&m_hWndParent));
			COM_VERIFY(spWebBrowserApp->get_HWND((SHANDLE_PTR*)&m_hWndParent));
			FTLASSERT(m_hWndParent != NULL);
			if (m_hWndParent)
			{
				COM_VERIFY(CreateToolWindow());
			}
		}
		

		m_spUnkSite = pUnkSite;
	}
	else
	{
		COM_VERIFY_EXCEPT1(RegisterEventHandler(FALSE), CONNECT_E_NOCONNECTION);
	}
	if(SUCCEEDED(hr))
	{
		COM_VERIFY(IObjectWithSiteImpl<CFanxianBand>::SetSite(pUnkSite));
	}
	return hr;
}

#if ENABLE_INVOKE_DUMP_INFO

STDMETHODIMP CFanxianBand::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,
								  DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
									EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr)
{
	HRESULT hr = S_OK;

	FTL::CFIExplorerDispidInfo  idspidInfo(dispIdMember, pDispParams);

#ifdef FTL_DEBUG
	//FTLTRACE(TEXT("Invoke, dispIdMember=%d(%s)\n"), dispIdMember, idspidInfo.GetConvertedInfo());
#else
	//OutputDebugString(idspidInfo.GetConvertedInfo());
#endif 

	hr = IDispEventImpl::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	return hr;
}

#endif //ENABLE_INVOKE_DUMP_INFO


STDMETHODIMP CFanxianBand::OnBeforeNavigate2(IDispatch* pDispatch, VARIANT* pvUrl, VARIANT* /*pvFlags*/,
										  VARIANT* pvTarget, VARIANT* /*pv*/, VARIANT* /*pvHeader*/,
										  VARIANT_BOOL* pvCancel) // NS
{
	HRESULT hr = S_OK;
	//MessageBox(NULL, TEXT("OnBeforeNavigate2"), NULL, MB_OK);

	if (pvUrl)
	{
		CAtlStringW atlStrURL;
		atlStrURL.Format(L"%Ls", pvUrl->bstrVal );
		FTLTRACE(TEXT("OnBeforeNavigate2, %s\n"), atlStrURL);

        static LPCTSTR pszWeb[] = {
            _T("tmall.com"),
            _T("taobao.com"),
        };

        int nPos = -1;
        for (int i = 0; i < _countof(pszWeb); i++)
        {
            nPos = atlStrURL.Find(pszWeb[i]);
            if (nPos >= 0)
            {
                //找到一个需要替换的网站
                break;
            }
        }


        if (nPos >= 0)
        {
            if ((atlStrURL.Find(TEXT("id=")) > 0) && (atlStrURL.Find(TEXT("ali_trackid=2:mm_43549321_0_0")) < 0))
            {
                const CAtlString strTrackId = _T("ali_trackid=2:");
                const CAtlString strMyTrackId = _T("mm_43549321_0_0");

                FTLTRACE(TEXT("Before Change %s\n"), atlStrURL);

                nPos = atlStrURL.Find(strTrackId);
                if (nPos < 0)
                {
                    atlStrURL = atlStrURL + L"&ali_trackid=2:mm_43549321_0_0";
                }
                else
                {
                    CAtlStringW strNewUrl = atlStrURL.Left(nPos) + strTrackId + strMyTrackId + atlStrURL.Mid(nPos + strTrackId.GetLength() + strMyTrackId.GetLength());
                    atlStrURL = strNewUrl;
                }
                FTLTRACE(TEXT("After Change %s\n"), atlStrURL);

                CComBSTR bstrUrl(atlStrURL);
                CComVariant varEmpty;
                *pvCancel = VARIANT_TRUE;
                COM_VERIFY(m_spWebBrowser2->Stop());
                COM_VERIFY(m_spWebBrowser2->Navigate(bstrUrl, &varEmpty, &varEmpty, &varEmpty, &varEmpty));

            }
        }
	}

	return hr;
}

STDMETHODIMP CFanxianBand::OnDocumentComplete(IDispatch *pDisp, VARIANT *pvarURL)
{
	HRESULT hr = S_OK;

	// Query for the IWebBrowser2 interface.
	CComQIPtr<IWebBrowser2> spTempWebBrowser = pDisp;

	// Is this event associated with the top-level browser?
	if (spTempWebBrowser && m_spWebBrowser2 &&
		m_spWebBrowser2.IsEqualObject(spTempWebBrowser))
	{
		// Get the current document object from browser...
		CComPtr<IDispatch> spDispDoc;
		COM_VERIFY(hr = m_spWebBrowser2->get_Document(&spDispDoc));
		if (SUCCEEDED(hr))
		{
			// ...and query for an HTML document.
			CComQIPtr<IHTMLDocument2> spHTMLDoc = spDispDoc;
			if (spHTMLDoc != NULL)
			{
				// Finally, remove the images.
				//RemoveImages(spHTMLDoc);
			}
		}
	}

	return hr;
}