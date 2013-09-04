// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"
#include "../ComicHelper/ComicHelper.h"

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
    m_pProtectWndInfoFileMap = NULL;

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

    HRESULT hr = E_FAIL;
    COM_VERIFY(m_FileMap.OpenMapping(COMIC_PROTECT_WND_FILE_MAP_NAME, sizeof(ProtectWndInfoFileMap)));
    if (SUCCEEDED(hr))
    {
        m_pProtectWndInfoFileMap = (ProtectWndInfoFileMap*)m_FileMap.GetData();
        if (m_pProtectWndInfoFileMap)
        {
            EnableWindowProtected(GetCurrentProcessId(), m_pProtectWndInfoFileMap->hWndProtect);
        }
    }
    
    //FTL::CFSharedVariableT<
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

    if (NULL != m_pProtectWndInfoFileMap && NULL != m_pProtectWndInfoFileMap->hWndProtect)
    {
        DisableWindowProtected(m_pProtectWndInfoFileMap->hWndProtect);
        m_pProtectWndInfoFileMap->hWndProtect = NULL;
    }

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnUpdateProtectWnd(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    return 0;
}

void CMainFrame::OnPaint(CDCHandle /*dc*/)
{
    CPaintDC dc(m_hWnd);
    CAtlString strUpdateWndInfo;
    HWND hWnd = m_pProtectWndInfoFileMap ? m_pProtectWndInfoFileMap->hWndProtect : NULL;
    strUpdateWndInfo.Format(TEXT("UpdateWndInfo=0x%x"), hWnd);

    CRect rcClient;
    GetClientRect(&rcClient);
    dc.DrawText(strUpdateWndInfo, strUpdateWndInfo.GetLength(), &rcClient, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}