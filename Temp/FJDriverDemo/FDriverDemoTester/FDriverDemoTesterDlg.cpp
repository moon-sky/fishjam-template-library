
// FDriverDemoTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FDriverDemoTester.h"
#include "FDriverDemoTesterDlg.h"
#include "../FDriverDemoDefine.h"

#include <ftlSystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFDriverDemoTesterDlg dialog


CFDriverDemoTesterDlg::CFDriverDemoTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFDriverDemoTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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
		API_VERIFY(m_DriverController.InstallService(m_strDriverPath , FDRIVER_DEMO_SERVICE_NAME));
	}
}


void CFDriverDemoTesterDlg::OnBnClickedBtnOpenDriver()
{
	BOOL bRet = FALSE;
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

typedef struct _SCROLL_HOOK_TARGET
{
	HWND hTargetWindow;
	HANDLE hSelfProcess;
	HANDLE hTargetProcess;
	//WindowFromDC
} SCROLL_HOOK_TARGET, *PSCROLL_HOOK_TARGET;


void CFDriverDemoTesterDlg::OnBnClickedBtnInstallHook()
{
	BOOL bRet = FALSE;
	SCROLL_HOOK_TARGET	hookTarget;
	hookTarget.hTargetWindow = m_hWnd;
	hookTarget.hSelfProcess = (HANDLE)GetCurrentProcessId();
	hookTarget.hTargetProcess = (HANDLE)FTL::CFSystemUtil::GetPidFromProcessName(TEXT("csrss.exe"));
	API_VERIFY(m_DriverController.IoControl(IOCTL_FDRIVER_INSTALL_HOOK, &hookTarget, sizeof(hookTarget), NULL, 0));
}

void CFDriverDemoTesterDlg::OnBnClickedBtnUninstallHook()
{
	BOOL bRet = FALSE;
	API_VERIFY(m_DriverController.IoControl(IOCTL_FDRIVER_UNINSTALL_HOOK, NULL, 0, NULL, 0));

}
