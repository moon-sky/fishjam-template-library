// RegPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "RegPage.h"
#include "ftlFile.h"
#include "ftlReg.h"

// CRegPage dialog

IMPLEMENT_DYNAMIC(CRegPage, CPropertyPage)

CRegPage::CRegPage()
	: CPropertyPage(CRegPage::IDD)
{

}

CRegPage::~CRegPage()
{
}

void CRegPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_REG_EXPORT, &CRegPage::OnBnClickedBtnRegExport)
END_MESSAGE_MAP()


// CRegPage message handlers

void CRegPage::OnBnClickedBtnRegExport()
{
    BOOL bRet = FALSE;
    LONG lRet = ERROR_SUCCESS;

    //API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(GetCurrentProcess(), SE_BACKUP_NAME, TRUE));

    //HKEY hkResult = NULL;
    //REG_VERIFY(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\WinRAR\\Capabilities\\FileAssociations"), 
    //    0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, 
    //    &hkResult));
    //if (lRet == ERROR_SUCCESS)
    //{
    //    REG_VERIFY(RegSaveKeyEx(hkResult, TEXT("D:\\RegSaveKey4.reg"), NULL, REG_NO_COMPRESSION));
    //}

    //API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(GetCurrentProcess(), SE_BACKUP_NAME, FALSE));
    //return;
    CFUnicodeFile   regFile(tfeUnicode);
    API_VERIFY(regFile.Create(TEXT("D:\\test.reg")));
    API_VERIFY(regFile.WriteFileHeader());
    API_VERIFY(regFile.WriteString(TEXT("Windows Registry Editor Version 5.00\r\n")));

    REG_VERIFY(FTL::CFRegUtil::ExportRegValueToFile(TEXT("HKEY_LOCAL_MACHINE\\SOFTWARE"), 
        _T("*"), 
        &regFile, 
        WRITE_PATH_AUTO | WRITE_SUB_KEY, KEY_READ | KEY_WOW64_64KEY));

    regFile.Close();
}
