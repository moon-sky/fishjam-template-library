
// GifLibDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GifLibDemo.h"
#include "GifLibDemoDlg.h"

#include "../giflib/lib/gif_lib.h"
#ifdef _DEBUG
#  pragma comment(lib, "../Debug/giflib.lib")
#else
#  pragma comment(lib, "../Release/giflib.lib")
#endif 

#ifdef _DEBUG
#  pragma comment(lib, "../Debug/gifMaker.lib")
#else
#  pragma comment(lib, "../Release/gifMaker.lib")
#endif 

#define GIF_VERIFY(x, err) \
    nRet = (x);\
    if(nRet == GIF_ERROR){\
        FTLTRACE(TEXT("%s(%d) :\t Gif Error: %s(%d)\n"), TEXT(__FILE__), __LINE__, TEXT(#x), (err)); \
    }

#include <ftlBase.h>
#include <ftlGdi.h>
#include <ftlDebug.h>
#include <ftlAlgorithm.h>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGifLibDemoDlg dialog




CGifLibDemoDlg::CGifLibDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGifLibDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_nClipIndex = 0;
    m_pTimerGifMaker = NULL;
    m_nParserScreenWidth = 0;
    m_nParserScreenHeight = 0;
    m_nTargetBpp = 32;
}

void CGifLibDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGifLibDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON1, &CGifLibDemoDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CGifLibDemoDlg::OnBnClickedButton2)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_TIMER_CLIP, &CGifLibDemoDlg::OnBnClickedBtnTimerClip)
    ON_BN_CLICKED(IDC_BTN_WU_COLOR_QUANTIZER, &CGifLibDemoDlg::OnBnClickedBtnWuColorQuantizer)
    ON_BN_CLICKED(IDC_BTN_GIF_PARSER, &CGifLibDemoDlg::OnBnClickedBtnGifParser)
END_MESSAGE_MAP()


// CGifLibDemoDlg message handlers

BOOL CGifLibDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGifLibDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGifLibDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGifLibDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGifLibDemoDlg::_OverlayMouseToScreen(HDC hdc, LPRECT lpRect)
{
    //ATLASSERT(m_rcCapture == *lpRect);

    BOOL bRet = FALSE;
    POINT point = {0};
    API_VERIFY(::GetCursorPos(&point));
    if (!PtInRect(lpRect, point))
    {
        //Mouse is not in rect, so donot draw it and return TRUE;
        return TRUE;
    }
    CURSORINFO CursorInfo;
    CursorInfo.cbSize = sizeof(CURSORINFO);

    API_VERIFY(GetCursorInfo(&CursorInfo));
    if (bRet)
    {
        API_VERIFY_EXCEPT1(::DrawIconEx(hdc, CursorInfo.ptScreenPos.x - lpRect->left, CursorInfo.ptScreenPos.y - lpRect->top , CursorInfo.hCursor,
            32, 32, 0, NULL, DI_NORMAL), ERROR_INVALID_CURSOR_HANDLE);
        if (!bRet && GetLastError() == ERROR_INVALID_CURSOR_HANDLE)
        {
            SetLastError(0);
            bRet = TRUE;
        }
        ::ReleaseDC(NULL, hdc);
    }
    return bRet;
}


void CGifLibDemoDlg::OnBnClickedButton1()
{
    BOOL bRet = FALSE;
    int nRet = 0;
    int nError = 0;
    int nWidth = 1280;
    int nHeight = 800;
    int nGifColorRes = 8;
    int NumLevels = 256;
    int nDelay = 1;

    GifColorType ColorMap256[256] = {0};
    for (int i = 0; i < 255; i++)
    {
        ColorMap256[i].Red = rand() % 255;
        ColorMap256[i].Green = rand() % 255;  //255 - i; //
        ColorMap256[i].Blue = rand() % 255;
    }
    //ColorMap256[1].Red = 255;

    FTL::CFCanvas canvas;
    CRect rectCapture(0, 0, nWidth, nHeight);
    API_VERIFY(canvas.Create(m_hWnd, nWidth, -nHeight, 24));
    CWindowDC desktopDC(GetDesktopWindow());
    API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, nWidth, nHeight, desktopDC, 0, 0, SRCCOPY));
    _OverlayMouseToScreen(canvas.GetCanvasDC(), &rectCapture);

#if 0
    CDC dcMem;
    dcMem.Attach(canvas.GetCanvasDC());
    for(int i = 0; i < 100; i ++){
        dcMem.FillSolidRect(i, 0, i + 1, nHeight, RGB(i * 2, 0, 0));
    }
    dcMem.Detach();
#endif 

    //CClientDC wndDC(this);
    //::BitBlt(wndDC.GetSafeHdc(), 0, 0, nWidth, nHeight, canvas.GetCanvasDC(), 0, 0, SRCCOPY);

    int nSize = sizeof(GifPixelType) * nWidth * nHeight;
    GifByteType* pOutputBuffer = (GifByteType *) malloc(nSize);
    ZeroMemory(pOutputBuffer, nSize);
    
    nRet = GifQuantizeRGBBuffer(nWidth, nHeight, 24, canvas.GetBuffer(), ColorMap256, pOutputBuffer);
    
    TRACE(TEXT("After quantize\n"));
    

    //return;
    
    ColorMapObject *pColorMap = GifMakeMapObject(NumLevels, ColorMap256);
    if (pColorMap)
    {
        //pColorMap->SortFlag = true;
        GifFileType* pGifFile = EGifOpenFileName(L"gifDemo.gif", false, &nError);
        if (pGifFile)
        {
            GIF_VERIFY(EGifPutScreenDesc(pGifFile, nWidth, nHeight, nGifColorRes, 0, pColorMap),pGifFile->Error);

            unsigned char ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
            ExtStr[0] = 0x04;
            ExtStr[1] = nDelay % 256;
            ExtStr[2] = nDelay / 256;

            for (int index = 0; index < 100; index++)
            {

                /* Dump graphics control block. */
                GIF_VERIFY(EGifPutExtension(pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr),pGifFile->Error);
                
                //ColorMapObject *pColorMap2 = GifMakeMapObject(NumLevels, ColorMap256);
                GIF_VERIFY(EGifPutImageDesc(pGifFile, 0, 0, nWidth, nHeight, FALSE, NULL),pGifFile->Error);//pColorMap2));
                //GifFreeMapObject(pColorMap2);

                int nLength = nWidth * nHeight;
                //GifByteType* pOutputBuffer = (GifByteType *) malloc(nLength * sizeof(GifByteType));
                //for(int i = 0; i < nLength; i++){
                //    pOutputBuffer[i] = rand() % 255;//i % nHeight;
                //}           
                //GifRowType* pRasterBuffer = (GifRowType*)malloc(sizeof(GifRowType) * nHeight);
                //for (int j = 0; j < nHeight; j++)
                //{
                //    pRasterBuffer[j] = (GifRowType)calloc(nWidth, sizeof(GifPixelType));
                //    //ZeroMemory(pRasterBuffer[j], sizeof(GifPixelType) * nWidth);
                //    for (int i = 0; i < nWidth; i++)
                //    {
                //        *(pRasterBuffer[j] + i)= 10;//rand() % 30; //*(pRasterBuffer + j * nHeight + i) //[j][i]
                //    }
                //}

                GifByteType *Ptr = pOutputBuffer;
                for (int j = 0; j < nHeight; j++) 
                {
                    //GifPixelType* pPixel = (GlblGifBuffer + j * nWidth);
                    //GIF_VERIFY(EGifPutLine(pGifFile, pRasterBuffer[j], nWidth));
                    GIF_VERIFY(EGifPutLine(pGifFile, Ptr, nWidth),pGifFile->Error);
                    Ptr += nWidth;
                }

                Sleep(nDelay * 10);
                //CDC dcMem;
                //dcMem.Attach(canvas.GetCanvasDC());
                //for(int i = 0; i < 100; i ++){
                //    dcMem.FillSolidRect(i, 0, i + 1, nHeight, RGB(i * 2, index * 50, 255 - index * 50));
                //}
                //dcMem.Detach();

                CWindowDC desktopDC(GetDesktopWindow());
                API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, nWidth, nHeight, desktopDC, 0, 0, SRCCOPY));
                _OverlayMouseToScreen(canvas.GetCanvasDC(), &rectCapture);

                ZeroMemory(pOutputBuffer, nSize);
                nRet = GifQuantizeRGBBuffer(nWidth, nHeight, 24, canvas.GetBuffer(), ColorMap256, pOutputBuffer);

                //for (int j = 0; j < nHeight; j++)
                //{
                //   free(pRasterBuffer[j]);
                //}
                //free(pRasterBuffer);
            }
            

            GIF_VERIFY(EGifCloseFile(pGifFile, &nError), nError);
        }
        GifFreeMapObject(pColorMap);
    }

    //free(GlblGifBuffer);
    free(pOutputBuffer);

    AfxMessageBox(TEXT("After Generate Gif"));
}

void CGifLibDemoDlg::OnBnClickedButton2()
{

    {
        int nWidth = 100;//77;
        int nHeight = 100;//77;
        int nGifColorRes = 8;
        int NumLevels = 256;
        int nBpp = 8;

        BOOL bRet = FALSE;
        FTL::CFCanvas canvas;
        API_VERIFY(canvas.Create(m_hWnd, nWidth, -nHeight, nBpp));
        //memset(canvas.GetBuffer(), 0x78, canvas.GetBufferSize());
        CWindowDC desktopDC(GetDesktopWindow());
        API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, nWidth, nHeight, desktopDC, 0, 0, SRCCOPY));

        CClientDC wndDC(this);
        ::BitBlt(wndDC.GetSafeHdc(), 0, 0, nWidth, nHeight, canvas.GetCanvasDC(), 0, 0, SRCCOPY);


        CString strFileName;
        API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), TEXT("OnBnClickedButton2.bmp")));
        

        //CDC memDC;
        //memDC.Attach(canvas.GetCanvasDC());
        //for (int j = 0; j < 6; j++)
        //{
        //   memDC.FillSolidRect(0, j, 7, 1, RGB(255, 0, j * 10));
        //}
        //memDC.Detach();

        //BYTE* pBuffer = canvas.GetBuffer();
        //INT nSize = canvas.GetBufferSize();
        
        return;
    }

    BOOL bRet = FALSE;
    int nRet = 0;
    int nError = 0;
    int nWidth = 100;//77;
    int nHeight = 100;//77;
    int nGifColorRes = 8;
    int NumLevels = 256;
    int nBpp = 24;
    int nDelay = 1;

    IGifMaker*    pGifMaker = IGifMaker::GetInstance();

   FTL::CFCanvas canvas;
   CRect rectCapture(0, 0, nWidth, nHeight);
   API_VERIFY(canvas.Create(m_hWnd, nWidth, -nHeight, nBpp));

   pGifMaker->BeginMakeGif(nWidth, nHeight, L"gifMakerDemo.gif");
   int i = 0;
   CRect rcDiffColor(10, 10, 30, 30);
   for (i = 0; i < 5; i++)
   {
       CWindowDC desktopDC(GetDesktopWindow());
       API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, nWidth, nHeight, desktopDC, 0, 0, SRCCOPY));

       CDC memDC;
       memDC.Attach(canvas.GetCanvasDC());
       //CRect rcTotal(0, 0, nWidth, nHeight);
       //memDC.FillSolidRect(rcTotal, RGB(1, 2, 3));
       memDC.FillSolidRect(rcDiffColor, RGB(255, 0, 0));
       FTLTRACE(TEXT("[%d] Fill SolidRect %s\n"), i + 1, FTL::CFRectDumpInfo(rcDiffColor).GetConvertedInfo());
       memDC.Detach();
       rcDiffColor.OffsetRect(10, 10);
        
       CString strFileName;
       strFileName.Format(TEXT("gifMakerSrc_%d.bmp"), i);
       API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), strFileName));
       //_OverlayMouseToScreen(canvas.GetCanvasDC(), &rectCapture);
       CRect rcFrame(0, 0, canvas.GetWidth(), canvas.GetHeight());
       pGifMaker->AddGifImage(rcFrame, canvas.GetBuffer(), canvas.GetBufferSize(),canvas.GetBpp(),  (i + 1) * 1000);
   }
   pGifMaker->EndMakeGif( (i) * 100);
}

int         g_nWidth = 100;
int         g_nHeight = 100;
int         g_nBpp = 24;

void CGifLibDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
    BOOL bRet = FALSE;

    m_nClipIndex++;
    if (m_nClipIndex < 50)
    {
        FTL::CFCanvas canvas;
        CRect rectCapture(0, 0, g_nWidth, g_nHeight);
        API_VERIFY(canvas.Create(m_hWnd, g_nWidth, -g_nHeight, g_nBpp));
        CWindowDC desktopDC(GetDesktopWindow());
        API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, g_nWidth, g_nHeight, desktopDC, 100, 100, SRCCOPY));
        _OverlayMouseToScreen(canvas.GetCanvasDC(), &rectCapture);

#ifdef _DEBUG
        CString strFileName;
        strFileName.Format(TEXT("gifTimerClip_%d.bmp"), m_nClipIndex);
        API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), strFileName));
#endif

        CRect rcFrame(0, 0, canvas.GetWidth(), canvas.GetHeight());
        m_pTimerGifMaker->AddGifImage(rcFrame, canvas.GetBuffer(), canvas.GetBufferSize(), canvas.GetBpp(), GetTickCount());
    }
    else
    {
        KillTimer(1);
        m_pTimerGifMaker->EndMakeGif(GetTickCount());
        m_pTimerGifMaker->Release();
        AfxMessageBox(TEXT("Timer Clip Over"));
        
    }
    

    CDialog::OnTimer(nIDEvent);
}


void CGifLibDemoDlg::OnBnClickedBtnTimerClip()
{
    BOOL bRet = FALSE;
    int nRet = 0;
    int nError = 0;

    ASSERT(m_pTimerGifMaker == NULL);
    m_pTimerGifMaker = IGifMaker::GetInstance();
    m_pTimerGifMaker->BeginMakeGif(g_nWidth, g_nHeight, L"gifTimerClip.gif");
    m_nClipIndex = 0;
    
    SetTimer(1, 100, NULL);
}

void CGifLibDemoDlg::OnBnClickedBtnWuColorQuantizer()
{
    BOOL bRet = FALSE;

    FTL::CFElapseCounter    elpaseCounter;

    //CBitmap bmp;
    //API_VERIFY(bmp.LoadBitmap(IDB_BITMAP1));
    //if (bRet)
    //{
    //    BITMAP bitmap = {0};
    //    bmp.GetBitmap(&bitmap);
    //    //CALC_BMP_ALLIGNMENT_WIDTH_COUNT(bitmap.bmWidthBytes bitmap.bmBitsPixel
    //    DWORD dwSize = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(bitmap.bmWidth, bitmap.bmBitsPixel) * bitmap.bmHeight;
    //    BITMAPINFO  bmInfo = {0};
    //    bmInfo.bmiHeader.biSize = sizeof(bmInfo.bmiHeader);
    //    bmInfo.bmiHeader.biWidth = bitmap.bmWidth;
    //    bmInfo.bmiHeader.biHeight = bitmap.bmHeight;
    //    bmInfo.bmiHeader.biPlanes = 1;
    //    bmInfo.bmiHeader.biBitCount = bitmap.bmBitsPixel;
    //    bmInfo.bmiHeader.biSizeImage = dwSize;
    //    BYTE* pBuffer = new BYTE[dwSize ];
    //    //bmp.GetBitmapBits(dwSize, pBuffer);
    //    int nRet = GetDIBits(NULL, bmp, 0, bitmap.bmHeight-1, pBuffer, &bmInfo, DIB_RGB_COLORS);
    //    CGifMaker    gifMaker;
    //    gifMaker.BeginMakeGif(bitmap.bmWidth, bitmap.bmHeight, bitmap.bmBitsPixel, TEXT("WuColorQuantizer.gif"));
    //    gifMaker.AddGifImage(pBuffer, dwSize, GetTickCount());
    //    gifMaker.EndMakeGif(GetTickCount());
    //    
    //    return;
    //}

    CRect rectCapture(0, 0, g_nWidth, g_nHeight);
    FTL::CFCanvas canvas;
    API_VERIFY(canvas.Create(m_hWnd, g_nWidth, -g_nHeight, g_nBpp));
    
#if 1
    CDC memDC;
    memDC.Attach(canvas.GetCanvasDC());
    int nFillWidth = 10, nFillHeight = 10;
    int nColorCount = 0;
    for (int w = 0; w < g_nWidth; w+=nFillWidth)
    {
        for (int h = 0; h < g_nHeight; h+=nFillHeight)
        {
            nColorCount++;
            memDC.FillSolidRect(w, h, nFillWidth, nFillHeight, RGB(w * (g_nWidth / nFillWidth), 0x0, 0xFF - h * (g_nHeight / nFillHeight)));
        }
    }
    FTLTRACE(TEXT("nCOlorCount=%d\n"), nColorCount);
    memDC.Detach();
#else
    CWindowDC desktopDC(GetDesktopWindow());
    API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, g_nWidth, g_nHeight, desktopDC, 100, 100, SRCCOPY));
#endif 

#ifdef _DEBUG
    API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), TEXT("WuColorQuantizer.bmp")));
#endif

    IGifMaker*  pGifMaker = IGifMaker::GetInstance();
    CRect rcFrame(0, 0, canvas.GetWidth(), canvas.GetHeight());
    pGifMaker->BeginMakeGif(canvas.GetWidth(), canvas.GetHeight(), TEXT("WuColorQuantizer.gif"));
    pGifMaker->AddGifImage(rcFrame, canvas.GetBuffer(), canvas.GetBufferSize(), canvas.GetBpp(), GetTickCount());
    pGifMaker->EndMakeGif(GetTickCount());
    pGifMaker->Release();

    //COLORREF* pPaletteArray = colorQuantizer.GetPalette(&nPaletteCount);

    //for (int i = 0; i < nPaletteCount; i++)
    //{
    //    COLORREF palette = *(pPaletteArray + i);
    //    FTLTRACE(TEXT("palette=0x%x\n"), palette);
    //}

    int i = 0;

    FTL::CFStringFormater formater;
    formater.Format(TEXT("Elapse %d"), elpaseCounter.GetElapseTime() / NANOSECOND_PER_MILLISECOND);
    MessageBox(formater.GetString());
}

VOID CGifLibDemoDlg::OnGetGifInfo(INT nWidth, INT nHeight, DWORD_PTR callbackData)
{
    m_nParserScreenWidth = nWidth;
    m_nParserScreenHeight = nHeight;
}

BOOL CGifLibDemoDlg::OnParseFrame(INT nIndex, const GifControlInfo& gifControlInfo, const RECT& rcFrame, BYTE* pBmpBuffer, INT nLength, DWORD_PTR callbackData)
{
    FTLTRACE(TEXT("CGifLibDemoDlg::OnParseFrame[%d], rcFrame=%s, nLength=%d, delay=%d, translateColor=%d\n"), 
        nIndex, FTL::CFRectDumpInfo(rcFrame).GetConvertedInfo(), nLength,
        gifControlInfo.nDelayMilliSeconds, gifControlInfo.nTransparentColorIndex);

#if 1
    BOOL bRet = FALSE;
    FTL::CFCanvas canvas;
    API_VERIFY(canvas.Create(NULL, m_nParserScreenWidth, -m_nParserScreenHeight, m_nTargetBpp));
    FTLASSERT(canvas.GetBufferSize() == nLength);
    CopyMemory(canvas.GetBuffer(), pBmpBuffer, nLength);
    FTL::CFStringFormater formaterName;
    formaterName.Format(TEXT("ParseBmp_%d.bmp"), nIndex);
    API_VERIFY(FTL::CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), formaterName));
#endif 

    return TRUE;
}


void CGifLibDemoDlg::OnBnClickedBtnGifParser()
{
    BOOL bRet = FALSE;
    IGifParser*     pGifParser = IGifParser::GetInstance();
    
    CFileDialog dlg(TRUE);
    if (dlg.DoModal() == IDOK)
    {
        m_nTargetBpp = 24;

        API_VERIFY(pGifParser->OpenGifFile(dlg.GetPathName()));// TEXT("gifParserSource.gif")));
        INT nCount = pGifParser->ParseGif(m_nTargetBpp, this, (DWORD_PTR)this);
        for (INT nIndex = 0; nIndex < nCount; nIndex++)
        {
            //pGifParser->GetGifImage(nIndex, )
        }
        MessageBox(TEXT("After Analyze"));
        pGifParser->CloseFile();
    }
    pGifParser->Release();
}
