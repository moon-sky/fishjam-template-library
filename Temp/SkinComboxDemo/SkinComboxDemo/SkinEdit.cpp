#include "StdAfx.h"
#include "SkinEdit.h"

CSkinEdit::CSkinEdit(void)
{
}

CSkinEdit::~CSkinEdit(void)
{
}

void CSkinEdit::OnPaint(CDCHandle /*dc*/)
{
    CRect rcClient;
    GetClientRect(rcClient);

    CRect rcText = rcClient;
    rcText.left += EDIT_MARGIN_LEFT;
    rcText.top += EDIT_MARGIN_TOP;
    //rcText.right -= ( ARROW_MARGIN_RIGHT + ARROW_WIDTH );
    rcText.bottom -= EDIT_MARGIN_BOTTOM;

    CPaintDC dc(m_hWnd);
    dc.SetBkMode(TRANSPARENT);
    dc.FillSolidRect(rcClient, RGB(255,0,0));
    dc.FillSolidRect(rcText, RGB(0,255,0));

    CString strText;
    GetWindowText(strText);
    dc.DrawText(strText, -1, rcText, DT_LEFT | DT_TOP );

}

BOOL CSkinEdit::OnEraseBkgnd(CDCHandle dc)
{
    return TRUE;
}