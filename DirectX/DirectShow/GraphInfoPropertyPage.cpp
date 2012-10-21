#include "StdAfx.h"
#include "GraphInfoPropertyPage.h"
#include "TextMediaType.h"

const int LIST_BOX_MARGIN = 4;

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
: CBasePropertyPage(NAME("Graph Info Property"), 
					pUnk,
					IDD_GRPAH_INFO_PAGE, 
					IDS_GRAPH_INFO_PAGE)
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
	ASSERT(NULL == m_spFilterGraph);

	DX_VERIFY(pUnknown->QueryInterface(IID_IBaseFilter, (LPVOID*)&m_spFilter));
	if (SUCCEEDED(hr))
	{
		FILTER_INFO thisFilterInfo = {0};
		COM_VERIFY(m_spFilter->QueryFilterInfo(&thisFilterInfo));
		m_spFilterGraph = thisFilterInfo.pGraph;
		QueryFilterInfoReleaseGraph(thisFilterInfo);
	}

	return hr;
}

HRESULT CGraphInfoPropertyPage::OnDisconnect()
{
	HRESULT hr = S_OK;
	m_spFilter.Release();
	m_spFilterGraph.Release();
	COM_VERIFY(__super::OnDisconnect());
	
	return hr;   
}

HRESULT CGraphInfoPropertyPage::OnActivate()
{
	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;

	DX_VERIFY(__super::OnActivate());
	API_VERIFY(DoDataExchange(FALSE));
	DX_VERIFY(_GetFilterList());

	return hr;
}

HRESULT CGraphInfoPropertyPage::OnDeactivate()
{
	HRESULT hr = E_FAIL;

	_ClearInterfaceList();
	_ClearMediaTypeList();
	_ClearPinList();
	_ClearFilterList();

	m_listInterfaces.Detach();
	m_listMediaTypes.Detach();
	m_listPins.Detach();
	m_listFilters.Detach();

	COM_VERIFY(__super::OnDeactivate());
	return hr;
}


INT_PTR CGraphInfoPropertyPage::OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//LRESULT lResult = 0;
	INT_PTR nRet = 0;
	BOOL bRet = FALSE;
	LRESULT lResult = 0;
	bRet = ProcessWindowMessage(hwnd, uMsg, wParam, lParam, lResult, 0);
	if (!bRet)
	{
		nRet = __super::OnReceiveMessage(hwnd,uMsg,wParam,lParam);
	}
	return nRet;
}


HRESULT CGraphInfoPropertyPage::_ClearFilterList()
{
	FTLASSERT(m_listFilters.IsWindow());
	if (m_listFilters.IsWindow())
	{
		int nCount = m_listFilters.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			IBaseFilter* pFilter = (IBaseFilter*)m_listFilters.GetItemDataPtr(i);
			if (pFilter)
			{
				pFilter->Release();
				m_listFilters.SetItemDataPtr(i, NULL);
			}
		}
		m_listFilters.ResetContent();
	}

	return S_OK;
}

HRESULT CGraphInfoPropertyPage::_GetFilterList()
{
	//ASSERT((m_spFilterGraph != NULL));

	HRESULT hr = S_OK;
	DX_VERIFY(_ClearFilterList());

	if (m_spFilterGraph)
	{
		CComPtr<IEnumFilters> spEnumFilters;
		DX_VERIFY(m_spFilterGraph->EnumFilters(&spEnumFilters));
		if (spEnumFilters)
		{
			IBaseFilter* pFilter = NULL;
			TCHAR szFilterClsidInfo[128] = {0};
			while ((hr = spEnumFilters->Next(1, &pFilter, NULL)) == S_OK)
			{
				CLSID clsidFilter = CLSID_NULL;
				DX_VERIFY(pFilter->GetClassID(&clsidFilter));
				szFilterClsidInfo[0] = TEXT('\0');
				if(FAILED(CTextMediaType::CLSID2String(szFilterClsidInfo, _countof(szFilterClsidInfo), &clsidFilter)))
				{
					StringFromGUID2(clsidFilter, szFilterClsidInfo, _countof(szFilterClsidInfo));
				}

				FILTER_INFO	filterInfo = {0};
				DX_VERIFY(pFilter->QueryFilterInfo(&filterInfo));
				
				CString strFilterInfo;
				strFilterInfo.Format(TEXT("%s : %s"), szFilterClsidInfo, filterInfo.achName);
				int nIndex = m_listFilters.AddString(strFilterInfo);
				QueryFilterInfoReleaseGraph(filterInfo);
				if (nIndex >= 0)
				{
					m_listFilters.SetItemDataPtr(nIndex, pFilter);
				}
				else
				{
					pFilter->Release();
				}
				pFilter = NULL;
			}
			if (S_FALSE == hr)
			{
				//enum over
				hr = S_OK;
			}
		}
	}

	FTL::CFControlUtil::UpdateListboxHorizontalExtent(m_listFilters, LIST_BOX_MARGIN);
	if (m_listFilters.GetCount() > 0)
	{
		m_listFilters.SetCurSel(0);
	}
	
	return hr;
}


HRESULT CGraphInfoPropertyPage::_ClearPinList()
{
	FTLASSERT(m_listPins.IsWindow());
	if (m_listPins.IsWindow())
	{
		int nCount = m_listPins.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			IPin* pPin = (IPin*)m_listPins.GetItemDataPtr(i);
			if (pPin)
			{
				pPin->Release();
				m_listPins.SetItemDataPtr(i, NULL);
			}
		}
		m_listPins.ResetContent();
	}

	return S_OK;
}

HRESULT CGraphInfoPropertyPage::_GetPinList(IBaseFilter* pFilter)
{
	HRESULT hr = E_FAIL;
	FTLASSERT(pFilter);
	CComPtr<IEnumPins> spEnumPins;
	if (pFilter)
	{
		DX_VERIFY(pFilter->EnumPins(&spEnumPins));
	}
	if (spEnumPins)
	{
		IPin* pPin = NULL;
		while((hr = spEnumPins->Next(1, &pPin, NULL)) == S_OK)
		{
			PIN_INFO pinInfo = {0};
			DX_VERIFY(pPin->QueryPinInfo(&pinInfo));
			QueryPinInfoReleaseFilter(pinInfo);
			CString strPinInfo;
			strPinInfo.Format(TEXT("%s:%s"), 
				pinInfo.dir == PINDIR_INPUT ? TEXT("In") : TEXT("Out"),
				CW2T(pinInfo.achName));
			int nIndex = m_listPins.AddString(strPinInfo);
			if (nIndex >= 0)
			{
				m_listPins.SetItemDataPtr(nIndex, pPin);
			}
			else
			{
				pPin->Release();
			}
			pPin = NULL;
		}

		if (S_FALSE == hr)
		{	
			//enum over
			hr = S_OK;
		}
	}
	if (m_listPins.GetCount() > 0)
	{
		m_listPins.SetCurSel(0);
		FTL::CFControlUtil::UpdateListboxHorizontalExtent(m_listPins, LIST_BOX_MARGIN);
	}
	return hr;
}

void CGraphInfoPropertyPage::OnListFilterSelChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = S_OK;
	DX_VERIFY(_ClearPinList());


	int nSelectFilter = m_listFilters.GetCurSel();
	if (nSelectFilter >= 0)
	{
		IBaseFilter* pFilter = (IBaseFilter*)m_listFilters.GetItemDataPtr(nSelectFilter);
		if (pFilter)
		{
			_GetPinList(pFilter);
			_GetInterfaceList(pFilter);
		}
	}
}

HRESULT CGraphInfoPropertyPage::_ClearMediaTypeList()
{
	FTLASSERT(m_listMediaTypes.IsWindow());
	if (m_listMediaTypes.IsWindow())
	{
		int nCount = m_listMediaTypes.GetCount();
		for (int i = 0; i < nCount; i++)
		{
			AM_MEDIA_TYPE* pMediaType = (AM_MEDIA_TYPE*)m_listMediaTypes.GetItemDataPtr(i);
			if (pMediaType)
			{
				DeleteMediaType(pMediaType);				
				m_listMediaTypes.SetItemDataPtr(i, NULL);
			}
		}
		m_listMediaTypes.ResetContent();
	}

	return S_OK;
}

HRESULT CGraphInfoPropertyPage::_GetMediaTypeList(IPin* pPin)
{
	HRESULT hr = S_OK;
	CComPtr<IEnumMediaTypes> spEnumMediaTypes;

	if (pPin)
	{
		DX_VERIFY(pPin->EnumMediaTypes(&spEnumMediaTypes));
	}
	if (spEnumMediaTypes)
	{
		AM_MEDIA_TYPE* pMediaType = NULL;
		TCHAR szMediaInfo[1024] = {0};
		while((hr = spEnumMediaTypes->Next(1, &pMediaType, NULL)) == S_OK)
		{
			CTextMediaType mediaTypeText(*pMediaType);
			DX_VERIFY(mediaTypeText.AsText(szMediaInfo, _countof(szMediaInfo)));
			int nIndex = m_listMediaTypes.AddString(szMediaInfo);
			if (nIndex >= 0)
			{
				m_listMediaTypes.SetItemDataPtr(nIndex, pMediaType);
			}
			else
			{
				DeleteMediaType(pMediaType);
			}
			pMediaType = NULL;
		}

		if (S_FALSE == hr)
		{	
			//enum over
			hr = S_OK;
		}
	}
	FTL::CFControlUtil::UpdateListboxHorizontalExtent(m_listMediaTypes, LIST_BOX_MARGIN);
	return hr;
}

void CGraphInfoPropertyPage::OnListPinSelChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	DX_VERIFY(_ClearMediaTypeList());
	int nSelectPin = m_listPins.GetCurSel();
	if (nSelectPin >= 0)
	{
		IPin* pPin = (IPin*)m_listPins.GetItemDataPtr(nSelectPin);
		if (pPin)
		{
			_GetMediaTypeList(pPin);
			_GetInterfaceList(pPin);
		}
	}
}


HRESULT CGraphInfoPropertyPage::_ClearInterfaceList()
{
	HRESULT hr = E_FAIL;
	FTLASSERT(m_listInterfaces.IsWindow());
	if (m_listInterfaces.IsWindow())
	{
		//int nCount = m_listInterfaces.GetCount();
		//for (int i = 0; i < nCount; i++)
		//{
		//}
		m_listInterfaces.ResetContent();
	}

	return S_OK;

	return hr;
}

HRESULT CGraphInfoPropertyPage::OnDetectInterfaceCallBack(DWORD_PTR pParam, IUnknown* pUnknwon, REFIID checkedRIIF, LPCTSTR pszInterfaceName)
{
	if (pParam)
	{
		CGraphInfoPropertyPage* pThis = reinterpret_cast<CGraphInfoPropertyPage*>(pParam);
		pThis->m_listInterfaces.AddString(pszInterfaceName);
	}
	return S_OK;
}


HRESULT CGraphInfoPropertyPage::_GetInterfaceList(IUnknown* pUnknown)
{
	HRESULT hr = E_FAIL;
	_ClearInterfaceList();
	if (pUnknown)
	{
		FTL::CFComDetect::CoDetectInterfaceFromList(pUnknown, GUID_NULL, CFComDetect::cdtInterface,
			OnDetectInterfaceCallBack, reinterpret_cast<DWORD_PTR>(this));

		FTL::CFControlUtil::UpdateListboxHorizontalExtent(m_listMediaTypes, LIST_BOX_MARGIN);
	}
	return hr;
}