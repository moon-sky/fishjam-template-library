#pragma once


// CWindowPage dialog
#include "MyTooltipCtrl.h"
class CWindowPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWindowPage)

public:
	CWindowPage();
	virtual ~CWindowPage();

// Dialog Data
	enum { IDD = IDD_PAGE_WINDOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnMessageInfo();
	afx_msg void OnBnClickedBtnRegisterMessageInfo();
    afx_msg void OnBnClickedBtnHideShowTaskBar();
    afx_msg void OnBnClickedBtnMenuInfoDump();

    BOOL    m_bHideTaskBar;

	BOOL	m_bSetTooltip;
	CMyTooltipCtrl    m_tooltip;
	//CMFCToolTipCtrl m_mfcTooltipCtrl;
	//CTooltipManager	m_tooltipMgr;
	afx_msg void OnBnClickedBtnTooltip();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
