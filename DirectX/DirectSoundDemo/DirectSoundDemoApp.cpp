#include "Stdafx.h"
#include "DirectSoundDemoApp.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDDrawApp

BEGIN_MESSAGE_MAP(CDirectSoundDemoApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// DirectSoundDemoApp 构造

CDirectSoundDemoApp::CDirectSoundDemoApp()
{
    // TODO: 在此处添加构造代码，
    // 将所有重要的初始化放置在 InitInstance 中
}

CDirectSoundDemoApp::~CDirectSoundDemoApp()
{

}
// 唯一的一个 CTimeSaverApp 对象

CDirectSoundDemoApp theApp;


// CTimeSaverApp 初始化

BOOL CDirectSoundDemoApp::InitInstance()
{
    // 如果一个运行在 Windows XP 上的应用程序清单指定要
    // 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
    //则需要 InitCommonControls()。否则，将无法创建窗口。
    InitCommonControls();

    CWinApp::InitInstance();

    AfxEnableControlContainer();
    //SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    CMainFrame *pMainFrame = new CMainFrame();
    BOOL bRet = FALSE;
    API_VERIFY(pMainFrame->Create(NULL,TEXT("DirectSoundDemoApp")));
    if(FALSE == bRet)
    {
        delete pMainFrame;
        return FALSE;
    }

    m_pMainWnd = pMainFrame;
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();

    return TRUE;
}
