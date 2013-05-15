// ProgressListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TextProgressCtrl.h"
#include "ProgressListCtrl.h"

// CProgressListCtrl

IMPLEMENT_DYNAMIC(CProgressListCtrl, CListCtrl)

CProgressListCtrl::CProgressListCtrl()
{
    m_maxProgress = 100;
}

CProgressListCtrl::~CProgressListCtrl()
{
    POSITION pos = m_ProgressMap.GetStartPosition();
    LONG lKey = 0;
    CTextProgressCtrl* pProgress = NULL;
    while (NULL != pos)
    {
        m_ProgressMap.GetNextAssoc(pos, lKey, pProgress);
        delete pProgress;
    }

    m_ProgressMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CProgressListCtrl, CListCtrl)
END_MESSAGE_MAP()


// CProgressListCtrl message handlers

VOID CProgressListCtrl::SetMaxProgress(int maxProgress)
{
    m_maxProgress = maxProgress;
}

VOID CProgressListCtrl::ResetAllProgress()
{
    POSITION pos = m_ProgressMap.GetStartPosition();
    LONG lKey = 0;
    CTextProgressCtrl* pProgress = NULL;
    while (NULL != pos)
    {
        m_ProgressMap.GetNextAssoc(pos, lKey, pProgress);
        delete pProgress;
    }
    m_ProgressMap.RemoveAll();
}

BOOL CProgressListCtrl::SetProgress(int iItem, int iSubItem, int progress)
{
    CString strProgress;
    strProgress.Format(TEXT("%d"), progress);
    //SetItemText(iItem, iSubItem, strPercent);

    LONG lKey = MAKELONG(iItem,iSubItem);
    CTextProgressCtrl* pProgress = NULL;
    if (FALSE == m_ProgressMap.Lookup(lKey,pProgress))
    {
        CHeaderCtrl* pHeader = GetHeaderCtrl();
        CRect headRect;
        pHeader->GetItemRect(iSubItem, headRect);
        
        CRect itemRect;
        GetItemRect(iItem,itemRect,LVIR_LABEL);
        itemRect.top += 1;
        itemRect.bottom -= 1;
        itemRect.left += headRect.left;
        itemRect.right = itemRect.left + headRect.Width() - 4;

        pProgress = new CTextProgressCtrl();
        pProgress->Create(0, itemRect, this, (UINT)lKey);
        pProgress->ShowWindow(SW_SHOWNORMAL);
        pProgress->SetRange(0, m_maxProgress);
        m_ProgressMap.SetAt(lKey, pProgress);
    }   

    FTLASSERT(pProgress);
    pProgress->SetPos(progress);
    return TRUE;
}
