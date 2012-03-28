#pragma once

//#include <ftlWindow.h>

struct COMBOEDIT_SKIN_INFO
{
	CImage*		m_pImgMenuDown;

	COLORREF	m_clrBackgroundNormal;
	//COLORREF	m_clrBackgroundFocus;

	COLORREF	m_clrStrokeNormal;
	COLORREF	m_clrStrokeFocus;
	COLORREF	m_clrStrokeDisabled;

	COLORREF	m_clrFontNormal;
	COLORREF	m_clrFontDisabled;

	HFONT		m_hFontNormal;
	HFONT		m_hFontDisabled;

	COMBOEDIT_SKIN_INFO()
	{
		m_pImgMenuDown = NULL;

		m_clrBackgroundNormal =	RGB(0xff, 0xff, 0xff);	//#ffffff
		//m_clrBackgroundFocus = 
		m_clrStrokeNormal = RGB(0xca, 0xc4, 0xbd);	//#bdc4ca
		m_clrStrokeFocus = RGB(0xce, 0x86, 0x3f);	//#3f86ce
		m_clrStrokeDisabled = RGB(0xe4, 0xe0, 0xdb);//#dbe0e4

		m_clrFontNormal = RGB(0x40, 0x3a, 0x35);	//#353a40
		m_clrFontDisabled = RGB(0x40, 0x3a, 0x35);	//#353a40

		m_hFontNormal = NULL;
		m_hFontDisabled = NULL;
	}

	COMBOEDIT_SKIN_INFO &operator = (const COMBOEDIT_SKIN_INFO &other)
	{
		if (this != &other)
		{
			m_pImgMenuDown = other.m_pImgMenuDown;
			m_clrBackgroundNormal = other.m_clrBackgroundNormal;

			m_clrStrokeNormal = other.m_clrStrokeNormal;
			m_clrStrokeFocus = other.m_clrStrokeFocus;
			m_clrStrokeDisabled = other.m_clrStrokeDisabled;

			m_clrFontNormal = other.m_clrFontNormal;
			m_clrFontDisabled = other.m_clrFontDisabled;
			m_hFontNormal = other.m_hFontNormal;
			m_hFontDisabled = other.m_hFontDisabled;
		}

		FTLASSERT(m_pImgMenuDown);
		FTLASSERT(m_hFontNormal);
		FTLASSERT(m_hFontDisabled);
		return *this;
	}

};


class CSkinComboEdit : public CWindowImpl<CSkinComboEdit, CEdit>//,
	//public COwnerDraw<CSkinButton>
{
public:
	CSkinComboEdit(void);
	~CSkinComboEdit(void);
	BOOL Initialize(const COMBOEDIT_SKIN_INFO& rSkinInfo , HWND hParentCombobox);
	
	BEGIN_MSG_MAP_EX(CSkinComboEdit)
		//DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam);

		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		//MSG_WM_LBUTTONDOWN(OnLButtonDown)
		//MSG_WM_LBUTTONUP(OnLButtonUp)
		//MESSAGE_HANDLER( CB_INSERTSTRING, OnInsertString )
		//MSG_OCM_CTLCOLOREDIT(OnReflectedCtlColorEdit)
		//MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)

		//CHAIN_MSG_MAP_ALT(COwnerDraw<CSkinButton>, 1)
		//DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
public:
	void DoPaint();
	void OnPaint(CDCHandle /*dc*/);
	BOOL OnEraseBkgnd(CDCHandle dc);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnMouseLeave();
private:
	BOOL		m_bOver;
	BOOL		m_bOverOnArrow;
	HWND		m_hParentCombobox;
	COMBOEDIT_SKIN_INFO		m_skinInfo;
	NDGraphics::CGDIPImageList		m_imgMenuDown;
	int			m_nArrowWidth;
	int			m_nArrowHeight;
	BOOL _IsMouseInMenuDownRect(const POINT& pt);
};
