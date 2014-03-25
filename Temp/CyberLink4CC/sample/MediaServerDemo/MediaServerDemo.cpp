// MediaServerDemo.cpp : main source file for MediaServerDemo.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainDlg.h"

#include <uhttp/util/Logger.h>
#include <uhttp/util/LoggerOutputDebugString.h>
#include <uhttp/util/Debug.h>

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	uHTTP::Debug::on();
	uHTTP::HostInterface::USE_ONLY_IPV4_ADDR = true;

	uHTTP::Logger *pLogger = uHTTP::Logger::GetSharedInstance();
	//pLogger->setLevel(uHTTP::LoggerLevel::INFO);
	pLogger->addTarget(new uHTTP::LoggerOutputDebugString());
	

	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;
	// BLOCK: Run application
	{
		CMainDlg dlgMain;
		nRet = dlgMain.DoModal();
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
