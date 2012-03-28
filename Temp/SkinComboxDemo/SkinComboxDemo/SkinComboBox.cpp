#include "StdAfx.h"
//#include "SkinScrollBar.h"
#include "SkinComboBox.h"
#include <atlimage.h>
#include <ftlgdi.h>



const int ITEM_MARGIN_TOP = 4; //6 - textMetrics.tmInternalLeading 
const int ITEM_MARGIN_BOTTOM = 4; //6 - textMetrics.tmInternalLeading
const int ITEM_MARGIN_LEFT = 8;

const int FOCUS_MARGIN_TOP = 3;
const int FOCUS_MARGIN_BOTTON = 3;
const int FOCUS_MARGIN_LEFT = 3;
const int FOCUS_MARGIN_RIGHT = 2;

//const COLORREF COLOR_STOKE_FOCUS = RGB(0x3f, 0x86, 0xce);
//const COLORREF COLOR_ITEM_NORMAL_BACK = RGB(255,255,255);

//const COLORREF COLOR_ITEM_FOUCS_BACK = RGB(230,239,248);  //TODO
//const COLORREF COLOR_EDIT_BACKGROUND = RGB(0,255,0);

CSkinComboBox::CSkinComboBox()
{
	m_nFontHeigth = -1;
	//m_nInternalLeading = -1;
	m_bOver = FALSE;
	//m_oldListBoxWinProc = NULL;
}

CSkinComboBox::~CSkinComboBox()
{

}

void CSkinComboBox::DrawEdit(HDC hDC, UINT iState,const CRect& rcClient, int itemID)
{
	if (m_SkinEdit.m_hWnd && ::IsWindow(m_SkinEdit.m_hWnd))
	{
		m_SkinEdit.DoPaint();
	}
}

BOOL CSkinComboBox::Initialize( const COMBOBOX_SKIN_INFO& rSkinInfo )
{
	BOOL bRet = TRUE;
	//FTLASSERT(!m_SkinEdit.m_hWnd);		//just call once

	if (!m_SkinEdit.m_hWnd)
	{
		CRect rcCombobox;
		GetWindowRect(rcCombobox);
		GetParent().ScreenToClient(rcCombobox);
		rcCombobox.bottom = rcCombobox.bottom + rSkinInfo.m_nComboBoxHeight;
		API_VERIFY(SetWindowPos(NULL, rcCombobox.left, rcCombobox.top, rcCombobox.Width(), rcCombobox.Height(),
			SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOMOVE));

		m_ComboBoxSkinInfo = rSkinInfo;

		//Get Item Font Height
		CClientDC dc(m_hWnd);
		HFONT hOldFont = NULL;

		hOldFont = dc.SelectFont(rSkinInfo.m_EditSkinInfo.m_hFontNormal);
		TEXTMETRIC textMetrics = { 0 };
		dc.GetTextMetrics(&textMetrics);
		m_nFontHeigth = textMetrics.tmHeight; // - 2 * textMetrics.tmInternalLeading;

		dc.SelectFont(hOldFont);

		BOOL bRet = FALSE;
		COMBOBOXINFO cmbInfo = { sizeof(cmbInfo) };
		API_VERIFY(GetComboBoxInfo(&cmbInfo));
		FTLASSERT(cmbInfo.hwndItem != NULL);

		API_VERIFY(m_SkinEdit.Initialize(rSkinInfo.m_EditSkinInfo, m_hWnd));
		API_VERIFY(m_SkinEdit.SubclassWindow(cmbInfo.hwndItem));
	}
	return bRet;
}

void CSkinComboBox::DrawContent(HDC hDC, UINT iState,const CRect& rcClient, int itemID)
{
	CDCHandle dc(hDC);
	dc.SetBkMode( TRANSPARENT );

	CString strText;
	GetLBText(itemID, strText);
	CRect rcText = rcClient;
	rcText.top +=  ITEM_MARGIN_TOP;
	rcText.left += ITEM_MARGIN_LEFT;
	
	CRect rcFocus = rcClient;

	if (iState & (ODS_SELECTED|ODS_FOCUS))
	{
		rcFocus.DeflateRect(FOCUS_MARGIN_LEFT, FOCUS_MARGIN_TOP,FOCUS_MARGIN_RIGHT, FOCUS_MARGIN_BOTTON);
		dc.FillSolidRect(rcFocus, m_ComboBoxSkinInfo.m_clrItemBackgroundHighlight );
	}
	else
	{
		dc.FillSolidRect(rcFocus, m_ComboBoxSkinInfo.m_clrItemBackgroundNormal) ;// COLOR_ITEM_NORMAL_BACK);
	}
	dc.DrawText( strText,-1, &rcText, DT_TOP | DT_LEFT);
}

void CSkinComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	UINT itemID = lpDrawItemStruct->itemID;
	CRect rcItem = lpDrawItemStruct->rcItem;
	UINT  iState = lpDrawItemStruct->itemState;

	//FTL::CFStringFormater formaterState;
	//FTL::CFStringFormater formaterAction;
	//FTLTRACE(TEXT("CSkinComboBox::DrawItem(%d): itemId=%d, iState= 0x%x(%s), iAction=0x%x(%s)\n"),
	//	GetTickCount(),
	//	lpDrawItemStruct->itemID, 
	//	iState, FTL::CFWinUtil::GetOwnerDrawState(formaterState, iState),
	//	lpDrawItemStruct->itemAction, FTL::CFWinUtil::GetOwnerDrawAction(formaterAction, lpDrawItemStruct->itemAction));

	FTLASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);

	if (ODS_COMBOBOXEDIT & iState)
	{
		DrawEdit(lpDrawItemStruct->hDC , iState, rcItem,  itemID );
	}
	else
	{
		DrawContent(lpDrawItemStruct->hDC, iState, rcItem, itemID );
	}
}

void CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//FTLTRACE(TEXT("CSkinComboBox::MeasureItem: itemId=%d\n"), lpMeasureItemStruct->itemID);
	lpMeasureItemStruct->itemHeight = ITEM_MARGIN_TOP + m_nFontHeigth + ITEM_MARGIN_BOTTOM;
}

int  CSkinComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	FTLTRACE(TEXT("CSkinComboBox::CompareItem: itemID1=%d, itemID2=%d\n"),
		lpCompareItemStruct->itemID1, lpCompareItemStruct->itemID2);
	return 0;
}

void CSkinComboBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	//FTLTRACE(TEXT("CSkinComboBox::DeleteItem: itemId=%d\n"), lpDeleteItemStruct->itemID);
}

void CSkinComboBox::OnPaint(CDCHandle /*dc*/)
{
	//FTLTRACE(TEXT("CSkinComboBox::OnPaint\n"));
	CPaintDC dc(m_hWnd); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	//CBitmap bmp;
	//bmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());

	CMemoryDC memDC(dc, rcClient);
	//HBITMAP hOldBmp = memDC.SelectBitmap(bmp);
	HFONT hOldFont = memDC.SelectFont(GetFont());

	//memDC.FillSolidRect(rcClient, COLOR_EDIT_BACKGROUND);
	CRect rc3DClient = rcClient;
	rc3DClient.DeflateRect(5,5);
	//memDC.Draw3dRect(rc3DClient, RGB(255, 0, 0), RGB(0, 255, 0));
	//FTL::CFGdiUtil::DrawCoordinate(memDC, rcClient, FALSE, TRUE);

	//CRect rcText = rcClient;
	
	DrawEdit( memDC, ODS_COMBOBOXEDIT, rcClient, GetCurSel());
	//DrawArray(memDC, rcClient);

	memDC.SelectFont(hOldFont);
}

BOOL CSkinComboBox::OnEraseBkgnd(CDCHandle dc)
{
	FTLTRACE(TEXT("CSkinComboBox::OnEraseBkgnd\n"));
	return TRUE;
}

//LRESULT CALLBACK CSkinComboBox::ListBoxWindowProc(  HWND hwnd,   UINT uMsg,   WPARAM wParam, 
//							LPARAM lParam )
//{
//	DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam);
//	CSkinComboBox* pCombox = (CSkinComboBox*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
//	if (pCombox)
//	{
//		switch(uMsg)
//		{
//		case WM_DESTROY:
//			::SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
//			::SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)(pCombox->m_oldListBoxWinProc));
//			pCombox->m_hListBox = NULL;
//			break;
//		}
//		return pCombox->m_oldListBoxWinProc(hwnd, uMsg, wParam, lParam);
//	}
//	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
//}

//HBRUSH CSkinComboBox::OnCtlColorEdit(CDCHandle dc, CEdit edit)
//{
//	HBRUSH hBrush = (HBRUSH)DefWindowProc();
//	FTLTRACE(TEXT("CSkinComboBox::OnCtlColorEdit\n"));
//	FTLASSERT(FALSE);
//
//	return hBrush;
//}

