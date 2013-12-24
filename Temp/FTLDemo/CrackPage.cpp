// CrackPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "CrackPage.h"
#include "ftlCrack.h"

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
    DDX_Text(pDX, IDC_STATIC_CODE_SIGN_INFO, m_strCodeSignInfo);

	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCrackPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_CRACK, &CCrackPage::OnBnClickedBtnCrack)
    ON_BN_CLICKED(IDC_BTN_CRACK_GET_SIGN_INFO, &CCrackPage::OnBnClickedBtnCrackGetSignInfo)
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

void CCrackPage::OnBnClickedBtnCrackGetSignInfo()
{
    BOOL bRet = FALSE;
    CString strFilter = _T("Execute Files(*.exe;*.dll;*.sys)|*.exe;*.dll;*.sys|All Files(*.*)|*.*");
    CFileDialog dlg(TRUE, NULL, NULL, 0, strFilter, this);
    if (dlg.DoModal() == IDOK)
    {
        FTL::CFCodeSignInfo codeSignInfo;
        bRet = codeSignInfo.OpenCodeSignByFile(dlg.GetPathName());
        if (bRet)
        {
            CString strInfoTime;
            SYSTEMTIME st = {0};
            API_VERIFY(codeSignInfo.GetDateOfTimeStamp(&st));
            if (bRet)
            {
                strInfoTime.Format(TEXT("%04d%02d%02d%02d%02d%02d"),
                    st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            }
            m_strCodeSignInfo.Format(TEXT("%s:\nTime=%s\nProgramName=%s\nPublisher=%s\nMoreInfoLine=%s\n") ,
                dlg.GetFileName(),
                strInfoTime,
                codeSignInfo.GetProgramName(),
                codeSignInfo.GetPublisherLink(),
                codeSignInfo.GetMoreInfoLink()
                );
        }
        else
        {
            m_strCodeSignInfo.Format(TEXT("Get SignInfo For %s Fail\nReason=0x%x\n"),
                dlg.GetFileName(), GetLastError());
        }
        UpdateData(FALSE);
    }
}