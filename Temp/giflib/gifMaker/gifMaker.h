#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GIFMAKER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GIFMAKER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef GIF_UTILS_EXPORTS
#define GIF_UTILS_API __declspec(dllexport)
#else
#define GIF_UTILS_API __declspec(dllimport)
#endif

enum GIF_UTILS_API CompressType{
    ctFast = 0,
    ctHighQuality,
};

class GIF_UTILS_API IGifMaker {
public:
    virtual ~IGifMaker();
    virtual CompressType SetCompressType(CompressType type) = 0;

    virtual BOOL BeginMakeGif(INT nWidth, INT nHeight, LPCTSTR pszFileName) = 0;
    virtual BOOL AddGifImage(const RECT& rcFrame, BYTE* pBmpData, INT nLength, INT nBpp, DWORD dwTicket) = 0;
    virtual BOOL EndMakeGif(DWORD dwTicket, DWORD dwWaitTimeOut = INFINITE) = 0;
    //virtual BOOL Pause() = 0;
    //virtual BOOL Resume() = 0;
    virtual void Release() = 0;

public:
    static IGifMaker* GetInstance();
protected:
    IGifMaker();
};

struct GifControlInfo
{
    int nDelayMilliSeconds;
    int nTransparentColorIndex;
};

class IGifParserCallback{
public:
    virtual VOID OnGetGifInfo(INT nWidth, INT nHeight, DWORD_PTR callbackData) = 0;
    virtual BOOL OnParseFrame(INT nIndex, const GifControlInfo& gifControlInfo, const RECT& rcFrame, BYTE* pBmpBuffer, INT nLength, DWORD_PTR callbackData) = 0;
};

class GIF_UTILS_API IGifParser
{
public:
    virtual ~IGifParser();
    virtual BOOL OpenGifFile(LPCTSTR pszFileName) = 0;
    virtual INT ParseGif(INT nTargetBpp, IGifParserCallback* pCallBack, DWORD_PTR callbackData) = 0;
    //virtual BOOL GetGifImage(INT nIndex, BYTE* pBmpData, INT *pLength) = 0;
    virtual VOID CloseFile() = 0;
    virtual void Release() = 0;
public:
    static IGifParser* GetInstance();
protected:
    IGifParser();
};