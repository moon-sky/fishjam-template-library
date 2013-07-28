#include "StdAfx.h"
#include "FDriverHelloTester.h"
#include "../FDriverDemoDefine.h"

#ifdef ENABLE_CPPUNIT

void CFDriverHelloTester::setUp()
{
    BOOL bRet = FALSE;

	TCHAR szDriverFileName[MAX_PATH] = {0};
	GetModuleFileName( NULL, szDriverFileName, MAX_PATH);
	LPTSTR pszFileName = PathFindFileName(szDriverFileName);
	FTLASSERT(pszFileName != NULL);
	if (pszFileName)
	{
		*pszFileName = NULL;
		StringCchCat(szDriverFileName, _countof(szDriverFileName), FDRIVER_DEMO_FILENAME);
		API_VERIFY(m_FDriverDemoController.InstallService(szDriverFileName, FDRIVER_DEMO_SERVICE_NAME));
	}
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

#endif //ENABLE_CPPUNIT