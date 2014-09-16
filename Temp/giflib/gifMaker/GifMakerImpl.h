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

#define GIF_VERIFY(x, err) \
    nRet = (x);\
    if(nRet == GIF_ERROR){\
        FTLTRACE(TEXT("%s(%d) :\t Gif Error: %s(%d)\n"), TEXT(__FILE__), __LINE__, TEXT(#x), (err)); \
    }

class SnapBiampInfo{
    DISABLE_COPY_AND_ASSIGNMENT(SnapBiampInfo);
public:
    SnapBiampInfo();
    SnapBiampInfo(int nIndex, BYTE* pBmpData, int nLength, DWORD dwTicket, const RECT& rcTarget, int nBpp, BOOL bAttachMem);
    ~SnapBiampInfo();
public:
    int     nIndex;
    int     nLength;
    BYTE*   pBmpData;
    DWORD   dwTicket;
    RECT    rcTarget; 
    int     nBpp;
};
typedef CFSharePtr<SnapBiampInfo>   SnapBiampInfoPtr;

#define MAX_COLOR_MAP_COUNT (256)

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
    int m_nWidth, m_nHeight, m_nBpp;
    int m_nPreBmpBytes;     //上一张图片的大小
    int m_nDiffResultSize;
    int m_nGifColorRes;     //8
    int m_nGifNumLevels;    //256
    int m_nImgIndex;
    int m_nGifBufferSize;
    BOOL m_bFirstImage;
    BOOL m_bDelayImage;
    BOOL m_bWriteFirst;

    BYTE* m_pPreBmp;
    BYTE* m_pDiffResult;
    //BYTE* m_pGiffDiffBuffer;
    RECT  m_rcDiff;

    GifFileType* m_pGifFile;
    GifColorType* m_pColorMap;   //量化后Gif的颜色表数值
    GifByteType*  m_pGifBuffer;     //量化后的像素索引值

    FTL::IFColorQuantizer*  m_pColorQuantizer;

    DWORD m_dwLastTicket;
    //CQuantizer* m_pQuantizer;
    

    BOOL _WriteGifData(SnapBiampInfoPtr pSnapBitmapInfo);

    BYTE* _DuplicateBmpRect(BYTE* pSrcBmpData, int nSrcWidth, int nSrcHeight, int nBpp, RECT rcSrc, int* pReturnBufSize);
    VOID _FreeDuplicateBmpData(BYTE* pData);

};
