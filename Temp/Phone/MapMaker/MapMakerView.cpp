// MapMakerView.cpp : implementation of the CMapMakerView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MapMakerView.h"

BOOL CMapMakerView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

void CMapMakerView::DoPaint(CDCHandle dc)
{
	CString strCaption;
    CRect rcClient;
    GetClientRect(rcClient);
    dc.DrawText(TEXT("Fishjam"), -1, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
