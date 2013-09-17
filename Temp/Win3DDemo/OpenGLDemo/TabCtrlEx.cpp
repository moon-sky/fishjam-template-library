#include "StdAfx.h"
#include "TabCtrlEx.h"
#include ".\tabctrlex.h"

CTabCtrlEx::CTabCtrlEx(void)
{
    m_CurrentIndex = 0;
}

CTabCtrlEx::~CTabCtrlEx(void)
{
}
int CTabCtrlEx::AddWindow(CTabCtrlWindow* pWnd,LPCTSTR pszTitle)
{
    return InsertWindow(GetItemCount(),pWnd,pszTitle);
}

int CTabCtrlEx::InsertWindow(int nItem, CTabCtrlWindow* pWnd,LPCTSTR pszTitle)
{
    ATLASSERT(pWnd);
    ATLASSERT(m_AlltabPages.GetSize() == this->GetItemCount());

    m_AlltabPages.Add(pWnd);
    return InsertItem(nItem,pszTitle);
}

BOOL CTabCtrlEx::RemoveWindow(int nItem)
{
    ATLASSERT(nItem >= 0);
    ATLASSERT(nItem < GetItemCount());
    ATLASSERT(nItem < m_AlltabPages.GetSize());
    
    BOOL bRet = FALSE;

    m_AlltabPages.RemoveAt(nItem);
    API_VERIFY(DeleteItem(nItem));
    return bRet;
}

CTabCtrlWindow* CTabCtrlEx::GetWindow(int nItem)
{
    ATLASSERT(nItem >= 0);
    ATLASSERT(nItem < GetItemCount());
    ATLASSERT(nItem < m_AlltabPages.GetSize());

    return m_AlltabPages[nItem];
}

LRESULT CTabCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
    DefWindowProc();

    if(m_CurrentIndex != GetCurFocus()){
        m_AlltabPages[m_CurrentIndex]->ShowWindow(SW_HIDE);
        m_CurrentIndex = GetCurFocus();
        m_AlltabPages[m_CurrentIndex]->ShowWindow(SW_SHOW);
        //m_AlltabPages[m_CurrentIndex]->SetFocus();
    }
    return 0;
}

void CTabCtrlEx::SetRectangle()
{
    CRect tabRect, itemRect;
    int nX, nY, nXc, nYc;

    GetClientRect(&tabRect);
    GetItemRect(0, &itemRect);

    nX=itemRect.left;
    nY=itemRect.bottom + 1;
    nXc=tabRect.right - itemRect.left - 2;
    nYc=tabRect.bottom - nY - 2;

    for(int nCount=0; nCount < GetItemCount(); nCount++)
    {
        if (nCount == m_CurrentIndex)
        {    
            m_AlltabPages[m_CurrentIndex]->SetWindowPos(HWND_TOP, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
        }
        else
        {
            m_AlltabPages[nCount]->SetWindowPos(HWND_TOP, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
        }
    }
}
LRESULT CTabCtrlEx::OnSize(UINT state, CSize Size)
{
    //DefWindowProc();
    SetRectangle();
    SetMsgHandled(FALSE);
    return 0;
}
