#pragma once


// CGuiPage dialog

class CGuiPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGuiPage)

public:
	CGuiPage();
	virtual ~CGuiPage();

// Dialog Data
	enum { IDD = IDD_PAGE_GUI };
    virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    CBitmap     m_bitmap;
    BITMAP      m_bmpInfo;
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnRefresh();
    afx_msg void OnBnClickedBtnLoadOemBitmap();
};
