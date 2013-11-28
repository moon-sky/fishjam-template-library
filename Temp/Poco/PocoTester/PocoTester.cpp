// PocoTester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PocoTester.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/mfc/TestRunner.h>
//#include <cppunit/ui/qt/QtTestRunner.h>
//#include <cppunit/ui/text/TestRunner.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//利用模板防止宏值重复定义 -- 利用宏在定义变量时同时特化一个类，如果重了的话，编译时就会报错
//  但目前定义以后宏以后，必须定义使用该宏的全局变量才能检查出来。
template <DWORD n>
class VALUE_DUPLICATE_CHECKER;

#define MY_VALUE(SECTION, OFFSET)  ( (SECTION) << 16 | (OFFSET) ); \
    template<> class VALUE_DUPLICATE_CHECKER < DWORD ( SECTION << 16 ) | ( OFFSET ) > {};
;
#define NAME1 MY_VALUE(100, 1)
#define NAME2 MY_VALUE(100, 1)

#if 0
const DWORD d1 = NAME1;
const DWORD d2 = NAME2;
#endif

// CPocoTesterApp

BEGIN_MESSAGE_MAP(CPocoTesterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPocoTesterApp construction

CPocoTesterApp::CPocoTesterApp()
{
    //TRACE(TEXT("%d:%d\n"), MYVALUE1, MYVALUE2);
}


// The one and only CPocoTesterApp object

CPocoTesterApp theApp;


// CPocoTesterApp initialization

BOOL CPocoTesterApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

    SetRegistryKey(TEXT("Poco Tester"));

    CPPUNIT_NS::MfcUi::TestRunner runner;
    //CPPUNIT_NS::QtUi::TestRunner runner;
    //CPPUNIT_NS::TextUi::TestRunner runner;
    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run();


	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
