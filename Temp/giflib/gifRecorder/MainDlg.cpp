// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CMainDlg::CMainDlg()
{
    m_strSavePath = TEXT("gifRecorder.gif");
    m_pGifMaker = NULL;
    m_nLeft = 200;
    m_nTop = 200;
    m_nWidth = 640;
    m_nHeight = 480;
    m_nFps = 10;
}

CMainDlg::~CMainDlg()
{

}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

    // center the dialog on the screen
	CenterWindow();
    DoDataExchange(FALSE);

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	return TRUE;
}

void CMainDlg::OnDestroy()
{
    if (m_pGifMaker)
    {
        m_pGifMaker->EndMakeGif(GetTickCount());
        delete m_pGifMaker;
        m_pGifMaker = NULL;
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

void CMainDlg::OnBtnChooseSavePath(UINT uNotifyCode, int nID, CWindow wndCtl)
{

}


void CMainDlg::OnBtnStartRecord(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    FTLASSERT(NULL == m_pGifMaker);
    
    
}

void CMainDlg::OnBtnStopRecord(UINT uNotifyCode, int nID, CWindow wndCtl)
{

}
