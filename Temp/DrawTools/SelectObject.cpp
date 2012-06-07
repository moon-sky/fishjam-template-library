#include "StdAfx.h"
#include "SelectObject.h"
#include <ftlGdi.h>


CImageResourceManager::CImageResourceManager()
{

}

CImageResourceManager::~CImageResourceManager()
{

}

CImageResourceManager* CImageResourceManager::Instance()
{
	static CImageResourceManager ImageResourceManager;
	return &ImageResourceManager;
}

BOOL CImageResourceManager::CreateImage(CImage* pImage, int nWidth, int nHeight, int nBPP, DWORD dwFlags)
{
	if (pImage != NULL && pImage->Create(nWidth, nHeight, nBPP, dwFlags))
	{
		BITMAPRESINFO stBitMapInfo((HBITMAP)(*pImage));
		stBitMapInfo.nRefCount++;
		m_arBitMap.push_back(stBitMapInfo);
		return TRUE;
	}
	return FALSE;
}

void CImageResourceManager::ReleaseImage(CImage* pImage)
{
	std::vector<BITMAPRESINFO>::iterator it = m_arBitMap.begin();
	while(it != m_arBitMap.end())
	{
		if (it->hBitMap == (HBITMAP)(*pImage))
		{
			pImage->Detach();
			it->nRefCount--;
			if (it->nRefCount == 0)
			{
				::DeleteObject(it->hBitMap);
				m_arBitMap.erase(it);
			}
			return;
		}
		it++;
	}
}

BOOL CImageResourceManager::CopyImage(CImage* pSrc, CImage* pDst)
{
	if (pSrc == NULL || pDst == NULL)
	{
		return FALSE;
	}
	std::vector<BITMAPRESINFO>::iterator it = m_arBitMap.begin();
	while(it != m_arBitMap.end())
	{
		if (it->hBitMap == (HBITMAP)(*pSrc))
		{
			pDst->Attach(it->hBitMap);
			it->nRefCount++;
			return TRUE;
		}
		it++;
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////                CSelectObject       /////////////////////////////////

CSelectObject::CSelectObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo)
: CDrawObject(pDrawCanvas, position, objType, stDrawObjInfo)
{
	//m_rcSrc = position;

	m_bBrush = FALSE;
	m_logpen.lopnStyle = PS_DOT;
	m_logpen.lopnColor = RGB(0, 0, 0);
	m_objType = dotSelectRect;

	//m_bClipImage = FALSE;

}

CSelectObject::~CSelectObject()
{
	//if (!m_imgClipDst.IsNull())
	//{
	//	//m_imgClipDst.Destroy();
	//	CImageResourceManager::Instance()->ReleaseImage(&m_imgClipDst);
	//}
}

void CSelectObject::Draw(HDC hDC, BOOL bOriginal)
{
	//if (m_bClipImage)
	//{
	//	COLORREF clBG = RGB(255, 0, 0);
	//	CDCHandle dc(hDC);
	//	dc.FillSolidRect(&m_rcSrc, clBG);

	//	if (!m_imgClipDst.IsNull() && m_position.Width() != 0 && m_position.Height() != 0)
	//	{
	//		m_imgClipDst.Draw(hDC, m_position.left, m_position.top, abs(m_position.Width()), abs(m_position.Height()));
	//	}
	//}
}

// rect must be in logical coordinates
BOOL CSelectObject::Intersects(const CRect& rect)
{
	CRect rectT = rect;
	rectT.NormalizeRect();

	CRect fixed = m_position;
	fixed.NormalizeRect();
	if ((rectT & fixed).IsRectEmpty())
	{
		return FALSE;
	}
	return TRUE;
}

CDrawObject* CSelectObject::Clone()
{
	DRAWOBJBASEINFO stDrawInfo;
	stDrawInfo.logbrush = m_logbrush;
	stDrawInfo.logpen   = m_logpen;
	CSelectObject* pClone = new CSelectObject(m_pDrawCanvas, m_position, m_objType, stDrawInfo);
	pClone->m_bPen = m_bPen;
	//pClone->m_logpen = m_logpen;
	pClone->m_bBrush = m_bBrush;
	//pClone->m_logbrush = m_logbrush;
	//pClone->m_rcSrc    = m_rcSrc;
	//pClone->m_bClipImage = m_bClipImage;
	//if (m_bClipImage)
	//{
	//	CImageResourceManager::Instance()->CopyImage(&m_imgClipDst, &pClone->m_imgClipDst);
	//}
	
	return pClone;
}


void CSelectObject::DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool)
{
	BOOL bRet = FALSE;
	CDCHandle dc(hDC);

	CBrush brush;
	CPen pen;
	API_VERIFY(NULL != brush.CreateBrushIndirect(&m_logbrush));
	API_VERIFY(NULL != pen.CreatePenIndirect(&m_logpen));

	CBrushHandle pOldBrush;
	CPenHandle pOldPen;

	if (m_bBrush)
	{
		pOldBrush = dc.SelectBrush(brush);
	}
	else
	{
		pOldBrush = dc.SelectStockBrush(NULL_BRUSH);
	}

	if (m_bPen)
	{
		pOldPen = dc.SelectPen(pen);
	}
	else
	{
		pOldPen = dc.SelectStockPen(NULL_PEN);
	}

	//ATLTRACE(_T("DrawTracker top %d btn %d left %d right %d \n"), m_position.top, m_position.bottom, m_position.left, m_position.right);
	//draw on device units
	CRect rect = m_position;
	//ATLTRACE(_T("DrawTracker DocToClient top %d btn %d left %d right %d \n"), m_position.top, m_position.bottom, m_position.left, m_position.right);
	m_pDrawCanvas->DocToClient(&rect);
	//ATLTRACE(_T("DrawTracker DocToClient top %d btn %d left %d right %d \n"), rect.top, rect.bottom, rect.left, rect.right);

	HBRUSH hOldBrush = dc.SelectStockBrush(NULL_BRUSH);
	dc.Rectangle(rect);
	dc.SelectBrush(hOldBrush);
	dc.SelectPen(pOldPen);
	CDrawObject::DrawTracker(hDC, state, bDrawSelectTool);
}	


void CSelectObject::EndMoveHandle()
{
	//if (!m_bClipImage)
	//{
	//	m_rcSrc = m_position;
	//	m_rcSrc.NormalizeRect();
	//	//if(m_imgClipDst.Create(m_rcSrc.Width(), m_rcSrc.Height(), 32, CImage::excludePNG))
	//	if (CImageResourceManager::Instance()->CreateImage(&m_imgClipDst, 
	//		m_rcSrc.Width(), m_rcSrc.Height(), 32, CImage::excludePNG))
	//	{
	//		m_pDrawCanvas->GetImageByRect(m_rcSrc, m_imgClipDst);
	//	}
	//	m_bClipImage = TRUE;
	//}
}

BOOL CSelectObject::CheckAvailObject()
{
	//if (m_position == m_rcSrc)
	//{
	//	return FALSE;
	//}
	return FALSE;
}

void CSelectObject::UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo)
{

}