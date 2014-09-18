#pragma once

#include "gifMaker.h"
#include "gifUtilDefine.h"

#include <ColorQuantizer/WuColorQuantizer.h>
#include <ColorQuantizer/OctreeColorQuantizer.h>
#include "Quantize.h"

#include "../giflib/lib/gif_lib.h"
#include "GifFrameInfo.h"

#define MAX_COLOR_MAP_COUNT (256)

class CGifMakerImpl : public IGifMaker
{
public:
    CGifMakerImpl(void);
    virtual ~CGifMakerImpl();
    virtual CompressType SetCompressType(CompressType type);

    virtual BOOL BeginMakeGif(INT nWidth, INT nHeight, LPCTSTR pszFileName);
    virtual BOOL AddGifImage(const RECT& rcFrame, BYTE* pBmpData, INT nLength, INT nBpp, DWORD dwTicket);
    virtual BOOL EndMakeGif(DWORD dwTicket, DWORD dwWaitTimeOut = INFINITE);
    virtual void Release();

private:
    FTL::CFThread<>* m_pThreadMaker;
    FTL::CFProducerResumerQueue<CGifFrameInfoPtr>   m_WaitingFrameInfoQueue;
    static DWORD  __stdcall MakerThreadProc(LPVOID lpThreadParameter);
    DWORD  _innerMakerThreadProc();

    CompressType    m_compressType;
    INT m_nWidth, m_nHeight;// m_nBpp;
    INT m_nPreBmpBytes;     //上一张图片的大小
    INT m_nDiffResultSize;
    INT m_nGifColorRes;     //8
    INT m_nGifNumLevels;    //256
    INT m_nImgIndex;
    INT m_nGifBufferSize;
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
    

    BOOL _WriteGifData(CGifFrameInfoPtr pFrameInfo);

    BYTE* _DuplicateBmpRect(BYTE* pSrcBmpData, INT nSrcWidth, INT nSrcHeight, INT nBpp, RECT rcSrc, INT* pReturnBufSize);
    VOID _FreeDuplicateBmpData(BYTE* pData);

};
