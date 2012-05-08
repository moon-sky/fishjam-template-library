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
typedef std::list<CDrawTool*> DrawToolList;

enum ToolType
{
	ttNone = -1,
	ttSelection,
	ttLine,
	ttRect,
	ttRoundRect,
	ttEllipse,
	ttPoly,
	ttText,
};

enum DrawObjectType
{ 
	dotNone = -1,
	dotSelectRect, 
	dotLine,
	dotRect, 
	dotRoundRect,
	dotEllipse,
	dotPoly,
	dotText,
};


enum SelectMode
{
	smNone = -1,
	smNetSelect,
	smNetSelectSize,
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

	virtual CSelectTool* GetSelectTool() = 0;
	virtual ToolType   GetCurrentToolType() = 0;
	virtual CDrawTool*  GetCurrentTool() = 0;
	virtual VOID SetCurrentToolType(ToolType nToolType) = 0;
	virtual CDrawTool*  FindTool(ToolType nToolType) = 0;
	virtual SelectMode GetCurrentSelectMode() = 0;
	virtual VOID SetCurrentSelectMode(SelectMode selectMode) = 0;
	virtual int GetDragHandle() = 0;
	virtual VOID SetDragHandle(int nDragHandle) = 0;


	virtual BOOL CreateSelectRect(const CRect& rcPosition, BOOL bSelect = TRUE) = 0;
	virtual BOOL ReleaseSelectRect() = 0;
	virtual CDrawRect* GetSelectRect() = 0;

	virtual void Add(CDrawObject* pObj) = 0;
	virtual void Remove(CDrawObject* pObj) = 0;
	virtual void InvalObject(CDrawObject* pObj) = 0;

	virtual CPoint GetOffset() = 0;
	virtual CRect GetDrawTarget() = 0;

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
	virtual void EndCapture() = 0;
	virtual HWND GetHWnd() = 0;
	//virtual void UpdateAllViews(CDrawCanvas* pSender) = 0;
	virtual CDrawObject* ObjectAt(const CPoint& point) = 0;
	virtual void CloneSelection() = 0;
	virtual DrawObjectList& GetSelection() = 0;
	virtual void DeleteSelectObjects() = 0;
};
