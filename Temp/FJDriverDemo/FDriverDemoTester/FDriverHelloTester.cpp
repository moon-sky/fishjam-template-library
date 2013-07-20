#include "StdAfx.h"
#include "FDriverHelloTester.h"


void CFDriverHelloTester::setUp()
{
    BOOL bRet = FALSE;
    API_VERIFY(m_FDriverDemoController.InstallService());
}

void CFDriverHelloTester::tearDown()
{
    BOOL bRet = FALSE;
    API_VERIFY(m_FDriverDemoController.UnInstallService());
}

void CFDriverHelloTester::test_DriverHelloWorld()
{
    //CFMessageBoxHook hook(GetCurrentThreadId(),TEXT("MyClose"));
    //::MessageBox(NULL,TEXT("Test Text"),TEXT("Caption"),MB_OKCANCEL);
    CPPUNIT_ASSERT(1 == 1 );
}
