#pragma once


// CControlsPage dialog
//#include <atlapp.h>
//#include <atlctrls.h>
//#include <ftlControls.h>

class CControlsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CControlsPage)

public:
	CControlsPage();
	virtual ~CControlsPage();

// Dialog Data
	enum { IDD = IDD_PAGE_CONTROLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

    //CCheckedTreeViewCtrlEx  m_CheckTreeCtrl;
};
