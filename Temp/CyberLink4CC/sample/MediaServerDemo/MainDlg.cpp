// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include <ftlShell.h>

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
        m_pMediaServer->loadPreferences(CAVMediaServer::DEFAULT_PREFERENCE_FILENAME);
	}
	m_pMediaServer->start();
}

void CMainDlg::OnBtnServerStopClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pMediaServer)
	{
		m_pMediaServer->stop();
        m_pMediaServer->savePreferences(CAVMediaServer::DEFAULT_PREFERENCE_FILENAME);
		delete m_pMediaServer;
		m_pMediaServer = NULL;
	}
}

void CMainDlg::OnBtnServerAddDir(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (m_pMediaServer)
	{
        FTL::CFDirBrowser dirBrowser;
        if (dirBrowser.DoModal())
        {
            std::string strUtf8Path;
            FTL::CFConversion convName;
            FTL::CFConversion convPath;
            m_pMediaServer->AddDirectory("share", 
                convPath.TCHAR_TO_MBCS(dirBrowser.GetSelectPath()));
        }
		//m_pMediaServer->DumpInfo();
	}
}

void CMainDlg::OnBtnPlayerStartClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if (!m_pMediaPlayer)
	{
		m_pMediaPlayer = new CAVMediaPlayer(this);
	}
    m_DevicesTree.SetControlPoint(m_pMediaPlayer);
	m_pMediaPlayer->start("urn:schemas-upnp-org:device:MediaRenderer:1");// "upnp:rootdevice");
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

LRESULT CMainDlg::OnRefreshDevice(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    m_DevicesTree.Refresh();
    return 0;
}

void CMainDlg::OnDeviceAdded(CyberLink::Device *dev)
{
    PostMessage(UM_REFRESH_DEVICE, 1, (LPARAM)dev);
}

void CMainDlg::OnDeviceRemoved(CyberLink::Device *dev)
{
    PostMessage(UM_REFRESH_DEVICE, 0, (LPARAM)dev);
}