// MapMakerView.cpp : implementation of the CMapMakerView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MapMakerView.h"

#include <ftlDebug.h>
using namespace FTL;

//#define GRID_HEIGHT     8
//#define GRID_WIDTH      8

const COLORREF COLOR_DRAWTOOL_TYPES[dttCount] = {
    RGB(0, 0, 0),
    RGB(0, 255, 0),
    RGB(0, 0, 255)
};

CMapMakerView::CMapMakerView()
{
    m_pImage = NULL;
    m_pCalcRect = CFCalcRect::GetCalcRectObject(CFCalcRect::modeAutoFitIfBigSize);;
    m_rcDrawTarget.SetRect(0,0,0,0);// (100, 100, 400, 400);
    m_RowCount = 0;
    m_ColCount = 0;
    m_isCapturing = FALSE;
    m_nGridWidth = m_nGridHeight = 32;
    m_drawToolType = dttWall;
}

CMapMakerView::~CMapMakerView()
{
    SAFE_DELETE(m_pCalcRect);
}

VOID CMapMakerView::ResetTileGrids()
{
    m_RowCount = m_pImage->GetHeight() / m_nGridHeight;
    m_ColCount = m_pImage->GetWidth() / m_nGridWidth;

    m_tileGrids.resize(m_RowCount);
    for (int i = 0; i < m_RowCount; i++)
    {
        m_tileGrids[i].resize(m_ColCount);
    }
    Invalidate(FALSE);

}
BOOL CMapMakerView::SetImagePath(const CString& strImagePath)
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;
    FTLASSERT(NULL == m_pImage);
    m_pImage = new CImage();
    if (m_pImage)
    {
        COM_VERIFY(m_pImage->Load(strImagePath));
        if (FAILED(hr))
        {
            SAFE_DELETE(m_pImage);
        }
        else{
            bRet = TRUE;
            ResetTileGrids();
        }
    }
    return bRet;
}

BOOL CMapMakerView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

CRect CMapMakerView::_GetPhotoRect( CRect rcClient, LPSIZE lpSize)
{
    CRect rcTarget( 0, 0, 0, 0 );
    if (m_pCalcRect)
    {
        rcTarget = m_pCalcRect->GetFitRect(rcClient, *lpSize);
    }
    //FTLTRACE(TEXT("rcClient=[%d,%d]x[%d,%d], lpSize={%d,%d}, rcTarget=[%d,%d]x[%d,%d],{%d,%d}, DocToClient(0,0)=[%d,%d], ClientToDoc(0,0)=[%d,%d]\n"), 
    //	rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, lpSize->cx, lpSize->cy,
    //	rcTarget.left, rcTarget.top, rcTarget.right, rcTarget.bottom,
    //	rcTarget.Width(), rcTarget.Height(),
    //	ptOriginalLogical.x, ptOriginalLogical.y, ptOriginalDevice.x, ptOriginalDevice.y);

    //rcTarget.OffsetRect(-rcTarget.TopLeft());
    return 	rcTarget;
}

void CMapMakerView::PrepareDC(CDCHandle dc)
{
    ATLASSERT(m_sizeAll.cx >= 0 && m_sizeAll.cy >= 0);
    dc.SetMapMode(MM_ANISOTROPIC);
    dc.SetWindowExt(m_sizeLogAll);
    dc.SetViewportExt(m_sizeAll);
    dc.SetViewportOrg(-m_ptOffset.x, -m_ptOffset.y);

    if (m_pImage)
    {
        CRect rcPaintAll(CPoint(0), m_sizeAll);	//device unit
        if (m_sizeClient.cx > m_sizeAll.cx)
        {
            rcPaintAll.right = m_sizeClient.cx;
        }
        if (m_sizeClient.cy > m_sizeAll.cy)
        {
            rcPaintAll.bottom = m_sizeClient.cy;
        }

        dc.DPtoLP((LPPOINT)(&rcPaintAll), 2);  //change to logical
        CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
        //m_rcDrawTarget = _GetPhotoRect(rcPaintAll, &(rcImage.Size()));
    }

    dc.SetWindowOrg(-m_rcDrawTarget.left, -m_rcDrawTarget.top);
}


void CMapMakerView::_DrawGridLine(CDCHandle dc, const CRect& rcClient)
{
    BOOL bRet = FALSE;
    CPen penLine;
    API_VERIFY(NULL != penLine.CreatePen(PS_SOLID, 1, RGB(0, 255, 0)));

    HPEN hOldPen = dc.SelectPen(penLine);

    for (LONG h = rcClient.top; h < rcClient.bottom; h += m_nGridHeight)
    {
        API_VERIFY(dc.MoveTo(0, h));
        API_VERIFY(dc.LineTo(rcClient.Width(), h));
    }

    for (LONG w = rcClient.left; w < rcClient.right; w += m_nGridWidth)
    {
        API_VERIFY(dc.MoveTo(w, 0));
        API_VERIFY(dc.LineTo(w, rcClient.Height()));
    }

    for (int x = 0; x < m_ColCount; x++)
    {
        for (int y = 0; y < m_RowCount; y++)
        {
            int nType = m_tileGrids[y][x];
            if (nType != dttEmpty)
            {
                CRect rcClickGrid(x * m_nGridWidth, y * m_nGridHeight, 
                    (x + 1) * m_nGridWidth, (y + 1) * m_nGridHeight);


                dc.FillSolidRect(rcClickGrid, COLOR_DRAWTOOL_TYPES[nType]);
            }
        }
    }
    dc.SelectPen(hOldPen);
}

void CMapMakerView::DoPaint(CDCHandle dc)
{
    BOOL bRet = FALSE;

    CRect rcClientLogical;
    GetClientRect(rcClientLogical);
    dc.DPtoLP(rcClientLogical);
    rcClientLogical.InflateRect(4, 4);	//fix scroll brush display error

    {
        FTL::CFScrollZoomMemoryDC memDC (dc.m_hDC, rcClientLogical);

        PrepareDC(memDC.m_hDC);
        //memDC.SetBrushOrg( -m_ptOffset.x , -m_ptOffset.y);
        memDC.FillSolidRect(rcClientLogical, RGB(255, 0, 0));

        if (m_pImage && !m_pImage->IsNull())
        {
            CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
            if (m_Canvas.IsCanvasChanged(rcImage))
            {
                m_Canvas.Release();
                API_VERIFY(m_Canvas.Create(m_hWnd, rcImage.Width(), rcImage.Height()));

                API_VERIFY(m_pImage->Draw(m_Canvas, rcImage));
            }

            API_VERIFY(memDC.BitBlt(
                0, 0, //rcClientLogical.left, rcClientLogical.top
                rcImage.Width(), rcImage.Height(), // rcClientLogical.Width(), rcClientLogical.Height(),
                m_Canvas.GetCanvasDC(), 
                0, 0, //rcClientLogical.left, rcClientLogical.top, 
                SRCCOPY));

            _DrawGridLine(memDC.m_hDC, rcClientLogical);
        }

        //memDC.DrawText(TEXT("Fishjam"), -1, rcClientLogical, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}


BOOL CMapMakerView::OnEraseBkgnd(CDCHandle dc)
{
    return TRUE;
}

void CMapMakerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetCapture();
    m_isCapturing = TRUE;
}

void CMapMakerView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_isCapturing)
    {
        int nRow = point.y / m_nGridHeight;
        int nCol = point.x / m_nGridWidth;

        if (nRow < m_RowCount && nCol < m_ColCount)
        {
            m_tileGrids[nRow][nCol] = m_drawToolType;
            CRect rcClickGrid(nCol * m_nGridWidth, nRow * m_nGridHeight, 
                (nCol + 1) * m_nGridWidth, (nRow + 1) * m_nGridHeight);

            FTLTRACE(TEXT("OnMouseMove, nRow=%d, nCol=%d, rcClickGrid=%s\n"), nRow, nCol,
                CFRectDumpInfo(rcClickGrid).GetConvertedInfo());
            InvalidateRect(rcClickGrid, FALSE);
        }
    }
}
void CMapMakerView::OnLButtonUp(UINT nFlags, CPoint point)
{
    BOOL bRet = FALSE;
    if (m_isCapturing)
    {
        m_isCapturing = FALSE;
        API_VERIFY(ReleaseCapture());
    }
}