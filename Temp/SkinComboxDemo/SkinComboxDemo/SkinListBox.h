#pragma once

#include <ftlWindow.h>

class CSkinListBox : public CWindowImpl<CSkinListBox, CListBox>,
    public COwnerDraw<CSkinListBox>
{
public:
    CSkinListBox(void);
    ~CSkinListBox(void);

    BEGIN_MSG_MAP_EX(CSkinListBox)
        //DUMP_WINDOWS_MSG(__FILE__LINE__, DEFAULT_DUMP_FILTER_MESSAGE, uMsg, wParam, lParam);

        //MSG_WM_PAINT(OnPaint)
        //MSG_WM_ERASEBKGND(OnEraseBkgnd)
        //MSG_WM_MOUSEMOVE(OnMouseMove)
        //MSG_WM_MOUSELEAVE(OnMouseLeave)
        //MSG_WM_LBUTTONDOWN(OnLButtonDown)
        //MSG_WM_LBUTTONUP(OnLButtonUp)
        //MESSAGE_HANDLER( CB_INSERTSTRING, OnInsertString )
        //MSG_OCM_CTLCOLOREDIT(OnReflectedCtlColorEdit)
        //MSG_WM_CTLCOLORLISTBOX(OnCtlColorListBox)
        CHAIN_MSG_MAP_ALT(COwnerDraw<CSkinListBox>, 1)
        DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    int  CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
    void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
};
