#pragma once


// CShellPage dialog

class CShellPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CShellPage)

public:
	CShellPage();
	virtual ~CShellPage();

// Dialog Data
	enum { IDD = IDD_PAGE_SHELL };
    virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnShellExecute();
    afx_msg void OnBnClickedBtnShellExecuteEx();
};
