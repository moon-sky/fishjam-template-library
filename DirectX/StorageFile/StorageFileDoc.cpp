// StorageFileDoc.cpp :  CStorageFileDoc 类的实现
//

#include "stdafx.h"
#include "StorageFile.h"

#include "StorageFileDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStorageFileDoc

IMPLEMENT_DYNCREATE(CStorageFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CStorageFileDoc, CDocument)
END_MESSAGE_MAP()


// CStorageFileDoc 构造/析构

CStorageFileDoc::CStorageFileDoc()
{
	// TODO: 在此添加一次性构造代码

}

CStorageFileDoc::~CStorageFileDoc()
{
}

BOOL CStorageFileDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CStorageFileDoc 序列化

void CStorageFileDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CStorageFileDoc 诊断

#ifdef _DEBUG
void CStorageFileDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStorageFileDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStorageFileDoc 命令
