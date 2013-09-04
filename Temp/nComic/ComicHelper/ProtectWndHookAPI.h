#pragma once

//struct PROTECT_WND_INFO
//{
//    HWND        hFilterWnd;
//    DWORD       curProcessId;
//    COLORREF    clrDisabled;
//};
#include <atlfile.h>
#include "../ComicHelperProxy/ComicHelperProxy.h"

class CProtectWndHookAPI
{
public:
    CProtectWndHookAPI(void);
    ~CProtectWndHookAPI(void);
    BOOL StartHook();
    BOOL StopHook();
private:
    CAtlFileMapping<ProtectWndInfoFileMap> m_FileMap;
};
