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
        m_pResultPalette = NULL;
        m_pResultBuffer = NULL;
    }

    CFColorQuantizerBase::~CFColorQuantizerBase(){
        SAFE_DELETE_ARRAY(m_pResultBuffer);
        SAFE_DELETE_ARRAY(m_pResultPalette);
        SAFE_DELETE_ARRAY(m_pBmpData);
    }

    BOOL CFColorQuantizerBase::SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize)
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

        SAFE_DELETE_ARRAY(m_pBmpData);

        if (nBmpDataSize > 0)
        {
            m_pBmpData = new BYTE[nBmpDataSize];
            if (m_pBmpData)
            {
                m_nWidth = nWidth;
                m_nHeight = nHeight;
                m_nBpp = nBpp;
                m_nBmpLineBytes = nBmpLineBytes;
                m_nBmpDataSize = nBmpDataSize;
                CopyMemory(m_pBmpData, pBmpData, nBmpDataSize);
            }
            else{
                SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                return FALSE;
            }
        }

        return TRUE;
    }

    BOOL CFColorQuantizerBase::ProcessQuantizer(UINT colorCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(100);
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
}
