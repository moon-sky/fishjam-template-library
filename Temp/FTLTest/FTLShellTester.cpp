#include "StdAfx.h"
#include "FTLShellTester.h"
#include "ftlShell.h"

void CFTLShellTester::test_ITEMIDLIST()
{
	BOOL bRet = FALSE;
	TCHAR szModulePat[MAX_PATH] = {0};
	API_VERIFY(GetModuleFileName(NULL, szModulePat, _countof(szModulePat)));

	LPITEMIDLIST pItemIdListModulePath = ILCreateFromPath(szModulePat);
	CPPUNIT_ASSERT(pItemIdListModulePath != NULL);
	if (pItemIdListModulePath)
	{
		ILFree(pItemIdListModulePath);
	}
}

void CFTLShellTester::test_LaunchIE()
{
    FTL::CFShellUtil::LaunchIE(TEXT("www.baidu.com"), SW_SHOW);
}

void CFTLShellTester::test_GetFileVersionInfo()
{
    HRESULT hr = E_FAIL;
    VS_FIXEDFILEINFO fileInfo = {0};
    COM_VERIFY(FTL::CFShellUtil::GetFileVersionInfo(TEXT("C:\\Program Files\\Internet Explorer\\iexplore.exe"), &fileInfo));

    CFStringFormater verString;
    verString.Format(TEXT("%d.%d.%d.%d"), HIWORD(fileInfo.dwFileVersionMS), LOWORD(fileInfo.dwFileVersionMS),
        HIWORD(fileInfo.dwFileVersionLS), LOWORD(fileInfo.dwFileVersionLS));

    //比如会输出 IE version=10.0.9200.17148
    FTLTRACE(TEXT("IE version=%s\n"), verString.GetString());
}

