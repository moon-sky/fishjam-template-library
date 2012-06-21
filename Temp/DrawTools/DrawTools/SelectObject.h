#pragma once

#include "DrawObject.h"

typedef struct stu_BitMapResInfo
{
	int     nRefCount;
	HBITMAP hBitMap;
	
	stu_BitMapResInfo(HBITMAP hBp)
	{
		nRefCount = 0;
		hBitMap   = hBp;
	}
}BITMAPRESINFO, *LPBITMAPRESINFO;

class CImageResourceManager
{
public:
	static CImageResourceManager* Instance();
	BOOL CreateImage(CImage* pImage, int nWidth, int nHeight, int nBPP, DWORD dwFlags);
	void ReleaseImage(CImage* pImage);
	BOOL CopyImage(CImage* pSrc, CImage* pDst);
private:
	CImageResourceManager();
	~CImageResourceManager();

	std::vector<BITMAPRESINFO> m_arBitMap;
};

class CSelectObject : public CDrawObject
{
public:
	CSelectObject(IDrawCanvas* pDrawCanvas, const CRect& position, DrawObjectType objType, const DRAWOBJBASEINFO& stDrawObjInfo);
	virtual ~CSelectObject();
	// Implementation
public:
	virtual void Draw(HDC hDC, BOOL bOriginal);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObject* Clone();
	virtual void DrawTracker(HDC hDC, TrackerState state, BOOL bDrawSelectTool);
	virtual void EndMoveHandle();
	virtual BOOL CheckAvailObject();
	virtual BOOL UpdateDrawInfo(const DRAWOBJBASEINFO& stDrawObjInfo);

//private:
//	CRect m_rcSrc; //logical
//	BOOL  m_bClipImage;
//
//	CImage m_imgClipDst;
};
