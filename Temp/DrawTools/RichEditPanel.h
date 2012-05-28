#pragma once

#include <textserv.h>
#include <tom.h>


typedef void (* PNOTIFY_CALLBACK)(int iNotify, void* pParam);

#define RICH_EDIT_PANEL_FONT_MASK_NAME		(DWORD)(0x00000001)
#define RICH_EDIT_PANEL_FONT_MASK_SIZE		(DWORD)(0x00000002)
#define RICH_EDIT_PANEL_FONT_MASK_BOLD		(DWORD)(0x00000004)
#define RICH_EDIT_PANEL_FONT_MASK_ITALIC	(DWORD)(0x00000008)
#define RICH_EDIT_PANEL_FONT_MASK_UNDERLINE	(DWORD)(0x00000010)
//#define RICH_EDIT_PANEL_FONT_MASK_WEIGHT	(DWORD)(0x00000080)

#define RICH_EDIT_PANEL_FONT_MASK_ALL		\
	RICH_EDIT_PANEL_FONT_MASK_NAME | RICH_EDIT_PANEL_FONT_MASK_SIZE|RICH_EDIT_PANEL_FONT_MASK_BOLD\
	| RICH_EDIT_PANEL_FONT_MASK_ITALIC | RICH_EDIT_PANEL_FONT_MASK_UNDERLINE
	
	

class CRichEditPanel
	: public ITextHost
	, public CMessageFilter
{
public:
	CRichEditPanel();
	~CRichEditPanel();
	HRESULT Init(HWND hWndOwner, const RECT* prcClient, PNOTIFY_CALLBACK pNotifyCallback = NULL);
	BOOL SetActive(BOOL bActive);
	BOOL IsActive();

	//dwFontMask see RICH_EDIT_PANEL_FONT_MASK_XXXX
	HRESULT SetTextFont(long nStart, long nEnd, PLOGFONT pLogFont, DWORD dwFontMask);
	HRESULT SetTextFont(long nStart, long nEnd, HFONT	hFont, DWORD dwFontMask);

	
	//if clr is COLORREF(-1), then will be tomAutoColor
	HRESULT SetTextForeColor(long nStart, long nEnd, COLORREF clr);
	HRESULT SetTextBackColor(long nStart, long nEnd, COLORREF clr);

	HRESULT GetTextStream(long nStart, long nEnd, IStream** ppStream);
	HRESULT SetTextStream(long nStart, long nEnd, IStream* pStream);

	void DoPaint(CDCHandle dcParent);

	HRESULT GetTextServices(ITextServices** ppTextServices)
	{
		*ppTextServices = m_spTextServices;
		(*ppTextServices)->AddRef();
		return S_OK;
	}
	HRESULT SetClientRect(const RECT *prc, BOOL fUpdateExtent = TRUE);
	RECT* GetClientRect() { return &m_rcClient; }

	HRESULT SetText(LPCTSTR pszText);		//TODO:change to RTF?
	HRESULT Range(long cpFirst, long cpLim, ITextRange** ppRange);
	PNOTIFY_CALLBACK SetNotifyCallback(PNOTIFY_CALLBACK pNotifyCallback);


	//property
	BOOL GetWordWrap(void);
	void SetWordWrap(BOOL fWordWrap);
	BOOL GetReadOnly();
	void SetReadOnly(BOOL fReadOnly);
	BOOL GetAllowBeep();
	void SetAllowBeep(BOOL fAllowBeep);
	RECT* GetViewInset(void);
	void SetViewInset(RECT *prc);
	WORD GetDefaultAlign();
	void SetDefaultAlign(WORD wNewAlign);
	BOOL GetRichTextFlag();
	void SetRichTextFlag(BOOL fNew);
	LONG GetDefaultLeftIndent();
	void SetDefaultLeftIndent(LONG lNewIndent);
	
	BOOL SetSaveSelection(BOOL fSaveSelection);
	SIZEL *GetExtent(void);
	void SetExtent(SIZEL *psizelExtent, BOOL fNotify);

	BOOL GetActiveState(void);
	BOOL DoSetCursor(RECT *prc, POINT *pt);
	void SetTransparent(BOOL fTransparent);
	void GetControlRect(LPRECT prc);
	LONG SetAccelPos(LONG laccelpos);
	WCHAR SetPasswordChar(WCHAR chPasswordChar);
	void SetDisabled(BOOL fOn);
	LONG SetSelBarWidth(LONG lSelBarWidth);
	BOOL GetTimerState();

	//static LONG GetXPerInch(void) { return xPerInch; }
	//static LONG	GetYPerInch(void) { return yPerInch; }

	HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
	HRESULT OnTxInPlaceDeactivate();

	BEGIN_MSG_MAP_EX(CRichEditPanel)
		//MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		//MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		//MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		//MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessageHandler)
		MESSAGE_RANGE_HANDLER(WM_KEYFIRST, WM_KEYLAST, OnKeyMessageHandler)
	END_MSG_MAP()

	//BEGIN_COM_MAP(CRichEditPanel)
	//	COM_INTERFACE_ENTRY(ITextHost)
	//END_COM_MAP()

	//IUnknown interface
	virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void ** ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef( void);
	virtual ULONG STDMETHODCALLTYPE Release( void);

	//ITextHost interface
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

	//Message Handle

	LRESULT OnMouseMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	HRESULT InitDefaultCharFormat(HFONT hfont);
	HRESULT InitDefaultParaFormat();
	
	// nStart => nEnd
	//   0    =>   0   -- Current Select
	//   0    =>  -1   -- All Text
	//   n    =>   m   -- From n To m
	HRESULT _GetTextRange(long nStart, long nEnd, CComPtr<ITextRange>& spTextRange);
	BOOL	_IsNeedHandleMsg(MSG* pMsg);
private:
	//this must be the first member variable
	ULONG					m_cRefs;
	PNOTIFY_CALLBACK		m_pNotifyCallback;

	HWND					m_hWndOwner;
	CRect					m_rcClient;
	//CRect					m_rcViewInset;
	SIZEL					m_sizelExtent;
	SIZEL					m_szlExtent;
	DWORD					m_dwStyle;
	CHARFORMAT				m_charFormat;
	PARAFORMAT				m_paraFormat;
	COLORREF 				m_crBackground;			//background color
	COLORREF				m_crAuto;
	LONG					m_xPixPerInch;
	LONG					m_yPixPerInch;
	LONG					m_lAcceleratorPos;
	LONG					m_cchTextMost;
	LONG					m_lSelBarWidth;			//width of the selection bar

	TCHAR					m_chPasswordChar;
	CComPtr<ITextServices>	m_spTextServices;
	CComPtr<ITextDocument>	m_spTextDocument;
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
	unsigned	m_fCapture				:1;	// Captured
};
