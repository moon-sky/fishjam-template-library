#include "StdAfx.h"
#include "WTLControlsTabCtrl.h"
#include "DlgTranscode.h"

CWTLControlsTabCtrl::CWTLControlsTabCtrl(void)
{
}

CWTLControlsTabCtrl::~CWTLControlsTabCtrl(void)
{
}


BOOL CWTLControlsTabCtrl::Init()
{
    BOOL bRet = FALSE;
    CDlgTranscode* pDlgTranscode = new CDlgTranscode();
    pDlgTranscode->Create(*this);
    AddWindow(pDlgTranscode,TEXT("Transcode"));

    SetRectangle();
    return bRet;
}

BOOL CWTLControlsTabCtrl::UnInit()
{
    BOOL bRet = FALSE;
    int dwItemCount = GetItemCount();
    for (int nItem = dwItemCount - 1; nItem >= 0 ; nItem--)
    {
        CWindowImplRoot<CWindow>* pWnd = GetWindow(nItem);
        API_VERIFY(pWnd->DestroyWindow());
        delete pWnd;
        API_VERIFY(RemoveWindow(nItem));
    }
    API_VERIFY(DeleteAllItems());
    return bRet;
}