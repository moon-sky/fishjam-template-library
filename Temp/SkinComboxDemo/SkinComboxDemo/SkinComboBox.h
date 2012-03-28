#pragma once

//#include "SkinScrollWnd.h"

//#include <ftlWindow.h>

#include "SkinComboEdit.h"
//#include "SkinListBox.h"

struct COMBOBOX_SKIN_INFO
{
	COMBOEDIT_SKIN_INFO		m_EditSkinInfo;
	UINT					m_nComboBoxHeight;
	COLORREF				m_clrItemStrokeHighlight;
	COLORREF				m_clrItemBackgroundHighlight;
	COLORREF				m_clrItemBackgroundNormal;
	COLORREF				m_clrListBoxStroke;
	

	COMBOBOX_SKIN_INFO()
	{
		m_nComboBoxHeight = 200;
		m_clrItemStrokeHighlight = RGB( 0xd9, 0xe3, 0xf1 ); //#TODO
		m_clrItemBackgroundHighlight = RGB( 0xe6, 0xee, 0xf8 );  //#TODO
		m_clrItemBackgroundNormal = RGB( 0xff, 0xff, 0xff ); //#fffff
		m_clrListBoxStroke = RGB(0x64, 0x56, 0x55); //#555664
	}

	COMBOBOX_SKIN_INFO& operator=(const COMBOBOX_SKIN_INFO& other)
	{
		if (this != &other)
		{
			m_EditSkinInfo = other.m_EditSkinInfo;
			m_nComboBoxHeight = other.m_nComboBoxHeight;
			m_clrItemStrokeHighlight = other.m_clrItemStrokeHighlight;
			m_clrItemBackgroundHighlight = other.m_clrItemBackgroundHighlight;
			m_clrItemBackgroundNormal = other.m_clrItemBackgroundNormal;
			m_clrListBoxStroke = other.m_clrListBoxStroke;
		}
		return *this;
	}

	//const COLORREF COLOR_HIGHLIGHT_BG = RGB( 0xe6, 0xee, 0xf8 );
	//const COLORREF COLOR_HIGHLIGHT_BORDER = RGB( 0xd9, 0xe3, 0xf1 );
};



class CSkinComboBox : public CWindowImpl<CSkinComboBox, CComboBox>,
	public COwnerDraw<CSkinComboBox>
{
public:
	CSkinComboBox(void);
	~CSkinComboBox(void);
	BOOL Initialize(const COMBOBOX_SKIN_INFO& rSkinInfo );

	BEGIN_MSG_MAP_EX(CSkinComboBox)
		//MSG_WM_DESTROY(OnDestroy)
		//MSG_WM_GETDLGCODE(OnGetDlgCode)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		//MSG_WM_MOUSEMOVE(OnMouseMove)
		//MSG_WM_MOUSELEAVE(OnMouseLeave)
		//MSG_WM_LBUTTONDOWN(OnLButtonDown)
		//MSG_WM_LBUTTONUP(OnLButtonUp)
		//MESSAGE_HANDLER( CB_INSERTSTRING, OnInsertString )
		//MSG_OCM_CTLCOLOREDIT(OnReflectedCtlColorEdit)
		//MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		//MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSkinComboBox>, 1)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	int  CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);


	UINT OnGetDlgCode(LPMSG lpMsg);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	void OnPaint(CDCHandle /*dc*/);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//void OnMouseMove(UINT nFlags, CPoint point);
	//void OnMouseLeave();
	//void OnLButtonDown(UINT nFlags, CPoint point);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//HBRUSH OnReflectedCtlColorEdit(CDCHandle dc, CEdit edit);
 //   HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);
	//HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox);

private:
	BOOL	m_bOver;
	int		m_nFontHeigth;
	CSkinComboEdit     m_SkinEdit;
	COMBOBOX_SKIN_INFO	m_ComboBoxSkinInfo;

	//WNDPROC		m_oldListBoxWinProc;
	void DrawEdit(HDC hDC, UINT iState, const CRect& rcClient, int itemID);
	void DrawContent( HDC hDC, UINT iState,const CRect& rcClient, int itemID);
	void UpdateRect(void);
	void OnNcPaint( HDC hDC );
	static LRESULT CALLBACK ListBoxWindowProc(  HWND hwnd,   UINT uMsg,   WPARAM wParam, 
		LPARAM lParam );
};
