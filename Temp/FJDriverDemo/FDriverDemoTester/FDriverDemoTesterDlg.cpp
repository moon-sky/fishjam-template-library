
// FDriverDemoTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FDriverDemoTester.h"
#include "FDriverDemoTesterDlg.h"
//#include "../../ProtDrv/ProtDrv/ProtDrvDef.h"
#include "../FDriverDemoDefine.h"
#include <winioctl.h>

//#include <NapTypes.h>

#include <ftlSystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFDriverDemoTesterDlg dialog


CFDriverDemoTesterDlg::CFDriverDemoTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFDriverDemoTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hOldBmp = NULL;
    m_hFileMapping = NULL;
    m_hBmpWindow = NULL;
}

void CFDriverDemoTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFDriverDemoTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_INSTALL_SERVICE, &CFDriverDemoTesterDlg::OnBnClickedBtnInstallService)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_DRIVER_FILE, &CFDriverDemoTesterDlg::OnBnClickedBtnChooseDriverFile)
	ON_BN_CLICKED(IDC_BTN_OPEN_DRIVER, &CFDriverDemoTesterDlg::OnBnClickedBtnOpenDriver)
	ON_BN_CLICKED(IDC_BTN_CLOSE_DRIVER, &CFDriverDemoTesterDlg::OnBnClickedBtnCloseDriver)
	ON_BN_CLICKED(IDC_BTN_UNINSTALL_SERVICE, &CFDriverDemoTesterDlg::OnBnClickedBtnUninstallService)
	ON_BN_CLICKED(IDC_BTN_INSTALL_HOOK, &CFDriverDemoTesterDlg::OnBnClickedBtnInstallHook)
	ON_BN_CLICKED(IDC_BTN_UNINSTALL_HOOK, &CFDriverDemoTesterDlg::OnBnClickedBtnUninstallHook)
	ON_BN_CLICKED(IDC_BTN_DO_BITBLT, &CFDriverDemoTesterDlg::OnBnClickedBtnDoBitblt)
	ON_BN_CLICKED(IDC_BTN_FILTER_DESKTOP, &CFDriverDemoTesterDlg::OnBnClickedBtnFilterDesktop)
    ON_BN_CLICKED(IDC_BTN_DO_TEXTOUT, &CFDriverDemoTesterDlg::OnBnClickedBtnDoTextout)
	ON_BN_CLICKED(IDC_BTN_TEST_DESKTOP, &CFDriverDemoTesterDlg::OnBnClickedBtnTestDesktop)
END_MESSAGE_MAP()


// CFDriverDemoTesterDlg message handlers

BOOL CFDriverDemoTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	_RefreshMemoryDC();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFDriverDemoTesterDlg::OnPaint()
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
HCURSOR CFDriverDemoTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFDriverDemoTesterDlg::OnBnClickedBtnChooseDriverFile()
{
	//DWORD dwPid = FTL::CFSystemUtil::GetPidFromProcessName(TEXT("csrss.exe"));
	//return ;

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, TEXT(".sys|*.sys|AllFiles(*.*)|*.*||"));
	if (dlg.DoModal())
	{
		m_strDriverPath = dlg.GetPathName();
	}
}

void CFDriverDemoTesterDlg::OnBnClickedBtnInstallService()
{
	BOOL bRet = FALSE;
	if (!m_strDriverPath.IsEmpty())
	{
		//API_VERIFY(m_DriverController.InstallService(m_strDriverPath , PROTECT_SERVICE_NAME)); //FDRIVER_DEMO_SERVICE_NAME));
		API_VERIFY(m_DriverController.InstallService(m_strDriverPath , FDRIVER_DEMO_SERVICE_NAME));
	}
}

void CFDriverDemoTesterDlg::OnBnClickedBtnOpenDriver()
{
	BOOL bRet = FALSE;
	//API_VERIFY(m_DriverController.OpenDemoDriver(PROTECT_NT_CONTROL_FILE_NAME)); //FDRIVER_DEMO_WIN2K_DEVICE_NAME));
	API_VERIFY(m_DriverController.OpenDemoDriver(FDRIVER_DEMO_WIN2K_DEVICE_NAME)); //L"\\DosDevices\\Open_CaptureDriver")); // FDRIVER_DEMO_WIN2K_DEVICE_NAME));
}

void CFDriverDemoTesterDlg::OnBnClickedBtnCloseDriver()
{
	BOOL bRet = FALSE;
	API_VERIFY(m_DriverController.CloseDemoDriver());
}

void CFDriverDemoTesterDlg::OnBnClickedBtnUninstallService()
{
	BOOL bRet = FALSE;
	API_VERIFY(m_DriverController.UnInstallService());

}

BOOL CFDriverDemoTesterDlg::_RefreshMemoryDC()
{
	BOOL bRet = FALSE;
	CWindowDC deskDC(NULL);
	//CRect rcClient;
	GetClientRect(&m_rcClient);

	if (m_MemoryDC.m_hDC)
	{
		m_MemoryDC.SelectObject(m_hOldBmp);

        API_VERIFY(DeleteObject(m_hBmpWindow));
        m_hBmpWindow = NULL;
        API_VERIFY(CloseHandle(m_hFileMapping));
        m_hFileMapping = NULL;
		API_VERIFY(m_MemoryDC.DeleteDC());
        
	}
    PVOID pvBits = NULL;
    BITMAPINFO bmpInfo = {0};
    bmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    bmpInfo.bmiHeader.biWidth = m_rcClient.Width();
    bmpInfo.bmiHeader.biHeight = m_rcClient.Height(); //-m_nHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = abs( m_rcClient.Width() * m_rcClient.Height() * 32 >> 3 );

    API_VERIFY((m_hFileMapping = CreateFileMapping(NULL, NULL, PAGE_READWRITE, 0, bmpInfo.bmiHeader.biSizeImage, NULL)) != NULL);
    if (bRet)
    {
        API_VERIFY((m_hBmpWindow = CreateDIBSection(deskDC.m_hDC, &bmpInfo, DIB_RGB_COLORS, &pvBits, m_hFileMapping, 0)) != NULL);
        //API_VERIFY(m_bmpWindow.CreateCompatibleBitmap(&deskDC, rcClient.Width(), rcClient.Height()));
        API_VERIFY(m_MemoryDC.CreateCompatibleDC(&deskDC));
        m_MemoryDC.SelectObject(m_hBmpWindow);
        m_MemoryDC.FillSolidRect(&m_rcClient, RGB(0, 127, 127));
        m_MemoryDC.DrawText(TEXT("Filter Wnd Pos"), m_rcClient, DT_LEFT| DT_TOP| DT_SINGLELINE);
    }


	return bRet;

}
void CFDriverDemoTesterDlg::OnBnClickedBtnInstallHook()
{
	BOOL bRet = FALSE;
	CRect rcClient;
	GetClientRect(&rcClient);

	//DWORD dwProcessId = GetCurrentProcessId();
	//API_VERIFY(m_DriverController.IoControl(IOCTL_PROTDRV_INSTALL_HOOK, &dwProcessId, sizeof(dwProcessId), NULL, 0))
	PROTECT_WND_INFO	protectInfo;
	protectInfo.hWndDeskTop =  ::GetDesktopWindow(); //GetDlgItem(IDC_STATIC_DRAW)->m_hWnd;// 
	//protectInfo.hDCDesktop = ::GetDC(protectInfo.hWndDeskTop);

	//protectInfo.hProtectWindow = m_hWnd;
	//protectInfo.hSelfProcess = (HANDLE)GetCurrentProcessId();
	protectInfo.hTargetProcess = (HANDLE)FTL::CFSystemUtil::GetPidFromProcessName(TEXT("csrss.exe"));
	protectInfo.hDCWndProtect = m_MemoryDC.m_hDC;
	ClientToScreen(&rcClient);
	protectInfo.rcProtectWindow = rcClient;

	API_VERIFY(m_DriverController.IoControl(IOCTL_FDRIVER_INSTALL_HOOK, &protectInfo, sizeof(protectInfo), NULL, 0));
	//::ReleaseDC(protectInfo.hWndDeskTop, protectInfo.hDCDesktop);
}

void CFDriverDemoTesterDlg::OnBnClickedBtnUninstallHook()
{
	BOOL bRet = FALSE;
	//API_VERIFY(m_DriverController.IoControl(IOCTL_PROTDRV_UNINSTALL_HOOK, NULL, 0, NULL, 0));
	API_VERIFY(m_DriverController.IoControl(IOCTL_FDRIVER_UNINSTALL_HOOK, NULL, 0, NULL, 0));

}

void CFDriverDemoTesterDlg::OnBnClickedBtnDoBitblt()
{
	BOOL bRet = FALSE;
	CWnd* pWndDraw = GetDlgItem(IDC_STATIC_DRAW);
	if (pWndDraw)
	{
		CDC* pDC = pWndDraw->GetDC();
		if (pDC)
		{
			CWindowDC deskDC(NULL);
			
			CDC dcMemory;
			API_VERIFY(dcMemory.CreateCompatibleDC(pDC));
			CRect rcStaticDraw;
			pWndDraw->GetClientRect(&rcStaticDraw);
			//pWndDraw->ScreenToClient(&rcStaticDraw);
			CBitmap bmp;
			API_VERIFY(bmp.CreateCompatibleBitmap(pDC, rcStaticDraw.Width(), rcStaticDraw.Height()));
			CBitmap* pOldBmp = dcMemory.SelectObject(&bmp);
			dcMemory.FillSolidRect(rcStaticDraw, RGB(255, 0, 0));

			FTLTRACE(TEXT("OnBnClickedBtnDoBitblt, hDCDest=0x%x(hWnd=0x%x), hDCSrc=0x%x, \n"), 
				deskDC.m_hDC, WindowFromDC(deskDC.m_hDC), dcMemory.m_hDC);
			//API_VERIFY(pDC->BitBlt(0, 0, rcStaticDraw.Width(), rcStaticDraw.Height(), &dcMemory, 0, 0, SRCCOPY));
			
			deskDC.BitBlt(0, 0, rcStaticDraw.Width(), rcStaticDraw.Height(), &dcMemory, 0, 0, SRCCOPY);

			//pDC->DrawText(TEXT("fishjam"), &rcStaticDraw, DT_CENTER | DT_VCENTER);
			//pDC->FillSolidRect(rcStaticDraw, RGB(0, 255, 0));
			dcMemory.SelectObject(pOldBmp);

			pWndDraw->ReleaseDC(pDC);
		}
	}
}

void CFDriverDemoTesterDlg::OnBnClickedBtnFilterDesktop()
{
	//BOOL bRet = FALSE;
	//HWND hWndDesktop = ::GetDesktopWindow();
	//HDC hDC = ::GetDC(hWndDesktop);
	//HWND hWndFromDC = ::WindowFromDC(hDC);
	//FTLTRACE(TEXT("hWndDesktop = 0x%x, hWndFromDC=0x%x, hDC=0x%x\n"), hWndDesktop, hWndFromDC, hDC);
	//::ReleaseDC(hWndDesktop, hDC);
	////API_VERIFY(m_DriverController.IoControl(IOCTL_FDRIVER_FILTER_DESKTOP, &hDesktop, sizeof(hDesktop), NULL, 0));

	INT nSize = sizeof(PROTECT_WND_INFO);
	CString strInfo;
	strInfo.Format(TEXT("sizeof PROTECT_WND_INFO is %d"), nSize);
	MessageBox(strInfo);
}

void CFDriverDemoTesterDlg::OnBnClickedBtnDoTextout()
{
    BOOL bRet = FALSE;
    CWnd* pWndDraw = GetDlgItem(IDC_STATIC_DRAW);
    if (pWndDraw)
    {
		CWindowDC wndDC(NULL);
        CDC* pDC =  pWndDraw->GetDC(); //&wndDC;// 
        if (pDC)
        {
            CRect rcStaticDraw;
            pWndDraw->GetClientRect(&rcStaticDraw);
            //rcStaticDraw.OffsetRect(10, 10);

			FTLTRACE(TEXT("Before BitBlt From MemoryDC"));
			API_VERIFY(pDC->BitBlt(rcStaticDraw.left, rcStaticDraw.top, rcStaticDraw.Width(), rcStaticDraw.Height(),
				&m_MemoryDC, 0, 0, SRCCOPY));

			Sleep(1000);

			pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rcStaticDraw, NULL, 0, NULL);
            pDC->DrawText(TEXT("DrawTextDemo"), rcStaticDraw, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            pDC->TextOut(rcStaticDraw.left, rcStaticDraw.top, TEXT("TextOutDemo"));

			Sleep(1000);

			pDC->FillSolidRect(rcStaticDraw, RGB(255, 0, 0));
            
			
			pWndDraw->ReleaseDC(pDC);
        }
    }
}

void CFDriverDemoTesterDlg::OnBnClickedBtnTestDesktop()
{
    BOOL bRet = FALSE;

	DWORD ProcessIdOfDesktop = 0;
	HWND hWndDesktop = ::GetDesktopWindow();
	DWORD dwThreadId = GetWindowThreadProcessId(hWndDesktop, &ProcessIdOfDesktop);
    HDC hDCDesktop = ::GetWindowDC(hWndDesktop);
    CClientDC dcClient(GetDlgItem(IDC_STATIC_DRAW));
    API_VERIFY(BitBlt(dcClient.m_hDC, 0, 0, m_rcClient.Width(), m_rcClient.Height(), hDCDesktop, 0, 0, SRCCOPY));

    FTLTRACE(TEXT("DesktopWindow=0x%x,hDCDesktop=0x%x, dcClient=0x%x, ProcessIdOfDesktop=%d, ThreadId=%d\n"),
        hWndDesktop, hDCDesktop, dcClient.m_hDC,  ProcessIdOfDesktop, dwThreadId);

    ::ReleaseDC(hWndDesktop, hDCDesktop);
}
