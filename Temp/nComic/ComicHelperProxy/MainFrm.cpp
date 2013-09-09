// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"
#include "ProtectEventMonitorThread.h"

CMainFrame::CMainFrame()
{
    m_pProtectEventMonitorThread = NULL;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

    m_pProtectEventMonitorThread = new CProtectEventMonitorThread(this);
    if (m_pProtectEventMonitorThread)
    {
        m_pProtectEventMonitorThread->Start();
    }

#if defined _M_IX86
    SetWindowText(TEXT("ComicHelperProxy32"));
#else
    SetWindowText(TEXT("ComicHelperProxy64"));
#endif 
    return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    BOOL bRet = FALSE;
    if (m_pProtectEventMonitorThread)
    {
        API_VERIFY(m_pProtectEventMonitorThread->Stop());
        delete m_pProtectEventMonitorThread;
        m_pProtectEventMonitorThread = NULL;
    }

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

void CMainFrame::OnPaint(CDCHandle /*dc*/)
{
    CPaintDC dc(m_hWnd);
    CAtlString strUpdateWndInfo;
    HWND hWndProtect = NULL;
    if (m_pProtectEventMonitorThread)
    {
        hWndProtect = m_pProtectEventMonitorThread->GetProtectWnd();
    }
    strUpdateWndInfo.Format(TEXT("UpdateWndInfo=0x%x"), hWndProtect);

    CRect rcClient;
    GetClientRect(&rcClient);
    dc.DrawText(strUpdateWndInfo, strUpdateWndInfo.GetLength(), &rcClient, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

