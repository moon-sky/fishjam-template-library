#ifndef FTL_CONTROLS_HPP
#define FTL_CONTROLS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlControls.h"
#endif

namespace FTL
{
    LRESULT CFSkinComboBox::OnCbnDropDown(UINT uNotifyCode, int nID, CWindow wndCtl)
    {
        return 0;
    }

    LRESULT CFSkinComboBox::OnCbnCloseup(UINT uNotifyCode, int nID, CWindow wndCtl)
    {
        return 0;
    }
    HBRUSH CFSkinComboBox::OnCtlColorListBox(CDCHandle dc, CListBox listBox)
    {
        static HBRUSH hBrush = CreateSolidBrush(RGB(255,0,0));
        if (!m_hWndList)
        {
            SkinWndScroll(m_hWndList, m_pSkinInfo);
            m_hWndList = listBox.m_hWnd;
        }
        return hBrush;
    }
    BOOL CFSkinComboBox::SkinScroll(const FScrollSkinInfo* pSkinInfo)
    {
        m_pSkinInfo = pSkinInfo;
        //手动弹出下拉列表以便进行列表窗口的子类化
        ShowDropDown(TRUE);
        ShowDropDown(FALSE);
        return TRUE;
    }
    void CFSkinComboBox::UnSkinScroll()
    {
    }


    CFSkinScrollBar::CFSkinScrollBar()
    {

    }
    CFSkinScrollBar::~CFSkinScrollBar()
    {

    }
    int CFSkinScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        return 0;
    }


    CFSkinScrollWindow::CFSkinScrollWindow()
    {
        m_nScrollWidth = 16;
    }
    CFSkinScrollWindow::~CFSkinScrollWindow()
    {

    }
    BOOL CFSkinScrollWindow::SkinScroll(HWND hWnd, const FScrollSkinInfo* pSkinInfo)
    {
        BOOL bRet = FALSE;
        FTLASSERT(NULL == m_wndMiddle.m_hWnd);

        ::SetWindowLong(hWnd, GWL_USERDATA, (LONG) this);
        m_funOldProc=(WNDPROC)::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)HookWndProc);

        //CWindow wndCtrl(hWndCtrl);
        //CWindow wndParent(::GetParent(hWndCtrl));
        //FTLASSERT(wndParent.IsWindow());

        //CRect rcFrame,rcWnd;
        //wndCtrl.GetWindowRect(&rcFrame);
        //wndParent->ScreenToClient(&rcFrame);
        //rcWnd=rcFrame;
        //
        //UINT uID = ::GetDlgCtrlID(hWndCtrl);
        return bRet;
    }
    void CFSkinScrollWindow::UnSkinScroll()
    {

    }

    LRESULT CALLBACK CFSkinScrollWindow::HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        CFSkinScrollWindow *pSkinFrame = (CFSkinScrollWindow*)::GetWindowLong(hwnd,GWL_USERDATA);
        if (pSkinFrame)
        {
            if (WM_NCCALCSIZE == msg)
            {
                LPNCCALCSIZE_PARAMS pNcCalcSizeParam=(LPNCCALCSIZE_PARAMS)lp;
                int nWid=::GetSystemMetrics(SM_CXVSCROLL);
                pNcCalcSizeParam->rgrc[0].right += nWid - pSkinFrame->m_nScrollWidth;
            }
        }
        LRESULT ret =::CallWindowProc(pSkinFrame->m_funOldProc,hwnd,msg,wp,lp);
        return ret;
    }

    extern "C" CFSkinScrollWindow* SkinWndScroll(HWND hWnd, const FScrollSkinInfo* pScrollSkinInfo)
    {
        CFSkinScrollWindow *pFrame=new CFSkinScrollWindow();
        pFrame->SkinScroll(hWnd, pScrollSkinInfo);
        return pFrame;

    }
    extern "C" BOOL UnSkinWndScroll(HWND  hWnd)
    {
        CFSkinScrollWindow *pFrame=(CFSkinScrollWindow *)GetWindowLong(hWnd,GWL_USERDATA);
        if(pFrame) 
        {
            //RECT rc;
            //CWnd *pParent=pFrame->GetParent();
            //pFrame->GetWindowRect(&rc);
            //pParent->ScreenToClient(&rc);
            //SetWindowLong(pWnd->m_hWnd,GWL_WNDPROC,(LONG)pFrame->m_funOldProc);
            //SetWindowLong(pWnd->m_hWnd,GWL_USERDATA,0);
            //pWnd->SetParent(pParent);
            //pWnd->MoveWindow(&rc);
            pFrame->DestroyWindow();
            delete pFrame;
        }
        return TRUE;

    }
}

#endif //FTL_CONTROLS_HPP
