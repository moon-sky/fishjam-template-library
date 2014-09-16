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
    
        m_pBmpData = NULL;
        m_bCopyMem = FALSE;
        m_pResultPalette = NULL;
        m_pResultBuffer = NULL;
    }

    CFColorQuantizerBase::~CFColorQuantizerBase(){
        SAFE_DELETE_ARRAY(m_pResultBuffer);
        SAFE_DELETE_ARRAY(m_pResultPalette);

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
        m_bCopyMem = bCopyMem;

        UINT nBmpLineBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nWidth, nBpp);
        UINT nBmpDataSizeCheck = nBmpLineBytes * nHeight; 
        FTLASSERT(nBmpDataSizeCheck == nBmpDataSize && TEXT("BMP图片的大小需要四字节对齐"));

        m_nWidth = nWidth;
        m_nHeight = nHeight;
        m_nBpp = nBpp;
        m_nBmpLineBytes = nBmpLineBytes;
        m_nBmpDataSize = nBmpDataSize;

        if (nBmpDataSizeCheck != nBmpDataSize)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }

        if (bCopyMem)
        {
            SAFE_DELETE_ARRAY(m_pBmpData);
        }

        if (nBmpDataSize > 0)
        {
            if (bCopyMem)
            {
                m_pBmpData = new BYTE[nBmpDataSize];
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

        return TRUE;
    }

    BOOL CFColorQuantizerBase::ProcessQuantizer(UINT colorCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(500);
        BOOL bRet = FALSE;
        API_VERIFY(OnPrepare());
        if (bRet)
        {
            API_VERIFY(OnProcessQuantizer(colorCount, pResultClrCount));

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
        if (pResultCount && m_pResultPalette)
        {
            *pResultCount = m_nPaletteItemCount;
        }
        return m_pResultPalette;
    }

    int* CFColorQuantizerBase::GetQuantizerBuffer(INT* pSize)
    {
        if (pSize)
        {
            *pSize = m_nWidth * m_nHeight;
        }
        return &m_indices[0];
    }

    void CFColorQuantizerBase::_AnalyzeColorMeta()
    {
        m_redList.clear();
        m_greenList.clear();
        m_blueList.clear();

        FTLASSERT(24 == m_nBpp || 32 == m_nBpp);

        UINT nColorCount = m_nWidth * m_nHeight;
        UINT nPixOffset = (m_nBpp / 8);
        UINT nRowBytes = (m_nWidth * m_nBpp + 31) >> 5 << 2;  //4字节对齐，计算每行的字节数 //<< 2

        for (UINT h = 0; h < m_nHeight; h++)
        {
            BYTE* pBuf = m_pBmpData + (nRowBytes * h);
            for (UINT w = 0; w < m_nWidth; w++)
            {
                unsigned char Red = *(pBuf + 2);
                unsigned char Green = *(pBuf+1);
                unsigned char Blue = *(pBuf);
                
                m_redList.push_back(Red);
                m_greenList.push_back(Green);
                m_blueList.push_back(Blue);

                pBuf += nPixOffset;
            }
        }

    }
}
