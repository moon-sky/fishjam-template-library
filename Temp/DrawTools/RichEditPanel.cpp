#include "stdafx.h"
#include "RichEditPanel.h"
#ifdef FTL_DEBUG
#include <ftlwindow.h>
#endif 

#define LY_PER_INCH   1440

// These constants are for backward compatibility. They are the 
// sizes used for initialization and reset in RichEdit 1.0
const LONG cInitTextMax = (32 * 1024) - 1;
const LONG cResetTextMax = (64 * 1024);

EXTERN_C const IID IID_ITextServices = { // 8d33f740-cf58-11ce-a89d-00aa006cadc5
	0x8d33f740,
	0xcf58,
	0x11ce,
	{0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

EXTERN_C const IID IID_ITextHost = { /* c5bdd8d0-d26e-11ce-a89e-00aa006cadc5 */
	0xc5bdd8d0,
	0xd26e,
	0x11ce,
	{0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};


EXTERN_C const IID IID_ITextEditControl = { /* f6642620-d266-11ce-a89e-00aa006cadc5 */
	0xf6642620,
	0xd266,
	0x11ce,
	{0xa8, 0x9e, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}
};

CRichEditPanel::CRichEditPanel(HWND hWndOwner, const RECT* prcClient)
{
	FTLTRACE(TEXT("CRichEditPanel::CRichEditPanel, hWndOwner=0x%x, prcClient=[%d,%d]-[%d,%d]\n"),
		hWndOwner, prcClient->left, prcClient->top, prcClient->right, prcClient->bottom);
	ZeroMemory(&m_cRefs, sizeof(CRichEditPanel) - offsetof(CRichEditPanel, m_cRefs));

	m_hWndOwner = hWndOwner;
	m_rcClient = *prcClient;

	m_rcViewInset = m_rcClient;
	m_szlExtent.cx = m_rcClient.Width();
	m_szlExtent.cy = m_rcClient.Height();

	m_cRefs = 1;
	m_dwStyle = ES_MULTILINE;
	m_cchTextMost = cInitTextMax;
}

CRichEditPanel::~CRichEditPanel()
{
	FTLTRACE(TEXT("CRichEditPanel::~CRichEditPanel\n"));
}

HRESULT STDMETHODCALLTYPE CRichEditPanel::QueryInterface(REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObject = NULL;

	//if (IsEqualIID(riid, IID_ITextEditControl))
	//{
	//	*ppvObject = (ITextEditControl *) this;
	//	AddRef();
	//	hr = S_OK;
	//}
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
	FTLTRACE(TEXT("Before CRichEditPanel::AddRef, m_cRefs=%d\n"), m_cRefs);
	return ++m_cRefs;
}
ULONG STDMETHODCALLTYPE CRichEditPanel::Release(void)
{
	ULONG c_Refs = --m_cRefs;
	FTLTRACE(TEXT("After CRichEditPanel::Release, m_cRefs=%d\n"), m_cRefs);

	if (c_Refs == 0)
	{
		delete this;
	}
	return c_Refs;
}

BOOL CRichEditPanel::Init()
{
	BOOL bRet = FALSE;
	HRESULT hr = E_FAIL;


	COM_VERIFY(InitDefaultCharFormat(&m_charFormat, NULL));
	COM_VERIFY(InitDefaultParaFormat(&m_paraFormat));

	CComPtr<IUnknown>	spUnknown;
	COM_VERIFY(CreateTextServices(NULL, this, &spUnknown));
	if (SUCCEEDED(hr) && spUnknown)
	{
		COM_VERIFY(spUnknown->QueryInterface(IID_ITextServices, (void**)&m_spTextServices));
	}
	if (SUCCEEDED(hr))
	{
		FTLTRACE(TEXT("Before Call m_spTextServices->OnTxInPlaceActivate, rcClient=[%d,%d]-[%d,%d]\n"),
			m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
		m_spTextServices->OnTxInPlaceActivate(&m_rcClient);
		FTLTRACE(TEXT("After Call m_spTextServices->OnTxInPlaceActivate\n"));
		bRet = TRUE;
	}
	return bRet;
}

void CRichEditPanel::DoPaint(CDCHandle dcParent)
{
	HRESULT hr = E_FAIL;
	//CDCHandle dcHandle(hDC);
	//dcHandle.DrawText(TEXT("CRichEditPanel::Draw"), -1, m_rcClient, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	// Put a frame around the control so it can be seen
	FrameRect(dcParent, &m_rcClient, (HBRUSH) GetStockObject(BLACK_BRUSH));

	RECT rcClient = m_rcClient;
	RECT *prc = NULL;
	LONG lViewId = TXTVIEW_ACTIVE;

	if (!m_fInplaceActive)
	{
		//GetControlRect(&rcClient);
		prc = &rcClient;
		lViewId = TXTVIEW_INACTIVE;
	}

	// Remember wparam is actually the hdc and lparam is the update
	// rect because this message has been preprocessed by the window.
	COM_VERIFY(m_spTextServices->TxDraw(
		DVASPECT_CONTENT,  		// Draw Aspect
		/*-1*/0,						// Lindex
		NULL,					// Info for drawing optimazation
		NULL,					// target device information
		dcParent,				// Draw device HDC
		NULL, 				   	// Target device HDC
		(RECTL *) prc,			// Bounding client rectangle
		NULL, 					// Clipping rectangle for metafiles
		(RECT *) m_rcClient,	// Update rectangle
		NULL, 	   				// Call back function
		NULL,					// Call back parameter
		lViewId));				// What view of the object				

	//if(TxGetEffects() == TXTEFFECT_SUNKEN)
	//	DrawSunkenBorder(hwnd, (HDC) wparam);
}

HRESULT CRichEditPanel::SetClientRect(const RECT *prc, BOOL fUpdateExtent /* = TRUE */)
{
	FTLTRACE(TEXT("CRichEditPanel::SetClientRect, newRect=[%d,%d]-[%d,%d]\n"),
		prc->left, prc->top, prc->right, prc->bottom);
	m_rcClient = *prc;
	return S_OK;
}

HDC CRichEditPanel::TxGetDC()
{
	ATLASSERT(::IsWindow(m_hWndOwner));

	HDC hDC = ::GetDC(m_hWndOwner);
	FTLTRACE(TEXT("CRichEditPanel::TxGetDC, getDC=0x%x\n"), hDC);
	return hDC;
}

INT CRichEditPanel::TxReleaseDC( HDC hdc )
{
	ATLASSERT(::IsWindow(m_hWndOwner));

	FTLTRACE(TEXT("CRichEditPanel::TxReleaseDC hdc=0x%x\n"), hdc);
	return ReleaseDC(m_hWndOwner, hdc);
}

BOOL CRichEditPanel::TxShowScrollBar( INT fnBar, BOOL fShow )
{
	FTLTRACE(TEXT("CRichEditPanel::TxShowScrollBar\n"));

	throw std::exception("The method or operation is not implemented.");
}

BOOL CRichEditPanel::TxEnableScrollBar( INT fuSBFlags, INT fuArrowflags )
{
	FTLTRACE(TEXT("CRichEditPanel::TxEnableScrollBar\n"));
	throw std::exception("The method or operation is not implemented.");
}

BOOL CRichEditPanel::TxSetScrollRange( INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetScrollRange\n"));
	throw std::exception("The method or operation is not implemented.");
}

BOOL CRichEditPanel::TxSetScrollPos( INT fnBar, INT nPos, BOOL fRedraw )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetScrollPos\n"));

	throw std::exception("The method or operation is not implemented.");
}

void CRichEditPanel::TxInvalidateRect( LPCRECT prc, BOOL fMode )
{
	FTLTRACE(TEXT("CRichEditPanel::TxInvalidateRect\n"));

	::InvalidateRect(m_hWndOwner, prc, fMode);
}

void CRichEditPanel::TxViewChange( BOOL fUpdate )
{
	FTLTRACE(TEXT("CRichEditPanel::TxViewChange\n"));
	::UpdateWindow (m_hWndOwner);
}

BOOL CRichEditPanel::TxCreateCaret( HBITMAP hbmp, INT xWidth, INT yHeight )
{
	FTLTRACE(TEXT("CRichEditPanel::TxCreateCaret\n"));
	return ::CreateCaret (m_hWndOwner, hbmp, xWidth, yHeight);
}

BOOL CRichEditPanel::TxShowCaret( BOOL fShow )
{
	FTLTRACE(TEXT("CRichEditPanel::TxShowCaret, fShow=%d\n"), fShow);

	BOOL bRet = FALSE;
	if (fShow)
	{
		bRet = ::ShowCaret(m_hWndOwner);
	}
	else
	{
		bRet = HideCaret(m_hWndOwner);
	}
	return bRet;	
}

BOOL CRichEditPanel::TxSetCaretPos( INT x, INT y )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetCaretPos, x=%d, y=%d\n"), x, y);
	return ::SetCaretPos(x, y);
}

BOOL CRichEditPanel::TxSetTimer( UINT idTimer, UINT uTimeout )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetTimer, idTimer=%d, uTimeOut=%d\n"), idTimer, uTimeout);
	m_fTimer = TRUE;
	return ::SetTimer(m_hWndOwner, idTimer, uTimeout, NULL);
}

void CRichEditPanel::TxKillTimer( UINT idTimer )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetTimer, idTimer=%d\n"), idTimer);

	::KillTimer(m_hWndOwner, idTimer);
	m_fTimer = FALSE;
}

void CRichEditPanel::TxScrollWindowEx( INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll )
{
	FTLTRACE(TEXT("CRichEditPanel::TxScrollWindowEx\n"));

	throw std::exception("The method or operation is not implemented.");
}

void CRichEditPanel::TxSetCapture( BOOL fCapture )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetCapture, fCapture=%d\n"), fCapture);
	if (fCapture)
	{
		::SetCapture(m_hWndOwner);
	}
	else
	{
		::ReleaseCapture();
	}
}

void CRichEditPanel::TxSetFocus()
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetFocus\n"));

	::SetFocus(m_hWndOwner);
}

void CRichEditPanel::TxSetCursor( HCURSOR hcur, BOOL fText )
{
	FTLTRACE(TEXT("CRichEditPanel::TxSetCursor\n"));
	::SetCursor(hcur);	
}

BOOL CRichEditPanel::TxScreenToClient( LPPOINT lppt )
{
	FTLTRACE(TEXT("CRichEditPanel::TxScreenToClient, lppt=[%d,%d]\n"), lppt->x, lppt->y);

	BOOL bRet = FALSE;
	bRet = ::ScreenToClient(m_hWndOwner, lppt);
	return bRet;
}

BOOL CRichEditPanel::TxClientToScreen( LPPOINT lppt )
{
	FTLTRACE(TEXT("CRichEditPanel::TxClientToScreen, lppt=[%d,%d]\n"), lppt->x, lppt->y);

	return ClientToScreen(m_hWndOwner, lppt);
}

HRESULT CRichEditPanel::TxActivate( LONG * plOldState )
{
	FTLTRACE(TEXT("CRichEditPanel::TxActivate"));
	*plOldState = m_lState;
	return S_OK;
}

HRESULT CRichEditPanel::TxDeactivate( LONG lNewState )
{
	FTLTRACE(TEXT("CRichEditPanel::TxDeactivate, lNewState=%d\n"), lNewState);
	return S_OK;
}

HRESULT CRichEditPanel::TxGetClientRect( LPRECT prc )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetClientRect\n"));
	*prc = m_rcClient;
	return S_OK;
}

HRESULT CRichEditPanel::TxGetViewInset( LPRECT prc )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetViewInset, m_rcViewInset=[%d,%d]-[%d,%d]\n"),
		m_rcViewInset.left, m_rcViewInset.top, m_rcViewInset.right, m_rcViewInset.bottom);
	*prc = m_rcViewInset;
	return S_OK;
}

HRESULT CRichEditPanel::TxGetCharFormat( const CHARFORMATW **ppCF )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetCharFormat,szFaceName=%s, crTextColor=0x%x, yHeight=%d\n"),
		m_charFormat.szFaceName, m_charFormat.crTextColor, m_charFormat.yHeight);
	*ppCF = &m_charFormat;
	return S_OK;
}

HRESULT CRichEditPanel::TxGetParaFormat( const PARAFORMAT **ppPF )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetParaFormat, dxStartIndent=%d, dxRightIndent=%d, cTabCount=%d\n"),
		m_paraFormat.dxStartIndent, m_paraFormat.dxRightIndent, m_paraFormat.cTabCount);
	*ppPF = &m_paraFormat;
	return S_OK;
}

COLORREF CRichEditPanel::TxGetSysColor( int nIndex )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetSysColor, nIndex=%d\n"), nIndex);
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
	FTLTRACE(TEXT("CRichEditPanel::TxGetBackStyle, m_fTransparent=%d\n"), m_fTransparent);
	*pstyle = !m_fTransparent ? TXTBACK_OPAQUE : TXTBACK_TRANSPARENT;
	return S_OK;
}

HRESULT CRichEditPanel::TxGetMaxLength( DWORD *plength )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetMaxLength, m_cchTextMost=%d\n"), m_cchTextMost);

	*plength = m_cchTextMost;
	return S_OK;

}

HRESULT CRichEditPanel::TxGetScrollBars( DWORD *pdwScrollBar )
{
	*pdwScrollBar =  m_dwStyle & (WS_VSCROLL | WS_HSCROLL | ES_AUTOVSCROLL | 
		ES_AUTOHSCROLL | ES_DISABLENOSCROLL);

	FTLTRACE(TEXT("CRichEditPanel::TxGetScrollBars, *pdwScrollBar=0x%x\n"), *pdwScrollBar);

	return S_OK;
}

HRESULT CRichEditPanel::TxGetPasswordChar( TCHAR *pch )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetPasswordChar\n"));
	*pch = m_chPasswordChar;
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::TxGetAcceleratorPos( LONG *pcp )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetAcceleratorPos\n"));
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::TxGetExtent( LPSIZEL lpExtent )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetExtent, m_szlExtent={%d,%d}\n"), 
		m_szlExtent.cx, m_szlExtent.cy);
	*lpExtent = m_szlExtent;
	return S_OK;
}

HRESULT CRichEditPanel::OnTxCharFormatChange( const CHARFORMATW * pcf )
{
	FTLTRACE(TEXT("CRichEditPanel::OnTxCharFormatChange\n"));
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::OnTxParaFormatChange( const PARAFORMAT * ppf )
{
	FTLTRACE(TEXT("CRichEditPanel::OnTxParaFormatChange\n"));
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::TxGetPropertyBits( DWORD dwMask, DWORD *pdwBits )
{
	FTLTRACE(TEXT("CRichEditPanel::TxGetPropertyBits, dwMask=0x%x\n"));
	DWORD dwProperties = 0;

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

	*pdwBits = dwProperties & dwMask; 
	return NOERROR;
	
	return E_NOTIMPL;
}

HRESULT CRichEditPanel::TxNotify( DWORD iNotify, void *pv )
{
	FTLTRACE(TEXT("CRichEditPanel::TxNotify\n"));
	return E_NOTIMPL;
}

HIMC CRichEditPanel::TxImmGetContext()
{
	FTLTRACE(TEXT("CRichEditPanel::TxImmGetContext\n"));
	return NULL;
}

void CRichEditPanel::TxImmReleaseContext( HIMC himc )
{
	FTLTRACE(TEXT("CRichEditPanel::TxImmReleaseContext\n"));
}

HRESULT CRichEditPanel::TxGetSelectionBarWidth( LONG *lSelBarWidth )
{
	*lSelBarWidth = m_lSelBarWidth;

	FTLTRACE(TEXT("CRichEditPanel::TxGetSelectionBarWidth, m_lSelBarWidth=%d\n"), m_lSelBarWidth);
	return E_NOTIMPL;
}

BOOL CRichEditPanel::PreTranslateMessage(MSG* pMsg)
{
#ifdef FTL_DEBUG
	DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, pMsg->message, pMsg->wParam, pMsg->lParam);
#endif 
	LONG lResult = 0;
	BOOL bRet = ProcessWindowMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam, lResult, 0);
	return bRet;
}

HRESULT CRichEditPanel::InitDefaultCharFormat(CHARFORMATW * pcf, HFONT hfont) 
{
	FTLTRACE(TEXT("CRichEditPanel::InitDefaultCharFormat\n"));

	HWND hwnd;
	LOGFONT lf;
	HDC hdc;
	LONG yPixPerInch;

	// Get LOGFONT for default font
	if (!hfont)
		hfont = (HFONT)GetStockObject(SYSTEM_FONT);

	// Get LOGFONT for passed hfont
	if (!GetObject(hfont, sizeof(LOGFONT), &lf))
		return E_FAIL;

	// Set CHARFORMAT structure
	pcf->cbSize = sizeof(CHARFORMAT2);

	hwnd = GetDesktopWindow();
	hdc = GetDC(hwnd);
	yPixPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
	pcf->yHeight = lf.lfHeight * LY_PER_INCH / yPixPerInch;
	ReleaseDC(hwnd, hdc);

	pcf->yOffset = 0;
	pcf->crTextColor = m_crAuto;

	pcf->dwEffects = CFM_EFFECTS | CFE_AUTOBACKCOLOR;
	pcf->dwEffects &= ~(CFE_PROTECTED | CFE_LINK);

	if(lf.lfWeight < FW_BOLD)
		pcf->dwEffects &= ~CFE_BOLD;
	if(!lf.lfItalic)
		pcf->dwEffects &= ~CFE_ITALIC;
	if(!lf.lfUnderline)
		pcf->dwEffects &= ~CFE_UNDERLINE;
	if(!lf.lfStrikeOut)
		pcf->dwEffects &= ~CFE_STRIKEOUT;

	pcf->dwMask = CFM_ALL | CFM_BACKCOLOR;
	pcf->bCharSet = lf.lfCharSet;
	pcf->bPitchAndFamily = lf.lfPitchAndFamily;
#ifdef UNICODE
	_tcscpy(pcf->szFaceName, lf.lfFaceName);
#else
	//need to thunk pcf->szFaceName to a standard char string.in this case it's easy because our thunk is also our copy
	MultiByteToWideChar(CP_ACP, 0, lf.lfFaceName, LF_FACESIZE, pcf->szFaceName, LF_FACESIZE) ;
#endif

	return S_OK;
}

HRESULT CRichEditPanel::InitDefaultParaFormat(PARAFORMAT * ppf) 
{	
	memset(ppf, 0, sizeof(PARAFORMAT));

	ppf->cbSize = sizeof(PARAFORMAT);
	ppf->dwMask = PFM_ALL;
	ppf->wAlignment = PFA_LEFT;
	ppf->cTabCount = 1;
	ppf->rgxTabs[0] = lDefaultTab;

	return S_OK;
}