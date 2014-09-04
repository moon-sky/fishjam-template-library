// gifMaker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gifMaker.h"

#include <ftlBase.h>
#include <ftlThread.h>
#include <ftlGdi.h>
#include <ftlDebug.h>

#include "../giflib/lib/gif_lib.h"
#ifdef _DEBUG
#  pragma comment(lib, "../Debug/giflib.lib")
#else
#  pragma comment(lib, "../Release/giflib.lib")
#endif 

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#define GIF_VERIFY(x) \
    nRet = (x);\
    if(nRet == GIF_ERROR){\
        FTLTRACE(TEXT("ERROR: %s(%d)"), TEXT(#x), 0); \
    }

//// This is the constructor of a class that has been exported.
//// see gifMaker.h for the class definition

CGifMaker::CGifMaker()
{
    m_nPreBmpBytes = 0;
    m_nDiffResultSize = 0;
    m_bWriteFirst = TRUE;

    m_nPreWidth = 0;
    m_nPreHeight = 0;
    m_nPreBpp = 0;
    //m_pThreadMaker = NULL;
    m_pPreBmp = NULL;
    m_pDiffResult = NULL;
    m_pGifFile = NULL;
    m_pGifBuffer = NULL;
    m_pTranslateBuffer = NULL;
    m_pColorMap256 = NULL;
    m_bFirstImage = TRUE;
    m_bDelayImage = FALSE;
    m_pGiffDiffBuffer = NULL;
    ZeroMemory(&m_rcDiff, sizeof(m_rcDiff));

    m_nGifColorRes = 8;
    m_nGifNumLevels = 256;
    m_dwLastTicket = 0;
    m_nImgCount = 0;
}

CGifMaker::~CGifMaker()
{
    FTLASSERT(m_pGifFile == NULL);
    SAFE_DELETE_ARRAY(m_pTranslateBuffer);
    SAFE_DELETE_ARRAY(m_pColorMap256);
    SAFE_DELETE_ARRAY(m_pGifBuffer);
    SAFE_DELETE_ARRAY(m_pPreBmp);
    SAFE_DELETE_ARRAY(m_pDiffResult);
    if (m_pGiffDiffBuffer)
    {
        _FreeDuplicateBmpData(m_pGiffDiffBuffer);
        m_pGiffDiffBuffer = NULL;
    }
}

BOOL CGifMaker::BeginMakeGif(int nWidth, int nHeight, int bpp, const char* fileName)
{
    int nRet = 0;
    m_nPreWidth = nWidth;
    m_nPreHeight = nHeight;
    m_nPreBpp = bpp;

    m_nPreBmpBytes = ((nWidth * bpp + 31) / 32 * 4) * nHeight;  //四字节对齐
    m_pPreBmp = new BYTE[m_nPreBmpBytes];
    ZeroMemory(m_pPreBmp, m_nPreBmpBytes);

    m_pTranslateBuffer = new BYTE[m_nPreBmpBytes];
    ZeroMemory(m_pTranslateBuffer, m_nPreBmpBytes);

    m_nDiffResultSize= ((nWidth * 8 + 31) / 32 * 4) * nHeight;  //四字节对齐的
    m_pDiffResult = new BYTE[m_nDiffResultSize];
    ZeroMemory(m_pDiffResult, m_nDiffResultSize);

    int nError = 0;
    m_pGifFile = EGifOpenFileName(fileName, false, &nError);
    GIF_VERIFY(EGifPutScreenDesc(m_pGifFile, nWidth, nHeight, bpp, 0, NULL));
    EGifSetGifVersion(m_pGifFile, true);

    int nSize = sizeof(GifPixelType) * nWidth * nHeight;
    m_pGifBuffer = (GifByteType *) new BYTE[nSize];
    ZeroMemory(m_pGifBuffer, nSize);

    m_pColorMap256 = new GifColorType[256];
    ZeroMemory(m_pColorMap256, sizeof(GifColorType) * 256);

    m_bFirstImage = TRUE; 
    m_nImgCount = 0;

    return TRUE;
}

BOOL CGifMaker::AddGifImage(BYTE* pBmpData, int nLength, DWORD dwTicket)
{
    int nRet = 0;
    FTLASSERT(m_pGifFile);
    FTLASSERT(m_nPreBmpBytes == nLength);
    m_nImgCount++;

    BOOL bWriteImageData = FALSE;
    BOOL bWriteGraphControl = FALSE;
    FTLTRACE(TEXT("AddGifImage, Index=%d, nLength=%d, dwTicket=%d\n"), m_nImgCount, nLength, dwTicket);
    if (m_bFirstImage)
    {
        m_bFirstImage = FALSE;    
        m_dwLastTicket = dwTicket;
        CopyMemory(m_pPreBmp, pBmpData, nLength);
        ::SetRect(&m_rcDiff, 0, 0, m_nPreWidth, m_nPreHeight);
        m_pGiffDiffBuffer = _DuplicateBmpRect(pBmpData, m_nPreWidth, m_nPreHeight, m_nPreBpp, m_rcDiff);
    }
    else
    {
        RECT rcMinDiff = {0};
        //CopyMemory(m_pTranslateBuffer, pBmpData, nLength);
        int nDiffCount = FTL::CFGdiUtil::CompareBitmapData(m_nPreWidth, m_nPreHeight, m_nPreBpp, m_pPreBmp, pBmpData, m_pDiffResult, 
            m_nDiffResultSize, &rcMinDiff);
        FTLTRACE(TEXT("[%d], dwTicket=%d, nDiffCount=%d/%d, rcMinDiff=%s\n"), m_nImgCount, dwTicket, nDiffCount, m_nDiffResultSize, 
            FTL::CFRectDumpInfo(rcMinDiff).GetConvertedInfo());
        if (0 == nDiffCount)
        {
            bWriteGraphControl = FALSE;
            bWriteImageData = FALSE;
        }else{
            bWriteGraphControl = TRUE;
            bWriteImageData = TRUE;

            if (m_pGiffDiffBuffer)
            {
                _WriteGifData(m_pGiffDiffBuffer, m_rcDiff, dwTicket);
                _FreeDuplicateBmpData(m_pGiffDiffBuffer);
                m_pGiffDiffBuffer = NULL;
                ZeroMemory(&m_rcDiff, sizeof(m_rcDiff));
            }
            //if (m_bWriteFirst)
            //{
            //    RECT rcTotal = {0, 0, m_nPreWidth, m_nPreHeight};
            //    _WriteGifData(pBmpData, rcTotal, dwTicket);
            //}
            //else
            {
                if (nDiffCount * 10 / m_nDiffResultSize < 6  //不同的数小于 60%
                    && ((rcMinDiff.right - rcMinDiff.left) < m_nPreWidth)
                    && ((rcMinDiff.bottom - rcMinDiff.top) < m_nPreHeight))
                {

                    m_rcDiff = rcMinDiff;
                    //::SetRect(&m_rcDiff, 0, 0, m_nPreWidth, m_nPreHeight);
                    m_pGiffDiffBuffer = _DuplicateBmpRect(pBmpData, m_nPreWidth, m_nPreHeight, m_nPreBpp, m_rcDiff);

//#if 0
//                    RECT rcTotal = {0, 0, m_nPreWidth, m_nPreHeight};
//                    _WriteGifData(pBmpData, rcTotal, dwTicket);
//#else
//                    BYTE* pNewBmpData = _DuplicateBmpRect(pBmpData, m_nPreWidth, m_nPreHeight, m_nPreBpp, rcMinDiff);
//                    _WriteGifData(pNewBmpData, rcMinDiff, dwTicket);
//                    _FreeDuplicateBmpData(pNewBmpData);
//#endif 
                }
                else{
                    RECT rcTotal = {0, 0, m_nPreWidth, m_nPreHeight};
                    _WriteGifData(pBmpData, rcTotal, dwTicket);
                }
            }
            CopyMemory(m_pPreBmp, pBmpData, nLength);
        }
    }
    m_bDelayImage = !bWriteImageData;

    return TRUE;
}

BOOL CGifMaker::EndMakeGif(DWORD dwTicket)
{
    int nRet = 0;
    int nError = 0;
    if (m_pGifFile)
    {
        //if (!m_bDelayImage)
        //{
        //    RECT rcTotal = {0, 0, m_nPreWidth, m_nPreHeight};
        //    _WriteGifData(m_pPreBmp, rcTotal, dwTicket);
        //}
        if (m_pGiffDiffBuffer)
        {
            _WriteGifData(m_pGiffDiffBuffer, m_rcDiff, dwTicket);
            _FreeDuplicateBmpData(m_pGiffDiffBuffer);
            m_pGiffDiffBuffer = NULL;
            ZeroMemory(&m_rcDiff, sizeof(m_rcDiff));
        }

        GIF_VERIFY(EGifCloseFile(m_pGifFile, &nError));
        m_pGifFile = NULL;
    }
    return FALSE;
}

BOOL CGifMaker::_WriteGifData(BYTE* pBmpData, RECT rcBmp,DWORD dwTicket)
{
    FTLTRACE(TEXT("_WriteGifData, %d\n "), m_nImgCount);
    int nRet = 0;
    m_bWriteFirst = FALSE;
    //if (bWriteGraphControl)
    {
        GifByteType Extension[4] = {0};
        GraphicsControlBlock gcb = {0};

        gcb.DelayTime = (dwTicket - m_dwLastTicket) / 10;
        gcb.TransparentColor = NO_TRANSPARENT_COLOR;
        gcb.UserInputFlag = 0;
        gcb.DisposalMode = DISPOSAL_UNSPECIFIED;
        EGifGCBToExtension(&gcb, Extension);

        //unsigned char ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
        //ExtStr[0] = 0x04;
        //ExtStr[1] = dwElapse % 256;
        //ExtStr[2] = dwElapse / 256;

        GIF_VERIFY(EGifPutExtension(m_pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, Extension));

        m_dwLastTicket = dwTicket;
    }

    int nWidth = rcBmp.right - rcBmp.left;
    int nHeight = rcBmp.bottom - rcBmp.top;

    //if (bWriteImageData)
    {
        nRet = GifQuantizeRGBBuffer(nWidth, nHeight, m_nPreBpp, pBmpData, m_pColorMap256, m_pGifBuffer);
        ColorMapObject *pColorMap = GifMakeMapObject(m_nGifNumLevels, m_pColorMap256);

        GIF_VERIFY(EGifPutImageDesc(m_pGifFile, rcBmp.left, rcBmp.top, nWidth, nHeight, FALSE, pColorMap));

        GifByteType *Ptr = m_pGifBuffer;
        for (int j = 0; j < nHeight; j++) 
        {
            GIF_VERIFY(EGifPutLine(m_pGifFile, Ptr, nWidth));
            Ptr += nWidth;
        }
        GifFreeMapObject(pColorMap);
    }
    return FALSE;
}

BYTE* CGifMaker::_DuplicateBmpRect(BYTE* pSrcBmpData, int nSrcWidth, int nSrcHeight, int nBpp, RECT rcSrc)
{
    FTLTRACE(TEXT("_DuplicateBmpRect[%d], nSrcWidth=%d, nSrcHeight=%d, rcSrc=%s\n"), m_nImgCount, nSrcWidth, nSrcHeight, 
        FTL::CFRectDumpInfo(rcSrc).GetConvertedInfo());
    BYTE* pBufResult = NULL;
    FTLASSERT(rcSrc.left >= 0 && rcSrc.top >= 0);
    FTLASSERT(rcSrc.right <= nSrcWidth && rcSrc.bottom <= nSrcHeight);
    
    DWORD dwOldWidthBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nSrcWidth, nBpp); 
    DWORD dwNewWidthBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(rcSrc.right - rcSrc.left, nBpp); 
    DWORD dwNewBmpSize = dwNewWidthBytes * (rcSrc.bottom - rcSrc.top);
    FTLASSERT(dwNewBmpSize > 0);
    pBufResult = new BYTE[dwNewBmpSize];
    if (pBufResult)
    {
        ZeroMemory(pBufResult, dwNewBmpSize);
        for (int j = rcSrc.top; j < rcSrc.bottom ; j++)
        {
            BYTE* pSrc = pSrcBmpData + j * dwOldWidthBytes + rcSrc.left * nBpp / 8;
            BYTE* pDest = pBufResult + (j - rcSrc.top) * dwNewWidthBytes;
            
            CopyMemory(pDest, pSrc, (rcSrc.right - rcSrc.left) * nBpp / 8);
        }
    }

    return pBufResult;
}

VOID CGifMaker::_FreeDuplicateBmpData(BYTE* pData)
{
    SAFE_DELETE_ARRAY(pData);
}
