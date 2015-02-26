#include "StdAfx.h"
#include "FTLUserTester.h"
#include <ftlUser.h>
#include <Aclapi.h>

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

void CFTLUserTester::dump_NamedObjectSecurityInformation()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;
    TCHAR szFilePath[MAX_PATH] = {0};
    API_VERIFY(GetModuleFileName(NULL, szFilePath, _countof(szFilePath) - 1) > 0);

    if (bRet)
    {
        PSID pSidOwner = NULL;
        PSID pSidGroup = NULL;
        PACL pDacl = NULL;
        PACL pSacl = NULL;
        PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;
        DWORD dwRetrieveSecurityInfo = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION | LABEL_SECURITY_INFORMATION;
        DWORD dwReturn = GetNamedSecurityInfo(szFilePath, SE_FILE_OBJECT, dwRetrieveSecurityInfo, &pSidOwner, &pSidGroup, &pDacl, &pSacl, &pSecurityDescriptor);
        FTLASSERT(ERROR_SUCCESS == dwReturn);
        if (ERROR_SUCCESS == dwReturn)
        {
            CFStringFormater formater;
            CFStringFormater formaterSidOwner, formaterSidGroup, formaterDacl, formaterSacl, formaterSecurityDesc;
            COM_VERIFY(formater.Format(TEXT("SidOwner=%s, SidGroup=%s, Dacl=%s, Sacl=%s, SecurityDesc=%s\n"), 
                CFUserUtil::GetSidInfo(formaterSidOwner, pSidOwner, FALSE),
                CFUserUtil::GetSidInfo(formaterSidGroup, pSidGroup, FALSE),
                CFUserUtil::GetAclInfo(formaterDacl, pDacl),
                CFUserUtil::GetAclInfo(formaterSacl, pSacl),
                CFUserUtil::GetSecurityDescriptorinfo(formaterSecurityDesc, pSecurityDescriptor)
            ));
            SAFE_LOCAL_FREE(pSidOwner);
            SAFE_LOCAL_FREE(pSidGroup);
            SAFE_LOCAL_FREE(pDacl);
            SAFE_LOCAL_FREE(pSacl);
            SAFE_LOCAL_FREE(pSecurityDescriptor);
        }
        else
        {
            REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, dwReturn, GetNamedSecurityInfo);
        }
    }
    
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

void CFTLUserTester::test_IsVistaUACEnabled()
{
    BOOL bUACEnabled = CFUserUtil::IsVistaUACEnabled();
    CPPUNIT_ASSERT(FALSE == bUACEnabled);
}
