#pragma once

#include "Gdiplus.h"
#include "afxwin.h"

#include "CheckComboBox.h"
#include "ButtonSSL.h"

enum DrawImageFunctionType
{
	difDestRect,				//DrawImage(Image*, const Rect),	自动缩放来适应大小
	difDestPoint,				//DrawImage(Image*, const Point),   从指定点开始绘制原图，超过的部分自动被Clip
	difDestRectF,				//DrawImage(Image*, const RectF),	自动缩放来适应大小
	difDestXY,					//DrawImage(Image*, INT, INT),		同 difDestPoint
	difDestPointF,				//DrawImage(Image*, const PointF),  
	difDestRealXY,				//DrawImage(Image*,REAL,REAL),
	difDestPointArray,			//DrawImage(Image*,Point*,INT).		平行四边形的点数组(3个)，1->0->2->3，可以绘制自动缩放的倾斜图
	difDestPointFArray,			//DrawImage(Image*,PointF*,INT).	同 difDestPointArray

	difDestXYWH,				//DrawImage(Image*, INT, INT, INT, INT), 同 difDestRect
	difDestRealXYWH,			//DrawImage(Image*, REAL, REAL, REAL, REAL), 同 difDestRect

	difInt6Unit,				//DrawImage(Image*,INT,INT,INT,INT,INT,INT,Unit) 
	difReal6Unit,				//DrawImage(Image*, REAL, REAL, REAL, REAL, REAL, REAL, Unit) 
};

struct GdiPlusTestParam
{
public:
	GdiPlusTestParam();
	BOOL GetCharacterRangeInfo(INT& nStart, INT& nLength);
	LPCTSTR GetParamInfo(FTL::CFStringFormater& formater);
public:
	INT     m_nFontHeight;
	INT     m_nRenderingOriginX , m_nRenderingOriginY;
	INT		m_nCharacterRangeStart, m_nCharacterRangeStop;
	INT     m_nStringFormatFlags;
	
	DrawImageFunctionType		m_drawImageFunctionType;

	Gdiplus::REAL	m_fRotateAngle;
	Gdiplus::Unit	m_nUint;

	Gdiplus::StringAlignment m_nAlignment;
	Gdiplus::StringAlignment m_nLineAlignment;
	Gdiplus::FontStyle		 m_nFontStyle;
	Gdiplus::Color			 m_clrBrush;

	Gdiplus::TextRenderingHint   m_TextRenderingHint;
	Gdiplus::PixelOffsetMode     m_pixelOffsetMode;

	CString m_strPaintString;
	CString m_strPaintImagePath;

	CString m_strFontFamily;

	Gdiplus::Rect	m_rtPaintDest;
	Gdiplus::RectF  m_rtfPaintDest;
};

enum GdiPlusTestType
{
	//String
    gpttDrawString,
    gpttMeasureString,
    gpttDrawDriverString,
	gpttMeasureCharacterRanges,

	//Image
	gpttDrawImage,
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
private:
	GdiPlusTestParam	m_testParam;

	BOOL    _InitControls();
	BOOL	_InitFontCollection();
	BOOL	_InitComboboxControls();
	BOOL	_InitTestButtons();
	BOOL	_InitSpinControls();
	BOOL    _UnInitControls();

	BOOL	_GetTestParam(const CRect& rcPaintDest, Gdiplus::Graphics* pGraphics);
	INT     _GetCheckComboxSelectedFlags(const CCheckComboBox& checkCombobox);

	void    _DoGdiPlusTest(GdiPlusTestType testType);
	Gdiplus::Status _TestDrawString(Gdiplus::Graphics* pGraphics);
	Gdiplus::Status _TestDrawImage(Gdiplus::Graphics* pGraphics);
	Gdiplus::Status _TestMeasureCharacterRanges(Gdiplus::Graphics* pGraphics);
protected:

	CStatic m_staticPaint;
	CComboBox m_cmbAlignment;
    CComboBox m_cmbLineAlignment;
    CComboBox m_cmbTextRenderingHint;
	
    CComboBox m_cmbFontFamily;
    CComboBox m_cmbPixelOffset;
	CComboBox m_cmbUnit;
	CComboBox m_cmbDrawImageFunctionType;

	CCheckComboBox m_cmbFontStyle;
	CCheckComboBox m_cmbStringFormatFlags;
	
	CSpinButtonCtrl	m_spinRotateAngle;

	CButtonSSL m_BtnGdiPlusStringTestMenu;
	CButtonSSL m_BtnGdiPlusImageTestMenu;

    afx_msg void OnDestroy();
	afx_msg void OnGdiplustestDrawstring();
	afx_msg void OnGdiplustestMeasureCharacterRanges();
	afx_msg void OnImagetestDrawimage();
	afx_msg void OnBnClickedBtnChooseImage();
};
