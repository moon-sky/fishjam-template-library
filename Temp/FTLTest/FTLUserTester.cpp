#include "StdAfx.h"
#include "FTLUserTester.h"
#include <ftlUser.h>

void CFTLUserTester::dump_TokenInfo()
{
    BOOL bRet = FALSE;
    HANDLE hToken = NULL;
    HANDLE hProcess = GetCurrentProcess();
    API_VERIFY(OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hToken));
    if (bRet)
    {
        API_VERIFY(CFUserUtil::DumpTokenInformation(hToken));

        SAFE_CLOSE_HANDLE(hToken, NULL);
    }
}