#pragma once


// CCrackPage dialog

class CCrackPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCrackPage)

public:
	CCrackPage();
	virtual ~CCrackPage();

// Dialog Data
	enum { IDD = IDD_PAGE_CRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString m_strPathOfAppWithMemOverFlow;
    afx_msg void OnBnClickedBtnCrack();
};
