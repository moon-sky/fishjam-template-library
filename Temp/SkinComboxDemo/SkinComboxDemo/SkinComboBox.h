#pragma once

//#include "SkinScrollWnd.h"

#include <ftlWindow.h>

#include "SkinEdit.h"
#include "SkinListBox.h"

class CSkinComboBox : public CWindowImpl<CSkinComboBox, CComboBox>,
	public COwnerDraw<CSkinComboBox>
{
public:
	CSkinComboBox(void);
	~CSkinComboBox(void);
    BOOL Initialize();

	//void SetFont(HFONT hFont);
	BEGIN_MSG_MAP_EX(CSkinComboBox)
        if (uMsg == WM_COMMAND)
        {
            FTLTRACE(TEXT("WM_COMMAND, Code=%s\n"), FTL::CFWinUtil::GetCommandNotifyString(NULL, 
                HIWORD(wParam)));
        }
        //DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam);
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
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnMouseLeave();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	HBRUSH OnReflectedCtlColorEdit(CDCHandle dc, CEdit edit);
    HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);
	HBRUSH OnCtlColorListBox(CDCHandle dc, CListBox listBox);

private:
	BOOL	m_bOver;
	int		m_nFontHeigth;
	HFONT	m_hFont;
	//HWND	m_hListBox;
    CSkinEdit     m_SkinEdit;
    //CSkinListBox    m_SkinListBox;
	WNDPROC		m_oldListBoxWinProc;
	//int		m_nOldSelectItem;
	void DrawEdit(HDC hDC, UINT iState, const CRect& rcClient, int itemID);
	void DrawContent( HDC hDC, UINT iState,const CRect& rcClient, int itemID);
	void DrawArray(HDC hDC, const CRect& rcClient);
    void UpdateRect(void);
	void OnNcPaint( HDC hDC );
    
    //BOOL HookListBoxProc(HWND hWndListBox);
    //BOOL UnHookListBoxProc();

	static LRESULT CALLBACK ListBoxWindowProc(  HWND hwnd,   UINT uMsg,   WPARAM wParam, 
		LPARAM lParam );
};


//CSkinScrollWnd* SkinWndScroll(HWND hWnd, const SCROLLBAR_SKIN_INFO* m_pSkinInfo);
