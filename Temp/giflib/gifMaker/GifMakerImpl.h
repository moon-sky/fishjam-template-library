#pragma once

#include "gifMaker.h"
#include "gifUtilDefine.h"

#include <ColorQuantizer/WuColorQuantizer.h>
#include <ColorQuantizer/OctreeColorQuantizer.h>
#include "Quantize.h"

#include "../giflib/lib/gif_lib.h"
#include "GifFrameInfo.h"


class CGifMakerImpl : public IGifMaker
{
public:
    CGifMakerImpl(void);
    virtual ~CGifMakerImpl();

    virtual CompressType SetCompressType(CompressType type);
    virtual VOID SetCallBack(IGifMakerCallback* pCallback, DWORD_PTR callbackData);
    virtual COLORREF SetBackgroundColor(COLORREF color);
    virtual VOID EnableCompareWithPrevious(BOOL bEnabled);

    virtual BOOL BeginMakeGif(INT nWidth, INT nHeight, INT nWantColorCount, LPCTSTR pszFileName);
    virtual INT AddGifFrame(const RECT& rcFrame, BYTE* pBmpData, INT nLength, INT nBpp, DWORD dwTicket);
    virtual BOOL EndMakeGif(DWORD dwTicket, BOOL bCancelUnwritten);
    virtual void Release();

private:
    IGifMakerCallback*  m_pCallback;
    DWORD_PTR           m_callbackData;
    COLORREF            m_backgroundColor;
    BOOL                m_bEnableCompareWithPrevious;

    CompressType    m_compressType;
    INT m_nScreenWidth, m_nScreenHeight;// m_nBpp;
    INT m_nWantColorCount;

    INT m_nScreenBufferSize;
    INT m_nDiffResultSize;
    INT m_nGifColorRes;     //8
    INT m_nGifNumLevels;    //256
    INT m_nFrameIndex;
    INT m_nWrittenFrameIndex;  //排除掉重复的
    INT m_nGifBufferSize;
    BOOL m_bFirstImage;
    BOOL m_bDelayImage;

    BYTE* m_pScreenBuffer;
    BYTE* m_pDiffResult;
    //BYTE* m_pGiffDiffBuffer;
    RECT  m_rcPreviousFrame;
    INT   m_nPreviousBpp;
    INT   m_nPreviousBmpDataLength;

    GifFileType* m_pGifFile;
    GifColorType* m_pColorMap;   //量化后Gif的颜色表数值
    GifByteType*  m_pGifBuffer;     //量化后的像素索引值

    COLORREF    m_clrCompareSameTransparent; //前后两张图片，比较时如果相同，则设置为该透明色
    FTL::IFColorQuantizer*  m_pColorQuantizer;

    //CQuantizer* m_pQuantizer;
    
    FTL::CFThread<>* m_pThreadMaker;
    FTL::CFProducerResumerQueue<CGifFrameInfoPtr>*   m_pWaitingFrameInfoQueue;

    static DWORD  __stdcall MakerThreadProc(LPVOID lpThreadParameter);
    DWORD  _innerMakerThreadProc();

    BOOL _WriteGifData(CGifFrameInfoPtr pFrameInfo, DWORD dwPreviousTicket);

    BYTE* _DuplicateBmpRect(BYTE* pSrcBmpData, INT nSrcWidth, INT nSrcHeight, INT nBpp, RECT rcSrc, INT* pReturnBufSize);
    VOID _FreeDuplicateBmpData(BYTE* pData);

};
