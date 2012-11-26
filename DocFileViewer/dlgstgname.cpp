// dlgstgname.cpp: implementation of the dlgstgname class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocFileViewer.h"
#include "dlgstgname.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgStgName::CDlgStgName()
{
	ZeroMemory(m_szName, sizeof(m_szName));	
}

CDlgStgName::~CDlgStgName()
{
	
}

LRESULT CDlgStgName::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	uMsg, wParam, lParam, bHandled;
	CenterWindow();
	DoDataExchange(DDX_LOAD);
	return 0;
}

LRESULT CDlgStgName::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	wNotifyCode, wID, hWndCtl, bHandled;
	if (wID==IDOK)
	{
		DoDataExchange(DDX_SAVE);

		if (lstrlen(m_szName) == 0) {
			CWindow(GetDlgItem(IDC_EDT_NAME)).SetFocus();
			return 0;
		}
	}
	EndDialog(wID);
	return 0;
}
