#pragma once

#include <textserv.h>
#include <tom.h>

#pragma comment(lib, "riched20.lib")

class CRichEditPanel
	: public ITextHost
	, public CMessageFilter
{
public:
	CRichEditPanel(HWND hWndOwner, const RECT* prcClient);
	~CRichEditPanel();
	BOOL Init();
	void DoPaint(CDCHandle dcParent);

	HRESULT GetTextServices(ITextServices** pTextServices);
	HRESULT SetClientRect(const RECT *prc, BOOL fUpdateExtent = TRUE);
	RECT* GetClientRect();

	BEGIN_MSG_MAP_EX(CRichEditPanel)
		MSG_WM_CHAR(OnChar)
		{
			HRESULT hr = m_spTextServices->TxSendMessage(uMsg, wParam, lParam, &lResult);
			if (hr == S_FALSE)
			{
				lResult = ::DefWindowProc(m_hWndOwner, uMsg, wParam, lParam);
			}
		}
	END_MSG_MAP()

	//BEGIN_COM_MAP(CRichEditPanel)
	//	COM_INTERFACE_ENTRY(ITextHost)
	//	COM_INTERFACE_ENTRY(ITextEditControl)
	//END_COM_MAP()

	//IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void ** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef( void);
	virtual ULONG STDMETHODCALLTYPE Release( void);

	//ITextHost
	virtual HDC TxGetDC();
	virtual INT TxReleaseDC( HDC hdc );
	virtual BOOL TxShowScrollBar( INT fnBar, BOOL fShow );
	virtual BOOL TxEnableScrollBar( INT fuSBFlags, INT fuArrowflags );
	virtual BOOL TxSetScrollRange( INT fnBar, LONG nMinPos, INT nMaxPos, BOOL fRedraw );
	virtual BOOL TxSetScrollPos( INT fnBar, INT nPos, BOOL fRedraw );
	virtual void TxInvalidateRect( LPCRECT prc, BOOL fMode );
	virtual void TxViewChange( BOOL fUpdate );
	virtual BOOL TxCreateCaret( HBITMAP hbmp, INT xWidth, INT yHeight );
	virtual BOOL TxShowCaret( BOOL fShow );
	virtual BOOL TxSetCaretPos( INT x, INT y );
	virtual BOOL TxSetTimer( UINT idTimer, UINT uTimeout );
	virtual void TxKillTimer( UINT idTimer );
	virtual void TxScrollWindowEx( INT dx, INT dy, LPCRECT lprcScroll, LPCRECT lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate, UINT fuScroll );
	virtual void TxSetCapture( BOOL fCapture );
	virtual void TxSetFocus();
	virtual void TxSetCursor( HCURSOR hcur, BOOL fText );
	virtual BOOL TxScreenToClient( LPPOINT lppt );
	virtual BOOL TxClientToScreen( LPPOINT lppt );
	virtual HRESULT TxActivate( LONG * plOldState );
	virtual HRESULT TxDeactivate( LONG lNewState );
	virtual HRESULT TxGetClientRect( LPRECT prc );
	virtual HRESULT TxGetViewInset( LPRECT prc );
	virtual HRESULT TxGetCharFormat( const CHARFORMATW **ppCF );
	virtual HRESULT TxGetParaFormat( const PARAFORMAT **ppPF );
	virtual COLORREF TxGetSysColor( int nIndex );
	virtual HRESULT TxGetBackStyle( TXTBACKSTYLE *pstyle );
	virtual HRESULT TxGetMaxLength( DWORD *plength );
	virtual HRESULT TxGetScrollBars( DWORD *pdwScrollBar );
	virtual HRESULT TxGetPasswordChar( TCHAR *pch );
	virtual HRESULT TxGetAcceleratorPos( LONG *pcp );
	virtual HRESULT TxGetExtent( LPSIZEL lpExtent );
	virtual HRESULT OnTxCharFormatChange( const CHARFORMATW * pcf );
	virtual HRESULT OnTxParaFormatChange( const PARAFORMAT * ppf );
	virtual HRESULT TxGetPropertyBits( DWORD dwMask, DWORD *pdwBits );
	virtual HRESULT TxNotify( DWORD iNotify, void *pv );
	virtual HIMC TxImmGetContext();
	virtual void TxImmReleaseContext( HIMC himc );
	virtual HRESULT TxGetSelectionBarWidth( LONG *lSelBarWidth );

	//CMessageFilter
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	ULONG					m_cRefs;
	HWND					m_hWndOwner;
	CRect					m_rcClient;
	CRect					m_rcViewInset;
	SIZEL					m_szlExtent;
	DWORD					m_dwStyle;
	CHARFORMAT				m_charFormat;
	PARAFORMAT				m_paraFormat;
	COLORREF 				m_crBackground;			// background color
	COLORREF				m_crAuto;

	LONG					m_cchTextMost;
	LONG					m_lSelBarWidth;

	TCHAR					m_chPasswordChar;
	CComPtr<ITextServices>	m_spTextServices;

	LONG					m_lState;

	unsigned	m_fBorder				:1;	// control has border
	unsigned	m_fCustRect				:1;	// client changed format rect
	unsigned	m_fInBottomless			:1;	// inside bottomless callback
	unsigned	m_fInDialogBox			:1;	// control is in a dialog box
	unsigned	m_fEnableAutoWordSel	:1;	// enable Word style auto word selection?
	unsigned	m_fVertical				:1;	// vertical writing
	unsigned	m_fIconic				:1;	// control window is iconic
	unsigned	m_fHidden				:1;	// control window is hidden
	unsigned	m_fNotSysBkgnd			:1;	// not using system background color
	unsigned	m_fWindowLocked			:1;	// window is locked (no update)
	unsigned	m_fRegisteredForDrop	:1; // whether host has registered for drop
	unsigned	m_fVisible				:1;	// Whether window is visible or not.
	unsigned	m_fResized				:1;	// resized while hidden
	unsigned	m_fWordWrap				:1;	// Whether control should word wrap
	unsigned	m_fAllowBeep			:1;	// Whether beep is allowed
	unsigned	m_fRich					:1;	// Whether control is rich text
	unsigned	m_fSaveSelection		:1;	// Whether to save the selection when inactive
	unsigned	m_fInplaceActive		:1; // Whether control is inplace active
	unsigned	m_fTransparent			:1; // Whether control is transparent
	unsigned	m_fTimer				:1;	// A timer is set
private:
	HRESULT InitDefaultCharFormat(CHARFORMATW * pcf, HFONT hfont);
	HRESULT InitDefaultParaFormat(PARAFORMAT * ppf);

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		FTLTRACE(TEXT("RichEditPanel::OnChar, nChar=%d, nRepCnt=%d, nFlags=%d\n"), nChar, nRepCnt, nFlags);
		SetMsgHandled(FALSE);
	}
};
