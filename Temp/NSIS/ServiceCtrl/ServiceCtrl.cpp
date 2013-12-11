// ServiceCtrl.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "nsisapi.h"
#include "winerror.h"
#include <tchar.h>

/*
*/

DWORD GetServiceTypeValue(LPCTSTR ServiceType);
DWORD GetStartTypeValue(LPCTSTR StartType);

extern "C" void __declspec(dllexport) InstallService(
	HWND hwndParent, 
	int string_size, 
	TCHAR *variables, 
	stack_t **stacktop)
{
    TCHAR ServiceName[100] = {0};
    TCHAR DisplayName[100] = {0};
    TCHAR BinaryPath[260] = {0};
    TCHAR ServiceType[100] = {0};
    TCHAR StartType[100] = {0};
    
	TCHAR* reason = 0;
    TCHAR* stopString = NULL;
	BOOL okay = FALSE;
	DWORD error = 0;
    SERVICE_STATUS status = {0};

    DWORD nServiceType = SERVICE_WIN32_SHARE_PROCESS; //SERVICE_WIN32_SHARE_PROCESS;
    DWORD nStartType = SERVICE_DEMAND_START;

	g_hwndParent=hwndParent;
	g_stringsize=string_size;
	g_stacktop=stacktop;
	g_variables=variables;

	if (0 == popstring(ServiceName) 
        && 0 == popstring(DisplayName)
        && 0 == popstring(BinaryPath) 
        )
	{
        //MessageBox(hwndParent, BinaryPath, TEXT("CheckPath"), MB_OK);

        if (0 == popstring(ServiceType))
        {
            nServiceType = GetServiceTypeValue(ServiceType);
        }
        if (0 == popstring(StartType))
        {
            nStartType = GetStartTypeValue(StartType);
        }

        SC_HANDLE hSCM = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
		if (hSCM)
		{
            SC_HANDLE hService = CreateService(hSCM,
                   ServiceName,
                   DisplayName,
                   SERVICE_ALL_ACCESS, //SERVICE_START or SERVICE_QUERY_STATUS or _DELETE,
                   nServiceType,
                   nStartType,
                   SERVICE_ERROR_NORMAL,
                   BinaryPath,
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   NULL);

			if (hService)
			{
                if (nStartType == SERVICE_AUTO_START)
                {
                    okay = StartService(hService, 0, 0);
                    if (!okay)
                    {
                        error = GetLastError();
                    }
                }
                CloseServiceHandle(hService);
			}
			else
            {
				error = GetLastError();
            }
			CloseServiceHandle(hSCM);
		}
		else
        {
			error = GetLastError();
        }
	}
	else
    {
        SetLastError(ERROR_INVALID_PARAMETER);
		//reason = TEXT("Bad parameters");
    }

	if (FALSE == okay)
	{
		if (!reason)
		{
			LPVOID lpMsgBuf = NULL;
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


DWORD GetServiceTypeValue(LPCTSTR ServiceType)
{
    DWORD nServiceType = 0;
    //= _tcstol(ServiceType, &stopString, 10);

    if (lstrcmpi(TEXT("SERVICE_FILE_SYSTEM_DRIVER"), ServiceType) == 0)
    {
        nServiceType = SERVICE_FILE_SYSTEM_DRIVER;
    }
    else if(lstrcmpi(TEXT("SERVICE_KERNEL_DRIVER"), ServiceType) == 0)
    {
        nServiceType = SERVICE_KERNEL_DRIVER;
    }
    else if(lstrcmpi(TEXT("SERVICE_WIN32_OWN_PROCESS"), ServiceType) == 0)
    {
        nServiceType = SERVICE_WIN32_OWN_PROCESS;
    }
    else if(lstrcmpi(TEXT("SERVICE_WIN32_SHARE_PROCESS"), ServiceType) == 0)
    {
        nServiceType = SERVICE_WIN32_SHARE_PROCESS;
    }
    
    return nServiceType;
}

DWORD GetStartTypeValue(LPCTSTR StartType)
{
    DWORD nStartType = 0;
    if (lstrcmpi(TEXT("SERVICE_AUTO_START"), StartType) == 0)
    {
        nStartType = SERVICE_AUTO_START;
    }
    else if (lstrcmpi(TEXT("SERVICE_BOOT_START"), StartType) == 0)
    {
        nStartType = SERVICE_BOOT_START;
    }
    else if (lstrcmpi(TEXT("SERVICE_DEMAND_START"), StartType) == 0)
    {
        nStartType = SERVICE_DEMAND_START;
    }
    else if (lstrcmpi(TEXT("SERVICE_DISABLED"), StartType) == 0)
    {
        nStartType = SERVICE_DISABLED;
    }
    else if (lstrcmpi(TEXT("SERVICE_SYSTEM_START"), StartType) == 0)
    {
        nStartType = SERVICE_SYSTEM_START;
    }

    return nStartType;
}


extern "C" void __declspec(dllexport) RemoveService(
    HWND hwndParent, 
    int string_size, 
    TCHAR *variables, 
    stack_t **stacktop)
{
    TCHAR ServiceName[100] = {0};

    TCHAR* reason = 0;
    TCHAR* stopString = NULL;
    BOOL okay = FALSE;
    DWORD error = 0;

    g_hwndParent=hwndParent;
    g_stringsize=string_size;
    g_stacktop=stacktop;
    g_variables=variables;

    //MessageBox(hwndParent, TEXT("Enter Remove Service"), TEXT("RemoveService"), MB_OK);

    if (0 == popstring(ServiceName))
    {
        //MessageBox(hwndParent, ServiceName, TEXT("RemoveService"), MB_OK);

        SC_HANDLE hSCM = OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
        if (hSCM)
        {
            SC_HANDLE hService = OpenService(hSCM, ServiceName, SERVICE_ALL_ACCESS);
            if (hService)
            {
                SC_LOCK hLock = LockServiceDatabase(hSCM);
                if(hLock)
                {
                    okay = DeleteService(hService);
                    if (!okay)
                    {
                        error = GetLastError();
                    }
                    UnlockServiceDatabase(hLock);
                }
                else
                {
                    error = GetLastError();
                }
                CloseServiceHandle(hService);
            }
            else
            {
                error = GetLastError();
            }
            CloseServiceHandle(hSCM);
        }
        else
        {
            error = GetLastError();
        }
    }
    else
    {
        SetLastError(ERROR_INVALID_PARAMETER);
    }

    if (FALSE == okay)
    {
        if (!reason)
        {
            LPVOID lpMsgBuf = NULL;
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
        {
            pushstring(reason);
        }
    }
    else
    {
        pushstring(TEXT("Ok"));
    }

}