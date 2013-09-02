#pragma once

struct PROTECT_WND_INFO
{
    HWND        hFilterWnd;
    DWORD       curProcessId;
    COLORREF    clrDisabled;
};


class CProtectWndHookAPI
{
public:
    CProtectWndHookAPI(void);
    ~CProtectWndHookAPI(void);
    BOOL StartHook();
    BOOL StopHook();
private:

};
