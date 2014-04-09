#include "StdAfx.h"
#include "CastTesterTabCtrl.h"
#include "DlgDLNATester.h"

CCastTesterTabCtrl::CCastTesterTabCtrl(void)
{
}

CCastTesterTabCtrl::~CCastTesterTabCtrl(void)
{
}


BOOL CCastTesterTabCtrl::Init()
{
    BOOL bRet = FALSE;
    CDlgDLNATester* pDlgDLNATester = new CDlgDLNATester();
    pDlgDLNATester->Create(*this);
    AddWindow(pDlgDLNATester,TEXT("DLNATester"));

    //CDlgList* pDlgList = new CDlgList();
    //pDlgList->Create(*this);
    //AddWindow(pDlgList,TEXT("列表控件"));

    //CDlgExtensions* pDlgExtensions = new CDlgExtensions();
    //pDlgExtensions->Create(*this);
    //AddWindow(pDlgExtensions,TEXT("扩展控件"));

    SetRectangle();
    return bRet;
}

BOOL CCastTesterTabCtrl::UnInit()
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