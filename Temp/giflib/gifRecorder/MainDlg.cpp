// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

#include "../gifMaker/gifMaker.h"

CMainDlg::CMainDlg()
{
    m_strSavePath = TEXT("gifRecorder.gif");
    m_pGifMaker = NULL;
    m_nLeft = 200;
    m_nTop = 200;
    m_nWidth = 640;
    m_nHeight = 480;
    m_nFps = 10;
    m_nBpp = 24;
    m_nImageIndex = 0;
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
            m_pGifMaker = new CGifMaker();
            m_pGifMaker->BeginMakeGif(m_nWidth, m_nHeight, m_nBpp, m_strSavePath);
            SetTimer(ID_TIMER_FPS, 1000/m_nFps, NULL);
        }
    }
    
}

void CMainDlg::OnBtnStopRecord(UINT uNotifyCode, int nID, CWindow wndCtl)
{
    if(m_pGifMaker)
    {
        KillTimer(ID_TIMER_FPS);
        m_pGifMaker->EndMakeGif(GetTickCount());
        SAFE_DELETE(m_pGifMaker);
    }

}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
    BOOL bRet = FALSE;

    m_nImageIndex++;
    {
        FTL::CFCanvas canvas;

        CRect rectCapture(0, 0, m_nWidth, m_nHeight);
        API_VERIFY(canvas.Create(m_hWnd, m_nWidth, -m_nHeight, m_nBpp));
        CWindowDC desktopDC(GetDesktopWindow());
        API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, m_nWidth, m_nHeight, desktopDC, m_nLeft, m_nTop, SRCCOPY));

#if 0
        CPath   path(m_strSavePath);
        path.RemoveExtension();
        CString strFileName = path.m_strPath;
        strFileName.AppendFormat(TEXT("_%d.bmp"), m_nImageIndex);
        API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), strFileName));
#endif

        m_pGifMaker->AddGifImage(canvas.GetBuffer(), canvas.GetBufferSize(), GetTickCount());
    }
}