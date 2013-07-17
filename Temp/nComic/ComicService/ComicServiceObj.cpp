// ComicServiceObj.cpp : Implementation of CComicServiceObj

#include "stdafx.h"
#include "ComicServiceObj.h"


// CComicServiceObj


STDMETHODIMP CComicServiceObj::ProtectWnd(LONG hWnd, LONG clrBackground, BSTR bstrDisplayInfo)
{
	FTLTRACE(TEXT("CComicServiceObj::ProtectWnd, hWnd=0x%x\n"), hWnd);
	return S_OK;
}

STDMETHODIMP CComicServiceObj::UnProtectWnd(LONG hWnd)
{
	FTLTRACE(TEXT("CComicServiceObj::UnProtectWnd, hWnd=0x%x\n"), hWnd);

	return S_OK;
}
