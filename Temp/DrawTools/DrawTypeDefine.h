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
};


enum SelectMode
{
	smNone = -1,
	//smNetSelect,
	//smNetSelectSize,
	smMove,
	smSize
};

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
};


typedef struct stu_DrawObjBaseInfo
{
	LOGPEN			logpen;
	LOGBRUSH		logbrush;
	stu_DrawObjBaseInfo()
	{
		logpen.lopnStyle = PS_INSIDEFRAME;
		logpen.lopnWidth.x = 1;
		logpen.lopnWidth.y = 1;
		logpen.lopnColor = RGB(128,   0,  64);

		logbrush.lbStyle = BS_SOLID;
		logbrush.lbColor = RGB(255, 255, 184);
		logbrush.lbHatch = HS_HORIZONTAL;
	}

	stu_DrawObjBaseInfo& operator = (const stu_DrawObjBaseInfo& other)
	{
		logpen   = other.logpen;
		logbrush = other.logbrush;
		return *this;
	}
}DRAWOBJBASEINFO, *LPDRAWOBJBASEINFO;