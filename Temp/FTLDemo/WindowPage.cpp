// WindowPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "WindowPage.h"
#include <ftlwindow.h>

// CWindowPage dialog

IMPLEMENT_DYNAMIC(CWindowPage, CPropertyPage)

CWindowPage::CWindowPage()
	: CPropertyPage(CWindowPage::IDD)
{

}

CWindowPage::~CWindowPage()
{
}

void CWindowPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWindowPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_MESSAGE_INFO, &CWindowPage::OnBnClickedBtnMessageInfo)
	ON_BN_CLICKED(IDC_BTN_REGISTER_MESSAGE_INFO, &CWindowPage::OnBnClickedBtnRegisterMessageInfo)
END_MESSAGE_MAP()


// CWindowPage message handlers

void CWindowPage::OnBnClickedBtnMessageInfo()
{
    CFMessageInfo msgInfo(WM_SIZE, 0, 0);
    LPCTSTR pszMessageInfo = msgInfo.GetConvertedInfo();
    AfxMessageBox(pszMessageInfo);
}

void CWindowPage::OnBnClickedBtnRegisterMessageInfo()
{
	UINT nHtmlGetObjectMsg = ::RegisterWindowMessage( TEXT("WM_HTML_GETOBJECT") );
	CFMessageInfo msgInfo(nHtmlGetObjectMsg, 0, 0);
	LPCTSTR pszMessageInfo = msgInfo.GetConvertedInfo();
	AfxMessageBox(pszMessageInfo);
}
