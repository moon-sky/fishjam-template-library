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

    HANDLE hTokenProcess = NULL;
    HANDLE hTokenCurThread = NULL;
    HANDLE hTokenNewThread = NULL;

    unsigned int threadId = 0;
    HANDLE hSubThread = (HANDLE)_beginthreadex(NULL, 0, _DumpThreadTokenProc, this, CREATE_SUSPENDED, &threadId);

    API_VERIFY(OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hTokenProcess));
    //API_VERIFY(OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hTokenCurThread));
    //API_VERIFY(OpenThreadToken(hSubThread, TOKEN_ALL_ACCESS, FALSE, &hTokenNewThread));
    HANDLE handles[] = {
        hTokenProcess,
        hTokenCurThread,
        hTokenNewThread
    };

    for (int i = 0; i < _countof(handles); i++)
    {
        FTLTRACE(TEXT(">>>>>>>>>>>>>>> DumpTokenInformation %d <<<<<<<<<<<<<<<<<<<<<\n"), i);
        if (NULL != handles[i])
        {
            API_VERIFY(CFUserUtil::DumpTokenInformation(handles[i]));
            SAFE_CLOSE_HANDLE(handles[i], NULL);
        }
    }

    ResumeThread(hSubThread);
    WaitForSingleObject(hSubThread, INFINITE);
    CloseHandle(hSubThread);
}

void CFTLUserTester::test_IsVistaUACEnabled()
{
    BOOL bUACEnabled = CFUserUtil::IsVistaUACEnabled();
    CPPUNIT_ASSERT(FALSE == bUACEnabled);
}

void CFTLUserTester::dump_WellKnownSid()
{
    BOOL bRet = FALSE;
    BYTE localAdminSID[SECURITY_MAX_SID_SIZE] = {0};  
    PSID pLocalAdminSID = &localAdminSID;  
    DWORD cbSID = sizeof(localAdminSID);

    //WinBuiltinAdministratorsSid -- 管理员账户组
    //WinWorldSid -- 所有账户(AllUser?)

    const int MAX_DEFINE_WELL_KNOWN_SID = (int)WinCapabilityRemovableStorageSid;
    for(int type = WinNullSid; type < MAX_DEFINE_WELL_KNOWN_SID; type++)
    {
        API_VERIFY_EXCEPT1(CreateWellKnownSid(WELL_KNOWN_SID_TYPE(type), NULL, pLocalAdminSID, &cbSID), ERROR_INVALID_PARAMETER);
        if (bRet)
        {
            FTL::CFStringFormater formater;
            FTLTRACE(TEXT("%s = %s\n"), CFUserUtil::GetWellKnownSidTypeString(WELL_KNOWN_SID_TYPE(type)), CFUserUtil::GetSidInfo(formater, pLocalAdminSID, FALSE));
        }
        else{
            DWORD dwLastError = GetLastError();
            FTLTRACE(TEXT("%s Fail, Reason=%d\n"), CFUserUtil::GetWellKnownSidTypeString(WELL_KNOWN_SID_TYPE(type)), dwLastError);
        }
    }
    
}