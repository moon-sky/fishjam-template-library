// PortableInstallerMaker.cpp : main source file for PortableInstallerMaker.exe
//

#include "stdafx.h"

#include "resource.h"
#include "MakerWizard.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	//HRESULT hRes = ::CoInitialize(NULL);
    HRESULT hRes = ::OleInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

    // add flags to support other controls
    AtlInitCommonControls(//ICC_BAR_CLASSES |
        ICC_WIN95_CLASSES |
        ICC_DATE_CLASSES |
        ICC_USEREX_CLASSES |
        ICC_COOL_CLASSES |
        ICC_PAGESCROLLER_CLASS |
        ICC_NATIVEFNTCTL_CLASS);	

    // We use a RichEdit control
    HINSTANCE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
    ATLASSERT(hInstRich != NULL);

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
        CMakerWizard  wizard;
        wizard.ExecuteWizard();
		//CMainDlg dlgMain;
		//nRet = dlgMain.DoModal();
	}
    ::FreeLibrary(hInstRich);

	_Module.Term();
	//::CoUninitialize();
    ::OleUninitialize();

	return nRet;
}
