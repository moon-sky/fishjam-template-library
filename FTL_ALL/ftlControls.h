#ifndef FTL_CONTROLS_H
#define FTL_CONTROLS_H
#pragma once

#include "ftlDefine.h"

#ifndef __ATLCTRLS_H__
#  error ftlControls.h requires WTL( atlctrls.h )
#endif //__ATLCTRLS_H__
#include <atlbase.h>
namespace FTL
{
    struct FScrollSkinInfo
    {
        CImage* pBtnVUp;
        CImage* pBtnVDown;
    };

    //template </*class T,*/ class TBase = WTL::CScrollBar, class TWinTraits = ATL::CControlWinTraits>
    class CFSkinScrollBar : public ATL::CWindowImpl<CFSkinScrollBar, WTL::CScrollBar>
    {
    public:
        BEGIN_MSG_MAP_EX(CFSkinScrollBar)
            //MSG_WM_CREATE(OnCreate)
            //MSG_WM_ERASEBKGND(OnEraseBkgnd)
            //CHAIN_MSG_MAP(CZoomScrollWindowImpl<CScrollImageViewView>) // CScrollWindowImpl<CScrollImageViewView>)
        END_MSG_MAP()

    public:
        CFSkinScrollBar()
        {

        }
        ~CFSkinScrollBar()
        {

        }
    private:
        FScrollSkinInfo    m_SkinInfo;
    };

    template <class T> //, class TBase = ATL::CWindow, class TWinTraits = ATL::CControlWinTraits>
    class ATL_NO_VTABLE CFSkinScrollWindow// : public ATL::CWindowImpl<T, TBase, TWinTraits>//, TWinTraits>
    {
    public:
        BEGIN_MSG_MAP_EX(CFSkinScrollWindow)
        END_MSG_MAP()
    public:
        BOOL Initialze(HWND hWndCtrl, const FScrollSkinInfo* pSkinInfo)
        {
            BOOL bRet = FALSE;
            FTLASSERT(NULL == m_wndMiddle.m_hWnd);
            
            CWindow wndCtrl(hWndCtrl);
            CWindow wndParent(::GetParent(hWndCtrl));
            FTLASSERT(wndParent.IsWindow());

            CRect rcFrame,rcWnd;
            wndCtrl.GetWindowRect(&rcFrame);
            wndParent->ScreenToClient(&rcFrame);
            rcWnd=rcFrame;
            
            UINT uID = ::GetDlgCtrlID(hWndCtrl);

            
            return bRet;
        }
        void Finalize()
        {

        }
    private:
        CFSkinScrollBar     m_ScrollBarVert;
        CFSkinScrollBar     m_ScrollBarHorz;
        ATL::CWindow        m_wndMiddle;
        WNDPROC             m_funOldProc;
    };

}

#endif //FTL_CONTROLS_H

#ifndef USE_EXPORT
#  include "ftlControls.hpp"
#endif