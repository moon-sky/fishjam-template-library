#include "StdAfx.h"
#include "GraphInfoPropertyPage.h"

CUnknown* CGraphInfoPropertyPage::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	ASSERT(phr);

	CGraphInfoPropertyPage *pNewGraphInfoPropertyPage = new CGraphInfoPropertyPage(lpunk, phr);
	if (pNewGraphInfoPropertyPage == NULL) 
	{
		if (phr)
			*phr = E_OUTOFMEMORY;
	}
	return pNewGraphInfoPropertyPage;
}

CGraphInfoPropertyPage::CGraphInfoPropertyPage(IUnknown * pUnk, HRESULT * phr)
: CBasePropertyPage(NAME("Graph Info Property"),pUnk,IDD_GRPAH_INFO_PAGE, IDS_GRAPH_INFO_PAGE)
{
}


void CGraphInfoPropertyPage::SetDirty(BOOL bDirty)
{
	m_bDirty = bDirty;
	if(m_pPageSite && m_bDirty)  //只在变为“Dirty”时通知Apply按钮有效,False时会自动实效？
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	}
}


HRESULT CGraphInfoPropertyPage::OnConnect(IUnknown *pUnknown)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pUnknown,E_POINTER);
	HRESULT hr = E_FAIL;
	COM_VERIFY(__super::OnConnect(pUnknown));	

	//COM_DETECT_INTERFACE_FROM_REGISTER(pUnknown);
	//COM_DETECT_INTERFACE_FROM_LIST(pUnknown);

	ASSERT(NULL == m_spFilter);
	DX_VERIFY(pUnknown->QueryInterface(IID_IBaseFilter, (LPVOID*)&m_spFilter));
	if (SUCCEEDED(hr))
	{
		ASSERT(m_spFilter);
	}

	return hr;
}

HRESULT CGraphInfoPropertyPage::OnDisconnect()
{
	HRESULT hr = S_OK;
	m_spFilter.Release();
	m_spFilterGraph.Release();
	hr = __super::OnDisconnect();
	
	return hr;   
}

HRESULT CGraphInfoPropertyPage::OnActivate()
{
	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;

	COM_VERIFY(__super::OnActivate());

	API_VERIFY(DoDataExchange(FALSE));

	FILTER_INFO thisFilterInfo = {0};
	COM_VERIFY(m_spFilter->QueryFilterInfo(&thisFilterInfo));
	m_spFilterGraph = thisFilterInfo.pGraph;
	thisFilterInfo.pGraph = NULL;

	if (m_spFilterGraph)
	{
		CComPtr<IEnumFilters> spEnumFilters;
		DX_VERIFY(m_spFilterGraph->EnumFilters(&spEnumFilters));
		if (spEnumFilters)
		{
			CComPtr<IBaseFilter> spFilter;
			while ((hr = spEnumFilters->Next(1, &spFilter, NULL)) == S_OK)
			{
				FILTER_INFO	filterInfo = {0};
				DX_VERIFY(spFilter->QueryFilterInfo(&filterInfo));
				m_listFilters.AddString(CW2T(filterInfo.achName));
				QueryFilterInfoReleaseGraph(filterInfo);
				spFilter.Release();
			}
			if (S_FALSE == hr)
			{
				//enum over
				hr = S_OK;
			}
		}
	}

	return S_OK;
}

HRESULT CGraphInfoPropertyPage::OnDeactivate()
{
	HRESULT hr = E_FAIL;
	m_listFilters.ResetContent();
	m_listFilters.Detach();

	COM_VERIFY(__super::OnDeactivate());
	return hr;
}