#include "stdafx.h"

#include "nsisapi.h"
#include "winerror.h"
#include <tchar.h>

/*
  --- NSIS SCRIPTABLE FUNCTION ---
  DESC: Send a command (Start, Stop, Pause, Continue or Delete) to the Service
  DESC:   start/stop/pause/continue/delete
  DESC:   custom <commandId>
  DESC: Control Manager (SCM) for a given service.
  ARGS: Push service name string onto the stack
  ARGS: Push service command string onto the stack
  RETN: Pop the result string from the stack.
  RETN: The result will be 'Ok' if there were no problems, or the reason for
  RETN: failure otherwise.
  USGE: Use it like this :-
  USGE: 
  USGE:   push 'MyServiceName'
  USGE:   push 'start'
  USGE:   CallInstDLL x18Services.dll SendServiceCommand
  USGE:   Pop $0
  USGE:   StrCmp $0 'Ok' success 0
  USGE:     MessageBox MB_OK|MB_ICONSTOP 'Failed to send service command: Reason: $0' 0 0
  USGE:     Abort
  USGE:   success:
*/
extern "C" void __declspec(dllexport) SendServiceCommand(
	HWND hwndParent, 
	int string_size, 
	TCHAR *variables, 
	stack_t **stacktop)
{
	TCHAR service[100];
	TCHAR command[100];
    TCHAR customCmdId[100];

	TCHAR* reason = 0;
    TCHAR* stopString = NULL;
	BOOL okay = FALSE;
	DWORD error;
    DWORD dwCustomCmdId = 0;
	SERVICE_STATUS status;

	g_hwndParent=hwndParent;
	g_stringsize=string_size;
	g_stacktop=stacktop;
	g_variables=variables;

	if (0 == popstring(command) && 0 == popstring(service))
	{
		SC_HANDLE hSCM = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);

		if (hSCM)
		{
			SC_HANDLE hService = OpenService(hSCM,service,SERVICE_ALL_ACCESS);

			if (hService)
			{
                if (lstrcmpi(TEXT("start"), command) == 0)
					okay = StartService(hService,0,0);
				else if (lstrcmpi(TEXT("stop"),command) == 0)
					okay = ControlService(hService,SERVICE_CONTROL_STOP,&status);
				else if (lstrcmpi(TEXT("pause"),command) == 0)
					okay = ControlService(hService,SERVICE_CONTROL_PAUSE,&status);
				else if (lstrcmpi(TEXT("continue"),command) == 0)
					okay = ControlService(hService,SERVICE_CONTROL_CONTINUE,&status);
				else if (lstrcmpi(TEXT("delete"),command) == 0)
					okay = DeleteService(hService);
                else if (lstrcmpi(TEXT("custom"), command) == 0)
                {
                    if (0 == popstring(customCmdId))
                    {
                        dwCustomCmdId = _tcstol(customCmdId, &stopString, 10);
                        okay = ControlService(hService, dwCustomCmdId, &status);
                    }
                    else
                    {
                        reason = TEXT("Bad custom command id");
                    }
                }
				if (!okay)
					error = GetLastError();

				CloseServiceHandle(hService);
			}
			else
				error = GetLastError();

			CloseServiceHandle(hSCM);
		}
		else
			error = GetLastError();
	}
	else
    {
		reason = TEXT("Bad parameters");
    }

	if (FALSE == okay)
	{
		if (!reason)
		{
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);

			pushstring((TCHAR*)lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
		else
			pushstring(reason);
	}
	else
		pushstring(TEXT("Ok"));
}
