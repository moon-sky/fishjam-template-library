#include "stdafx.h"

/*
  DESC: The method implemented in this file was taken from a document found in
  DESC: the April 2000 MSDN Library, which has been included in full. I am
  DESC: extremely grateful to the author(s) of the document as it has saved me
  DESC: an unimaginable age of painstaking work with the beast that is the 
  DESC: Windows NT Security API.
  MODS: XGE 06/06/02 19:35 Created, stylistic improvements over the original
  MODS:                    but no material changes to the logic. Removed the
  MODS:                    main() function and made other changes necessary to
  MODS:                    make the method a standalone tool method.
*/


/*
HOWTO: Manage User Privileges Programmatically in Windows NT
ID: Q132958 

------------------------------------------------------------------------------
The information in this article applies to:

Microsoft Win32 Application Programming Interface (API), used with:
Microsoft Windows NT versions 3.51, 4.0 
Microsoft Windows 2000
------------------------------------------------------------------------------



SUMMARY
-------

In Windows NT, privileges are used to provide a means of access control that 
differs from discretionary access control. A system manager uses privileges to
control which users/groups are able to manipulate various aspects of the 
system. An application may use privileges when it changes a system resource, 
such as the system time, or when it shuts down the system. 

The User Manager tool can be used to grant and revoke privileges from accounts. 

Windows NT 3.51 provides functionality which allows the developer to manage 
privileges programmatically. This functionality is made available through LSA 
(Local Security Authority). 

An example of an application that would benefit from LSA is a service install 
program. If the service is configured to run under a user account, it is 
necessary for that account to have the SeServiceLogonRight "logon as a 
service" privilege. This article discusses how to take advantage of LSA to 
grant and revoke privileges from users and groups. 



MORE INFORMATION
----------------

Managing user privileges can be achieved programmatically using the following 
steps: 

  1. Open the policy on the target machine with LsaOpenPolicy(). To grant 
     privileges, open the policy with POLICY_CREATE_ACCOUNT and 
	 POLICY_LOOKUP_NAMES access. To revoke privileges, open the policy with 
	 POLICY_LOOKUP_NAMES access. 

  2. Obtain a SID (security identifier) representing the user/group of 
     interest. The LookupAccountName() and LsaLookupNames() APIs can obtain a 
	 SID from an account name. 

  3. Call LsaAddAccountRights() to grant privileges to the user(s) represented 
     by the supplied SID. 

  4. Call LsaRemoveAccountRights() to revoke privileges from the user(s) 
     represented by the supplied SID. 

  5. Close the policy with LsaClose(). 

To successfully grant and revoke privileges, the caller needs to be an 
administrator on the target system. 

The LSA API LsaEnumerateAccountRights() can be used to determine which 
privileges have been granted to an account. 

The LSA API LsaEnumerateAccountsWithUserRight() can be used to determine which 
accounts have been granted a specified privilege. 

Documentation and header files for these LSA APIs is provided in the Windows 
32 SDK in the MSTOOLS\SECURITY directory. 

In the lastest versions of the Win32 SDK, the headers appear in the 
mstools\samples\win32\winnt\security\include directory and the documentation 
is in ....\security\lsasamp\lsaapi.hlp. 

NOTE: These LSA APIs are currently implemented as Unicode only. 

This sample will grant the privilege SeServiceLogonRight to the account 
specified on argv[1]. 

This sample is dependant on these import libs 

    advapi32.lib (for LsaXxx)
    user32.lib (for wsprintf) 

This sample will work properly compiled ANSI or Unicode. 


  
SAMPLE CODE
-----------

++
You can use domain\account as argv[1]. For instance, mydomain\scott will
grant the privilege to the mydomain domain account scott.

The optional target machine is specified as argv[2], otherwise, the
account database is updated on the local machine.

The LSA APIs used by this sample are Unicode only.

Use LsaRemoveAccountRights() to remove account rights.

Scott Field (sfield)    12-Jul-95
-- 
*/


#ifndef UNICODE
#define UNICODE
#endif // UNICODE


#include "nsisapi.h"
#include "winerror.h"
//#include <stdio.h>
#include "ntsecapi.h"


NTSTATUS
OpenPolicy(
    LPWSTR ServerName,          // machine to open policy on (Unicode)
    DWORD DesiredAccess,        // desired access to policy
    PLSA_HANDLE PolicyHandle    // resultant policy handle
    );

BOOL
GetAccountSid(
    LPTSTR SystemName,          // where to lookup account
    LPTSTR AccountName,         // account of interest
    PSID *Sid                   // resultant buffer containing SID
    );

NTSTATUS
SetPrivilegeOnAccount(
    LSA_HANDLE PolicyHandle,    // open policy handle
    PSID AccountSid,            // SID to grant privilege to
    LPWSTR PrivilegeName,       // privilege to grant (Unicode)
    BOOL bEnable                // enable or disable
    );

void
InitLsaString(
    PLSA_UNICODE_STRING LsaString, // destination
    LPWSTR String                  // source (Unicode)
    );

/*void
DisplayNtStatus(
    LPSTR szAPI,                // pointer to function name (ANSI)
    NTSTATUS Status             // NTSTATUS error value
    );

void
DisplayWinError(
    LPSTR szAPI,                // pointer to function name (ANSI)
    DWORD WinError              // DWORD WinError
    );
*/


#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13


// 
// If you have the ddk, include ntstatus.h.
// 
#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#endif


/*int _cdecl
main(int argc, TCHAR *argv[])
{
    LSA_HANDLE PolicyHandle;
    WCHAR wComputerName[256]=L"";   // static machine name buffer
    TCHAR AccountName[256];         // static account name buffer
    PSID pSid;
    NTSTATUS Status;
    int iRetVal=RTN_ERROR;          // assume error from main

    if(argc == 1)
    {
        fprintf(stderr,"Usage: %s <Account> [TargetMachine]\n",
            argv[0]);
        return RTN_USAGE;
    }

    // 
    // Pick up account name on argv[1].
    // Assumes source is ANSI. Resultant string is ANSI or Unicode
    // 
    wsprintf(AccountName, TEXT("%hS"), argv[1]);

    // 
    // Pick up machine name on argv[2], if appropriate
    // assumes source is ANSI. Resultant string is Unicode.
    // 
    if(argc == 3) wsprintfW(wComputerName, L"%hS", argv[2]);

    // 
    // Open the policy on the target machine.
    // 
    if((Status=OpenPolicy(
                wComputerName,      // target machine
                POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES,
                &PolicyHandle       // resultant policy handle
                )) != STATUS_SUCCESS) {
        DisplayNtStatus("OpenPolicy", Status);
        return RTN_ERROR;
    }

    // 
    // Obtain the SID of the user/group.
    // Note that we could target a specific machine, but we don't.
    // Specifying NULL for target machine searches for the SID in the
    // following order: well-known, Built-in and local, primary domain,
    // trusted domains.
    // 
    if(GetAccountSid(
            NULL,       // default lookup logic
            AccountName,// account to obtain SID
            &pSid       // buffer to allocate to contain resultant SID
            )) {
        // 
        // We only grant the privilege if we succeeded in obtaining the
        // SID. We can actually add SIDs which cannot be looked up, but
        // looking up the SID is a good sanity check which is suitable for
        // most cases.

        // 
        // Grant the SeServiceLogonRight to users represented by pSid.
        // 
        if((Status=SetPrivilegeOnAccount(
                    PolicyHandle,           // policy handle
                    pSid,                   // SID to grant privilege
                    L"SeServiceLogonRight", // Unicode privilege
                    TRUE                    // enable the privilege
                    )) == STATUS_SUCCESS)
            iRetVal=RTN_OK;
        else
            DisplayNtStatus("AddUserRightToAccount", Status);
    }
    else {
        // 
        // Error obtaining SID.
        // 
        DisplayWinError("GetAccountSid", GetLastError());
    }

    // 
    // Close the policy handle.
    // 
    LsaClose(PolicyHandle);

    // 
    // Free memory allocated for SID.
    // 
    if(pSid != NULL) HeapFree(GetProcessHeap(), 0, pSid);

    return iRetVal;
}*/

void
InitLsaString(
    PLSA_UNICODE_STRING LsaString,
    LPWSTR String
    )
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

NTSTATUS
OpenPolicy(
    LPWSTR ServerName,
    DWORD DesiredAccess,
    PLSA_HANDLE PolicyHandle
    )
{
    LSA_OBJECT_ATTRIBUTES ObjectAttributes;
    LSA_UNICODE_STRING ServerString;
    PLSA_UNICODE_STRING Server = NULL;

    // 
    // Always initialize the object attributes to all zeroes.
    // 
    ZeroMemory(&ObjectAttributes, sizeof(ObjectAttributes));

    if (ServerName != NULL) {
        // 
        // Make a LSA_UNICODE_STRING out of the LPWSTR passed in
        // 
        InitLsaString(&ServerString, ServerName);
        Server = &ServerString;
    }

    // 
    // Attempt to open the policy.
    // 
    return LsaOpenPolicy(
                Server,
                &ObjectAttributes,
                DesiredAccess,
                PolicyHandle
                );
}

/*++
This function attempts to obtain a SID representing the supplied
account on the supplied system.

If the function succeeds, the return value is TRUE. A buffer is
allocated which contains the SID representing the supplied account.
This buffer should be freed when it is no longer needed by calling
HeapFree(GetProcessHeap(), 0, buffer)

If the function fails, the return value is FALSE. Call GetLastError()
to obtain extended error information.

Scott Field (sfield)    12-Jul-95
--*/ 

BOOL
GetAccountSid(
    LPTSTR SystemName,
    LPTSTR AccountName,
    PSID *Sid
    )
{
    LPTSTR ReferencedDomain=NULL;
    DWORD cbSid=128;    // initial allocation attempt
    DWORD cchReferencedDomain=16; // initial allocation size
    SID_NAME_USE peUse;
    BOOL bSuccess=FALSE; // assume this function will fail

    __try {

    // 
    // initial memory allocations
    // 
    if((*Sid=HeapAlloc(
                    GetProcessHeap(),
                    0,
                    cbSid
                    )) == NULL) __leave;

    if((ReferencedDomain=(LPTSTR)HeapAlloc(
                    GetProcessHeap(),
                    0,
                    cchReferencedDomain * sizeof(TCHAR)
                    )) == NULL) __leave;

    // 
    // Obtain the SID of the specified account on the specified system.
    // 
    while(!LookupAccountName(
                    SystemName,         // machine to lookup account on
                    AccountName,        // account to lookup
                    *Sid,               // SID of interest
                    &cbSid,             // size of SID
                    ReferencedDomain,   // domain account was found on
                    &cchReferencedDomain,
                    &peUse
                    )) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            // 
            // reallocate memory
            // 
            if((*Sid=HeapReAlloc(
                        GetProcessHeap(),
                        0,
                        *Sid,
                        cbSid
                        )) == NULL) __leave;

            if((ReferencedDomain=(LPTSTR)HeapReAlloc(
                        GetProcessHeap(),
                        0,
                        ReferencedDomain,
                        cchReferencedDomain * sizeof(TCHAR)
                        )) == NULL) __leave;
        }
        else __leave;
    }

    // 
    // Indicate success.
    // 
    bSuccess=TRUE;

    } // finally
    __finally {

    // 
    // Cleanup and indicate failure, if appropriate.
    // 

    HeapFree(GetProcessHeap(), 0, ReferencedDomain);

    if(!bSuccess) {
        if(*Sid != NULL) {
            HeapFree(GetProcessHeap(), 0, *Sid);
            *Sid = NULL;
        }
    }

    } // finally

    return bSuccess;
}

NTSTATUS
SetPrivilegeOnAccount(
    LSA_HANDLE PolicyHandle,    // open policy handle
    PSID AccountSid,            // SID to grant privilege to
    LPWSTR PrivilegeName,       // privilege to grant (Unicode)
    BOOL bEnable                // enable or disable
    )
{
    LSA_UNICODE_STRING PrivilegeString;

    // 
    // Create a LSA_UNICODE_STRING for the privilege name.
    // 
    InitLsaString(&PrivilegeString, PrivilegeName);

    // 
    // grant or revoke the privilege, accordingly
    // 
    if(bEnable) {
        return LsaAddAccountRights(
                PolicyHandle,       // open policy handle
                AccountSid,         // target SID
                &PrivilegeString,   // privileges
                1                   // privilege count
                );
    }
    else {
        return LsaRemoveAccountRights(
                PolicyHandle,       // open policy handle
                AccountSid,         // target SID
                FALSE,              // do not disable all rights
                &PrivilegeString,   // privileges
                1                   // privilege count
                );
    }
}

/*void
DisplayNtStatus(
    LPSTR szAPI,
    NTSTATUS Status
    )
{
    // 
    // Convert the NTSTATUS to Winerror. Then call DisplayWinError().
    // 
    DisplayWinError(szAPI, LsaNtStatusToWinError(Status));
}

void
DisplayWinError(
    LPSTR szAPI,
    DWORD WinError
    )
{
    LPSTR MessageBuffer;
    DWORD dwBufferLength;

    // 
    // TODO: Get this fprintf out of here!
    // 
    fprintf(stderr,"%s error!\n", szAPI);

    if(dwBufferLength=FormatMessageA(
                        FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL,
                        WinError,
                        GetUserDefaultLangID(),
                        (LPSTR) &MessageBuffer,
                        0,
                        NULL
                        ))
    {
        DWORD dwBytesWritten; // unused

        // 
        // Output message string on stderr.
        // 
        WriteFile(
            GetStdHandle(STD_ERROR_HANDLE),
            MessageBuffer,
            dwBufferLength,
            &dwBytesWritten,
            NULL
            );

        // 
        // Free the buffer allocated by the system.
        // 
        LocalFree(MessageBuffer);
    }
} */


#include "nsisapi.h"


/*
  DESC:
  MODS:
*/
void ToolFunc_SetLogonAsAService(
	HWND      hwndParent, 
	int       string_size, 
	TCHAR*     variables, 
	stack_t** stacktop,
	BOOL      enable)
{
    LSA_HANDLE PolicyHandle;
    PSID       pSid;
    NTSTATUS   Status;
	BOOL       failed = TRUE;
	//TCHAR       asciiAccountName[256];
	WCHAR      AccountName[256];

	popstring(AccountName);//asciiAccountName);
	//if (!MultiByteToWideChar(CP_ACP,0,asciiAccountName,-1,AccountName,255))
	//{
	//	PushLastError();
	//	return;
	//}

    // 
    // Open the policy on the target machine.
    // 
    if ((Status = OpenPolicy(
            0,
            POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES,
            &PolicyHandle)) != STATUS_SUCCESS)			// resultant policy
	{
        PushWinError(LsaNtStatusToWinError(Status));
        return;
    }

    // 
    // Obtain the SID of the user/group.
    // Note that we could target a specific machine, but we don't.
    // Specifying NULL for target machine searches for the SID in the
    // following order: well-known, Built-in and local, primary domain,
    // trusted domains.
    // 
    if (GetAccountSid(
        NULL,       // default lookup logic
        AccountName,// account to obtain SID
        &pSid))     // buffer to allocate to contain resultant SID
	{
        // 
        // We only grant the privilege if we succeeded in obtaining the
        // SID. We can actually add SIDs which cannot be looked up, but
        // looking up the SID is a good sanity check which is suitable for
        // most cases.

        // 
        // Grant the SeServiceLogonRight to users represented by pSid.
        // 
        if ((Status = SetPrivilegeOnAccount(
            PolicyHandle,				// policy handle
            pSid,						// SID to grant privilege
            L"SeServiceLogonRight",		// Unicode privilege
            enable)) == STATUS_SUCCESS)	// enable the privilege
		{
			failed = FALSE;
		}
        else
            PushWinError(LsaNtStatusToWinError(Status));
    }
    else
	{
        // 
        // Error obtaining SID.
        // 
        PushLastError();
    }

    // 
    // Close the policy handle.
    // 
    LsaClose(PolicyHandle);

    // 
    // Free memory allocated for SID.
    // 
    if(pSid != NULL)
		HeapFree(GetProcessHeap(),0,pSid);

	if (FALSE == failed)
		pushstring(TEXT("Ok"));
}

/*
  --- NSIS SCRIPTABLE FUNCTION ---
  DESC: Find out if the given user has the LSA (Local Security Authority)
  DESC: policy right 'SeServiceLogonRight'. The username must be domain\username.
  RETN: Pop the result string from the stack. It 
  RETN: The result will be 'Yes' or 'No' if there were no problems, otherwise
  RETN: the reason for the failure will be in the result string.
  USGE: Use it like this :-
  USGE: 
  USGE:   Push '3B2\Ximon_Eighteen'
  USGE:   CallInstDLL x18Services.dll HasLogonAsAService
  USGE:   Pop $0
  USGE:   ; $0 now contains either 'Yes', 'No' or an error description
  MODS: v0.0  10th June 2002 - Created
  MODS: v0.01 11th June 2002 - Bug Fix: Documentation incorrectly stated that this
  MODS:                        method worked on the current user rather than a
  MODS:                        given user.
*/
extern "C" void __declspec(dllexport) HasLogonAsAService(
	HWND      hwndParent, 
	int       string_size, 
	TCHAR*     variables, 
	stack_t** stacktop)
{
    LSA_HANDLE PolicyHandle;
    PSID       pSid;
    NTSTATUS   Status;
	//TCHAR       asciiAccountName[256];
	WCHAR      AccountName[256];

	g_hwndParent=hwndParent;
	g_stringsize=string_size;
	g_stacktop=stacktop;
	g_variables=variables;

	popstring(AccountName);//asciiAccountName);
	//if (!MultiByteToWideChar(CP_ACP,0,asciiAccountName,-1,AccountName,255))
	//{
	//	PushLastError();
	//	return;
	//}

    // 
    // Open the policy on the target machine.
    // 
    if ((Status = OpenPolicy(
            0,
            POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES,
            &PolicyHandle)) != STATUS_SUCCESS)			// resultant policy
	{
        PushWinError(LsaNtStatusToWinError(Status));
        return;
    }

    // 
    // Obtain the SID of the user/group.
    // Note that we could target a specific machine, but we don't.
    // Specifying NULL for target machine searches for the SID in the
    // following order: well-known, Built-in and local, primary domain,
    // trusted domains.
    // 
    if (GetAccountSid(
        NULL,       // default lookup logic
        AccountName,// account to obtain SID
        &pSid))     // buffer to allocate to contain resultant SID
	{
        // 
        // We only grant the privilege if we succeeded in obtaining the
        // SID. We can actually add SIDs which cannot be looked up, but
        // looking up the SID is a good sanity check which is suitable for
        // most cases.

        // 
        // Enumerate the rights on this account
        // 
		ULONG count = 0;
		LSA_UNICODE_STRING* rights;

		if ((Status = LsaEnumerateAccountRights(
			PolicyHandle,
			pSid,
			&rights,
			&count)) == STATUS_SUCCESS)
		{
			BOOL found = FALSE;
			ULONG i;
			//TCHAR ascii[256];

			for (i = 0; found == FALSE && i < count; i++)
			{
				//WideCharToMultiByte(CP_ACP,0,rights[i].Buffer,rights[i].Length,ascii,255,0,0);
				//if (0 == _tcsicmp(ascii, TEXT("SeServiceLogonRight")))
                if (0 == lstrcmpi(rights[i].Buffer, TEXT("SeServiceLogonRight")))
					found = TRUE;
			}

			if (found == FALSE)
				pushstring(TEXT("No"));
			else
				pushstring(TEXT("Yes"));
		}

		LsaFreeMemory(rights);
    }
    else
	{
        // 
        // Error obtaining SID.
        // 
        PushLastError();
    }

    // 
    // Close the policy handle.
    // 
    LsaClose(PolicyHandle);

    // 
    // Free memory allocated for SID.
    // 
    if(pSid != NULL)
		HeapFree(GetProcessHeap(),0,pSid);
}

/*
  --- NSIS SCRIPTABLE FUNCTION ---
  DESC: Grant the current user (the owner of the process) the LSA
  DESC: (Local Security Authority) policy right 'SeServiceLogonRight'.
  RETN: Pop the result string from the stack.
  RETN: The result will be 'Ok' if there were no problems, otherwise
  RETN: the reason for the failure will be in the result string.
  USGE: Use it like this :-
  USGE: 
  USGE:   CallInstDLL x18Services.dll GrantLogonAsAService
  USGE:   Pop $0
  USGE:   ; $0 now contains either 'Yes', 'No' or an error description
*/
extern "C" void __declspec(dllexport) GrantLogonAsAService(
	HWND      hwndParent, 
	int       string_size, 
	TCHAR*     variables, 
	stack_t** stacktop)
{
	g_hwndParent=hwndParent;
	g_stringsize=string_size;
	g_stacktop=stacktop;
	g_variables=variables;

	ToolFunc_SetLogonAsAService(hwndParent,string_size,variables,stacktop,TRUE);
}

/*
  --- NSIS SCRIPTABLE FUNCTION ---
  DESC: Deny the current user (the owner of the process) the LSA
  DESC: (Local Security Authority) policy right 'SeServiceLogonRight'.
  RETN: Pop the result string from the stack.
  RETN: The result will be 'Ok' if there were no problems, otherwise
  RETN: the reason for the failure will be in the result string.
  USGE: Use it like this :-
  USGE: 
  USGE:   CallInstDLL x18Services.dll RemoveLogonAsAService
  USGE:   Pop $0
  USGE:   ; $0 now contains either 'Yes', 'No' or an error description
*/
extern "C" void __declspec(dllexport) RemoveLogonAsAService(
	HWND      hwndParent, 
	int       string_size, 
	TCHAR*     variables, 
	stack_t** stacktop)
{
	g_hwndParent=hwndParent;
	g_stringsize=string_size;
	g_stacktop=stacktop;
	g_variables=variables;

	ToolFunc_SetLogonAsAService(hwndParent,string_size,variables,stacktop,FALSE);
}


#undef UNICODE