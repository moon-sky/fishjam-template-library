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

#define ZOOM_COUNT	19

class CMapMakerView : public CZoomScrollWindowImpl<CMapMakerView>
    // CScrollWindowImpl<CMapMakerView>
{
    typedef CZoomScrollWindowImpl<CMapMakerView>    BaseClass;

public:
    BOOL SetImagePath(const CString& strImagePath);
    VOID SetDrawToolType(DrawToolType newType) {m_drawToolType = newType; }
    VOID SetTileParams(int nTileWidth, int nTileHeight)
    {
        m_nTileWidth = nTileWidth;
        m_nTileHeight = nTileHeight;
        ResetTileGrids(dttWall);
    }
    SIZE GetImageSize() const;
public:
	DECLARE_WND_CLASS(TEXT("CMapMakerView"))

    CMapMakerView();
    virtual ~CMapMakerView();

protected:
    BEGIN_MSG_MAP(CMapMakerView)
        //DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, uMsg, wParam, lParam)
        MSG_WM_CREATE(OnCreate)
        MSG_WM_ERASEBKGND(OnEraseBkgnd)
        MSG_WM_LBUTTONDOWN(OnLButtonDown);
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_MOUSEWHEEL(OnMouseWheel)
        CHAIN_MSG_MAP(BaseClass) //CZoomScrollWindowImpl<CMapMakerView>)
    END_MSG_MAP()

    virtual void ClientToDoc(CPoint* pPoint);
    virtual void ClientToDoc(CRect* pRect);
    virtual void DocToClient(CPoint* pPoint);
    virtual void DocToClient(CRect* pRect);

    BOOL PreTranslateMessage(MSG* pMsg);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void PrepareDC(CDCHandle dc);
    void DoPaint(CDCHandle dc);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    BOOL OnEraseBkgnd(CDCHandle dc);


    CRect _GetPhotoRect( CRect rcClient, LPSIZE lpSize);

    void _DrawGridLine(CDCHandle dc, const CRect& rcClipBox);
    VOID ResetTileGrids(DrawToolType newType);
    BOOL NextZoom(BOOL bBigger);
    int  _GetNextFixedZoomIndex(float curZoom, BOOL bBigger);
    void _SetScrollInfo(float newZoomScale, BOOL bUseCenterPoint = FALSE);
    BOOL SaveMapInfo(const CString& strFilePath);
    BOOL SaveTileGrids(const CString& strMapPath);
    BOOL LoadTileGrids(const CString& strMapPath);
    
    CAtlString GetExportMapString();
    VOID SetGridStatus(CPoint point);
    void _setPromptInfo(const CString& strPrompt);
    const CString& GetImagePath() { return m_strImagePath; }
private:
    CImage* m_pImage;
    CString m_strImagePath;
    float   m_ZoomScale;

    FTL::CFCanvas   m_Canvas;
    FTL::CFCalcRect *m_pCalcRect;
    BOOL                m_isCapturing;
    CRect				m_rcDrawTarget;
    DrawToolType        m_drawToolType;

    typedef std::vector<BYTE> TileRow;
    typedef std::vector<TileRow>    TileGrid;

public:
    int     m_nTileWidth;
    int     m_nTileHeight;
    int     m_RowCount;
    int     m_ColCount;
    int     m_nTranspant;

    TileGrid    m_tileGrids;
    //CToolTipCtrl    m_toolTip;

    static const float s_FixedZoomScales[ZOOM_COUNT];
    static const int   s_NormalZoomIndex;
    
};
