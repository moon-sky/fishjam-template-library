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

	LPCTSTR CFControlUtil::GetCharFormatMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide)
	{
		DWORD dwOldMasks = dwMasks;

		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_BOLD, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_ITALIC, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_UNDERLINE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_STRIKEOUT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_PROTECTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_LINK, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_SIZE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_COLOR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_FACE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_OFFSET, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwMasks, CFM_CHARSET, pszDivide);

		FTLASSERT( 0 == dwMasks);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
		if (0 != dwMasks)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetCharFormatMaskString Not Complete, total=0x%x, remain=0x%x\n"),
				__FILE__LINE__, dwOldMasks, dwMasks);
		}

		return formater.GetString();
	}

	LPCTSTR CFControlUtil::GetCharFormatEffectString(CFStringFormater& formater, DWORD dwEffects, LPCTSTR pszDivide)
	{
		DWORD dwOldEffects = dwEffects;
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_BOLD, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_ITALIC, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_UNDERLINE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_STRIKEOUT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_PROTECTED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_LINK, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_SMALLCAPS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_ALLCAPS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_HIDDEN, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_OUTLINE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_SHADOW, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_EMBOSS, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_IMPRINT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_DISABLED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_REVISED, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_REVAUTHOR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_SUBSCRIPT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_SUPERSCRIPT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_ANIMATION, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_STYLE, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_KERNING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_SPACING, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_WEIGHT, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_UNDERLINETYPE, pszDivide);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, XXXXXXX, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_LCID, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFM_BACKCOLOR, pszDivide);
		HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwEffects, CFE_AUTOCOLOR, pszDivide);	//0x40000000

		FTLASSERT( 0 == dwEffects);
		//HANDLE_COMBINATION_VALUE_TO_STRING(formater, lStyle, XXXXXXXXX, pszDivide);
		if (0 != dwEffects)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("%s: GetCharFormatEffectString Not Complete, total=0x%x, remain=0x%x\n"),
				__FILE__LINE__, dwOldEffects, dwEffects);
		}
		return formater.GetString();
	}

	LPCTSTR CFControlUtil::GetParaFormatMaskString(FTL::CFStringFormater& formater, DWORD dwMasks, LPCTSTR pszDivide)
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
