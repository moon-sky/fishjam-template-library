#pragma once


// CRegPage dialog

class CRegPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CRegPage)

public:
	CRegPage();
	virtual ~CRegPage();

// Dialog Data
	enum { IDD = IDD_PAGE_REG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnRegExport();
};
