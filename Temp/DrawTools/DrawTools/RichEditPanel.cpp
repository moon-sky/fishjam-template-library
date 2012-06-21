#include "stdafx.h"
#include "RichEditPanel.h"

#ifdef FTL_DEBUG
//#include <ftlwindow.h>
//#include <ftlControls.h>
#endif 
#include <ftlGdi.h> 

#pragma comment(lib, "riched20.lib")

//#include <ftlConfigDetect.h>

#define LY_PER_INCH   1440
// HIMETRIC units per inch (used for conversion)
#define HIMETRIC_PER_INCH	2540

// HIMETRIC units per inch (used for conversion)
#define HIMETRIC_PER_INCH 2540

// Convert Himetric along the X axis to X pixels
LONG HimetricXtoDX(LONG xHimetric, LONG xPerInch)
{
	return (LONG) MulDiv(xHimetric, xPerInch, HIMETRIC_PER_INCH);
}

// Convert Himetric along the Y axis to Y pixels
LONG HimetricYtoDY(LONG yHimetric, LONG yPerInch)
{
	return (LONG) MulDiv(yHimetric, yPerInch, HIMETRIC_PER_INCH);
}

// Convert Pixels on the X axis to Himetric
LONG DXtoHimetricX(LONG dx, LONG xPerInch)
{
	return (LONG) MulDiv(dx, HIMETRIC_PER_INCH, xPerInch);
}

// Convert Pixels on the Y axis to Himetric
LONG DYtoHimetricY(LONG dy, LONG yPerInch)
{
	return (LONG) MulDiv(dy, HIMETRIC_PER_INCH, yPerInch);
}

// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0
const LONG cInitTextMax = (32 * 1024) - 1;
const LONG cResetTextMax = (64 * 1024);

// The IID in riched20.lib in the Plaform SDK appears to be incorrect.
// This one is from the Microsoft Knowledge Base, article Q270161.
const IID IID_ITextServices = 
{	
	// 8d33f740-cf58-11ce-a89d-00aa006cadc5
	0x8d33f740, 0xcf58, 0x11ce,	{0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

//EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
//	0xc5bdd8d0,
//	0xd26e,
//	0x11ce,
//	{0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
//};
//


CRichEditPanel::CRichEditPanel()
{
	FTLTRACEEX(FTL::tlTrace, TEXT("CRichEditPanel::CRichEditPanel, new for 0x%x\n"), this);

	ZeroMemory(&m_cRefs, sizeof(CRichEditPanel) - offsetof(CRichEditPanel, m_cRefs));

	m_cRefs = 1;
	m_dwStyle = ES_MULTILINE | ES_NOHIDESEL ;
	m_fRich = TRUE;
	m_cchTextMost = cInitTextMax;
	m_chPasswordChar = TEXT('*');
	m_fCapture = FALSE;
	m_fAutoVExpand = TRUE;
	m_fTransparent = TRUE;
}

CRichEditPanel::~CRichEditPanel()
{
	m_spTextDocument.Release();
	m_spTextServices.Release();
	
	FTLTRACEEX(FTL::tlTrace, TEXT("CRichEditPanel::~CRichEditPanel, destrcutor for 0x%x\n"), this);
}

HRESULT STDMETHODCALLTYPE CRichEditPanel::QueryInterface(REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObject = NULL;
	
	//{13E670F5-1A5A-11CF-ABEB-00AA00B65EA1}  -- ?
	if (IsEqualIID(riid, IID_IUnknown) 
		|| IsEqualIID(riid, IID_ITextHost))
	{
		AddRef();
		*ppvObject = (ITextHost *) this;
		hr = S_OK;
	}
	return hr;
}

ULONG CRichEditPanel::AddRef(void)
{
	FTLTRACEEX(FTL::tlDetail, TEXT("Before CRichEditPanel::AddRef, m_cRefs=%d\n"), m_cRefs);
	return ++m_cRefs;
}

ULONG STDMETHODCALLTYPE CRichEditPanel::Release(void)
{
	ULONG c_Refs = --m_cRefs;
	FTLTRACEEX(FTL::tlDetail, TEXT("After CRichEditPanel::Release, m_cRefs=%d\n"), m_cRefs);

	if (c_Refs == 0)
	{
		delete this;
	}
	return c_Refs;
}

HRESULT CRichEditPanel::InitDefaultCharFormat(const LOGFONT* pLogFont, COLORREF clrTextFore) 
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::InitDefaultCharFormat, hFont=0x%x\n"), pLogFont);

#if 1 
	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;
	HWND hWnd = NULL;
	HDC hDC = NULL;
	LOGFONT logFont = {0};
	
	// Get LOGFONT for default font
	if (!pLogFont)
	{
		HFONT hDefaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		API_VERIFY(0 != GetObject(hDefaultFont, sizeof(LOGFONT), &logFont));
		if (!bRet)
		{
			return E_FAIL;
		}
	}
	else
	{
		logFont = *pLogFont;
	}

	//Set m_charFormat structure
	ZeroMemory(&m_charFormat, sizeof(m_charFormat));
	m_charFormat.cbSize = sizeof(m_charFormat);

	hWnd = GetDesktopWindow();
	hDC = GetDC(hWnd);
	m_xPixPerInch = GetDeviceCaps(hDC, LOGPIXELSX);
	m_yPixPerInch = GetDeviceCaps(hDC, LOGPIXELSY);

	//int iPointSize = -1 * ::MulDiv(logFont.lfHeight, 72, m_yPixPerInch);
	int iPointSize = -1 * ::MulDiv(logFont.lfHeight, m_yPixPerInch, 72);
	m_charFormat.yHeight = iPointSize * 20;
	//m_charFormat.yHeight = -MulDiv(pLogFont->lfHeight, 72, m_yPixPerInch);//  * LY_PER_INCH / m_yPixPerInch;
	ReleaseDC(hWnd, hDC);

	m_charFormat.yOffset = 0;
	m_charFormat.crTextColor = clrTextFore;

	m_charFormat.dwEffects = CFM_BOLD | CFM_COLOR; // CFM_EFFECTS | CFE_AUTOBACKCOLOR;
	//m_charFormat.dwEffects &= ~(CFE_PROTECTED | CFE_LINK);

	//if(lf.lfWeight < FW_BOLD)
	//	m_charFormat.dwEffects &= ~CFE_BOLD;
	//if(!lf.lfItalic)
	//	m_charFormat.dwEffects &= ~CFE_ITALIC;
	//if(!lf.lfUnderline)
	//	m_charFormat.dwEffects &= ~CFE_UNDERLINE;
	//if(!lf.lfStrikeOut)
	//	m_charFormat.dwEffects &= ~CFE_STRIKEOUT;

	m_charFormat.dwMask = CFM_ALL | CFM_COLOR | CFM_BACKCOLOR;
	m_charFormat.bCharSet = logFont.lfCharSet;
	m_charFormat.bPitchAndFamily = logFont.lfPitchAndFamily;
	COM_VERIFY(StringCchCopy(m_charFormat.szFaceName, _countof(m_charFormat.szFaceName),  pLogFont->lfFaceName));
#else
	// Get the current font settings
	NONCLIENTMETRICS nonClientMetrics = {0};
	nonClientMetrics.cbSize = sizeof (NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS), &nonClientMetrics, 0);

	// Work out the name and point size of the font
	CString strFontName(nonClientMetrics.lfMessageFont.lfFaceName);
	HDC hDC = ::GetDC(NULL);
	int iPointSize = -1 * ::MulDiv(nonClientMetrics.lfMessageFont.lfHeight,72,::GetDeviceCaps(hDC,LOGPIXELSY));
	::ReleaseDC(NULL,hDC);

	// Create a default character format
	::ZeroMemory(&m_charFormat,sizeof(CHARFORMAT));
	m_charFormat.cbSize = sizeof(CHARFORMAT);

	//TODO:CFM_ALL
	m_charFormat.dwMask =  CFM_BOLD|CFM_CHARSET|CFM_COLOR|CFM_FACE|CFM_ITALIC|CFM_OFFSET|
		CFM_PROTECTED|CFM_SIZE|CFM_STRIKEOUT|CFM_UNDERLINE;
	//m_charFormat.dwEffects = CFM_EFFECTS | CFE_AUTOBACKCOLOR | CFE_BOLD| CFE_ITALIC| CFE_UNDERLINE| CFE_STRIKEOUT;
	//m_charFormat.dwEffects &= ~(CFE_PROTECTED | CFE_LINK);

	m_charFormat.yHeight = 20 * iPointSize;
	m_charFormat.crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	m_charFormat.bPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	_tcscpy(m_charFormat.szFaceName,strFontName);
#endif 

	return S_OK;
}

HRESULT CRichEditPanel::InitDefaultParaFormat() 
{	
#if 0
	memset(ppf, 0, sizeof(PARAFORMAT));
	ppf->cbSize = sizeof(PARAFORMAT);
	ppf->dwMask = PFM_ALL;
	ppf->wAlignment = PFA_LEFT;
	ppf->cTabCount = 1;
	ppf->rgxTabs[0] = lDefaultTab;
#else
	::ZeroMemory(&m_paraFormat ,sizeof(PARAFORMAT));
	m_paraFormat.cbSize = sizeof(PARAFORMAT);
	m_paraFormat.dwMask = PFM_ALIGNMENT|PFM_NUMBERING|PFM_OFFSET|PFM_OFFSETINDENT|PFM_RIGHTINDENT|PFM_RTLPARA|PFM_STARTINDENT|PFM_TABSTOPS;
	m_paraFormat.wAlignment = PFA_LEFT;
#endif 
	return S_OK;
}

HRESULT CRichEditPanel::Init(HWND hWndOwner, const RECT* prcBound,
							 IDrawCanvas* pDrawCanvas,
							 const LOGFONT* pLogFont,
							 COLORREF clrFontFore,
							 INotifyCallBack* pNotifyCallback /* = NULL */)
{
	HRESULT hr = E_FAIL;
	LRESULT lResult = 0;

	FTLTRACEEX(FTL::tlTrace, TEXT("CRichEditPanel::Init, hWndOwner=0x%x, prcBound=[%d,%d]-[%d,%d]\n"),
		hWndOwner, prcBound->left, prcBound->top, prcBound->right, prcBound->bottom);

	m_hWndOwner = hWndOwner;
	//m_rcBound = *prcBound;
	//m_rcBound.NormalizeRect();
	//m_rcClient = m_rcClient;
	m_pDrawCanvas = pDrawCanvas;


	//m_pDrawCanvas->DocToClient(&m_rcClient);

	//m_rcClient.DeflateRect(MARGIN_LEFT, MARGIN_TOP, MARGIN_RIGHT, MARGIN_BOTTOM);
	//CSize szMin = GetMinBoundSize(2, 5);
	//if (m_rcClient.top - szMin.cy > m_rcClient.bottom)
	//{
	//	m_rcClient.bottom = m_rcClient.top + MARGIN_TOP + 1 + MARGIN_BOTTOM;
	//}
	//if (m_rcClient.left > m_rcClient.right)
	//{
	//	m_rcClient.right = m_rcClient.left + MARGIN_LEFT + 1 + MARGIN_RIGHT;
	//}

	m_pNotifyCallback = pNotifyCallback;

	//m_rcViewInset = m_rcClient;

	COM_VERIFY(InitDefaultCharFormat(pLogFont, clrFontFore));
	COM_VERIFY(InitDefaultParaFormat());

//#pragma TODO(szlExtent is Himetric)
	// The extent matches the full client rectangle in HIMETRIC
	//m_szlExtent.cx = DXtoHimetricX(m_rcClient.Width(), m_xPixPerInch);
	//m_szlExtent.cy = DYtoHimetricY(m_rcClient.Height(), m_yPixPerInch);

	CComPtr<IUnknown>	spUnknown;
	COM_VERIFY(CreateTextServices(NULL, this, &spUnknown));
	if (SUCCEEDED(hr) && spUnknown)
	{
		COM_VERIFY(spUnknown->QueryInterface(IID_ITextServices, (void**)&m_spTextServices));
		COM_VERIFY(spUnknown->QueryInterface(__uuidof(ITextDocument), (void**)&m_spTextDocument));
		COM_VERIFY(m_spTextServices->OnTxPropertyBitsChange(TXTBIT_BACKSTYLECHANGE, TXTBIT_BACKSTYLECHANGE));

		LRESULT eventMask = 0;
		//LRESULT oldEventMask = 0;
		//COM_VERIFY(m_spTextServices->TxSendMessage(EM_GETEVENTMASK, 0, 0, &eventMask));
		//eventMask |= ENM_CHANGE | ENM_UPDATE | ENM_SCROLL | ENM_SCROLLEVENTS | ENM_PARAGRAPHEXPANDED| ENM_PAGECHANGE| ENM_KEYEVENTS | ENM_MOUSEEVENTS
		//	| ENM_REQUESTRESIZE| ENM_SELCHANGE| ENM_DROPFILES| ENM_PROTECTED| ENM_CORRECTTEXT| ENM_IMECHANGE
		//	| ENM_LANGCHANGE | ENM_OBJECTPOSITIONS | ENM_LINK | ENM_LOWFIRTF; 
		eventMask = ENM_SELCHANGE | ENM_REQUESTRESIZE;
		COM_VERIFY(m_spTextServices->TxSendMessage(EM_SETEVENTMASK, 0, (LPARAM)eventMask, NULL));

		//COM_VERIFY(m_spTextServices->TxSendMessage(EM_GETEVENTMASK, 0, 0, &oldEventMask));
		//int i = 0;
		//COM_VERIFY(m_spTextServices->TxSendMessage(EM_SETOLECALLBACK, NULL, (LPARAM)this, &lResult));


		//COM_DETECT_INTERFACE_FROM_REGISTER(m_spTextServices);
		//COM_DETECT_INTERFACE_FROM_LIST(m_spTextServices);

		//COM_VERIFY(m_spTextServices->TxSetText(TEXT("this is sample")));
	}
	if (SUCCEEDED(hr))
	{
		//FTLTRACEEX(FTL::tlDetail, TEXT("Before Call m_spTextServices->OnTxInPlaceActivate, rcClient=[%d,%d]-[%d,%d]\n"),
		//	m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
		//SetActive(TRUE);
		//m_spTextServices->OnTxInPlaceActivate(&m_rcClient);
		//FTLTRACEEX(FTL::tlDetail, TEXT("After Call m_spTextServices->OnTxInPlaceActivate\n"));
	}
	SetClientBound(prcBound, NULL, TRUE);
	return hr;
}

BOOL CRichEditPanel::SetActive(BOOL bActive)
{
	FTLTRACEEX(FTL::tlInfo, TEXT("CRichEditPanel::SetActive, bActive=%d, m_fInplaceActive=%d\n"), bActive, m_fInplaceActive);
	BOOL bRet = TRUE;
	if (bActive != m_fInplaceActive)
	{
		HRESULT hr = E_FAIL;
		if (bActive)
		{
			CRect rcClient = m_rcClient;
			m_pDrawCanvas->DocToClient(&rcClient);
			COM_VERIFY(m_spTextServices->OnTxInPlaceActivate(&rcClient));
			COM_VERIFY(m_spTextServices->OnTxUIActivate());
			COM_VERIFY(m_spTextServices->TxSendMessage(WM_SETFOCUS, NULL, 0, NULL));
		}
		else
		{
			//Clear Select
			CComPtr<ITextSelection>	spSelection;
			COM_VERIFY(m_spTextDocument->GetSelection(&spSelection));
			if (spSelection)
			{
				long nEnd = 0;
				COM_VERIFY(spSelection->GetEnd(&nEnd));
				COM_VERIFY_EXCEPT1(spSelection->SetStart(nEnd), S_FALSE);
			}

			COM_VERIFY(m_spTextServices->TxSendMessage(WM_KILLFOCUS, (WPARAM)NULL, 0, NULL));
			COM_VERIFY(m_spTextServices->OnTxUIDeactivate());
			COM_VERIFY(m_spTextServices->OnTxInPlaceDeactivate());
		}
		//if (SUCCEEDED(hr))
		{
			m_fInplaceActive = bActive;
		}
	}
	
	return bRet;
}

BOOL CRichEditPanel::Undo()
{
	BOOL bRet = FALSE;
	HRESULT hr = E_FAIL;
	long prop = 0;
	COM_VERIFY_EXCEPT1(m_spTextDocument->Undo(1, &prop), S_FALSE);
	FTLTRACEEX(FTL::tlInfo, TEXT("CRichEditPanel::Undo, prop=%d, result=0x%x\n"), prop, hr);
	if (S_OK == hr)
	{
		return TRUE;
	}
	if (S_FALSE == hr)
	{
		return FALSE;
	}
	return FALSE;
}

BOOL CRichEditPanel::Redo()
{
	BOOL bRet = FALSE;
	HRESULT hr = E_FAIL;
	long prop = 0;
	COM_VERIFY_EXCEPT1(m_spTextDocument->Redo(1, &prop), S_FALSE);
	FTLTRACEEX(FTL::tlInfo, TEXT("CRichEditPanel::Redo, prop=%d, result=0x%x\n"), prop, hr);
	if (S_OK == hr)
	{
		return TRUE;
	}
	if (S_FALSE == hr)
	{
		return FALSE;
	}
	return FALSE;
}


BOOL CRichEditPanel::IsActive()
{
	return m_fInplaceActive;
}

HRESULT CRichEditPanel::GetTextRange(long nStart, long nEnd, CComPtr<ITextRange>& spTextRange)
{
	HRESULT hr = E_FAIL;
	if (0 == nStart && 0 == nEnd)
	{
		//current select
		CComPtr<ITextSelection> spCurtSelection;
		COM_VERIFY(m_spTextDocument->GetSelection(&spCurtSelection));
		if (spCurtSelection)
		{
			spTextRange = spCurtSelection;
		}
	}
	else if (0 == nStart && -1 == nEnd)
	{
		nEnd = m_cchTextMost;// std::numeric_limits<long>::max();
		COM_VERIFY(m_spTextDocument->Range(nStart, nEnd, &spTextRange));
	}
	else
	{
		COM_VERIFY(m_spTextDocument->Range(nStart, nEnd, &spTextRange));
	}
	if (SUCCEEDED(hr))
	{
		//FTL::CFTextRangeDumper rangeDumper(spTextRange, FTL::CFOutputWindowInfoOutput::Instance(), 0);
	}
	return hr;
}

HRESULT CRichEditPanel::SetTextFont(long nStart, long nEnd, PLOGFONT pLogFont, DWORD dwFontMask)
{
	CHECK_POINTER_READABLE_DATA_RETURN_VALUE_IF_FAIL(pLogFont, sizeof(LOGFONT), FALSE);

	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));
		//FTL::CFTextFontDumper fontDumper(spFont, FTL::CFOutputWindowInfoOutput::Instance(), 0);

		if (dwFontMask & RICH_EDIT_PANEL_FONT_MASK_NAME)
		{
			COM_VERIFY(spFont->SetName(CComBSTR(pLogFont->lfFaceName)));
		}
		if (dwFontMask & RICH_EDIT_PANEL_FONT_MASK_SIZE)
		{
			LONG nSize = -MulDiv(pLogFont->lfHeight, m_yPixPerInch, 72);
			COM_VERIFY(spFont->SetSize(nSize));
		}
		//if (RICH_EDIT_PANEL_FONT_MASK_WEIGHT & dwFontMask)
		//{
		//	COM_VERIFY(spFont->SetWeight(pLogFont->lfWidth));
		//}
		if (RICH_EDIT_PANEL_FONT_MASK_BOLD & dwFontMask)
		{
			long isBold = (FW_BOLD == pLogFont->lfWeight ? tomTrue : tomFalse);
			COM_VERIFY(spFont->SetBold( isBold ));
		}
		if (RICH_EDIT_PANEL_FONT_MASK_ITALIC & dwFontMask)
		{
			long isItalic = (0 != pLogFont->lfItalic ? tomTrue : tomFalse);
			COM_VERIFY(spFont->SetItalic( isItalic ));
		}
		if (RICH_EDIT_PANEL_FONT_MASK_UNDERLINE & dwFontMask)
		{
			long isUnderLine = (0 != pLogFont->lfUnderline ? tomTrue : tomFalse);
			COM_VERIFY(spFont->SetUnderline( isUnderLine ));
		}
	}
	return hr;
}

HRESULT CRichEditPanel::SetTextFont(long nStart, long nEnd, HFONT	hFont, DWORD dwFontMask)
{

	HRESULT hr = E_FAIL;
	BOOL bRet = FALSE;

//#ifdef FTL_DEBUG
//	FTL::CFGdiObjectInfoDump fontInfoDump;
//	fontInfoDump.GetGdiObjectInfo(hFont);
//	FTLTRACEEX(FTL::tlDetail, TEXT("hFont Info = %s\n"), fontInfoDump.GetGdiObjectInfoString());
//#endif 

	LOGFONT logFont = {0};
	API_VERIFY(0 != ::GetObject(hFont, sizeof(LOGFONT), &logFont));
	if (bRet)
	{
		COM_VERIFY(SetTextFont(nStart, nEnd, &logFont, dwFontMask));
	}
	return hr;
}

HRESULT CRichEditPanel::SetTextFontName(long nStart, long nEnd, LPCTSTR pszFontName)
{
	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;

	FTLTRACEEX(FTL::tlInfo, TEXT("SetTextFontName %d to %d Name is %s\n"),
		nStart, nEnd, pszFontName);


	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));
		//FTL::CFTextFontDumper fontDumper(spFont, FTL::CFOutputWindowInfoOutput::Instance(), 0);

		if (spFont)
		{
			COM_VERIFY(spFont->SetName(CComBSTR(pszFontName)));
		}
	}
	return hr;
}

HRESULT CRichEditPanel::SetTextFontSize(long nStart, long nEnd, int nSize)
{
	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	FTLTRACEEX(FTL::tlInfo, TEXT("SetTextFontSize %d to %d Size if %d\n"),
		nStart, nEnd, nSize);

	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));
		//FTL::CFTextFontDumper fontDumper(spFont, FTL::CFOutputWindowInfoOutput::Instance(), 0);

		if (spFont)
		{
			long lFontHeight = -::MulDiv(nSize, m_yPixPerInch, 72);
			COM_VERIFY(spFont->SetSize(lFontHeight));
		}
	}
	return hr;

}

HRESULT CRichEditPanel::GetTextFontName(long nStart, long nEnd, LPTSTR pszFontName, int nCount)
{
	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;

	//Get Want Range and check every character for multi format selection -- font can not return tomUndefined
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	COM_VERIFY(spRange->GetStart(&nStart));
	COM_VERIFY(spRange->GetEnd(&nEnd));

	BOOL isSameFont = TRUE;
	BOOL isFirstCharFontName = TRUE;
	CComBSTR bstrFirstCharFontName;

	for (long nIndex = nStart; nIndex <= nEnd; nIndex++)
	{
		CComPtr<ITextRange>		spSubRange;
		COM_VERIFY(GetTextRange(nIndex, nIndex, spSubRange));

		if (SUCCEEDED(hr) && spSubRange)
		{
			CComPtr<ITextFont> spFont;
			COM_VERIFY(spSubRange->GetFont(&spFont));

			//FTL::CFTextFontDumper fontDumper(spFont, FTL::CFOutputWindowInfoOutput::Instance(), 0);
			if (spFont)
			{
				CComBSTR bstrName;
				COM_VERIFY(spFont->GetName(&bstrName));
				if (SUCCEEDED(hr))
				{
					if (isFirstCharFontName)
					{
						isFirstCharFontName = FALSE;
						bstrFirstCharFontName = bstrName;
						COM_VERIFY(StringCchCopy(pszFontName, nCount, bstrName));
					}
					else if(bstrFirstCharFontName != bstrName)
					{
						isSameFont = FALSE;
						hr = S_FALSE;
						COM_VERIFY(StringCchCopy(pszFontName, nCount, TEXT("")));
						break;
					}
				}
			}
		}

	}
	FTLTRACEEX(FTL::tlDetail, TEXT("GetTextFontName %d to %d is %s\n"),
		nStart, nEnd, pszFontName);

	return hr;
}

HRESULT CRichEditPanel::GetTextFontSize(long nStart, long nEnd, int* pFontSize)
{
	CHECK_POINTER_WRITABLE_RETURN_VALUE_IF_FAIL(pFontSize, E_POINTER);

	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));
		//FTL::CFTextFontDumper fontDumper(spFont, FTL::CFOutputWindowInfoOutput::Instance(), 0);

		if (spFont)
		{
			float fSize = 0.0f;
			COM_VERIFY(spFont->GetSize(&fSize));
			if (tomUndefined != (int)fSize)
			{
				*pFontSize = -::MulDiv(fSize, 72, m_yPixPerInch);
				//tomUndefined == -9999999 -- When choose multi size font
			}
			else
			{
				*pFontSize = -1;
				hr = S_FALSE;
			}

			FTLTRACEEX(FTL::tlDetail, TEXT("GetTextFontSize %d to %d FontSize is %d\n"),
				nStart, nEnd, *pFontSize);
		}
	}
	return hr;

}

HRESULT CRichEditPanel::SetTextForeColor(long nStart, long nEnd, COLORREF clr)
{
	FTLTRACEEX(FTL::tlInfo, TEXT("SetTextForeColor %d to %d Color is 0x%x\n"),
		nStart, nEnd, clr);

	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));

		if ((COLORREF)(-1) == clr)
		{
			COM_VERIFY(spFont->SetForeColor(tomAutoColor));
		}
		else
		{
			COM_VERIFY(spFont->SetForeColor(clr));
		}
	}
	return hr;
}

HRESULT CRichEditPanel::GetTextForeColor(long nStart, long nEnd, COLORREF* pClr)
{
	CHECK_POINTER_WRITABLE_RETURN_VALUE_IF_FAIL(pClr, E_INVALIDARG);

	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));
		LONG nColor = 0;
		COM_VERIFY(spFont->GetForeColor(&nColor));
		if (SUCCEEDED(hr))
		{
			if (tomUndefined == nColor)
			{
				*pClr = (COLORREF)(-1);
				hr = S_FALSE;
			}
			else if (tomAutoColor == nColor)
			{
				*pClr = (COLORREF)(-2);
				hr = S_FALSE;
			}
			else
			{
				*pClr = nColor;
			}
			
#ifdef FTL_DEBUG
			//FTL::CFStringFormater formater;
			//FTLTRACEEX(FTL::tlDetail, TEXT("GetTextForeColor %d to %d Color is %s\n"),
			//	nStart, nEnd, FTL::CFControlUtil::GetRichEditColorString(formater, *pClr));
#endif
		}
	}
	return hr;
}


HRESULT CRichEditPanel::SetTextBackColor(long nStart, long nEnd, COLORREF clr)
{
	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));

		if ((COLORREF)(-1) == clr)
		{
			COM_VERIFY(spFont->SetBackColor(tomAutoColor));
		}
		else
		{
			COM_VERIFY(spFont->SetForeColor(clr));
		}
	}
	return hr;
}

HRESULT CRichEditPanel::GetTextForeColor(long nStart, long nEnd, COLORREF& clr)
{
	HRESULT hr = E_FAIL;
	CComPtr<ITextRange>		spRange;
	COM_VERIFY(GetTextRange(nStart, nEnd, spRange));
	if (SUCCEEDED(hr) && spRange)
	{
		CComPtr<ITextFont> spFont;
		COM_VERIFY(spRange->GetFont(&spFont));
	}
	return hr;
}

HRESULT CRichEditPanel::GetTextFont(long nStart, long nEnd, PLOGFONT pLogFont)
{
	CHECK_POINTER_READABLE_DATA_RETURN_VALUE_IF_FAIL(pLogFont, sizeof(LOGFONT), FALSE);

	HRESULT hr = E_FAIL;
	return hr;
}

void CRichEditPanel::DoPaint(CDCHandle dcParent)
{
	HRESULT hr = E_FAIL;
	m_fNotify = FALSE;
	BOOL bRet = FALSE;

#ifdef FTL_DEBUG
	//FTL::HDCProperty	hdcProperty;
	//FTL::CFGdiUtil::GetHDCProperty(dcParent, &hdcProperty);
	//FTLTRACEEX(FTL::tlDetail, TEXT("hdcParent = %s\n"), hdcProperty.GetPropertyString(HDC_PROPERTY_GET_ALL));
#endif 
	
	//FTL::CFMMTextDCGuard mmTextDCGuard(dcParent);
	//CDCHandle dcHandle(hDC);
	//dcParent.DrawText(TEXT("CRichEditPanel::Draw"), -1, m_rcClient, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	//m_pDrawCanvas->DocToClient(&rcClient);

	CRect rcClient = m_rcClient;
	rcClient.NormalizeRect();

	//CRect rcMemory = rcClient;
	//rcMemory.OffsetRect(rcMemory.TopLeft());
	//CMemoryDC memDC ( dcParent.m_hDC, rcClient);
	//memDC.SetStretchBltMode(HALFTONE);
	//m_pDrawCanvas->PrepareDC(memDC.m_hDC);
	//memDC.FillSolidRect(rcClient, RGB(0, 255, 0));

	RECTL rcL = { rcClient.left, rcClient.top, rcClient.right, rcClient.bottom };
	LONG lViewId = TXTVIEW_ACTIVE;  //TXTVIEW_INACTIVE
	//dcParent.FillSolidRect(rcClient, RGB(255,0,0));

	if (m_fInplaceActive)
	{
		//CRect rcDevice = rcClient;
		//m_pDrawCanvas->DocToClient(&rcDevice);
		//COM_VERIFY(m_spTextServices->OnTxInPlaceActivate(&rcDevice));
	}
	//if (!m_fInplaceActive)
	//{
	//	//GetControlRect(&rcClient);
	//	prc = &rcClient;
	//	lViewId = TXTVIEW_ACTIVE;//TXTVIEW_INACTIVE;
	//}
	//CRect rcClient = rcClient;

	//rcNewClient.SetRectEmpty();
	//CDC dcTemp;
	//API_VERIFY(!dcTemp.CreateCompatibleDC(dcParent));

	//CDC tmpDC;
	//tmpDC.CreateCompatibleDC(dcParent);
	//LONG newWidth = 0;
	//LONG newHeight = 0;
	//SIZEL newSizeLExtent = {0};
	//COM_VERIFY(m_spTextServices->TxGetNaturalSize(DVASPECT_CONTENT,
	//	dcParent, 
	//	NULL, //tmpDC,
	//	NULL,
	//	TXTNS_FITTOCONTENT,
	//	&newSizeLExtent,
	//	&newWidth,
	//	&newHeight));

	COM_VERIFY(m_spTextServices->TxDraw(
		DVASPECT_CONTENT,  		// Draw Aspect
		/*-1*/0,				// Lindex
		NULL,					// Info for drawing optimazation
		NULL,					// target device information
		dcParent.m_hDC,				// Draw device HDC
		NULL, 				   	// Target device HDC
		&rcL,					// Bounding client rectangle
		NULL, 					// Clipping rectangle for metafiles
		&rcClient, //(RECT *) m_rcClient,	// Update rectangle
		NULL, 	   				// Call back function
		0,					// Call back parameter
		lViewId));				// What view of the object				
	
#ifdef FTL_DEBUG
	//CString strTmpFileName;
	//strTmpFileName.Format(TEXT("G:\\FJCODE_GOOGLE\\TmpTxDrawFile_%d.PNG"), GetTickCount());
	//API_VERIFY(FTL::CFGdiUtil::SaveDCImageToFile(memDC, strTmpFileName));
#endif 

	//FTL::HDCProperty	hdcProperty;
	//FTL::CFGdiUtil::GetHDCProperty(dcTemp, &hdcProperty);

	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::DoPaint, dcParent=0x%x\n"), dcParent);
	m_fNotify = TRUE;

	// Put a frame around the control so it can be seen
	//FrameRect(dcParent, &m_rcClient, (HBRUSH) GetStockObject(GRAY_BRUSH));

	//if(TxGetEffects() == TXTEFFECT_SUNKEN)
	//	DrawSunkenBorder(hwnd, (HDC) wparam);
}

HRESULT CRichEditPanel::SetClientBound(const RECT *pRcBound, RECT* pNewRcBound /* = NULL */, BOOL fUpdateExtent /* = TRUE */)
{
	//FTLASSERT(pRcBound->left < pRcBound->right);
	//FTLASSERT(pRcBound->top < pRcBound->bottom);
	HRESULT hr = S_OK;
	m_rcBound = *pRcBound;
	m_rcBound.NormalizeRect();
#if 0
	m_rcBound.NormalizeRect();
	CSize szMin = GetMinBoundSize(MIN_ROW_COUNT, MIN_COL_COUNT);
	
	if (m_rcBound.left + MARGIN_LEFT + szMin.cx + MARGIN_RIGHT > m_rcBound.right)
	{
		m_rcBound.right = m_rcBound.left + MARGIN_LEFT + szMin.cx + MARGIN_RIGHT;
	}
	if (m_rcBound.top + MARGIN_TOP + szMin.cy + MARGIN_BOTTOM > m_rcBound.bottom)
	{
		m_rcBound.bottom = m_rcBound.top + MARGIN_TOP + szMin.cy + MARGIN_BOTTOM;
	}
#endif
	m_rcClient = m_rcBound;
	m_rcClient.DeflateRect(RTPANEL_MARGIN_LEFT, RTPANEL_MARGIN_TOP, RTPANEL_MARGIN_RIGHT, RTPANEL_MARGIN_BOTTOM);
	
	if (pNewRcBound)
	{
		*pNewRcBound = m_rcBound;
	}
	
	float fCurZoom = 1.0f;

	if (fUpdateExtent)
	{
		m_szlExtent.cx = DXtoHimetricX(m_rcClient.Width() , m_xPixPerInch);
		m_szlExtent.cy = DYtoHimetricY(m_rcClient.Height(), m_yPixPerInch);

		UINT nOldNnumerator = 1;
		UINT nOldDenominator = 1;
		LRESULT lResult = 0;
		COM_VERIFY(m_spTextServices->TxSendMessage(EM_GETZOOM, (WPARAM)&nOldNnumerator, (WPARAM)&nOldDenominator, &lResult));
		if (nOldDenominator != 0)
		{
			FTLASSERT(FALSE);
			fCurZoom = (float)nOldNnumerator/nOldDenominator;
			FTLTRACEEX(FTL::tlInfo, TEXT("SetClientBound fCurZoom=%f( %d/%d )\n"),
				(float)nOldNnumerator/nOldDenominator, nOldNnumerator, nOldDenominator);
			m_szlExtent.cx = m_szlExtent.cx / fCurZoom;
			m_szlExtent.cy = m_szlExtent.cy / fCurZoom;
		}

		//TODO: property change ?
		COM_VERIFY(m_spTextServices->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE));
	}


	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::SetClientBound, fUpdateExtent=%d, fCurZoom=%f, inRect=(%d,%d)-(%d,%d), %dx%d,")
		TEXT("outRect=(%d,%d)-(%d,%d), %dx%d, m_szExtent=%dx%d\n"),
		fUpdateExtent, fCurZoom, 
		pRcBound->left, pRcBound->top, pRcBound->right, pRcBound->bottom, 
		pRcBound->right- pRcBound->left, pRcBound->bottom - pRcBound->top,
		m_rcBound.left, m_rcBound.top, m_rcBound.right, m_rcBound.bottom, m_rcBound.Width(), m_rcBound.Height(),
		m_szlExtent.cx, m_szlExtent.cy);

	return hr;
}

void CRichEditPanel::SetExtent(SIZEL *psizelExtent, BOOL fNotify)
{
	HRESULT hr = E_FAIL;
	FTLASSERT(FALSE);

	//static INT zoom = 1;
	//zoom *= 2;

	// Set our extent
	m_szlExtent = *psizelExtent; 
	double zoom = (double)m_rcClient.Height() * 2540 / psizelExtent->cy * m_yPixPerInch;
	{
		COM_VERIFY(m_spTextServices->TxSendMessage(EM_SETZOOM, zoom * 10000, 10000, NULL));
	}
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::SetExtent,psizelExtent=[%d,%d], zoom=%f\n "),
		psizelExtent->cx, psizelExtent->cy, zoom);

	// Notify the host that the extent has changed
	if (fNotify)
	{
		COM_VERIFY(m_spTextServices->OnTxPropertyBitsChange(TXTBIT_EXTENTCHANGE, TXTBIT_EXTENTCHANGE));
	}
}

void CRichEditPanel::SetZoom(UINT Numerator, UINT Denominator)
{
	HRESULT hr = E_FAIL;
#if 0 
	UINT nOldNnumerator = 1;
	UINT nOldDenominator = 1;
	LRESULT lResult = 0;
	COM_VERIFY(m_spTextServices->TxSendMessage(EM_GETZOOM, (WPARAM)nOldDenominator, (WPARAM)nOldDenominator, &lResult));

	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::SetZoom,newZoom=%f( %d/%d ), oldZoom=%f( %d/%d )\n"),
		(float)Numerator/Denominator, Numerator, Denominator,
		(float)nOldNnumerator/nOldDenominator, nOldNnumerator, nOldDenominator);
	
	COM_VERIFY(m_spTextServices->TxSendMessage(EM_SETZOOM, Numerator, Denominator, &lResult));
#endif 
}

HRESULT CRichEditPanel::TxGetExtent( LPSIZEL lpExtent )
{
	HRESULT hr = S_OK;
//#ifdef FTL_DEBUG
	//static SIZEL oldExtent = {0};
	//if (oldExtent.cx != m_szlExtent.cx || oldExtent.cy != m_szlExtent.cy)
	{
		FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetExtent, lpExtent={%d,%d}, m_szlExtent= {%d,%d}\n"), 
			lpExtent->cx, lpExtent->cy, m_szlExtent.cx, m_szlExtent.cy);

		*lpExtent = m_szlExtent;

		//float fCurZoom = 1.0f;

		//oldExtent = m_szlExtent;
	}
//#endif 
	return hr;
}

VOID CRichEditPanel::SetNotifyCallback(INotifyCallBack* pNotifyCallback)
{
	m_pNotifyCallback = pNotifyCallback;
}

HDC CRichEditPanel::TxGetDC()
{
	ATLASSERT(::IsWindow(m_hWndOwner));
	HDC hDC = ::GetDC(m_hWndOwner);
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetDC, getDC=0x%x\n"), hDC);
	return hDC;
}

INT CRichEditPanel::TxReleaseDC( HDC hdc )
{
	ATLASSERT(::IsWindow(m_hWndOwner));
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxReleaseDC hdc=0x%x\n"), hdc);
	return ReleaseDC(m_hWndOwner, hdc);
}

BOOL CRichEditPanel::TxShowScrollBar( INT fnBar, BOOL fShow )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxShowScrollBar, fnBar=%d, fShow=%d\n"), fnBar, fShow);
	BOOL bRet = FALSE;
	//API_VERIFY(::ShowScrollBar(m_hWndOwner, fnBar, fShow));
	return TRUE;
}

BOOL CRichEditPanel::TxEnableScrollBar( INT fuSBFlags, INT fuArrowflags )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxEnableScrollBar, fuSBFlags=%d, fuArrowflags=%d\n"),
		fuSBFlags, fuArrowflags);
	BOOL bRet = FALSE;
	//API_VERIFY(::EnableScrollBar(m_hWndOwner, fuSBFlags, fuArrowflags));
	return bRet;
}

BOOL CRichEditPanel::TxSetScrollRange( INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxSetScrollRange, fnBar=%d, nMinPos=%d, nMaxPos=%d, fRedraw=%d, m_rcClient.Height=%d\n"),
		fnBar, nMinPos, nMaxPos, fRedraw, m_rcClient.Height());
	FTLASSERT(SB_VERT == fnBar);
	FTLASSERT(FALSE);  //TxGetScrollBars return 0

	BOOL bRet = FALSE;
	//if (m_fAutoVExpand && nMaxPos - nMinPos > m_rcClient.Height())
	//{
	//	if (m_pNotifyCallback)
	//	{
	//		m_pNotifyCallback->OnExpand(fnBar, nMaxPos - nMinPos + MARGIN_TOP + MARGIN_BOTTOM);
	//	}
	//}
	API_VERIFY(::SetScrollRange(m_hWndOwner, fnBar, nMinPos, nMaxPos, fRedraw));
	return bRet;
}

BOOL CRichEditPanel::TxSetScrollPos( INT fnBar, INT nPos, BOOL fRedraw )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxSetScrollPos, fnBar=%d, nPos=%d, fRedraw=%d\n"),
		fnBar, nPos, fRedraw);
	BOOL bRet = FALSE;
	//API_VERIFY(SetScrollPos(m_hWndOwner, fnBar, nPos, fRedraw));
	return bRet;
}

void CRichEditPanel::TxInvalidateRect( LPCRECT prc, BOOL fMode )
{
	if (prc)
	{
		FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxInvalidateRect, *pRC=[%d,%d]-[%d,%d], fMode=%d\n"),
			prc->left, prc->top, prc->right, prc->bottom, fMode);
	}
	else
	{
		FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxInvalidateRect, prc is NULL, fMode=%d\n"),
			fMode);
	}
	::InvalidateRect(m_hWndOwner, prc, fMode);
}

void CRichEditPanel::TxViewChange( BOOL fUpdate )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxViewChange, fUpdate =%d\n"), fUpdate);
	if (fUpdate)
	{
		CRect rcClient;
		m_pDrawCanvas->DocToClient(&rcClient);
		InvalidateRect(m_hWndOwner, &rcClient, FALSE);
		//::UpdateWindow (m_hWndOwner);
	}
}

BOOL CRichEditPanel::TxCreateCaret( HBITMAP hbmp, INT xWidth, INT yHeight )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxCreateCaret, hbmp=0x%x, xWidth=%d, yHeight=%d\n"),
		hbmp, xWidth, yHeight);
	BOOL bRet = FALSE;
	API_VERIFY(::CreateCaret (m_hWndOwner, hbmp, xWidth, yHeight));
	return bRet;
}

BOOL CRichEditPanel::TxShowCaret( BOOL fShow )
{
	//FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxShowCaret, fShow=%d\n"), fShow);

	BOOL bRet = FALSE;
	if (fShow)
	{
		API_VERIFY_EXCEPT1(::ShowCaret(m_hWndOwner), ERROR_ACCESS_DENIED);//, ERROR_INVALID_WINDOW_HANDLE);
	}
	else
	{
		API_VERIFY_EXCEPT1(::HideCaret(m_hWndOwner), ERROR_ACCESS_DENIED);//, ERROR_INVALID_WINDOW_HANDLE);
	}
	return bRet;	
}

BOOL CRichEditPanel::TxSetCaretPos( INT x, INT y )
{
	//FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxSetCaretPos, x=%d, y=%d\n"), x, y);
	BOOL bRet = FALSE;
	API_VERIFY(::SetCaretPos(x, y));
	return bRet;
}

BOOL CRichEditPanel::TxSetTimer( UINT idTimer, UINT uTimeout )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxSetTimer, idTimer=%d, uTimeOut=%d\n"), idTimer, uTimeout);
	//m_fTimer = TRUE;
	BOOL bRet = FALSE;
	//API_VERIFY(::SetTimer(m_hWndOwner, idTimer, uTimeout, NULL));
	return bRet;
}

void CRichEditPanel::TxKillTimer( UINT idTimer )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxKillTimer, idTimer=%d\n"), idTimer);
	BOOL bRet = FALSE;
	//API_VERIFY(::KillTimer(m_hWndOwner, idTimer));
	//m_fTimer = FALSE;
}

void CRichEditPanel::TxScrollWindowEx( INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxScrollWindowEx\n"));
	//FTLTRACEEX(FTL::tlDetail, FALSE);
	//m_spTextServices->
}

void CRichEditPanel::TxSetCapture( BOOL fCapture )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxSetCapture, fCapture=%d\n"), fCapture);
	BOOL bRet = FALSE;
	if (fCapture)
	{
		::SetCapture(m_hWndOwner);
	}
	else
	{
		API_VERIFY(::ReleaseCapture());
	}
	m_fCapture = fCapture;
}

void CRichEditPanel::TxSetFocus()
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxSetFocus\n"));

	::SetFocus(m_hWndOwner);
}

void CRichEditPanel::TxSetCursor( HCURSOR hcur, BOOL fText )
{
	FTLTRACEEX(FTL::tlTrace, TEXT("CRichEditPanel::TxSetCursor, hCur=0x%x, fText=%d\n"),
		hcur, fText);
	::SetCursor(hcur);	
}

BOOL CRichEditPanel::TxScreenToClient( LPPOINT lppt )
{
	BOOL bRet = FALSE;
	POINT ptOld = *lppt;
	API_VERIFY(::ScreenToClient(m_hWndOwner, lppt));
	//FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxScreenToClient, [%d,%d] => [%d,%d]\n"), ptOld.x, ptOld.y, lppt->x, lppt->y);
	return bRet;
}

BOOL CRichEditPanel::TxClientToScreen( LPPOINT lppt )
{
	BOOL bRet = FALSE;
	API_VERIFY(::ClientToScreen(m_hWndOwner, lppt));

	POINT ptOld = *lppt;
	//FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxClientToScreen, [%d,%d] => [%d,%d]\n"), ptOld.x, ptOld.y, lppt->x, lppt->y);
	return bRet;
}

HRESULT CRichEditPanel::TxActivate( LONG * plOldState )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxActivate"));
	*plOldState = m_lState;
	return S_OK;
}

HRESULT CRichEditPanel::TxDeactivate( LONG lNewState )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxDeactivate, lNewState=%d\n"), lNewState);
	return S_OK;
}

HRESULT CRichEditPanel::TxGetClientRect( LPRECT prc )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetClientRect, m_rcClient=[%d,%d]-[%d,%d]\n"),
		m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	CRect rcClient = m_rcClient;
	m_pDrawCanvas->DocToClient(&rcClient);
	*prc = rcClient;

	return S_OK;
}

HRESULT CRichEditPanel::SetText(LPCTSTR pszText)
{
	HRESULT hr = E_FAIL;
	//m_spTextServices->TxSetText(pszText);
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::SetText, Text=%s\n"), pszText);
	// Get a range for the whole of the text in the control
	CComPtr<ITextRange> spTextRange;
	COM_VERIFY(m_spTextDocument->Range(0, 0, &spTextRange));
	COM_VERIFY_EXCEPT1(spTextRange->MoveEnd(tomStory, 1, NULL), S_FALSE);
	
	// Replace the whole text
	COM_VERIFY(spTextRange->SetText(CComBSTR(pszText)));

#ifdef FTL_DEBUG
	//USES_CONVERSION;
	//CComBSTR bstrNewText;
	//COM_VERIFY(spTextRange->GetText(&bstrNewText));
	//FTLTRACEEX(FTL::tlDetail, TEXT("After SetText, New String=%s\n"), COLE2CT(bstrNewText));
#endif 
	return hr;
}

HRESULT CRichEditPanel::Range(long cpFirst, long cpLim, ITextRange** ppRange)
{
	// Get the given range of text in the control
	HRESULT hr = E_FAIL;
	COM_VERIFY(m_spTextDocument->Range(cpFirst,cpLim,ppRange));
	if (SUCCEEDED(hr))
	{
		USES_CONVERSION;
		CComBSTR bstrText;
		COM_VERIFY((*ppRange)->GetText(&bstrText));
		FTLTRACEEX(FTL::tlDetail, TEXT("After Call Range For [%d,%d] Text is %s\n"), cpFirst, cpLim, COLE2CT(bstrText));
	}
	return hr;
}


//
HRESULT CRichEditPanel::TxGetViewInset( LPRECT prc )
{
	LONG nLeft = ::MulDiv(RTPANEL_MARGIN_LEFT , HIMETRIC_INCH, m_xPixPerInch);
	LONG nTop = ::MulDiv(RTPANEL_MARGIN_TOP , HIMETRIC_INCH, m_yPixPerInch);
	LONG nRight = ::MulDiv(RTPANEL_MARGIN_RIGHT , HIMETRIC_INCH, m_xPixPerInch);
	LONG nBottom = ::MulDiv(RTPANEL_MARGIN_BOTTOM , HIMETRIC_INCH, m_yPixPerInch);
	//*prc = CRect(nLeft, nTop, nRight, nBottom);

	*prc = CRect(0, 0, 0, 0);
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetViewInset, prc=[%d,%d]-[%d,%d]\n"),
		prc->left, prc->top, prc->right, prc->bottom);
	return S_OK;
}

HRESULT CRichEditPanel::TxGetCharFormat( const CHARFORMATW **ppCF )
{
	FTL::CFStringFormater strFormaterEffects;
	FTL::CFStringFormater strFormaterMasks;

	//FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetCharFormat,szFaceName=%s, crTextColor=0x%x, yHeight=%d, yOffset=%d\n")
	//	TEXT("Effects=%s, Mask=%s\n"),
	//	m_charFormat.szFaceName, m_charFormat.crTextColor, m_charFormat.yHeight, m_charFormat.yOffset,
	//	FTL::CFControlUtil::GetCharFormatEffectAndMaskString(strFormaterEffects, m_charFormat.dwEffects),
	//	FTL::CFControlUtil::GetCharFormatEffectAndMaskString(strFormaterMasks, m_charFormat.dwMask)
	//	);

	*ppCF = &m_charFormat;
	return S_OK;
}

HRESULT CRichEditPanel::TxGetParaFormat( const PARAFORMAT **ppPF )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetParaFormat,wNumbering=%d, dxStartIndent=%d, dxRightIndent=%d, dxOffset=%d, cTabCount=%d\n"),
		m_paraFormat.wNumbering, m_paraFormat.dxStartIndent, 
		m_paraFormat.dxRightIndent, m_paraFormat.dxOffset, m_paraFormat.cTabCount);
	*ppPF = &m_paraFormat;
	return S_OK;
}

COLORREF CRichEditPanel::TxGetSysColor( int nIndex )
{
	//FTLTRACEEX(FTL::tlInfo, TEXT("CRichEditPanel::TxGetSysColor, nIndex=%s\n"), 
	//	FTL::CFWinUtil::GetColorString(FTL::ctSysColor, nIndex));

	if (nIndex == COLOR_WINDOW)
	{
		if(!m_fNotSysBkgnd)
			return GetSysColor(COLOR_WINDOW);
		return m_crBackground;
	}
	return GetSysColor(nIndex);
}

HRESULT CRichEditPanel::TxGetBackStyle( TXTBACKSTYLE *pstyle )
{
	*pstyle = m_fTransparent ? TXTBACK_TRANSPARENT : TXTBACK_OPAQUE ;
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetBackStyle, m_fTransparent=%d, pStyle=%d\n"), m_fTransparent, *pstyle);
	return S_OK;
}

HRESULT CRichEditPanel::TxGetMaxLength( DWORD *plength )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetMaxLength, m_cchTextMost=%d\n"), m_cchTextMost);

	*plength = m_cchTextMost;
	return S_OK;

}

HRESULT CRichEditPanel::TxGetScrollBars( DWORD *pdwScrollBar )
{
	//if return 0, then do not allow scrollbars
	*pdwScrollBar = 0; //WS_VSCROLL ; //ES_AUTOVSCROLL

	//*pdwScrollBar =  m_dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
	//	ES_AUTOHSCROLL | ES_DISABLENOSCROLL);


	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetScrollBars, *pdwScrollBar=0x%x\n"), *pdwScrollBar);

	return S_OK;
}

HRESULT CRichEditPanel::TxGetPasswordChar( TCHAR *pch )
{
	*pch = m_chPasswordChar;
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetPasswordChar, char=%c\n"), m_chPasswordChar);
	return S_OK;
}

HRESULT CRichEditPanel::TxGetAcceleratorPos( LONG *pcp )
{
	*pcp = -1;
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetAcceleratorPos, AcceleratorPos=%d\n"), *pcp);
	return S_OK;
}

HRESULT CRichEditPanel::OnTxCharFormatChange( const CHARFORMATW * pcf )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::OnTxCharFormatChange\n"));
	FTLASSERT(FALSE);
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::OnTxParaFormatChange( const PARAFORMAT * ppf )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::OnTxParaFormatChange\n"));
	FTLASSERT(FALSE);
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::TxGetPropertyBits( DWORD dwMask, DWORD *pdwBits )
{
	//FTL::CFStringFormater propertyFormater;
	//FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetPropertyBits, dwMask=%s\n"),
	//	FTL::CFControlUtil::GetRichEditPropertyBits(propertyFormater, dwMask, TEXT("|")));
	DWORD dwProperties =  TXTBIT_RICHTEXT|TXTBIT_MULTILINE|TXTBIT_SHOWACCELERATOR|TXTBIT_WORDWRAP |
		TXTBIT_VERTICAL | TXTBIT_DISABLEDRAG;
		
		//TXTBIT_RICHTEXT | TXTBIT_MULTILINE | TXTBIT_SHOWACCELERATOR | TXTBIT_WORDWRAP;

#if 0
	if (m_fRich)
	{
		dwProperties = TXTBIT_RICHTEXT;
	}

	if (m_dwStyle & ES_MULTILINE)
	{
		dwProperties |= TXTBIT_MULTILINE;
	}

	if (m_dwStyle & ES_READONLY)
	{
		dwProperties |= TXTBIT_READONLY;
	}


	if (m_dwStyle & ES_PASSWORD)
	{
		dwProperties |= TXTBIT_USEPASSWORD;
	}

	if (!(m_dwStyle & ES_NOHIDESEL))
	{
		dwProperties |= TXTBIT_HIDESELECTION;
	}

	if (m_fEnableAutoWordSel)
	{
		dwProperties |= TXTBIT_AUTOWORDSEL;
	}

	if (m_fVertical)
	{
		dwProperties |= TXTBIT_VERTICAL;
	}

	if (m_fWordWrap)
	{
		dwProperties |= TXTBIT_WORDWRAP;
	}

	if (m_fAllowBeep)
	{
		dwProperties |= TXTBIT_ALLOWBEEP;
	}

	if (m_fSaveSelection)
	{
		dwProperties |= TXTBIT_SAVESELECTION;
	}
#endif 
	*pdwBits = dwProperties & dwMask; 
	return S_OK;
}

HRESULT CRichEditPanel::TxNotify( DWORD iNotify, void *pv )
{
#if 0
	FTL::CFStringFormater formater;
	FTL::CFControlUtil::GetEditNotifyCodeString(formater, iNotify, pv);
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxNotify, iNotify=%s, pv=0x%x\n"),
		formater.GetString(), pv);
#endif 
	//dwBits := dwBits and TXTBIT_ALL_NOTIFICATIONS;
	//FServices.OnTxPropertyBitsChange(dwBits, dwBits);

	if (m_pNotifyCallback && m_fNotify)
	{
		m_pNotifyCallback->OnNotify(iNotify, pv);
	}
	// Claim to have handled the notifcation, even though we always ignore it
	return S_OK;
}

HIMC CRichEditPanel::TxImmGetContext()
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxImmGetContext\n"));
	return NULL;
}

void CRichEditPanel::TxImmReleaseContext( HIMC himc )
{
	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxImmReleaseContext, himc=0x%x\n"), himc);
}

HRESULT CRichEditPanel::TxGetSelectionBarWidth( LONG *lSelBarWidth )
{
	*lSelBarWidth = m_lSelBarWidth;

	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::TxGetSelectionBarWidth, m_lSelBarWidth=%d\n"), m_lSelBarWidth);
	return E_NOTIMPL;
}

BOOL CRichEditPanel::_IsNeedHandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRet = FALSE;
	if (IsActive())
	{
		do 
		{
#if 1
			
			CPoint ptClient;
			GetCursorPos(&ptClient);
			TxScreenToClient(&ptClient);
			//m_pDrawCanvas->ClientToDoc(&ptLocalClient);
			CRect rcDevice = m_rcBound;
			m_pDrawCanvas->DocToClient(&rcDevice);
			rcDevice.DeflateRect(TRACK_MARGIN, TRACK_MARGIN, TRACK_MARGIN, TRACK_MARGIN);
			if (rcDevice.PtInRect(ptClient))
			{
				if (WM_LBUTTONDOWN == uMsg)
				{
					
					//make sure canvas capture
					//if (m_pDrawCanvas->GetCurrentTool())
					//{
					//	m_pDrawCanvas->GetCurrentTool()->OnLButtonDown(m_pDrawCanvas, (UINT)pMsg->wParam, pMsg->pt);
					//}
					bRet = TRUE;
					break;
				}
				else if (WM_MOUSEWHEEL == uMsg)
				{
					bRet = FALSE; //TRUE;
					break;
				}
			}
			if (m_fCapture) // && selMode == smNone)
			{
				if (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST)
				{

					if (WM_MOUSEMOVE == uMsg
						|| WM_LBUTTONDOWN == uMsg
						|| WM_LBUTTONUP == uMsg)
					{
						bRet = TRUE;
						break;
					}
				}
			}
#endif 
			if(WM_KEYFIRST <= uMsg && uMsg <= WM_KEYLAST)
			{
				if(VK_ESCAPE != uMsg)
				{
					bRet = TRUE;
				}
			}
		} while (0);
	}
	return bRet;
}

LRESULT CRichEditPanel::OnKeyMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HRESULT hr = E_FAIL;
	LRESULT lResult = 0;
	hr = m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult);
	if (S_OK != hr)
	{
		bHandled = FALSE;
	}
	//else
	//{
	//	bHandled = FALSE;
	//	//COM_VERIFY(m_spTextServices->OnTxPropertyBitsChange(TXTBIT_SCROLLBARCHANGE, TXTBIT_SCROLLBARCHANGE));
	//}
	return lResult;
}

LRESULT CRichEditPanel::OnMouseMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HRESULT hr = E_FAIL;
	LRESULT lResult = 0;

	if (WM_LBUTTONDOWN == uMsg)
	{
		//CComPtr<ITextSelection> spSelection;
		//COM_VERIFY(m_spTextDocument->GetSelection(&spSelection));
		//if (spSelection)
		//{
			//long nFlags = 0;
			//COM_VERIFY(spSelection->GetFlags(&nFlags));

			//long nType = 0;
			//COM_VERIFY(spSelection->GetType(&nType));
			//COM_VERIFY(m_spTextDocument->BeginEditCollection());

			//if (nFlags & tomSelStartActive == 0)
			//{
			//	nFlags |= tomSelStartActive;
			//	//COM_VERIFY(spSelection->SetFlags(nFlags));
			//}
		//}

		//BOOL bRet = FALSE;
		//POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		//API_VERIFY(TxSetCaretPos(pt.x, pt.y));
		//API_VERIFY(TxClientToScreen(&pt));
		//CComPtr<ITextRange> spRange;
		//COM_VERIFY(m_spTextDocument->RangeFromPoint(pt.x, pt.y, &spRange));
		//if (spRange)
		//{
		//	
		//	COM_VERIFY(spRange->Select());
		//}


		//SetActive(FALSE);
		//SetActive(TRUE);
		//TxSetCapture(TRUE);
	}


	hr = m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult);
	if (S_OK != hr)
	{
		bHandled = FALSE;
	}
	return lResult;
}

BOOL CRichEditPanel::HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	//DUMP_WINDOWS_MSG(__FILE__LINE__, (DEFAULT_DUMP_FILTER_MESSAGE | DUMP_FILTER_TIMER | DUMP_FILTER_KEYDOWN), pMsg->message, pMsg->wParam, pMsg->lParam);
	BOOL bRet = FALSE;
	HRESULT hr = E_FAIL;
	//BOOL bWillHandle = FALSE;
	if (_IsNeedHandleMsg(uMsg, wParam, lParam))
	{
		bRet = ProcessWindowMessage(pView->GetHWnd(), uMsg, wParam, lParam, lResult, 0);
		if (!bRet)
		{
			hr = m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult);
			if (S_OK == hr)
			{
				return TRUE;
			}
			//if (S_FALSE == hr)
			//{
			//	return FALSE;
			//}
		}

	//	if (WM_MOUSEMOVE != pMsg->message)
	//	{
	//		FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::PreTranslateMessage TxSendMessage for %s return 0x%x\n"), 
	//			FTL::CFMessageInfo(pMsg->message, pMsg->wParam, pMsg->lParam).GetConvertedInfo(), hr);
	//	}

//#if 1
//		//Handle Mouse Message
//		if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
//		{
//			POINT ptLocalClient = pMsg->pt;
//			TxScreenToClient(&ptLocalClient);
//			if (m_rcClient.PtInRect(ptLocalClient))
//			{
//				//if in Rich Edit Panel Client
//				hr = m_spTextServices->TxSendMessage(pMsg->message, pMsg->wParam, pMsg->lParam, &lResult);
//				if (WM_MOUSEMOVE != pMsg->message)
//				{
//					FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::PreTranslateMessage TxSendMessage for %s return 0x%x\n"), 
//						FTL::CFMessageInfo(pMsg->message, pMsg->wParam, pMsg->lParam).GetConvertedInfo(), hr);
//				}
//				if (S_OK == hr)
//				{
//					return TRUE;
//				}
//			}
//		}
//#else
//		bRet = ProcessWindowMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, lResult, 0);
//
//		if (bRet)
//		{
//			FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::PreTranslateMessage Handle Msg(%s) return TRUE\n"),
//				FTL::CFMessageInfo(pMsg->message, pMsg->wParam, pMsg->lParam).GetConvertedInfo());
//		}
//#endif 
	}

	//HRESULT hr = m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult);
	//if (hr == S_FALSE)
	//{
	//	lResult = ::DefWindowProc(m_hWndOwner, uMsg, wParam, lParam);
	//}

	return bRet;
}

//LRESULT CRichEditPanel::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	FTLTRACEEX(FTL::tlDetail, TEXT("CRichEditPanel::OnSetFocus\n"));
//
//	return 0;
//}
//
//LRESULT CRichEditPanel::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	HRESULT hr = E_FAIL;
//	LRESULT lResult = 0;
//	COM_VERIFY(m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult));
//
//	COM_VERIFY(m_spTextServices->OnTxPropertyBitsChange(TXTBIT_SCROLLBARCHANGE, TXTBIT_SCROLLBARCHANGE));
//
//	return lResult;
//}

//LRESULT CRichEditPanel::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	HRESULT hr = E_FAIL;
//	LRESULT lResult = 0;
//	switch(TCHAR(wParam))
//	{
//	case VK_BACK:
//	//	FTLTRACEEX(FTL::tlDetail, TEXT("In KeyDown for Back\n"));
//	//	break;
//	default:
//		hr = m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult);
//		break;
//	}
//	if (S_FALSE == hr)
//	{
//		bHandled = FALSE;
//	}
//	return lResult;
//}

HRESULT CRichEditPanel::_SetPropertyBits(DWORD dwProperty)
{
//	var
//dwMask: DWORD;
//	begin
//Value := Value and TXTBIT_ALL_PROPERTIES;
//	if FPropertyBits <> Value then
//		begin
//dwMask := FPropertyBits xor Value;
//FPropertyBits := Value;
//	FServices.OnTxPropertyBitsChange(dwMask, Value and dwMask);
//	end;
	return E_NOTIMPL;
}

SIZE CRichEditPanel::GetMinBoundSize(int nRow, int nCol)
{
	//TODO:check font height
	return CSize(100, 30);
}

DWORD CALLBACK EditStreamOutCallback(DWORD_PTR dwCookie,
						 LPBYTE pbBuff,
						 LONG cb,
						 LONG *pcb
						 )
{
	HRESULT hr = E_FAIL;

	HGLOBAL hGlobalStream = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, cb);
	if (hGlobalStream)
	{
		FTLTRACEEX(FTL::tlInfo, TEXT("EditStreamOutCallback(%d) = %s\n"), cb, CA2T((CHAR*)pbBuff));

		CopyMemory((hGlobalStream), pbBuff, cb);
		IStream** ppStream = reinterpret_cast<IStream**>(dwCookie);
		COM_VERIFY(CreateStreamOnHGlobal(hGlobalStream, TRUE, ppStream));
		if (FAILED(hr))
		{
			GlobalFree(hGlobalStream);
		}
	}
	return 0;
	//USES_CONVERSION;
	//CStringA strInfo((CHAR*)pbBuff);
	//FTLTRACEEX(FTL::tlDetail, TEXT("On EditStreamCallback, cb=%d, pbBufff=%s\n"), cb, CA2T(strInfo));
	//return 0;
}


HRESULT CRichEditPanel::GetTextStream(long nStart, long nEnd, IStream** ppStream)
{
	CHECK_POINTER_RETURN_VALUE_IF_FAIL(ppStream, E_POINTER);
	HRESULT hr = E_FAIL;

	EDITSTREAM	editStream = {0};
	editStream.dwCookie = DWORD_PTR(ppStream);
	editStream.dwError = 0;
	editStream.pfnCallback = EditStreamOutCallback;

	COM_VERIFY(m_spTextServices->TxSendMessage(EM_STREAMOUT, (WPARAM)(SF_RTF), (LPARAM)&editStream, NULL));

	return hr;
}

DWORD CALLBACK EditStreamInCallback(DWORD_PTR dwCookie,
									 LPBYTE pbBuff,
									 LONG cb,
									 LONG *pcb
									 )
{
	HRESULT hr = E_FAIL;
	IStream* pStream = reinterpret_cast<IStream*>(dwCookie);
	if (pStream)
	{
		ULARGE_INTEGER nLength;
		nLength.QuadPart = 0L;

		LARGE_INTEGER nStart;
		nStart.LowPart = 0;
		nStart.HighPart = 0;
		COM_VERIFY(pStream->Seek(nStart, STREAM_SEEK_END, &nLength));
		COM_VERIFY(pStream->Seek(nStart, STREAM_SEEK_SET, NULL));

		ULONG unWrite = 0;
		COM_VERIFY(pStream->Read(pbBuff, FTL_MIN(cb, nLength.LowPart), &unWrite));
		*pcb = (LONG)unWrite;

		FTLTRACEEX(FTL::tlInfo, TEXT("EditStreamInCallback(%d) = %s\n"), *pcb, CA2T((CHAR*)pbBuff));
	}
	return 0;
}

HRESULT CRichEditPanel::SetTextStream(long nStart, long nEnd, IStream* pStream)
{
	HRESULT hr = E_FAIL;

	EDITSTREAM	editStream = {0};
	editStream.dwCookie = DWORD_PTR(pStream);
	editStream.dwError = 0;
	editStream.pfnCallback = EditStreamInCallback;
	LRESULT lResult = 0;
	COM_VERIFY(m_spTextServices->TxSendMessage(EM_STREAMIN, (WPARAM)(SF_RTF), (LPARAM)&editStream, &lResult));

	return hr;
}