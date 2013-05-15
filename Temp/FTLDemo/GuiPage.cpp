// GuiPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "GuiPage.h"
#include <afxwin.h>
#include <ftlGdi.h>
#include <ftlGdiPlus.h>

// CGuiPage dialog

IMPLEMENT_DYNAMIC(CGuiPage, CPropertyPage)

CGuiPage::CGuiPage()
	: CPropertyPage(CGuiPage::IDD)
{

}

CGuiPage::~CGuiPage()
{
}

void CGuiPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGuiPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_REFRESH, &CGuiPage::OnBnClickedBtnRefresh)
    ON_BN_CLICKED(IDC_BTN_LOAD_OEM_BITMAP, &CGuiPage::OnBnClickedBtnLoadOemBitmap)
END_MESSAGE_MAP()

BOOL CGuiPage::OnInitDialog()
{
    BOOL bRet = TRUE;
    //API_VERIFY(m_bitmap.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP1)));
    //m_bitmap.GetBitmap(&m_bmpInfo);
    return bRet;
}
// CGuiPage 消息处理程序

void CGuiPage::OnBnClickedBtnRefresh()
{
    BOOL bRet = FALSE;
    CWnd *pWndDraw = GetDlgItem(IDC_STATIC_DRAW);
    FTLASSERT(pWndDraw);
    CRect rcWndDraw;
    pWndDraw->GetClientRect(&rcWndDraw);
    CDC *pDC = pWndDraw->GetDC();
    
    CFCanvas canvas;
    API_VERIFY(canvas.Create(m_hWnd, rcWndDraw.Width(), rcWndDraw.Height()));
    DWORD* pBuffer = (DWORD*)canvas.GetBuffer();
    for (int h = 0; h < rcWndDraw.Height(); h++)
    {
        for (int w = 0; w < rcWndDraw.Width(); w++)
        {
            COLORREF cor = RGBA(255*h/255, 255*w/255, 255, (h*w)%255);
            *pBuffer = cor;
            pBuffer++;
        }
    }
    CDC memDC;
    memDC.Attach(canvas.GetCanvasDC());
    //API_VERIFY(pDC->BitBlt(0,0,rcWndDraw.Width(), rcWndDraw.Height(),
    //    &memDC, 0,0, SRCCOPY));
    BLENDFUNCTION blend = {AC_SRC_OVER , 0, 255, AC_SRC_ALPHA };
    API_VERIFY(pDC->AlphaBlend(0,0,rcWndDraw.Width(),rcWndDraw.Height(),
        &memDC, 0,0, rcWndDraw.Width(),rcWndDraw.Height(), blend));
    memDC.Detach();

    //CDC memDC;
    //API_VERIFY(memDC.CreateCompatibleDC(pDC));
    //CBitmap* pOldBmp = memDC.SelectObject(&m_bitmap);
    //API_VERIFY(pDC->StretchBlt(0,0,rcWndDraw.Width(), rcWndDraw.Height(),
    //    &memDC, 0,0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, SRCCOPY));
    //memDC.SelectObject(pOldBmp);

    pWndDraw->ReleaseDC(pDC);
}

#ifndef OBM_CLOSE
#  define OBM_CLOSE 32754
#endif 

void CGuiPage::OnBnClickedBtnLoadOemBitmap()
{
    BOOL bRet = FALSE;
    CBitmap bmp;
    API_VERIFY(bmp.LoadOEMBitmap(OBM_CLOSE));
    BITMAP bmpInfo = {0};
    bmp.GetBitmap(&bmpInfo);

    CWnd *pWndDraw = GetDlgItem(IDC_STATIC_DRAW);
    FTLASSERT(pWndDraw);
    CRect rcWndDraw;
    pWndDraw->GetClientRect(&rcWndDraw);
    CDC *pDC = pWndDraw->GetDC();
    
    CDC dcMem;
    API_VERIFY(dcMem.CreateCompatibleDC(pDC));
    CBitmap* pOldBmp = dcMem.SelectObject(&bmp);
    API_VERIFY(pDC->BitBlt(0,0,bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMem, 0,0,SRCCOPY));

    dcMem.SelectObject(pOldBmp);
    API_VERIFY(dcMem.DeleteDC());
    pWndDraw->ReleaseDC(pDC);
}
