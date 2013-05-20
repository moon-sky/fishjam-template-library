#include "stdafx.h"
#include "FTLDemo.h"
#include "FTLDemoSheet.h"

#include "BasePage.h"
#include "CrackPage.h"
#include "CrashHandlerPage.h"
#include "GdiPage.h"
#include "GdiPlusPage.h"
#include "GuiPage.h"
#include "NetPage.h"
#include "NLSPage.h"
//#include "PipelinePage.h"
#include "MemPage.h"
#include "ShellPage.h"
#include "SystemPage.h"
#include "ThreadPage.h"
#include "ThreadPoolPage.h"
#include "VistaPage.h"
#include "WindowPage.h"

// CFTLDemoSheet

IMPLEMENT_DYNAMIC(CFTLDemoSheet, CPropertySheet)

CFTLDemoSheet::CFTLDemoSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    InitSheet();
}

CFTLDemoSheet::CFTLDemoSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    InitSheet();
}

CFTLDemoSheet::~CFTLDemoSheet()
{
}

BOOL CFTLDemoSheet::InitSheet()
{
    BOOL bRet = TRUE;

    //this->AddPage(new CBasePage());
    //this->AddPage(new CCrackPage());
    //this->AddPage(new CCrashHandlerPage());
    //this->AddPage(new CGdiPage());
    //this->AddPage(new CGdiPlusPage());
    //this->AddPage(new CGuiPage());
    //this->AddPage(new CMemPage());

    //this->AddPage(new CPipelinePage());

	//this->AddPage(new CNetPage());
	//this->AddPage(new CNLSPage());
 //   this->AddPage(new CShellPage());
 //   this->AddPage(new CSystemPage());
 //   this->AddPage(new CThreadPage());
	this->AddPage(new CThreadPoolPage());
 //   this->AddPage(new CVistaPage());
 //   this->AddPage(new CWindowPage());
    return bRet;
}

BOOL CFTLDemoSheet::UninitSheet()
{
	BOOL bRet = TRUE;
	for (int i = 0; i < m_pages.GetSize(); i++)
	{
		CPropertyPage* pPage = GetPage(i);
		pPage->DestroyWindow();
		delete pPage;
	}
	m_pages.RemoveAll();
	return bRet;
}

BEGIN_MESSAGE_MAP(CFTLDemoSheet, CPropertySheet)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CFTLDemoSheet 消息处理程序

void CFTLDemoSheet::OnDestroy()
{
	UninitSheet();
	CPropertySheet::OnDestroy();

	// TODO: Add your message handler code here
}
