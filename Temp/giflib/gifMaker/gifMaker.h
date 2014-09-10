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

//// This class is exported from the gifMaker.dll

namespace FTL{
    //template <typename ThreadTraits = DefaultThreadTraits>
    //class CFThread;
}

struct GifFileType;
struct GifColorType;
class CQuantizer;

class GIFMAKER_API CGifMaker {
public:
	CGifMaker(void);
    ~CGifMaker();

    BOOL BeginMakeGif(int nWidth, int nHeight, int bpp, LPCTSTR pszFileName);
    BOOL AddGifImage(BYTE* pBmpData, int nLength, DWORD dwTicket);
    BOOL EndMakeGif(DWORD dwTicket);
private:
    //FTL::CFThread<>* m_pThreadMaker;
    int m_nPreWidth, m_nPreHeight, m_nPreBpp, m_nPreBmpBytes, m_nDiffResultSize;
    int m_nGifColorRes;     //8
    int m_nGifNumLevels;    //256
    int m_nImgCount;
    int m_nGifBufferSize;
    BOOL m_bFirstImage;
    BOOL m_bDelayImage;
    BOOL m_bWriteFirst;
    
    BYTE* m_pPreBmp;
    BYTE* m_pDiffResult;
    BYTE* m_pGifBuffer;
    BYTE* m_pGiffDiffBuffer;
    BYTE* m_pTranslateBuffer;
    RECT  m_rcDiff;


    GifFileType* m_pGifFile;
    GifColorType* m_pColorMap256;
    DWORD m_dwLastTicket;
    CQuantizer* m_pQuantizer;

    BOOL _WriteGifData(BYTE* pBmpData, RECT rcBmp, DWORD dwTicket);

    BYTE* _DuplicateBmpRect(BYTE* pSrcBmpData, int nSrcWidth, int nSrcHeight, int nBpp, RECT rcSrc);
    VOID _FreeDuplicateBmpData(BYTE* pData);
};

//extern GIFMAKER_API int ngifMaker;
