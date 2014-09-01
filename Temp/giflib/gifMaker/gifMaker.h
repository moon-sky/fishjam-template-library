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
class GIFMAKER_API CGifMaker {
public:
	CGifMaker(void);

    int BeginMakeGif(int nWidth, int nHeight, int bpp);
    int AddGifImage(, byte* pBmpData);
    int EndMakeGif();
};

//extern GIFMAKER_API int ngifMaker;
