#include "stdafx.h"
#include "FDriverDemoController.h"
//#include "D:\\Code\\OpenCapture\\trunk\\CaptureDriver\\CaptureDriver.h"

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

BOOL CFDriverDemoController::InstallService(LPCTSTR pszDriverFilePath, LPCTSTR pszServiceName)
{
    BOOL bRet = TRUE;

    if (NULL == m_hSCManager)
    {
        FTLASSERT(NULL == m_hDriverDemoService);

        API_VERIFY((m_hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) != NULL);
        if (m_hSCManager)
        {
            API_VERIFY_EXCEPT1((m_hDriverDemoService = ::CreateService(
                m_hSCManager, 
                pszServiceName, 
                TEXT("FDriver Demo Service"),
                SERVICE_ALL_ACCESS,
                SERVICE_KERNEL_DRIVER,	//SERVICE_FILE_SYSTEM_DRIVER
                SERVICE_DEMAND_START,
                SERVICE_ERROR_NORMAL,
                pszDriverFilePath, 
                NULL, 
                NULL,
                NULL, 
                NULL, 
                NULL
                )) != NULL, ERROR_SERVICE_EXISTS);
            if (!m_hDriverDemoService)
            {
                m_hDriverDemoService = ::OpenService(m_hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
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
    
	FTLTRACE(TEXT("InstallService, DriverPath=%s, m_hSCManager=0x%x\n"), pszDriverFilePath,  m_hSCManager);
    return bRet;
}

BOOL CFDriverDemoController::UnInstallService()
{
    BOOL bRet = TRUE;
    if (m_hDriverDemoService)
    {
        SERVICE_STATUS ServiceStatus;
        API_VERIFY_EXCEPT1(ControlService(m_hDriverDemoService, SERVICE_CONTROL_STOP, &ServiceStatus), ERROR_SERVICE_NOT_ACTIVE);
        API_VERIFY(DeleteService(m_hDriverDemoService));
        API_VERIFY(CloseServiceHandle(m_hDriverDemoService));
        m_hDriverDemoService = NULL;
    }
    if (m_hSCManager)
    {
		FTLTRACE(TEXT("UnInstallService, m_hSCManager=0x%x\n"), m_hSCManager);
        API_VERIFY(CloseServiceHandle(m_hSCManager));
        m_hSCManager = NULL;
    }

    return bRet;
}

BOOL CFDriverDemoController::OpenDemoDriver(LPCTSTR pszDosDriverName)
{
	BOOL bRet = FALSE;
	if (NULL == m_hDemoDriverFile)
	{
		API_VERIFY((m_hDemoDriverFile = ::CreateFile(pszDosDriverName, GENERIC_READ | GENERIC_WRITE,
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
		FTLTRACE(TEXT("After OpenDemoDriver %s, m_hDemoDriverFile=0x%x\n"), pszDosDriverName, m_hDemoDriverFile);
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

		FTLTRACE(TEXT("After CloseDemoDriver\n"));
	}
	return bRet;

}

BOOL CFDriverDemoController::IoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize,
		LPVOID lpOutBuffer, DWORD nOutBufferSize)
{
	BOOL bRet = FALSE;
	FTLASSERT(m_hDemoDriverFile);
	if (m_hDemoDriverFile)
	{
		DWORD nBytesReturned = 0;
		API_VERIFY(DeviceIoControl(m_hDemoDriverFile, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer,
			nInBufferSize, &nBytesReturned, NULL));
		FTLTRACE(TEXT("After IoControl \n"));
	}
	return bRet;
}