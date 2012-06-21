#pragma once

#define INVALID_POS_X       (-1)
#define INVALID_POS_Y       (-1)

#define TRACK_MARGIN		(3)

#include <list>

class CDrawObject;
class CDrawRect;
class CDrawTool;
class CSelectTool;

typedef std::list<CDrawObject*> DrawObjectList;
typedef std::vector<CDrawTool*> DrawToolList;

enum ToolType
{
	ttNone = -1,
	ttMove,
	ttSelection,
	ttLine,
	ttLineArrow,
	ttRect,
	ttRoundRect,
	ttEllipse,
	ttBalloon,
	ttArrow,
	ttPoly,
	ttFreeObject,
	ttText,
	ttImage,
};

enum DrawObjectType
{ 
	dotNone = -1,
	dotSelectRect, 
	dotLine,
	dotLineArrow,
	dotRect, 
	dotRoundRect,
	dotEllipse,
	dotArrow,
	dotPoly,
	dotFreeObject,
	dotText,
	dotBalloon,
	dotImage,
};


enum SelectMode
{
	smNone = -1,
	//smNetSelect,
	//smNetSelectSize,
	smMove,
	smSize
};

#define DRAWOBJECT_BASE_PENCLR     0x00000001
#define DRAWOBJECT_BASE_PENWIDTH   0x00000002
#define DRAWOBJECT_BASE_FONTNAME   0x00000004
#define DRAWOBJECT_BASE_FONTCLR    0x00000008
#define DRAWOBJECT_BASE_FONTSIZE   0x0000000f
#define DRAWOBJECT_BASE_BPEN       0x00000010
#define DRAWOBJECT_BASE_BBRUSH     0x00000020
#define DRAWOBJECT_BASE_IMAGEFILE  0x00000040
#define DRAWOBJECT_BASE_BRUSHCLR   0x00000080

typedef struct stu_DrawObjBaseInfo
{
	LOGPEN			logpen;
	LOGBRUSH		logbrush;
	CString         strFontName;
	COLORREF		clrFontFore;
	int             nFontSize;

	CString         strImageFileName;

	BOOL            bPen;
	BOOL            bBrush;

	DWORD_PTR       dwDrawMask;
	stu_DrawObjBaseInfo()
	{
		logpen.lopnStyle = PS_INSIDEFRAME;
		logpen.lopnWidth.x = 1;
		logpen.lopnWidth.y = 1;
		logpen.lopnColor = RGB(255, 0,  0);

		logbrush.lbStyle = BS_SOLID;
		logbrush.lbColor = RGB(255, 0, 0);
		logbrush.lbHatch = HS_HORIZONTAL;

		strFontName =  _T("Arial");
		nFontSize   = 18;
		clrFontFore = RGB(255, 0, 0);

		strImageFileName = _T("");

		bPen   = TRUE;
		bBrush = FALSE;

		dwDrawMask = 0;
	}

	stu_DrawObjBaseInfo& operator = (const stu_DrawObjBaseInfo& other)
	{
		if (other.dwDrawMask & DRAWOBJECT_BASE_PENCLR)
		{
			logpen.lopnColor = other.logpen.lopnColor;
		}

		if (other.dwDrawMask & DRAWOBJECT_BASE_BRUSHCLR)
		{
			logbrush.lbColor = other.logbrush.lbColor;
		}

		if (other.dwDrawMask & DRAWOBJECT_BASE_PENWIDTH)
		{
			logpen.lopnWidth.x = other.logpen.lopnWidth.x;
			logpen.lopnWidth.y = other.logpen.lopnWidth.y;
		}
		

		if (other.dwDrawMask & DRAWOBJECT_BASE_FONTNAME)
		{
			strFontName = other.strFontName;
		}

		if (other.dwDrawMask & DRAWOBJECT_BASE_FONTCLR)
		{
			clrFontFore = other.clrFontFore;
		}

		if (other.dwDrawMask & DRAWOBJECT_BASE_FONTSIZE)
		{
			nFontSize   = other.nFontSize;
		}

		if (other.dwDrawMask & DRAWOBJECT_BASE_BPEN)
		{
			bPen   = other.bPen;
		}
		
		if (other.dwDrawMask & DRAWOBJECT_BASE_BBRUSH)
		{
			bBrush = other.bBrush;
		}
		
		if (other.dwDrawMask & DRAWOBJECT_BASE_IMAGEFILE)
		{
			strImageFileName = other.strImageFileName;
		}
		return *this;
	}
}DRAWOBJBASEINFO, *LPDRAWOBJBASEINFO;

class IDrawCanvas
{
public:
	~IDrawCanvas() {}
public:
	virtual CPoint GetMouseDownLogicalPoint() = 0;
	virtual void SetMouseDownLogicalPoint(const CPoint& ptLast) = 0;
	virtual CPoint GetMouseLastLogicalPoint() = 0;
	virtual void SetMouseLastLogicalPoint(const CPoint& ptLast) = 0;
	//virtual BOOL IsNearByPoint(const CPoint& pt1, const CPoint& pt2, int Diff);

	//virtual CSelectTool* GetSelectTool() = 0;
	virtual ToolType CalcCurrentToolType() = 0;
	virtual ToolType   GetCurrentToolType() = 0;
	virtual CDrawTool*  GetCurrentTool() = 0;
	virtual VOID SetCurrentToolType(ToolType nToolType) = 0;
	virtual CDrawTool*  FindTool(ToolType nToolType) = 0;
	virtual SelectMode GetCurrentSelectMode() = 0;
	virtual VOID SetCurrentSelectMode(SelectMode selectMode) = 0;
	virtual int GetDragHandle() = 0;
	virtual VOID SetDragHandle(int nDragHandle) = 0;


	//virtual BOOL CreateSelectRect(const CRect& rcPosition, BOOL bSelect = TRUE) = 0;
	//virtual BOOL ReleaseSelectRect() = 0;
	virtual CDrawObject* GetSelectRect() = 0;

	virtual void Add(CDrawObject* pObj) = 0;
	virtual void Remove(CDrawObject* pObj, BOOL bPaint = FALSE) = 0;
	virtual void SetActive(CDrawObject* pObj, BOOL bActive = TRUE) = 0;
	virtual void InvalObject(CDrawObject* pObj) = 0;

	virtual CPoint GetOffset() = 0;
	virtual CRect GetDrawTarget() = 0;

	virtual void PrepareDC(CDCHandle dc) = 0;
	virtual void ClientToDoc(CPoint* pPoint) = 0;
	virtual void ClientToDoc(CRect* pRect) = 0;
	virtual void DocToClient(CPoint* pPoint) = 0;
	virtual void DocToClient(CRect* pRect) = 0;

	virtual BOOL IsSelected(const CDrawObject* pDocItem) const = 0;
	virtual void Select(CDrawObject* pObj, BOOL bAdd = FALSE) = 0;
	virtual void Deselect(CDrawObject* pObj) = 0;
	virtual void SelectWithinRect(CRect rect, BOOL bAdd) = 0;
	virtual BOOL IsCapture() = 0;
	virtual void BeginCapture() = 0;
	virtual void EndCapture(BOOL bBackupData = TRUE) = 0;
	virtual HWND GetHWnd() = 0;
	//virtual void UpdateAllViews(CDrawCanvas* pSender) = 0;
	virtual CDrawObject* ObjectAt(const CPoint& point) = 0;
	virtual void CloneSelection() = 0;
	virtual DrawObjectList& GetSelection() = 0;

	virtual void SetCurrentOffsetPoint(LPPOINT lpPoint) = 0;
	virtual void GetCurrentOffsetPoint(LPPOINT lpPoint) = 0;

	//the rcSrc is logical
	virtual BOOL GetImageByRect(const CRect& rcSrc, CImage& Image) = 0;

	virtual BOOL BackupDrawObjectData(LPCTSTR strName) = 0;
	virtual void SetDrawObjectBaseInfo(const DRAWOBJBASEINFO& stDrawInfo, BOOL bPaintObject = FALSE) = 0;
	virtual void NotifyDrawObjectBaseInfo(const DRAWOBJBASEINFO& stDrawInfo) = 0;
};

