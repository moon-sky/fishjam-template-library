#pragma once


// CWindowPage dialog

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

    BOOL    m_bHideTaskBar;
};
