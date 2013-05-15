#pragma once

#include "Gdiplus.h"
#include "afxwin.h"

// CGdiPlusPage dialog

enum TestGdiPlusStringType
{
    gpstDrawString,
    gpstMeasureString,
    gpstDrawDriverString,
};

class CGdiPlusPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CGdiPlusPage)

public:
	CGdiPlusPage();
	virtual ~CGdiPlusPage();

// Dialog Data
	enum { IDD = IDD_PAGE_GDIPLUS };
    virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
protected:
    INT     m_nFontHeight;
    Gdiplus::FontStyle     m_fontStyle;
    INT     m_nRenderingOriginX;
    INT     m_nRenderingOriginY;
    Gdiplus::TextRenderingHint   m_TextRenderingHint;
    Gdiplus::PixelOffsetMode     m_pixelOffsetMode;
    CString m_strPaintString;
    CString m_strFontFamily;
private:
    void    _MeasureString(Gdiplus::Graphics* pGraphics);

    CStatic m_staticPaint;
    CListCtrl m_lstStringFormatFlags;

    BOOL    _InitControls();
    BOOL    _UnInitControls();
    void    _TestGdiPlusString(TestGdiPlusStringType testType);
public:
    afx_msg void OnBnClickedBtnDrawString();
    INT     GetSelectedStringFormatFlags();
    CComboBox m_cmbAlignment;
    CComboBox m_cmbLineAlignment;
    CComboBox m_cmbTextRenderingHint;
    
    CComboBox m_cmbFontFamily;
    CComboBox m_cmbPixelOffset;
    CComboBox m_cmbFontStyle;

    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnDrawDriverString();
};
