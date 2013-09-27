#pragma once
// ComicHelperProxy.h

#define COMMAND_BEGIN_PROTECT_WND           (DWORD)(1)
#define COMMAND_END_PROTECT_WND             (DWORD)(2)
#define COMMAND_NOTIFY_END_HELPER_PROXY     (DWORD)(3)

struct ProtectWndInfoFileMap32
{
    DWORD       dwCommand;
    DWORD       dwProtectProcessId;
    ULONG       hWndProtect;       //HWND is 4 on 32 and 8 on 64
    COLORREF    clrDisabled;
    //TCHAR       szTargetLogFile;
};

extern ProtectWndInfoFileMap32* g_pProtectWndInfoFileMap;

#define COMIC_PROTECT_WND_FILE_MAP_NAME       TEXT("Global\\ComicViewerFileMap")
#define COMIC_PROTECT_NOTIFY_EVENT_NAME32     TEXT("Global\\ComicViewerNotifyEvent32")
#define COMIC_PROTECT_NOTIFY_EVENT_NAME64     TEXT("Global\\ComicViewerNotifyEvent64")