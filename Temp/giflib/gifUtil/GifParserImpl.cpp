#include "StdAfx.h"
#include "GifParserImpl.h"

CGifParserImpl::CGifParserImpl(void)
{
    m_pCallBack = NULL;
    m_callbackData = 0;

    m_nFrameIndex = 0;
    m_nGifBufferSize = 0;
    m_nTargetBpp = 32;
    m_pGifBuffer = NULL;
    m_pGifFile = NULL;
    m_pBmpBuffer = NULL;
    m_nBmpBufferSize = 0;
}

CGifParserImpl::~CGifParserImpl(void)
{
    FTLASSERT(NULL == m_pGifFile);
    _InnerCloseFile();
}

VOID CGifParserImpl::Release()
{
    delete this;
}

BOOL CGifParserImpl::OpenGifFile(LPCTSTR pszFileName)
{
    BOOL bRet = FALSE;
    INT nRet = GIF_ERROR;
    FTLASSERT(NULL == m_pGifFile);

    if (NULL == m_pGifFile)
    {
       INT nError = 0;
       m_pGifFile = DGifOpenFileName(pszFileName, &nError);
       bRet = (NULL != m_pGifFile);
       if (bRet)
       {
           m_nGifBufferSize = m_pGifFile->SWidth * m_pGifFile->SHeight;
           m_pGifBuffer = new GifPixelType[m_nGifBufferSize];
           ZeroMemory(m_pGifBuffer, m_nGifBufferSize * sizeof(GifPixelType));
           //_FillBackGroundColor(m_pGifFile->SBackGroundColor);
       }
       else
       {
            GIF_VERIFY(GIF_ERROR, nError);
       }
    }

    return bRet;
}

INT CGifParserImpl::_innerHandleRecordType(GifRecordType recordType)
{
    INT nRet = GIF_ERROR;
    switch (recordType)
    {
    case SCREEN_DESC_RECORD_TYPE:
        {
            break;
        }
    case IMAGE_DESC_RECORD_TYPE:    //图像数据块
        {
            GIF_VERIFY(_InnerHandleImageDescRecord(), nRet);
            break;
        }
    case EXTENSION_RECORD_TYPE:
        {
            GIF_VERIFY(_InnerHandleExtensionRecord(), nRet);
            break;
        }
    case TERMINATE_RECORD_TYPE:
        {
            nRet = GIF_OK;
            break;
        }
    default:
        {
            FTLASSERT(FALSE);
            break;
        }
    }

    return nRet;
}

INT CGifParserImpl::_InnerHandleImageDescRecord()
{
    BOOL bRet = FALSE;
    INT nRet = GIF_ERROR;
    GIF_VERIFY(DGifGetImageDesc(m_pGifFile), nRet);
    if (nRet != GIF_ERROR)
    {
        _FillBackGroundColor(m_pGifFile->SBackGroundColor);

        //交织处理
        int InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
        int InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */

        m_nFrameIndex++;
        INT nLeft = m_pGifFile->Image.Left;
        INT nTop = m_pGifFile->Image.Top;
        INT nWidth = m_pGifFile->Image.Width;
        INT nHeight = m_pGifFile->Image.Height;

        /* Image Position relative to Screen. */
        RECT rcFrame = {0}; 
        API_VERIFY(::SetRect(&rcFrame, nLeft, nTop, nLeft + nWidth, nTop + nHeight));

        FTLTRACE(TEXT("ImageParser [%d], rect=%s\n"), m_nFrameIndex, FTL::CFRectDumpInfo(rcFrame).GetConvertedInfo());
        if (rcFrame.right > m_pGifFile->SWidth || rcFrame.bottom > m_pGifFile->SHeight)
        {
            FTLTRACEEX(FTL::tlError, TEXT("Frame %d is not confined to screen dimension\n"), m_nFrameIndex);
            nRet = D_GIF_ERR_IMAGE_DEFECT;
        }

        if (m_pGifFile->Image.Interlace){
            /* Need to perform 4 passes on the images: */
            for (int i = 0; i < 4; i++){
                for (int j = rcFrame.top + InterlacedOffset[i]; j < rcFrame.bottom; j += InterlacedJumps[i]) {
                    GifPixelType* pLineBuf = (m_pGifBuffer + j * nWidth + rcFrame.left);
                    GIF_VERIFY(DGifGetLine(m_pGifFile, pLineBuf , nWidth), nRet);
                }
            }

        }
        else {
            //for (int i = 0; i < nHeight; i++) 
            for (int i = nTop; i < nTop + nHeight ; i++)
            {
                GifPixelType* pLineBuf = (m_pGifBuffer + i * nWidth + nLeft);
                GIF_VERIFY(DGifGetLine(m_pGifFile, pLineBuf, nWidth), nRet);
            }
        }

        ColorMapObject *pColorMap = m_pGifFile->Image.ColorMap;
        if (!pColorMap)
        {
            pColorMap = m_pGifFile->SColorMap;
        }

        GIF_VERIFY(_InnerAnalyze2FrameInfo(rcFrame, pColorMap), nRet);
    }
    return nRet;
}

INT CGifParserImpl::_InnerAnalyze2FrameInfo(const RECT& rcFrame, ColorMapObject* pColorMap)
{
    INT nRet = GIF_ERROR;
    
    if (pColorMap && m_pGifBuffer)
    {
        INT nScreenWidth = m_pGifFile->SWidth;
        INT nScreenHeight = m_pGifFile->SHeight;
        INT nFrameWidth = rcFrame.right - rcFrame.left;

        INT nWidthBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nScreenWidth, m_nTargetBpp);
        //INT nTotalSize = nWidthBytes * nScreenHeight;
        int nPixelSize = m_nTargetBpp / 8;

        //BYTE* pBmpBuffer = new BYTE[nTotalSize];

        int nTransparentColorIndex = m_gifControlInfo.nTransparentColorIndex;
        for (INT h = 0; h < nScreenHeight; h++)
        {
            GifPixelType* pLineBuf = m_pGifBuffer + h * nScreenWidth;
            BYTE* pBmpColor = m_pBmpBuffer + (h * nWidthBytes);

            for (INT w = 0; w < nScreenWidth; w++)
            {
                INT nLineIndex = pLineBuf[w];
#if 1
                if (m_nFrameIndex != 1      //不处理第一张图的透明色
                    && nTransparentColorIndex != NO_TRANSPARENT_COLOR
                    && nTransparentColorIndex == nLineIndex)
                {
                    pBmpColor += nPixelSize;
                    continue;
                }
#endif 
                GifColorType& ColorMapEntry = pColorMap->Colors[nLineIndex];
                *pBmpColor++ = ColorMapEntry.Blue;
                *pBmpColor++ = ColorMapEntry.Green;
                *pBmpColor++ = ColorMapEntry.Red;
                if (m_nTargetBpp == 32)
                {
                    *pBmpColor++ = 0xFF;
                }
            }
        }

        nRet = GIF_OK;
        if (m_pCallBack)
        {
            if(!m_pCallBack->OnParseGifFrame(m_nFrameIndex, m_gifControlInfo, rcFrame, m_pBmpBuffer, m_nBmpBufferSize, m_callbackData))
            {
                //User Cancel
                FTLTRACE(TEXT("OnParseGifFrame Return FALSE, indicate user cancel\n"));
                nRet = GIF_ERROR;
            }
        }
        
        //CGifFrameInfoPtr pFrameInfo(new CGifFrameInfo(m_nFrameIndex, m_pBmpBuffer, m_nBmpBufferSize, 0, rcFrame, m_nTargetBpp, FALSE));
        //m_ResultFrames.push_back(pFrameInfo);
    }
    return nRet;
}

INT CGifParserImpl::_InnerHandleExtensionRecord()
{
    INT nRet = GIF_ERROR;
    GifByteType *pExtension = NULL;
    int ExtCode = 0;
    ZeroMemory(&m_gifControlInfo, sizeof(m_gifControlInfo));

    GIF_VERIFY(DGifGetExtension(m_pGifFile, &ExtCode, &pExtension), m_pGifFile->Error);
    if (GIF_ERROR != nRet)
    {
        switch (ExtCode)
        {
        case GRAPHICS_EXT_FUNC_CODE: //GIF_CONTROL_EXT_CODE:
            {
                FTLASSERT(pExtension[0] == 4); //GIF_CONTROL_EXT_SIZE
                GraphicsControlBlock gcb = {0};
                GIF_VERIFY(DGifExtensionToGCB(pExtension[0], pExtension+1, &gcb), nRet);
                if (GIF_ERROR != nRet)
                {
                    m_gifControlInfo.nDelayMilliSeconds = gcb.DelayTime * 10;
                    m_gifControlInfo.nTransparentColorIndex = gcb.TransparentColor;
                }
                break;
            }
        }

        while (pExtension != NULL) {
            GIF_VERIFY(DGifGetExtensionNext(m_pGifFile, &pExtension), m_pGifFile->Error);
        }
    }
    
    return nRet;
}

INT CGifParserImpl::ParseGif(INT nTargetBpp, IGifParserCallback* pCallBack, DWORD_PTR callbackData)
{
    FTLASSERT(24 == nTargetBpp || 32 == nTargetBpp);
    INT nRet = 0;
    INT nError = 0;
    m_nTargetBpp = nTargetBpp;
    m_pCallBack = pCallBack;
    m_callbackData = callbackData;

    if (m_pGifFile)
    {
        if (m_pCallBack)
        {
            m_pCallBack->OnGetGifInfo(m_pGifFile->SWidth, m_pGifFile->SHeight, m_callbackData);
        }

        FTLASSERT(NULL == m_pBmpBuffer);

        m_nBmpBufferSize = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(m_pGifFile->SWidth, m_nTargetBpp) * m_pGifFile->SHeight;
        m_pBmpBuffer = new BYTE[m_nBmpBufferSize];
        ZeroMemory(m_pBmpBuffer, m_nBmpBufferSize);

        GifRecordType recordType = UNDEFINED_RECORD_TYPE;
        do {

            GIF_VERIFY(DGifGetRecordType(m_pGifFile, &recordType), nRet);
            if (GIF_ERROR == nRet){
                break;
            }

            GIF_VERIFY(_innerHandleRecordType(recordType), nRet);
            if (GIF_ERROR == nRet){
                break;
            }
        }while(recordType != TERMINATE_RECORD_TYPE);
    }
    
    return 0;
}


VOID CGifParserImpl::CloseFile()
{
    _InnerCloseFile();
}

VOID CGifParserImpl::_InnerCloseFile()
{
    if (NULL != m_pGifFile)
    {
        INT nError = 0;
        INT nRet = 0;
        GIF_VERIFY(DGifCloseFile(m_pGifFile, &nError), nError);
        m_pGifFile = NULL;
    }

    SAFE_DELETE_ARRAY(m_pGifBuffer);
    SAFE_DELETE_ARRAY(m_pBmpBuffer);
}

VOID CGifParserImpl::_FillBackGroundColor(GifPixelType backGroundColorIndex)
{
    FTLASSERT(m_pGifFile);
    FTLASSERT(m_pGifBuffer);
    FTLASSERT(m_nGifBufferSize > 0 && m_nGifBufferSize == (m_pGifFile->SWidth * m_pGifFile->SHeight));

    if (m_pGifBuffer && m_pGifFile)
    {
        FTLASSERT(0 <= backGroundColorIndex && backGroundColorIndex < 0xFF);
        FillMemory(m_pGifBuffer, m_nGifBufferSize, backGroundColorIndex); //填充背景色
    }
}
