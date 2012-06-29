#pragma once

#include <textserv.h>
#include <tom.h>
#include <stack>

#include "DrawTypeDefine.h"
class INotifyCallBack
{
public:
	virtual void OnNotify(int iNotify, void* pParam) = 0;
	//virtual void OnExpand(int nDir, int nValue) = 0;
};

#define RTPANEL_MARGIN_LEFT			3
#define RTPANEL_MARGIN_TOP			3
#define RTPANEL_MARGIN_RIGHT		3
#define RTPANEL_MARGIN_BOTTOM		3
#define RTPANEL_MIN_ROW_COUNT		2
#define RTPANEL_MIN_COL_COUNT		5

#define RICH_EDIT_PANEL_FONT_MASK_NAME		(DWORD)(0x00000001)
#define RICH_EDIT_PANEL_FONT_MASK_SIZE		(DWORD)(0x00000002)
#define RICH_EDIT_PANEL_FONT_MASK_BOLD		(DWORD)(0x00000004)
#define RICH_EDIT_PANEL_FONT_MASK_ITALIC	(DWORD)(0x00000008)
#define RICH_EDIT_PANEL_FONT_MASK_UNDERLINE	(DWORD)(0x00000010)
//#define RICH_EDIT_PANEL_FONT_MASK_WEIGHT	(DWORD)(0x00000080)

#define RICH_EDIT_PANEL_FONT_MASK_ALL		\
	RICH_EDIT_PANEL_FONT_MASK_NAME | RICH_EDIT_PANEL_FONT_MASK_SIZE|RICH_EDIT_PANEL_FONT_MASK_BOLD\
	| RICH_EDIT_PANEL_FONT_MASK_ITALIC | RICH_EDIT_PANEL_FONT_MASK_UNDERLINE
	

struct UNDO_REDO_INFO
{
	UNDO_REDO_INFO()
	{
		spStream = NULL;
		nStart = 0;
		nEnd = -1; 
		nSelectStart = 0;
		nSelectEnd = 0;
	}
	UNDO_REDO_INFO(CComPtr<IStream>& spStream, long nSelectStart, long nSelectEnd)
	{
		//default is select all
		this->spStream = spStream;
		nStart = 0;
		nEnd = -1; 
		this->nSelectStart = nSelectStart;
		this->nSelectEnd = nSelectEnd;
	}
	UNDO_REDO_INFO&	operator = (const UNDO_REDO_INFO& r)
	{
		if (this != &r)
		{
			this->spStream = r.spStream;
			this->nStart = r.nStart;
			this->nEnd = r.nEnd;
			//this->nPos = r.nPos;
			this->nSelectStart = r.nSelectStart;
			this->nSelectEnd = r.nSelectEnd;
		}
		return *this;
	}

	void Reset()
	{
		spStream.Release();
	}

	BOOL IsValid()
	{
		return (!!spStream);
	}
public:
	CComPtr<IStream>	spStream;
	//CAtlStringA			strRTF;
	long				nStart;
	long				nEnd;
	long				nSelectStart;
	long				nSelectEnd;
	//long				nPos;
};

typedef std::list<UNDO_REDO_INFO>	UNDO_REDO_COLLECTION;

class CRichEditPanel
	: public ITextHost
	//, public IRichEditOleCallback
	//, public CMessageFilter
{
public:
	CRichEditPanel();
	virtual ~CRichEditPanel();
	HRESULT Init(HWND hWndOwner, const RECT* prcBound, IDrawCanvas* pDrawCanvas,
		const LOGFONT* pLogFont, COLORREF clrFontFore, INotifyCallBack* pNotifyCallback = NULL);
	BOOL SetActive(BOOL bActive);
	BOOL IsActive();

	BOOL CanUndo();
	BOOL CanRedo();
	BOOL Undo();
	BOOL Redo();
	BOOL ClearUndoRedoInfo(BOOL bUndo, BOOL bRedo);

	//dwFontMask see RICH_EDIT_PANEL_FONT_MASK_XXXX
	
	//HRESULT SetTextFont(long nStart, long nEnd, PLOGFONT pLogFont, DWORD dwFontMask);
	//HRESULT SetTextFont(long nStart, long nEnd, HFONT	hFont, DWORD dwFontMask);

	HRESULT BeginEdit();
	HRESULT EndEdit(BOOL bPushUndo = TRUE);

	HRESULT SetTextFontName(long nStart, long nEnd, LPCTSTR pszFontName, BOOL bPreview = FALSE);
	HRESULT GetTextFontName(long nStart, long nEnd, LPTSTR pszFontName, int nCount);
	HRESULT SetTextFontSize(long nStart, long nEnd, int nSize, BOOL bPreview = FALSE);
	HRESULT GetTextFontSize(long nStart, long nEnd, int* pFontSize);

	//if clr is COLORREF(-1), then will be tomAutoColor
	HRESULT SetTextForeColor(long nStart, long nEnd, COLORREF clr, BOOL bPreview = FALSE);
	HRESULT GetTextForeColor(long nStart, long nEnd, COLORREF* pClr);

	HRESULT SetTextBackColor(long nStart, long nEnd, COLORREF clr);
	HRESULT GetTextBackColor(long nStart, long nEnd, COLORREF* pClr);

	HRESULT GetTextForeColor(long nStart, long nEnd, COLORREF& clr);

	//nStart and nEnd does not use now, must be zero
	HRESULT GetTextStream(long nStart, long nEnd, IStream** ppStream);
	HRESULT SetTextStream(long nStart, long nEnd, IStream* pStream);

	void DoPaint(CDCHandle dcParent);
	
	HRESULT GetTextServices(ITextServices** ppTextServices)
	{
		*ppTextServices = m_spTextServices;
		(*ppTextServices)->AddRef();
		return S_OK;
	}

	
	HRESULT SetClientBound(const RECT *pRcBound, RECT* pNewRcBound = NULL, BOOL fUpdateExtent = TRUE);
	SIZE	GetMinBoundSize(int nRow, int nCol);
	//CRect   CheckClientBoundRect();

	//RECT* GetClientRect() { return &m_rcClient; }

	HRESULT SetText(LPCTSTR pszText);		//TODO:change to RTF?
	HRESULT Range(long cpFirst, long cpLim, ITextRange** ppRange);
	VOID SetNotifyCallback(INotifyCallBack* pNotifyCallback);
	BOOL HandleControlMessage(IDrawCanvas* pView, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	//property
	//BOOL GetWordWrap(void);
	//void SetWordWrap(BOOL fWordWrap);
	//BOOL GetReadOnly();
	//void SetReadOnly(BOOL fReadOnly);
	//BOOL GetAllowBeep();
	//void SetAllowBeep(BOOL fAllowBeep);
	//RECT* GetViewInset(void);
	//void SetViewInset(RECT *prc);
	//WORD GetDefaultAlign();
	//void SetDefaultAlign(WORD wNewAlign);
	//BOOL GetRichTextFlag();
	//void SetRichTextFlag(BOOL fNew);
	//LONG GetDefaultLeftIndent();
	//void SetDefaultLeftIndent(LONG lNewIndent);
	//
	//BOOL SetSaveSelection(BOOL fSaveSelection);
	SIZEL *GetExtent(void);
	void SetExtent(SIZEL *psizelExtent, BOOL fNotify);
	void SetZoom(UINT Numerator, UINT Denominator);
	//BOOL GetActiveState(void);
	//BOOL DoSetCursor(RECT *prc, POINT *pt);
	//void SetTransparent(BOOL fTransparent);
	//void GetControlRect(LPRECT prc);
	//LONG SetAccelPos(LONG laccelpos);
	//WCHAR SetPasswordChar(WCHAR chPasswordChar);
	//void SetDisabled(BOOL fOn);
	//LONG SetSelBarWidth(LONG lSelBarWidth);
	//BOOL GetTimerState();

	//static LONG GetXPerInch(void) { return xPerInch; }
	//static LONG	GetYPerInch(void) { return yPerInch; }

	//HRESULT OnTxInPlaceActivate(LPCRECT prcClient);
	//HRESULT OnTxInPlaceDeactivate();

	BEGIN_MSG_MAP_EX(CRichEditPanel)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessageHandler)
		MESSAGE_RANGE_HANDLER(WM_KEYFIRST, WM_KEYLAST, OnKeyMessageHandler)
		//MESSAGE_RANGE_HANDLER(WM_IME_SETCONTEXT, WM_IME_KEYUP, OnIMEMessageHandler)
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
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	//Message Handle
	LRESULT OnMouseMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnIMEMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// nStart => nEnd
	//   0    =>   0   -- Current Select
	//   0    =>  -1   -- All Text
	//   n    =>   m   -- From n To m
	HRESULT GetTextRange(long nStart, long nEnd, CComPtr<ITextRange>& spTextRange);

	void    InitEmptyUndoInfo();

	//LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	HRESULT InitDefaultCharFormat(const LOGFONT* pLogFont, COLORREF clrTextFore);
	HRESULT InitDefaultParaFormat();

	BOOL	_IsNeedHandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL	_CheckAndPushRedoInfo();
	BOOL	_IsDocumentChanged();	
	HRESULT _SetPropertyBits(DWORD dwProperty);
	CAtlString _GetStreamText(IStream* pStream);
	HRESULT    _GetCurrentEditInfo(UNDO_REDO_INFO& stEditInfo);

private:
	UNDO_REDO_COLLECTION	m_undoCounts;
	UNDO_REDO_COLLECTION	m_redoCounts;
	CComPtr<ITextServices>	m_spTextServices;
	CComPtr<ITextDocument>	m_spTextDocument;
	CAtlString				m_strOldText;
	UNDO_REDO_INFO          m_stBeginEditInfo;
	UNDO_REDO_INFO          m_stCurrentEditInfo;
	UNDO_REDO_INFO          m_stEmptyEditInfo;
	//this must be the first member variable
	ULONG					m_cRefs;
	INotifyCallBack*		m_pNotifyCallback;
	
	HWND					m_hWndOwner;
	CRect					m_rcClient;
	CRect					m_rcBound;
	IDrawCanvas*			m_pDrawCanvas;
	//CRect					m_rcViewInset;
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
	LONG					m_lState;
	//UINT					m_nUndoRedoCount;
	UINT					m_maxUndoRedoCount;
	BOOL					m_bFirstInput;

	unsigned    m_fNotify				:1;
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
	unsigned	m_fAutoVExpand			:1; // Will Auto Expand
};
