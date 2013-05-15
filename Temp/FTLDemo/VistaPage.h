#pragma once


// CVistaPage dialog

class CVistaPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CVistaPage)

public:
	CVistaPage();
	virtual ~CVistaPage();

// Dialog Data
	enum { IDD = IDD_PAGE_VISTA };
    virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    //afx_msg void OnBnClickedStartThreadPoolIo();
};
