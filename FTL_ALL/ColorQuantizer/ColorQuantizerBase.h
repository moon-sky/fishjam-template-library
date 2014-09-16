#pragma once

#include <ftlBase.h>
#include "ColorQuantizer.h"

#include <list>
namespace FTL
{
    class CFColorQuantizerBase : public IFColorQuantizer{
    public:
        CFColorQuantizerBase();
        virtual ~CFColorQuantizerBase();
    public:
        //IFColorQuantizer
        virtual BOOL SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize, BOOL bCopyMem);
        virtual BOOL ProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
        virtual COLORREF*   GetPalette(UINT* pResultCount);
        virtual int* GetQuantizerBuffer(INT* pSize);
    protected:
        virtual BOOL OnPrepare();
        virtual void OnFinish();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount) = 0;

    protected:
        //辅助性的功能函数
        void _AnalyzeColorMeta();       //生成 m_redList/m_greenList/m_blueList 的三原色列表

    protected:
        UINT m_nWidth;
        UINT m_nHeight;
        UINT m_nBpp;
        UINT m_nBmpDataSize;    //总的字节数
        UINT m_nBmpLineBytes;   //每行的字节数(4字节对齐)
        UINT m_nPaletteItemCount;

        BOOL        m_bCopyMem;
        BYTE*       m_pBmpData;
        COLORREF*   m_pResultPalette;
        UCHAR*      m_pResultBuffer;
        std::vector<int> m_indices;
        
        //三原色的列表 -- 考虑4字节对齐
        typedef std::list<unsigned char>    ColorMetaList;
        ColorMetaList   m_redList;
        ColorMetaList   m_greenList;
        ColorMetaList   m_blueList;
    };
}
