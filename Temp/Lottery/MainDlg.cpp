// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CMainDlg::CMainDlg()
{
    m_szThumbnail.SetSize(100, 100);
    //m_strInitPath = _T("C:\\Users\\Public\\Pictures");
    m_pCalcRect = CFCalcRect::GetCalcRectObject(CFCalcRect::modeAutoFit);
    m_dwImageCount = 0;
}

CMainDlg::~CMainDlg()
{
    SAFE_DELETE(m_pCalcRect);
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

    DoDataExchange(DDX_LOAD);
	return TRUE;
}

void CMainDlg::OnDestroy()
{
    m_lotteryMgr.Stop();
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

void CMainDlg::OnBtnInitClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    CFDirBrowser dirBrowser(_T("Choose Picture Directory"), m_hWnd, m_strInitPath);
    if (dirBrowser.DoModal())
    {
        m_dwImageCount = 0;
        CRect rcPic;
        m_StaicPic.GetClientRect(&rcPic);
        m_strInitPath = dirBrowser.GetSelectPath();
        m_dwImageCount = m_lotteryMgr.Init(m_hWnd, rcPic.Size(), m_strInitPath);
        DoDataExchange(DDX_LOAD, IDC_STATIC_IMAGE_COUNT);
        SetWindowText(TEXT("Init Complete"));
        _SetButtonStatus(TRUE, FALSE, FALSE);
    }
}

void CMainDlg::OnBtnStartClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_lotteryMgr.Start(100);
    _SetButtonStatus(TRUE, TRUE, FALSE);
}

void CMainDlg::OnBtnPauseResumeClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_lotteryMgr.TogglePause();
    _SetButtonStatus(TRUE, TRUE, m_lotteryMgr.IsPaused());
}

void CMainDlg::OnBtnResetClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_lotteryMgr.Stop();
    m_lotteryMgr.ResetSelectionState();
    _SetButtonStatus(TRUE, FALSE, FALSE);
}

void CMainDlg::_SetButtonStatus(BOOL bInited, BOOL bStarted, BOOL bPaused)
{
    //GetDlgItem(IDC_BTN_INIT).EnableWindow(!bInited);
    GetDlgItem(IDC_BTN_START).EnableWindow(!bStarted);

    GetDlgItem(IDC_BTN_PAUSE_RESUME).EnableWindow(bInited && bStarted);
    GetDlgItem(IDC_BTN_PAUSE_RESUME).SetWindowText(bPaused ? TEXT("Resume") : _T("Pause"));

    GetDlgItem(IDC_BTN_RESET).EnableWindow(bInited && bStarted);
}

LRESULT CMainDlg::OnAddLotteryInfo(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;
    LotteryInfo* pInfo = (LotteryInfo*)wParam;
    FTLASSERT(pInfo);
    if (pInfo)
    {
        m_dwImageCount++;
        DoDataExchange(DDX_LOAD, IDC_STATIC_IMAGE_COUNT);

        CPath pathFileName(pInfo->strPicturePath);
        int nFileName =pathFileName.FindFileName();
        if (nFileName != -1)
        {
            SetWindowText(pInfo->strPicturePath.Mid(nFileName));
        }
    }
    return 0;
}

LRESULT CMainDlg::OnUpdateLotteryInfo(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BOOL bRet = FALSE;
    LotteryInfo* pInfo = (LotteryInfo*)wParam;
    FTLASSERT(pInfo);
    if (pInfo)
    {
        CRect rcPic;
        CSize szPic;

        m_StaicPic.GetClientRect(&rcPic);
        szPic.cx = pInfo->imgThumbnail.GetWidth();
        szPic.cy = pInfo->imgThumbnail.GetHeight();

        CRect rcDrawTarget = m_pCalcRect->GetFitRect(rcPic, szPic);

        CDC dcPic = m_StaicPic.GetDC();
        API_VERIFY(pInfo->imgThumbnail.Draw(dcPic.m_hDC, rcDrawTarget));

        CPath pathFileName(pInfo->strPicturePath);
        int nFileName =pathFileName.FindFileName();
        if (nFileName != -1)
        {
            SetWindowText(pInfo->strPicturePath.Mid(nFileName));
        }
    }
    return 0;
}