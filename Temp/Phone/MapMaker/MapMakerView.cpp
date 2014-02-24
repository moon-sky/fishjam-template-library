// MapMakerView.cpp : implementation of the CMapMakerView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MapMakerView.h"
#include "MainFrm.h"
#include <atlfile.h>
#include <atlpath.h>

#include <ftlDebug.h>
#include <ftlFile.h>
using namespace FTL;

#define MAX_FILE_HEADER_PARAM_COUNT   (10)
struct MapFileHeader
{
    int nVersion;
    int nGridWidth;
    int nGridHeight;
    int nRowCount;
    int nColCount;
    int nTranspant;
    int nPadding[MAX_FILE_HEADER_PARAM_COUNT - 6];

    MapFileHeader(){
        nVersion = 1;
        nGridWidth = 0;
        nGridHeight = 0;
        nRowCount = 0;
        nColCount = 0;
        nTranspant = 0;

        for (int i = 0; i < _countof(nPadding); i++)
        {
            nPadding[i] = 0x12345678;
        }
    }
};

const COLORREF COLOR_GRID_LINE = RGB(127, 127, 127);
const COLORREF COLOR_MAP_BACKGROUND = RGB(255, 255, 255);
const CString STR_MAP_APPENDIX   = _T("_map.map");

const COLORREF COLOR_DRAWTOOL_TYPES[dttCount] = {
    RGB(0, 0, 0),       //Empty
    RGB(0, 0, 0),       //不能通过的墙等
    RGB(0, 0, 255)      //电梯
};

const float CMapMakerView::s_FixedZoomScales[ZOOM_COUNT] = 
{
    0.05f,
    0.10f,
    0.20f,
    0.25f,
    0.5f,
    0.6f,
    0.7f,
    0.8f,
    0.9f,
    1.0f,
    1.25f,
    1.5f,
    2.0f,
    3.0f,
    4.0f,
    6.0f,
    8.0f,
    10.0f,
    15.0f
};
const int CMapMakerView::s_NormalZoomIndex = 9;

CMapMakerView::CMapMakerView()
{
    m_pImage = NULL;
    m_pCalcRect = CFCalcRect::GetCalcRectObject(CFCalcRect::modeAutoFitIfBigSize);;
    m_rcDrawTarget.SetRect(0,0,0,0);// (100, 100, 400, 400);
    m_RowCount = 0;
    m_ColCount = 0;
    m_isCapturing = FALSE;
    m_nGridWidth = m_nGridHeight = 8;
    m_nTranspant = 70;
    m_drawToolType = dttEmpty;
    m_ZoomScale = 1.0f;
}

CMapMakerView::~CMapMakerView()
{
    SAFE_DELETE(m_pImage);
    SAFE_DELETE(m_pCalcRect);
}

int CMapMakerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    BOOL bRet = FALSE;
    //API_VERIFY(m_toolTip.Create(m_hWnd) != NULL);
    //m_toolTip.SetDelayTime(100, 1000);
    SetMsgHandled(FALSE);
    return 1;
}
VOID CMapMakerView::ResetTileGrids(DrawToolType newType)
{
    m_RowCount = (m_pImage->GetHeight() + m_nGridHeight - 1 ) / m_nGridHeight;
    m_ColCount = (m_pImage->GetWidth() + m_nGridWidth - 1 ) / m_nGridWidth;

    m_tileGrids.resize(m_RowCount);
    for (int i = 0; i < m_RowCount; i++)
    {
        m_tileGrids[i].resize(m_ColCount);
        for (int j = 0; j < m_ColCount; j++)
        {
            m_tileGrids[i][j] = newType;
        }
    }
    Invalidate(FALSE);
}

BOOL CMapMakerView::LoadTileGrids(const CString& strMapPath)
{
    BOOL bRet = FALSE;
    CFFile file;
    API_VERIFY(file.Create(strMapPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING));
    if (bRet)
    {
        DWORD dwReadCount = 0;
        MapFileHeader header;
        API_VERIFY(file.Read(&header, sizeof(header), &dwReadCount));

        m_ColCount = header.nColCount;
        m_RowCount = header.nRowCount;
        m_nGridHeight = header.nGridHeight;
        m_nGridWidth = header.nGridWidth;
        m_nTranspant = header.nTranspant;

        m_tileGrids.resize(m_RowCount);
        for (int i = 0; i < m_RowCount; i++)
        {
            m_tileGrids[i].resize(m_ColCount);
            API_VERIFY(file.Read(&m_tileGrids[i][0], sizeof(m_tileGrids[0][0]) * m_ColCount, &dwReadCount));
        }

        API_VERIFY(file.Close());
    }
    return bRet;
}

BOOL CMapMakerView::SaveTileGrids(const CString& strMapPath)
{
    BOOL bRet = FALSE;
    CFFile file;
    API_VERIFY(file.Create(strMapPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        CREATE_ALWAYS));
    if (bRet)
    {
        MapFileHeader header;
        header.nColCount = m_ColCount;
        header.nRowCount = m_RowCount;
        header.nGridHeight = m_nGridHeight;
        header.nGridWidth = m_nGridWidth;
        header.nTranspant = m_nTranspant;
        DWORD dwWriteCount = 0;
        API_VERIFY(file.Write(&header, sizeof(header), &dwWriteCount));

        for (int i = 0; i < m_RowCount; i++)
        {
            API_VERIFY(file.Write(&m_tileGrids[i][0], sizeof(m_tileGrids[0][0]) * m_ColCount, &dwWriteCount));
        }

        API_VERIFY(file.Close());
    }
    return bRet;
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
        else
        {
            bRet = TRUE;
            m_strImagePath = strImagePath;
            CPath pathMap(m_strImagePath);
            pathMap.RemoveExtension();
            pathMap.m_strPath += STR_MAP_APPENDIX;
            if (pathMap.FileExists())
            {
                LoadTileGrids(pathMap.m_strPath);
            }
            else
            {
                ResetTileGrids(dttWall);
            }
            
        }
    }
    return bRet;
}

BOOL CMapMakerView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}


void CMapMakerView::ClientToDoc(CPoint* pPoint) 
{
    CWindowDC dc(m_hWnd);
    PrepareDC(dc.m_hDC);
    dc.DPtoLP(pPoint, 1);
}
void CMapMakerView::ClientToDoc(CRect* pRect)
{
    CWindowDC dc(m_hWnd);
    PrepareDC(dc.m_hDC);
    dc.DPtoLP((LPPOINT)pRect, 2);
}

void CMapMakerView::DocToClient(CPoint* pPoint)
{
    CWindowDC dc(m_hWnd);
    PrepareDC(dc.m_hDC);
    dc.LPtoDP(pPoint, 1);
}
void CMapMakerView::DocToClient(CRect* pRect)
{
    CWindowDC dc(m_hWnd);
    PrepareDC(dc.m_hDC);
    dc.LPtoDP((LPPOINT)pRect, 2);
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


void CMapMakerView::_DrawGridLine(CDCHandle dc, const CRect& rcClipBox)
{
    FUNCTION_BLOCK_TRACE(50);

    BOOL bRet = FALSE;
    CPen penLine;
    API_VERIFY(NULL != penLine.CreatePen(PS_SOLID, 1, COLOR_GRID_LINE));

    HPEN hOldPen = dc.SelectPen(penLine);

    CString strText;
    dc.SetBkMode(TRANSPARENT);

    int nStartRow = (rcClipBox.top + m_nGridHeight - 1) / m_nGridHeight;
    int nEndRow  = (rcClipBox.bottom + m_nGridHeight - 1 ) / m_nGridHeight;
    int nStartCol = (rcClipBox.left + m_nGridWidth - 1 ) / m_nGridWidth;
    int nEndCol = (rcClipBox.right + m_nGridWidth - 1 ) / m_nGridWidth;
    
    nEndCol = nEndCol < m_ColCount - 1 ? nEndCol : m_ColCount - 1;
    nEndRow = nEndRow < m_RowCount - 1 ? nEndRow : m_RowCount - 1;
    
    FTLTRACE(TEXT("rcClipBox=%s, ImageSize=(%dx%d), Row=%d->%d(RowCount=%d), Col=%d->%d(ColCount=%d)\n"), 
        CFRectDumpInfo(rcClipBox).GetConvertedInfo(), m_pImage->GetWidth(), m_pImage->GetHeight(),
        nStartRow, nEndRow, m_RowCount, nStartCol, nEndCol, m_ColCount);

    for (int h = 0; h < m_RowCount; h++)
    //for( int h = nStartRow; h <= nEndRow; h++)
    {
        API_VERIFY(dc.MoveTo(0, h * m_nGridHeight));
        API_VERIFY(dc.LineTo(m_nGridWidth * m_ColCount, h * m_nGridHeight));
        if (h % 5 == 0)
        {
            strText.Format(TEXT("%d"), h);
            dc.TextOut(m_nGridWidth * m_ColCount + 4, h * m_nGridHeight, strText);
        }
    }

    for (int w = 0; w < m_ColCount; w++)
    //for(int w = nStartCol; w <= nEndCol; w++)
    {
        API_VERIFY(dc.MoveTo(w * m_nGridWidth, 0));
        API_VERIFY(dc.LineTo(w * m_nGridWidth, m_nGridHeight * m_RowCount));
        if (w % 5 == 0)
        {
             strText.Format(TEXT("%d"), w);
             dc.TextOut(w * m_nGridWidth, m_nGridHeight * m_RowCount + 4, strText);
        }
    }

    CWindowDC deskDC(GetDesktopWindow());
    CDC memDC;
    API_VERIFY(memDC.CreateCompatibleDC(deskDC) != NULL);
    //PrepareDC(memDC.m_hDC);
    CBitmap memBitmap;
    API_VERIFY(memBitmap.CreateCompatibleBitmap(deskDC, m_nGridWidth, m_nGridHeight)!= NULL);
    HBITMAP hOldBmp = memDC.SelectBitmap(memBitmap);

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = (BYTE)(m_nTranspant * 255 / 100);
    bf.AlphaFormat = 0;

    //CFont font;
    //font.CreatePointFont(6, _T("Arial"));
    //HFONT hOldFont = dc.SelectFont(font);
    for (int x = 0; x < m_ColCount; x++)
    //for(int x = nStartCol; x <= nEndCol; x++)
    {
        for (int y = 0; y < m_RowCount; y++)
        //for(int y = nStartRow; y <= nEndRow; y++)
        {
            DrawToolType  nType = (DrawToolType)m_tileGrids[y][x];
            if (nType != dttEmpty)
            {
                memDC.FillSolidRect(0, 0, m_nGridWidth, m_nGridHeight, COLOR_DRAWTOOL_TYPES[nType]);

                CRect rcClickGrid(x * m_nGridWidth, y * m_nGridHeight, 
                    (x + 1) * m_nGridWidth, (y + 1) * m_nGridHeight);
                //API_VERIFY(dc.BitBlt(rcClickGrid.left, rcClickGrid.top, rcClickGrid.Width(), rcClickGrid.Height(),
                //        memDC, 0, 0, SRCCOPY));
                {
                    //CFMMTextDCGuard mmTextDCGuard(dc);
                    API_VERIFY(dc.AlphaBlend(rcClickGrid.left, rcClickGrid.top, rcClickGrid.Width(), rcClickGrid.Height(),
                        memDC, 0, 0, m_nGridWidth, m_nGridHeight, bf));
                }
                //dc.FillSolidRect(rcClickGrid, COLOR_DRAWTOOL_TYPES[nType]);
            }
            //strText.Format(TEXT("%d,%d"), y, x);            
            //dc.DrawText(strText, -1, rcClickGrid, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
        }
    }
    //dc.SelectFont(hOldFont);
    //memDC.SelectBitmap(hOldBmp);
    dc.SelectPen(hOldPen);
}

void CMapMakerView::DoPaint(CDCHandle dc)
{
    BOOL bRet = FALSE;
    FUNCTION_BLOCK_TRACE(50);

    CRect rcClientLogical;
    GetClientRect(rcClientLogical);
    dc.DPtoLP(rcClientLogical);
    rcClientLogical.InflateRect(8, 8);	//fix scroll brush display error

    {
        CRect rcClipBox;
        dc.GetClipBox(rcClipBox);

        CRect rcClipBoxLogical = rcClipBox;
        //dc.DPtoLP(&rcClipBoxLogical);
        //ClientToDoc(&rcClipBoxLogical);
        //FTL::CFScrollZoomMemoryDC memDC (dc.m_hDC, rcClipBox);
		FTL::CFScrollZoomMemoryDC memDC (dc.m_hDC, rcClientLogical);

        PrepareDC(memDC.m_hDC);
        //memDC.SetBrushOrg( -m_ptOffset.x , -m_ptOffset.y);
        memDC.FillSolidRect(rcClientLogical, COLOR_MAP_BACKGROUND);

        if (m_pImage && !m_pImage->IsNull())
        {
            CRect rcImage(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
            if (m_Canvas.IsCanvasChanged(rcImage))
            {
                m_Canvas.Release();
                API_VERIFY(m_Canvas.Create(m_hWnd, rcImage.Width(), rcImage.Height()));

                
                API_VERIFY(m_pImage->Draw(m_Canvas, rcImage));
            }

            memDC.SetStretchBltMode(HALFTONE);
            API_VERIFY(memDC.BitBlt(
                0, 0, //rcClientLogical.left, rcClientLogical.top
                rcImage.Width(), rcImage.Height(), // rcClientLogical.Width(), rcClientLogical.Height(),
                m_Canvas.GetCanvasDC(), 
                0, 0, //rcClientLogical.left, rcClientLogical.top, 
                SRCCOPY));

            _DrawGridLine(memDC.m_hDC, rcClipBoxLogical);
        }

        //memDC.DrawText(TEXT("Fishjam"), -1, rcClientLogical, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}


BOOL CMapMakerView::OnEraseBkgnd(CDCHandle dc)
{
    return TRUE;
}

VOID CMapMakerView::SetGridStatus(CPoint point)
{
    CRect rcDrawTarget = m_rcDrawTarget;
    rcDrawTarget.OffsetRect(-rcDrawTarget.TopLeft());
    CPoint ptLogical = point;
    ClientToDoc(&ptLogical);

    int nRow = ptLogical.y / m_nGridHeight;
    int nCol = ptLogical.x / m_nGridWidth;

    if (nRow < m_RowCount && nCol < m_ColCount)
    {
        CRect rcClickGrid(nCol * m_nGridWidth, nRow * m_nGridHeight, 
            (nCol + 1) * m_nGridWidth, (nRow + 1) * m_nGridHeight);

        if (m_isCapturing)
        {
            m_tileGrids[nRow][nCol] = m_drawToolType;

            DocToClient(&rcClickGrid);
            InvalidateRect(rcClickGrid, FALSE);

            FTLTRACE(TEXT("OnMouseMove, point=(%d,%d), ptLogical=(%d,%d), nRow=%d, nCol=%d, rcClickGrid=%s\n"), 
                point.x, point.y, ptLogical.x, ptLogical.y,
                nRow, nCol,
                CFRectDumpInfo(rcClickGrid).GetConvertedInfo());
        }

        CString strToolTip;
        strToolTip.Format(TEXT("Image:(h=%d,w=%d), Grid(r=%d, c=%d)"), 
            m_pImage->GetHeight(), m_pImage->GetWidth(),
            nRow, nCol);
        _setPromptInfo(strToolTip);
    }
}

void CMapMakerView::_setPromptInfo(const CString& strPrompt)
{
    g_pMainFrame->SetPromptInfo(strPrompt);
}

void CMapMakerView::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetCapture();
    m_isCapturing = TRUE;
    SetGridStatus(point);
}

void CMapMakerView::OnMouseMove(UINT nFlags, CPoint point)
{
    SetGridStatus(point);
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

BOOL CMapMakerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (KEY_DOWN(VK_CONTROL))
    {
        if (m_pImage)
        {
            NextZoom( (BOOL)(zDelta > 0));
        }
        return TRUE;
    }
    SetMsgHandled(FALSE);
    return FALSE;
}

BOOL CMapMakerView::NextZoom(BOOL bBigger)
{
    if (!m_pImage)
    {
        return FALSE;
    }

    float zoomScale = this->GetZoomScale();
    //int oldZoomIndex = m_iFixedZoomIndex;
    int newZoomIndex = 0;
    if (bBigger)
    {
        newZoomIndex = _GetNextFixedZoomIndex(zoomScale, TRUE);
    }
    else
    {
        newZoomIndex = _GetNextFixedZoomIndex(zoomScale, FALSE);
    }
    if (!FTL::IsSameNumber(zoomScale, s_FixedZoomScales[newZoomIndex], 0.01f))
    {
        _SetScrollInfo(s_FixedZoomScales[newZoomIndex]);
        return TRUE;
    }
    return FALSE;
}

int  CMapMakerView::_GetNextFixedZoomIndex(float curZoom, BOOL bBigger)
{
    if (bBigger)
    {
        for (int i = 0; i < _countof(s_FixedZoomScales); ++i)
        {
            if (curZoom < s_FixedZoomScales[i])
            {
                return i;
            }
        }
        return _countof(s_FixedZoomScales)-1;
    }
    else
    {
        for (int i = _countof(s_FixedZoomScales)-1; i >= 0 ; --i)
        {
            if (curZoom > s_FixedZoomScales[i])
            {
                return i;
            }
        }
        return 0;
    }
}



void CMapMakerView::_SetScrollInfo(float newZoomScale, BOOL bUseCenterPoint)
{
	if (m_pImage && !m_pImage->IsNull())
	{
        m_ZoomScale = newZoomScale;
		BOOL bRet = FALSE;
		CRect rcClient;
		API_VERIFY(GetClientRect(&rcClient));

		//save old ptLogical
		CPoint ptMouseDevice(0, 0);
		API_VERIFY(::GetCursorPos(&ptMouseDevice));
		API_VERIFY(ScreenToClient(&ptMouseDevice));

		if (!rcClient.PtInRect(ptMouseDevice) || bUseCenterPoint)
		{
			ptMouseDevice = rcClient.CenterPoint();
		}

		CPoint ptOldLogical = ptMouseDevice;
		ClientToDoc(&ptOldLogical);
		SetZoomScale(newZoomScale);

		//FTLTRACE(TEXT("Zoom=%f, image=[%d,%d], sizeAll=[%d,%d]\n"),
		//	s_FixedZoomScales[m_iFixedZoomIndex], m_pImage->GetWidth(), m_pImage->GetHeight(),
		//	(int)((float)m_pImage->GetWidth() * s_FixedZoomScales[m_iFixedZoomIndex]),
		//	(int)((float)m_pImage->GetHeight() * s_FixedZoomScales[m_iFixedZoomIndex]));

		SetScrollSize(m_pImage->GetWidth(), m_pImage->GetHeight(), TRUE, true);
		CPoint ptNewClient = ptOldLogical;

		DocToClient(&ptNewClient);
		CPoint ptNewOffset = ptNewClient;
		ptNewOffset.Offset(-ptMouseDevice);
		SetScrollOffset(ptNewOffset.x, ptNewOffset.y);

		//_UpdateMousePosInfo(ptMouseDevice);
		//SetCurrentToolType(CalcCurrentToolType());
	}
	else //make scrollbar disappear
	{
		SetZoomScale(s_FixedZoomScales[s_NormalZoomIndex]);
		SetScrollSize(1, 1, FALSE, FALSE);
	}
}

SIZE CMapMakerView::GetImageSize() const
{
    SIZE szImage = {0, 0};
    if (m_pImage)
    {
        szImage.cx = m_pImage->GetWidth();
        szImage.cy = m_pImage->GetHeight();
    }
    return szImage;
}
CAtlString CMapMakerView::GetExportMapString()
{
    CString strExportMap;
    strExportMap.Format(TEXT("int mapData[][] = { \n"));
    for (int i = 0; i < m_RowCount; i++)
    {
        strExportMap.Append(TEXT("\t{ "));
        for (int j = 0; j < m_ColCount - 1; j++)
        {
            strExportMap.AppendFormat(TEXT("%d,"), m_tileGrids[i][j]);
        }
        strExportMap.AppendFormat(TEXT("%d },"), m_tileGrids[i][m_ColCount-1]);

        if (i == m_RowCount - 1)
        {
            strExportMap.Delete(strExportMap.GetLength() - 1); //去掉最后的一个逗号
        }
        strExportMap.Append(TEXT("\r\n"));
    }
    strExportMap.AppendFormat(TEXT("}; \r\n"));
    return strExportMap;
}

BOOL CMapMakerView::SaveMapInfo(const CString& strFilePath)
{
    BOOL bRet = FALSE;
    CFAnsiFile textFile(tfeUnknown);
    API_VERIFY(textFile.Create(strFilePath));
    if (bRet)
    {
        API_VERIFY(textFile.WriteFileHeader());
        textFile.WriteString(GetExportMapString());

        CPath pathMap(m_strImagePath);
        pathMap.RemoveExtension();
        pathMap.m_strPath += STR_MAP_APPENDIX;
        //API_VERIFY(pathMap.AddExtension(STR_MAP_APPENDIX));
        API_VERIFY(SaveTileGrids(pathMap.m_strPath));
    }
    return bRet;
}