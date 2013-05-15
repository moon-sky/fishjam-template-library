// GuiPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "GdiPage.h"
#include <afxwin.h>
#include <ftlGdi.h>

// CGdiPage dialog

IMPLEMENT_DYNAMIC(CGdiPage, CPropertyPage)

CGdiPage::CGdiPage()
	: CPropertyPage(CGdiPage::IDD)
{

}

CGdiPage::~CGdiPage()
{
}

void CGdiPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGdiPage, CPropertyPage)
END_MESSAGE_MAP()

BOOL CGdiPage::OnInitDialog()
{
	return TRUE;
}
// CGdiPage 消息处理程序
