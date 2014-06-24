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
	m_bSetTooltip = FALSE;
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
	ON_BN_CLICKED(IDC_BTN_TOOLTIP, &CWindowPage::OnBnClickedBtnTooltip)
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

void CWindowPage::OnBnClickedBtnTooltip()
{
	BOOL bRet = FALSE;
	if (!m_bSetTooltip)
	{
		m_bSetTooltip = TRUE;

		CWnd* pBtnToolTip = GetDlgItem(IDC_BTN_TOOLTIP);
		ASSERT(pBtnToolTip);
		if (pBtnToolTip)
		{
			//m_tooltip.UpdateTipText(m_toolText,this,1);
			m_tooltip.SetMaxTipWidth(500);
			m_tooltip.SetDelayTime(500);
			m_tooltip.SetTipBkColor((0,0,255));//背景色
			m_tooltip.SetTipTextColor((255,0,0));//字体色
			m_tooltip.SetTitle(TTI_INFO,TEXT("ToolTip Demo"));//设置提示框标题

			API_VERIFY(m_tooltip.AddTool(pBtnToolTip, 
				TEXT("http://download.csdn.net/user/fishjam\nAuthor:fishjam@163.com")));
		}
	}
}

BOOL CWindowPage::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE)
	{
		m_tooltip.RelayEvent(pMsg);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CWindowPage::OnInitDialog()
{
	BOOL bRet = FALSE;
	API_VERIFY(CPropertyPage::OnInitDialog());

	API_VERIFY(m_tooltip.CreateEx(this, TTS_ALWAYSTIP | TTS_BALLOON | TTS_CLOSE, WS_EX_LAYERED | WS_EX_TRANSPARENT));

	LONG_PTR newExStyle = ::GetWindowLongPtr(m_tooltip.m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT;
	::SetWindowLongPtr(m_tooltip.m_hWnd, GWL_EXSTYLE, newExStyle);
	API_VERIFY(m_tooltip.SetLayeredWindowAttributes(0, 255, LWA_ALPHA));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
