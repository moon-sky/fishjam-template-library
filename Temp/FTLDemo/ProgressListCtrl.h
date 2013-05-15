#pragma once


// CProgressListCtrl

#include "TextProgressCtrl.h"

class CProgressListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CProgressListCtrl)

public:
	CProgressListCtrl();
	virtual ~CProgressListCtrl();
public:
    VOID SetMaxProgress(INT maxProgress);
    BOOL SetProgress(INT iItem, INT iSubItem, INT progress);
    VOID ResetAllProgress();
private:
    CMap<LONG, LONG, CTextProgressCtrl*, CTextProgressCtrl*>  m_ProgressMap;
    INT m_maxProgress;
protected:
	DECLARE_MESSAGE_MAP()
};


