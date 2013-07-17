// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "../ComicService/ComicService_i.c"
#include "../ComicHelper/ComicHelper.h"

#include <ftlComDetect.h>

CMainDlg::CMainDlg()
{
	m_bHookInDll = FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	CoInitialize(NULL);
	return TRUE;
}

void CMainDlg::OnDestroy()
{
	if (m_bHookInDll)
	{
		m_bHookInDll = FALSE;
		SetApiUnHook();
	}
	if (m_spComicService)
	{
		m_spComicService.Release();
	}
	CoUninitialize();
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

void CMainDlg::OnBtnInitService(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(m_spComicService.CoCreateInstance(CLSID_ComicServiceObj, NULL, CLSCTX_LOCAL_SERVER));
	if (m_spComicService)
	{
		COM_DETECT_INTERFACE_FROM_REGISTER(m_spComicService);
	}
	BOOL bEnabled = SUCCEEDED(hr);
	::EnableWindow(GetDlgItem(IDC_BTN_INIT_SERVICE), !bEnabled);
	::EnableWindow(GetDlgItem(IDC_BTN_SERVICE_HOOK), bEnabled);
	::EnableWindow(GetDlgItem(IDC_BTN_SERVICE_UNHOOK), bEnabled);
}

void CMainDlg::OnBtnServiceHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	//CComQIPtr<IComicServiceObj> spComicServiceOb(m_spComicService);
	if (m_spComicService)
	{
		COM_VERIFY(m_spComicService->ProtectWnd((LONG)m_hWnd, RGB(255, 0, 0), CComBSTR(TEXT("Copy Protected"))));
	}
}

void CMainDlg::OnBtnServiceUnHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	//CComQIPtr<IComicServiceObj> spComicServiceOb(m_spComicService);
	if (m_spComicService)
	{
		COM_VERIFY(m_spComicService->UnProtectWnd((LONG)m_hWnd));
	}
}

void CMainDlg::OnBtnFinService(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_spComicService)
	{
		m_spComicService.Release();
		::EnableWindow(GetDlgItem(IDC_BTN_INIT_SERVICE), TRUE);
	}
}

void CMainDlg::OnBtnDllHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	m_bHookInDll = TRUE;
	SetApiHook(m_hWnd);
}

void CMainDlg::OnBtnDllUnHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	SetApiUnHook();
	m_bHookInDll = FALSE;
}
