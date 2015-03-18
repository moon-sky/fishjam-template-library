// FTLDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "FTLDemoSheet.h"
#include <ftlCrashHandler.h>
#include <ftlGdi.h>
#include <ftlgdiplus.h>
#include <ftlSocket.h>
//#include "vld.h"
#include <ftlfile.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTLDemoApp

BEGIN_MESSAGE_MAP(CFTLDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFTLDemoApp 构造

CFTLDemoApp::CFTLDemoApp()
{
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFTLDemoApp 对象

CFTLDemoApp theApp;
//CComModule _Module;     //为了使用CFResourcelessDlg

// CFTLDemoApp 初始化

BOOL CFTLDemoApp::InitInstance()
{
    //FUNCTION_BLOCK_INIT();
    BOOL bRet = FALSE;
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    {
        //VLDEnable();
        //FAST_TRACE_OPTIONS traceOptions;
        //traceOptions.traceThreshold = tlTrace;
        //traceOptions.bWriteToFile = TRUE;
        //FTL::CFFastTrace::GetInstance().SetTraceOptions(&traceOptions);

        // 如果一个运行在 Windows XP 上的应用程序清单指定要
        // 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
        //则需要 InitCommonControlsEx()。否则，将无法创建窗口。
        INITCOMMONCONTROLSEX InitCtrls;
        InitCtrls.dwSize = sizeof(InitCtrls);
        // 将它设置为包括所有要在应用程序中使用的
        // 公共控件类。
        InitCtrls.dwICC = ICC_WIN95_CLASSES;
        InitCommonControlsEx(&InitCtrls);

        CWinApp::InitInstance();

        AfxEnableControlContainer();

        //FTL::CFFileUtil::DumpAllAPISystemFolderPath();

        // 标准初始化
        // 如果未使用这些功能并希望减小
        // 最终可执行文件的大小，则应移除下列
        // 不需要的特定初始化例程
        // 更改用于存储设置的注册表项
        SetRegistryKey(_T("Fishjam"));
        API_VERIFY(FTL::CFGdiUtil::SetProcessDPIAdware());
        FTLTRACE(TEXT("SetProcessDPIAdware:%d\n"), bRet);

		FTL::CFCrashHandler crashHandler;
        crashHandler.SetDefaultCrashHandlerFilter();

        FTL::CFAutoGdiplus   gdiPlusInitObject;
        FTL::CFWinsockEnvInit winsockInit;

        //CFTLDemoDlg dlg;
        //m_pMainWnd = &dlg;
        //INT_PTR nResponse = dlg.DoModal();
        CFTLDemoSheet sheet(TEXT("FTL Demo"));
        INT_PTR nResponse  = sheet.DoModal();
        if (nResponse == IDOK)
        {
        }
        else if (nResponse == IDCANCEL)
        {
        }

        // 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
        //  而不是启动应用程序的消息泵。
    }
    CoUninitialize();
    //FUNCTION_BLOCK_UNINIT();
    return FALSE;
}

CString GetCurrentTimeString()
{
    SYSTEMTIME st = {0};
    GetLocalTime(&st);
    CString strTime;
    strTime.Format(TEXT("%02d:%02d:%02d:%03d"),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
    return strTime;
}