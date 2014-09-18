#pragma once

#include <ftlSharePtr.h>

class CGifFrameInfo
{
    DISABLE_COPY_AND_ASSIGNMENT(CGifFrameInfo);

public:
    CGifFrameInfo(void);
    CGifFrameInfo(INT nIndex, BYTE* pBmpData, INT nLength, DWORD dwTicket, const RECT& rcTarget, INT nBpp, BOOL bAttachMem);

    ~CGifFrameInfo(void);
public:
    INT     nIndex;
    INT     nLength;
    BYTE*   pBmpData;
    DWORD   dwTicket;
    RECT    rcTarget; 
    INT     nBpp;

};
typedef CFSharePtr<CGifFrameInfo>   CGifFrameInfoPtr;
