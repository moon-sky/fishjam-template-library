#pragma once


// CHookApiPage dialog

class CHookApiPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CHookApiPage)

public:
	CHookApiPage();
	virtual ~CHookApiPage();

// Dialog Data
	enum { IDD = IDD_PAGE_HOOKAPI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnHookApiDetours();
};
