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
	m_nFontHeight = 10;
	m_TextRenderingHint = Gdiplus::TextRenderingHintSystemDefault;
	m_pixelOffsetMode = Gdiplus::PixelOffsetModeDefault;
	m_nRenderingOriginX = 0;
	m_nRenderingOriginY = 0;
	m_nCharacterRangeStart = 0;
	m_nCharacterRangeStop = -1;
	m_nStringFormatFlags = 0;
	m_fRotateAngle = 90.0f;
	m_nUint = UnitPixel;
	m_drawImageFunctionType = difDestRect;

	m_nAlignment = StringAlignmentNear;
	m_nLineAlignment = StringAlignmentNear;
	m_nFontStyle = FontStyleRegular;
	m_clrBrush = (ARGB)Color::Red;

	m_strPaintString = _T("测试文字,有中文有 English，如 A B C 和 abc");
	m_strPaintImagePath = _T("C:\\Users\\Public\\Pictures\\Sample Pictures\\Koala.jpg");
	m_strFontFamily = _T("Arial");
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

void CGdiPlusPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_STRING_FORMAT_FLAGS, m_cmbStringFormatFlags);
	DDX_Control(pDX, IDC_COMBO_ALIGNMENT, m_cmbAlignment);
	DDX_Control(pDX, IDC_COMBO_LINE_ALIGNMENT, m_cmbLineAlignment);
	DDX_Control(pDX, IDC_COMBO_TEXT_RENDERING_HINT, m_cmbTextRenderingHint);
	DDX_Control(pDX, IDC_COMBO_FONT_FAMILY, m_cmbFontFamily);
	DDX_Control(pDX, IDC_COMBO_PIXEL_OFFSET, m_cmbPixelOffset);
	DDX_Control(pDX, IDC_COMBO_FONT_STYLE, m_cmbFontStyle);
	DDX_Control(pDX, IDC_COMBO_UNIT, m_cmbUnit);
	DDX_Control(pDX, IDC_COMBO_DRAW_IMAGE_FUNCTION_TYPE, m_cmbDrawImageFunctionType);

	DDX_Control(pDX, IDC_SPIN_ROTATE_ANGLE, m_spinRotateAngle);

	DDX_Control(pDX, IDC_STATIC_PAINT, m_staticPaint);

	DDX_Control(pDX, IDC_BTN_GDI_PLUS_STRING_TEST, m_BtnGdiPlusStringTestMenu);
	DDX_Control(pDX, IDC_BTN_GDI_PLUS_IMAGE_TEST, m_BtnGdiPlusImageTestMenu);

	DDX_Text(pDX, IDC_EDIT_FONT_HEIGHT, m_testParam.m_nFontHeight);
    DDX_Text(pDX, IDC_EDIT_RENDERING_ORIGIN_X, m_testParam.m_nRenderingOriginX);
    DDX_Text(pDX, IDC_EDIT_RENDERING_ORIGIN_Y, m_testParam.m_nRenderingOriginY);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_RANGE_START, m_testParam.m_nCharacterRangeStart);
	DDX_Text(pDX, IDC_EDIT_CHARACTER_RANGE_STOP, m_testParam.m_nCharacterRangeStop);
	DDX_Text(pDX, IDC_EDIT_ROTATE_ANGLE, m_testParam.m_fRotateAngle);

    DDX_Text(pDX, IDC_EDIT_PAINT_STRING, m_testParam.m_strPaintString);
	DDX_Text(pDX, IDC_EDIT_IMAGE_PATH, m_testParam.m_strPaintImagePath);


}

BEGIN_MESSAGE_MAP(CGdiPlusPage, CPropertyPage)
    ON_WM_DESTROY()
	ON_COMMAND(ID_GDIPLUSTEST_DRAWSTRING, &CGdiPlusPage::OnGdiplustestDrawstring)
	ON_COMMAND(ID_GDIPLUSTEST_MEASURECHARACTERRANGES, &CGdiPlusPage::OnGdiplustestMeasureCharacterRanges)
	ON_COMMAND(ID_IMAGETEST_DRAWIMAGE, &CGdiPlusPage::OnImagetestDrawimage)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_IMAGE, &CGdiPlusPage::OnBnClickedBtnChooseImage)
END_MESSAGE_MAP()

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


	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintSystemDefault, TEXT("SystemDefault"));                   // Glyph with system default rendering hint
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintSingleBitPerPixelGridFit, TEXT("BitPerPixelGridFit"));   // Glyph bitmap with hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintSingleBitPerPixel, TEXT("BitPerPixel"));                 // Glyph bitmap without hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintAntiAliasGridFit, TEXT("AntiAliasGridFit"));             // Glyph anti-alias bitmap with hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintAntiAlias, TEXT("AntiAlias"));                           // Glyph anti-alias bitmap without hinting
	ADD_COMBO_VALUE_STRING_EX(m_cmbTextRenderingHint, TextRenderingHintClearTypeGridFit, TEXT("ClearTypeGridFit"));             // Glyph CT bitmap with hinting
	m_cmbTextRenderingHint.SetCurSel(0);

	ADD_COMBO_VALUE_STRING(m_cmbAlignment, StringAlignmentNear);
	ADD_COMBO_VALUE_STRING(m_cmbAlignment, StringAlignmentCenter);
	ADD_COMBO_VALUE_STRING(m_cmbAlignment, StringAlignmentFar);
	m_cmbAlignment.SetCurSel(0);

	ADD_COMBO_VALUE_STRING(m_cmbLineAlignment, StringAlignmentNear);
	ADD_COMBO_VALUE_STRING(m_cmbLineAlignment, StringAlignmentCenter);
	ADD_COMBO_VALUE_STRING(m_cmbLineAlignment, StringAlignmentFar);
	m_cmbLineAlignment.SetCurSel(0);

	//ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffset, PixelOffsetModeInvalid, TEXT("Invalid"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffset, PixelOffsetModeDefault, TEXT("Default"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffset, PixelOffsetModeHighSpeed, TEXT("HighSpeed"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffset, PixelOffsetModeHighQuality, TEXT("HighQuality"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffset, PixelOffsetModeNone, TEXT("None"));
	ADD_COMBO_VALUE_STRING_EX(m_cmbPixelOffset, PixelOffsetModeHalf, TEXT("Half"));
	m_cmbPixelOffset.SetCurSel(0);

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

	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitWorld, TEXT("World"));				//World coordinate (non-physical unit)
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitDisplay, TEXT("Display"));			//Variable -- for PageTransform only
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitPixel, TEXT("Pixel"));				//Each unit is one device pixel.
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitPoint, TEXT("Point"));				//Each unit is a printer's point, or 1/72 inch.
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitInch, TEXT("Inch"));				//Each unit is 1 inch.
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitDocument, TEXT("Document"));		//Each unit is 1/300 inch.
	ADD_COMBO_VALUE_STRING_EX(m_cmbUnit, UnitMillimeter, TEXT("Millimeter"));	//Each unit is 1 millimeter.
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
	//ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, XXXX, TEXT("XXX"));
	//ADD_COMBO_VALUE_STRING_EX(m_cmbDrawImageFunctionType, XXXX, TEXT("XXX"));
	
	m_cmbDrawImageFunctionType.SetCurSel(0);
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
	
	m_spinRotateAngle.SetBuddy(GetDlgItem(IDC_EDIT_ROTATE_ANGLE));
	m_spinRotateAngle.SetRange32(0, 360);
	m_spinRotateAngle.SetPos((INT)m_testParam.m_fRotateAngle);

	return bRet;
}

BOOL CGdiPlusPage::_InitControls()
{
    BOOL bRet = FALSE;
    Status sts;
	
	_InitFontCollection();
	_InitComboboxControls();
	_InitTestButtons();
	_InitSpinControls();

    return bRet;
}


BOOL CGdiPlusPage::_UnInitControls()
{
    BOOL bRet = TRUE;

    INT nFontCount = m_cmbFontFamily.GetCount();
    //for (INT nIndex = 0; nIndex < nFontCount; nIndex++)
    //{
    //    FontFamily* pFontFamily = (FontFamily*)m_cmbFontFamily.GetItemData(nIndex);
    //    if (pFontFamily)
    //    {
    //        delete pFontFamily;
    //        m_cmbFontFamily.SetItemData(nIndex, NULL);
    //    }
    //}
    
#if 0
	m_cmbAlignment.ResetContent();
	m_cmbLineAlignment.ResetContent();
	m_cmbTextRenderingHint.ResetContent();
	m_cmbStringFormatFlags.ResetContent();
	m_cmbFontFamily.ResetContent();
	m_cmbPixelOffset.ResetContent();
	m_cmbFontStyle.ResetContent();
#endif 

    return bRet;
}

BOOL CGdiPlusPage::OnInitDialog()
{
    UpdateData(FALSE);
    _InitControls();
	return TRUE;
}

BOOL CGdiPlusPage::_GetTestParam(const CRect& rcPaintDest, Gdiplus::Graphics* pGraphics)
{
	BOOL bRet = FALSE;
	API_VERIFY(UpdateData(FALSE));
	if (!bRet)
	{
		return bRet;
	}

	if (m_testParam.m_nCharacterRangeStop < m_testParam.m_nCharacterRangeStart && m_testParam.m_nCharacterRangeStop != -1)
	{
		AfxMessageBox(TEXT("stop Range must bigger than start or -1 indicate all"));
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

	m_testParam.m_nAlignment = (StringAlignment)m_cmbAlignment.GetItemData(m_cmbAlignment.GetCurSel());
	m_testParam.m_nLineAlignment = (StringAlignment)m_cmbLineAlignment.GetItemData(m_cmbLineAlignment.GetCurSel());
	m_cmbFontFamily.GetLBText(m_cmbFontFamily.GetCurSel(), m_testParam.m_strFontFamily);

	m_testParam.m_TextRenderingHint = (TextRenderingHint)m_cmbTextRenderingHint.GetItemData(m_cmbTextRenderingHint.GetCurSel());
	m_testParam.m_pixelOffsetMode = (PixelOffsetMode)m_cmbPixelOffset.GetItemData(m_cmbPixelOffset.GetCurSel());

	m_testParam.m_drawImageFunctionType = (DrawImageFunctionType)m_cmbDrawImageFunctionType.GetItemData(m_cmbDrawImageFunctionType.GetCurSel());


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


		FTL::CFStringFormater formater;
        FTLTRACE(TEXT("_DoGdiPlusTest[%d], %s\n"), testType,  m_testParam.GetParamInfo(formater));

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

    m_staticPaint.ReleaseDC(pDrawDC);
}

Gdiplus::Status CGdiPlusPage::_TestDrawString(Graphics* pGraphics)
{
	Gdiplus::Status sts = Gdiplus::InvalidParameter;

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


	GDIPLUS_VERIFY(pGraphics->SetTextRenderingHint(m_testParam.m_TextRenderingHint));
	//pGraphics->SetPixelOffsetMode(m_testParam.m_pixelOffsetMode);
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
				GDIPLUS_VERIFY(pGraphics->DrawImage(pImage, m_testParam.m_rtPaintDest));
				break;
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
						PointF(m_testParam.m_rtfPaintDest.X + m_testParam.m_rtfPaintDest.Width / 2, m_testParam.m_rtfPaintDest.GetBottom()),
						PointF(m_testParam.m_rtfPaintDest.X + m_testParam.m_rtfPaintDest.Width / 2, m_testParam.m_rtfPaintDest.Y )
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
