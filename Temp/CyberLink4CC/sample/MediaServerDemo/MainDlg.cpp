// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CMainDlg::CMainDlg()
{
	m_pMediaServer = NULL;
	m_pMediaPlayer = NULL;
}
CMainDlg::~CMainDlg()
{

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

    DoDataExchange(FALSE);
	return TRUE;
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

void CMainDlg::OnDestroy()
{
    FUNCTION_BLOCK_TRACE(100);
	if (m_pMediaPlayer)
	{
		m_pMediaPlayer->stop();
		delete m_pMediaPlayer;
		m_pMediaPlayer = NULL;
	}
	if (m_pMediaServer)
	{
		//m_pMediaServer->stop();
		delete m_pMediaServer;
		m_pMediaServer = NULL;
	}
}

void CMainDlg::OnBtnServerStartClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (!m_pMediaServer)
	{
		m_pMediaServer = new CAVMediaServer(this);
	}
	m_pMediaServer->start();
}

void CMainDlg::OnBtnServerStopClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pMediaServer)
	{
		m_pMediaServer->stop();
		delete m_pMediaServer;
		m_pMediaServer = NULL;
	}
}

void CMainDlg::OnBtnServerDumpInfo(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pMediaServer)
	{
		m_pMediaServer->DumpInfo();
	}
}

void CMainDlg::OnBtnPlayerStartClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (!m_pMediaPlayer)
	{
		m_pMediaPlayer = new CAVMediaPlayer(this);
	}
    m_DevicesTree.SetControlPoint(m_pMediaPlayer);
	m_pMediaPlayer->start("upnp:rootdevice");
}

void CMainDlg::OnBtnPlayerDumpInfo(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pMediaPlayer)
	{
		m_pMediaPlayer->DumpInfo();
	}
    m_DevicesTree.Refresh();
}

void CMainDlg::OnBtnPlayerStopClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pMediaPlayer)
	{
		m_pMediaPlayer->stop();
		delete m_pMediaPlayer;
		m_pMediaPlayer = NULL;
	}
}
