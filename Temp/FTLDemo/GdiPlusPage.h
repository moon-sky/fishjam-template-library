#pragma once

#include "Gdiplus.h"
#include "afxwin.h"

#include "ColourPicker.h"
#include "CheckComboBox.h"
#include "ButtonSSL.h"

enum DrawImageFunctionType
{
	//http://msdn.microsoft.com/en-us/library/windows/desktop/ms535746(v=vs.85).aspx

	//1.0 版本
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

	difInt6Unit,				//DrawImage(Image*,INT,INT,INT,INT,INT,INT,Unit), 可以绘制部分图片，但不能缩放
	difReal6Unit,				//DrawImage(Image*, REAL, REAL, REAL, REAL, REAL, REAL, Unit) ， 同上

	difRectFsUnitAttributes,	//DrawImage(Image*,RectF&,RectF&,Unit,ImageAttributes*)

	difRectXYWHAttributesAbort, //DrawImage(Image*,Rect&,INT,INT,INT,INT,Unit,ImageAttributes*,DrawImageAbort,VOID*)
	difRectFXYHWAttributeAbort,			//DrawImage(Image*,RectF&,REAL,REAL,REAL,REAL,Unit,ImageAttributes*,DrawImageAbort,VOID*), 同上

	difPointArrayXYHWUnitAttributesAbort,  //DrawImage(Image*,Point*,INT,INT,INT,INT,INT,Unit,ImageAttributes*,DrawImageAbort,VOID*)
	difPointFArrayXYHWUnitAttributesAbort,	//DrawImage(Image*,PointF*,INT,REAL,REAL,REAL,REAL,Unit,ImageAttributes*,DrawImageAbort,VOID*)

	//1.1 版本  -- MSDN 中 Unit 写错了
	difRectFMaxtrixEffectAttributesUnit, //DrawImage(Image*,RectF*,Matrix*,Effect*,ImageAttributes*,Unit)
};

//Rotate/Scale/Translate 的顺序 -- 6种
enum TransformOrder
{
	toRST,		//Rotate -> Scale -> Translate
	toRTS,
	toSRT,
	toSTR,
	toTRS,
	toTSR,
};

struct GdiPlusTestParam
{
public:
	GdiPlusTestParam();
	BOOL GetCharacterRangeInfo(INT& nStart, INT& nLength);
	LPCTSTR GetParamInfo(FTL::CFStringFormater& formater);
public:
	CString m_strPaintString;
	CString m_strPaintImagePath;
	CRect   m_rcDrawPanelClient;

	INT     m_nFontHeight;
	INT     m_nRenderingOriginX , m_nRenderingOriginY;
	INT		m_nCharacterRangeStart, m_nCharacterRangeStop;
	INT     m_nStringFormatFlags;
	
	DrawImageFunctionType		m_drawImageFunctionType;

	Gdiplus::Unit				m_nUint;
	Gdiplus::PixelFormat		m_nPixelFormat;

	//字体和画刷属性
	Gdiplus::StringAlignment m_nAlignment;
	Gdiplus::StringAlignment m_nLineAlignment;
	Gdiplus::FontStyle		 m_nFontStyle;
	Gdiplus::Color			 m_clrBrush;
	Gdiplus::Color			 m_clrPen;
	Gdiplus::Color			 m_clrFont;
	Gdiplus::REAL			 m_rPenWidth;
	Gdiplus::DashStyle		 m_nDashStyle;
	Gdiplus::DashCap		 m_nDashCap;

	//质量属性
	BOOL						m_bEnabledCompositingMode;
	Gdiplus::CompositingMode	m_compositingMode;
	BOOL						m_bEnabledCompositingQuality;
	Gdiplus::CompositingQuality m_compositingQuality;
	BOOL						m_bEnabledInterpolationMode;
	Gdiplus::InterpolationMode  m_interpolationMode;
	BOOL						m_bEnabledPixelOffsetMode;
	Gdiplus::PixelOffsetMode    m_pixelOffsetMode;
	BOOL						m_bEnabledTextRenderingHint;
	Gdiplus::TextRenderingHint  m_textRenderingHint;
	BOOL						m_bEnabledSmoothingMode;
	Gdiplus::SmoothingMode		m_smoothingMode;


	//坐标和转换
	TransformOrder				m_transformOrder;
	Gdiplus::MatrixOrder		m_graphicsMatrixOrder;
	BOOL						m_bEnabledGraphicsRotate;
	Gdiplus::REAL				m_graphicsRotateAngle;
	BOOL						m_bEnabledGraphicsScale;
	Gdiplus::REAL				m_graphicsScaleSx;
	Gdiplus::REAL				m_graphicsScaleSy;
	BOOL						m_bEnabledGraphicsTranslate;
	Gdiplus::REAL				m_graphicsTranslateDx;
	Gdiplus::REAL				m_graphicsTranslateDy;

	Gdiplus::ColorAdjustType	 m_colorAdjustType;


	CString m_strFontFamily;

	Gdiplus::Rect	m_rtPaintDest;
	Gdiplus::RectF  m_rtfPaintDest;
};

enum GdiPlusTestType
{
	//Font
	gpttFontConstructor,

    //Pen & Brush
    gpttGradientBrush,

	//String
    gpttDrawString,
    gpttMeasureString,
    gpttDrawDriverString,				//可以控制行间距?
	gpttMeasureCharacterRanges,

    gpttGraphicsPath,

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
	BOOL    _SetDrawPanelInfo(const CPoint& ptMouse);
	BOOL	_InitFontCollection();
	BOOL	_InitComboboxControls();
	BOOL	_InitTestButtons();
	BOOL	_InitSpinControls();
	BOOL	_InitGraphicsQualityControls();
	BOOL	_InitGraphicsCoordinateAndTransform();

	BOOL    _UnInitControls();

	BOOL	_GetTestParam(const CRect& rcPaintDest, Gdiplus::Graphics* pGraphics);
	INT     _GetCheckComboxSelectedFlags(const CCheckComboBox& checkCombobox);
	Gdiplus::Status _CalcImageAttributes(Gdiplus::ImageAttributes& attributes);
	Gdiplus::Status _CalcImageMatrix(Gdiplus::Matrix& matrix);
#if (GDIPVER >= 0x0110)
	Gdiplus::Status _CalcImageEffect(Gdiplus::Effect& effect);
#endif //(GDIPVER >= 0x0110)

	void    _DoGdiPlusTest(GdiPlusTestType testType);
	Gdiplus::Status _SetGraphicsQualityParam(Gdiplus::Graphics* pGraphics);
	Gdiplus::Status _SetGraphicsCoordinateAndTransformParam(Gdiplus::Graphics* pGraphics);
    Gdiplus::Status _TestFontConstructor(HDC hdc, Gdiplus::Graphics* pGraphics);
    Gdiplus::Status _TestGradientBrush(HDC hdc, Gdiplus::Graphics* pGraphics);
	Gdiplus::Status _TestDrawString(HDC hdc, Gdiplus::Graphics* pGraphics);
	Gdiplus::Status _TestDrawImage(HDC hdc, Gdiplus::Graphics* pGraphics);
	Gdiplus::Status _TestMeasureCharacterRanges(HDC hdc, Gdiplus::Graphics* pGraphics);
protected:

	CStatic m_staticPaint;
	CStatic m_staticDrawPanelInfo;

	CComboBox m_cmbAlignment;
    CComboBox m_cmbLineAlignment;
	
    CComboBox m_cmbFontFamily;
	CComboBox m_cmbUnit;
	CComboBox m_cmbPixelFormat;
	CComboBox m_cmbDrawImageFunctionType;
	CComboBox m_cmbColorAdjustType;

	//字体和画刷属性
	CColourPicker			m_btnColorPen;
	CComboBox				m_cmbDashStyle;

	//质量属性
	CButton	  m_btnCheckCompositingMode;
	CComboBox m_cmbCompositingMode;
	CButton   m_btnCheckCompositingQuality;
	CComboBox m_cmbCompositingQuality;
	CButton   m_btnCheckInterpolationMode;
	CComboBox m_cmbInterpolationMode;
	CButton   m_btnCheckPixelOffsetMode;
	CComboBox m_cmbPixelOffsetMode;
	CButton	  m_btnCheckSmoothingMode;
	CComboBox m_cmbSmoothingMode;
	CButton   m_btnCheckTextRenderingHint;
	CComboBox m_cmbTextRenderingHint;

	//坐标和转换
	CComboBox	m_cmbTransformOrder;
	CComboBox   m_cmbGraphicsMatrixOrder;
	CButton		m_btnCheckGraphicsRotate;
	CButton		m_btnCheckGraphicsScale;
	CButton		m_btnCheckGraphicsTranslate;


	CCheckComboBox m_cmbFontStyle;
	CCheckComboBox m_cmbStringFormatFlags;
	
    CButtonSSL m_BtnGdiPlusFontTestMenu;
    CButtonSSL m_BtnGdiPlusPenBrushTestMenu;
	CButtonSSL m_BtnGdiPlusStringTestMenu;
	CButtonSSL m_BtnGdiPlusImageTestMenu;

    afx_msg void OnDestroy();
	afx_msg void OnFontTestConstructor();
    afx_msg void OnPenBrushGradientBrush();
	afx_msg void OnGdiplustestDrawstring();
	afx_msg void OnGdiplustestMeasureCharacterRanges();
	afx_msg void OnImagetestDrawimage();
	afx_msg void OnBnClickedBtnChooseFont();
	afx_msg void OnBnClickedBtnChooseImage();
	afx_msg void OnBnClickedBtnDumpImageProperty();
protected:
	static BOOL CALLBACK _DrawImageAbortProc(VOID * pParam);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
