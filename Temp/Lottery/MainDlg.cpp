// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
const INT IMAGE_WIDTH = 128;
const INT IMAGE_HEIGHT = 128;

CMainDlg::CMainDlg()
{
    m_szThumbnail.SetSize(100, 100);
#ifdef _DEBUG
    //m_strInitPath = _T("C:\\Users\\Public\\Pictures\\Sample Pictures");
    m_strInitPath = _T("G:\\FJCODE_GOOGLE\\Temp\\Lottery\\res");
#endif 
    m_pCalcRect = CFCalcRect::GetCalcRectObject(CFCalcRect::modeAutoFit);
    m_dwImageCount = 0;
}

CMainDlg::~CMainDlg()
{
    SAFE_DELETE(m_pCalcRect);
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    BOOL bRet = FALSE;
	// center the dialog on the screen
	CenterWindow();
    DlgResize_Init();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

    DoDataExchange(DDX_LOAD);

    //COLORREF	crBtnColor;
    // Calculate a color effect for high-lighting the button
    //crBtnColor = ::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);

    //m_btnStart.SetBitmaps(IDB_PNG_ON, crBtnColor);
    //CRect rcBtnStart;
    //m_btnStart.GetClientRect(&rcBtnStart);
    //rcBtnStart.right =  rcBtnStart.left + 128;
    //rcBtnStart.bottom = rcBtnStart.top + 128;
    //m_btnStart.SetWindowPos(NULL, rcBtnStart, SWP_NOMOVE | SWP_NOZORDER);

    //m_btnStart.SetWindowText(TEXT(""));
    //m_btnStart.SetIcon(IDI_ICON1,IDI_ICON2);
    //m_btnStart.SetFlat(true);
    //m_btnStart.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
    //m_btnStart.SetTooltipText(TEXT("抽奖"));

    API_VERIFY(FTL::CFGdiUtil::LoadPNGFromResource(m_pImage, _Module.GetResourceInstance(), IDB_PNG_START_ALL));

    API_VERIFY(m_imgList.Create(IMAGE_WIDTH, IMAGE_HEIGHT , ILC_COLOR32, 4, 1));
    //API_VERIFY(m_imgList.Create(IDB_PNG_START_ALL, IMAGE_WIDTH, 1, CLR_NONE));
    for (INT i = IDB_PNG2; i < IDB_PNG5; i++)
    {
        CImage image;
        API_VERIFY(FTL::CFGdiUtil::LoadPNGFromResource(image, _Module.GetResourceInstance(), i));
        if (bRet)
        {
            m_imgList.Add(image, CLR_NONE);
        }
    }

    
    //pThumbnail->GetHBITMAP(Color(255,255,255),&hBmp);  

    //CBitmap *pImage = CBitmap::FromHandle(hBmp);         //转换成CBitmap格式位图  

    //int a=m_pImageList->Add(pImage,RGB(255,255,255));  
    //pImage->DeleteObject();  

    
    //API_VERIFY(m_imgList.CreateFromImage(IDB_PNG_START_ALL, IMAGE_WIDTH, 1, 
    //    CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION));
    ////m_btnStart.SetImageList()
    m_btnStart.SetImageList(m_imgList);
    m_btnStart.SetImages(0, 1, 2, 3);
	return TRUE;
}

void CMainDlg::OnDestroy()
{
    m_lotteryMgr.StopInit();
    m_lotteryMgr.Stop();
}

BOOL CMainDlg::OnEraseBkgnd(CDCHandle dc)
{
    BOOL bRet = FALSE;
    if (m_StaicPic.m_hWnd)
    {
        CRect rcClient;
        API_VERIFY(GetClientRect(rcClient));
        
        dc.FillSolidRect(rcClient,GetSysColor(COLOR_BTNFACE)); // RGB(0x127, 0x127, 0x127));

        CRect rcStaticPic;
        m_StaicPic.GetWindowRect(rcStaticPic);
        API_VERIFY(ScreenToClient(rcStaticPic));

        CMemoryDC memDC(dc, rcStaticPic);
        CFont font;
        font.CreatePointFont(200, TEXT("宋体"));
        HFONT hOldFont = memDC.SelectFont(font);
        memDC.FillSolidRect(rcStaticPic, RGB(0, 0, 255));
        memDC.SetBkMode(TRANSPARENT);
        //memDC.DrawText(TEXT("祝范宗林、杨澜结婚快乐！"), -1, rcStaticPic, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        memDC.SelectFont(hOldFont);
    }
    
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
	//EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

void CMainDlg::OnBtnInitClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    BOOL bRet = FALSE;
    CFDirBrowser dirBrowser(_T("Choose Picture Directory"), m_hWnd, m_strInitPath);
    if (dirBrowser.DoModal())
    {
        m_dwImageCount = 0;
        CRect rcPic;
        m_StaicPic.GetClientRect(&rcPic);
        m_strInitPath = dirBrowser.GetSelectPath();
        API_VERIFY(m_lotteryMgr.Stop());
        API_VERIFY(m_lotteryMgr.StopInit());
        API_VERIFY(m_lotteryMgr.Init(m_hWnd, rcPic.Size(), m_strInitPath, TEXT("*.jpg;*.png;*.ico"), TRUE));

        _SetButtonStatus(TRUE, m_lotteryMgr.IsStarted(), m_lotteryMgr.IsPaused());
    }
}

void CMainDlg::OnBtnStartClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if (!m_lotteryMgr.IsStarted())
    {
        m_lotteryMgr.Start(100);
    }
    else
    {
        m_lotteryMgr.TogglePause();
    }
    
    _SetButtonStatus(FALSE, m_lotteryMgr.IsStarted(), m_lotteryMgr.IsPaused());
}

void CMainDlg::OnBtnResetClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    m_lotteryMgr.Stop();
    m_lotteryMgr.ResetSelectionState();
    _SetButtonStatus(FALSE, FALSE, FALSE);
}

void CMainDlg::_SetButtonStatus(BOOL bIniting, BOOL bStarted, BOOL bPaused)
{
    GetDlgItem(IDC_BTN_INIT).EnableWindow(!bIniting);
    GetDlgItem(IDC_BTN_START).EnableWindow(!bIniting);

    //GetDlgItem(IDC_BTN_PAUSE_RESUME).EnableWindow(bStarted);
    //GetDlgItem(IDC_BTN_PAUSE_RESUME).SetWindowText(bPaused ? TEXT("Resume") : _T("Pause"));

    GetDlgItem(IDC_BTN_RESET).EnableWindow(bStarted);
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

LRESULT CMainDlg::OnInitLotteryComplete(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DWORD dwCount = (DWORD)wParam;
    FTLASSERT(m_dwImageCount == dwCount);
    _SetButtonStatus(FALSE, FALSE, FALSE);
    SetWindowText(TEXT("Init Complete"));
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

        CMemoryDC memDC(dcPic, rcPic);
        memDC.FillSolidRect(rcPic, RGB(0, 0, 0));

        API_VERIFY(pInfo->imgThumbnail.Draw(memDC.m_hDC, rcDrawTarget));

        CPath pathFileName(pInfo->strPicturePath);
        int nFileName =pathFileName.FindFileName();
        if (nFileName != -1)
        {
            SetWindowText(pInfo->strPicturePath.Mid(nFileName));
        }
    }
    return 0;
}

void CMainDlg::OnBtnConfigClick(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    //
}

void CMainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (VK_ESCAPE == nChar)
    {
        MessageBox(TEXT("Esc"));
    }
}
