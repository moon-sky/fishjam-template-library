// ComicServiceObj.cpp : Implementation of CComicServiceObj

#include "stdafx.h"
#include "ComicServiceObj.h"
#include "../ComicHelper/ComicHelper.h"

// CComicServiceObj


STDMETHODIMP CComicServiceObj::ProtectWnd(LONG hWnd, LONG clrBackground, BSTR bstrDisplayInfo)
{
	SetApiHook((HWND)hWnd, clrBackground);
	FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
	return S_OK;
}

STDMETHODIMP CComicServiceObj::UnProtectWnd(LONG hWnd)
{
	SetApiUnHook();
	FTLTRACE(TEXT("CComicServiceObj::UnProtectWnd, hWnd=0x%x\n"), hWnd);

	return S_OK;
}
