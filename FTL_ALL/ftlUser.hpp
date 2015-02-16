#ifndef FTL_USER_HPP
#define FTL_USER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlUser.h"
#endif
#include <sddl.h>
#include <ftlConversion.h>
#include <ftlSystem.h>

namespace FTL
{
#define GET_ALL_TOKEN_INFOMATION

//winnt.h 中没有定义足够的枚举(sdk版本问题) -- 如何根据sdk版本自动检测编译方式?
#ifdef GET_ALL_TOKEN_INFOMATION
    enum MORE_WELL_KNOWN_SID_TYPE{
        WinApplicationPackageAuthoritySid            = 83,
        WinBuiltinAnyPackageSid                      = 84,
        WinCapabilityInternetClientSid               = 85,
        WinCapabilityInternetClientServerSid         = 86,
        WinCapabilityPrivateNetworkClientServerSid   = 87,
        WinCapabilityPicturesLibrarySid              = 88,
        WinCapabilityVideosLibrarySid                = 89,
        WinCapabilityMusicLibrarySid                 = 90,
        WinCapabilityDocumentsLibrarySid             = 91,
        WinCapabilitySharedUserCertificatesSid       = 92,
        WinCapabilityEnterpriseAuthenticationSid     = 93,
        WinCapabilityRemovableStorageSid             = 94
    };


    enum MORE_TOKEN_INFORMATION_CLASS{
        TokenNone = 0,
        TokenIsAppContainer = 29,
        TokenCapabilities,
        TokenAppContainerSid,
        TokenAppContainerNumber,
        TokenUserClaimAttributes,
        TokenDeviceClaimAttributes,
        TokenRestrictedUserClaimAttributes,
        TokenRestrictedDeviceClaimAttributes,
        TokenDeviceGroups,
        TokenRestrictedDeviceGroups,
        TokenSecurityAttributes,
        TokenIsRestricted
    };
    typedef struct _TOKEN_APPCONTAINER_INFORMATION {
        PSID TokenAppContainer;
    } TOKEN_APPCONTAINER_INFORMATION, *PTOKEN_APPCONTAINER_INFORMATION;

    typedef struct _CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE {
        DWORD64 Version;
        PWSTR   Name;
    } CLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE, *PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE;

    typedef struct _CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE {
        PVOID pValue;
        DWORD ValueLength;
    } CLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE, *PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE;

    typedef struct _CLAIM_SECURITY_ATTRIBUTE_V1 {
        PWSTR Name;
        WORD  ValueType;
        WORD  Reserved;
        DWORD Flags;
        DWORD ValueCount;
        union {
            PLONG64                                      pInt64;
            PDWORD64                                     pUint64;
            PWSTR                                        *ppString;
            PCLAIM_SECURITY_ATTRIBUTE_FQBN_VALUE         pFqbn;
            PCLAIM_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE pOctetString;
        } Values;
    } CLAIM_SECURITY_ATTRIBUTE_V1, *PCLAIM_SECURITY_ATTRIBUTE_V1;

    typedef struct _CLAIM_SECURITY_ATTRIBUTES_INFORMATION {
        WORD  Version;
        WORD  Reserved;
        DWORD AttributeCount;
        union {
            PCLAIM_SECURITY_ATTRIBUTE_V1 pAttributeV1;
        } Attribute;
    } CLAIM_SECURITY_ATTRIBUTES_INFORMATION, *PCLAIM_SECURITY_ATTRIBUTES_INFORMATION;

#endif //GET_ALL_TOKEN_INFOMATION

    LPCTSTR CFUserUtil::GetWellKnownSidTypeString(WELL_KNOWN_SID_TYPE sidType){
        switch (sidType)
        {
            HANDLE_CASE_RETURN_STRING(WinNullSid);
            HANDLE_CASE_RETURN_STRING(WinWorldSid);
            HANDLE_CASE_RETURN_STRING(WinLocalSid);
            HANDLE_CASE_RETURN_STRING(WinCreatorOwnerSid);
            HANDLE_CASE_RETURN_STRING(WinCreatorGroupSid);
            HANDLE_CASE_RETURN_STRING(WinCreatorOwnerServerSid);
            HANDLE_CASE_RETURN_STRING(WinCreatorGroupServerSid);
            HANDLE_CASE_RETURN_STRING(WinNtAuthoritySid);
            HANDLE_CASE_RETURN_STRING(WinDialupSid);
            HANDLE_CASE_RETURN_STRING(WinNetworkSid);
            HANDLE_CASE_RETURN_STRING(WinBatchSid);
            HANDLE_CASE_RETURN_STRING(WinInteractiveSid);
            HANDLE_CASE_RETURN_STRING(WinServiceSid);
            HANDLE_CASE_RETURN_STRING(WinAnonymousSid);
            HANDLE_CASE_RETURN_STRING(WinProxySid);
            HANDLE_CASE_RETURN_STRING(WinEnterpriseControllersSid);
            HANDLE_CASE_RETURN_STRING(WinSelfSid);
            HANDLE_CASE_RETURN_STRING(WinAuthenticatedUserSid);
            HANDLE_CASE_RETURN_STRING(WinRestrictedCodeSid);
            HANDLE_CASE_RETURN_STRING(WinTerminalServerSid);
            HANDLE_CASE_RETURN_STRING(WinRemoteLogonIdSid);
            HANDLE_CASE_RETURN_STRING(WinLogonIdsSid);
            HANDLE_CASE_RETURN_STRING(WinLocalSystemSid);
            HANDLE_CASE_RETURN_STRING(WinLocalServiceSid);
            HANDLE_CASE_RETURN_STRING(WinNetworkServiceSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinDomainSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinAdministratorsSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinUsersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinGuestsSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinPowerUsersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinAccountOperatorsSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinSystemOperatorsSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinPrintOperatorsSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinBackupOperatorsSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinReplicatorSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinPreWindows2000CompatibleAccessSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinRemoteDesktopUsersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinNetworkConfigurationOperatorsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountAdministratorSid);
            HANDLE_CASE_RETURN_STRING(WinAccountGuestSid);
            HANDLE_CASE_RETURN_STRING(WinAccountKrbtgtSid);
            HANDLE_CASE_RETURN_STRING(WinAccountDomainAdminsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountDomainUsersSid);
            HANDLE_CASE_RETURN_STRING(WinAccountDomainGuestsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountComputersSid);
            HANDLE_CASE_RETURN_STRING(WinAccountControllersSid);
            HANDLE_CASE_RETURN_STRING(WinAccountCertAdminsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountSchemaAdminsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountEnterpriseAdminsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountPolicyAdminsSid);
            HANDLE_CASE_RETURN_STRING(WinAccountRasAndIasServersSid);
            HANDLE_CASE_RETURN_STRING(WinNTLMAuthenticationSid);
            HANDLE_CASE_RETURN_STRING(WinDigestAuthenticationSid);
            HANDLE_CASE_RETURN_STRING(WinSChannelAuthenticationSid);
            HANDLE_CASE_RETURN_STRING(WinThisOrganizationSid);
            HANDLE_CASE_RETURN_STRING(WinOtherOrganizationSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinIncomingForestTrustBuildersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinPerfMonitoringUsersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinPerfLoggingUsersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinAuthorizationAccessSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinTerminalServerLicenseServersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinDCOMUsersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinIUsersSid);
            HANDLE_CASE_RETURN_STRING(WinIUserSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinCryptoOperatorsSid);
            HANDLE_CASE_RETURN_STRING(WinUntrustedLabelSid);
            HANDLE_CASE_RETURN_STRING(WinLowLabelSid);
            HANDLE_CASE_RETURN_STRING(WinMediumLabelSid);
            HANDLE_CASE_RETURN_STRING(WinHighLabelSid);
            HANDLE_CASE_RETURN_STRING(WinSystemLabelSid);
            HANDLE_CASE_RETURN_STRING(WinWriteRestrictedCodeSid);
            HANDLE_CASE_RETURN_STRING(WinCreatorOwnerRightsSid);
            HANDLE_CASE_RETURN_STRING(WinCacheablePrincipalsGroupSid);
            HANDLE_CASE_RETURN_STRING(WinNonCacheablePrincipalsGroupSid);
            HANDLE_CASE_RETURN_STRING(WinEnterpriseReadonlyControllersSid);
            HANDLE_CASE_RETURN_STRING(WinAccountReadonlyControllersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinEventLogReadersGroup);
            HANDLE_CASE_RETURN_STRING(WinNewEnterpriseReadonlyControllersSid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinCertSvcDComAccessGroup);
            HANDLE_CASE_RETURN_STRING(WinMediumPlusLabelSid);
            HANDLE_CASE_RETURN_STRING(WinLocalLogonSid);
            HANDLE_CASE_RETURN_STRING(WinConsoleLogonSid);
            HANDLE_CASE_RETURN_STRING(WinThisOrganizationCertificateSid);

#ifdef GET_ALL_TOKEN_INFOMATION
            HANDLE_CASE_RETURN_STRING(WinApplicationPackageAuthoritySid);
            HANDLE_CASE_RETURN_STRING(WinBuiltinAnyPackageSid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityInternetClientSid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityInternetClientServerSid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityPrivateNetworkClientServerSid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityPicturesLibrarySid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityVideosLibrarySid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityMusicLibrarySid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityDocumentsLibrarySid);
            HANDLE_CASE_RETURN_STRING(WinCapabilitySharedUserCertificatesSid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityEnterpriseAuthenticationSid);
            HANDLE_CASE_RETURN_STRING(WinCapabilityRemovableStorageSid);
            //HANDLE_CASE_RETURN_STRING(xxxxx);
#endif //GET_ALL_TOKEN_INFOMATION
        default:
            FTLTRACEEX(tlError, TEXT("Unknown the well known sid type: %d\n"), sidType);
            FTLASSERT(FALSE && TEXT("Unknown the well known sid type"));
            break;
        }
        return TEXT("");
        
    }

    IntegrityLevel CFUserUtil::GetIntegrityLevel(DWORD dwIntegrityLevel)
    {
        IntegrityLevel ilRet = ilUnknown;

        if(dwIntegrityLevel >= SECURITY_MANDATORY_PROTECTED_PROCESS_RID){
            ilRet = ilProtectedProcess;
        }else if(dwIntegrityLevel >= SECURITY_MANDATORY_SYSTEM_RID){
            ilRet = ilSystem;
        }else if(dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID){
            ilRet = ilHigh;
        }else if(dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID){
            ilRet = ilMedium;
        }else if(dwIntegrityLevel >= SECURITY_MANDATORY_LOW_RID){
            ilRet = ilLow;
        }else{
            ilRet = ilUntrusted;
        }
        return ilRet;
    }

    LPCTSTR CFUserUtil::GetIntegrityLevelString(IntegrityLevel iLevel)
    {
        switch (iLevel)
        {
            HANDLE_CASE_RETURN_STRING(ilUnknown);
            HANDLE_CASE_RETURN_STRING(ilAppContainer);
            HANDLE_CASE_RETURN_STRING(ilLowCompat);
            HANDLE_CASE_RETURN_STRING(ilUntrusted);
            HANDLE_CASE_RETURN_STRING(ilLow);
            HANDLE_CASE_RETURN_STRING(ilMedium);
            HANDLE_CASE_RETURN_STRING(ilHigh);
            HANDLE_CASE_RETURN_STRING(ilSystem);
            HANDLE_CASE_RETURN_STRING(ilProtectedProcess);
        default:
            FTLASSERT(FALSE);
            break;
        }
        return NULL;
    }

    LPCTSTR CFUserUtil::GetAclInfo(CFStringFormater& formater, PACL pAcl)
    {
        HRESULT hr = E_FAIL;
        FTLASSERT(ACL_REVISION == pAcl->AclRevision || ACL_REVISION_DS == pAcl->AclRevision);
        COM_VERIFY(formater.Format(TEXT("ver=%d, aceCount=%d"), pAcl->AclRevision, pAcl->AceCount));
        return formater.GetString();
    }

    LPCTSTR CFUserUtil::GetPrivilegeNameByLuid(CFStringFormater& formater, PLUID pLuid, LPCTSTR lpSystemName /* = NULL */)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;
        TCHAR szPrivilegeName[40] = {0};
        DWORD dwPrivilegeNameLength = _countof(szPrivilegeName) - 1;
        API_VERIFY(LookupPrivilegeName(lpSystemName, pLuid, szPrivilegeName, &dwPrivilegeNameLength));
        COM_VERIFY(formater.Format(TEXT("%s"), szPrivilegeName));
        return formater.GetString();
    }

    LPCTSTR CFUserUtil::GetSidInfo(CFStringFormater& formater, PSID pSid, BOOL bGetSubAuthority)
    {
        //SID 的信息一般是通过API函数操作, 比如 EqualSid 比较用于比较是否和 Admin 的SID 相同
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;

        FTLASSERT(IsValidSid(pSid));

        LPTSTR pStringSid = NULL;
        API_VERIFY(ConvertSidToStringSid(pSid, &pStringSid));
        if (bRet)
        {
            COM_VERIFY(formater.AppendFormat(TEXT("%s"), pStringSid));
            LocalFree(pStringSid); 

            // Get the identifier authority value from the SID.
            PSID_IDENTIFIER_AUTHORITY pSidIdAuth =  GetSidIdentifierAuthority(pSid);
            if (pSidIdAuth)
            {
                LONG nStrBinaryCount = 0;
                API_VERIFY(CFConvUtil::HexFromBinary((const BYTE*)pSidIdAuth, sizeof(SID_IDENTIFIER_AUTHORITY), NULL, &nStrBinaryCount, _T(',')));
                CFMemAllocator<TCHAR> sidAuthBuf(nStrBinaryCount);
                API_VERIFY(CFConvUtil::HexFromBinary((const BYTE*)pSidIdAuth, sizeof(SID_IDENTIFIER_AUTHORITY), sidAuthBuf.GetMemory(), &nStrBinaryCount, _T(',')));
            }

            // Get the number of subauthorities in the SID.
            if (bGetSubAuthority)
            {
                DWORD dwSubAuthorityCount = *GetSidSubAuthorityCount(pSid);
                COM_VERIFY(formater.AppendFormat(TEXT(",SubAuthorityCount=%d,"), dwSubAuthorityCount));
                for (DWORD dwSubAuthorIndex = 0; dwSubAuthorIndex < dwSubAuthorityCount; dwSubAuthorIndex++)
                {
                    DWORD dwSubAuthority = *GetSidSubAuthority(pSid, dwSubAuthorIndex);
                    COM_VERIFY(formater.AppendFormat(TEXT("[%d]=%d(0x%x),"), dwSubAuthorIndex, dwSubAuthority, dwSubAuthority));

                    //TokenIntegrityLevel 的最后一个 SubAuthority 的值代表 Integrity Level
                    if(dwSubAuthorIndex == (dwSubAuthorityCount - 1))
                    {
                        IntegrityLevel il = GetIntegrityLevel(dwSubAuthority);
                        COM_VERIFY(formater.AppendFormat(TEXT("IL=%d(%s),"), il, GetIntegrityLevelString(il)));
                    }
                }
            }
        }

        return formater.GetString();
    }

    LPCTSTR CFUserUtil::GetSidAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /* = TEXT("|") */)
    {
        DWORD dwOldAttributes = dwAttributes;
        formater.Format(TEXT(""));

        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_MANDATORY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_ENABLED_BY_DEFAULT, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_ENABLED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_OWNER, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_USE_FOR_DENY_ONLY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_INTEGRITY, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_INTEGRITY_ENABLED, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_LOGON_ID, pszDivide);
        HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_GROUP_RESOURCE, pszDivide);
        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, xxxx, pszDivide);

        FTLASSERT( 0 == dwAttributes);
        if (0 != dwAttributes)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Sid Attributes String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, dwOldAttributes, dwAttributes);
        }
        return formater.GetString();
    }

    LPCTSTR CFUserUtil::GetSidAndAttributesInfo(CFStringFormater& formater, PSID_AND_ATTRIBUTES pSidAndAttributes, 
        BOOL bGetSubAuthority ,LPCTSTR pszDivide /* = TEXT("|") */)
    {
        HRESULT hr = E_FAIL;
        CFStringFormater formaterSid, formaterAttributes;
        
        COM_VERIFY(formater.Format(TEXT("sid=%s, attributes=0x%x(%s)"), GetSidInfo(formaterSid, pSidAndAttributes->Sid, bGetSubAuthority),
            pSidAndAttributes->Attributes, GetSidAttributesString(formaterAttributes, pSidAndAttributes->Attributes, pszDivide)));
        return formater.GetString();
    }

    LPCTSTR CFUserUtil::GetPrivilegeAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /* = TEXT("|") */)
    {
        DWORD dwOldAttributes = dwAttributes;
        formater.Format(TEXT(""));
        if (0 == dwAttributes)
        {
            formater.Format(TEXT("Disabled"));
        }
        else
        {
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_PRIVILEGE_ENABLED_BY_DEFAULT, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_PRIVILEGE_ENABLED, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_PRIVILEGE_REMOVED, pszDivide);
            HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, SE_PRIVILEGE_USED_FOR_ACCESS, pszDivide);
        }

        //HANDLE_COMBINATION_VALUE_TO_STRING(formater, dwAttributes, xxxx, pszDivide);
        FTLASSERT( 0 == dwAttributes);
        if (0 != dwAttributes)
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("%s:Check Sid Attributes String Not Complete, total=0x%08x, remain=0x%08x\n"),
                __FILE__LINE__, dwOldAttributes, dwAttributes);
        }
        return formater.GetString();
    }

    BOOL CFUserUtil::IsProcessUserAdministrator()
    {
        if(CFOSInfo::Instance()->GetOSType() <= CFOSInfo::ostWinMe ){
            return TRUE;
        }

        BOOL bRet = FALSE;
        BOOL bIsAdmin = FALSE;
        DWORD dwError = 0;

        HANDLE                   hToken = NULL;
        DWORD                    cbTokenGroups;
        DWORD                    dwGroup;

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
            API_VERIFY_EXCEPT1(GetTokenInformation(hToken, TokenGroups, NULL, 0, &cbTokenGroups), ERROR_INSUFFICIENT_BUFFER);
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
                    SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;
                    API_VERIFY(AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, 
                        DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&psidAdmin));
                    if (bRet)
                    {
                        TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)tokenBuf.GetMemory();
                        for (dwGroup = 0; dwGroup < pTokenGroups->GroupCount; dwGroup++)
                        {
                            if (EqualSid(pTokenGroups->Groups[dwGroup].Sid, psidAdmin))
                            {
                                bIsAdmin = TRUE;
                                break;
                            }
                        }
                        FreeSid(psidAdmin);
                    }
                }
            }
            SAFE_CLOSE_HANDLE(hToken, NULL);
        }
        FTLASSERT(NULL == hToken);
        return bIsAdmin;
    }

    BOOL CFUserUtil::IsVistaUACEnabled()
    {
        if(CFOSInfo::Instance()->GetOSType() < CFOSInfo::ostVista){
            return FALSE;
        }
        BOOL bRet = FALSE;
        BOOL bUACEnabled = FALSE;

        HKEY hKey = NULL;
        LSTATUS lRet = ERROR_SUCCESS;
        REG_VERIFY(::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"),
            0,  KEY_READ, &hKey));//STANDARD_RIGHTS_READ | KEY_QUERY_VALUE, &hKey));
        if (ERROR_SUCCESS == lRet)
        {
            DWORD dwUAC	= 0;
            DWORD nSize = sizeof(dwUAC);
            DWORD dwType = REG_NONE;
            //REG_VERIFY(::RegSetValueEx(hKey, TEXT("fishjam_UACTestPos"), NULL, REG_DWORD, (const BYTE*)&dwUAC, nSize));
            REG_VERIFY(::RegQueryValueEx(hKey, TEXT("EnableLUA"), NULL, &dwType, (LPBYTE)&dwUAC, &nSize));
            if(ERROR_SUCCESS == lRet)
            {
                bUACEnabled =  (1 == dwUAC);
            }
            SAFE_CLOSE_REG(hKey);
        }
        return bUACEnabled;
    }

    IntegrityLevel CFUserUtil::GetProcessIntegrityLevel(HANDLE hProcess)
    {
        const TOKEN_INFORMATION_CLASS kTokenIsAppContainer = static_cast<TOKEN_INFORMATION_CLASS>(29);  // TokenIsAppContainer
        BOOL bRet = FALSE;

        HANDLE hToken = NULL;
        DWORD dwLengthNeeded = 0;
        DWORD dwError = ERROR_SUCCESS;

        PTOKEN_MANDATORY_LABEL pTIL = NULL;
        //LPWSTR pStringSid;
        DWORD dwIntegrityLevel;
        IntegrityLevel retIL = ilUnknown;

       API_VERIFY(OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_QUERY_SOURCE, &hToken));
       if (bRet)
       {
           DWORD returned_size = 0;
           DWORD retval = 0;
           API_VERIFY(GetTokenInformation(hToken, kTokenIsAppContainer, &retval, sizeof(retval), &returned_size));
            if(bRet)
           {
               if ( returned_size == sizeof(retval) && retval != 0 )
               {
                   retIL = ilAppContainer;
               }
           }

           // Get the Integrity level.
           if (retIL != ilAppContainer && 
               ! GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwLengthNeeded))
           {
               dwError = GetLastError();
               if (dwError != ERROR_INSUFFICIENT_BUFFER)
               {
                   CloseHandle(hToken);
                   return retIL;
               }

               pTIL = (PTOKEN_MANDATORY_LABEL)LocalAlloc(0, dwLengthNeeded);
               if (pTIL == NULL)
               {
                   CloseHandle(hToken);
                   return retIL;
               }
               if (GetTokenInformation(hToken, TokenIntegrityLevel, 
                   pTIL, dwLengthNeeded, &dwLengthNeeded))
               {
                   dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid, 
                       (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid)-1));

                   if (dwIntegrityLevel < SECURITY_MANDATORY_MEDIUM_RID)
                   {
                       // Low Integrity
                       retIL = ilLow;
                   }

                   else if (dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID && 
                       dwIntegrityLevel < SECURITY_MANDATORY_HIGH_RID)
                   {
                       // Medium Integrity
                       retIL = ilMedium;
                   }
                   else if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID)
                   {
                       // High Integrity
                       retIL = ilHigh;
                   }
               }
               LocalFree(pTIL);
           }
           CloseHandle(hToken);
           if ( retIL != ilLow)
           {
               //DLOG (_T("##11]] GetIntegrity Level : %s"), 
               //	  retIL == IEIL_APPCONTAIN ? _T("AppCon") : retIL == IEIL_MEDIUM ?  _T("Midium") : _T("ELSE"));
           }

       }
        return retIL;
    }

    BOOL CFUserUtil::CreateEmptySecurityAttributes( SECURITY_ATTRIBUTES* pSecurityAttr )
    {
        FTLASSERT(FALSE); //TODO: 函数不正确: 怎么传递参数? 有局部变量 SECURITY_DESCRIPTOR 

        BOOL bRet = FALSE;
        SECURITY_DESCRIPTOR sd = { 0 };
        API_VERIFY(InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION));
        API_VERIFY(SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE));

        pSecurityAttr->nLength = sizeof(SECURITY_ATTRIBUTES);
        pSecurityAttr->lpSecurityDescriptor = &sd;
        pSecurityAttr->bInheritHandle = FALSE;
        
        return bRet;
    }

    //h -- hToken
    //c -- TOKEN_INFORMATION_CLASS, such as TokenUser
    //a -- CFMemAllocator instance
    //l -- ReturnLength
    //e -- exception error for GetTokenInformation
#define GET_TOKEN_INFORMATION_DYNAMIC(h, c, a, l, e) \
    API_VERIFY_EXCEPT2(GetTokenInformation(h, (TOKEN_INFORMATION_CLASS)c, NULL, 0, &l), ERROR_INSUFFICIENT_BUFFER, e); \
    if(!bRet) { \
        FTLTRACEEX(FTL::tlDetail,  TEXT("GetTokenInformation: %d need memory Bytes %d\n"), c, l); \
        if((ERROR_INSUFFICIENT_BUFFER == GetLastError()) && (l > 0)){ \
            DWORD dwWant = l;\
            API_VERIFY_EXCEPT1( (GetTokenInformation(h, (TOKEN_INFORMATION_CLASS)c, (LPVOID)a.GetMemory(dwWant * 2), dwWant * 2, &l) && (dwWant == l)), e); \
        }\
    }

    typedef LPCTSTR (WINAPI *PTOKEN_INFO_TYPE_DUMP_PROC)(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
    struct TokenInfoDumpParams{
        DWORD tokenInformationClass;
        LPCTSTR pszClassName;
        PTOKEN_INFO_TYPE_DUMP_PROC  pDumpProc;
        DWORD dwSkipError;
    };

    LPCTSTR WINAPI CFUserUtil::GetTokenDwordInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        DWORD *pDword = (DWORD*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("%d(0x%x)"), *pDword, *pDword));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenReservedInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        formater.Format(TEXT("<Reserved>, value=0x%p"), TokenInformation);
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenUserInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        CFStringFormater formaterSid, formaterAttributes;

        TOKEN_USER* pTokenUser = (TOKEN_USER*)TokenInformation;
        DWORD dwAttributes = pTokenUser->User.Attributes;
        COM_VERIFY(formater.Format(TEXT("Sid=%s, Attributes=0x%x(%s)"), GetSidInfo(formaterSid, pTokenUser->User.Sid, FALSE), 
            dwAttributes, GetSidAttributesString(formaterAttributes, dwAttributes, TEXT("|"))));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenGroupsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)TokenInformation;
        formater.Format(TEXT("Count=%d"), pTokenGroups->GroupCount);
        for (DWORD dwIndex = 0; dwIndex < pTokenGroups->GroupCount; dwIndex++)
        {
            CFStringFormater formaterSid, formaterAttributes;
            DWORD dwAttributes = pTokenGroups->Groups[dwIndex].Attributes;
            formater.AppendFormat(TEXT("\n  [%d]=%s, Attributes=0x%x(%s)"), dwIndex, GetSidInfo(formaterSid, pTokenGroups->Groups[dwIndex].Sid, FALSE), 
                dwAttributes, GetSidAttributesString(formaterAttributes, dwAttributes, TEXT("|")));
        }

        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        BOOL bRet = FALSE;
        TOKEN_PRIVILEGES* pTokenPrivileges = (TOKEN_PRIVILEGES*)TokenInformation;
        formater.Format(TEXT("Count=%d"), pTokenPrivileges->PrivilegeCount);
        TCHAR szPrivilegeName[40] = {0};
        for (DWORD dwIndex = 0; dwIndex < pTokenPrivileges->PrivilegeCount; dwIndex++)
        {
            DWORD dwNameLength = _countof(szPrivilegeName);
            LUID_AND_ATTRIBUTES* pPrivilege = &pTokenPrivileges->Privileges[dwIndex];
            API_VERIFY(LookupPrivilegeName(NULL, &(pPrivilege->Luid), szPrivilegeName, &dwNameLength));
            if(bRet){
                CFStringFormater formaterPrivilege;
                formater.AppendFormat(TEXT("\n  [%d]=%s, Attributes=0x%x(%s)"), dwIndex, szPrivilegeName, pPrivilege->Attributes, 
                    GetPrivilegeAttributesString(formaterPrivilege, pPrivilege->Attributes, TEXT("|")));
            }
        }
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenOwnerInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        CFStringFormater formaterSid;
        TOKEN_OWNER* pTokenOwner = (TOKEN_OWNER*)TokenInformation;
        formater.Format(TEXT("Owner=%s"), GetSidInfo(formaterSid, pTokenOwner->Owner, FALSE));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenPrimaryGroupInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        CFStringFormater formaterSid;
        TOKEN_PRIMARY_GROUP* pTokenPrimaryGroup = (TOKEN_PRIMARY_GROUP*)TokenInformation;
        formater.Format(TEXT("PrimaryGroup=%s"), GetSidInfo(formaterSid, pTokenPrimaryGroup->PrimaryGroup, FALSE));

        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenDefaultDaclInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        CFStringFormater formaterAcl;
        TOKEN_DEFAULT_DACL* pTokenDefaultDacl = (TOKEN_DEFAULT_DACL*)TokenInformation;

        formater.Format(TEXT("DACL=%s"), GetAclInfo(formaterAcl, pTokenDefaultDacl->DefaultDacl));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenSourceInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        TOKEN_SOURCE* pTokenSource = (TOKEN_SOURCE*)TokenInformation;
        CFStringFormater formaterLuid;
        COM_VERIFY(formater.Format(TEXT("SourceName=%s, Identifier=0x%I64x(%x:%x)"), CFConversion().MBCS_TO_TCHAR(pTokenSource->SourceName), 
            *(LONGLONG*)&pTokenSource->SourceIdentifier, pTokenSource->SourceIdentifier.HighPart, pTokenSource->SourceIdentifier.LowPart));
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;

        TOKEN_TYPE* pTokenType = (TOKEN_TYPE*)TokenInformation;
        LPCTSTR pszTokenType = NULL;
        if (TokenPrimary == *pTokenType){
            pszTokenType = TEXT("TokenPrimary");
        }
        else if(TokenImpersonation == *pTokenType){
            pszTokenType = TEXT("TokenImpersonation");
        }
        else{
            FTLASSERT(FALSE);
        }
        COM_VERIFY(formater.Format(TEXT("%d(%s)"), *pTokenType, pszTokenType));
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetSecurityImpersonationLevelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        SECURITY_IMPERSONATION_LEVEL* pSecurityImpersonationLevel = (SECURITY_IMPERSONATION_LEVEL*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenStatisticsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        //一些统计信息?
        HRESULT hr = E_FAIL;
        CFStringFormater formaterTokenType;
        TOKEN_STATISTICS* pTokenStatistics = (TOKEN_STATISTICS*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("TokenId=0x%I64x(0x%x:0x%x), TokenType=%s, DynamicCharged=%d, DynamicAvailable=%d, GroupCount=%d, PrivilegeCount=%d"), 
            *(LONGLONG*)&pTokenStatistics->TokenId, pTokenStatistics->TokenId.HighPart, pTokenStatistics->TokenId.LowPart,
            GetTokenTypeInfo(formaterTokenType, &pTokenStatistics->TokenType, sizeof(pTokenStatistics->TokenType)),
            pTokenStatistics->DynamicCharged, pTokenStatistics->DynamicAvailable, pTokenStatistics->GroupCount, pTokenStatistics->PrivilegeCount));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenGroupsAndPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        //TODO: 一些统计信息, 对应信息可通过其他 Token class(如 TokenPrivileges) 获取到?
        HRESULT hr = E_FAIL;
        TOKEN_GROUPS_AND_PRIVILEGES* pTokenGroupsAndPrivileges = (TOKEN_GROUPS_AND_PRIVILEGES*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("SidCount=%d, SidLength=%d,")
            TEXT("RestrictedSidCount=%d, RestrictedSidLength=%d,")
            TEXT("PrivilegeCount=%d, PrivilegeLength=%d"), 
            pTokenGroupsAndPrivileges->SidCount, pTokenGroupsAndPrivileges->SidLength, 
            pTokenGroupsAndPrivileges->RestrictedSidCount, pTokenGroupsAndPrivileges->RestrictedSidLength,
            pTokenGroupsAndPrivileges->PrivilegeCount, pTokenGroupsAndPrivileges->PrivilegeLength));
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenOriginInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        TOKEN_ORIGIN* pTokenOrigin = (TOKEN_ORIGIN*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("0x%I64x(0x%x:%x)"), *(LONGLONG*)&pTokenOrigin->OriginatingLogonSession,
            pTokenOrigin->OriginatingLogonSession.HighPart, pTokenOrigin->OriginatingLogonSession.LowPart));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenElevationTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        TOKEN_ELEVATION_TYPE* pTokenElevationType = (TOKEN_ELEVATION_TYPE*)TokenInformation;
        LPCTSTR pszElevationType = TEXT("");

        if (TokenElevationTypeDefault == *pTokenElevationType){
            pszElevationType = TEXT("TokenElevationTypeDefault");
        }else if(TokenElevationTypeFull == *pTokenElevationType){
            pszElevationType = TEXT("TokenElevationTypeFull");
        }else if(TokenElevationTypeLimited == *pTokenElevationType){
            pszElevationType = TEXT("TokenElevationTypeLimited");
        }
        else{
            FTLASSERT(FALSE);
        }
        COM_VERIFY(formater.Format(TEXT("%s"), pszElevationType));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenLinkedTokenInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        TOKEN_LINKED_TOKEN* pTokenLinkedToken = (TOKEN_LINKED_TOKEN*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("LinkedToken=0x%x"), pTokenLinkedToken->LinkedToken));
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenElevationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        TOKEN_ELEVATION* pTokenElevation = (TOKEN_ELEVATION*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("IsElevated=%d"), pTokenElevation->TokenIsElevated));
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenAccessInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        TOKEN_ACCESS_INFORMATION* pTokenAccessInformation = (TOKEN_ACCESS_INFORMATION*)TokenInformation;

        COM_VERIFY(formater.Format(TEXT("Flags=0x%x"), pTokenAccessInformation->Flags));
#if 0
        //这个结构似乎只有新版本SDK中才有?
        pTokenAccessInformation->AppContainerNumber
#endif 

        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenMandatoryLabelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        TOKEN_MANDATORY_LABEL* pTokenMandatoryLabel = (TOKEN_MANDATORY_LABEL*)TokenInformation;
        return GetSidAndAttributesInfo(formater, &pTokenMandatoryLabel->Label, TRUE, TEXT("|"));
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenMandatoryPolicyInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        TOKEN_MANDATORY_POLICY* pTokenMandatoryPolicy = (TOKEN_MANDATORY_POLICY*)TokenInformation;
        switch (pTokenMandatoryPolicy->Policy)
        {
            HANDLE_CASE_TO_STRING_FORMATER(formater, TOKEN_MANDATORY_POLICY_OFF);
            HANDLE_CASE_TO_STRING_FORMATER(formater, TOKEN_MANDATORY_POLICY_NO_WRITE_UP);
            HANDLE_CASE_TO_STRING_FORMATER(formater, TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN);
            HANDLE_CASE_TO_STRING_FORMATER(formater, (TOKEN_MANDATORY_POLICY_NO_WRITE_UP | TOKEN_MANDATORY_POLICY_NEW_PROCESS_MIN));
        default:
            FTLASSERT(FALSE);
            formater.Format(TEXT("<Unknown:0x%x>"), pTokenMandatoryPolicy->Policy);
            break;
        }
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenAppcontainerInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        TOKEN_APPCONTAINER_INFORMATION* pTokenAppcontainerInformation = (TOKEN_APPCONTAINER_INFORMATION*)TokenInformation;
        return GetSidInfo(formater, pTokenAppcontainerInformation->TokenAppContainer, FALSE);
    }
    LPCTSTR WINAPI CFUserUtil::GetClaimSecurityAttributesInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD /* TokenInformationLength */)
    {
        HRESULT hr = E_FAIL;
        CLAIM_SECURITY_ATTRIBUTES_INFORMATION* pClaimSecurityAttributesInformation = (CLAIM_SECURITY_ATTRIBUTES_INFORMATION*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("version=%d, reserved=%d, attrCount=%d"), 
            pClaimSecurityAttributesInformation->Version, pClaimSecurityAttributesInformation->Reserved, pClaimSecurityAttributesInformation->AttributeCount)); 
        return formater.GetString();
    }

    BOOL CFUserUtil::DumpTokenInformation(HANDLE hToken)
    {
        //获取 Token 中所有的信息
        BOOL bRet = FALSE;
        FTLASSERT(hToken != NULL);

        static TokenInfoDumpParams dumpTokenParams[] = {
            //{ TokenNone, _T("TokenNone"), GetTokenDwordInfo },                        //0 -- 测试一下是否能获取到 0 值对应的信息, 实测获取不到
            { TokenUser, _T("TokenUser"), GetTokenUserInfo },                           //1 -- TOKEN_USER 
            { TokenGroups, _T("TokenGroups"), GetTokenGroupsInfo },                     //2 -- TOKEN_GROUPS

            //获得权限列表, 如 SeDebugPrivilege 等
            { TokenPrivileges, _T("TokenPrivileges"), GetTokenPrivilegesInfo },         //3 -- TOKEN_PRIVILEGES
            { TokenOwner, _T("TokenOwner"), GetTokenOwnerInfo },                        //4 -- TOKEN_OWNER
            { TokenPrimaryGroup, _T("TokenPrimaryGroup"), GetTokenPrimaryGroupInfo },   //5 -- TOKEN_PRIMARY_GROUP
            { TokenDefaultDacl, _T("TokenDefaultDacl"), GetTokenDefaultDaclInfo },      //6 -- TOKEN_DEFAULT_DACL
            { TokenSource, _T("TokenSource"), GetTokenSourceInfo },                     //7 -- TOKEN_SOURCE
            { TokenType, _T("TokenType"), GetTokenTypeInfo },                           //8 -- TOKEN_TYPE
            
            //只有 TokenType 为 TokenImpersonation 的Token才能获得该信息
            { TokenImpersonationLevel, _T("TokenImpersonationLevel"), GetSecurityImpersonationLevelInfo, ERROR_INVALID_PARAMETER },  //9 -- SECURITY_IMPERSONATION_LEVEL

            { TokenStatistics, _T("TokenStatistics"), GetTokenStatisticsInfo },         //10 -- TOKEN_STATISTICS
            { TokenRestrictedSids, _T("TokenRestrictedSids"), GetTokenGroupsInfo },     //11 -- TOKEN_GROUPS

            //Session Id 值: 比如 0(服务进程), 1~n(普通的用户进程) -- 注意: Vista 以后服务始终在 Session 0
            { TokenSessionId, _T("TokenSessionId"), GetTokenDwordInfo },                //12 -- DWORD
            { TokenGroupsAndPrivileges, _T("TokenGroupsAndPrivileges"), GetTokenGroupsAndPrivilegesInfo },  //13 -- TOKEN_GROUPS_AND_PRIVILEGES
            { TokenSessionReference, _T("TokenSessionReference"), GetTokenReservedInfo , ERROR_INVALID_PARAMETER },                   //14 -- Reserved
            { TokenSandBoxInert, _T("TokenSandBoxInert"), GetTokenDwordInfo },          //15 -- DWORD
            { TokenAuditPolicy, _T("TokenAuditPolicy"), GetTokenReservedInfo },         //16 -- Reserved, TODO: Win7下实测有值

            //根据其值是否为0,可以区分出登录类型?
            { TokenOrigin, _T("TokenOrigin"), GetTokenOriginInfo },                     //17 -- TOKEN_ORIGIN
            { TokenElevationType, _T("TokenElevationType"), GetTokenElevationTypeInfo },//18 -- TOKEN_ELEVATION_TYPE
            { TokenLinkedToken, _T("TokenLinkedToken"), GetTokenLinkedTokenInfo },      //19 -- TOKEN_LINKED_TOKEN
            { TokenElevation, _T("TokenElevation"), GetTokenElevationInfo },            //20 -- TOKEN_ELEVATION
            { TokenHasRestrictions, _T("TokenHasRestrictions"), GetTokenDwordInfo },    //21 -- DWORD
            { TokenAccessInformation, _T("TokenAccessInformation"), GetTokenAccessInformationInfo },        //22 -- TOKEN_ACCESS_INFORMATION
            { TokenVirtualizationAllowed, _T("TokenVirtualizationAllowed"), GetTokenDwordInfo },            //23 -- DWORD
            { TokenVirtualizationEnabled, _T("TokenVirtualizationEnabled"), GetTokenDwordInfo },            //24 -- DWORD
            { TokenIntegrityLevel, _T("TokenIntegrityLevel"), GetTokenMandatoryLabelInfo },                 //25 -- TOKEN_MANDATORY_LABEL
            { TokenUIAccess, _T("TokenUIAccess"), GetTokenDwordInfo },                  //26 -- DWORD
            { TokenMandatoryPolicy, _T("TokenMandatoryPolicy"), GetTokenMandatoryPolicyInfo },              //27 -- TOKEN_MANDATORY_POLICY
            { TokenLogonSid, _T("TokenLogonSid"), GetTokenGroupsInfo },                 //28 -- TOKEN_GROUPS
#ifdef GET_ALL_TOKEN_INFOMATION
            //以下信息, 如果在Win7下运行, 会返回 ERROR_INVALID_PARAMETER(87)

            //表明其完整性级别(Integrity Level) 是否是 AppContainer(如Win8下IE的增强保护模式)
            { TokenIsAppContainer, _T("TokenIsAppContainer"), GetTokenDwordInfo, ERROR_INVALID_PARAMETER},      //29 -- DWORD
            { TokenCapabilities, _T("TokenCapabilities"), GetTokenGroupsInfo,ERROR_INVALID_PARAMETER },         //30 -- TOKEN_GROUPS
            { TokenAppContainerSid, _T("TokenAppContainerSid"), GetTokenAppcontainerInformationInfo, ERROR_INVALID_PARAMETER },      //31 -- TOKEN_APPCONTAINER_INFORMATION
            { TokenAppContainerNumber, _T("TokenAppContainerNumber"), GetTokenDwordInfo, ERROR_INVALID_PARAMETER },                  //32 -- DWORD
            { TokenUserClaimAttributes, _T("TokenUserClaimAttributes"), GetClaimSecurityAttributesInformationInfo, ERROR_INVALID_PARAMETER },        //33 -- CLAIM_SECURITY_ATTRIBUTES_INFORMATION
            { TokenDeviceClaimAttributes, _T("TokenDeviceClaimAttributes"), GetClaimSecurityAttributesInformationInfo, ERROR_INVALID_PARAMETER },    //34 -- CLAIM_SECURITY_ATTRIBUTES_INFORMATION
            { TokenRestrictedUserClaimAttributes, _T("TokenRestrictedUserClaimAttributes"),  GetTokenReservedInfo, ERROR_INVALID_PARAMETER },        //35 -- Reserved
            { TokenRestrictedDeviceClaimAttributes, _T("TokenRestrictedDeviceClaimAttributes"), GetTokenReservedInfo, ERROR_INVALID_PARAMETER },     //36 -- Reserved
            { TokenDeviceGroups, _T("TokenDeviceGroups"),  GetTokenGroupsInfo, ERROR_INVALID_PARAMETER },        //37 -- TOKEN_GROUPS
            { TokenRestrictedDeviceGroups, _T("TokenRestrictedDeviceGroups"), GetTokenGroupsInfo, ERROR_INVALID_PARAMETER },         //38 -- TOKEN_GROUPS
            { TokenSecurityAttributes, _T("TokenSecurityAttributes"), GetTokenReservedInfo, ERROR_INVALID_PARAMETER },               //39 -- Reserved
            { TokenIsRestricted, _T("TokenIsRestricted"), GetTokenReservedInfo, ERROR_INVALID_PARAMETER },       //40 -- Reserved
            //{ xxxxxx,   _T("xxxxx"),  xxxxxxx },     //x -- xxxxxxxxxxxxxxxxxxxxxx
#endif //GET_ALL_TOKEN_INFOMATION
            { (DWORD)(-1), NULL, NULL }
        };
        DWORD dwCheckCount = _countof(dumpTokenParams);
        for (DWORD dwIndex = 0; dwIndex < dwCheckCount; dwIndex++)
        {
            TokenInfoDumpParams* pDumpParams = &dumpTokenParams[dwIndex];
            if (pDumpParams->pszClassName != NULL)
            {
                CFMemAllocator<BYTE> memAllocator; 
                DWORD dwReturnLength = 0;
                GET_TOKEN_INFORMATION_DYNAMIC(hToken, pDumpParams->tokenInformationClass, memAllocator, dwReturnLength, pDumpParams->dwSkipError);
                if (bRet)
                {
                    CFStringFormater formater(512, 16);
                    FTLTRACE(TEXT("%s(%d): %s\n"), pDumpParams->pszClassName, pDumpParams->tokenInformationClass,
                        pDumpParams->pDumpProc(formater, memAllocator.GetMemory(), dwReturnLength));
                }
            }
        }
        return bRet;
    }

    //////////////////////////////////////////////////////////////////////////
    CFUserPrivileges::CFUserPrivileges()
    {
        //m_hPolicy = NULL;
    }
    CFUserPrivileges::~CFUserPrivileges()
    {
        Close();
    }
    BOOL CFUserPrivileges::Open()
    {
        BOOL bRet = FALSE;
        //LSA_OBJECT_ATTRIBUTES ObjectAttributes = {0};
        //LsaOpenPolicy(NULL, &ObjectAttributes, POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, m_hPolicy);
        return bRet;
    }
    VOID CFUserPrivileges::Close()
    {
        //if (m_hPolicy)
        //{
        //    LsaClose(m_hPolicy);
        //    m_hPolicy = NULL;
        //}
    }

    //void CFUserPrivileges::_InitLsaString(PLSA_UNICODE_STRING LsaString,LPWSTR String)
    //{
    //    DWORD StringLength;
    //    if (String == NULL) {
    //        LsaString->Buffer = NULL;
    //        LsaString->Length = 0;
    //        LsaString->MaximumLength = 0;
    //        return;
    //    }
    //    StringLength = wcslen(String);
    //    LsaString->Buffer = String;
    //    LsaString->Length = (USHORT) StringLength * sizeof(WCHAR);
    //    LsaString->MaximumLength=(USHORT)(StringLength+1) * sizeof(WCHAR);
    //}
}
#endif //FTL_USER_HPP