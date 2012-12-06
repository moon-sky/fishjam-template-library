#include "stdafx.h"
#include "DebugInfoFilterProperty.h"
#include "TextMediaType.h"

CUnknown* CDebugInfoFilterProperty::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
	ASSERT(phr);

	CDebugInfoFilterProperty *pNewDebugInfoProperty = new CDebugInfoFilterProperty(lpunk, phr);
	if (pNewDebugInfoProperty == NULL) 
	{
		if (phr)
			*phr = E_OUTOFMEMORY;
	}
	return pNewDebugInfoProperty;
}

CDebugInfoFilterProperty::CDebugInfoFilterProperty(IUnknown * pUnk, HRESULT * phr)
: CBasePropertyPage(NAME("Debug Info Filter Property"),pUnk,IDD_DEBUG_FILTER_PAGE,IDS_DEBUG_INFO_PAGE),
m_pDebugInfoFilter(NULL),
m_pInputConnectedPin(NULL),
m_pOutputConnectedPin(NULL),
m_nInputIndex(0),
m_nOutputIndex(0)
{
	ZeroMemory(&m_FilterDebugParam,sizeof(m_FilterDebugParam));
	//InitCommonControls();
}


void CDebugInfoFilterProperty::SetDirty(BOOL bDirty)
{
	m_bDirty = bDirty;
	if(m_pPageSite && m_bDirty)  //只在变为“Dirty”时通知Apply按钮有效,False时会自动实效？
	{
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
	}
}

HRESULT CDebugInfoFilterProperty::OnConnect(IUnknown *pUnknown)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pUnknown,E_POINTER);
	HRESULT hr = E_FAIL;
	COM_VERIFY(__super::OnConnect(pUnknown));	

	//COM_DETECT_INTERFACE_FROM_REGISTER(pUnknown);
	//COM_DETECT_INTERFACE_FROM_LIST(pUnknown);

	//IBaseFilter
	ASSERT(NULL == m_pDebugInfoFilter);
	DX_VERIFY(pUnknown->QueryInterface(IID_IDebugInfoFilter, (LPVOID*)&m_pDebugInfoFilter));
	if (SUCCEEDED(hr))
	{
		ASSERT(m_pDebugInfoFilter);
	}

	return hr;
}

HRESULT CDebugInfoFilterProperty::OnDisconnect()
{
	HRESULT hr = S_OK;
	hr = __super::OnDisconnect();
	SAFE_RELEASE(m_pDebugInfoFilter);
	return hr;   
}

HRESULT CDebugInfoFilterProperty::_DisplayPinInfo(IPin* pPin, HWND hwndListbox, HWND hwndInfo, int& nCurIndex )
{
	HRESULT hr = E_FAIL;
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(pPin,E_POINTER);
	CListBox	listBox(hwndListbox);
	CStatic		stInfo(hwndInfo);

	TCHAR szFilterName[260] = {0};

	PIN_INFO pinInfo = {0};
	DX_VERIFY(pPin->QueryPinInfo(&pinInfo));
	if (SUCCEEDED(hr))
	{
		CLSID clsidFilter;
		CComQIPtr<IPersist> spFilterPersist(pinInfo.pFilter);
		if (spFilterPersist)
		{
			DX_VERIFY_EXCEPT1(spFilterPersist->GetClassID(&clsidFilter), S_FALSE);
			if(S_OK != CTextMediaType::CLSID2String(szFilterName, _countof(szFilterName), &clsidFilter))
			{
				StringFromGUID2(clsidFilter, szFilterName, _countof(szFilterName));
			}
		}

		QueryPinInfoReleaseFilter(pinInfo);


		//CAtlString strPinInfo;
		//FILTER_INFO filterInfo = {0};
		//if (pinInfo.pFilter)
		//{
		//	DX_VERIFY(pinInfo.pFilter->QueryFilterInfo(&filterInfo));
		//	SAFE_RELEASE(pinInfo.pFilter);
		//}
		//strPinInfo.Format(TEXT("Dir: %s, PinName=%s, FilterName=%s"),
		//	PINDIR_INPUT == pinInfo.dir ? TEXT("Input") : TEXT("Output"),
		//	NULL != pinInfo.achName[0] ? CW2T(pinInfo.achName) : TEXT("NoName"),
		//	NULL != filterInfo.achName[0] ? CW2T(filterInfo.achName) : TEXT("NoName")
		//	);
		//::SetWindowText(hwndInfo, strPinInfo);
	}

	CMediaType ConnectionMediaType;
	COM_VERIFY(pPin->ConnectionMediaType(&ConnectionMediaType)); //得到Pin上连接的媒体类型

	CComPtr<IEnumMediaTypes> pEnumMediaTypes;
	COM_VERIFY(pPin->EnumMediaTypes(&pEnumMediaTypes));
	TCHAR bufMediaInfo[1024] = {0};
	FTL::CFStringFormater strFormater;
	if (S_OK == hr)
	{
		AM_MEDIA_TYPE *pMediaType= NULL;
		ULONG count= 0;
		DX_VERIFY_EXCEPT1(pEnumMediaTypes->Next(1,&pMediaType,&count), S_FALSE);
		int nIndex = 0;
		while (S_OK == hr)
		{
			DX_VERIFY(CTextMediaType(*pMediaType).AsText(bufMediaInfo,_countof(bufMediaInfo)));
			strFormater.Format(TEXT("%03d => %s"), nIndex + 1, bufMediaInfo);
			nIndex = listBox.AddString(strFormater.GetString());
			if (ConnectionMediaType == *pMediaType)
			{
				nCurIndex = nIndex;
			}
			listBox.SetItemDataPtr(nIndex, pMediaType);
			pMediaType = NULL;
			//DeleteMediaType(pMediaType);
			DX_VERIFY_EXCEPT1(pEnumMediaTypes->Next(1, &pMediaType, &count),S_FALSE);
			nIndex ++;
		}
		pEnumMediaTypes.Release();
		if (S_FALSE == hr) //当到最后一个时，返回S_FALSE
		{
			hr = S_OK;
		}
	}

	DWORD dwCount = ListBox_GetCount(hwndListbox);
	if(0 == dwCount)
	{
		//No IEnumMediaTypes or Next get Null
		DX_VERIFY(CTextMediaType(ConnectionMediaType).AsText(bufMediaInfo,_countof(bufMediaInfo)));
		strFormater.Format(TEXT("%03d => %s"), 1, bufMediaInfo);
		nCurIndex = listBox.AddString(strFormater.GetString());
		listBox.SetItemDataPtr(nCurIndex, CreateMediaType(&ConnectionMediaType));

		nCurIndex = 0;
	}
	SendMessage (hwndListbox, LB_SETCURSEL, nCurIndex, 0);
	FTL::CFControlUtil::UpdateListboxHorizontalExtent(hwndListbox, 4);
	
	CString strFilterInfo;
	strFilterInfo.Format(TEXT("Name= %s , ActiveIndex= %03d"), szFilterName, nCurIndex + 1);
	stInfo.SetWindowText(strFilterInfo);

	return hr;
}

HRESULT CDebugInfoFilterProperty::_ClearDisplayPinInfo(HWND hwndListbox)
{
	CListBox	listBox(hwndListbox);
	int nCount = listBox.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		AM_MEDIA_TYPE* pMediaType = static_cast<AM_MEDIA_TYPE*> (listBox.GetItemDataPtr(i));
		if (pMediaType)
		{
			DeleteMediaType(pMediaType);
			pMediaType = NULL;
			//FreeMediaType(*pMediaType);
		}
	}
	listBox.ResetContent();
	return S_OK;
}

HRESULT CDebugInfoFilterProperty::OnActivate()
{
	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;

	COM_VERIFY(__super::OnActivate());

	API_VERIFY(DoDataExchange(FALSE));

	if (NULL == m_pInputConnectedPin)
	{
		DX_VERIFY_EXCEPT1(m_pDebugInfoFilter->GetConnectedPin(TRUE,&m_pInputConnectedPin),VFW_E_NOT_CONNECTED);
	}

	if (SUCCEEDED(hr) && m_pInputConnectedPin)
	{
		DX_VERIFY(_DisplayPinInfo(m_pInputConnectedPin, m_listInput, m_wndInputInfo,
			m_nInputIndex));
	}
	if (NULL == m_pOutputConnectedPin)
	{
		DX_VERIFY_EXCEPT1(m_pDebugInfoFilter->GetConnectedPin(FALSE,&m_pOutputConnectedPin),VFW_E_NOT_CONNECTED);
	}
	if (SUCCEEDED(hr) && m_pOutputConnectedPin)
	{
		DX_VERIFY(_DisplayPinInfo(m_pOutputConnectedPin,
			m_listOutput, m_wndOutputInfo, m_nOutputIndex));
	}

	DX_VERIFY(m_pDebugInfoFilter->GetFilterDebugParam(&m_FilterDebugParam));

	API_VERIFY(CheckDlgButton(m_hwnd,IDC_CHECK_DUMP,m_FilterDebugParam.bDumpSample));
	EnableWindow(GetDlgItem(IDC_EDIT_START_INDEX),m_FilterDebugParam.bDumpSample);
	EnableWindow(GetDlgItem(IDC_BTN_CHOOSE_DUMP_FILE_PATH),m_FilterDebugParam.bDumpSample);
	EnableWindow(GetDlgItem(IDC_SPIN_LIMIT_COUNT),m_FilterDebugParam.bDumpSample);
	EnableWindow(GetDlgItem(IDC_EDIT_LIMIT_COUNT),m_FilterDebugParam.bDumpSample);

	API_VERIFY(SetDlgItemInt(m_hwnd,IDC_EDIT_START_INDEX,
		m_FilterDebugParam.dwDumpSampleStartIndex,FALSE));
	API_VERIFY(SetDlgItemInt(m_hwnd,IDC_EDIT_LIMIT_COUNT,
		m_FilterDebugParam.dwDumpSampleLimitCount,FALSE));
	if (m_FilterDebugParam.pstrDumpFilePath && SysStringLen(m_FilterDebugParam.pstrDumpFilePath) > 0)
	{
		USES_CONVERSION;
		API_VERIFY(SetDlgItemText(m_hwnd,IDC_EDIT_DUMP_FILE_PATH,
			OLE2T(m_FilterDebugParam.pstrDumpFilePath)));
	}

	FilterRunningInfo runningInfo;
	runningInfo.dwSampleCount = 0;
	runningInfo.llElapse = 0;
	DX_VERIFY(m_pDebugInfoFilter->GetFilterRunningInfo(&runningInfo));
	if (SUCCEEDED(hr))
	{
		SetDlgItemInt(m_hwnd, IDC_STATIC_SAMPLE_COUNT, runningInfo.dwSampleCount, FALSE);
		CString strRunningTime;
		strRunningTime.Format(TEXT("%.3f"), (float)runningInfo.llElapse/1000000);
		SetDlgItemText(m_hwnd, IDC_STATIC_RUNNING_TIME, strRunningTime);
	}
	return S_OK;
}

HRESULT CDebugInfoFilterProperty::OnDeactivate()
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(_ClearDisplayPinInfo(m_listInput));
	COM_VERIFY(_ClearDisplayPinInfo(m_listOutput));

	m_listInput.Detach();
	m_listOutput.Detach();
	m_wndInputInfo.Detach();
	m_wndOutputInfo.Detach();

	SAFE_RELEASE(m_pInputConnectedPin);
	SAFE_RELEASE(m_pOutputConnectedPin);
	SAFE_FREE_BSTR(m_FilterDebugParam.pstrDumpFilePath);
	
	COM_VERIFY(__super::OnDeactivate());
	return hr;
}

HRESULT CDebugInfoFilterProperty::_ReConnectUseMediaType(AM_MEDIA_TYPE* pMediaType)
{
	HRESULT hr = E_FAIL;
	//use change input media type
	//FTLASSERT(spFilter);
	//if (spFilter)
	{
		DX_VERIFY(m_pDebugInfoFilter->SetAcceptMediaType(pMediaType));
		if (SUCCEEDED(hr))
		{
			CComPtr<IPin> spFilterInputPin;
			CComPtr<IPin> spFilterOutputPin;

			if (m_pInputConnectedPin)
			{
				DX_VERIFY(m_pInputConnectedPin->ConnectedTo(&spFilterInputPin));
				if(spFilterInputPin)
				{
					DX_VERIFY(m_pInputConnectedPin->Disconnect());
				}
				DX_VERIFY(spFilterInputPin->Disconnect());
			}
			if (FAILED(hr))
			{
				return hr;
			}
			
			if (m_pOutputConnectedPin)
			{
				DX_VERIFY(m_pOutputConnectedPin->ConnectedTo(&spFilterOutputPin));
				if (spFilterOutputPin)
				{
					DX_VERIFY(spFilterOutputPin->Disconnect());
				}
				DX_VERIFY(m_pOutputConnectedPin->Disconnect());
			}
			
			if (FAILED(hr))
			{
				return hr;
			}
#if 0
			if (m_pInputConnectedPin)
			{
				if (spFilterInputPin)
				{
					//DX_VERIFY(spLastOutput->Disconnect());
					DX_VERIFY(m_pInputConnectedPin->Connect(spFilterInputPin, pMediaType));
				}
			}
			if (m_pOutputConnectedPin)
			{
				if (spFilterOutputPin)
				{
					//DX_VERIFY(spLastOutput->Disconnect());
					DX_VERIFY(spFilterOutputPin->Connect(m_pOutputConnectedPin, pMediaType));
				}
			}

#else
			CComQIPtr<IBaseFilter> spFilter(m_pDebugInfoFilter);
			FILTER_INFO filterInfo = {0};
			DX_VERIFY(spFilter->QueryFilterInfo(&filterInfo));
			if (SUCCEEDED(hr))
			{
				CComQIPtr<IFilterGraph2> spFilterGraph2(filterInfo.pGraph);
				if(spFilterInputPin)
				{
					DX_VERIFY(spFilterGraph2->ConnectDirect(m_pInputConnectedPin, spFilterInputPin, pMediaType));
				}
				if (spFilterOutputPin)
				{
					DX_VERIFY(spFilterGraph2->ConnectDirect(spFilterOutputPin, m_pOutputConnectedPin, pMediaType));
				}
				QueryFilterInfoReleaseGraph(filterInfo);
			}
#endif
		}
	}
	return hr;
}

HRESULT CDebugInfoFilterProperty::_CheckAndReconnectUseMediaType()
{
	HRESULT hr = S_OK;
	int nNewInputSelIndex = m_listInput.GetCurSel();
	if(m_nInputIndex != nNewInputSelIndex)
	{
		AM_MEDIA_TYPE* pNewMediaType = static_cast<AM_MEDIA_TYPE*> (m_listInput.GetItemDataPtr(nNewInputSelIndex));
		DX_VERIFY(_ReConnectUseMediaType(pNewMediaType));
		if (FAILED(hr))
		{
			HRESULT hrError = hr;
			FormatMessageBox(m_hwnd, TEXT("Error"),
				MB_OK, TEXT("Use New MediaType Connect Failed(0x%x)\r\n%s"), 
				hr, FTL::CFDirectXErrorInfo(hr).GetConvertedInfo());
			//AM_MEDIA_TYPE* pOldMediaType = static_cast<AM_MEDIA_TYPE*> (m_listInput.GetItemDataPtr(m_nInputIndex));
			//DX_VERIFY(_ReConnectUseMediaType(pOldMediaType));
			return hrError;
		}
	}

	return hr;
}

HRESULT CDebugInfoFilterProperty::OnApplyChanges()
{
	HRESULT hr = E_FAIL;
	DX_VERIFY(_CheckAndReconnectUseMediaType());
	if (FAILED(hr))
	{
		return hr;
	}

	TCHAR bufDumpFilePath[MAX_PATH] = {0};
	if (GetDlgItemText(m_hwnd,IDC_EDIT_DUMP_FILE_PATH,bufDumpFilePath,_countof(bufDumpFilePath)) > 0)
	{
		SAFE_FREE_BSTR(m_FilterDebugParam.pstrDumpFilePath);
		m_FilterDebugParam.pstrDumpFilePath = T2BSTR(bufDumpFilePath);
	}
	m_FilterDebugParam.bDumpSample = IsDlgButtonChecked(m_hwnd,IDC_CHECK_DUMP);
	m_FilterDebugParam.dwDumpSampleStartIndex = GetDlgItemInt(m_hwnd,IDC_EDIT_START_INDEX, NULL, FALSE);
	m_FilterDebugParam.dwDumpSampleLimitCount = GetDlgItemInt(m_hwnd,IDC_EDIT_LIMIT_COUNT,NULL,FALSE);

	DX_VERIFY(m_pDebugInfoFilter->SetFilterDebugParam(&m_FilterDebugParam));
	if (SUCCEEDED(hr))
	{
		DX_VERIFY(CBasePropertyPage::OnApplyChanges());
	}
	return hr;
}

void CDebugInfoFilterProperty::_OnCommand(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	BOOL bRet = FALSE;
	if ((IDC_LIST_INPUT == nID) || IDC_LIST_OUTPUT == nID)
	{
		SetDirty(TRUE);
	}
	else if (IDC_BTN_CHOOSE_DUMP_FILE_PATH == nID)
	{
		CFileDialog dlg(FALSE,TEXT("ssf"),NULL,
			OFN_EXPLORER | OFN_HIDEREADONLY | OFN_ENABLESIZING| OFN_OVERWRITEPROMPT,
			TEXT("Struct Storage File(*.ssf)\0*.ssf\0All Files(*.*)\0*.*\0"),
			m_hwnd);
		dlg.m_ofn.lpstrTitle = _T("Select where to output the dump file");
		if (dlg.DoModal() == IDOK)
		{
			API_VERIFY(SetDlgItemText(m_hwnd,IDC_EDIT_DUMP_FILE_PATH,dlg.m_szFileName));
			SetDirty(TRUE);
		}
	}
	else if(IDC_CHECK_DUMP == nID)
	{
		SetDirty(TRUE);
		BOOL bChecked = IsDlgButtonChecked(m_hwnd,IDC_CHECK_DUMP);
		EnableWindow(GetDlgItem(IDC_BTN_CHOOSE_DUMP_FILE_PATH),bChecked);
		EnableWindow(GetDlgItem(IDC_EDIT_START_INDEX),bChecked);
		EnableWindow(GetDlgItem(IDC_SPIN_LIMIT_COUNT),bChecked);
		EnableWindow(GetDlgItem(IDC_EDIT_LIMIT_COUNT),bChecked);
	}
	else if (IDC_SPIN_LIMIT_COUNT == nID)
	{
		SetDirty(TRUE);
	}
	else if(IDC_EDIT_START_INDEX == nID && EN_CHANGE == uNotifyCode)
	{
		SetDirty(TRUE);
	}
	else if (IDC_EDIT_LIMIT_COUNT == nID  && EN_CHANGE == uNotifyCode)
	{
		SetDirty(TRUE);
	}
	SetMsgHandled(FALSE);

}

void CDebugInfoFilterProperty::_OnBtnResetRunningInfoClicked(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	FilterRunningInfo runningInfo;
	runningInfo.dwSampleCount = 0;
	runningInfo.llElapse = 0;
	DX_VERIFY(m_pDebugInfoFilter->SetFilterRunningInfo(&runningInfo));

	//refresh running info
	DX_VERIFY(m_pDebugInfoFilter->GetFilterRunningInfo(&runningInfo));
	if (SUCCEEDED(hr))
	{
		SetDlgItemInt(m_hwnd, IDC_STATIC_SAMPLE_COUNT, runningInfo.dwSampleCount, FALSE);
		CString strRunningTime;
		strRunningTime.Format(TEXT("%.3f"), (float)runningInfo.llElapse/1000000);
		SetDlgItemText(m_hwnd, IDC_STATIC_RUNNING_TIME, strRunningTime);
	}

	//DX_VERIFY(m_pDebugInfoFilter->ResetCurrentFrameCount());
	//LONG nCount = m_pDebugInfoFilter->GetCurrentFrameCount();
	//SetDlgItemInt(m_hwnd, IDC_STATIC_FRAME_COUNT, nCount, FALSE);
}

INT_PTR CDebugInfoFilterProperty::OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
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

void CDebugInfoFilterProperty::_OnContextMenu(CWindow wnd, CPoint point)
{
	BOOL bRet = TRUE;
	if (wnd.IsWindow())
	{
		if (wnd.m_hWnd == m_listInput.m_hWnd || wnd.m_hWnd == m_listOutput.m_hWnd)
		{
			CMenu menuMain;
			API_VERIFY(menuMain.LoadMenu(MAKEINTRESOURCE(IDR_MENU_CONTEXT)));
			CMenuHandle menuContext = menuMain.GetSubMenu(0);
			API_VERIFY(menuContext.TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN, point.x, point.y, m_hwnd, NULL ));
		}
	}
}

void CDebugInfoFilterProperty::_OnMenuCopyClicked(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	BOOL bRet = FALSE;
	CPoint ptCursor(0, 0);
	CString strSelect;
	API_VERIFY(GetCursorPos(&ptCursor));
	if (bRet)
	{
		CListBox* pListBox = NULL;
		HWND hWndList = WindowFromPoint(ptCursor);
		if (hWndList == m_listInput.m_hWnd)
		{
			pListBox = &m_listInput;
		}
		else if(hWndList == m_listOutput.m_hWnd)
		{
			pListBox = &m_listOutput;
		}
		if (pListBox)
		{
			int nCurSelect = pListBox->GetCurSel();
			if (nCurSelect != -1)
			{
				pListBox->GetText(nCurSelect, strSelect);
			}
		}
	}
	if (strSelect)
	{
		API_VERIFY(OpenClipboard(m_hwnd));
		if (bRet)
		{
			EmptyClipboard();
			int cch = strSelect.GetLength() ;
			HGLOBAL hglbCopy = GlobalAlloc(GMEM_FIXED,(cch + 1) * sizeof(TCHAR)); 
			if (hglbCopy) 
			{ 
				LPTSTR  lptstrCopy = (LPTSTR)GlobalLock(hglbCopy); 
				CopyMemory(lptstrCopy,  (LPCTSTR)strSelect, cch * sizeof(TCHAR)); 
				lptstrCopy[cch] = (TCHAR)0;// null character 
				GlobalUnlock(hglbCopy);
#ifdef _UNICODE
				API_VERIFY(NULL != SetClipboardData(CF_UNICODETEXT, hglbCopy));
#else
				API_VERIFY(NULL != SetClipboardData(CF_TEXT, hglbCopy));
#endif 
				
				GlobalFree(hglbCopy);
			} 
			API_VERIFY(CloseClipboard());
		}
	}
	//MessageBox(m_hwnd, TEXT("_OnMenuCopyClicked"), TEXT("aaa"), MB_OK);
}

//////////////////////////////////////////////////////////////////////////

