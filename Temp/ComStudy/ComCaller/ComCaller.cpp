// ComCaller.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ComCaller.h"
#include "ComCallerDlg.h"

//包含学习资料的头文件
#include "ComAutomation.h"
#include "ComActiveX.h"
#include "ComDebug.h"
#include "ComIDL.h"
#include "ComReuse.h"
#include "COMRgs.h"
#include "ComStudy.h"
#include "ComThreadingModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "FJUtilityFun.h"

// CComCallerApp

BEGIN_MESSAGE_MAP(CComCallerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CComCallerApp construction

CComCallerApp::CComCallerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CComCallerApp object

CComCallerApp theApp;


// CComCallerApp initialization

BOOL CComCallerApp::InitInstance()
{
    //FishJam::Utility::CFUCrashHandler::SetDefaultCrashHandlerFilter();

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

    HRESULT hr = E_FAIL;
    BOOL bRet = FALSE;
    COM_VERIFY(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED));
    //API_VERIFY(AfxOleInit());

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    {
        CComCallerDlg dlg;
        m_pMainWnd = &dlg;
        INT_PTR nResponse = dlg.DoModal();
        if (nResponse == IDOK)
        {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with OK
        }
        else if (nResponse == IDCANCEL)
        {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with Cancel
	    }
    }

    //注意：由于 CComCallerDlg 中有智能指针的成员变量，要在析构时才会释放，因此必须保证相关的Release
    //      在调用 CoUninitialize 前完成，否则会在 ~CComPtrBase 处出现 内存访问异常(0xC0000005: Access violation reading location 0xXXXXXXX)
    //      方法有：1.加括号，保证CComCallerDlg先析构；2.在 CComCallerDlg::OnDestroy 中手工释放(注意：不能是OnClose，点击Cancel不到OnClose)
    CoUninitialize(); 

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
