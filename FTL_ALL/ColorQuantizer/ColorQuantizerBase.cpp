#include "stdafx.h"
#include "ColorQuantizerBase.h"
#include <ftlGdi.h>

namespace FTL
{
    CFColorQuantizerBase::CFColorQuantizerBase(){
        m_nWidth = 0;
        m_nHeight = 0;
        m_nBpp = 0;
        m_nBmpDataSize = 0;
        m_nBmpLineBytes = 0;
        m_nPaletteItemCount = 0;
        m_nBmpPixelCount = 0;

        m_bCopyMem = FALSE;
        m_pBmpData = NULL;
        m_pResultPalette = NULL;
        m_pPixelClrList = NULL;
    }

    CFColorQuantizerBase::~CFColorQuantizerBase()
    {
        FUNCTION_BLOCK_TRACE(100);
        SAFE_DELETE_ARRAY(m_pResultPalette);
        SAFE_DELETE_ARRAY(m_pPixelClrList);

        if (m_bCopyMem)
        {
            SAFE_DELETE_ARRAY(m_pBmpData);
        }
        else
        {
            m_pBmpData = NULL;
        }
    }

    BOOL CFColorQuantizerBase::SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize, BOOL bCopyMem)
    {
        BOOL bRet = FALSE;

        UINT nBmpLineBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nWidth, nBpp);
        UINT nBmpDataSizeCheck = nBmpLineBytes * nHeight; 
        FTLASSERT(nBmpDataSizeCheck == nBmpDataSize && TEXT("BMP图片的大小需要四字节对齐"));
        if (nBmpDataSizeCheck != nBmpDataSize)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }

        if (m_bCopyMem)
        {
            if (m_nBmpDataSize != nBmpDataSize)
            {
                //当旧的数据是拷贝出来的，且大小不一样时，清除旧的
                SAFE_DELETE_ARRAY(m_pBmpData);
            }
        }
        else
        {
            m_pBmpData = NULL;
        }

        if (nBmpDataSize > 0)
        {
            if (bCopyMem)
            {
                if(!m_pBmpData){
                    m_pBmpData = new BYTE[nBmpDataSize];
                }
                
                if (m_pBmpData)
                {
                    CopyMemory(m_pBmpData, pBmpData, nBmpDataSize);
                }
                else{
                    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                    return FALSE;
                }
            }
            else
            {
                m_pBmpData = pBmpData;
            }
        }

        m_nWidth = nWidth;
        m_nHeight = nHeight;
        m_nBpp = nBpp;
        m_nBmpLineBytes = nBmpLineBytes;
        m_nBmpDataSize = nBmpDataSize;
        m_bCopyMem = bCopyMem;
        m_nBmpPixelCount = nWidth * nHeight;

        return TRUE;
    }

    BOOL CFColorQuantizerBase::ProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(500);
        BOOL bRet = FALSE;
        API_VERIFY(OnPrepare());
        if (bRet)
        {
            API_VERIFY(OnProcessQuantizer(nWantClrCount, pResultClrCount));

            OnFinish();
        }
        return bRet;
    }

    BOOL CFColorQuantizerBase::OnPrepare()
    {
        return TRUE;
    }
    void CFColorQuantizerBase::OnFinish()
    {
    }

    COLORREF* CFColorQuantizerBase::GetPalette(UINT* pResultCount)
    {
        if (pResultCount)
        {
            if (m_pResultPalette){
                *pResultCount = m_nPaletteItemCount;
            }
            else{
                *pResultCount = 0;
            }
        }
        return m_pResultPalette;
    }

    UCHAR* CFColorQuantizerBase::GetQuantizerResult(UINT* pBufferSize)
    {
        FTLASSERT(m_QuantizerResultIndexes.size() == m_nWidth * m_nHeight);

        if (pBufferSize)
        {
            *pBufferSize = m_QuantizerResultIndexes.size();
        }
        return &m_QuantizerResultIndexes[0];
    }

    BOOL CFColorQuantizerBase::_AnalyzeColorMeta()
    {
        FTLASSERT(24 == m_nBpp || 32 == m_nBpp);
        if (24 != m_nBpp && 32 != m_nBpp)
        {
            return FALSE;
        }

        SAFE_DELETE_ARRAY(m_pPixelClrList);
        
        m_pPixelClrList = new COLORREF[m_nBmpPixelCount];
        ZeroMemory(m_pPixelClrList, sizeof(COLORREF) * m_nBmpPixelCount);

        //UINT nColorCount = m_nWidth * m_nHeight;
        UINT nPixOffset = (m_nBpp / 8);
        UINT nRowBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(m_nWidth, m_nBpp); //4字节对齐，计算每行的字节数
        UINT nPixelIndex = 0;

        for (UINT h = 0; h < m_nHeight; h++)
        {
            BYTE* pBuf = m_pBmpData + (nRowBytes * h);
            for (UINT w = 0; w < m_nWidth; w++)
            {
                UCHAR Alpha = 0xFF;
                UCHAR Red = *(pBuf + 2);
                UCHAR Green = *(pBuf+1);
                UCHAR Blue = *(pBuf);

                if (32 == m_nBpp)
                {
                    Alpha = *(pBuf + 3);
                }
                //COLORREF clr = MAKE_RGBA(Red, Green, Blue, Alpha);
                //m_clrList.push_back(clr);
                m_pPixelClrList[nPixelIndex++] = MAKE_RGBA(Red, Green, Blue, Alpha);

                pBuf += nPixOffset;
            }
        }
        FTLASSERT(nPixelIndex == m_nBmpPixelCount);

        return TRUE;
    }
}
