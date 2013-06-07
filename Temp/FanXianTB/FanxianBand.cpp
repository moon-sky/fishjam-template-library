// FanxianBand.cpp : Implementation of CFanxianBand

#include "stdafx.h"
#include "FanxianBand.h"

#include <ShlGuid.h>


// CFanxianBand

STDMETHODIMP CFanxianBand::RegisterEventHandler(BOOL inAdvise)
{
	HRESULT hr = E_FAIL;
	//CComPtr<IConnectionPoint>   spConnectionPoint;
	//CComQIPtr<IConnectionPointContainer> spConnectionPointContainer(m_spWebBrowser2);
	//COM_VERIFY(spConnectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &spConnectionPoint));
	if (inAdvise)
	{
		//DWORD dwCookie = 0;
		COM_VERIFY(DispEventAdvise(m_spWebBrowser2));
		//COM_VERIFY(spConnectionPoint->Advise(static_cast<IDispatch*>(this), &dwCookie));
		//m_Cookies.push_back(dwCookie);
	}
	else
	{
		COM_VERIFY_EXCEPT1(DispEventUnadvise(m_spWebBrowser2), 0x80040200);
		//for (std::list<DWORD>::iterator iter = m_Cookies.begin(); iter != m_Cookies.end();
		//	++iter)
		//{
		//	COM_VERIFY(spConnectionPoint->Unadvise(*iter));
		//}
		//m_Cookies.clear();
	}
	return hr;
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

	spOleWindow->GetWindow(&m_hReBar);
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
	FTLTRACE(TEXT("CFanxianBand::SetSite, pUnkSite=0x%p\n"), pUnkSite);

	if (pUnkSite)
	{
		//过滤资源管理器(explorer.exe), 只处理IE(iexplore.exe)
		if (CFSystemUtil::IsSpecialProcessName(TEXT("explorer.exe"), NULL))
		{
			FTLTRACE(TEXT("IsSpecialProcessName return\n"));
			return E_FAIL;
		}

		FormatMessageBox(NULL, TEXT("SetSite"), MB_OK, TEXT("SetSite for 0x%p"), pUnkSite);

		m_spWebBrowser2 = pUnkSite;
		if (m_spWebBrowser2)
		{
			COM_VERIFY(RegisterEventHandler(TRUE));
		}
		m_spUnkSite = pUnkSite;
	}
	else
	{
		COM_VERIFY(RegisterEventHandler(FALSE));
	}
	if(SUCCEEDED(hr))
	{
		COM_VERIFY(__super::SetSite(pUnkSite));
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
	FTLTRACE(TEXT("Invoke, dispIdMember=%d(%s)\n"), dispIdMember, idspidInfo.GetConvertedInfo());
#else
	OutputDebugString(idspidInfo.GetConvertedInfo());
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
		CAtlStringW wstrURL;
		wstrURL.Format(L"%Ls", pvUrl->bstrVal );
		FTLTRACE(TEXT("OnBeforeNavigate2, %s\n"), wstrURL);
		int nPos = wstrURL.Find(L"baidu");
		if (nPos > 0)
		{
			CComBSTR bstrUrl(L"www.google.com");
			CComVariant varEmpty;
			*pvCancel = VARIANT_TRUE;
			COM_VERIFY(m_spWebBrowser2->Stop());
			COM_VERIFY(m_spWebBrowser2->Navigate(bstrUrl, &varEmpty, &varEmpty, &varEmpty, &varEmpty));

		}
		//if (wstrURL.Find(L"fishjam") > 0)
		{
			//SysFreeString(pvUrl->bstrVal);
			//pvUrl->bstrVal = SysAllocString(L"www.fishjam.com");
		}
	}

	return hr;
}