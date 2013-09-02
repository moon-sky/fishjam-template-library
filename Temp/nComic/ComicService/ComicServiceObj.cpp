// ComicServiceObj.cpp : Implementation of CComicServiceObj

#include "stdafx.h"
#include "ComicServiceObj.h"
#include "../ComicHelper/ComicHelper.h"
#include "../ComicHelperProxy/ComicHelperProxy.h"
#include <ftlService.h>
#include <WtsApi32.h>
// CComicServiceObj


STDMETHODIMP CComicServiceObj::ProtectWnd(OLE_HANDLE hWnd, OLE_HANDLE hDIBSecion)
{
    DWORD dwResult = 0;
    BOOL bRet = FALSE;
    FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);

    //MessageBox(NULL, TEXT("Will ProtectWnd"), TEXT("Caption"), MB_OK);
    //API_VERIFY(WTSSendMessage(WTS_CURRENT_SERVER_HANDLE , WTSGetActiveConsoleSessionId(), TEXT("ProtectWnd"),
    //    10 * sizeof(TCHAR), TEXT("Cap"), 3 * sizeof(TCHAR), MB_OK, 5, &dwResult, TRUE));

#pragma TODO(Need Change PID)

//#ifdef _DEBUG
//#  ifdef _M_IX86
//    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\Win32\\Debug\\ComicHelperProxy.exe");
//#  else
//    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\x64\\Debug\\ComicHelperProxy.exe");
//#  endif 
//#else
//#  ifdef _M_IX86
//    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\Win32\\Release\\ComicHelperProxy.exe");
//#  else
//    LPCTSTR pszFilePath = TEXT("F:\\Fujie\\FJCODE_GOOGLE\\Temp\\nComic\\x64\\Release\\ComicHelperProxy.exe");
//#  endif 
//#endif 
    TCHAR szHelperProxyPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szHelperProxyPath, _countof(szHelperProxyPath));
    API_VERIFY(PathRemoveFileSpec(szHelperProxyPath));
    API_VERIFY(PathAppend(szHelperProxyPath, TEXT("ComicHelperProxy.exe")));


    FTLTRACE(TEXT("ProxyProcess is %s\n"), szHelperProxyPath);
    
    if (m_pProtectWndInfoMap)
    {
        m_pProtectWndInfoMap->hWndProtect = (HWND)hWnd;
    }
    
    PROCESS_INFORMATION processInfo = {0};
    API_VERIFY(FTL::CFService::CreateServiceUIProcess(szHelperProxyPath, TRUE, &processInfo, WTSGetActiveConsoleSessionId()));
    if (bRet)
    {
        FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, Before WaitForInputIdle, process=%d, threadid=%d\n"), 
            processInfo.hProcess, processInfo.dwThreadId);

        WaitForInputIdle(processInfo.hProcess, INFINITE);
        API_VERIFY(PostThreadMessage(processInfo.dwThreadId, UM_UPDATE_PROTECT_WND, hWnd, NULL));

        SAFE_CLOSE_HANDLE(processInfo.hProcess, NULL);
        SAFE_CLOSE_HANDLE(processInfo.hThread, NULL);
        FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, After PostThreadMessage\n")); 
    }

    //EnableWindowProtected(GetCurrentProcessId(), (HWND)hWnd, clrBackground);
    FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
    return S_OK;
}

STDMETHODIMP CComicServiceObj::UnProtectWnd(OLE_HANDLE hWnd)
{
    DisableWindowProtected((HWND)hWnd);
    if (m_pProtectWndInfoMap)
    {
        m_pProtectWndInfoMap->hWndProtect = NULL;
    }

    FTLTRACE(TEXT("CComicServiceObj::UnProtectWnd, hWnd=0x%x\n"), hWnd);

    return S_OK;
}
