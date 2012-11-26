// dlgstgname.h: interface for the dlgstgname class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGSTGNAME_H__3858DC8F_D182_4EA3_B694_F480376160A8__INCLUDED_)
#define AFX_DLGSTGNAME_H__3858DC8F_D182_4EA3_B694_F480376160A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CDlgStgName 
	: public CDialogImpl<CDlgStgName>
	, public CWinDataExchange<CDlgStgName>
{
public:
	enum { IDD = IDD_ADD_STG };
	
	CDlgStgName();
	virtual ~CDlgStgName();

	BEGIN_DDX_MAP(CDlgStgName)
		DDX_TEXT_LEN(IDC_EDT_NAME, m_szName, _countof(m_szName))
	END_DDX_MAP()

	BEGIN_MSG_MAP(CDlgStgName)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()
		
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
public:
	TCHAR m_szName[MAX_PATH];
};

#endif // !defined(AFX_DLGSTGNAME_H__3858DC8F_D182_4EA3_B694_F480376160A8__INCLUDED_)
