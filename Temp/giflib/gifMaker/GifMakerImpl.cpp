#include "StdAfx.h"
#include "GifMakerImpl.h"

#ifdef _DEBUG
#  pragma comment(lib, "../Debug/giflib.lib")
#else
#  pragma comment(lib, "../Release/giflib.lib")
#endif 


CGifMakerImpl::CGifMakerImpl()
{
    m_pCallback = NULL;
    m_callbackData = 0;
    m_backgroundColor = MAKE_RGBA(0xFF, 0x0, 0x0, 0xFF);
    m_bEnableCompareWithPrevious = FALSE;

    m_compressType = ctHighQuality;
    m_nScreenWidth = 0;
    m_nScreenHeight = 0;
    m_nWantColorCount = MAX_COLOR_MAP_COUNT;
    //m_nBpp = 0;

    m_nScreenBufferSize = 0;
    m_nDiffResultSize = 0;

    m_pScreenBuffer = NULL;
    m_pDiffResult = NULL;
    m_pGifFile = NULL;

    m_pColorMap = NULL;
    m_pGifBuffer = NULL;
    m_pColorQuantizer = NULL;

    //m_pQuantizer = new CQuantizer(256, 8);

    m_bFirstImage = TRUE;
    m_bDelayImage = FALSE;
    //m_pGiffDiffBuffer = NULL;

    ::SetRectEmpty(&m_rcPreviousFrame);
    m_nPreviousBpp = 0;
    m_nPreviousBmpDataLength = 0;

    m_nGifColorRes = 8;
    m_nGifNumLevels = 256;
    m_nFrameIndex = 0;
    m_nWrittenFrameIndex = 0;
    m_nGifBufferSize = 0;

    m_pThreadMaker = NULL;
    m_pWaitingFrameInfoQueue = NULL;
}

CGifMakerImpl::~CGifMakerImpl()
{
    FUNCTION_BLOCK_TRACE(100);

    FTLASSERT(NULL == m_pGifFile);
    FTLASSERT(NULL == m_pWaitingFrameInfoQueue);

    //SAFE_DELETE(m_pQuantizer);
    {
        FUNCTION_BLOCK_NAME_TRACE(TEXT("Delete m_pColorQuantizer"), 100);
        SAFE_DELETE(m_pColorQuantizer);
    }

    {
        FUNCTION_BLOCK_NAME_TRACE(TEXT("Delete Memory"), 100);

        SAFE_DELETE_ARRAY(m_pColorMap);
        SAFE_DELETE_ARRAY(m_pGifBuffer);
        SAFE_DELETE_ARRAY(m_pScreenBuffer);
        SAFE_DELETE_ARRAY(m_pDiffResult);
    }
    //if (m_pGiffDiffBuffer)
    //{
    //    _FreeDuplicateBmpData(m_pGiffDiffBuffer);
    //    m_pGiffDiffBuffer = NULL;
    //}
}

void CGifMakerImpl::Release()
{
    delete this;
}

CompressType CGifMakerImpl::SetCompressType(CompressType type)
{
    CompressType oldType = type;  
    if (m_compressType != type)
    {
        SAFE_DELETE(m_pColorQuantizer);
        switch (type)
        {
        case ctHighQuality:
            m_pColorQuantizer = new FTL::CFOctreeColorQuantizer();
            break;
        default:
            break;
        }
        m_compressType = type;
    }
    return oldType; 
}

DWORD  CGifMakerImpl::MakerThreadProc(LPVOID lpThreadParameter)
{
    CGifMakerImpl* pThis = reinterpret_cast<CGifMakerImpl*>(lpThreadParameter);
    DWORD dwResult = pThis->_innerMakerThreadProc();
    return dwResult;
}

DWORD  CGifMakerImpl::_innerMakerThreadProc()
{
    FUNCTION_BLOCK_TRACE(100);

    FTLASSERT(m_pGifFile);
    DWORD dwResult = 0;
    if (m_pGifFile)
    {
        CGifFrameInfoPtr    pFrameInfo;
        FTL::FTLThreadWaitType waitType = FTL::ftwtStop;
        DWORD dwPreviousTicket = 0;
        BOOL bFirstFrame = TRUE;
        do 
        {
            waitType = m_pWaitingFrameInfoQueue->Remove(pFrameInfo, INFINITE);
            if (FTL::ftwtContinue == waitType)
            {
                if (bFirstFrame)
                {
                    //第一帧
                    bFirstFrame = FALSE;
                    dwPreviousTicket = pFrameInfo->dwTicket;
                }

                _WriteGifData(pFrameInfo, dwPreviousTicket);
                dwPreviousTicket = pFrameInfo->dwTicket;
            }
            pFrameInfo.reset();
        } while (FTL::ftwtContinue == waitType);
    }

    dwResult = 0;
    return dwResult;
}

COLORREF CGifMakerImpl::SetBackgroundColor(COLORREF color)
{
    COLORREF oldColor = m_backgroundColor;
    m_backgroundColor = color;
    return oldColor;
}

VOID CGifMakerImpl::EnableCompareWithPrevious(BOOL bEnabled)
{
    m_bEnableCompareWithPrevious = bEnabled;
}

VOID CGifMakerImpl::SetCallBack(IGifMakerCallback* pCallback, DWORD_PTR callbackData)
{
    m_pCallback = pCallback;
    m_callbackData = callbackData;
}

BOOL CGifMakerImpl::BeginMakeGif(INT nWidth, INT nHeight, INT nWantColorCount, LPCTSTR pszFileName)
{
    FTLASSERT(nWidth > 0);
    FTLASSERT(nHeight > 0);

    BOOL bRet = FALSE;
    INT nRet = GIF_ERROR;
    INT nError = E_GIF_SUCCEEDED;

    FTLASSERT(0 < nWantColorCount && nWantColorCount <= MAX_COLOR_MAP_COUNT);
    if (nWantColorCount <= 0 || nWantColorCount > MAX_COLOR_MAP_COUNT)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    m_pGifFile = EGifOpenFileName(pszFileName, false, &nError);
    if (m_pGifFile)
    {
        int nColorRes = GifBitSize(nWantColorCount);
        GIF_VERIFY(EGifPutScreenDesc(m_pGifFile, nWidth, nHeight, nColorRes, 0, NULL), m_pGifFile->Error);
        EGifSetGifVersion(m_pGifFile, true);

        m_nScreenWidth = nWidth;
        m_nScreenHeight = nHeight;
        m_nWantColorCount = nWantColorCount;

        //以RGBA的方式来设置最大的缓存空间,这样可以支持添加多种格式的图片,加入的图片数据可能只占用其中的一部分
        m_nScreenBufferSize = nWidth * nHeight * sizeof(COLORREF); //  ((nWidth * 32 + 31) / 32 * 4) * nHeight;
        m_pScreenBuffer = new BYTE[m_nScreenBufferSize];
        ZeroMemory(m_pScreenBuffer, m_nScreenBufferSize);

        m_nDiffResultSize= ((nWidth * 8 + 31) / 32 * 4) * nHeight;  //四字节对齐的方式，按行列保存图像比较数据
        m_pDiffResult = new BYTE[m_nDiffResultSize];
        ZeroMemory(m_pDiffResult, m_nDiffResultSize);

        m_nGifBufferSize = sizeof(GifPixelType) * nWidth * nHeight;
        m_pGifBuffer = new GifByteType[m_nGifBufferSize];
        ZeroMemory(m_pGifBuffer, m_nGifBufferSize);

        m_pColorMap = new GifColorType[MAX_COLOR_MAP_COUNT];
        ZeroMemory(m_pColorMap, sizeof(GifColorType) * MAX_COLOR_MAP_COUNT);

        m_bFirstImage = TRUE;
        m_bDelayImage = FALSE;
        m_nFrameIndex = 0;
        m_nWrittenFrameIndex = 0;
        m_nPreviousBpp = 0;
        m_nPreviousBmpDataLength = 0;

        FTLASSERT(NULL == m_pThreadMaker);
        FTLASSERT(NULL == m_pColorQuantizer);
        if (NULL == m_pThreadMaker)
        {
            SAFE_DELETE(m_pColorQuantizer);
            m_pColorQuantizer = new FTL::CFOctreeColorQuantizer();
            m_pThreadMaker = new FTL::CFThread<>();
            m_pWaitingFrameInfoQueue = new FTL::CFProducerResumerQueue<CGifFrameInfoPtr>(1);

            API_VERIFY(m_pWaitingFrameInfoQueue->Start());
            API_VERIFY(m_pThreadMaker->Start(MakerThreadProc, this));
        }
    }
    else
    {
        //EGifOpenFileName Fail
        //SetLastError(ERROR_OPEN_FILES);
    }
    bRet = (nRet != GIF_ERROR);
    return bRet;
}

INT CGifMakerImpl::AddGifFrame(const RECT& rcFrame, BYTE* pBmpData, INT nLength, INT nBpp, DWORD dwTicket)
{
    FUNCTION_BLOCK_TRACE(500);
    FTLASSERT(m_pGifFile);
    FTLASSERT(m_nScreenBufferSize >= nLength);

    m_nFrameIndex++;
    FTLTRACE(TEXT("AddGifFrame, Index=%d, nLength=%d, nBpp=%d, dwTicket=%d, rcFrame=%s\n"), 
        m_nFrameIndex, nLength, nBpp, dwTicket, 
        FTL::CFRectDumpInfo(rcFrame).GetConvertedInfo());

    BOOL bRet = FALSE;
    INT nRet = 0;

    INT nDiffCount = nLength;
    BOOL bWriteFrameData = TRUE;
    BOOL bWriteFullRect = TRUE;

    if (m_bFirstImage)
    {
        m_bFirstImage = FALSE;
    }
    else
    {
        if (m_bEnableCompareWithPrevious 
            && ::EqualRect(&m_rcPreviousFrame, &rcFrame)
            && m_nPreviousBpp == nBpp)
        {
            FTLASSERT(nLength == m_nPreviousBmpDataLength);

            //比较差异
            RECT rcMinDiff = {0};
            INT nFrameWidth = rcFrame.right - rcFrame.left;
            INT nFrameHeight = rcFrame.bottom - rcFrame.top;
            nDiffCount = FTL::CFGdiUtil::CompareBitmapData(nFrameWidth, nFrameHeight, nBpp, m_pScreenBuffer, pBmpData, m_pDiffResult, 
                m_nDiffResultSize, &rcMinDiff);
            FTLTRACE(TEXT("[nFrameIndex=%d], dwTicket=%d, nDiffCount=%d/%d, rcMinDiff=%s\n"), m_nFrameIndex, dwTicket, nDiffCount, m_nDiffResultSize, 
                FTL::CFRectDumpInfo(rcMinDiff).GetConvertedInfo());

            if (0 == nDiffCount)
            {
                //完全相同
                bWriteFrameData = FALSE;
            }
            else
            {
                if(((rcMinDiff.right - rcMinDiff.left) < nFrameWidth) || ((rcMinDiff.bottom - rcMinDiff.top) < nFrameHeight))
                {
                    //比较不同区域的范围，比原来的范围小，拷贝数据
                    m_nWrittenFrameIndex++;
                    bWriteFullRect = FALSE;
                    INT nDiffBufferSize = 0;
                    BYTE* pDiffBuffer = _DuplicateBmpRect(pBmpData, nFrameWidth, nFrameHeight, nBpp, rcMinDiff, &nDiffBufferSize);
                    API_VERIFY(::OffsetRect(&rcMinDiff, rcFrame.left, rcFrame.top));
                    CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nWrittenFrameIndex, pDiffBuffer, nDiffBufferSize, dwTicket, rcMinDiff, nBpp, TRUE));
                    m_pWaitingFrameInfoQueue->Append(pFrameInfo, INFINITE);
                }
            }
        }
    }

    if (bWriteFrameData && bWriteFullRect)
    {
        //写Frame的全部区域
        m_nWrittenFrameIndex++;
        CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nWrittenFrameIndex, pBmpData, nLength, dwTicket, rcFrame, nBpp, FALSE));
        m_pWaitingFrameInfoQueue->Append(pFrameInfo, INFINITE);
    }

    if (0 != nDiffCount)
    {
        //不是完全相同,则备份数据
        CopyMemory(m_pScreenBuffer, pBmpData, nLength);
    }

    m_rcPreviousFrame = rcFrame;
    m_nPreviousBpp = nBpp;
    m_nPreviousBmpDataLength = nLength;

    m_bDelayImage = !bWriteFrameData;

    return m_nWrittenFrameIndex;
}

BOOL CGifMakerImpl::EndMakeGif(DWORD dwTicket, BOOL bCancelUnwritten)
{
    FUNCTION_BLOCK_TRACE(100);

    INT nRet = 0;
    INT nError = 0;
    if (m_pGifFile)
    {
        if (m_bDelayImage)
        {
            //如果最后一张是延迟写入的,即和倒数第二张完全一样
            m_nWrittenFrameIndex++;
            CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nWrittenFrameIndex, m_pScreenBuffer, m_nPreviousBmpDataLength, 
                dwTicket, m_rcPreviousFrame, m_nPreviousBpp, FALSE));
            m_pWaitingFrameInfoQueue->Append(pFrameInfo, INFINITE);
        }

        if (!bCancelUnwritten)
        {
            //等待剩下的写完
            while (m_pWaitingFrameInfoQueue->GetElementCount() != 0)
            {
                m_pThreadMaker->SleepAndCheckStop(100);
            }
        }
        
        m_pWaitingFrameInfoQueue->Stop();
        CGifFrameInfoPtr pFrameInfo;
        while (m_pWaitingFrameInfoQueue->RemoveAfterStop(pFrameInfo))
        {
            FTLTRACE(TEXT("Remove Unwritten Frame:%d\n"), pFrameInfo->nIndex);
            //remove all the waiting snap bitmap info, will clear automatic
        }
        FTLASSERT(NULL != m_pThreadMaker);
        if (m_pThreadMaker)
        {
            m_pThreadMaker->StopAndWait(INFINITE);
            SAFE_DELETE(m_pThreadMaker);
        }
        SAFE_DELETE(m_pWaitingFrameInfoQueue);

        GIF_VERIFY(EGifCloseFile(m_pGifFile, &nError), nError);
        m_pGifFile = NULL;
    }
    return FALSE;
}

BOOL CGifMakerImpl::_WriteGifData(CGifFrameInfoPtr pFrameInfo, DWORD dwPreviousTicket)
{
    FUNCTION_BLOCK_TRACE(1000);

    FTLTRACE(TEXT("_WriteGifData, %d\n "), pFrameInfo->nIndex);
    BOOL bRet = FALSE;
    INT nRet = 0;

    {
        GraphicsControlBlock gcb = {0};
        gcb.DelayTime = (pFrameInfo->dwTicket - dwPreviousTicket) / 10;
        gcb.TransparentColor = NO_TRANSPARENT_COLOR;
        gcb.UserInputFlag = 0;
        gcb.DisposalMode = DISPOSAL_UNSPECIFIED;

        GifByteType Extension[4] = {0};
        EGifGCBToExtension(&gcb, Extension);

        GIF_VERIFY(EGifPutExtension(m_pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, Extension), m_pGifFile->Error);
    }

    const RECT& rcBmp = pFrameInfo->rcTarget;
    INT nWidth = rcBmp.right - rcBmp.left;
    INT nHeight = rcBmp.bottom - rcBmp.top;

     if (m_compressType == ctFast)
    {
        nRet = GifQuantizeRGBBuffer(nWidth, nHeight, pFrameInfo->nBpp, pFrameInfo->pBmpData, m_pColorMap, m_pGifBuffer);
    }
    else{
        UINT nPaletteSize = 0;
        UINT nBufferSize = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nWidth, pFrameInfo->nBpp) * nHeight;

        API_VERIFY(m_pColorQuantizer->SetBmpInfo(nWidth, nHeight, pFrameInfo->nBpp, pFrameInfo->pBmpData, nBufferSize, FALSE));
        API_VERIFY(m_pColorQuantizer->ProcessQuantizer(m_nWantColorCount, &nPaletteSize));

        COLORREF* pPalette = m_pColorQuantizer->GetPalette(&nPaletteSize);
        FTLASSERT(nPaletteSize <= MAX_COLOR_MAP_COUNT);

        ZeroMemory(m_pColorMap, sizeof(GifColorType) * MAX_COLOR_MAP_COUNT);
        for (INT i = 0; i < nPaletteSize; i++)
        {
            COLORREF color = *(pPalette + i);
            m_pColorMap[i].Red = GetRValue(color);
            m_pColorMap[i].Green = GetGValue(color);
            m_pColorMap[i].Blue = GetBValue(color);
        }

        UINT nGifBufferSize = 0;
        UCHAR* pQuantizerResult = m_pColorQuantizer->GetQuantizerResult(&nGifBufferSize);
        FTLASSERT(nGifBufferSize == nWidth * nHeight);
        for (UINT i = 0; i < nGifBufferSize; i++)
        {
            m_pGifBuffer[i] = pQuantizerResult[i];
        }
    }

    ColorMapObject *pColorMap = GifMakeMapObject(m_nGifNumLevels, m_pColorMap);

    GIF_VERIFY(EGifPutImageDesc(m_pGifFile, rcBmp.left, rcBmp.top, nWidth, nHeight, FALSE, pColorMap), m_pGifFile->Error);

    GifByteType *Ptr = m_pGifBuffer;
    for (INT j = 0; j < nHeight; j++) 
    {
        GIF_VERIFY(EGifPutLine(m_pGifFile, Ptr, nWidth), m_pGifFile->Error);
        Ptr += nWidth;
    }
    GifFreeMapObject(pColorMap);
   
    return bRet;
}

BYTE* CGifMakerImpl::_DuplicateBmpRect(BYTE* pSrcBmpData, INT nSrcWidth, INT nSrcHeight, INT nBpp, RECT rcSrc, INT* pReturnBufSize)
{
    FTLTRACE(TEXT("_DuplicateBmpRect[%d], nSrcWidth=%d, nSrcHeight=%d, rcSrc=%s\n"), m_nFrameIndex, nSrcWidth, nSrcHeight, 
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
        for (INT j = rcSrc.top; j < rcSrc.bottom ; j++)
        {
            BYTE* pSrc = pSrcBmpData + j * dwOldWidthBytes + rcSrc.left * nBpp / 8;
            BYTE* pDest = pBufResult + (j - rcSrc.top) * dwNewWidthBytes;

            CopyMemory(pDest, pSrc, (rcSrc.right - rcSrc.left) * nBpp / 8);
        }
    }
    if (pReturnBufSize)
    {
        *pReturnBufSize = dwNewBmpSize;
    }
#if 1
    {
        static int index = 1;
        BOOL bRet = FALSE;
        FTL::CFCanvas canvas;
        API_VERIFY(canvas.Create(NULL, rcSrc.right - rcSrc.left, rcSrc.bottom - rcSrc.top, nBpp));
        CopyMemory(canvas.GetBuffer(), pBufResult, dwNewBmpSize);
        FTL::CFStringFormater formaterName;
        formaterName.Format(TEXT("DuplicateBmpRect_%d.bmp"), index++);
        API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), formaterName));
    }
#endif 
    return pBufResult;
}

VOID CGifMakerImpl::_FreeDuplicateBmpData(BYTE* pData)
{
    SAFE_DELETE_ARRAY(pData);
}

