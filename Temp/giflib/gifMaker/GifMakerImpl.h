#pragma once

#include "gifMaker.h"

#include <ftlBase.h>
#include <ftlThread.h>
#include <ftlGdi.h>
#include <ftlDebug.h>
#include <ftlAlgorithm.h>
#include <ftlSharePtr.h>

#include <ColorQuantizer/WuColorQuantizer.h>
#include <ColorQuantizer/OctreeColorQuantizer.h>
#include "Quantize.h"

#include "../giflib/lib/gif_lib.h"

#define GIF_VERIFY(x) \
    nRet = (x);\
    if(nRet == GIF_ERROR){\
        FTLTRACE(TEXT("ERROR: %s(%d)"), TEXT(#x), 0); \
    }


class SnapBiampInfo{
public:
    SnapBiampInfo();
    SnapBiampInfo(int nWidth, int nHeight, int nBpp, BYTE* pBmpData, DWORD dwTicket, BOOL bAttachMem);
    ~SnapBiampInfo();
public:
    int     nWidth;
    int     nHeight;
    int     nBpp;
    int     nTotalSize;
    BYTE*   pBmpData;
    DWORD   dwTicket;
};
typedef CFSharePtr<SnapBiampInfo>   SnapBiampInfoPtr;

class CGifMakerImpl : public IGifMaker
{
public:
    CGifMakerImpl(void);
    virtual ~CGifMakerImpl();
    virtual CompressType SetCompressType(CompressType type);

    virtual BOOL BeginMakeGif(int nWidth, int nHeight, int bpp, LPCTSTR pszFileName);
    virtual BOOL AddGifImage(BYTE* pBmpData, int nLength, DWORD dwTicket);
    virtual BOOL EndMakeGif(DWORD dwTicket, DWORD dwWaitTimeOut = INFINITE);
    virtual void Release();

private:
    FTL::CFThread<>* m_pThreadMaker;
    FTL::CFProducerResumerQueue<SnapBiampInfoPtr>   m_WaitingBitmapInfoQueue;
    static DWORD  __stdcall MakerThreadProc(LPVOID lpThreadParameter);
    DWORD  _innerMakerThreadProc();

    CompressType    m_compressType;
    int m_nWidth, m_nHeight, m_nBpp, m_nPreBmpBytes, m_nDiffResultSize;
    int m_nGifColorRes;     //8
    int m_nGifNumLevels;    //256
    int m_nImgCount;
    int m_nGifBufferSize;
    BOOL m_bFirstImage;
    BOOL m_bDelayImage;
    BOOL m_bWriteFirst;

    BYTE* m_pPreBmp;
    BYTE* m_pDiffResult;
    BYTE* m_pGifBuffer;
    BYTE* m_pGiffDiffBuffer;
    BYTE* m_pTranslateBuffer;
    RECT  m_rcDiff;


    GifFileType* m_pGifFile;
    GifColorType* m_pColorMap256;
    DWORD m_dwLastTicket;
    CQuantizer* m_pQuantizer;

    BOOL _WriteGifData(BYTE* pBmpData, RECT rcBmp, DWORD dwTicket);

    BYTE* _DuplicateBmpRect(BYTE* pSrcBmpData, int nSrcWidth, int nSrcHeight, int nBpp, RECT rcSrc);
    VOID _FreeDuplicateBmpData(BYTE* pData);

};
