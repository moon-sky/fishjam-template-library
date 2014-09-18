#include "StdAfx.h"
#include "GifMakerImpl.h"

#ifdef _DEBUG
#  pragma comment(lib, "../Debug/giflib.lib")
#else
#  pragma comment(lib, "../Release/giflib.lib")
#endif 


CGifMakerImpl::CGifMakerImpl()
:m_WaitingFrameInfoQueue(2)
{
    m_compressType = ctHighQuality;
    m_nPreBmpBytes = 0;
    m_nDiffResultSize = 0;
    m_bWriteFirst = TRUE;

    m_nWidth = 0;
    m_nHeight = 0;
    m_nBpp = 0;
    m_pThreadMaker = NULL;
    
    m_pPreBmp = NULL;
    m_pDiffResult = NULL;
    m_pGifFile = NULL;

    m_pColorMap = NULL;
    m_pGifBuffer = NULL;
    m_pColorQuantizer = NULL;

    //m_pQuantizer = new CQuantizer(256, 8);

    m_bFirstImage = TRUE;
    m_bDelayImage = FALSE;
    //m_pGiffDiffBuffer = NULL;

    ZeroMemory(&m_rcDiff, sizeof(m_rcDiff));

    m_nGifColorRes = 8;
    m_nGifNumLevels = 256;
    m_dwLastTicket = 0;
    m_nImgIndex = 0;
    m_nGifBufferSize = 0;
}

CGifMakerImpl::~CGifMakerImpl()
{
    FTLASSERT(m_pGifFile == NULL);
    //SAFE_DELETE(m_pQuantizer);
    SAFE_DELETE(m_pColorQuantizer);

    SAFE_DELETE_ARRAY(m_pColorMap);
    SAFE_DELETE_ARRAY(m_pGifBuffer);
    SAFE_DELETE_ARRAY(m_pPreBmp);
    SAFE_DELETE_ARRAY(m_pDiffResult);
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
    FUNCTION_BLOCK_TRACE(0);

    FTLASSERT(m_pGifFile);
    DWORD dwResult = 0;
    if (m_pGifFile)
    {
        CGifFrameInfoPtr    pFrameInfo;
        FTL::FTLThreadWaitType waitType = FTL::ftwtStop;
        do 
        {
            waitType = m_WaitingFrameInfoQueue.Remove(pFrameInfo, INFINITE);
            if (FTL::ftwtContinue == waitType)
            {
                //TODO: handle bitmap data
                _WriteGifData(pFrameInfo);
            }
            pFrameInfo.reset();
        } while (FTL::ftwtContinue == waitType);


    }

    dwResult = 0;
    return dwResult;
}



BOOL CGifMakerImpl::BeginMakeGif(INT nWidth, INT nHeight, INT bpp, LPCTSTR pszFileName)
{
    BOOL bRet = FALSE;

    INT nRet = 0;
    INT nError = 0;

    m_pGifFile = EGifOpenFileName(pszFileName, false, &nError);
    if (m_pGifFile)
    {
        GIF_VERIFY(EGifPutScreenDesc(m_pGifFile, nWidth, nHeight, bpp, 0, NULL), m_pGifFile->Error);
        EGifSetGifVersion(m_pGifFile, true);

        m_nWidth = nWidth;
        m_nHeight = nHeight;
        m_nBpp = bpp;

        m_nPreBmpBytes = ((nWidth * bpp + 31) / 32 * 4) * nHeight;  //四字节对齐
        m_pPreBmp = new BYTE[m_nPreBmpBytes];
        ZeroMemory(m_pPreBmp, m_nPreBmpBytes);

        m_nDiffResultSize= ((nWidth * 8 + 31) / 32 * 4) * nHeight;  //四字节对齐的
        m_pDiffResult = new BYTE[m_nDiffResultSize];
        ZeroMemory(m_pDiffResult, m_nDiffResultSize);

        m_nGifBufferSize = sizeof(GifPixelType) * nWidth * nHeight;
        m_pGifBuffer = new GifByteType[m_nGifBufferSize];
        ZeroMemory(m_pGifBuffer, m_nGifBufferSize);

        m_pColorMap = new GifColorType[MAX_COLOR_MAP_COUNT];
        ZeroMemory(m_pColorMap, sizeof(GifColorType) * MAX_COLOR_MAP_COUNT);

        m_bFirstImage = TRUE; 
        m_nImgIndex = 0;

        FTLASSERT(NULL == m_pThreadMaker);
        if (NULL == m_pThreadMaker)
        {
            m_pColorQuantizer = new FTL::CFOctreeColorQuantizer();
            m_pThreadMaker = new FTL::CFThread<>();

            API_VERIFY(m_WaitingFrameInfoQueue.Start());
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

BOOL CGifMakerImpl::AddGifImage(BYTE* pBmpData, INT nLength, DWORD dwTicket)
{
    INT nRet = 0;
    FUNCTION_BLOCK_TRACE(500);
    FTLASSERT(m_pGifFile);
    FTLASSERT(m_nPreBmpBytes == nLength);
    m_nImgIndex++;

    BOOL bWriteImageData = FALSE;
    FTLTRACE(TEXT("AddGifImage, Index=%d, nLength=%d, dwTicket=%d\n"), m_nImgIndex, nLength, dwTicket);
    if (m_bFirstImage)
    {
        m_bFirstImage = FALSE;
        m_dwLastTicket = dwTicket;
        CopyMemory(m_pPreBmp, pBmpData, nLength);
        ::SetRect(&m_rcDiff, 0, 0, m_nWidth, m_nHeight);
        CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nImgIndex, pBmpData, nLength, dwTicket, m_rcDiff, m_nBpp, FALSE));
        m_WaitingFrameInfoQueue.Append(pFrameInfo, INFINITE);
        //m_pGiffDiffBuffer = _DuplicateBmpRect(pBmpData, m_nWidth, m_nHeight, m_nBpp, m_rcDiff);
    }
    else
    {
        RECT rcMinDiff = {0};
        INT nDiffCount = FTL::CFGdiUtil::CompareBitmapData(m_nWidth, m_nHeight, m_nBpp, m_pPreBmp, pBmpData, m_pDiffResult, 
            m_nDiffResultSize, &rcMinDiff);
        FTLTRACE(TEXT("[ImageIndex=%d], dwTicket=%d, nDiffCount=%d/%d, rcMinDiff=%s\n"), m_nImgIndex, dwTicket, nDiffCount, m_nDiffResultSize, 
            FTL::CFRectDumpInfo(rcMinDiff).GetConvertedInfo());
        if (0 == nDiffCount)
        {
            bWriteImageData = FALSE;
        }else{
            bWriteImageData = TRUE;

            //if (m_pGiffDiffBuffer)
            //{
            //    _WriteGifData(m_pGiffDiffBuffer, m_rcDiff, dwTicket);
            //    _FreeDuplicateBmpData(m_pGiffDiffBuffer);
            //    m_pGiffDiffBuffer = NULL;
            //    ZeroMemory(&m_rcDiff, sizeof(m_rcDiff));
            //}
            {
                //if (nDiffCount * 10 / m_nDiffResultSize < 6  //不同的数小于 60%
                //    && ((rcMinDiff.right - rcMinDiff.left) < m_nWidth)
                //    && ()
                if(((rcMinDiff.right - rcMinDiff.left) < m_nWidth) || ((rcMinDiff.bottom - rcMinDiff.top) < m_nHeight))
                {
                    //need copy
                    m_rcDiff = rcMinDiff;
                    INT nDiffBufferSize = 0;
                    BYTE* pDiffBuffer = _DuplicateBmpRect(pBmpData, m_nWidth, m_nHeight, m_nBpp, m_rcDiff, &nDiffBufferSize);
                    CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nImgIndex, pDiffBuffer, nDiffBufferSize, dwTicket, m_rcDiff, m_nBpp, TRUE));
                    m_WaitingFrameInfoQueue.Append(pFrameInfo, INFINITE);
                    //m_pGiffDiffBuffer = _DuplicateBmpRect(pBmpData, m_nWidth, m_nHeight, m_nBpp, m_rcDiff);
                }
                else{
                    //write all
                    RECT rcTotal = {0, 0, m_nWidth, m_nHeight};
                    CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nImgIndex, pBmpData, nLength, dwTicket, rcTotal, m_nBpp, FALSE));
                    m_WaitingFrameInfoQueue.Append(pFrameInfo, INFINITE);
                    //_WriteGifData(pBmpData, rcTotal, dwTicket);
                }
            }
            CopyMemory(m_pPreBmp, pBmpData, nLength);
        }
    }
    m_bDelayImage = !bWriteImageData;

    return TRUE;
}

BOOL CGifMakerImpl::EndMakeGif(DWORD dwTicket, DWORD dwWaitTimeOut /* = INFINITE */)
{
    INT nRet = 0;
    INT nError = 0;
    if (m_pGifFile)
    {
        //if (!m_bDelayImage)
        //{
        //    RECT rcTotal = {0, 0, m_nPreWidth, m_nPreHeight};
        //    _WriteGifData(m_pPreBmp, rcTotal, dwTicket);
        //}

        //RECT rcTotal = {0, 0, m_nWidth, m_nHeight};
        //SnapBiampInfoPtr pSnapBitmapInfo(new SnapBiampInfo(m_nImgIndex, m_pPreBmp, m_nPreBmpBytes, dwTicket, rcTotal, m_nBpp, FALSE));
        //m_WaitingFrameInfoQueue.Append(pSnapBitmapInfo, INFINITE);

        if (INFINITE == dwWaitTimeOut)
        {
            while (m_WaitingFrameInfoQueue.GetElementCount() != 0)
            {
                m_pThreadMaker->SleepAndCheckStop(100);
            }
        }
        
        m_WaitingFrameInfoQueue.Stop();
        CGifFrameInfoPtr pFrameInfo;
        while (m_WaitingFrameInfoQueue.RemoveAfterStop(pFrameInfo))
        {
            FTLTRACE(TEXT("Remove Un Written Image:%d\n"), pFrameInfo->nIndex);
            //remove all the waiting snap bitmap info, will clear automatic
        }

        m_pThreadMaker->StopAndWait(dwWaitTimeOut);
        //if (m_pGiffDiffBuffer)
        //{
        //    _WriteGifData(m_pGiffDiffBuffer, m_rcDiff, dwTicket);
        //    _FreeDuplicateBmpData(m_pGiffDiffBuffer);
        //    m_pGiffDiffBuffer = NULL;
        //    ZeroMemory(&m_rcDiff, sizeof(m_rcDiff));
        //}

        GIF_VERIFY(EGifCloseFile(m_pGifFile, &nError), nError);
        m_pGifFile = NULL;
    }
    return FALSE;
}

BOOL CGifMakerImpl::_WriteGifData(CGifFrameInfoPtr pFrameInfo)
{
    //BYTE* pBmpData, const RECT& rcBmp,DWORD dwTicket
    FTLTRACE(TEXT("_WriteGifData, %d\n "), pFrameInfo->nIndex);
    INT nRet = 0;
    m_bWriteFirst = FALSE;
    //if (bWriteGraphControl)
    {
        GifByteType Extension[4] = {0};
        GraphicsControlBlock gcb = {0};

        gcb.DelayTime = (pFrameInfo->dwTicket - m_dwLastTicket) / 10;
        gcb.TransparentColor = NO_TRANSPARENT_COLOR;
        gcb.UserInputFlag = 0;
        gcb.DisposalMode = DISPOSAL_UNSPECIFIED;
        EGifGCBToExtension(&gcb, Extension);

        //unsigned char ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
        //ExtStr[0] = 0x04;
        //ExtStr[1] = dwElapse % 256;
        //ExtStr[2] = dwElapse / 256;

        GIF_VERIFY(EGifPutExtension(m_pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, Extension), m_pGifFile->Error);

        m_dwLastTicket = pFrameInfo->dwTicket;
    }

    const RECT& rcBmp = pFrameInfo->rcTarget;
    INT nWidth = rcBmp.right - rcBmp.left;
    INT nHeight = rcBmp.bottom - rcBmp.top;

    //if (bWriteImageData)
    {
        if (m_compressType == ctFast)
        {
            nRet = GifQuantizeRGBBuffer(nWidth, nHeight, m_nBpp, pFrameInfo->pBmpData, m_pColorMap, m_pGifBuffer);
        }
        else{
            UINT nPaletteSize = 0;
            UINT nBufferSize = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nWidth, m_nBpp) * nHeight;

            //FTL::IFColorQuantizer* pColorQuantizer = new FTL::CFWuColorQuantizer();
            //FTL::IFColorQuantizer* pColorQuantizer = new FTL::CFOctreeColorQuantizer();

            m_pColorQuantizer->SetBmpInfo(nWidth, nHeight, m_nBpp, pFrameInfo->pBmpData, nBufferSize, FALSE);
            m_pColorQuantizer->ProcessQuantizer(256, &nPaletteSize);
            INT nGifBufferSize = nWidth * nHeight;
            COLORREF* pPalette = m_pColorQuantizer->GetPalette(&nPaletteSize);
            ZeroMemory(m_pColorMap, sizeof(GifColorType) * MAX_COLOR_MAP_COUNT);
            for (INT i = 0; i < nPaletteSize; i++)
            {
                COLORREF color = *(pPalette + i);
                m_pColorMap[i].Red = GetRValue(color);
                m_pColorMap[i].Green = GetGValue(color);
                m_pColorMap[i].Blue = GetBValue(color);
            }
            //m_nGifNumLevels = nPaletteSize;

            INT* pQuantizerBuffer = m_pColorQuantizer->GetQuantizerBuffer(NULL);
            for (INT i = 0; i < nGifBufferSize; i++)
            {
                m_pGifBuffer[i] = pQuantizerBuffer[i];
            }
            //CopyMemory(m_pGifBuffer, &colorQuantizer.m_indices[0], m_nGifBufferSize);

            //API_VERIFY(m_pQuantizer->ProcessImage(nWidth, nHeight, m_nPreBpp, pBmpData));
            //m_pQuantizer->SetColorTable(m_pColorMap256);
            //delete pColorQuantizer;
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
    }
    return FALSE;
}

BYTE* CGifMakerImpl::_DuplicateBmpRect(BYTE* pSrcBmpData, INT nSrcWidth, INT nSrcHeight, INT nBpp, RECT rcSrc, INT* pReturnBufSize)
{
    FTLTRACE(TEXT("_DuplicateBmpRect[%d], nSrcWidth=%d, nSrcHeight=%d, rcSrc=%s\n"), m_nImgIndex, nSrcWidth, nSrcHeight, 
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
    return pBufResult;
}

VOID CGifMakerImpl::_FreeDuplicateBmpData(BYTE* pData)
{
    SAFE_DELETE_ARRAY(pData);
}

