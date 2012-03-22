#include "StdAfx.h"
//#include "SkinScrollBar.h"
#include "SkinComboBox.h"
#include <atlimage.h>
#include <ftlgdi.h>


const int ARROW_MARGIN_TOP = 5;
const int ARROW_MARGIN_RIGHT = 4;
const int ARROW_MARGIN_BOTTOM = 5;
const int ARROW_WIDTH = 10;


const int ITEM_MARGIN_TOP = 9;
const int ITEM_MARGIN_BOTTOM = 12;
const int ITEM_MARGIN_LEFT = 8;

const COLORREF COLOR_STOKE_FOCUS = RGB(0x3f, 0x86, 0xce);
const COLORREF COLOR_ITEM_NORMAL_BACK = RGB(255,255,255);

const COLORREF COLOR_ITEM_FOUCS_BACK = RGB(255,0,0);
const COLORREF COLOR_EDIT_BACKGROUND = RGB(0,255,0);
const COLORREF COLOR_ARROW_TEMP =      RGB(0,0, 255);

CSkinComboBox::CSkinComboBox()
{
	m_nFontHeigth = 11;
	m_bOver = FALSE;
    m_hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
//	m_hListBox = NULL;
	m_oldListBoxWinProc = NULL;
}

CSkinComboBox::~CSkinComboBox()
{

}

//void CSkinComboBox::SetFont(HFONT hFont)
//{
//  FTLASSERT(FALSE);
//	FTLASSERT(hFont);
//
//	m_hFont = hFont;
//	CClientDC dc(m_hWnd);
//	
//	HFONT hOldFont = NULL;
//	
//	hOldFont = dc.SelectFont(hFont);
//	TEXTMETRIC textMetrics = { 0 };
//	dc.GetTextMetrics(&textMetrics);
//	m_nFontHeigth = textMetrics.tmHeight;
//	dc.SelectFont(hOldFont);
//}

//void CSkinComboBox::UpdateRect(void)
//{
//	CRect rcClient;
//	GetWindowRect( &rcClient );
//	rcClient.DeflateRect( -2,-2 );
//	GetParent().ScreenToClient( &rcClient );
//	GetParent().InvalidateRect( &rcClient,FALSE );
//	return;
//}

void CSkinComboBox::DrawEdit(HDC hDC, UINT iState,const CRect& rcClient, int itemID)
{
	CRect rcText = rcClient;
    rcText.left += EDIT_MARGIN_LEFT;
    rcText.top += EDIT_MARGIN_TOP;
    rcText.right -= ( ARROW_MARGIN_RIGHT + ARROW_WIDTH );
    rcText.bottom -= EDIT_MARGIN_BOTTOM;

	CDCHandle dc(hDC);
    dc.SetBkMode(TRANSPARENT);
    CRect rcFill = rcClient;
    rcFill.DeflateRect(1,1);

    dc.FillSolidRect(rcFill, RGB(255,255,255));
    //dc.Draw3dRect(rcFill, RGB(64,64,64), RGB(212,202,220));

    //if (iState & (ODS_SELECTED | ODS_FOCUS))
    //{
    //    dc.FillSolidRect(rcClient, RGB(127,0,0));
    //}
    //else
    //{
    //    dc.FillSolidRect(rcClient, RGB(0,127,0));
    //}

    CString strText;
    GetLBText(itemID, strText);
	dc.DrawText(strText, -1, rcText, DT_LEFT | DT_TOP );
}

BOOL CSkinComboBox::Initialize()
{
    BOOL bRet = TRUE;
    //if (!m_SkinEdit.m_hWnd)
    //{
    //    BOOL bRet = FALSE;
    //    COMBOBOXINFO cmbInfo = { sizeof(cmbInfo) };
    //    API_VERIFY(GetComboBoxInfo(&cmbInfo));
    //    FTLASSERT(cmbInfo.hwndItem != NULL);
    //    API_VERIFY(m_SkinEdit.SubclassWindow(cmbInfo.hwndItem));
    //}
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
	
	if (iState & (ODS_SELECTED|ODS_FOCUS))
	{
		dc.FillSolidRect(rcClient, COLOR_ITEM_FOUCS_BACK );
	}
    else
    {
        dc.FillSolidRect(rcClient, RGB(255,255,255)) ;// COLOR_ITEM_NORMAL_BACK);
    }
	dc.DrawText( strText,-1, &rcText, DT_TOP | DT_LEFT);
}

void CSkinComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	UINT itemID = lpDrawItemStruct->itemID;
    CRect rcItem = lpDrawItemStruct->rcItem;
	UINT  iState = lpDrawItemStruct->itemState;

	FTL::CFStringFormater formaterState;
    FTL::CFStringFormater formaterAction;
	FTLTRACE(TEXT("CSkinComboBox::DrawItem(%d): itemId=%d, iState= 0x%x(%s), iAction=0x%x(%s)\n"),
        GetTickCount(),
		lpDrawItemStruct->itemID, 
        iState, FTL::CFWinUtil::GetOwnerDrawState(formaterState, iState),
        lpDrawItemStruct->itemAction, FTL::CFWinUtil::GetOwnerDrawAction(formaterAction, lpDrawItemStruct->itemAction));

	FTLASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);

    CDCHandle dc(lpDrawItemStruct->hDC);
    HFONT hOldFont = dc.SelectFont(m_hFont);

    //CBitmap bmp;
    //bmp.CreateCompatibleBitmap(lpDrawItemStruct->hDC, rcItem.Width(), rcItem.Height());

	//CDCHandle dc(lpDrawItemStruct->hDC);
    {
        //CMemoryDC memDC(lpDrawItemStruct->hDC, rcItem);
        //HBITMAP hOldBmp = memDC.SelectBitmap(bmp);
        //memDC.FillSolidRect(rcItem, RGB(0,255,0));

        //memDC.SetBkMode( TRANSPARENT );
        if (ODS_COMBOBOXEDIT & iState)
        {
            DrawEdit(lpDrawItemStruct->hDC , iState, rcItem,  itemID );
        }
        else
        {
            DrawContent(lpDrawItemStruct->hDC, iState, rcItem, itemID );
        }
        //memDC.SelectBitmap(hOldBmp);
    }

    dc.SelectFont(hOldFont);

	//CString strItem;
	////strItem.Format(TEXT("Item %d"), lpDrawItemStruct->itemID);



	//if (lpDrawItemStruct->itemID != -1)
	//{
	//	GetLBText(lpDrawItemStruct->itemID, strItem);
	//	//FTLASSERT(lpszText != NULL);

	//	// Save these value to restore them when done drawing.
	//	COLORREF crOldTextColor = dc.GetTextColor();
	//	COLORREF crOldBkColor = dc.GetBkColor();

	//	// If this item is selected, set the background color 
	//	// and the text color to appropriate values. Erase
	//	// the rect by filling it with the background color.
	//	if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
	//		(lpDrawItemStruct->itemState  & ODS_SELECTED))
	//	{
	//		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	//		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
	//		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	//	}
	//	else
	//	{
	//		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	//	}

	//	// Draw the text.
	//	dc.DrawText(
	//		strItem,
	//		-1,
	//		&lpDrawItemStruct->rcItem,
	//		DT_CENTER|DT_SINGLELINE|DT_VCENTER);

	//	// Reset the background color and the text color back to their
	//	// original values.
	//	dc.SetTextColor(crOldTextColor);
	//	dc.SetBkColor(crOldBkColor);
	//}
	//


	////dc.Detach();

}

void CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	FTLTRACE(TEXT("CSkinComboBox::MeasureItem: itemId=%d\n"), lpMeasureItemStruct->itemID);

	//if (lpMeasureItemStruct->itemID != (UINT) -1)
	//{
	//	LPCTSTR lpszText = (LPCTSTR) lpMeasureItemStruct->itemData;
	//	CSize   sz;
	//	CClientDC dc(m_hWnd);
	//	dc.GetTextExtent(lpszText, -1, &sz);
	//	lpMeasureItemStruct->itemHeight = 2*sz.cy;
	//}
	//else
	//{
	CRect rcClient;
	GetClientRect(rcClient);
	lpMeasureItemStruct->itemHeight = 30;// MARGIN_TOP + m_nFontHeigth + MARGIN_BOTTOM;
	lpMeasureItemStruct->itemWidth = rcClient.Width();
	//}

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

UINT CSkinComboBox::OnGetDlgCode(LPMSG lpMsg)
{
    UINT nRet = DefWindowProc();
    return nRet;
}

int CSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    //if (!m_SkinEdit.m_hWnd)
    //{
    //    BOOL bRet = FALSE;
    //    COMBOBOXINFO cmbInfo = { sizeof(cmbInfo) };
    //    API_VERIFY(GetComboBoxInfo(&cmbInfo));
    //    FTLASSERT(cmbInfo.hwndItem != NULL);
    //    API_VERIFY(m_SkinEdit.SubclassWindow(cmbInfo.hwndItem));
    //}
    SetMsgHandled(FALSE);
    return 0;
}

void CSkinComboBox::OnDestroy()
{
    //if (m_SkinListBox.m_hWnd != NULL)
    //{
    //    m_SkinListBox.UnsubclassWindow();
    //}
    DefWindowProc();
}

void CSkinComboBox::OnPaint(CDCHandle /*dc*/)
{
	FTLTRACE(TEXT("CSkinComboBox::OnPaint\n"));
	CPaintDC dc(m_hWnd); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

    //CBitmap bmp;
    //bmp.CreateCompatibleBitmap(dc, rcClient.Width(), rcClient.Height());

    CMemoryDC memDC(dc, rcClient);
    //HBITMAP hOldBmp = memDC.SelectBitmap(bmp);
    HFONT hOldFont = memDC.SelectFont(m_hFont);

	//memDC.FillSolidRect(rcClient, COLOR_EDIT_BACKGROUND);
    CRect rc3DClient = rcClient;
    rc3DClient.DeflateRect(5,5);
    //memDC.Draw3dRect(rc3DClient, RGB(255, 0, 0), RGB(0, 255, 0));

    //FTL::CFGdiUtil::DrawCoordinate(memDC, rcClient, FALSE, TRUE);


	//CRect rcText = rcClient;
	
	DrawEdit( memDC, ODS_COMBOBOXEDIT, rcClient, GetCurSel());
    DrawArray(memDC, rcClient);
	//DrawEdit(0, rc, dc, 0);
	//OnNcPaint( dc );
    
    memDC.SelectFont(hOldFont);
    //memDC.SelectBitmap(hOldBmp);
}

void CSkinComboBox::DrawArray(HDC hDC, const CRect& rcClient)
{
    CDCHandle dc(hDC);

    CRect rcArray = rcClient;
    rcArray.right -= ARROW_MARGIN_RIGHT;
    rcArray.left = rcArray.right - ARROW_WIDTH;
    rcArray.top = ARROW_MARGIN_TOP;
    rcArray.bottom -= ARROW_MARGIN_BOTTOM;

    dc.FillSolidRect(rcArray, COLOR_ARROW_TEMP );
}

void CSkinComboBox::OnNcPaint( HDC hDC )
{
	//CRect rc;
	//GetClientRect(&rc);
	//DrawEdit(0, rc, hDC, 0);
	//return ;

	////绘制客户区
	//CDC dMemDC;
	//dMemDC.CreateCompatibleDC(hdc);
	////dMemDC.SetMapMode(pDC->GetMapMode());

	////画动作
	//CBitmap mNewBmp;
	//CRect rc;
	//GetClientRect(&rc);

	//mNewBmp.CreateCompatibleBitmap( hDC, rc.right - rc.left, rc.bottom - rc.top);
	//CBitmap hOldBmp = dMemDC.SelectBitmap(&mNewBmp);

	////CPen* pOldPen = dMemDC.SelectObject( &m_bgPen );
	////CBrush* pOldBrsh = dMemDC.SelectObject( m_bgBrush );
	////dMemDC.Rectangle( &rc );
	////dMemDC.SelectObject( pOldPen );
	////dMemDC.SelectObject( &pOldBrsh );

	//if ( (GetCurSel() > 4)||(GetCurSel() < 0) )
	//{
	//	return;
	//}

	//PItemList pItem = m_vecItemList[GetCurSel()];

	//CPen	m_penLeft;
	//CPen	m_penRight;
	//CRect rcLeft,rcMid,rcRight;
	//rcLeft = rcMid = rcRight = rc;
	//dMemDC.SetBkMode( TRANSPARENT );

	////画第一段图标
	//rcLeft.right = rcLeft.left + 16;
	//rcLeft.left += 1;
	//rcLeft.top += 4;
	//::DrawIconEx( dMemDC.m_hDC,rcLeft.left,rcLeft.top,pItem->hIcon,16,16,NULL,NULL,DI_NORMAL );

	////画第二段文本
	//rcMid.left = rcLeft.right + 10;
	//rcMid.right = rc.right - 110;
	//CFont* pOldFont = dMemDC.SelectObject( &m_font );
	//dMemDC.SetTextColor( pItem->clrLeft );
	//dMemDC.DrawText( pItem->strUrl,&rcMid,DT_VCENTER|DT_LEFT|DT_SINGLELINE );

	////画第三段文本
	//rcRight.left = rcMid.right;
	//dMemDC.SetTextColor( pItem->clrRight );
	//dMemDC.DrawText( pItem->strTitle,&rcRight,DT_VCENTER|DT_LEFT|DT_SINGLELINE );
	//dMemDC.SelectObject( pOldFont );

	//CRect rcEnd(rc);
	//rcEnd.left = rc.right - 22;

	//if( m_bDown )
	//{
	//	dMemDC.DrawFrameControl( &rcEnd,DFC_SCROLL,DFCS_SCROLLDOWN|DFCS_FLAT|DFCS_MONO|DFCS_PUSHED );
	//}
	//else
	//{
	//	dMemDC.DrawFrameControl( &rcEnd,DFC_SCROLL,DFCS_SCROLLDOWN|DFCS_FLAT|DFCS_MONO );
	//}

	//pDC->BitBlt(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, &dMemDC,
	//	rc.left ,rc.top, SRCCOPY);

	////恢复
	//dMemDC.SelectObject(pOldBmp);
	//pOldBmp->DeleteObject();
	//dMemDC.DeleteDC();
}

BOOL CSkinComboBox::OnEraseBkgnd(CDCHandle dc)
{
	FTLTRACE(TEXT("CSkinComboBox::OnEraseBkgnd\n"));
	return TRUE;
}
void CSkinComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	FTLTRACE(TEXT("CSkinComboBox::OnMouseMove\n"));
	//if ( m_bOver == FALSE )
	//{
	//	m_bOver = TRUE;

	//	//更新当前区域
	//	UpdateRect();

	//	TRACKMOUSEEVENT tme;
	//	tme.cbSize = sizeof( TRACKMOUSEEVENT );
	//	tme.dwFlags = TME_LEAVE;
	//	tme.dwHoverTime = 0;
	//	tme.hwndTrack = m_hWnd;

	//	_TrackMouseEvent( &tme );
	//}
	////CComboBox::OnMouseMove(nFlags, point);
	SetMsgHandled(FALSE);
}

void CSkinComboBox::OnMouseLeave()
{
	FTLTRACE(TEXT("CSkinComboBox::OnMouseLeave\n"));
	if ( m_bOver )
	{
		m_bOver = FALSE;
	}
	//UpdateRect();
	
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

HBRUSH CSkinComboBox::OnCtlColorEdit(CDCHandle dc, CEdit edit)
{
    HBRUSH hBrush = (HBRUSH)DefWindowProc();
    FTLTRACE(TEXT("CSkinComboBox::OnCtlColorEdit\n"));
    FTLASSERT(FALSE);

    return hBrush;
}

HBRUSH CSkinComboBox::OnReflectedCtlColorEdit(CDCHandle dc, CEdit edit)
{
    FTLTRACE(TEXT("CSkinComboBox::OnReflectedCtlColorEdit\n"));

    BOOL bRet = FALSE;
    if(edit.m_hWnd == GetWindow(GW_CHILD).m_hWnd)
    {
        //绘制Edit,这里我在只是改变了背景色
        static HBRUSH m_hBrush = NULL;
        m_hBrush = ::CreateSolidBrush(RGB(216,216,216));
        dc.SetBkMode(TRANSPARENT);
        //SetBkColor(hdcEdit, m_UserName.Bkcol); //设置为透明，这句就没用了
        dc.SetTextColor(RGB(0,20,245));
        return m_hBrush;
    }
    else if(edit.m_hWnd == m_hWnd)
    { 
        //绘制ComboBox
        static HBRUSH m_hBrush = ::CreateSolidBrush(RGB(216,216,216));
        //CDC dcMem;
        //dcMem.CreateCompatibleDC(dc);
        //dc.SaveDC();
        //dcMem.SaveDC();
        //dcMem.SelectBitmap (m_DropDown);
        CRect rect;
        ::GetClientRect(edit.m_hWnd,&rect);
        dc.FillSolidRect(&rect, RGB(216,216,216));
        //绘制下拉按钮
        dc.FillSolidRect(rect.right-19,2, 16, 17, COLOR_ARROW_TEMP);
        //dc.BitBlt( rect.right-19,2, 16, 17, dcMem, 0, 0, SRCCOPY); 
        return  m_hBrush; 
    }
    HBRUSH hBrush = (HBRUSH)DefWindowProc();
    return hBrush;
}

HBRUSH CSkinComboBox::OnCtlColorListBox(CDCHandle dc, CListBox listBox)
{
    FTLTRACE(TEXT("CSkinComboBox::OnCtlColorListBox\n"));
    HBRUSH hBrush = (HBRUSH)DefWindowProc();
    return hBrush;
}

void CSkinComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	FTLTRACE(TEXT("CSkinComboBox::OnLButtonDown\n"));
	DefWindowProc();
}
void CSkinComboBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	FTLTRACE(TEXT("CSkinComboBox::OnLButtonUp\n"));
	DefWindowProc();
}

