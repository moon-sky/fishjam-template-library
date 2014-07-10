#include "StdAfx.h"
#include "FTLShellTester.h"
#include "ftlShell.h"

void CFTLShellTester::test_ITEMIDLIST()
{
	BOOL bRet = FALSE;
	TCHAR szModulePat[MAX_PATH] = {0};
	API_VERIFY(GetModuleFileName(NULL, szModulePat, _countof(szModulePat)));

	LPITEMIDLIST pItemIdListModulePath = ILCreateFromPath(szModulePat);
	CPPUNIT_ASSERT(pItemIdListModulePath != NULL);
	if (pItemIdListModulePath)
	{
		ILFree(pItemIdListModulePath);
	}
}
