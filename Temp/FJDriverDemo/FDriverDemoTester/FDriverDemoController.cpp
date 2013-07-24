#include "stdafx.h"
#include "FDriverDemoController.h"
#include "../FDriverDemoDefine.h"

CFDriverDemoController::CFDriverDemoController(void)
{
    m_hSCManager = NULL;
    m_hDriverDemoService = NULL;
	m_hDemoDriverFile = NULL;
}

CFDriverDemoController::~CFDriverDemoController(void)
{
	CloseDemoDriver();
	UnInstallService();
}

BOOL CFDriverDemoController::InstallService()
{
    BOOL bRet = TRUE;

    if (NULL == m_hSCManager)
    {
        FTLASSERT(NULL == m_hDriverDemoService);

        TCHAR szDriverFileName[MAX_PATH] = {0};

        GetModuleFileName( NULL, szDriverFileName, MAX_PATH);
        LPTSTR pszFileName = PathFindFileName(szDriverFileName);
        FTLASSERT(pszFileName != NULL);
        if (pszFileName)
        {
            *pszFileName = NULL;
            StringCchCat(szDriverFileName, _countof(szDriverFileName), FDRIVER_DEMO_FILENAME);

            API_VERIFY((m_hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) != NULL);
            if (m_hSCManager)
            {
                API_VERIFY_EXCEPT1((m_hDriverDemoService = ::CreateService(
                    m_hSCManager, 
                    FDRIVER_DEMO_SERVICE_NAME, 
                    TEXT("FDriver Demo Service"),
                    SERVICE_ALL_ACCESS,
                    SERVICE_KERNEL_DRIVER,	//SERVICE_FILE_SYSTEM_DRIVER
                    SERVICE_DEMAND_START,
                    SERVICE_ERROR_NORMAL,
                    szDriverFileName, 
                    NULL, 
                    NULL,
                    NULL, 
                    NULL, 
                    NULL
                    )) != NULL, ERROR_SERVICE_EXISTS);
                if (!m_hDriverDemoService)
                {
                    m_hDriverDemoService = ::OpenService(m_hSCManager, FDRIVER_DEMO_SERVICE_NAME, SERVICE_ALL_ACCESS);
                    API_ASSERT(m_hDriverDemoService != NULL);
                }

                if (m_hDriverDemoService)
                {
                    API_VERIFY(::StartService(m_hDriverDemoService, 0, NULL));
                }
                else
                {
                    //Create Or Open Service Fail, 
                    DWORD dwLastError = GetLastError();
                    API_VERIFY(CloseServiceHandle(m_hDriverDemoService));
                    m_hDriverDemoService = NULL;
                    SetLastError(dwLastError);
                    bRet = FALSE;
                }
            }
        }
    }
    
    return bRet;
}

BOOL CFDriverDemoController::UnInstallService()
{
    BOOL bRet = FALSE;
    if (m_hDriverDemoService)
    {
        SERVICE_STATUS ServiceStatus;
        API_VERIFY_EXCEPT1(ControlService(m_hDriverDemoService, SERVICE_CONTROL_STOP, &ServiceStatus), ERROR_SERVICE_NOT_ACTIVE);
        //API_VERIFY(DeleteService(m_hDriverDemoService));
        API_VERIFY(CloseServiceHandle(m_hDriverDemoService));
        m_hDriverDemoService = NULL;
    }
    if (m_hSCManager)
    {
        API_VERIFY(CloseServiceHandle(m_hSCManager));
        m_hDriverDemoService = NULL;
    }

    return bRet;
}

BOOL CFDriverDemoController::OpenDemoDriver()
{
	BOOL bRet = TRUE;
	if (NULL == m_hDemoDriverFile)
	{
		API_VERIFY((m_hDemoDriverFile = ::CreateFile(FDRIVER_DEMO_DOS_NAME, GENERIC_READ | GENERIC_WRITE,
			0, //·Ç¹²Ïí
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			)) != INVALID_HANDLE_VALUE);
		if (!bRet)
		{
			FTLASSERT(INVALID_HANDLE_VALUE == m_hDemoDriverFile);
			m_hDemoDriverFile = NULL;
		}
	}
	return bRet;
}

BOOL CFDriverDemoController::CloseDemoDriver()
{
	BOOL bRet = TRUE;
	if (m_hDemoDriverFile)
	{
		FTLASSERT(m_hDemoDriverFile != INVALID_HANDLE_VALUE);
		CloseHandle(m_hDemoDriverFile);
		m_hDemoDriverFile = NULL;
	}
	return bRet;

}