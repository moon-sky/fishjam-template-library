// dlgstmname.cpp: implementation of the Cdlgstmname class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DocFileViewer.h"
#include "dlgstmname.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgStmName::CDlgStmName()
{
	ZeroMemory(m_szName, sizeof(m_szName));
	ZeroMemory(m_szFilePath, sizeof(m_szFilePath));
}

CDlgStmName::~CDlgStmName()
{
}

LRESULT CDlgStmName::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	uMsg, wParam, lParam, bHandled;
	CenterWindow();
	DoDataExchange(DDX_LOAD);
	return 0;
}

LRESULT CDlgStmName::OnBtnFileBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("All Files (*.*)\0*.*\0\0"));
	if (IDOK == dlg.DoModal())
	{
		lstrcpyn(m_szFilePath, dlg.m_szFileName, _countof(m_szFilePath));
		DoDataExchange(DDX_LOAD, IDC_EDT_FILE);
	}
	return 0;
}

LRESULT CDlgStmName::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	wNotifyCode, wID, hWndCtl, bHandled;
	if (wID==IDOK)
	{
		DoDataExchange(DDX_SAVE);
		
		if (lstrlen(m_szName) == 0) {
			CWindow(GetDlgItem(IDC_EDT_NAME)).SetFocus();
			return 0;
		}
		if (lstrlen(m_szFilePath) == 0) {
			CWindow(GetDlgItem(IDC_EDT_FILE)).SetFocus();
			return 0;
		}
	}
	EndDialog(wID);
	return 0;
}
