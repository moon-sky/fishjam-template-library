// GuiPlusPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "GdiPlusPage.h"
#include <ftlGdiPlus.h>
#include <ftlDebug.h>

using namespace Gdiplus;
// CGdiPlusPage dialog

IMPLEMENT_DYNAMIC(CGdiPlusPage, CPropertyPage)

#define ADD_COMBO_VALUE_STRING_EX(cmb, flag, str) \
{\
    int nIndex = cmb.AddString(str);\
    cmb.SetItemData(nIndex, flag);\
}

#define ADD_COMBO_VALUE_STRING(cmb, flag) ADD_COMBO_VALUE_STRING_EX(cmb, flag, TEXT(#flag))

CGdiPlusPage::CGdiPlusPage()
	: CPropertyPage(CGdiPlusPage::IDD)
    , m_nFontHeight(10)
    , m_fontStyle(Gdiplus::FontStyleRegular)
    , m_TextRenderingHint(Gdiplus::TextRenderingHintSystemDefault)
    , m_pixelOffsetMode(Gdiplus::PixelOffsetModeDefault)
    , m_nRenderingOriginX(0)
    , m_nRenderingOriginY(0)
    , m_strPaintString(_T("测试文字,有中文有 English，如 A B C 和 abc"))
    , m_strFontFamily (_T("Arial"))
{

}

CGdiPlusPage::~CGdiPlusPage()
{
}

void CGdiPlusPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_STATIC_PAINT, m_staticPaint);
    DDX_Text(pDX, IDC_EDIT_FONT_HEIGHT, m_nFontHeight);
    DDX_Control(pDX, IDC_COMBO_FONT_STYLE, m_cmbFontStyle);

    DDX_Text(pDX, IDC_EDIT_RENDERING_ORIGIN_X, m_nRenderingOriginX);
    DDX_Text(pDX, IDC_EDIT_RENDERING_ORIGIN_Y, m_nRenderingOriginY);
    DDX_Text(pDX, IDC_EDIT_PAINT_STRING, m_strPaintString);
    //DDX_Control(pDX, IDC_LIST_DRAW_FORMAT, m_lstDrawFormat);
    DDX_Control(pDX, IDC_LIST_STRING_FORMAT_FLAGS, m_lstStringFormatFlags);

    DDX_Control(pDX, IDC_COMBO_ALIGNMENT, m_cmbAlignment);
    DDX_Control(pDX, IDC_COMBO_LINE_ALIGNMENT, m_cmbLineAlignment);
    DDX_Control(pDX, IDC_COMBO_TEXT_RENDERING_HINT, m_cmbTextRenderingHint);
    DDX_Control(pDX, IDC_COMBO_FONT_FAMILY, m_cmbFontFamily);
    DDX_Control(pDX, IDC_COMBO_PIXEL_OFFSET, m_cmbPixelOffset);
}

BEGIN_MESSAGE_MAP(CGdiPlusPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_DRAW_STRING, &CGdiPlusPage::OnBnClickedBtnDrawString)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_DRAW_DRIVER_STRING, &CGdiPlusPage::OnBnClickedBtnDrawDriverString)
END_MESSAGE_MAP()

BOOL CGdiPlusPage::_InitControls()
{
    BOOL bRet = FALSE;
    Status sts;

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
                    
                    FTLTRACE(TEXT("%s\n"), FTL::CFGdiPlusUtil::GetFontFamilyInfo(strFormater, &pFontFamily[i]));
                    //m_cmbFontFamily.SetItemData(nIndex, (DWORD_PTR)&pFontFamily[i]);
                }
            }
            delete [] pFontFamily;
        }
    }
    if (m_cmbFontFamily.GetCount() == 0)
    {
        //m_cmbFontFamily.AddString(TEXT("宋体"));
    }
    m_cmbFontFamily.SetCurSel(0);

    //FontStyle -- TODO:实际上这些类型是可以组合的
    ADD_COMBO_VALUE_STRING(m_cmbFontStyle, FontStyleRegular);
    ADD_COMBO_VALUE_STRING(m_cmbFontStyle, FontStyleBold);
    ADD_COMBO_VALUE_STRING(m_cmbFontStyle, FontStyleItalic);
    ADD_COMBO_VALUE_STRING(m_cmbFontStyle, FontStyleBoldItalic);
    ADD_COMBO_VALUE_STRING(m_cmbFontStyle, FontStyleUnderline);
    ADD_COMBO_VALUE_STRING(m_cmbFontStyle, FontStyleStrikeout);
    m_cmbFontStyle.SetCurSel(0);


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

    

    m_lstStringFormatFlags.ModifyStyle(LVS_ICON, LVS_REPORT, 0);
    m_lstStringFormatFlags.SetExtendedStyle(m_lstStringFormatFlags.GetExtendedStyle() | LVS_EX_CHECKBOXES);

    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    for(int i = 0; i< 1; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = TEXT("Flags");
        lvc.cx = 180;
        lvc.fmt = LVCFMT_LEFT;
        m_lstStringFormatFlags.InsertColumn(i,&lvc);
    }

    LV_ITEM lvi = {0};

    typedef std::pair<LPCTSTR, StringFormatFlags> StringFormatFlagsInfoType;
    StringFormatFlagsInfoType flagInfos[] = 
    {
        std::make_pair(TEXT("DirectionRightToLeft"),StringFormatFlagsDirectionRightToLeft),
        std::make_pair(TEXT("DirectionVertical"),StringFormatFlagsDirectionVertical),
        std::make_pair(TEXT("NoFitBlackBox"),StringFormatFlagsNoFitBlackBox),
        std::make_pair(TEXT("DisplayFormatControl"),StringFormatFlagsDisplayFormatControl),
        std::make_pair(TEXT("NoFontFallback"),StringFormatFlagsNoFontFallback),
        std::make_pair(TEXT("MeasureTrailingSpaces"),StringFormatFlagsMeasureTrailingSpaces),
        std::make_pair(TEXT("NoWrap"),StringFormatFlagsNoWrap),
        std::make_pair(TEXT("LineLimit"),StringFormatFlagsLineLimit),
        std::make_pair(TEXT("NoClip"),StringFormatFlagsNoClip),
        std::make_pair(TEXT("BypassGDI"),StringFormatFlagsBypassGDI),
    };

    for (int i = 0; i < _countof(flagInfos); i++)
    {
        lvi.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;
        lvi.stateMask = LVIS_STATEIMAGEMASK;
        lvi.state = INDEXTOSTATEIMAGEMASK(2);
        lvi.iSubItem = 0;

        lvi.iItem = i;
        lvi.pszText = const_cast<LPTSTR>(flagInfos[i].first);
        //lvi.iImage = i;
        int nIndex = m_lstStringFormatFlags.InsertItem(&lvi);
        API_VERIFY(m_lstStringFormatFlags.SetItemData(nIndex, flagInfos[nIndex].second));
        API_VERIFY(m_lstStringFormatFlags.SetCheck(nIndex, FALSE));
    }

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
    m_cmbFontFamily.ResetContent();

    return bRet;
}

BOOL CGdiPlusPage::OnInitDialog()
{
    UpdateData(FALSE);

    _InitControls();

	return TRUE;
}

void CGdiPlusPage::_MeasureString(Gdiplus::Graphics* pGraphics)
{
#if 0
    CEdit* pEditString = (CEdit*)GetDlgItem(IDC_EDIT_GDIPLUS_MEASURE_STRING);
    if (pEditString)
    {
        Gdiplus::Status sts = Ok;

        CRect rcEdit;
        rcEdit.SetRectEmpty();
        pEditString->GetWindowRect(&rcEdit);
        ScreenToClient(&rcEdit);

        //CDC* pEditDC = pEditString->GetDC();
        Gdiplus::Font font(TEXT("Arial"), 10.0f);
        GDIPLUS_VERIFY(font.GetLastStatus());

        PointF ptStart((REAL)rcEdit.left, (REAL)(rcEdit.bottom + 2));
        RectF rcBounds;

        SolidBrush brGreen(Color::Green);

        StringFormat format;//(StringFormatFlags::StringFormatFlagsMeasureTrailingSpaces); // = StringFormat::GenericDefault()->Clone();
        //pSF->SetFormatFlags(pSF->GetFormatFlags() | StringFormatFlags::StringFormatFlagsMeasureTrailingSpaces);

        
        Pen penRed(Color::Green, 3.0f);
        GDIPLUS_VERIFY(penRed.GetLastStatus());

        GDIPLUS_VERIFY(pGraphics->MeasureString(m_strMeasureString, -1, &font, ptStart, &format, &rcBounds));
        //GDIPLUS_VERIFY(pGraphics->DrawString(m_strMeasureString, -1, &font, ptStart, &format, &brGreen));

        Rect rcIntBounds((INT)rcBounds.X, (INT)rcBounds.Y, (INT)rcBounds.Width, (INT)rcBounds.Height);
        GDIPLUS_VERIFY(pGraphics->DrawLine(&penRed, rcBounds.X, rcBounds.Y, rcBounds.GetRight(), rcBounds.GetBottom()));
        GDIPLUS_VERIFY(pGraphics->DrawRectangle(&penRed, rcIntBounds));
        //GDIPLUS_VERIFY(pGraphics->FillRectangle(&brGreen, rcBounds));
        //pEditString->ReleaseDC(pEditDC);
        
        //delete pSF;
    }
#endif 

}

void CGdiPlusPage::_TestGdiPlusString(TestGdiPlusStringType testType)
{
    INT nStringFormatFlags = GetSelectedStringFormatFlags();
    StringAlignment alignment = (StringAlignment)m_cmbAlignment.GetItemData(m_cmbAlignment.GetCurSel());
    StringAlignment LineAlignment = (StringAlignment)m_cmbLineAlignment.GetItemData(m_cmbLineAlignment.GetCurSel());
    m_cmbFontFamily.GetLBText(m_cmbFontFamily.GetCurSel(), m_strFontFamily);
    
    m_fontStyle = (FontStyle)m_cmbFontStyle.GetItemData(m_cmbFontStyle.GetCurSel());
    m_TextRenderingHint = (TextRenderingHint)m_cmbTextRenderingHint.GetItemData(m_cmbTextRenderingHint.GetCurSel());
    m_pixelOffsetMode = (PixelOffsetMode)m_cmbPixelOffset.GetItemData(m_cmbPixelOffset.GetCurSel());

    CDC* pDrawDC = m_staticPaint.GetWindowDC();
    {
        Status sts = Ok;
        Graphics graphic(pDrawDC->m_hDC);

        Gdiplus::Font font(m_strFontFamily, (REAL)m_nFontHeight, m_fontStyle);
        PointF ptStart(0, 0);

        StringFormat format;
        format.SetAlignment(alignment);
        format.SetLineAlignment(LineAlignment);
        format.SetFormatFlags(nStringFormatFlags);

        SolidBrush brush((ARGB)Color::Red);

        CRect rcPaint;
        m_staticPaint.GetClientRect(rcPaint);
        pDrawDC->FillSolidRect(rcPaint, RGB(0, 0, 255));

        //向内部缩2个像素，可以看出边框
        rcPaint.DeflateRect(2, 2);

        graphic.SetTextRenderingHint(m_TextRenderingHint);
        graphic.SetPixelOffsetMode(m_pixelOffsetMode);

        RectF rcPaintGdiP(rcPaint.left, rcPaint.top, rcPaint.right, rcPaint.bottom);

        SolidBrush nullBrush(Color::Green);
        graphic.FillRectangle(&nullBrush, rcPaintGdiP);

        GDIPLUS_VERIFY(graphic.SetRenderingOrigin(m_nRenderingOriginX, m_nRenderingOriginY));
        //graphic.SetTextContrast()
        RectF rcBounds;
        GDIPLUS_VERIFY(graphic.MeasureString(m_strPaintString, m_strPaintString.GetLength(), &font, rcPaintGdiP, &format, &rcBounds));

        //pDrawDC->Rectangle(rcBounds.X, rcBounds.Y, rcBounds.GetRight(), rcBounds.GetBottom());

        Gdiplus::Pen penBounds(Color::Aqua);
        graphic.DrawRectangle(&penBounds, rcBounds);

        FTLTRACE(TEXT("DrawString, StringFormatFlags=0x%08x, alignment=%d, lineAlignment=%d, Font={%d, %s}, MeasureBounds={%f,%f, %f,%f}\n"), 
            nStringFormatFlags, alignment, LineAlignment, m_nFontHeight, m_strFontFamily, 
            rcBounds.X, rcBounds.Y, rcBounds.GetRight(), rcBounds.GetBottom());

        RectF r(rcBounds);

        switch (testType)
        {
        case gpstDrawString:
            GDIPLUS_VERIFY(graphic.DrawString(m_strPaintString, m_strPaintString.GetLength(), &font, rcPaintGdiP, &format,  &brush));
            break;
        case gpstDrawDriverString:
            {
                PointF positions[] = 
                {
                    PointF(rcBounds.GetLeft(), rcBounds.GetTop()),
                    PointF(rcBounds.GetRight(), rcBounds.GetBottom()),
                };
                //GDIPLUS_VERIFY(graphic.DrawDriverString(m_strPaintString, m_strPaintString.GetLength(), 
                //    &font, &brush, positions, ))
            }
            break;
        }
    }

    m_staticPaint.ReleaseDC(pDrawDC);
}

void CGdiPlusPage::OnBnClickedBtnDrawString()
{
    UpdateData(TRUE);
    _TestGdiPlusString(gpstDrawString);
}

INT CGdiPlusPage::GetSelectedStringFormatFlags()
{
    INT nFlags = 0;
    int nCount = m_lstStringFormatFlags.GetItemCount();
    for (int nIndex = 0; nIndex < nCount; nIndex++)
    {
        if (m_lstStringFormatFlags.GetCheck(nIndex))
        {
            nFlags |= m_lstStringFormatFlags.GetItemData(nIndex);
        }
    }
    return nFlags;
}


void CGdiPlusPage::OnDestroy()
{
    _UnInitControls();

    CPropertyPage::OnDestroy();
}

void CGdiPlusPage::OnBnClickedBtnDrawDriverString()
{
    _TestGdiPlusString(gpstDrawDriverString);
}
