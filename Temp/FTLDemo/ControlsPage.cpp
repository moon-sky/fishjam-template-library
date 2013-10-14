// ControlsPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "ControlsPage.h"


// CControlsPage dialog

IMPLEMENT_DYNAMIC(CControlsPage, CPropertyPage)

CControlsPage::CControlsPage()
	: CPropertyPage(CControlsPage::IDD)
{

}

CControlsPage::~CControlsPage()
{
}

void CControlsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CControlsPage, CPropertyPage)
END_MESSAGE_MAP()


// CControlsPage message handlers
