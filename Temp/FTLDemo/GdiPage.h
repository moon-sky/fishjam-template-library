#pragma once


// CGdiPage dialog

class CGdiPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGdiPage)

public:
	CGdiPage();
	virtual ~CGdiPage();

// Dialog Data
	enum { IDD = IDD_PAGE_GDI };
    virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    CBitmap     m_bitmap;
    BITMAP      m_bmpInfo;
	DECLARE_MESSAGE_MAP()
public:
};
