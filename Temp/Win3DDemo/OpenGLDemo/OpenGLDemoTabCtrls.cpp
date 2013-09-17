#include "StdAfx.h"
#include "OpenGLDemoTabCtrls.h"
#include "DlgFirst.h"

COpenGLDemoTabCtrls::COpenGLDemoTabCtrls(void)
{
}

COpenGLDemoTabCtrls::~COpenGLDemoTabCtrls(void)
{
}


BOOL COpenGLDemoTabCtrls::Init()
{
    BOOL bRet = FALSE;
    CDlgFirst* pDlgFirst = new CDlgFirst();
    pDlgFirst->Create(*this);
    AddWindow(pDlgFirst,TEXT("First Demo"));

    //CDlgList* pDlgList = new CDlgList();
    //pDlgList->Create(*this);
    //AddWindow(pDlgList,TEXT("列表控件"));

    //CDlgExtensions* pDlgExtensions = new CDlgExtensions();
    //pDlgExtensions->Create(*this);
    //AddWindow(pDlgExtensions,TEXT("扩展控件"));

    SetRectangle();
    return bRet;
}

BOOL COpenGLDemoTabCtrls::UnInit()
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