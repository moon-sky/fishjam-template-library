#pragma once


// CGdiPage dialog

#include <ftlGdi.h>

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
    //CBitmap     m_bitmap;
    //BITMAP      m_bmpInfo;
    FTL::CFCanvas    m_canvas;
    CStatic     m_staticDrawTarget;

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbStockObject;
	afx_msg void OnCbnSelchangeComboStockObject();

    afx_msg void OnBnClickedBtnCanvasAttachBmp();
    afx_msg void OnBnClickedBtnCanvasDraw();
    afx_msg void OnBnClickedBtnBitbltCanvas();

private:
	BOOL _InitControls();
public:
};
