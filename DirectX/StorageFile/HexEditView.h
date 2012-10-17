#pragma once


// CHexEditView view
#include "StorageFileDoc.h"

class CHexEditView : public CView
{
	DECLARE_DYNCREATE(CHexEditView)

protected:
	CHexEditView();           // protected constructor used by dynamic creation
	virtual ~CHexEditView();

//    // 属性
//public:
//    CStorageFileDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};

//#ifndef _DEBUG  // StorageFileStructureView.cpp 的调试版本
//inline CStorageFileDoc* CHexEditView::GetDocument() const
//{ return reinterpret_cast<CStorageFileDoc*>(m_pDocument); }
//#endif

