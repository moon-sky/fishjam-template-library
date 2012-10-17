// HexEditView.cpp : implementation file
//

#include "stdafx.h"
#include "StorageFile.h"
#include "HexEditView.h"


// CHexEditView

IMPLEMENT_DYNCREATE(CHexEditView, CView)

CHexEditView::CHexEditView()
{
}

CHexEditView::~CHexEditView()
{
}

BEGIN_MESSAGE_MAP(CHexEditView, CView)
END_MESSAGE_MAP()


// CHexEditView drawing

void CHexEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	

    //pDC->DrawText(TEXT("CHexEditView"),-1,NULL,DT_CENTER|DT_VCENTER);

}


// CHexEditView diagnostics

#ifdef _DEBUG
void CHexEditView::AssertValid() const
{
	CView::AssertValid();
}

void CHexEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//CStorageFileDoc* CHexEditView::GetDocument() const // 非调试版本是内联的
//{
//    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStorageFileDoc)));
//    return (CStorageFileDoc*)m_pDocument;
//}

#endif //_DEBUG


// CHexEditView message handlers
