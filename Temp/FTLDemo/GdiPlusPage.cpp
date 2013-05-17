// GuiPlusPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "GdiPlusPage.h"
#include <ftlGdiPlus.h>
#include <ftlDebug.h>

using namespace Gdiplus;
// CGdiPlusPage dialog

GdiPlusTestParam::GdiPlusTestParam()
{
	m_strPaintString = _T("测试文字,有中文有 English，如 A B C 和 abc");
	m_strPaintImagePath = _T("C:\\Users\\Public\\Pictures\\Sample Pictures\\Koala.jpg");
	m_rcDrawPanelClient.SetRectEmpty();

	m_nFontHeight = 10;
	m_nRenderingOriginX = 0;
	m_nRenderingOriginY = 0;
	m_nCharacterRangeStart = 0;
	m_nCharacterRangeStop = -1;
	m_nStringFormatFlags = 0;
	m_nUint = UnitPixel;
	m_drawImageFunctionType = difDestRect;
	m_colorAdjustType = ColorAdjustTypeDefault;

	m_nAlignment = StringAlignmentNear;
	m_nLineAlignment = StringAlignmentNear;
	m_nFontStyle = FontStyleRegular;
	m_clrBrush = (ARGB)Color::Red;

	m_strFontFamily = _T("Arial");

	//质量属性
	m_bEnabledCompositingMode = TRUE;
	m_compositingMode = CompositingModeSourceOver;
	m_bEnabledCompositingQuality = TRUE;
	m_compositingQuality = CompositingQualityDefault;
	m_bEnabledInterpolationMode = TRUE;
	m_interpolationMode = InterpolationModeDefault;
	m_bEnabledPixelOffsetMode = TRUE;
	m_pixelOffsetMode = PixelOffsetModeDefault;
	m_bEnabledSmoothingMode = TRUE;
	m_smoothingMode = SmoothingModeDefault;
	m_bEnabledTextRenderingHint = TRUE;
	m_textRenderingHint = TextRenderingHintSystemDefault;
	
	//坐标和转换
	m_transformOrder = toRST;
	m_graphicsMatrixOrder = MatrixOrderPrepend;
	m_bEnabledGraphicsRotate = TRUE;
	m_graphicsRotateAngle = 0.0f;
	m_bEnabledGraphicsScale = TRUE;
	m_graphicsScaleSx = 1.0f;
	m_graphicsScaleSy = 1.0f;
	m_bEnabledGraphicsTranslate = TRUE;
	m_graphicsTranslateDx = 0.0f;
	m_graphicsTranslateDy = 0.0f;
}

BOOL GdiPlusTestParam::GetCharacterRangeInfo(INT& nStart, INT& nLength)
{
	BOOL bRet = TRUE;
	nLength = (m_nCharacterRangeStop == -1) ? m_strPaintString.GetLength() : m_nCharacterRangeStop - m_nCharacterRangeStart;
	if (nLength + m_nCharacterRangeStart > m_strPaintString.GetLength())
	{
		nLength = m_strPaintString.GetLength() - m_nCharacterRangeStart;
	}
	return bRet;
}

LPCTSTR GdiPlusTestParam::GetParamInfo(FTL::CFStringFormater& formater)
{
	formater.Format(TEXT("String=%s,\n\tStringFormatFlags=0x%08x,")
		TEXT("alignment=%d, lineAlignment=%d, Font={%d, %s}\n"),

		m_strPaintString,  m_nStringFormatFlags, m_nAlignment, m_nLineAlignment, m_nFontHeight, m_strFontFamily
		//rcBounds.X, rcBounds.Y, rcBounds.GetRight(), rcBounds.GetBottom()
		);
	return formater.GetString();
}

IMPLEMENT_DYNAMIC(CGdiPlusPage, CPropertyPage)

CGdiPlusPage::CGdiPlusPage()
	: CPropertyPage(CGdiPlusPage::IDD)
{

}

CGdiPlusPage::~CGdiPlusPage()
{
}

BOOL CGdiPlusPage::_DrawImageAbortProc(VOID * pParam)
{
	//本函数被频繁调用，如果要中断执行，则返回TRUE，返回FALSE表示继续
	CGdiPlusPage* pThis = reinterpret_cast<CGdiPlusPage*>(pParam);
	if (pThis)
	{
		//if (pThis->m_bWillCancel)
		//{
		//    return TRUE;  //
		//}
		//pThis->SetWindowText(TEXT("DrawImageAbort"));
	}
	return FALSE;
}

void CGdiPlusPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_PAINT, m_staticPaint);
	DDX_Control(pDX, IDC_STATIC_DRAW_PANEL_INFO, m_staticDrawPanelInfo);

	DDX_Control(pDX, IDC_COMBO_STRING_FORMAT_FLAGS, m_cmbStringFormatFlags);
	DDX_Control(pDX, IDC_COMBO_ALIGNMENT, m_cmbAlignment);
	DDX_Control(pDX, IDC_COMBO_LINE_ALIGNMENT, m_cmbLineAlignment);
	DDX_Control(pDX, IDC_COMBO_FONT_FAMILY, m_cmbFontFamily);
	DDX_Control(pDX, IDC_COMBO_FONT_STYLE, m_cmbFontStyle);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_cmbUnit);
	DDX_Control(pDX, IDC_COMBO_DRAW_IMAGE_FUNCTION_TYPE, m_cmbDrawImageFunctionType);
	DDX_Control(pDX, IDC_COMBO_COLOR_ADJUST_TYPE, m_cmbColorAdjustType);

	//质量属性
	DDX_Control(pDX, IDC_CHECK_COMPOSITING_MODE, m_btnCheckCompositingMode);
	DDX_Control(pDX, IDC_COMBO_COMPOSITING_MODE, m_cmbCompositingMode);
	DDX_Control(pDX, IDC_CHECK_COMPOSITING_QUALITY, m_btnCheckCompositingQuality);
	DDX_Control(pDX, IDC_COMBO_COMPOSITING_QUALITY, m_cmbCompositingQuality);
	DDX_Control(pDX, IDC_CHECK_INTERPOLATION_MODE, m_btnCheckInterpolationMode);
	DDX_Control(pDX, IDC_COMBO_INTERPOLATION_MODE, m_cmbInterpolationMode);
	DDX_Control(pDX, IDC_CHECK_PIXEL_OFFSET_MODE, m_btnCheckPixelOffsetMode);
	DDX_Control(pDX, IDC_COMBO_PIXEL_OFFSET_MODE, m_cmbPixelOffsetMode);
	DDX_Control(pDX, IDC_CHECK_SMOOTHING_MODE, m_btnCheckSmoothingMode);
	DDX_Control(pDX, IDC_COMBO_SMOOTHING_MODE, m_cmbSmoothingMode);
	DDX_Control(pDX, IDC_CHECK_TEXT_RENDERING_HINT, m_btnCheckTextRenderingHint);
	DDX_Control(pDX, IDC_COMBO_TEXT_RENDERING_HINT, m_cmbTextRenderingHint);

	//坐标和转换
	DDX_Control(pDX, IDC_COMBO_TRANSLATE_ORDER, m_cmbTransformOrder);
	DDX_Control(pDX, IDC_COMBO_GRAPHICS_MATRIX_ORDER, m_cmbGraphicsMatrixOrder);
	DDX_Control(pDX, IDC_CHECK_GRAPHICS_ROTATE, m_btnCheckGraphicsRotate);
	DDX_Text(pDX, IDC_EDIT_GRAPHICS_ROTATE_ANGLE, m_testParam.m_graphicsRotateAngle);
	DDX_Control(pDX, IDC_CHECK_GRAPHICS_SCALE, m_btnCheckGraphicsScale);
	DDX_Text(pDX, IDC_EDIT_GRAPHICS_SCALE_SX, m_testParam.m_graphicsScaleSx);
	DDX_Text(pDX, IDC_EDIT_GRAPHICS_SCALE_SY, m_testParam.m_graphicsScaleSy);
	DDX_Control(pDX, IDC_CHECK_GRAPHICS_TRANSLATE, m_btnCheckGraphicsTranslate);
	DDX_Text(pDX, IDC_EDIT_GRAPHICS_TRANSLATE_DX, m_testParam.m_graphicsTranslateDx);
	DDX_Text(pDX, IDC_EDIT_GRAPHICS_TRANSLATE_DY, m_testParam.m_graphicsTranslateDy);


	DDX_Control(pDX, IDC_BTN_GDI_PLUS_STRING_TEST, m_BtnGdiPlusStringTestMenu);
	DDX_Control(pDX, IDC_BTN_GDI_PLUS_IMAGE_TEST, m_BtnGdiPlusImageTestMenu);

	DDX_Text(pDX, IDC_EDIT_FONT_HEIGHT, m_testParam.m_nFontHeight);
	DDX_Text(pDX, IDC_EDIT_RENDERING_ORIGIN_X, m_testParam.m_nRenderingOriginX);
	DDX_Text(pDX, IDC_EDIT_RENDERING_ORIGIN_Y, m_testParam.m_nRenderingOriginY);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_RANGE_START, m_testParam.m_nCharacterRangeStart);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_RANGE_STOP, m_testParam.m_nCharacterRangeStop);

	DDX_Text(pDX, IDC_EDIT_PAINT_STRING, m_testParam.m_strPaintString);
	DDX_Text(pDX, IDC_EDIT_IMAGE_PATH, m_testParam.m_strPaintImagePath);


}

BEGIN_MESSAGE_MAP(CGdiPlusPage, CPropertyPage)
    ON_WM_DESTROY()
	ON_COMMAND(ID_GDIPLUSTEST_DRAWSTRING, &CGdiPlusPage::OnGdiplustestDrawstring)
	ON_COMMAND(ID_GDIPLUSTEST_MEASURECHARACTERRANGES, &CGdiPlusPage::OnGdiplustestMeasureCharacterRanges)
	ON_COMMAND(ID_IMAGETEST_DRAWIMAGE, &CGdiPlusPage::OnImagetestDrawimage)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_IMAGE, &CGdiPlusPage::OnBnClickedBtnChooseImage)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CGdiPlusPage::_SetDrawPanelInfo(CPoint& ptMouse)
{
	BOOL bRet = TRUE;
	CString strDrawPanelInfo;
	strDrawPanelInfo.Format(TEXT("(%d,%d) at {%d,%d}"), ptMouse.x, ptMouse.y, 
		m_testParam.m_rcDrawPanelClient.Width(), m_testParam.m_rcDrawPanelClient.Height());
	m_staticDrawPanelInfo.SetWindowText(strDrawPanelInfo);

	return bRet;
}

BOOL CGdiPlusPage::_InitFontCollection()
{
	BOOL bRet = TRUE;

	Status sts;
	INT nFontFamilyIndex = 0;
	//枚举系统已经安装的所有字体
	Gdiplus::InstalledFontCollection  fonts;
	INT nFontCount = fonts.GetFamilyCount();
	GDIPLUS_VERIFY(fonts.GetLastStatus());
	if (nFontCount > 0)
	{
		Gdiplus::FontFamily* pFontFamily = new Gdiplus::FontFamily[nFontCount];
		if (pFontFamily)
		{
			INT nFound = 0;
			GDIPLUS_VERIFY(fonts.GetFamilies(nFontCount, pFontFamily, &nFound));
			if (Ok == sts)
			{
				FTLASSERT(nFound == nFontCount);
				FTL::CFStringFormater strFormater;

				TCHAR szFamilyName[LF_FACESIZE] = {0};
				for (INT i = 0; i < nFound; i++)
				{
					GDIPLUS_VERIFY(pFontFamily[i].GetFamilyName(szFamilyName));
					INT nIndex = m_cmbFontFamily.AddString(szFamilyName);
					if (m_testParam.m_strFontFamily.CompareNoCase(szFamilyName) == 0)
					{
						nFontFamilyIndex = nIndex;
					}
					FTLTRACE(TEXT("%s\n"), FTL::CFGdiPlusUtil::GetFontFamilyInfo(strFormater, &pFontFamily[i]));
					//m_cmbFontFamily.SetItemData(nIndex, (DWORD_PTR)&pFontFamily[i]);
				}
			}
			delete [] pFontFamily;
		}
	}
	m_cmbFontFamily.SetCurSel(nFontFamilyIndex);

	return bRet;
}

BOOL CGdiPlusPage::_InitComboboxControls()
{
	BOOL bRet = TRUE;
	Status sts;

	//m_cmbFontStyle.InitControl();
	//FontStyle -- TODO:实际上这些类型是可以组合的
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbFontStyle, FontStyleRegular, TEXT("Regular"), TRUE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbFontStyle, FontStyleBold, TEXT("Bold"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbFontStyle, FontStyleItalic, TEXT("Italic"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbFontStyle, FontStyleBoldItalic, TEXT("BoldItalic"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbFontStyle, FontStyleUnderline, TEXT("Underline"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbFontStyle, FontStyleStrikeout, TEXT("Strikeout"), FALSE);
	//m_cmbFontStyle.SetCurSel(0);



	ADD_COMBO_VALUE_STRING(m_cmbAlignment, StringAlignmentNear);
	ADD_COMBO_VALUE_STRING(m_cmbAlignment, StringAlignmentCenter);
	ADD_COMBO_VALUE_STRING(m_cmbAlignment, StringAlignmentFar);
	m_cmbAlignment.SetCurSel(0);

	ADD_COMBO_VALUE_STRING(m_cmbLineAlignment, StringAlignmentNear);
	ADD_COMBO_VALUE_STRING(m_cmbLineAlignment, StringAlignmentCenter);
	ADD_COMBO_VALUE_STRING(m_cmbLineAlignment, StringAlignmentFar);
	m_cmbLineAlignment.SetCurSel(0);


	//m_cmbStringFormatFlags.ModifyStyle(0xF, )

	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsDirectionRightToLeft, TEXT("DirectionRightToLeft"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsDirectionVertical, TEXT("DirectionVertical"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsNoFitBlackBox, TEXT("NoFitBlackBox"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsDisplayFormatControl, TEXT("DisplayFormatControl"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsNoFontFallback, TEXT("NoFontFallback"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsMeasureTrailingSpaces, TEXT("MeasureTrailingSpaces"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsNoWrap, TEXT("NoWrap"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsLineLimit, TEXT("LineLimit"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsNoClip, TEXT("NoClip"), FALSE);
	ADD_CHECK_COMBO_VALUE_STRING_EX(m_cmbStringFormatFlags, StringFormatFlagsBypassGDI, TEXT("BypassGDI"), FALSE);

	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitWorld, TEXT("World--non-physical unit"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitDisplay, TEXT("Display--for PageTransform only"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitPixel, TEXT("Pixel--one device pixel"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitPoint, TEXT("Point--printer's point, or 1/72 inch"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitInch, TEXT("Inch--1 inch"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitDocument, TEXT("Document--1/300 inch"));		//Each unit is .
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitMillimeter, TEXT("Millimeter--1 millimeter"));
	m_cmbUnit.SetCurSel(3); //Pixel
	
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestRect, TEXT("DestRect"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestPoint, TEXT("DestPoint"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestRectF, TEXT("DestRectF"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestXY, TEXT("DestXY"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestPointF, TEXT("DestPointF"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestRealXY, TEXT("DestRealXY"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestPointArray, TEXT("DestPointArray"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestPointFArray, TEXT("DestPointFArray"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestXYWH, TEXT("DestXYWH"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difDestRealXYWH, TEXT("DestRealXYWH"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difInt6Unit, TEXT("Int6Unit"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difReal6Unit, TEXT("Real6Unit"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difRectFsUnitAttributes, TEXT("RectFsUnitAttributes"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difRectXYWHAttributesAbort, TEXT("RectXYWHAttributesAbort"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difRectFXYHWAttributeAbort, TEXT("RectFXYHWAttributeAbort"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difPointArrayXYHWUnitAttributesAbort, TEXT("PointArrayXYHWUnitAttributesAbort"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difPointFArrayXYHWUnitAttributesAbort, TEXT("PointFArrayXYHWUnitAttributesAbort"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, difRectFMaxtrixEffectAttributesUnit, TEXT("1.1-RectFMaxtrixEffectAttributesUnit"));
	//ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, XXXX, TEXT("XXX"));
	//ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, XXXX, TEXT("XXX"));
	m_cmbDrawImageFunctionType.SetCurSel(0);

	
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypeDefault, TEXT("Default"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypeBitmap, TEXT("Bitmap"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypeBrush, TEXT("Brush"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypePen, TEXT("Pen"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypeText, TEXT("Text"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypeCount, TEXT("Count"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbColorAdjustType, ColorAdjustTypeAny, TEXT("Any"));
	m_cmbColorAdjustType.SetCurSel(0);

	return bRet;
}

BOOL CGdiPlusPage::_InitTestButtons()
{
	BOOL bRet = TRUE;
	m_BtnGdiPlusStringTestMenu.SetSSLButtonStyle(SSL_BS_MENU_BTN);
	m_BtnGdiPlusStringTestMenu.SetSSLButtonMenu(IDR_MENU_GDI_PLUS_TEST, 0);

	m_BtnGdiPlusImageTestMenu.SetSSLButtonStyle(SSL_BS_MENU_BTN);
	m_BtnGdiPlusImageTestMenu.SetSSLButtonMenu(IDR_MENU_GDI_PLUS_TEST, 1);

	return bRet;
}

BOOL CGdiPlusPage::_InitSpinControls()
{
	BOOL bRet = TRUE;
	
	//m_spinRotateAngle.SetBuddy(GetDlgItem(IDC_EDIT_ROTATE_ANGLE));
	//m_spinRotateAngle.SetRange32(0, 360);
	//m_spinRotateAngle.SetPos((INT)m_testParam.m_fRotateAngle);

	return bRet;
}

BOOL CGdiPlusPage::_InitGraphicsQualityControls()
{
	BOOL bRet = TRUE;

	m_btnCheckCompositingMode.SetCheck(m_testParam.m_bEnabledCompositingMode);
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingMode, CompositingModeSourceOver, TEXT("SourceOver"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingMode, CompositingModeSourceCopy, TEXT("SourceCopy"));
	m_cmbCompositingMode.SetCurSel(0);

	m_btnCheckCompositingQuality.SetCheck(m_testParam.m_bEnabledCompositingQuality);
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingQuality, CompositingQualityDefault, TEXT("Default"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingQuality, CompositingQualityHighSpeed, TEXT("HighSpeed"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingQuality, CompositingQualityHighQuality, TEXT("HighQuality"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingQuality, CompositingQualityGammaCorrected, TEXT("GammaCorrected"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbCompositingQuality, CompositingQualityAssumeLinear, TEXT("AssumeLinear"));
	m_cmbCompositingQuality.SetCurSel(0);

	m_btnCheckInterpolationMode.SetCheck(m_testParam.m_bEnabledInterpolationMode);
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeDefault, TEXT("Default"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeLowQuality, TEXT("LowQuality"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeHighQuality, TEXT("HighQuality"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeBilinear, TEXT("Bilinear"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeBicubic, TEXT("Bicubic"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeNearestNeighbor, TEXT("NearestNeighbor"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeHighQualityBilinear, TEXT("HighQualityBilinear"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbInterpolationMode, InterpolationModeHighQualityBicubic, TEXT("HighQualityBicubic"));
	m_cmbInterpolationMode.SetCurSel(0);		

	m_btnCheckPixelOffsetMode.SetCheck(m_testParam.m_bEnabledPixelOffsetMode);
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffsetMode, PixelOffsetModeDefault, TEXT("Default"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffsetMode, PixelOffsetModeHighSpeed, TEXT("HighSpeed"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffsetMode, PixelOffsetModeHighQuality, TEXT("HighQuality"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffsetMode, PixelOffsetModeNone, TEXT("None"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffsetMode, PixelOffsetModeHalf, TEXT("Half"));
	m_cmbPixelOffsetMode.SetCurSel(0);

	m_btnCheckSmoothingMode.SetCheck(m_testParam.m_bEnabledSmoothingMode);
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeDefault, TEXT("Default"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeHighSpeed, TEXT("HighSpeed"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeHighQuality, TEXT("HighQuality"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeNone, TEXT("None"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeAntiAlias, TEXT("AntiAlias"));
#if (GDIPVER >= 0x0110)
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeAntiAlias8x4, TEXT("AntiAlias8x4"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbSmoothingMode, SmoothingModeAntiAlias8x8, TEXT("AntiAlias8x8"));
#endif //(GDIPVER >= 0x0110) 
	m_cmbSmoothingMode.SetCurSel(0);

	m_btnCheckTextRenderingHint.SetCheck(m_testParam.m_bEnabledTextRenderingHint);
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintSystemDefault, TEXT("SystemDefault"));                   // Glyph with system default rendering hint
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintSingleBitPerPixelGridFit, TEXT("BitPerPixelGridFit"));   // Glyph bitmap with hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintSingleBitPerPixel, TEXT("BitPerPixel"));                 // Glyph bitmap without hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintAntiAliasGridFit, TEXT("AntiAliasGridFit"));             // Glyph anti-alias bitmap with hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintAntiAlias, TEXT("AntiAlias"));                           // Glyph anti-alias bitmap without hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintClearTypeGridFit, TEXT("ClearTypeGridFit"));             // Glyph CT bitmap with hinting
	m_cmbTextRenderingHint.SetCurSel(0);

	return bRet;
}

BOOL CGdiPlusPage::_InitGraphicsCoordinateAndTransform()
{
	ADD_COMBO_VALUE_STRING(m_cmbTransformOrder, toRST);
	ADD_COMBO_VALUE_STRING(m_cmbTransformOrder, toRTS);
	ADD_COMBO_VALUE_STRING(m_cmbTransformOrder, toSRT);
	ADD_COMBO_VALUE_STRING(m_cmbTransformOrder, toSTR);
	ADD_COMBO_VALUE_STRING(m_cmbTransformOrder, toTRS);
	ADD_COMBO_VALUE_STRING(m_cmbTransformOrder, toTSR);
	m_cmbTransformOrder.SetCurSel(0);

	ADD_COMBO_VALUE_STRING_EX(m_cmbGraphicsMatrixOrder, MatrixOrderPrepend, TEXT("Prepend"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbGraphicsMatrixOrder, MatrixOrderAppend, TEXT("Append"));
	m_cmbGraphicsMatrixOrder.SetCurSel(0);

	m_btnCheckGraphicsRotate.SetCheck(m_testParam.m_bEnabledGraphicsRotate);
	m_btnCheckGraphicsScale.SetCheck(m_testParam.m_bEnabledGraphicsScale);
	m_btnCheckGraphicsTranslate.SetCheck(m_testParam.m_bEnabledGraphicsTranslate);

	return TRUE;
}

BOOL CGdiPlusPage::_InitControls()
{
    BOOL bRet = FALSE;
    Status sts;
	_SetDrawPanelInfo(CPoint(0, 0));
	_InitFontCollection();
	_InitComboboxControls();
	_InitTestButtons();
	_InitSpinControls();
	
	_InitGraphicsQualityControls();
	_InitGraphicsCoordinateAndTransform();

    return bRet;
}


BOOL CGdiPlusPage::_UnInitControls()
{
    BOOL bRet = TRUE;

    return bRet;
}

BOOL CGdiPlusPage::OnInitDialog()
{
    UpdateData(FALSE);

	m_staticPaint.GetWindowRect(m_testParam.m_rcDrawPanelClient);
	ScreenToClient(&m_testParam.m_rcDrawPanelClient);

    _InitControls();
	return TRUE;
}


BOOL CGdiPlusPage::_GetTestParam(const CRect& rcPaintDest, Gdiplus::Graphics* pGraphics)
{
	BOOL bRet = FALSE;
	API_VERIFY(UpdateData(TRUE));
	if (!bRet)
	{
		return bRet;
	}

	if (m_testParam.m_nCharacterRangeStop < m_testParam.m_nCharacterRangeStart && m_testParam.m_nCharacterRangeStop != -1)
	{
		AfxMessageBox(TEXT("stop Range must bigger than start or -1 indicate all"));
		return FALSE;
	}

	if (m_testParam.m_graphicsScaleSx == 0.0f || m_testParam.m_graphicsScaleSy == 0.0f)
	{
		AfxMessageBox(TEXT("Graphics Scale X or Y Can NOT be Zero"));
		return FALSE;
	}

	m_testParam.m_rtPaintDest.X = rcPaintDest.left;
	m_testParam.m_rtPaintDest.Y = rcPaintDest.top;
	m_testParam.m_rtPaintDest.Width = rcPaintDest.Width();
	m_testParam.m_rtPaintDest.Height = rcPaintDest.Height();

	m_testParam.m_rtfPaintDest.X = rcPaintDest.left;
	m_testParam.m_rtfPaintDest.Y = rcPaintDest.top;
	m_testParam.m_rtfPaintDest.Width = rcPaintDest.Width();
	m_testParam.m_rtfPaintDest.Height = rcPaintDest.Height();


	m_testParam.m_nStringFormatFlags = _GetCheckComboxSelectedFlags(m_cmbStringFormatFlags);
	m_testParam.m_nFontStyle = (FontStyle) _GetCheckComboxSelectedFlags(m_cmbFontStyle);

	m_testParam.m_nAlignment = GET_COMBOBOX_TYPE_ITEMDATA(StringAlignment, m_cmbAlignment);
	m_testParam.m_nLineAlignment = GET_COMBOBOX_TYPE_ITEMDATA(StringAlignment, m_cmbLineAlignment);
	m_cmbFontFamily.GetLBText(m_cmbFontFamily.GetCurSel(), m_testParam.m_strFontFamily);

	m_testParam.m_nUint = GET_COMBOBOX_TYPE_ITEMDATA(Unit, m_cmbUnit);

	m_testParam.m_drawImageFunctionType = GET_COMBOBOX_TYPE_ITEMDATA(DrawImageFunctionType, m_cmbDrawImageFunctionType);
	m_testParam.m_colorAdjustType = GET_COMBOBOX_TYPE_ITEMDATA(ColorAdjustType, m_cmbColorAdjustType);

	//质量属性
	m_testParam.m_bEnabledCompositingMode = m_btnCheckCompositingMode.GetCheck();
	m_testParam.m_compositingMode = GET_COMBOBOX_TYPE_ITEMDATA(CompositingMode, m_cmbCompositingMode);
	m_testParam.m_bEnabledCompositingQuality = m_btnCheckCompositingQuality.GetCheck();
	m_testParam.m_compositingQuality = GET_COMBOBOX_TYPE_ITEMDATA(CompositingQuality, m_cmbCompositingQuality);
	m_testParam.m_bEnabledInterpolationMode = m_btnCheckInterpolationMode.GetCheck();
	m_testParam.m_interpolationMode = GET_COMBOBOX_TYPE_ITEMDATA(InterpolationMode, m_cmbInterpolationMode);
	m_testParam.m_bEnabledPixelOffsetMode = m_btnCheckPixelOffsetMode.GetCheck();
	m_testParam.m_pixelOffsetMode = GET_COMBOBOX_TYPE_ITEMDATA(PixelOffsetMode, m_cmbPixelOffsetMode);
	m_testParam.m_bEnabledSmoothingMode = m_btnCheckSmoothingMode.GetCheck();
	m_testParam.m_smoothingMode = GET_COMBOBOX_TYPE_ITEMDATA(SmoothingMode, m_cmbSmoothingMode);
	m_testParam.m_bEnabledTextRenderingHint = m_btnCheckTextRenderingHint.GetCheck();
	m_testParam.m_textRenderingHint = GET_COMBOBOX_TYPE_ITEMDATA(TextRenderingHint, m_cmbTextRenderingHint);

	//坐标和转换
	m_testParam.m_transformOrder = GET_COMBOBOX_TYPE_ITEMDATA(TransformOrder, m_cmbTransformOrder);
	m_testParam.m_graphicsMatrixOrder = GET_COMBOBOX_TYPE_ITEMDATA(MatrixOrder, m_cmbGraphicsMatrixOrder);
	m_testParam.m_bEnabledGraphicsRotate = m_btnCheckGraphicsRotate.GetCheck();
	m_testParam.m_bEnabledGraphicsScale= m_btnCheckGraphicsScale.GetCheck();
	m_testParam.m_bEnabledGraphicsTranslate= m_btnCheckGraphicsTranslate.GetCheck();

	return bRet;
}

void CGdiPlusPage::_DoGdiPlusTest(GdiPlusTestType testType)
{
	BOOL bRet = FALSE;

    CDC* pDrawDC = m_staticPaint.GetWindowDC();
    {
        Status sts = Ok;
        Graphics graphic(pDrawDC->m_hDC);

		CRect rcPaintDest;
		m_staticPaint.GetClientRect(rcPaintDest);

		pDrawDC->FillSolidRect(rcPaintDest, RGB(0, 0, 255));
		//向内部缩2个像素，可以看出边框
		rcPaintDest.DeflateRect(2, 2);

		API_VERIFY(_GetTestParam(rcPaintDest, &graphic));
		if (bRet)
		{
			FTL::CFStringFormater formater;
			FTLTRACE(TEXT("_DoGdiPlusTest[%d], %s\n"), testType,  m_testParam.GetParamInfo(formater));

			GDIPLUS_VERIFY(_SetGraphicsQualityParam(&graphic));
			GDIPLUS_VERIFY(_SetGraphicsCoordinateAndTransformParam(&graphic));

			switch (testType)
			{
			case gpttDrawString:
				GDIPLUS_VERIFY(_TestDrawString(&graphic));
				break;
			case gpttDrawDriverString:
				{
					//PointF positions[] = 
					//{
					//    PointF(rcBounds.GetLeft(), rcBounds.GetTop()),
					//    PointF(rcBounds.GetRight(), rcBounds.GetBottom()),
					//};
					//GDIPLUS_VERIFY(graphic.DrawDriverString(m_strPaintString, m_strPaintString.GetLength(), 
					//    &font, &brush, positions, ))
					break;
				}
			case gpttMeasureCharacterRanges:
				_TestMeasureCharacterRanges(&graphic);
				break;
			case gpttDrawImage:
				GDIPLUS_VERIFY(_TestDrawImage(&graphic));
				break;
			}
		}

    }

    m_staticPaint.ReleaseDC(pDrawDC);
}

Gdiplus::Status CGdiPlusPage::_SetGraphicsQualityParam(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Status sts = Gdiplus::Ok;
	if (m_testParam.m_bEnabledCompositingMode)
	{
		GDIPLUS_VERIFY(pGraphics->SetCompositingMode(m_testParam.m_compositingMode));
	}
	if (m_testParam.m_bEnabledCompositingQuality)
	{
		GDIPLUS_VERIFY(pGraphics->SetCompositingQuality(m_testParam.m_compositingQuality));
	}
	if (m_testParam.m_bEnabledInterpolationMode)
	{
		GDIPLUS_VERIFY(pGraphics->SetInterpolationMode(m_testParam.m_interpolationMode));
	}
	if (m_testParam.m_bEnabledPixelOffsetMode)
	{
		GDIPLUS_VERIFY(pGraphics->SetPixelOffsetMode(m_testParam.m_pixelOffsetMode));
	}
	if (m_testParam.m_bEnabledTextRenderingHint)
	{
		GDIPLUS_VERIFY(pGraphics->SetTextRenderingHint(m_testParam.m_textRenderingHint));
	}
	if (m_testParam.m_bEnabledSmoothingMode)
	{
		GDIPLUS_VERIFY(pGraphics->SetSmoothingMode(m_testParam.m_smoothingMode));
	}

	return sts;
}

Gdiplus::Status CGdiPlusPage::_SetGraphicsCoordinateAndTransformParam(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Status sts = Gdiplus::Ok;
	MatrixOrder matrixOrder = m_testParam.m_graphicsMatrixOrder;
	switch(m_testParam.m_transformOrder)
	{
	case toRST:
		{
			if (m_testParam.m_bEnabledGraphicsRotate) GDIPLUS_VERIFY(pGraphics->RotateTransform(m_testParam.m_graphicsRotateAngle, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsScale)  GDIPLUS_VERIFY(pGraphics->ScaleTransform(m_testParam.m_graphicsScaleSx, m_testParam.m_graphicsScaleSy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsTranslate) GDIPLUS_VERIFY(pGraphics->TranslateTransform(m_testParam.m_graphicsTranslateDx, m_testParam.m_graphicsTranslateDy, matrixOrder));
			break;
		}
	case toRTS:
		{
			if (m_testParam.m_bEnabledGraphicsRotate) GDIPLUS_VERIFY(pGraphics->RotateTransform(m_testParam.m_graphicsRotateAngle, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsTranslate) GDIPLUS_VERIFY(pGraphics->TranslateTransform(m_testParam.m_graphicsTranslateDx, m_testParam.m_graphicsTranslateDy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsScale)  GDIPLUS_VERIFY(pGraphics->ScaleTransform(m_testParam.m_graphicsScaleSx, m_testParam.m_graphicsScaleSy, matrixOrder));
			break;
		}
	case toSRT:
		{
			if (m_testParam.m_bEnabledGraphicsScale)  GDIPLUS_VERIFY(pGraphics->ScaleTransform(m_testParam.m_graphicsScaleSx, m_testParam.m_graphicsScaleSy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsRotate) GDIPLUS_VERIFY(pGraphics->RotateTransform(m_testParam.m_graphicsRotateAngle, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsTranslate) GDIPLUS_VERIFY(pGraphics->TranslateTransform(m_testParam.m_graphicsTranslateDx, m_testParam.m_graphicsTranslateDy, matrixOrder));
			break;
		}
	case toSTR:
		{
			if (m_testParam.m_bEnabledGraphicsScale)  GDIPLUS_VERIFY(pGraphics->ScaleTransform(m_testParam.m_graphicsScaleSx, m_testParam.m_graphicsScaleSy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsTranslate) GDIPLUS_VERIFY(pGraphics->TranslateTransform(m_testParam.m_graphicsTranslateDx, m_testParam.m_graphicsTranslateDy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsRotate) GDIPLUS_VERIFY(pGraphics->RotateTransform(m_testParam.m_graphicsRotateAngle, matrixOrder));
			break;
		}
	case toTRS:
		{
			if (m_testParam.m_bEnabledGraphicsTranslate) GDIPLUS_VERIFY(pGraphics->TranslateTransform(m_testParam.m_graphicsTranslateDx, m_testParam.m_graphicsTranslateDy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsRotate) GDIPLUS_VERIFY(pGraphics->RotateTransform(m_testParam.m_graphicsRotateAngle, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsScale)  GDIPLUS_VERIFY(pGraphics->ScaleTransform(m_testParam.m_graphicsScaleSx, m_testParam.m_graphicsScaleSy, matrixOrder));
			break;
		}
	case toTSR:
		{
			if (m_testParam.m_bEnabledGraphicsTranslate) GDIPLUS_VERIFY(pGraphics->TranslateTransform(m_testParam.m_graphicsTranslateDx, m_testParam.m_graphicsTranslateDy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsScale)  GDIPLUS_VERIFY(pGraphics->ScaleTransform(m_testParam.m_graphicsScaleSx, m_testParam.m_graphicsScaleSy, matrixOrder));
			if (m_testParam.m_bEnabledGraphicsRotate) GDIPLUS_VERIFY(pGraphics->RotateTransform(m_testParam.m_graphicsRotateAngle, matrixOrder));
			break;
		}

	default:
		{
			FTLASSERT(FALSE);
		}
	}
	return sts;
}

Gdiplus::Status CGdiPlusPage::_TestDrawString(Graphics* pGraphics)
{
	Gdiplus::Status sts = Gdiplus::Ok;

	Gdiplus::Font font(m_testParam.m_strFontFamily, (REAL)m_testParam.m_nFontHeight, m_testParam.m_nFontStyle);
	PointF ptStart(0, 0);

	StringFormat format;
	format.SetAlignment(m_testParam.m_nAlignment);
	format.SetLineAlignment(m_testParam.m_nLineAlignment);
	format.SetFormatFlags(m_testParam.m_nStringFormatFlags);
	SolidBrush brush(m_testParam.m_clrBrush);

	SolidBrush nullBrush(Color::Green);
	pGraphics->FillRectangle(&nullBrush, m_testParam.m_rtPaintDest);

	//GDIPLUS_VERIFY(pGraphics->SetRenderingOrigin(m_testParam.m_nRenderingOriginX, m_testParam.m_nRenderingOriginY));
	//graphic.SetTextContrast()

	RectF rcBounds;
	GDIPLUS_VERIFY(pGraphics->MeasureString(m_testParam.m_strPaintString, m_testParam.m_strPaintString.GetLength(),
		&font, m_testParam.m_rtfPaintDest, &format, &rcBounds));

	//pDrawDC->Rectangle(rcBounds.X, rcBounds.Y, rcBounds.GetRight(), rcBounds.GetBottom());

	Gdiplus::Pen penBounds(Color::Aqua);
	GDIPLUS_VERIFY(pGraphics->DrawRectangle(&penBounds, rcBounds));

	GDIPLUS_VERIFY(pGraphics->DrawString(m_testParam.m_strPaintString, m_testParam.m_strPaintString.GetLength(),
		&font, m_testParam.m_rtfPaintDest, &format,  &brush));

	return sts;
}

Gdiplus::Status CGdiPlusPage::_TestMeasureCharacterRanges(Gdiplus::Graphics* pGraphics)
{
	Gdiplus::Status sts = Gdiplus::InvalidParameter;

	Gdiplus::Font font(m_testParam.m_strFontFamily, (REAL)m_testParam.m_nFontHeight, m_testParam.m_nFontStyle);

	StringFormat format;
	format.SetAlignment(m_testParam.m_nAlignment);
	format.SetLineAlignment(m_testParam.m_nLineAlignment);
	format.SetFormatFlags(m_testParam.m_nStringFormatFlags);
	SolidBrush brush(m_testParam.m_clrBrush);

	GDIPLUS_VERIFY(pGraphics->DrawString(m_testParam.m_strPaintString, m_testParam.m_strPaintString.GetLength(), 
		&font, m_testParam.m_rtfPaintDest, &format,  &brush));

	INT nStart = 0, nLength = 0;
	m_testParam.GetCharacterRangeInfo(nStart, nLength);
	CharacterRange range(nStart, nLength);
	GDIPLUS_VERIFY(format.SetMeasurableCharacterRanges(1, &range));

	Region region;
	GDIPLUS_VERIFY(pGraphics->MeasureCharacterRanges(m_testParam.m_strPaintString, m_testParam.m_strPaintString.GetLength(), 
		&font, m_testParam.m_rtfPaintDest, &format, 1, &region));
	if (Gdiplus::Ok == sts)
	{
		SolidBrush blueBrush((ARGB)Color::Blue);
		GDIPLUS_VERIFY(pGraphics->FillRegion(&blueBrush, &region));
	}
	return sts;
}

Gdiplus::Status CGdiPlusPage::_TestDrawImage(Graphics* pGraphics)
{
	Gdiplus::Status sts = Gdiplus::InvalidParameter;

	if (!m_testParam.m_strPaintImagePath.IsEmpty())
	{
		Image* pImage = NULL;
		{
			FUNCTION_BLOCK_NAME_TRACE(TEXT("Image::FromFile"), 10);
			pImage = Image::FromFile(m_testParam.m_strPaintImagePath, FALSE);
		}

		if (pImage)
		{
			FUNCTION_BLOCK_NAME_TRACE(TEXT("DrawImage"), 10);
			GDIPLUS_VERIFY(pImage->GetLastStatus());
			
			switch (m_testParam.m_drawImageFunctionType)
			{
			case difDestRect:
				{
					//Point points[] = { Point(0, 0), Point(pImage->GetWidth(), 0), Point(0, pImage->GetHeight())};
					//Matrix matrix(1,0,0,1,10,10); // 定义一个单位矩阵，坐标原点在(10,10)
					//matrix.Rotate(30); // 顺时针旋转30度
					//matrix.Scale(0.1,0.1); // X 和 Y 方向分别乘以0.63和0.6比例因子
					//matrix.TransformPoints(points, 3); // 用该矩阵转换points
					//pGraphics->DrawImage(pImage, points, 3);
					//Point newpoints[] = {Point(450, 10), Point(510, 60), Point(350, 80)};
					//pGraphics->DrawImage(pImage, newpoints, 3);
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, m_testParam.m_rtPaintDest));
					break;
				}
				
			case difDestPoint:
				{
					Point ptLeftTop(m_testParam.m_rtPaintDest.X, m_testParam.m_rtPaintDest.Y);
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, ptLeftTop));
					break;
				}
			case difDestRectF:
				{
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, m_testParam.m_rtfPaintDest));
					break;
				}
			case difDestXY:
				{
					INT x = m_testParam.m_rtPaintDest.X;
					INT y = m_testParam.m_rtPaintDest.Y;
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, x, y));
					break;
				}
			case difDestPointF:
				{
					PointF ptfLeftTop(m_testParam.m_rtfPaintDest.X, m_testParam.m_rtfPaintDest.Y);
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, ptfLeftTop));
					break;
				}
			case difDestRealXY:
				{
					REAL x = m_testParam.m_rtfPaintDest.X;
					REAL y = m_testParam.m_rtfPaintDest.Y;
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, x, y));
					break;
				}
			case difDestPointArray:
				{
					Point destPoints[3] = {
						Point(m_testParam.m_rtPaintDest.X, m_testParam.m_rtPaintDest.Y),
						Point(m_testParam.m_rtPaintDest.X + m_testParam.m_rtPaintDest.Width / 2, m_testParam.m_rtPaintDest.Y ),
						Point(m_testParam.m_rtPaintDest.X + m_testParam.m_rtPaintDest.Width / 2, m_testParam.m_rtPaintDest.GetBottom())
					};
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destPoints, _countof(destPoints)));
					break;
				}
			case difDestPointFArray:
				{
					PointF destPointFs[3] = {
						PointF(m_testParam.m_rtfPaintDest.X, m_testParam.m_rtfPaintDest.Y),
						PointF(m_testParam.m_rtfPaintDest.X + m_testParam.m_rtfPaintDest.Width / 2.0f, m_testParam.m_rtfPaintDest.GetBottom()),
						PointF(m_testParam.m_rtfPaintDest.X + m_testParam.m_rtfPaintDest.Width / 2.0f, m_testParam.m_rtfPaintDest.Y )
					};
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destPointFs, _countof(destPointFs)));
					break;
				}
			case difDestXYWH:
				{
					INT x = m_testParam.m_rtPaintDest.X;
					INT y = m_testParam.m_rtPaintDest.Y;
					INT width = m_testParam.m_rtPaintDest.Width;
					INT height = m_testParam.m_rtPaintDest.Height;
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, x, y, width, height));
					break;
				}
			case difDestRealXYWH:
				{
					REAL x = m_testParam.m_rtfPaintDest.X;
					REAL y = m_testParam.m_rtfPaintDest.Y;
					REAL width = m_testParam.m_rtfPaintDest.Width;
					REAL height = m_testParam.m_rtfPaintDest.Height;
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, x, y, width, height));
					break;
				}
			case difInt6Unit:
				{
					INT x = m_testParam.m_rtPaintDest.X;
					INT y = m_testParam.m_rtPaintDest.Y;
					//绘制 1/4 ~ 3/4 部分
					INT srcx = pImage->GetWidth() / 4;
					INT srcy = pImage->GetHeight() / 4;
					INT srcwidth =  pImage->GetWidth() / 2;
					INT srcheight = pImage->GetHeight() / 2;
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, x, y, srcx, srcy, srcwidth, srcheight, m_testParam.m_nUint));
					break;
				}
			case difReal6Unit:
				{
					REAL x = m_testParam.m_rtfPaintDest.X;
					REAL y = m_testParam.m_rtfPaintDest.Y;
					//绘制 1/4 ~ 3/4 部分
					REAL srcx = (REAL)pImage->GetWidth() / 4.0f;
					REAL srcy = (REAL)pImage->GetHeight() / 4.0f;
					REAL srcwidth = (REAL)pImage->GetWidth() / 2.0f;
					REAL srcheight = (REAL)pImage->GetHeight() / 2.0f;
					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, x, y, srcx, srcy, srcwidth, srcheight, m_testParam.m_nUint));
					break;
				}
			case difRectFsUnitAttributes:
				{
					RectF destRect;
					m_testParam.m_rtfPaintDest.GetBounds(&destRect);
					RectF sourceRect((REAL)pImage->GetWidth() / 4.0f, (REAL)pImage->GetHeight() / 4.0f,
						(REAL)pImage->GetWidth() / 2.0f, (REAL)pImage->GetHeight() / 2.0f);

					ImageAttributes attributes;
					_CalcImageAttributes(attributes);

					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destRect, sourceRect, m_testParam.m_nUint, &attributes));
					break;
				}
			case difRectXYWHAttributesAbort:
				{
					Rect destRect;
					m_testParam.m_rtPaintDest.GetBounds(&destRect);
					INT srcx = pImage->GetWidth() / 4;
					INT srcy = pImage->GetHeight() / 4;
					INT srcwidth = pImage->GetWidth() / 2;
					INT srcheight = pImage->GetHeight() / 2;

					ImageAttributes attributes;
					_CalcImageAttributes(attributes);

					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destRect, srcx, srcy, srcwidth, srcheight,
						m_testParam.m_nUint, &attributes, _DrawImageAbortProc, this));
					break;
				}
			case difRectFXYHWAttributeAbort:
				{
					RectF destRect;
					m_testParam.m_rtfPaintDest.GetBounds(&destRect);
					REAL srcx = (REAL)pImage->GetWidth() / 4.0f;
					REAL srcy = (REAL)pImage->GetHeight() / 4.0f;
					REAL srcwidth = (REAL)pImage->GetWidth() / 2.0f;
					REAL srcheight = (REAL)pImage->GetHeight() / 2.0f;

					ImageAttributes attributes;
					_CalcImageAttributes(attributes);

					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destRect, srcx, srcy, srcwidth, srcheight,
						m_testParam.m_nUint, &attributes, _DrawImageAbortProc, this));
					break;
				}
			case difPointArrayXYHWUnitAttributesAbort:
				{
					Point destPoints[3] = {
						Point(m_testParam.m_rtPaintDest.X, m_testParam.m_rtPaintDest.Y),
						Point(m_testParam.m_rtPaintDest.X + m_testParam.m_rtPaintDest.Width / 2, m_testParam.m_rtPaintDest.Y ),
						Point(m_testParam.m_rtPaintDest.X + m_testParam.m_rtPaintDest.Width / 2, m_testParam.m_rtPaintDest.GetBottom())
					};
					INT srcx = pImage->GetWidth() / 4;
					INT srcy = pImage->GetHeight() / 4;
					INT srcwidth = pImage->GetWidth() / 2;
					INT srcheight = pImage->GetHeight() / 2;

					ImageAttributes attributes;
					_CalcImageAttributes(attributes);

					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destPoints, _countof(destPoints),
						srcx, srcy, srcwidth, srcheight, 
						m_testParam.m_nUint, &attributes, _DrawImageAbortProc, this))
					break;
				}
			case difPointFArrayXYHWUnitAttributesAbort:
				{
					PointF destPointFs[3] = {
						PointF(m_testParam.m_rtfPaintDest.X, m_testParam.m_rtfPaintDest.Y),
						PointF(m_testParam.m_rtfPaintDest.X + m_testParam.m_rtfPaintDest.Width / 2.0f, m_testParam.m_rtfPaintDest.GetBottom()),
						PointF(m_testParam.m_rtfPaintDest.X + m_testParam.m_rtfPaintDest.Width / 2.0f, m_testParam.m_rtfPaintDest.Y )
					};

					REAL srcx = (REAL)pImage->GetWidth() / 4.0f;
					REAL srcy = (REAL)pImage->GetHeight() / 4.0f;
					REAL srcwidth = (REAL)pImage->GetWidth() / 2.0f;
					REAL srcheight = (REAL)pImage->GetHeight() / 2.0f;

					ImageAttributes attributes;
					_CalcImageAttributes(attributes);

					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, destPointFs, _countof(destPointFs),
						srcx, srcy, srcwidth, srcheight, 
						m_testParam.m_nUint, &attributes, _DrawImageAbortProc, this))

					break;
				}
			case difRectFMaxtrixEffectAttributesUnit:
				{
					RectF sourceRect((REAL)pImage->GetWidth() / 4.0f, (REAL)pImage->GetHeight() / 4.0f,
						(REAL)pImage->GetWidth() / 2.0f, (REAL)pImage->GetHeight() / 2.0f);
					
					Matrix xForm(1,0,0,1,5,5); // 定义一个单位矩阵，坐标原点在(5,5)
					_CalcImageMatrix(xForm);

					Effect effect;
					_CalcImageEffect(effect);

					ImageAttributes attributes;
					_CalcImageAttributes(attributes);

					GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, &sourceRect, &xForm, &effect, &attributes, m_testParam.m_nUint));
					break;
				}
			default:
				FTLASSERT(FALSE);
				break;
			}
			
#if 0
			//Matrix matrix;
			//GDIPLUS_VERIFY(matrix.Rotate(m_testParam.m_fRotateAngle));

			REAL srcWidth = (REAL)pImage->GetWidth();
			REAL srcHeight = (REAL)pImage->GetHeight();

			Matrix matrix(2.0f, 0.2f, 0.3f, 1.0f, 3.0f, 2.0f);

			Gdiplus::Effect effect;

			BrightnessContrastParams briConParams;
			briConParams.brightnessLevel = 0;
			briConParams.contrastLevel = 30;
			BrightnessContrast briCon;
			GDIPLUS_VERIFY(briCon.SetParameters(&briConParams));

			ImageAttributes imageAttributes;
			//GDIPLUS_VERIFY(graphic.SetPageUnit(m_testParam.m_nUint));
			//GDIPLUS_VERIFY(graphic.DrawImage(pImage, &rcPaintGdiP, &matrix, &effect, &imageAttributes, m_testParam.m_nUint));
			GDIPLUS_VERIFY(graphic.DrawImage(pImage, &srcRect, &matrix, &briCon, NULL, m_testParam.m_nUint));

			//GDIPLUS_VERIFY(graphic.DrawImage(pImage, rcPaintGdiP));
#endif 
			delete pImage;
		}
	}


	return sts;
}

Gdiplus::Status CGdiPlusPage::_CalcImageAttributes(ImageAttributes& attributes)
{
	Gdiplus::Status sts = Gdiplus::InvalidParameter;
	GDIPLUS_VERIFY(attributes.SetNoOp(m_testParam.m_colorAdjustType));
	return sts;
}

Gdiplus::Status CGdiPlusPage::_CalcImageEffect(Gdiplus::Effect& effect)
{
	Gdiplus::Status sts = Gdiplus::InvalidParameter;
	return sts;
}

Gdiplus::Status CGdiPlusPage::_CalcImageMatrix(Gdiplus::Matrix& matrix)
{
	Gdiplus::Status sts = Gdiplus::InvalidParameter;
	return sts;
}


INT  CGdiPlusPage::_GetCheckComboxSelectedFlags(const CCheckComboBox& checkCombobox)
{
	INT nFlags = 0;
	int nCount = checkCombobox.GetCount();
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		if (checkCombobox.GetCheck(nIndex))
		{
			nFlags |= checkCombobox.GetItemData(nIndex);
		}
	}
	return nFlags;

}

void CGdiPlusPage::OnDestroy()
{
    _UnInitControls();

    CPropertyPage::OnDestroy();
}

void CGdiPlusPage::OnGdiplustestDrawstring()
{
	_DoGdiPlusTest(gpttDrawString);
}

void CGdiPlusPage::OnGdiplustestMeasureCharacterRanges()
{
	_DoGdiPlusTest(gpttMeasureCharacterRanges);
}

void CGdiPlusPage::OnImagetestDrawimage()
{
	_DoGdiPlusTest(gpttDrawImage);
}

void CGdiPlusPage::OnBnClickedBtnChooseImage()
{
	CFileDialog dlg(TRUE, NULL, m_testParam.m_strPaintImagePath);
	if (dlg.DoModal())
	{
		m_testParam.m_strPaintImagePath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CGdiPlusPage::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_testParam.m_rcDrawPanelClient.PtInRect(point))
	{
		ClientToScreen(&point);
		m_staticPaint.ScreenToClient(&point);
		_SetDrawPanelInfo(point);	
	}

	CPropertyPage::OnMouseMove(nFlags, point);
}
