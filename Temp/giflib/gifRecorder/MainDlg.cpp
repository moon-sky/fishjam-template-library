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
    m_nLeft = 0;
    m_nTop = 0;
    m_nWidth = 640;
    m_nHeight = 480;
    m_nFps = 10;
    m_nBpp = 24;
    m_nImageIndex = 0;
    m_nCompressType = ctHighQuality;
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
        m_pGifMaker->Release();
        //delete m_pGifMaker;
        //m_pGifMaker = NULL;
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
    BOOL bRet = FALSE;
    CString strFilter = _T("Gif Files(*.gif)|*.gif|All Files(*.*)|*.*||");
    strFilter.Replace(TEXT('|'), TEXT('\0'));

    CFileDialog dlg(FALSE, TEXT(".gif"), m_strSavePath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, m_hWnd);
    if (dlg.DoModal() == IDOK)
    {
        m_strSavePath = dlg.m_szFileName;
        API_VERIFY(DoDataExchange(FALSE, IDC_STATIC_SAVE_PATH));
    }
}


void CMainDlg::OnBtnStartRecord(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    BOOL bRet = FALSE;
    API_VERIFY(DoDataExchange(TRUE));
    if (m_nFps < 1 || m_nFps > 30)
    {
        MessageBox(TEXT("FPS must be in [1~30]"));
        return;
    }
    if(m_nBpp != 1 && m_nBpp != 2 && m_nBpp != 4 && m_nBpp != 8 && m_nBpp != 16 && m_nBpp != 24 && m_nBpp != 32){
        MessageBox(TEXT("Invalid Bpp"));
        return;
    }

    if (bRet)
    {
        FTLASSERT(NULL == m_pGifMaker);
        if (NULL == m_pGifMaker)
        {
            m_nImageIndex = 0;
            m_pGifMaker = IGifMaker::GetInstance(); //new CGifMaker();
            m_pGifMaker->SetCompressType((CompressType)m_nCompressType);
            m_pGifMaker->BeginMakeGif(m_nWidth, m_nHeight, m_strSavePath);
            //SetTimer(ID_TIMER_FPS, 1000/m_nFps, NULL);
            API_VERIFY(m_threadRecord.Start(RecordGifThreadProc, this));
            
            GetDlgItem(IDC_BTN_START_RECORD).EnableWindow(FALSE);
            GetDlgItem(IDC_BTN_PAUSE_RESUME_RECORD).EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_STOP_RECORD).EnableWindow(TRUE);
        }
    }
    
}

void CMainDlg::OnBtnPauseResumeRecord(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if (m_pGifMaker)
    {
        if(m_threadRecord.HadRequestPause()){
            m_threadRecord.Resume();
            //m_pGifMaker->Resume();
            SetDlgItemText(IDC_BTN_PAUSE_RESUME_RECORD, TEXT("Pause"));
        }
        else
        {
            m_threadRecord.Pause();
            //m_pGifMaker->Pause();
            SetDlgItemText(IDC_BTN_PAUSE_RESUME_RECORD, TEXT("Resume"));
        }
    }
}

void CMainDlg::OnBtnStopRecord(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if(m_pGifMaker)
    {
        m_threadRecord.Stop();
        GetDlgItem(IDC_BTN_STOP_RECORD).EnableWindow(FALSE);
    }
}

LRESULT CMainDlg::OnMessageThreadMakeGifQuit(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    m_threadRecord.Wait(INFINITE);
    SAFE_DELETE(m_pGifMaker);
    GetDlgItem(IDC_BTN_START_RECORD).EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_PAUSE_RESUME_RECORD).EnableWindow(FALSE);

    //KillTimer(ID_TIMER_FPS);
    //m_pGifMaker->EndMakeGif(GetTickCount());
    MessageBox(TEXT("Thread Make Gif Quit"));
    return 0;
}

//void CMainDlg::OnTimer(UINT_PTR nIDEvent)
//{
//    BOOL bRet = FALSE;
//
//    m_nImageIndex++;
//    {
//        FTL::CFCanvas canvas;
//
//        CRect rectCapture(0, 0, m_nWidth, m_nHeight);
//        API_VERIFY(canvas.Create(m_hWnd, m_nWidth, -m_nHeight, m_nBpp));
//        CWindowDC desktopDC(GetDesktopWindow());
//        API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, m_nWidth, m_nHeight, desktopDC, m_nLeft, m_nTop, SRCCOPY));
//
//#if 0
//        CPath   path(m_strSavePath);
//        path.RemoveExtension();
//        CString strFileName = path.m_strPath;
//        strFileName.AppendFormat(TEXT("_%d.bmp"), m_nImageIndex);
//        API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), strFileName));
//#endif
//
//        m_pGifMaker->AddGifImage(canvas.GetBuffer(), canvas.GetBufferSize(), GetTickCount());
//    }
//}

BOOL CMainDlg::_OverlayMouseToScreen(HDC hdc, LPRECT lpRect)
{
    BOOL bRet = FALSE;
    POINT point = {0};
    API_VERIFY(::GetCursorPos(&point));
    if (!PtInRect(lpRect, point))
    {
        //Mouse is not in rect, so donot draw it and return TRUE;
        return TRUE;
    }
    CURSORINFO CursorInfo;
    CursorInfo.cbSize = sizeof(CURSORINFO);

    API_VERIFY(GetCursorInfo(&CursorInfo));
    if (bRet)
    {
        API_VERIFY_EXCEPT1(::DrawIconEx(hdc, CursorInfo.ptScreenPos.x - lpRect->left, CursorInfo.ptScreenPos.y - lpRect->top , CursorInfo.hCursor,
            32, 32, 0, NULL, DI_NORMAL), ERROR_INVALID_CURSOR_HANDLE);
        if (!bRet && GetLastError() == ERROR_INVALID_CURSOR_HANDLE)
        {
            SetLastError(0);
            bRet = TRUE;
        }
        ::ReleaseDC(NULL, hdc);
    }
    return bRet;
}

DWORD CMainDlg::RecordGifThreadProc(LPVOID pParam)
{
    CMainDlg* pThis = (CMainDlg*)pParam;
    DWORD dwResult = pThis->_InnerRecordGifThreadProc();
    return dwResult;
}

DWORD CMainDlg::_InnerRecordGifThreadProc()
{
    FUNCTION_BLOCK_TRACE(0);

    BOOL bRet = FALSE;
    FTL::FTLThreadWaitType waitType = FTL::ftwtStop;
    int nSleepTime = 1000/m_nFps;
    FTLTRACE(TEXT("Record Thread [%d] Start, Sleep=%d\n"), GetCurrentThreadId(), nSleepTime);

    FTL::CFCanvas canvas;
    API_VERIFY(canvas.Create(m_hWnd, m_nWidth, -m_nHeight, m_nBpp));
    
    CRect rcRecord(0, 0, m_nWidth, m_nHeight);
    do 
    {
        m_nImageIndex++;

        DWORD dwStartTickCount = GetTickCount();
        CRect rectCapture(0, 0, m_nWidth, m_nHeight);
        CWindowDC desktopDC(GetDesktopWindow());
        API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, m_nWidth, m_nHeight, desktopDC, m_nLeft, m_nTop, SRCCOPY));
        _OverlayMouseToScreen(canvas.GetCanvasDC(), rectCapture);

#if 0
        CPath   path(m_strSavePath);
        path.RemoveExtension();
        CString strFileName = path.m_strPath;
        strFileName.AppendFormat(TEXT("_%d.bmp"), m_nImageIndex);
        API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), strFileName));
#endif 

        m_pGifMaker->AddGifImage(rcRecord, canvas.GetBuffer(), canvas.GetBufferSize(), canvas.GetBpp(), dwStartTickCount);

        DWORD dwEndTickCount = GetTickCount();
        FTLTRACE(TEXT("RecordGif[%d] start=%d, nSleep=%d, End=%d, needSleep=%d\n"), 
            m_nImageIndex, dwStartTickCount, nSleepTime, dwEndTickCount, 
            (LONG(nSleepTime) - (dwEndTickCount - dwStartTickCount)));
        if (dwStartTickCount + nSleepTime > dwEndTickCount)
        {
            waitType = m_threadRecord.SleepAndCheckStop( nSleepTime - (dwEndTickCount - dwStartTickCount));
        }

        waitType = m_threadRecord.GetThreadWaitType(INFINITE);
        if (waitType != FTL::ftwtContinue )
        {
            break;
        }
        
    } while (TRUE);
    

    m_pGifMaker->EndMakeGif(GetTickCount(), INFINITE);
    //SAFE_DELETE(m_pGifMaker);

    FTLTRACE(TEXT("Record Thread [%d] Quit\n"), GetCurrentThreadId());
    PostMessage(WM_THREAD_MAKEGIF_QUIT, 0, 0);
    return 0;
}
