// ComicServiceObj.cpp : Implementation of CComicServiceObj

#include "stdafx.h"
#include "ComicServiceObj.h"
#include "../ComicHelper/ComicHelper.h"
#include <ftlService.h>
#include <WtsApi32.h>
// CComicServiceObj


STDMETHODIMP CComicServiceObj::ProtectWnd(OLE_HANDLE hWnd, OLE_COLOR clrBackground, BSTR bstrDisplayInfo)
{
    DWORD dwResult = 0;
    BOOL bRet = FALSE;

    //MessageBox(NULL, TEXT("Will ProtectWnd"), TEXT("Caption"), MB_OK);
    //API_VERIFY(WTSSendMessage(WTS_CURRENT_SERVER_HANDLE , WTSGetActiveConsoleSessionId(), TEXT("ProtectWnd"),
    //    10 * sizeof(TCHAR), TEXT("Cap"), 3 * sizeof(TCHAR), MB_OK, 5, &dwResult, TRUE));

#pragma TODO(Need Change PID)

#ifdef _DEBUG
#  ifdef _M_IX86
    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\Win32\\Debug\\ComicHelperProxy.exe");
#  else
    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\x64\\Debug\\ComicHelperProxy.exe");
#  endif 
#else
#  ifdef _M_IX86
    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\Win32\\Release\\ComicHelperProxy.exe");
#  else
    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\x64\\Release\\ComicHelperProxy.exe");
#  endif 
   
#endif 
    API_VERIFY(FTL::CFService::CreateServiceUIProcess(pszFilePath, TRUE));

    //EnableWindowProtected(GetCurrentProcessId(), (HWND)hWnd, clrBackground);
    FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
    return S_OK;
}

STDMETHODIMP CComicServiceObj::UnProtectWnd(OLE_HANDLE hWnd)
{
    //DisableWindowProtected((HWND)hWnd);
    FTLTRACE(TEXT("CComicServiceObj::UnProtectWnd, hWnd=0x%x\n"), hWnd);

    return S_OK;
}
