#ifndef FTL_CONTROLS_HPP
#define FTL_CONTROLS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlControls.h"
#endif

namespace FTL
{
	LPCTSTR CFControlUtil::GetEditNotifyCodeString(DWORD iNotify)
	{
		switch(iNotify)
		{
			HANDLE_CASE_RETURN_STRING(EN_SETFOCUS);
			HANDLE_CASE_RETURN_STRING(EN_KILLFOCUS);
			HANDLE_CASE_RETURN_STRING(EN_CHANGE);
			HANDLE_CASE_RETURN_STRING(EN_UPDATE);
			HANDLE_CASE_RETURN_STRING(EN_ERRSPACE);
			HANDLE_CASE_RETURN_STRING(EN_MAXTEXT);
			HANDLE_CASE_RETURN_STRING(EN_HSCROLL);
			HANDLE_CASE_RETURN_STRING(EN_VSCROLL);
			HANDLE_CASE_RETURN_STRING(EN_MSGFILTER);	//EN_ALIGN_LTR_EC
			HANDLE_CASE_RETURN_STRING(EN_REQUESTRESIZE);	//EN_ALIGN_RTL_EC
			HANDLE_CASE_RETURN_STRING(EN_SELCHANGE);
			HANDLE_CASE_RETURN_STRING(EN_DROPFILES);
			HANDLE_CASE_RETURN_STRING(EN_PROTECTED);
			HANDLE_CASE_RETURN_STRING(EN_CORRECTTEXT);
			HANDLE_CASE_RETURN_STRING(EN_STOPNOUNDO);
			HANDLE_CASE_RETURN_STRING(EN_IMECHANGE);
			HANDLE_CASE_RETURN_STRING(EN_SAVECLIPBOARD);
			HANDLE_CASE_RETURN_STRING(EN_OLEOPFAILED);
			HANDLE_CASE_RETURN_STRING(EN_OBJECTPOSITIONS);
			HANDLE_CASE_RETURN_STRING(EN_LINK);
			HANDLE_CASE_RETURN_STRING(EN_DRAGDROPDONE);
			HANDLE_CASE_RETURN_STRING(EN_PARAGRAPHEXPANDED);
			HANDLE_CASE_RETURN_STRING(EN_PAGECHANGE);
			HANDLE_CASE_RETURN_STRING(EN_LOWFIRTF);
			HANDLE_CASE_RETURN_STRING(EN_ALIGNLTR);
			HANDLE_CASE_RETURN_STRING(EN_ALIGNRTL);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Edit Notify Code, %d\n"), iNotify);
			return TEXT("Unknown");
		}
	}

	LPCTSTR CFControlUtil::GetRichEditStoryTypeString(long nStoryType)
	{
		switch(nStoryType)
		{
			HANDLE_CASE_RETURN_STRING(tomUnknownStory);
			HANDLE_CASE_RETURN_STRING(tomMainTextStory);
			HANDLE_CASE_RETURN_STRING(tomFootnotesStory);
			HANDLE_CASE_RETURN_STRING(tomEndnotesStory);
			HANDLE_CASE_RETURN_STRING(tomCommentsStory);
			HANDLE_CASE_RETURN_STRING(tomTextFrameStory);
			HANDLE_CASE_RETURN_STRING(tomEvenPagesHeaderStory);
			HANDLE_CASE_RETURN_STRING(tomPrimaryHeaderStory);
			HANDLE_CASE_RETURN_STRING(tomEvenPagesFooterStory);
			HANDLE_CASE_RETURN_STRING(tomPrimaryFooterStory);
			HANDLE_CASE_RETURN_STRING(tomFirstPageHeaderStory);
			HANDLE_CASE_RETURN_STRING(tomFirstPageFooterStory);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: Unknown Rich Edit Story Type for %d\n"),
				__FILE__LINE__, nStoryType);
			break;
		}
		return TEXT("Unknown");
	}

	LPCTSTR CFControlUtil::GetRichEditTomBoolString(long nValue)
	{
		switch (nValue)
		{
			HANDLE_CASE_RETURN_STRING(tomTrue);
			HANDLE_CASE_RETURN_STRING(tomFalse);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: Unknown Rich Edit Tom bool for %d\n"),
				__FILE__LINE__, nValue);
			break;
		}
		return TEXT("Unknown");

	}

	LPCTSTR CFControlUtil::GetRichEditAnimationTypeString(long nAnimation)
	{
		switch(nAnimation)
		{
			HANDLE_CASE_RETURN_STRING(tomNoAnimation);
			HANDLE_CASE_RETURN_STRING(tomLasVegasLights);
			HANDLE_CASE_RETURN_STRING(tomBlinkingBackground);
			HANDLE_CASE_RETURN_STRING(tomSparkleText);
			HANDLE_CASE_RETURN_STRING(tomMarchingBlackAnts);
			HANDLE_CASE_RETURN_STRING(tomMarchingRedAnts);
			HANDLE_CASE_RETURN_STRING(tomShimmer);
			HANDLE_CASE_RETURN_STRING(tomWipeDown);
			HANDLE_CASE_RETURN_STRING(tomWipeRight);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: Unknown Rich Edit Animation Type for %d\n"),
				__FILE__LINE__, nAnimation);
			break;
		}
		return TEXT("Unknown");
	}

	LPCTSTR CFControlUtil::GetRichEditUnderLineStyleString(long nUnderLine)
	{
		switch(nUnderLine)
		{
			HANDLE_CASE_RETURN_STRING(tomNone);
			HANDLE_CASE_RETURN_STRING(tomSingle);
			HANDLE_CASE_RETURN_STRING(tomWords);
			HANDLE_CASE_RETURN_STRING(tomDouble);
			HANDLE_CASE_RETURN_STRING(tomDotted);
			HANDLE_CASE_RETURN_STRING(tomDash);
			HANDLE_CASE_RETURN_STRING(tomDashDot);
			HANDLE_CASE_RETURN_STRING(tomDashDotDot);
			HANDLE_CASE_RETURN_STRING(tomWave);
			HANDLE_CASE_RETURN_STRING(tomThick);
			HANDLE_CASE_RETURN_STRING(tomHair);
			HANDLE_CASE_RETURN_STRING(tomUndefined);
		default:
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: Unknown Rich Edit UnderLine Type for %d\n"),
				__FILE__LINE__, nUnderLine);
			break;
		}
		return TEXT("Unknown");
	}

#ifdef _WIN64
#   pragma TODO(Win64)
#	define TEXT_FONT_COLOR_TYPE_MASK		0x8000000000000000
#else
#	define TEXT_FONT_COLOR_TYPE_MASK		0x80000000
#endif

	LPCTSTR CFControlUtil::GetRichEditColorString(FTL::CFStringFormater& formater, long nColor)
	{
		if (tomAutoColor == nColor)
		{
			formater.Format(TEXT("tomAutoColor"));
		}
		else if((nColor & TEXT_FONT_COLOR_TYPE_MASK) == 0)
		{
			//The high-order byte is zero, and the three low-order bytes specify an RGB color
			COLORREF color = nColor & ~TEXT_FONT_COLOR_TYPE_MASK;
			BYTE bRed = GetRValue(color);
			BYTE bGreen = GetGValue(color);
			BYTE bBlue = GetBValue(color);
			formater.Format(TEXT("RGB: { Red=%d,Green=%d, Blue=%d }"), bRed, bGreen, bBlue);
		}
		else if((nColor & TEXT_FONT_COLOR_TYPE_MASK) == TEXT_FONT_COLOR_TYPE_MASK)
		{
			//The high-order byte is 1, and the low-order word specifies the index of a logical-color palette entry.
			formater.Format(TEXT("Palette Index: %d"), nColor);
		}
		else
		{
			FTLASSERT(FALSE);
		}
		return formater.GetString();
	}

	LPCTSTR CFControlUtil::GetCharFormatEffectAndMaskString(CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide)
	{
		DWORD dwOldMasks = dwMasks;
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_BOLD, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_ITALIC, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_UNDERLINE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_STRIKEOUT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_PROTECTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_LINK, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_SMALLCAPS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_ALLCAPS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_HIDDEN, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_OUTLINE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_SHADOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_EMBOSS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_IMPRINT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_DISABLED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_REVISED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_REVAUTHOR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_SUBSCRIPT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_SUPERSCRIPT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_ANIMATION, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_STYLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_KERNING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_SPACING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_WEIGHT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_UNDERLINETYPE, pszDivide);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, XXXXXXX, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_LCID, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_BACKCOLOR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_CHARSET, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_OFFSET, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_FACE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFE_AUTOCOLOR, pszDivide);	//0x40000000(CFM_COLOR)
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_SIZE, pszDivide);			//0x80000000

		FTLASSERT( 0 == dwMasks);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
		if (0 != dwMasks)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetCharFormatMasksString Not Complete, total=0x%x, remain=0x%x\n"),
				__FILE__LINE__, dwOldMasks, dwMasks);
		}
		return formater.GetString();
	}

	LPCTSTR CFControlUtil::GetParaFormatEffectAndMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide)
	{
		DWORD dwOldMasks = dwMasks;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_STARTINDENT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_RIGHTINDENT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_OFFSET, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_ALIGNMENT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_TABSTOPS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_NUMBERING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_SPACEBEFORE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_SPACEAFTER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_LINESPACING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_STYLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_BORDER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_SHADING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_NUMBERINGSTYLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_NUMBERINGTAB, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_NUMBERINGSTART, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_RTLPARA, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_KEEP, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_KEEPNEXT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_PAGEBREAKBEFORE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_NOLINENUMBER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_NOWIDOWCONTROL, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_DONOTHYPHEN, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_SIDEBYSIDE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_COLLAPSED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_OUTLINELEVEL, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_BOX, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_RESERVED2, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_TABLEROWDELIMITER, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_TEXTWRAPPINGBREAK, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_TABLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, PFM_OFFSETINDENT, pszDivide);	//0x80000000

		FTLASSERT( 0 == dwMasks);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
		if (0 != dwMasks)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetParaFormatMaskString Not Complete, total=0x%x, remain=0x%x\n"),
				__FILE__LINE__, dwOldMasks, dwMasks);
		}

		return formater.GetString();
	}

	LPCTSTR CFControlUtil::GetRichEditPropertyBits(FTL::CFStringFormater& formater, DWORD dwBits, LPCTSTR pszDivide /* = TEXT */)
	{
		DWORD dwOldBits = dwBits;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_RICHTEXT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_MULTILINE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_READONLY, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_SHOWACCELERATOR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_USEPASSWORD, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_HIDESELECTION, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_SAVESELECTION, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_AUTOWORDSEL, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_VERTICAL, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_SELBARCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_WORDWRAP, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_ALLOWBEEP, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_DISABLEDRAG, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_VIEWINSETCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_BACKSTYLECHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_MAXLENGTHCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_SCROLLBARCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_CHARFORMATCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_PARAFORMATCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_EXTENTCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_CLIENTRECTCHANGE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwBits, TXTBIT_USECURRENTBKG, pszDivide);

		FTLASSERT( 0 == dwBits);
		if (0 != dwBits)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetRichEditPropertyBits Not Complete, total=0x%x, remain=0x%x\n"),
				__FILE__LINE__, dwOldBits, dwBits);
		}

		return formater.GetString();
	}


	//////////////////////////////////////////////////////////////////////////
	HRESULT CFTextRangeDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("TextRange")));
		if (m_pObj)
		{
			CComQIPtr<ITextRange>     spTextRange(m_pObj);
			if (spTextRange)
			{
				long nStart = 0;
				COM_VERIFY(spTextRange->GetStart(&nStart));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Start"), nStart));

				long nEnd = 0;
				COM_VERIFY(spTextRange->GetEnd(&nEnd));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("End"), nEnd));

				CComBSTR bstrText;
				COM_VERIFY(spTextRange->GetText(&bstrText));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Text"), &bstrText));

				long nStoryType = 0;
				COM_VERIFY(spTextRange->GetStoryType(&nStoryType));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("StoryType"), CFControlUtil::GetRichEditStoryTypeString(nStoryType)));

				long nStoryLength = 0;
				COM_VERIFY(spTextRange->GetStoryLength(&nStoryLength));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("StoryLength"), nStoryLength));

				long bCanEdit = 0;
				COM_VERIFY(spTextRange->CanEdit(&bCanEdit));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("CanEdit"), bCanEdit));

				CComPtr<IUnknown>	spEmbeddedObject;
				hr = spTextRange->GetEmbeddedObject(&spEmbeddedObject);
				if (SUCCEEDED(hr) && spEmbeddedObject)
				{
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("EmbeddedObject")));
					COM_DETECT_INTERFACE_FROM_REGISTER(spEmbeddedObject);
				}
			}
		}
		return hr;
	}

	HRESULT CFTextFontDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("TextFont")));
		if (m_pObj)
		{
			CComQIPtr<ITextFont>     spTextFont(m_pObj);
			if (spTextFont)
			{
				FTL::CFStringFormater formater;
				long bCanChange = 0;
				COM_VERIFY(spTextFont->CanChange(&bCanChange));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("CanChange"), CFControlUtil::GetRichEditTomBoolString(bCanChange)));

				CComBSTR bstrFontName;
				COM_VERIFY(spTextFont->GetName(&bstrFontName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("FontName"), &bstrFontName));

				float fSize = 0.0f;
				COM_VERIFY(spTextFont->GetSize(&fSize));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Size"), fSize));

				long nWeight = 0;
				COM_VERIFY(spTextFont->GetWeight(&nWeight));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Weight"), nWeight));

				long nForeColor = 0;
				COM_VERIFY(spTextFont->GetForeColor(&nForeColor));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("ForeColor"), CFControlUtil::GetRichEditColorString(formater, nForeColor)));

				long nBackColor = 0;
				COM_VERIFY(spTextFont->GetBackColor(&nBackColor));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("BackColor"), CFControlUtil::GetRichEditColorString(formater, nForeColor)));

				long bBold = 0;
				COM_VERIFY(spTextFont->GetBold(&bBold));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Bold"), CFControlUtil::GetRichEditTomBoolString(bBold)));

				long bItalic = 0;
				COM_VERIFY(spTextFont->GetItalic(&bItalic));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Italic"), CFControlUtil::GetRichEditTomBoolString(bItalic)));

				long nStyle = 0;
				COM_VERIFY(spTextFont->GetStyle(&nStyle));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Style"), nStyle));

				long nUnderLine = 0;
				COM_VERIFY(spTextFont->GetUnderline(&nUnderLine));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("UnderLine"), CFControlUtil::GetRichEditUnderLineStyleString(nUnderLine)));

				long nLanguageID = 0;
				COM_VERIFY(spTextFont->GetLanguageID(&nLanguageID));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("LanguageID"), nLanguageID));

				long nAllCaps = 0;
				COM_VERIFY(spTextFont->GetAllCaps(&nAllCaps));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("AllCaps"), CFControlUtil::GetRichEditTomBoolString(nAllCaps)));

				long nAnimation = 0;
				COM_VERIFY(spTextFont->GetAnimation(&nAnimation));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Animation"), CFControlUtil::GetRichEditAnimationTypeString(nAnimation)));

				long bEmboss = 0;
				COM_VERIFY(spTextFont->GetEmboss(&bEmboss));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Emboss"), CFControlUtil::GetRichEditTomBoolString(bEmboss)));

				long bEngrave = 0;
				COM_VERIFY(spTextFont->GetEngrave(&bEngrave));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Engrave"), CFControlUtil::GetRichEditTomBoolString(bEngrave)));

				long bHidden = 0;
				COM_VERIFY(spTextFont->GetEngrave(&bHidden));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Hidden"), CFControlUtil::GetRichEditTomBoolString(bHidden)));

				float fKerning = 0;
				COM_VERIFY(spTextFont->GetKerning(&fKerning));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kerning"), fKerning));

			}
		}
		return hr;
	}

	//////////////////////////////////////////////////////////////////////////
    LRESULT CFSkinComboBox::OnCbnDropDown(UINT uNotifyCode, int nID, CWindow wndCtl)
    {
        return 0;
    }

    LRESULT CFSkinComboBox::OnCbnCloseup(UINT uNotifyCode, int nID, CWindow wndCtl)
    {
        return 0;
    }
    HBRUSH CFSkinComboBox::OnCtlColorListBox(CDCHandle dc, CListBox listBox)
    {
        static HBRUSH hBrush = CreateSolidBrush(RGB(255,0,0));
        if (!m_hWndList)
        {
            SkinWndScroll(m_hWndList, m_pSkinInfo);
            m_hWndList = listBox.m_hWnd;
        }
        return hBrush;
    }
    BOOL CFSkinComboBox::SkinScroll(const FScrollSkinInfo* pSkinInfo)
    {
        m_pSkinInfo = pSkinInfo;
        //手动弹出下拉列表以便进行列表窗口的子类化
        ShowDropDown(TRUE);
        ShowDropDown(FALSE);
        return TRUE;
    }
    void CFSkinComboBox::UnSkinScroll()
    {
    }


    CFSkinScrollBar::CFSkinScrollBar()
    {

    }
    CFSkinScrollBar::~CFSkinScrollBar()
    {

    }
    int CFSkinScrollBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
        return 0;
    }


    CFSkinScrollWindow::CFSkinScrollWindow()
    {
        m_nScrollWidth = 16;
    }
    CFSkinScrollWindow::~CFSkinScrollWindow()
    {

    }
    BOOL CFSkinScrollWindow::SkinScroll(HWND hWnd, const FScrollSkinInfo* pSkinInfo)
    {
        BOOL bRet = FALSE;
        FTLASSERT(NULL == m_wndMiddle.m_hWnd);

        ::SetWindowLong(hWnd, GWL_USERDATA, (LONG) this);
        m_funOldProc=(WNDPROC)::SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)HookWndProc);

        //CWindow wndCtrl(hWndCtrl);
        //CWindow wndParent(::GetParent(hWndCtrl));
        //FTLASSERT(wndParent.IsWindow());

        //CRect rcFrame,rcWnd;
        //wndCtrl.GetWindowRect(&rcFrame);
        //wndParent->ScreenToClient(&rcFrame);
        //rcWnd=rcFrame;
        //
        //UINT uID = ::GetDlgCtrlID(hWndCtrl);
        return bRet;
    }
    void CFSkinScrollWindow::UnSkinScroll()
    {

    }

    LRESULT CALLBACK CFSkinScrollWindow::HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
    {
        CFSkinScrollWindow *pSkinFrame = (CFSkinScrollWindow*)::GetWindowLong(hwnd,GWL_USERDATA);
        if (pSkinFrame)
        {
            if (WM_NCCALCSIZE == msg)
            {
                LPNCCALCSIZE_PARAMS pNcCalcSizeParam=(LPNCCALCSIZE_PARAMS)lp;
                int nWid=::GetSystemMetrics(SM_CXVSCROLL);
                pNcCalcSizeParam->rgrc[0].right += nWid - pSkinFrame->m_nScrollWidth;
            }
        }
        LRESULT ret =::CallWindowProc(pSkinFrame->m_funOldProc,hwnd,msg,wp,lp);
        return ret;
    }

    extern "C" CFSkinScrollWindow* SkinWndScroll(HWND hWnd, const FScrollSkinInfo* pScrollSkinInfo)
    {
        CFSkinScrollWindow *pFrame=new CFSkinScrollWindow();
        pFrame->SkinScroll(hWnd, pScrollSkinInfo);
        return pFrame;

    }
    extern "C" BOOL UnSkinWndScroll(HWND  hWnd)
    {
        CFSkinScrollWindow *pFrame=(CFSkinScrollWindow *)GetWindowLong(hWnd,GWL_USERDATA);
        if(pFrame) 
        {
            //RECT rc;
            //CWnd *pParent=pFrame->GetParent();
            //pFrame->GetWindowRect(&rc);
            //pParent->ScreenToClient(&rc);
            //SetWindowLong(pWnd->m_hWnd,GWL_WNDPROC,(LONG)pFrame->m_funOldProc);
            //SetWindowLong(pWnd->m_hWnd,GWL_USERDATA,0);
            //pWnd->SetParent(pParent);
            //pWnd->MoveWindow(&rc);
            pFrame->DestroyWindow();
            delete pFrame;
        }
        return TRUE;

    }
}

#endif //FTL_CONTROLS_HPP
