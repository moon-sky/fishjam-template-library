// StorageFileStructureView.h : CStorageFileStructureView 类的接口
//


#pragma once
#include "StorageFileDoc.h"
#include "StorageTreeCtrl.h"

class CStorageFileStructureView : public CTreeView
{
protected: // 仅从序列化创建
	CStorageFileStructureView();
	DECLARE_DYNCREATE(CStorageFileStructureView)

// 属性
public:
	CStorageFileDoc* GetDocument() const;

// 操作
public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CStorageFileStructureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CStorageTreeCtrl    m_StorageTreeCtrl;
    BOOL                m_bSubclassed;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    //CTreeCtrl& GetTreeCtrl() const
    //{
    //    if (m_bSubclassed) 
    //        return (CTreeCtrl&)m_StorageTreeCtrl; // 若子类化了，返回新的引用V4Eitug
    //    else 
    //        return CTreeView::GetTreeCtrl(); // 若未子类化，调用默认处理V4Eitug
    //}
};

#ifndef _DEBUG  // StorageFileStructureView.cpp 的调试版本
inline CStorageFileDoc* CStorageFileStructureView::GetDocument() const
   { return reinterpret_cast<CStorageFileDoc*>(m_pDocument); }
#endif

