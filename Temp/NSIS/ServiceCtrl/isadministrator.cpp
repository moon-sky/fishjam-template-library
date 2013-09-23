

/*
  DESC: This file implements two functions, one which queries Windows NT
  DESC: security configuration information, and the other which is designed
  DESC: to be incorporated into a Nullsoft NSIS installer extension DLL. This
  DESC: then allows any author of NSIS Installer scripts to call the method
  DESC: in this DLL and so extend the set of methods available to them when
  DESC: creating their product installation script.
*/

#include "stdafx.h"
#include "nsisapi.h"
#include "winerror.h"
#include <malloc.h>
//#include <stdio.h>


/*
  DESC: Taken from an article entitled "Windows NT Security" in the Technical 
  DESC: Articles section of the April 2000 MSDN Library. The article was
  DESC: signed Christopher Nefcy, September 1994, to him I would like to give
  DESC: a very big thanks as it would have taken me ages to work out how to
  DESC: do what the following code, taken from his article, does.
  MODS: XGE 06/06/02 19:18 Created, stylistic changes but no material changes.
*/


/*
  --- NSIS SCRIPTABLE FUNCTION ---
  DESC: Find out if the user (the owner of the current process) is a member of
  DESC: the administrators group on the local computer (not on the domain!).
  ARGS: Push the service name onto the stack.
  RETN: Pop the result string from the stack.
  RETN: The result will be 'Yes' or 'No' if there were no problems, otherwise
  RETN: the reason for the failure will be in the result string.
  USGE: Use it like this :-
  USGE: 
  USGE:   CallInstDLL x18Services.dll IsProcessUserAdministrator
  USGE:   Pop $0
  USGE:   ; $0 now contains either 'Yes', 'No' or an error description
*/
extern "C" void __declspec(dllexport) IsProcessUserAdministrator(
	HWND      hwndParent, 
	int       string_size, 
	TCHAR*     variables, 
	stack_t** stacktop)
{
	BOOL                     fAdmin;
	HANDLE                   hThread;
	TOKEN_GROUPS*            ptg = NULL;
	DWORD                    cbTokenGroups;
	DWORD                    dwGroup;
	PSID                     psidAdmin;
    SID_IDENTIFIER_AUTHORITY SystemSidAuthority= SECURITY_NT_AUTHORITY;

	g_hwndParent = hwndParent;
	g_stringsize = string_size;
	g_stacktop   = stacktop;
	g_variables  = variables;

	// First we must open a handle to the access token for this thread.
	if (!OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,FALSE,&hThread))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
		{
			// If the thread does not have an access token, we'll examine the
			// access token associated with the process.
			if (!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hThread))
			{
				PushLastError();
				return;
			}
		}
		else
		{
			PushLastError();
			return;
		}
	}


	// Then we must query the size of the group information associated with
	// the token. Note that we expect a FALSE result from GetTokenInformation
	// because we've given it a NULL buffer. On exit cbTokenGroups will tell
	// the size of the group information.
	if (GetTokenInformation(hThread,TokenGroups,NULL,0,&cbTokenGroups))
	{
		PushLastError();
		return;
	}

	// Here we verify that GetTokenInformation failed for lack of a large
	// enough buffer.
	if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
	{
		PushLastError();
		return;
	}

	// Now we allocate a buffer for the group information.
	// Since _alloca allocates on the stack, we don't have
	// to explicitly deallocate it. That happens automatically
	// when we exit this function.
	__try
	{
		if (!(ptg = (TOKEN_GROUPS*)_alloca(cbTokenGroups))) 
		{
			pushstring(TEXT("Memory allocation failure"));
			return;
		}
	}
	__except (pushstring(TEXT("Memory allocation exception")),1)
	{
	}

	// Now we ask for the group information again.
	// This may fail if an administrator has added this account
	// to an additional group between our first call to
	// GetTokenInformation and this one.
	if (!GetTokenInformation(
		hThread,
		TokenGroups,
		ptg,
		cbTokenGroups,
		&cbTokenGroups))
	{
		PushLastError();
		return;
	}

	// Now we must create a System Identifier for the Admin group.
	if (!AllocateAndInitializeSid(
		&SystemSidAuthority,
		2, 
		SECURITY_BUILTIN_DOMAIN_RID, 
		DOMAIN_ALIAS_RID_ADMINS,
		0,0,0,0,0,0,
		&psidAdmin))
	{
		PushLastError();
		return;
	}

	// Finally we'll iterate through the list of groups for this access
	// token looking for a match against the SID we created above.
	fAdmin = FALSE;

	for (dwGroup = 0; dwGroup < ptg->GroupCount; dwGroup++)
	{
		if (EqualSid(ptg->Groups[dwGroup].Sid,psidAdmin))
		{
			fAdmin = TRUE;
			break;
		}
	}

	// Before we exit we must explicity deallocate the SID we created.
	FreeSid(psidAdmin);

	if (TRUE == fAdmin)
		pushstring(TEXT("Yes"));
	else
		pushstring(TEXT("No"));
}
