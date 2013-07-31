// ComicServiceObj.cpp : Implementation of CComicServiceObj

#include "stdafx.h"
#include "ComicServiceObj.h"
#include "../ComicHelper/ComicHelper.h"
#include <WtsApi32.h>
#pragma comment(lib, "wtsapi32.lib")
// CComicServiceObj


STDMETHODIMP CComicServiceObj::ProtectWnd(OLE_HANDLE hWnd, OLE_COLOR clrBackground, BSTR bstrDisplayInfo)
{
	DWORD dwResult = 0;
	//MessageBox(NULL, TEXT("Will ProtectWnd"), TEXT("Caption"), MB_OK);
	WTSSendMessage(WTS_CURRENT_SERVER_HANDLE , WTSGetActiveConsoleSessionId(), TEXT("ProtectWnd"),
		10 * sizeof(TCHAR), TEXT("Cap"), 3 * sizeof(TCHAR), MB_OK, 5, &dwResult, TRUE);

#pragma TODO(Need Change PID)
	EnableWindowProtected(0, (HWND)hWnd, clrBackground);
	FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
	return S_OK;
}

STDMETHODIMP CComicServiceObj::UnProtectWnd(OLE_HANDLE hWnd)
{
	DisableWindowProtected((HWND)hWnd);
	FTLTRACE(TEXT("CComicServiceObj::UnProtectWnd, hWnd=0x%x\n"), hWnd);

	return S_OK;
}
