// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include "../ComicService/ComicService_i.c"

#include <ftlComDetect.h>

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

void CMainDlg::OnInitService(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	COM_VERIFY(m_spComicService.CoCreateInstance(CLSID_ComicServiceObj, NULL, CLSCTX_LOCAL_SERVER));
	if (m_spComicService)
	{
		COM_DETECT_INTERFACE_FROM_REGISTER(m_spComicService);
	}
	BOOL bEnabled = SUCCEEDED(hr);
	::EnableWindow(GetDlgItem(IDC_BTN_HOOK), bEnabled);
	::EnableWindow(GetDlgItem(IDC_BTN_UNHOOK), bEnabled);
}

void CMainDlg::OnBtnHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	//CComQIPtr<IComicServiceObj> spComicServiceOb(m_spComicService);
	if (m_spComicService)
	{
		COM_VERIFY(m_spComicService->ProtectWnd((LONG)m_hWnd, RGB(255, 0, 0), CComBSTR(TEXT("Copy Protected"))));
	}
}

void CMainDlg::OnBtnUnHook(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	HRESULT hr = E_FAIL;
	//CComQIPtr<IComicServiceObj> spComicServiceOb(m_spComicService);
	if (m_spComicService)
	{
		COM_VERIFY(m_spComicService->UnProtectWnd((LONG)m_hWnd));
	}
}
