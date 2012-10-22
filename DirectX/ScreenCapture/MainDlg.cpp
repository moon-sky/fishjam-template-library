// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CMainDlg::CMainDlg()
{
	m_pAutoGdiPlus = NULL;
	m_pNRecordGraph = NULL;
}

CMainDlg::~CMainDlg()
{

}

int CMainDlg::OnNotifyEvent(long EventCode, LONG_PTR EventParam1,LONG_PTR EventParam2)
{
	return 0;
}

BOOL CMainDlg::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	m_pAutoGdiPlus = new FTL::CFAutoGdiplus();

	GetDlgItem(IDC_STATIC_PREVIEW).ModifyStyle(0, WS_CLIPCHILDREN);

	_SetButtonStatus(rcNoInit);

	return TRUE;
}

void CMainDlg::OnDestroy()
{
	HRESULT hr = E_FAIL;
	if (m_pNRecordGraph)
	{
		DX_VERIFY(m_pNRecordGraph->Uninit());
		SAFE_DELETE(m_pNRecordGraph);
	}
	SAFE_DELETE(m_pAutoGdiPlus);

	CoUninitialize();
}

BOOL CMainDlg::OnEraseBkgnd(CDCHandle dc)
{
	CRect rect;
	CWindow wndPreview = GetDlgItem(IDC_STATIC_PREVIEW);
	wndPreview.GetWindowRect(&rect);  //获取Video播放窗体的范围
	wndPreview.ScreenToClient(&rect);
	dc.ExcludeClipRect(&rect);    //排除视频的播放区域 -- 不要刷背景
	//SetMsgHandled(FALSE);
	return TRUE;
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_pNRecordGraph)
	{
		HRESULT hr = E_FAIL;
		LONGLONG llPosition = 0;
		DX_VERIFY(m_pNRecordGraph->GetCurrentPosition(&llPosition));
		CString strTimeFormat;
		strTimeFormat.Format(TEXT("%f"), (float)llPosition / UNITS);
		SetWindowText(strTimeFormat);
	}
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

VOID CMainDlg::_SetButtonStatus(RecordStatus status)
{
	BOOL bCanInint = TRUE;
	BOOL bCanStart = FALSE;
	BOOL bCanPauseResume = FALSE;
	BOOL bCanStop = FALSE;
	BOOL bCanUninit = FALSE;

	switch (status)
	{
	case rcNoInit:
		bCanInint = TRUE;
		bCanStart = FALSE;
		bCanPauseResume = FALSE;
		bCanStop = FALSE;
		bCanUninit = FALSE;
		break;
	case rcInited:
	case rcStopped:
		bCanInint = FALSE;
		bCanStart = TRUE;
		bCanPauseResume = FALSE;
		bCanStop = FALSE;
		bCanUninit = TRUE;
		break;
	case rcRunning:
		bCanInint = FALSE;
		bCanStart = FALSE;
		bCanPauseResume = TRUE;
		SetDlgItemText(IDC_BTN_PAUSE_RESUME, TEXT("Pause"));
		bCanStop = TRUE;
		bCanUninit = FALSE;
		break;
	case rcPaused:
		bCanInint = FALSE;
		bCanStart = FALSE;
		bCanPauseResume = TRUE;
		SetDlgItemText(IDC_BTN_PAUSE_RESUME, TEXT("Resume"));
		bCanStop = TRUE;
		bCanUninit = FALSE;
		break;
	default:
		ATLASSERT(FALSE);
		break;
	}

	::EnableWindow(GetDlgItem(IDC_BTN_INIT), bCanInint);
	::EnableWindow(GetDlgItem(IDC_BTN_START), bCanStart);
	::EnableWindow(GetDlgItem(IDC_BTN_PAUSE_RESUME), bCanPauseResume);
	::EnableWindow(GetDlgItem(IDC_BTN_STOP), bCanStop);
	::EnableWindow(GetDlgItem(IDC_BTN_UNINIT), bCanUninit);
}
//#include <ftlDirectx.h>
//#include "../DirectShow/TextMediaType.h"
void CMainDlg::OnInitCapture(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
/*
	FTL::CFDShowHardwareMgr hardwareMgr;
	DX_VERIFY(hardwareMgr.Refresh(&CLSID_LegacyAmFilterCategory));
	FTL::HardwareMonikerInfoContainer& container = hardwareMgr.GetAllHardwares();
	TCHAR szName[MAX_PATH] = {0};
	for (FTL::HardwareMonikerInfoContainer::iterator iter = container.begin();
		iter != container.end(); ++iter)
	{
		FTL::HardwareMonikerInfo* pMonikerInfo = *iter;
		if (pMonikerInfo->clsid == CLSID_NULL)
		{
			CComPtr<IBaseFilter> spBaseFilter;
			if(pMonikerInfo->pMoniker->BindToObject(NULL, NULL,IID_IBaseFilter,(void**)&spBaseFilter))
			{
				DX_VERIFY(spBaseFilter->GetClassID(&(pMonikerInfo->clsid)));
			}
		}
		CTextMediaType::CLSID2String(szName, _countof(szName), &(pMonikerInfo->clsid));
		TCHAR szClsid[40] = {0};
		StringFromGUID2(pMonikerInfo->clsid, szClsid, _countof(szClsid));
		FTLTRACE(TEXT("FriendlyName=%s, szName=%s, clsid=%s\n"), pMonikerInfo->wachFriendlyName, szName, szClsid);
		//pMonikerInfo->clsid
	}
	return;
*/	
	NRecordParam	param;
	//param.hWndOwner = m_hWnd;
	//param.hWndTarget = m_hWnd;
	param.hWndPreview = GetDlgItem(IDC_STATIC_PREVIEW);
	GetClientRect(&param.rcRecord);
	param.strAudioInputName = TEXT("");

	FTLASSERT(NULL == m_pNRecordGraph);
	if (!m_pNRecordGraph)
	{
		m_pNRecordGraph = new CNRecordGraph();
		DX_VERIFY(m_pNRecordGraph->Init(param, this));
	}

	//DX_VERIFY(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
	//	IID_IGraphBuilder, (void**)&m_spGraphBuilder));

	//DX_VERIFY(CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2,
	//	(void**)&m_spCaptureGraphBuilder));

	//DX_VERIFY(m_spCaptureGraphBuilder->SetFiltergraph(m_spGraphBuilder));

	//CComPtr<IBaseFilter> spVideoCapture;
	//COM_VERIFY(m_VideoInputDevices.GetBindObject(TEXT("Fishjam Screen Capture"), &spVideoCapture));

	//m_spGraphBuilder->AddFilter(spVideoCapture, TEXT("Fishjam Screen Capture"));

	//CComPtr<IAMVideoCompression> pVC;

	//hr = m_spCaptureGraphBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
	//	&MEDIATYPE_Interleaved, spVideoCapture,
	//	IID_IAMVideoCompression, (void **)&pVC);
	//if(hr != S_OK)
	//{
	//	hr = m_spCaptureGraphBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
	//		&MEDIATYPE_Video, spVideoCapture,
	//		IID_IAMVideoCompression, (void **)&pVC);
	//}

	_SetButtonStatus(rcInited);
}

void CMainDlg::OnStopWhenReady(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	MessageBox(TEXT("TODO: StopWhenReady"), NULL, MB_OK);
}

void CMainDlg::OnStartCapture(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	//FOURCCMap fccMap = mmioFOURCC('A','V','I',' '); 
	//TCHAR szfccMap[160] = {0};
	//TCHAR szAvi[160] = {0};
	//StringFromGUID2(static_cast<GUID>(fccMap), szfccMap, _countof(szfccMap));
	//StringFromGUID2(MEDIASUBTYPE_Avi, szAvi, _countof(szAvi));
	//if (MEDIASUBTYPE_Avi == static_cast<GUID>(fccMap))
	//{
	//	MessageBox(TEXT("Same"), TEXT(""), MB_OK);
	//}
	//CString strInfo;
	//strInfo.Format(TEXT("fcc=%s\r\navi=%s"), szfccMap, szAvi);
	//MessageBox(strInfo, TEXT(""), MB_OK);


	HRESULT hr = E_FAIL;
	DX_VERIFY(m_pNRecordGraph->Start());
	_SetButtonStatus(rcRunning);
	SetTimer(1, 1000, NULL);
}

void CMainDlg::OnPauseResumeCapture(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	if (m_pNRecordGraph->GetStatus() == rcPaused)
	{
		m_pNRecordGraph->Resume();
		_SetButtonStatus(rcRunning);
	}
	else
	{
		m_pNRecordGraph->Pause();
		_SetButtonStatus(rcPaused);
	}
}
void CMainDlg::OnStopCapture(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	DX_VERIFY(m_pNRecordGraph->Stop());
	_SetButtonStatus(m_pNRecordGraph->GetStatus());
	KillTimer(1);
}

void CMainDlg::OnUnInitCapture(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	DX_VERIFY(m_pNRecordGraph->Uninit());
	_SetButtonStatus(m_pNRecordGraph->GetStatus());

	SAFE_DELETE(m_pNRecordGraph);

}


