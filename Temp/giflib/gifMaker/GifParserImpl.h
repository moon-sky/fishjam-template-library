#pragma once

#include "../giflib/lib/gif_lib.h"

#include "gifMaker.h"
#include "gifUtilDefine.h"
#include "GifFrameInfo.h"


class CGifParserImpl : public IGifParser
{
public:
    CGifParserImpl(void);
    virtual ~CGifParserImpl(void);

public:
    virtual BOOL OpenGifFile(LPCTSTR pszFileName);
    virtual INT ParseGif(INT nTargetBpp, IGifParserCallback* pCallBack, DWORD_PTR callbackData); //24 - RGB, 32 - ARGB
    //virtual BOOL GetGifImage(INT nIndex, BYTE* pBmpData, INT *pLength);
    virtual VOID CloseFile();
    virtual void Release();
private:
    IGifParserCallback* m_pCallBack;
    DWORD_PTR           m_callbackData;

    INT     m_nFrameIndex;
    INT     m_nGifBufferSize;
    INT     m_nTargetBpp;

    GifPixelType*   m_pGifBuffer;   //量化的像素索引值
    GifFileType*    m_pGifFile;

    GifControlInfo      m_gifControlInfo;
    BYTE*               m_pBmpBuffer;
    INT                 m_nBmpBufferSize;
    

    //typedef std::list<CGifFrameInfoPtr>     GifFrameInfoPtrList;
    //GifFrameInfoPtrList m_ResultFrames;
private:
    VOID _InnerCloseFile(); //避免析构中调用虚拟函数
    VOID _FillBackGroundColor(GifPixelType backGroundColorIndex);

    INT _innerHandleRecordType(GifRecordType recordType);
    INT _InnerHandleImageDescRecord();
    INT _InnerHandleExtensionRecord();
    INT _InnerAnalyze2FrameInfo(const RECT& rcFrame, ColorMapObject* pColorMap);
};
