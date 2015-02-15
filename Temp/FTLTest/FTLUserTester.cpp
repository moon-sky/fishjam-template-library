#include "StdAfx.h"
#include "FTLUserTester.h"
#include <ftlUser.h>


unsigned int CFTLUserTester::_DumpThreadTokenProc(void* pParam)
{
    CFTLUserTester* pThis = reinterpret_cast<CFTLUserTester*>(pParam);
    FTLTRACE(TEXT("[0x%x, %d]CFTLUserTester::_DumpThreadTokenProc, quit\n"), GetCurrentThread(), GetCurrentThreadId());
    return 0;
}

void CFTLUserTester::dump_TokenInfo()
{
    BOOL bRet = FALSE;

    HANDLE hToken = NULL;
    unsigned int threadId = 0;
    HANDLE hSubThread = (HANDLE)_beginthreadex(NULL, 0, _DumpThreadTokenProc, this, CREATE_SUSPENDED, &threadId);
    HANDLE handles[] = {
        GetCurrentProcess(),
        GetCurrentThread(),
        hSubThread
    };

    for (int i = 0; i < _countof(handles); i++)
    {
        API_VERIFY(OpenProcessToken(handles[i], TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hToken));
        if (bRet)
        {
            FTLTRACE(TEXT(">>>>>>>>>>>>>>> DumpTokenInformation %d <<<<<<<<<<<<<<<<<<<<<\n"), i);
            API_VERIFY(CFUserUtil::DumpTokenInformation(hToken));
            SAFE_CLOSE_HANDLE(hToken, NULL);
        }
    }

    ResumeThread(hSubThread);
    WaitForSingleObject(hSubThread, INFINITE);

}