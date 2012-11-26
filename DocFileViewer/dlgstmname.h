// dlgstmname.h: interface for the Cdlgstmname class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGSTMNAME_H__FD022248_5F82_4D81_94A5_A3C1208976B7__INCLUDED_)
#define AFX_DLGSTMNAME_H__FD022248_5F82_4D81_94A5_A3C1208976B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CDlgStmName 
: public CDialogImpl<CDlgStmName>
, public CWinDataExchange<CDlgStmName>
{
public:
	enum { IDD = IDD_ADD_STM };
	
	CDlgStmName();
	virtual ~CDlgStmName();
	
	BEGIN_DDX_MAP(CDlgStmName)
		DDX_TEXT_LEN(IDC_EDT_NAME, m_szName, _countof(m_szName))
		DDX_TEXT_LEN(IDC_EDT_FILE, m_szFilePath, _countof(m_szFilePath))
	END_DDX_MAP()
		
	BEGIN_MSG_MAP(CDlgStmName)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_BTN_BROWSE, BN_CLICKED, OnBtnFileBrowse)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBtnFileBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
public:
	TCHAR m_szName[MAX_PATH];
	TCHAR m_szFilePath[MAX_PATH];
};


#endif // !defined(AFX_DLGSTMNAME_H__FD022248_5F82_4D81_94A5_A3C1208976B7__INCLUDED_)
