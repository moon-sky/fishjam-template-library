// DatabaseTester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DatabaseTester.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/mfc/TestRunner.h>
//#include <cppunit/ui/qt/QtTestRunner.h>
//#include <cppunit/ui/text/TestRunner.h>
#include <ftlBase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDatabaseTesterApp

BEGIN_MESSAGE_MAP(CDatabaseTesterApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDatabaseTesterApp construction

CDatabaseTesterApp::CDatabaseTesterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDatabaseTesterApp object

CDatabaseTesterApp theApp;


// CDatabaseTesterApp initialization

BOOL CDatabaseTesterApp::InitInstance()
{
    // InitCommonControls() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    InitCommonControls();

    CWinApp::InitInstance();
    CWinApp::SetRegistryKey("DatabaseTester");
    
    AfxEnableControlContainer();

    CPPUNIT_NS::MfcUi::TestRunner runner;
    //CPPUNIT_NS::QtUi::TestRunner runner;
    //CPPUNIT_NS::TextUi::TestRunner runner;
    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
