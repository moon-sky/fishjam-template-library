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
#if 0
    //CFUnicodeFile   regFile(tfeUnicode);
    //API_VERIFY(regFile.Create(TEXT("D:\\test.reg")));
    //API_VERIFY(regFile.WriteFileHeader());
    //API_VERIFY(regFile.WriteString(TEXT("Windows Registry Editor Version 5.00\r\n")));

    //REG_VERIFY(FTL::CFRegUtil::ExportRegValueToFile(TEXT("HKEY_LOCAL_MACHINE\\SOFTWARE"), 
    //    _T("*"), 
    //    &regFile, 
    //    EXPORT_MULTI_VALUE | EXPORT_SUB_KEY, KEY_READ | KEY_WOW64_64KEY));

    //regFile.Close();
#endif 
    CWaitCursor waitCursor;
    FTL::CFElapseCounter elapseCounter;

    CFRegSerialize regSerialize;
    REG_VERIFY(regSerialize.CreateRegFile(TEXT("D:\\test_software2.reg")));

    REG_VERIFY(regSerialize.ExportReg(
        TEXT("HKEY_CURRENT_USER\\Software\\FTL\\Au_.exe"), 
        _T("*"), 
        EXPORT_SUB_KEY | EXPORT_MULTI_VALUE, 
        KEY_READ | KEY_WOW64_64KEY)); //| KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE |

    REG_VERIFY(regSerialize.ExportReg(
        TEXT("HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\Installer"), 
        _T("*"), 
        //_T("Microsoft.SmartDevice.Connectivity,version=\"9.0.0.0\",publicKeyToken=\"b03f5f7f11d50a3a\",processorArchitecture=\"MSIL\",fileVersion=\"9.0.30729.1\",culture=\"neutral\""),
        EXPORT_MULTI_VALUE | EXPORT_SUB_KEY | EXPORT_LINEWAPPER_BINARY, 
        KEY_READ | KEY_WOW64_64KEY)); //| KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE |
    
    REG_VERIFY(regSerialize.CloseRegFile());

    FormatMessageBox(m_hWnd, TEXT("Export Register"), MB_OK, TEXT("Elapse Count=%d(ms)"),
        elapseCounter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
    //::MessageBox(m_hWnd, TEXT("After Export"), TEXT("Over"), MB_OK);
}
