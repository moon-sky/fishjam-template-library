#ifndef FTL_USER_HPP
#define FTL_USER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlUser.h"
#endif

namespace FTL
{
    BOOL CFUserUtil::IsProcessUserAdministrator()
    {
        BOOL bRet = FALSE;
        BOOL bIsAdmin = FALSE;
        DWORD dwError = 0;

        HANDLE                   hToken = NULL;
        DWORD                    cbTokenGroups;
        DWORD                    dwGroup;
        SID_IDENTIFIER_AUTHORITY SystemSidAuthority= SECURITY_NT_AUTHORITY;

        API_VERIFY(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY, FALSE, &hToken));
        if (!bRet)
        {
            if (ERROR_NO_TOKEN == GetLastError())
            {
                // If the thread does not have an access token, we'll examine the access token associated with the process.
                API_VERIFY(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY, &hToken));
            }
        }
        if (bRet)
        {
            API_VERIFY_EXCEPT1(GetTokenInformation(hToken,TokenGroups,NULL,0,&cbTokenGroups), ERROR_INSUFFICIENT_BUFFER);
            dwError = GetLastError();
            if (ERROR_INSUFFICIENT_BUFFER == dwError)
            {
                CFMemAllocator<BYTE> tokenBuf(cbTokenGroups);
                //Now we ask for the group information again
                API_VERIFY(GetTokenInformation(hToken, TokenGroups, (LPVOID)tokenBuf, cbTokenGroups, &cbTokenGroups));
                if (bRet)
                {
                    // Now we must create a System Identifier for the Admin group.
                    PSID psidAdmin = NULL;
                    API_VERIFY(AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, 
                        DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&psidAdmin));
                    if (bRet)
                    {
                        TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)tokenBuf;
                        for (dwGroup = 0; dwGroup < pTokenGroups->GroupCount; dwGroup++)
                        {
                            if (EqualSid(pTokenGroups->Groups[dwGroup].Sid,psidAdmin))
                            {
                                bIsAdmin = TRUE;
                                break;
                            }
                        }
                        FreeSid(psidAdmin);
                    }
                }
            }
        }

        return bIsAdmin;
    }


    //////////////////////////////////////////////////////////////////////////
    CFUserPrivileges::CFUserPrivileges()
    {
        m_hPolicy = NULL;
    }
    CFUserPrivileges::~CFUserPrivileges()
    {
        Close();
    }
    BOOL CFUserPrivileges::Open()
    {
        BOOL bRet = FALSE;
        LSA_OBJECT_ATTRIBUTES ObjectAttributes = {0};
        LsaOpenPolicy(NULL, &ObjectAttributes, POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, m_hPolicy);
        return bRet;
    }
    VOID CFUserPrivileges::Close()
    {
        if (m_hPolicy)
        {
            LsaClose(m_hPolicy);
            m_hPolicy = NULL;
        }
    }

    void CFUserPrivileges::_InitLsaString(PLSA_UNICODE_STRING LsaString,LPWSTR String)
    {
        DWORD StringLength;

        if (String == NULL) {
            LsaString->Buffer = NULL;
            LsaString->Length = 0;
            LsaString->MaximumLength = 0;
            return;
        }

        StringLength = wcslen(String);
        LsaString->Buffer = String;
        LsaString->Length = (USHORT) StringLength * sizeof(WCHAR);
        LsaString->MaximumLength=(USHORT)(StringLength+1) * sizeof(WCHAR);
    }
}
#endif //FTL_USER_HPP