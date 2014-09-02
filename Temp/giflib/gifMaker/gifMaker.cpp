// gifMaker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gifMaker.h"

#include <ftlBase.h>
#include <ftlThread.h>
#include <ftlGdi.h>

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

    m_nPreWidth = 0;
    m_nPreHeight = 0;
    m_nPreBpp = 0;
    //m_pThreadMaker = NULL;
    m_pPreBmp = NULL;
    m_pDiffResult = NULL;
    m_pGifFile = NULL;
    m_pGifBuffer = NULL;
    m_pColorMap256 = NULL;
    m_bFirstImage = TRUE;
    m_bDelayImage = FALSE;
    
    m_nGifColorRes = 8;
    m_nGifNumLevels = 256;
    m_dwLastTicket = 0;
    m_nImgCount = 0;
}

CGifMaker::~CGifMaker()
{
    FTLASSERT(m_pGifFile == NULL);

    SAFE_DELETE_ARRAY(m_pColorMap256);
    SAFE_DELETE_ARRAY(m_pGifBuffer);
    SAFE_DELETE_ARRAY(m_pPreBmp);
    SAFE_DELETE_ARRAY(m_pDiffResult);
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

    m_nDiffResultSize= ((nWidth * 8 + 31) / 32 * 4) * nHeight;  //四字节对齐的
    m_pDiffResult = new BYTE[m_nDiffResultSize];
    ZeroMemory(m_pDiffResult, m_nDiffResultSize);

    int nError = 0;
    m_pGifFile = EGifOpenFileName(fileName, false, &nError);
    GIF_VERIFY(EGifPutScreenDesc(m_pGifFile, nWidth, nHeight, bpp, 0, NULL));

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

    if (m_bFirstImage)
    {
        m_bFirstImage = FALSE;    
        m_dwLastTicket = dwTicket;
        CopyMemory(m_pPreBmp, pBmpData, nLength);
    }
    else
    {
        int nDiffCount = FTL::CFGdiUtil::ComapreBitmapData(m_nPreWidth, m_nPreHeight, m_nPreBpp, m_pPreBmp, pBmpData, m_pDiffResult, m_nDiffResultSize);
        FTLTRACE(TEXT("[%d], dwTicket=%d, nDiffCount=%d/%d \n"), m_nImgCount, dwTicket, nDiffCount, m_nDiffResultSize);
        if (0 == nDiffCount)
        {
            bWriteGraphControl = FALSE;
            bWriteImageData = FALSE;
        }else{
            bWriteGraphControl = TRUE;
            bWriteImageData = TRUE;
            _WriteGifData(TRUE, TRUE, dwTicket);
            CopyMemory(m_pPreBmp, pBmpData, nLength);
        }
    }
    m_bDelayImage = !bWriteImageData;
    return TRUE;
}

BOOL CGifMaker::_WriteGifData(BOOL bWriteGraphControl, BOOL bWriteImageData, DWORD dwTicket)
{
    int nRet = 0;
    if (bWriteGraphControl)
    {
        DWORD dwElapse = (dwTicket - m_dwLastTicket) / 10;
        unsigned char ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
        ExtStr[0] = 0x04;
        ExtStr[1] = dwElapse % 256;
        ExtStr[2] = dwElapse / 256;

        GIF_VERIFY(EGifPutExtension(m_pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr));

        m_dwLastTicket = dwTicket;
    }

    if (bWriteImageData)
    {
        nRet = GifQuantizeRGBBuffer(m_nPreWidth, m_nPreHeight, m_nPreBpp, m_pPreBmp, m_pColorMap256, m_pGifBuffer);
        ColorMapObject *pColorMap = GifMakeMapObject(m_nGifNumLevels, m_pColorMap256);

        GIF_VERIFY(EGifPutImageDesc(m_pGifFile, 0, 0, m_nPreWidth, m_nPreHeight, FALSE, pColorMap));

        GifByteType *Ptr = m_pGifBuffer;
        for (int j = 0; j < m_nPreHeight; j++) 
        {
            GIF_VERIFY(EGifPutLine(m_pGifFile, Ptr, m_nPreWidth));
            Ptr += m_nPreWidth;
        }
        GifFreeMapObject(pColorMap);
    }
    return FALSE;
}

BOOL CGifMaker::EndMakeGif(DWORD dwTicket)
{
    int nRet = 0;
    int nError = 0;
    if (m_pGifFile)
    {
        if (m_bDelayImage)
        {
            _WriteGifData(TRUE, TRUE, dwTicket);
        }
        GIF_VERIFY(EGifCloseFile(m_pGifFile, &nError));
        m_pGifFile = NULL;
    }
    return FALSE;
}