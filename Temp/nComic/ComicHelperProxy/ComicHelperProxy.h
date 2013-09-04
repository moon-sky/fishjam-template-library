#pragma once
// ComicHelperProxy.h

#define UM_UPDATE_PROTECT_WND       (WM_USER + 100)

struct ProtectWndInfoFileMap
{
    //HANDLE  hEventProtectUpdate;
    HWND    hWndProtect;
    //HANDLE  hDibSection;
    COLORREF clrDisabled;
};

#define COMIC_PROTECT_WND_FILE_MAP_NAME     TEXT("Global\\ComicViewerFileMap")