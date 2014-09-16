#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GIFMAKER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GIFMAKER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GIFMAKER_EXPORTS
#define GIFMAKER_API __declspec(dllexport)
#else
#define GIFMAKER_API __declspec(dllimport)
#endif

enum GIFMAKER_API CompressType{
    ctFast = 0,
    ctHighQuality,
};


class GIFMAKER_API IGifMaker {
public:
    virtual ~IGifMaker();
    virtual CompressType SetCompressType(CompressType type) = 0;

    virtual BOOL BeginMakeGif(int nWidth, int nHeight, int bpp, LPCTSTR pszFileName) = 0;
    virtual BOOL AddGifImage(BYTE* pBmpData, int nLength, DWORD dwTicket) = 0;
    virtual BOOL EndMakeGif(DWORD dwTicket, DWORD dwWaitTimeOut = INFINITE) = 0;
    //virtual BOOL Pause() = 0;
    //virtual BOOL Resume() = 0;
    virtual void Release() = 0;

public:
    static IGifMaker* GetInstance();
protected:
    IGifMaker();
};
