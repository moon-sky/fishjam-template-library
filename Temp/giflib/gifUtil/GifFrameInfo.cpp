#include "StdAfx.h"
#include "GifFrameInfo.h"

CGifFrameInfo::CGifFrameInfo()
{
    nIndex = -1;
    nLength = 0;
    pBmpData = NULL;
    dwTicket = 0;
    ::SetRectEmpty(&rcTarget);
    nBpp = 0;
    FTLTRACE(TEXT("Enter CGifFrameInfo::CGifFrameInfo, nIndex=%d\n"), nIndex);
}

CGifFrameInfo::CGifFrameInfo(INT nIndex, BYTE* pBmpData, INT nLength, DWORD dwTicket, const RECT& rcTarget, INT nBpp, BOOL bAttachMem)
{
    BOOL bRet = FALSE;
    this->nIndex = nIndex;
    this->nLength = nLength;
    this->pBmpData = NULL;
    this->dwTicket = dwTicket;
    this->nBpp = nBpp;

    API_VERIFY(::CopyRect(&(this->rcTarget), &rcTarget));

    if (!bAttachMem)
    {
        INT nWidth = (rcTarget.right - rcTarget.left);
        INT nHeight = rcTarget.bottom - rcTarget.top;
        INT nTotalSize = ((nWidth * nBpp + 31) / 32 * 4) * nHeight;  //ËÄ×Ö½Ú¶ÔÆë
        FTLASSERT(nLength == nTotalSize);

        this->pBmpData = new BYTE[nTotalSize];
        CopyMemory(this->pBmpData, pBmpData, nTotalSize);
    }
    else
    {
        this->pBmpData = pBmpData;
    }
    FTLTRACE(TEXT("Enter CGifFrameInfo construction, nIndex=%d, bAttachMem=%d\n"), nIndex, bAttachMem);
}

CGifFrameInfo::~CGifFrameInfo()
{
    FTLTRACE(TEXT("Enter CGifFrameInfo destruct, nIndex=%d\n"), nIndex);
    SAFE_DELETE_ARRAY(pBmpData);
}