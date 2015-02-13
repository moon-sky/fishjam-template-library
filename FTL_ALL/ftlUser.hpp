#ifndef FTL_USER_HPP
#define FTL_USER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlUser.h"
#endif
#include <sddl.h>

namespace FTL
{
#define GET_ALL_TOKEN_INFOMATION

//winnt.h 中没有定义足够的枚举(sdk版本问题) -- 如何根据sdk版本自动检测编译方式?
#ifdef GET_ALL_TOKEN_INFOMATION
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


    LPCTSTR CFUserUtil::GetSidInfo(CFStringFormater& formater, PSID pSid)
    {
        BOOL bRet = FALSE;
        HRESULT hr = E_FAIL;

        FTLASSERT(IsValidSid(pSid));

        LPTSTR pStringSid = NULL;
        API_VERIFY(ConvertSidToStringSid(pSid, &pStringSid));
        if (bRet)
        {
            COM_VERIFY(formater.Format(TEXT("%s"), pStringSid));
            LocalFree(pStringSid); 
        }

        return formater.GetString();
    }

    LPCTSTR CFUserUtil::GetSidAttributesString(CFStringFormater& formater, DWORD dwAttributes, LPCTSTR pszDivide /* = TEXT("|") */)
    {
        BOOL bRet = FALSE;
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
                        TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)tokenBuf.GetMemory();
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

    //h -- hToken
    //c -- TOKEN_INFORMATION_CLASS, such as TokenUser
    //--  t -- TokenInformation Type instance, such as TOKEN_USER
    //a -- CFMemAllocator instance
    //l -- ReturnLength
#define GET_TOKEN_INFORMATION_DYNAMIC(h, c, a, l) \
    API_VERIFY_EXCEPT1(GetTokenInformation(h, (TOKEN_INFORMATION_CLASS)c, NULL, 0, &l), ERROR_INSUFFICIENT_BUFFER); \
    if(!bRet) { \
        if((ERROR_INSUFFICIENT_BUFFER == GetLastError()) && (l > 0)){ \
            API_VERIFY(GetTokenInformation(h, (TOKEN_INFORMATION_CLASS)c, (LPVOID)a.GetMemory(l), l, &l)); \
        }\
    }

    typedef LPCTSTR (WINAPI *PTOKEN_INFO_TYPE_DUMP_PROC)(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength);
    struct TokenInfoDumpParams{
        DWORD tokenInformationClass;
        LPCTSTR pszClassName;
        PTOKEN_INFO_TYPE_DUMP_PROC  pDumpProc;
    };

    LPCTSTR WINAPI CFUserUtil::GetTokenDwordInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        HRESULT hr = E_FAIL;
        DWORD *pDword = (DWORD*)TokenInformation;
        COM_VERIFY(formater.Format(TEXT("=0x%x"), *pDword));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenReservedInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        formater.Format(TEXT("<Reserved>, value=0x%p"), TokenInformation);
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenUserInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        HRESULT hr = E_FAIL;
        CFStringFormater formaterSid, formaterAttributes;

        TOKEN_USER* pTokenUser = (TOKEN_USER*)TokenInformation;
        DWORD dwAttributes = pTokenUser->User.Attributes;
        COM_VERIFY(formater.Format(TEXT("Sid=%s, Attributes=0x%x(%s)"), GetSidInfo(formaterSid, pTokenUser->User.Sid), 
            dwAttributes, GetSidAttributesString(formaterAttributes, dwAttributes)));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenGroupsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_GROUPS* pTokenGroups = (TOKEN_GROUPS*)TokenInformation;
        formater.Format(TEXT("Count=%d"), pTokenGroups->GroupCount);
        for (DWORD dwIndex = 0; dwIndex < pTokenGroups->GroupCount; dwIndex++)
        {
            CFStringFormater formaterSid, formaterAttributes;
            DWORD dwAttributes = pTokenGroups->Groups[dwIndex].Attributes;
            formater.AppendFormat(TEXT("\n  [%d]=%s, Attributes=0x%x(%s)"), dwIndex, GetSidInfo(formaterSid, pTokenGroups->Groups[dwIndex].Sid), 
                dwAttributes, GetSidAttributesString(formaterAttributes, dwAttributes));
        }

        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_PRIVILEGES* pTokenPrivileges = (TOKEN_PRIVILEGES*)TokenInformation;
        formater.Format(TEXT("Count=%d"), pTokenPrivileges->PrivilegeCount);
        for (DWORD dwIndex = 0; dwIndex < pTokenPrivileges->PrivilegeCount; dwIndex++)
        {
            LUID_AND_ATTRIBUTES* pPrivilege = &pTokenPrivileges->Privileges[dwIndex];
            formater.AppendFormat(TEXT("\n  [%d]=, Attributes=0x%x(%s)"), dwIndex, pPrivilege->Attributes, TEXT(""));
        }
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenOwnerInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        CFStringFormater formaterSid;
        TOKEN_OWNER* pTokenOwner = (TOKEN_OWNER*)TokenInformation;
        formater.Format(TEXT("Owner=%s"), GetSidInfo(formaterSid, pTokenOwner->Owner));
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenPrimaryGroupInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_PRIMARY_GROUP* pTokenPrimaryGroup = (TOKEN_PRIMARY_GROUP*)TokenInformation;
        
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenDefaultDaclInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_DEFAULT_DACL* pTokenDefaultDacl = (TOKEN_DEFAULT_DACL*)TokenInformation;
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenSourceInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_SOURCE* pTokenSource = (TOKEN_SOURCE*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_TYPE* pTokenType = (TOKEN_TYPE*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetSecurityImpersonationLevelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        SECURITY_IMPERSONATION_LEVEL* pSecurityImpersonationLevel = (SECURITY_IMPERSONATION_LEVEL*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenStatisticsInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_STATISTICS* pTokenStatistics = (TOKEN_STATISTICS*)TokenInformation;
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenGroupsAndPrivilegesInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_GROUPS_AND_PRIVILEGES* pTokenGroupsAndPrivileges = (TOKEN_GROUPS_AND_PRIVILEGES*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenOriginInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_ORIGIN* pTokenOrigin = (TOKEN_ORIGIN*)TokenInformation;
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenElevationTypeInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_ELEVATION_TYPE* pTokenElevationType = (TOKEN_ELEVATION_TYPE*)TokenInformation;
        return formater.GetString();
    }

    LPCTSTR WINAPI CFUserUtil::GetTokenLinkedTokenInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_LINKED_TOKEN* pTokenLinkedToken = (TOKEN_LINKED_TOKEN*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenElevationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_ELEVATION* pTokenElevation = (TOKEN_ELEVATION*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenAccessInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_ACCESS_INFORMATION* pTokenAccessInformation = (TOKEN_ACCESS_INFORMATION*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenMandatoryLabelInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_MANDATORY_LABEL* pTokenMandatoryLabel = (TOKEN_MANDATORY_LABEL*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenMandatoryPolicyInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_MANDATORY_POLICY* pTokenMandatoryPolicy = (TOKEN_MANDATORY_POLICY*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetTokenAppcontainerInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        TOKEN_APPCONTAINER_INFORMATION* pTokenAppcontainerInformation = (TOKEN_APPCONTAINER_INFORMATION*)TokenInformation;
        return formater.GetString();
    }
    LPCTSTR WINAPI CFUserUtil::GetClaimSecurityAttributesInformationInfo(CFStringFormater& formater, LPVOID TokenInformation, DWORD TokenInformationLength)
    {
        CLAIM_SECURITY_ATTRIBUTES_INFORMATION* pClaimSecurityAttributesInformation = (CLAIM_SECURITY_ATTRIBUTES_INFORMATION*)TokenInformation;
        return formater.GetString();
    }

    BOOL CFUserUtil::DumpTokenInformation(HANDLE hToken)
    {
        //获取 Token 中所有的信息
        BOOL bRet = FALSE;
        FTLASSERT(hToken != NULL);

        static TokenInfoDumpParams dumpTokenParams[] = {
            //{ TokenNone,            _T("TokenNone"),        GetTokenDwordInfo  },     //0 -- 测试一下是否能获取到 0 值对应的信息, 实测获取不到
            { TokenUser,            _T("TokenUser"),        GetTokenUserInfo  },      //1 -- TOKEN_USER 
            { TokenGroups,          _T("TokenGroups"),      GetTokenGroupsInfo },     //2 -- TOKEN_GROUPS
            { TokenPrivileges,      _T("TokenPrivileges"),  GetTokenPrivilegesInfo },     //3 -- TOKEN_PRIVILEGES
            { TokenOwner,           _T("TokenOwner"),       GetTokenOwnerInfo },     //4 -- TOKEN_OWNER
            { TokenPrimaryGroup,   _T("TokenPrimaryGroup"),  GetTokenPrimaryGroupInfo },     //5 -- TOKEN_PRIMARY_GROUP
            { TokenDefaultDacl,   _T("TokenDefaultDacl"),  GetTokenDefaultDaclInfo },     //6 -- TOKEN_DEFAULT_DACL
            { TokenSource,   _T("TokenSource"),  GetTokenSourceInfo },     //7 -- TOKEN_SOURCE
            { TokenType,   _T("TokenType"),  GetTokenTypeInfo },     //8 -- TOKEN_TYPE
            { TokenImpersonationLevel,   _T("TokenImpersonationLevel"),  GetSecurityImpersonationLevelInfo },     //9 -- SECURITY_IMPERSONATION_LEVEL
            { TokenStatistics,   _T("TokenStatistics"),  GetTokenStatisticsInfo },     //10 -- TOKEN_STATISTICS
            { TokenRestrictedSids,   _T("TokenRestrictedSids"),  GetTokenGroupsInfo },     //11 -- TOKEN_GROUPS
            { TokenSessionId,   _T("TokenSessionId"),  GetTokenDwordInfo },     //12 -- DWORD
            { TokenGroupsAndPrivileges,   _T("TokenGroupsAndPrivileges"),  GetTokenGroupsAndPrivilegesInfo },     //13 -- TOKEN_GROUPS_AND_PRIVILEGES
            { TokenSessionReference,   _T("TokenSessionReference"),  GetTokenReservedInfo },     //14 -- Reserved
            { TokenSandBoxInert,   _T("TokenSandBoxInert"),  GetTokenDwordInfo },     //15 -- DWORD
            { TokenAuditPolicy,   _T("TokenAuditPolicy"),  GetTokenReservedInfo },     //16 -- Reserved
            { TokenOrigin,   _T("TokenOrigin"),  GetTokenOriginInfo },     //17 -- TOKEN_ORIGIN
            { TokenElevationType,   _T("TokenElevationType"),  GetTokenElevationTypeInfo },     //18 -- TOKEN_ELEVATION_TYPE
            { TokenLinkedToken,   _T("TokenLinkedToken"),  GetTokenLinkedTokenInfo },     //19 -- TOKEN_LINKED_TOKEN
            { TokenElevation,   _T("TokenElevation"),  GetTokenElevationInfo },     //20 -- TOKEN_ELEVATION
            { TokenHasRestrictions,   _T("TokenHasRestrictions"),  GetTokenDwordInfo },     //21 -- DWORD
            { TokenAccessInformation,   _T("TokenAccessInformation"),  GetTokenAccessInformationInfo },     //22 -- TOKEN_ACCESS_INFORMATION
            { TokenVirtualizationAllowed,   _T("TokenVirtualizationAllowed"),  GetTokenDwordInfo },     //23 -- DWORD
            { TokenVirtualizationEnabled,   _T("TokenVirtualizationEnabled"),  GetTokenDwordInfo },     //24 -- DWORD
            { TokenIntegrityLevel,   _T("TokenIntegrityLevel"),  GetTokenMandatoryLabelInfo },     //25 -- TOKEN_MANDATORY_LABEL
            { TokenUIAccess,   _T("TokenUIAccess"),  GetTokenDwordInfo },     //26 -- DWORD
            { TokenMandatoryPolicy,   _T("TokenMandatoryPolicy"),  GetTokenMandatoryPolicyInfo },     //27 -- TOKEN_MANDATORY_POLICY
            { TokenLogonSid,   _T("TokenLogonSid"),  GetTokenGroupsInfo },     //28 -- TOKEN_GROUPS
#ifdef GET_ALL_TOKEN_INFOMATION
            { TokenIsAppContainer,   _T("TokenIsAppContainer"),  GetTokenDwordInfo },     //29 -- DWORD
            { TokenCapabilities,   _T("TokenCapabilities"),  GetTokenGroupsInfo },     //30 -- TOKEN_GROUPS
            { TokenAppContainerSid,   _T("TokenAppContainerSid"),  GetTokenAppcontainerInformationInfo },     //31 -- TOKEN_APPCONTAINER_INFORMATION
            { TokenAppContainerNumber,   _T("TokenAppContainerNumber"),  GetTokenDwordInfo },     //32 -- DWORD
            { TokenUserClaimAttributes,   _T("TokenUserClaimAttributes"),  GetClaimSecurityAttributesInformationInfo },     //33 -- CLAIM_SECURITY_ATTRIBUTES_INFORMATION
            { TokenDeviceClaimAttributes,   _T("TokenDeviceClaimAttributes"),  GetClaimSecurityAttributesInformationInfo },     //34 -- CLAIM_SECURITY_ATTRIBUTES_INFORMATION
            { TokenRestrictedUserClaimAttributes,   _T("TokenRestrictedUserClaimAttributes"),  GetTokenReservedInfo },     //35 -- Reserved
            { TokenRestrictedDeviceClaimAttributes,   _T("TokenRestrictedDeviceClaimAttributes"),  GetTokenReservedInfo },     //36 -- Reserved
            { TokenDeviceGroups,   _T("TokenDeviceGroups"),  GetTokenGroupsInfo },     //37 -- TOKEN_GROUPS
            { TokenRestrictedDeviceGroups,   _T("TokenRestrictedDeviceGroups"),  GetTokenGroupsInfo },     //38 -- TOKEN_GROUPS
            { TokenSecurityAttributes,   _T("TokenSecurityAttributes"),  GetTokenReservedInfo },     //39 -- Reserved
            { TokenIsRestricted,   _T("TokenIsRestricted"),  GetTokenReservedInfo },     //40 -- Reserved
            //{ xxxxxx,   _T("xxxxx"),  xxxxxxx },     //x -- xxxxxxxxxxxxxxxxxxxxxx
#endif //GET_ALL_TOKEN_INFOMATION
            { -1, NULL, NULL}
        };
        DWORD dwCheckCount = _countof(dumpTokenParams);
        for (DWORD dwIndex = 0; dwIndex < dwCheckCount; dwIndex++)
        {
            TokenInfoDumpParams* pDumpParams = &dumpTokenParams[dwIndex];
            if (pDumpParams->pszClassName != NULL)
            {
                CFMemAllocator<BYTE> memAllocator; 
                DWORD dwReturnLength = 0;
                GET_TOKEN_INFORMATION_DYNAMIC(hToken, pDumpParams->tokenInformationClass, memAllocator, dwReturnLength);
                if (bRet)
                {
                    CFStringFormater formater;
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