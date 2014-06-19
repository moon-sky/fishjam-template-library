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
    m_bHideTaskBar = TRUE;
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
    ON_BN_CLICKED(IDC_BTN_WINDOW_HIDE_SHOW_TASKBAR, &CWindowPage::OnBnClickedBtnHideShowTaskBar)
    ON_BN_CLICKED(IDC_BTN_WINDOW_MENU_INFO_DUMP, &CWindowPage::OnBnClickedBtnMenuInfoDump)
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

void CWindowPage::OnBnClickedBtnHideShowTaskBar()
{
    CFWinUtil::HideTaskBar(m_bHideTaskBar);
    m_bHideTaskBar = !m_bHideTaskBar;
}

void CWindowPage::OnBnClickedBtnMenuInfoDump()
{
    BOOL bRet = FALSE;
    CMenu menu;
    API_VERIFY(menu.LoadMenu(IDR_MENU_GDI_PLUS_TEST));

    CFMenuUtil::DumpMenuInfo(menu.GetSafeHmenu(), TRUE);
}
