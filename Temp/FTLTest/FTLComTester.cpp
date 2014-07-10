#include "StdAfx.h"
#include "ftlcomtester.h"
#include <ftlcom.h>

void CFTLComTester::test_RegisterComponent()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;
    TCHAR strPath[MAX_PATH] = {0};
    API_VERIFY( 0 != GetWindowsDirectory(strPath,_countof(strPath)));
    COM_VERIFY(StringCchCat(strPath,_countof(strPath),TEXT("\\System32\\shell32.dll")));
    API_VERIFY(FTL::CFComUtility::RegisterComponent(strPath,TRUE));
}

void CFTLComTester::test_SbsCreateInstance()
{
    HRESULT hr = E_FAIL;

    BOOL bRet = FALSE;
    TCHAR strPath[MAX_PATH] = {0};
    API_VERIFY( 0 != GetWindowsDirectory(strPath,_countof(strPath)));
    COM_VERIFY(StringCchCat(strPath,_countof(strPath),TEXT("\\System32\\shell32.dll")));

    IShellFolder* pShellFolder = NULL;
    COM_VERIFY(FTL::CFSideBySide::SbsCreateInstance(strPath,CLSID_ShellDesktop,NULL,CLSCTX_INPROC,IID_IShellFolder,
        (void**)&pShellFolder));
    if (SUCCEEDED(hr))
    {
        pShellFolder->Release();
    }

    //SAFE_RELEASE(pShellFolder);
}