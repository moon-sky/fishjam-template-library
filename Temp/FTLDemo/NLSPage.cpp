// NLSPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "NLSPage.h"

// CNLSPage 对话框

IMPLEMENT_DYNAMIC(CNLSPage, CPropertyPage)

CNLSPage::CNLSPage()
	: CPropertyPage(CNLSPage::IDD)
{
}

CNLSPage::~CNLSPage()
{

}

void CNLSPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNLSPage, CPropertyPage)
END_MESSAGE_MAP()


// CNLSPage 消息处理程序

