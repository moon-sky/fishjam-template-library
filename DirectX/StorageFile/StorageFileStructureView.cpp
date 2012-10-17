// StorageFileStructureView.cpp : CStorageFileStructureView 类的实现
//

#include "stdafx.h"
#include "StorageFile.h"

#include "StorageFileDoc.h"
#include "StorageFileStructureView.h"
#include ".\storagefilestructureview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStorageFileStructureView

IMPLEMENT_DYNCREATE(CStorageFileStructureView, CTreeView)

BEGIN_MESSAGE_MAP(CStorageFileStructureView, CTreeView)
END_MESSAGE_MAP()

// CStorageFileStructureView 构造/析构

CStorageFileStructureView::CStorageFileStructureView()
{
    m_bSubclassed = FALSE;
}

CStorageFileStructureView::~CStorageFileStructureView()
{
}

BOOL CStorageFileStructureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CTreeView::PreCreateWindow(cs);
}

// CStorageFileStructureView 绘制

void CStorageFileStructureView::OnDraw(CDC* pDC)
{
	CStorageFileDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    //pDC->DrawText(TEXT("CStorageFileStructureView"),-1,NULL,DT_CENTER|DT_VCENTER);
	// TODO: 在此处为本机数据添加绘制代码
}


// CStorageFileStructureView 诊断

#ifdef _DEBUG
void CStorageFileStructureView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CStorageFileStructureView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CStorageFileDoc* CStorageFileStructureView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStorageFileDoc)));
	return (CStorageFileDoc*)m_pDocument;
}
#endif //_DEBUG


// CStorageFileStructureView 消息处理程序

void CStorageFileStructureView::OnInitialUpdate()
{
    BOOL bRet = FALSE;
    CTreeView::OnInitialUpdate();

    //if(TRUE == GetTreeCtrl().IsWindowVisible()) 
    //{ 
    //    Detach(); 
    //    API_VERIFY(m_StorageTreeCtrl.SubclassWindow(GetTreeCtrl().GetSafeHwnd())); 
    //    //InitTree(); 
    //} 

    //CTreeCtrl& tree = GetTreeCtrl(); // 得到CTreeCtrl对象的引用V4Eitug
    //HWND hwndTreeCtrl = tree.m_hWnd; // 记录下它的句柄，下面马上会用到
    //tree.UnsubclassWindow(); // 因为这个窗口已经是子类化过的（用Spy++可以看出来），要先进行UnsubclassWindow
    //m_StorageTreeCtrl.SubclassWindow(hwndTreeCtrl); // 现在实现我的子类化
    //m_bSubclassed = TRUE; // 设置标志
    //// 下面这句很重要，否则很多API调用会认为CMyTreeView内部的控件窗口句柄无效
    //m_hWnd = m_StorageTreeCtrl.m_hWnd; // 修正CMyTreeView的m_hWnd成员变量

}


