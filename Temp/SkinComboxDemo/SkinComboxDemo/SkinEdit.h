#pragma once

#include <ftlWindow.h>

#define EDIT_MARGIN_LEFT  8
#define EDIT_MARGIN_TOP  5
#define EDIT_MARGIN_BOTTOM 5 


class CSkinEdit : public CWindowImpl<CSkinEdit, CEdit>//,
    //public COwnerDraw<CSkinButton>
{
public:
    CSkinEdit(void);
    ~CSkinEdit(void);

    BEGIN_MSG_MAP_EX(CSkinEdit)
        DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam);

        MSG_WM_PAINT(OnPaint)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        //MSG_WM_MOUSEMOVE(OnMouseMove)
        //MSG_WM_MOUSELEAVE(OnMouseLeave)
        //MSG_WM_LBUTTONDOWN(OnLButtonDown)
        //MSG_WM_LBUTTONUP(OnLButtonUp)
        //MESSAGE_HANDLER( CB_INSERTSTRING, OnInsertString )
        //MSG_OCM_CTLCOLOREDIT(OnReflectedCtlColorEdit)
        //MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)

        //CHAIN_MSG_MAP_ALT(COwnerDraw<CSkinButton>, 1)
        //DEFAULT_REFLECTION_HANDLER()
        END_MSG_MAP()
public:
    void OnPaint(CDCHandle /*dc*/);
    BOOL OnEraseBkgnd(CDCHandle dc);
};
