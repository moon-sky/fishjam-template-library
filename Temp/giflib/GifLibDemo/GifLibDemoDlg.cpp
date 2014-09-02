
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

#include "../gifMaker/gifMaker.h"
#ifdef _DEBUG
#  pragma comment(lib, "../Debug/gifMaker.lib")
#else
#  pragma comment(lib, "../Release/gifMaker.lib")
#endif 


#include <ftlBase.h>
#include <ftlGdi.h>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#define GIF_VERIFY(x) \
    nRet = (x);\
        if(nRet == GIF_ERROR){\
        TRACE(TEXT("ERROR: %s(%d)"), TEXT(#x), 0); \
    }



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
        GifFileType* pGifFile = EGifOpenFileName("gifDemo.gif", false, &nError);
        if (pGifFile)
        {
            GIF_VERIFY(EGifPutScreenDesc(pGifFile, nWidth, nHeight, nGifColorRes, 0, pColorMap));

            unsigned char ExtStr[4] = { 0x04, 0x00, 0x00, 0xff };
            ExtStr[0] = 0x04;
            ExtStr[1] = nDelay % 256;
            ExtStr[2] = nDelay / 256;

            for (int index = 0; index < 100; index++)
            {

                /* Dump graphics control block. */
                GIF_VERIFY(EGifPutExtension(pGifFile, GRAPHICS_EXT_FUNC_CODE, 4, ExtStr));
                
                //ColorMapObject *pColorMap2 = GifMakeMapObject(NumLevels, ColorMap256);
                GIF_VERIFY(EGifPutImageDesc(pGifFile, 0, 0, nWidth, nHeight, FALSE, NULL));//pColorMap2));
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
                    GIF_VERIFY(EGifPutLine(pGifFile, Ptr, nWidth));
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
            

            GIF_VERIFY(EGifCloseFile(pGifFile, &nError));
        }
        GifFreeMapObject(pColorMap);
    }

    //free(GlblGifBuffer);
    free(pOutputBuffer);

    AfxMessageBox(TEXT("After Generate Gif"));
}

void CGifLibDemoDlg::OnBnClickedButton2()
{
    BOOL bRet = FALSE;
    int nRet = 0;
    int nError = 0;
    int nWidth = 100;
    int nHeight = 100;
    int nGifColorRes = 8;
    int NumLevels = 256;
    int nBpp = 24;
    int nDelay = 1;

    CGifMaker    gifMaker;

   FTL::CFCanvas canvas;
   CRect rectCapture(0, 0, nWidth, nHeight);
   API_VERIFY(canvas.Create(m_hWnd, nWidth, -nHeight, nBpp));

   gifMaker.BeginMakeGif(nWidth, nHeight, nBpp, "gifMakerDemo.gif");
   int i = 0;
   for (i = 0; i < 5; i++)
   {
       CWindowDC desktopDC(GetDesktopWindow());
       API_VERIFY(::BitBlt(canvas.GetCanvasDC(), 0, 0, nWidth, nHeight, desktopDC, 0 + i * 10, 0 + i * 10, SRCCOPY));
       //_OverlayMouseToScreen(canvas.GetCanvasDC(), &rectCapture);
       gifMaker.AddGifImage(canvas.GetBuffer(), canvas.GetBufferSize(), (i + 1) * 1000);
   }
   gifMaker.EndMakeGif( (i) * 100);
}
