// FBhoObject.cpp : Implementation of CFBhoObject

#include "stdafx.h"
#include "FBhoObject.h"
#include <ExDispid.h>

#include <ftlbase.h>
#include <ftlCom.h>

// CFBhoObject

STDMETHODIMP CFBhoObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IFBhoObject
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CFBhoObject::RegisterEventHandler(BOOL inAdvise)
{
    HRESULT hr = E_FAIL;
    CComPtr<IConnectionPoint>   spConnectionPoint;
    CComQIPtr<IConnectionPointContainer> spConnectionPointContainer(m_spWebBrowser2);
    COM_VERIFY(spConnectionPointContainer->FindConnectionPoint(DIID_DWebBrowserEvents2, &spConnectionPoint));
    if (inAdvise)
    {
        DWORD dwCookie = 0;
        COM_VERIFY(spConnectionPoint->Advise(static_cast<IDispatch*>(this), &dwCookie));
        m_Cookies.push_back(dwCookie);
    }
    else
    {
        for (std::list<DWORD>::iterator iter = m_Cookies.begin(); iter != m_Cookies.end();
            ++iter)
        {
            COM_VERIFY(spConnectionPoint->Unadvise(*iter));
        }
        m_Cookies.clear();
    }
    return hr;
}

STDMETHODIMP CFBhoObject::SetSite(IUnknown *pUnkSite)
{
    HRESULT hr = E_POINTER;
    if (pUnkSite)
    {
        m_spWebBrowser2 = pUnkSite;
        if (m_spWebBrowser2)
        {
            hr = RegisterEventHandler(TRUE);
        }
    }
    return hr;
}

STDMETHODIMP CFBhoObject::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,
                  EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr)

{
    HRESULT hr = S_OK;
    FTL::CFIExplorerDispidInfo  idspidInfo(dispIdMember, pDispParams);

    FTLTRACE(TEXT("Invoke, dispIdMember=%d(%s)\n"), dispIdMember, idspidInfo.GetConvertedInfo());
    switch (dispIdMember)
    {
    case DISPID_NAVIGATECOMPLETE2:      //页面跳转完毕事件
        break;
    case DISPID_ONQUIT: //浏览器退出事件
        RegisterEventHandler(FALSE);  //注销BHO事件处理器
        break;
    }
    return hr;
}