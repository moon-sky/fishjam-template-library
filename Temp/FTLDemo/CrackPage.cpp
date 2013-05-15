// CrackPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "CrackPage.h"


// CCrackPage dialog

IMPLEMENT_DYNAMIC(CCrackPage, CPropertyPage)

CCrackPage::CCrackPage()
	: CPropertyPage(CCrackPage::IDD)
    , m_strPathOfAppWithMemOverFlow(TEXT(""))
{

}

CCrackPage::~CCrackPage()
{
}

void CCrackPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCrackPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_CRACK, &CCrackPage::OnBnClickedBtnCrack)
END_MESSAGE_MAP()


// CCrackPage message handlers

void CCrackPage::OnBnClickedBtnCrack()
{
    if (m_strPathOfAppWithMemOverFlow.IsEmpty())
    {
        CFileDialog dlg(TRUE, NULL, TEXT("AppWithMemOverFlow.exe"),0,TEXT("Exe"),
            this);
        if (dlg.DoModal() == IDOK)
        {
            m_strPathOfAppWithMemOverFlow = dlg.GetPathName();
        }
    }

    if (!m_strPathOfAppWithMemOverFlow.IsEmpty())
    {
        //CreateProcess()
    }
}
