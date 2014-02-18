// MapMakerView.h : interface of the CMapMakerView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <ftlGdi.h>

enum DrawToolType{
    dttEmpty = 0,
    dttWall,
    dttElevator,

    dttCount
};


class CMapMakerView : public CZoomScrollWindowImpl<CMapMakerView>
    // CScrollWindowImpl<CMapMakerView>
{
    typedef CZoomScrollWindowImpl<CMapMakerView>    BaseClass;

public:
    BOOL SetImagePath(const CString& strImagePath);
    VOID SetDrawToolType(DrawToolType newType) {m_drawToolType = newType; }
    VOID SetTileParams(int nTileWidth, int nTileHeight)
    {
        m_nGridWidth = nTileWidth;
        m_nGridHeight = nTileHeight;
        ResetTileGrids();
    }
public:
	DECLARE_WND_CLASS(TEXT("CMapMakerView"))

    CMapMakerView();
    virtual ~CMapMakerView();

protected:
    BEGIN_MSG_MAP(CMapMakerView)
        DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, uMsg, wParam, lParam)
        MSG_WM_ERASEBKGND()
        MSG_WM_LBUTTONDOWN(OnLButtonDown);
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        CHAIN_MSG_MAP(BaseClass) //CZoomScrollWindowImpl<CMapMakerView>)
    END_MSG_MAP()

    BOOL PreTranslateMessage(MSG* pMsg);
    void PrepareDC(CDCHandle dc);
    void DoPaint(CDCHandle dc);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);

    BOOL OnEraseBkgnd(CDCHandle dc);


    CRect _GetPhotoRect( CRect rcClient, LPSIZE lpSize);

    void _DrawGridLine(CDCHandle dc, const CRect& rcClient);
    VOID ResetTileGrids();
private:
    CImage* m_pImage;
    FTL::CFCanvas   m_Canvas;
    FTL::CFCalcRect *m_pCalcRect;
    BOOL                m_isCapturing;
    CRect				m_rcDrawTarget;
    DrawToolType        m_drawToolType;

    typedef std::vector<int> TileRow;
    typedef std::vector<TileRow>    TileGrid;

public:
    int     m_nGridWidth;
    int     m_nGridHeight;
    int     m_RowCount;
    int     m_ColCount;

    TileGrid    m_tileGrids;
};
