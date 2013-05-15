// VistaPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "VistaPage.h"


// CVistaPage dialog

IMPLEMENT_DYNAMIC(CVistaPage, CPropertyPage)

CVistaPage::CVistaPage()
	: CPropertyPage(CVistaPage::IDD)
{

}

CVistaPage::~CVistaPage()
{
}

void CVistaPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVistaPage, CPropertyPage)
END_MESSAGE_MAP()


// CVistaPage message handlers
BOOL CVistaPage::OnInitDialog()
{
	return TRUE;
}
