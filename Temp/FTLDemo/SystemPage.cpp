// SystemPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "SystemPage.h"
#include <ftlsystem.h>
#include <ftlService.h>
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "WtsApi32.lib")

// CSystemPage 对话框

IMPLEMENT_DYNAMIC(CSystemPage, CPropertyPage)

CSystemPage::CSystemPage()
	: CPropertyPage(CSystemPage::IDD)
{

}

CSystemPage::~CSystemPage()
{
}

void CSystemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_TEMP_FPU_RC_RESET, &CSystemPage::OnBnClickedBtnTempFpuRcReset)
	ON_BN_CLICKED(IDC_BTN_SYSTEM_METRICS, &CSystemPage::OnBnClickedBtnSystemMetrics)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_CREATE_PROCESS_AS_USER, &CSystemPage::OnBnClickedBtnCreateProcessAsUser)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_WAIT_ALL_CHILD_PROCESS, &CSystemPage::OnBnClickedBtnCreateProcessAndWaitAllChild)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_GET_REG_VALUE_EXPORT_STRING, &CSystemPage::OnBnClickedBtnGetRegValueExportString)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_TEST_OPENREG, &CSystemPage::OnBnClickedBtnTestOpenReg)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_CHECK_RUN_ON_VMWARE, &CSystemPage::OnBnClickedBtnCheckRunningOnVMWare)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_CHECK_RUN_ON_VPC, &CSystemPage::OnBnClickedBtnCheckRunningOnVirtualPC)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_CHECK_RUN_ON_VBOX, &CSystemPage::OnBnClickedBtnCheckRunningOnVirtualBox)
    ON_BN_CLICKED(IDC_BTN_SYSTEM_CHECK_RUNING_MACHINE_TYPE, &CSystemPage::OnBnClickedBtnCheckRunningMachineType)

END_MESSAGE_MAP()


// CSystemPage 消息处理程序

BOOL CSystemPage::OnInitDialog()
{
    BOOL bIs64Bit = CFSystemUtil::Is64BitWindows();
    SetDlgItemText(IDC_STATIC_IS64, bIs64Bit ? TEXT("TRUE") : TEXT("FALSE"));
    return TRUE;  // return TRUE unless you set the focus to a control
}

void CSystemPage::OnBnClickedBtnTempFpuRcReset()
{
    double f = 1.0 / 13.0;
    TRACE(TEXT("f = %f\n"),f);

    CFTempFpuRcReset rcReset;
    f = 1.0 / 13.0;
    TRACE(TEXT("f = %f\n"),f);
}

void CSystemPage::OnBnClickedBtnSystemMetrics()
{
	CFSystemMetricsProperty	systemMetricsProperty;
	FTLTRACE(TEXT("SystemMetricsProperty = %s\n"), systemMetricsProperty.GetPropertyString());
}

void CSystemPage::OnBnClickedBtnCreateProcessAsUser()
{
    BOOL bRet = FALSE;
    HANDLE hCurProcess = GetCurrentProcess();
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_CHANGE_NOTIFY_NAME, TRUE));
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_INCREASE_QUOTA_NAME, TRUE));
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_TCB_NAME, TRUE)); 
    API_VERIFY(FTL::CFSystemUtil::EnableProcessPrivilege(hCurProcess, SE_ASSIGNPRIMARYTOKEN_NAME, TRUE));
    API_VERIFY(FTL::CFService::CreateServiceUIProcess(TEXT("C:\\Windows\\System32\\calc.exe"), TRUE, NULL));
}

void CSystemPage::OnBnClickedBtnCreateProcessAndWaitAllChild()
{
    FTL::CFSystemUtil::CreateProcessAndWaitAllChild(TEXT("C:\\Windows\\System32\\cmd.exe"));
}

void CSystemPage::OnBnClickedBtnGetRegValueExportString()
{
    //BOOL bRet = FALSE;
    //LONG nBufStringCount = 0;
    //BYTE bufs[] = {0x12, 0x34, 0x56, 0x78};
    //LONG nBytesCount = sizeof(bufs);

    //API_VERIFY(CFConvUtil::HexFromBinary(bufs, nBytesCount, NULL, &nBufStringCount, _T(',')));
    //CFMemAllocator<TCHAR> bufString(nBufStringCount);
    //API_VERIFY(CFConvUtil::HexFromBinary(bufs, nBytesCount, bufString.GetMemory(), &nBufStringCount, _T(',')));
    //FTLTRACE(TEXT("string is %s\n"), bufString.GetMemory());
    //return;


    //LONG lRet = ERROR_SUCCESS;
    //CAtlString strResult;
    //DWORD dwRegType = REG_NONE;

    //CRegKey reg;
    //REG_VERIFY(reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), KEY_READ | KEY_WOW64_64KEY ));
    //if (ERROR_SUCCESS == lRet)
    //{
    //    LPCTSTR pszValueNames[] = {
    //        _T("PathName"),
    //        //_T("LogFileDirectory"),
    //    };

    //    for (int i = 0; i < _countof(pszValueNames); i++)
    //    {
    //        REG_VERIFY(FTL::CFRegUtil::GetRegValueExportString(reg, pszValueNames[i], strResult, &dwRegType));
    //        FTLTRACE(TEXT("%s(%d) export value is %s\n"), pszValueNames[i], dwRegType, strResult);
    //    }
    //    
    //}
}

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void CSystemPage::OnBnClickedBtnTestOpenReg()
{
    LONG lRet = ERROR_SUCCESS;
    CAtlString strSubKey = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer");
    CRegKey regLocalMachine;
    REG_VERIFY(regLocalMachine.Open(HKEY_LOCAL_MACHINE, strSubKey, KEY_ALL_ACCESS));


    //TCHAR    achKey[MAX_VALUE_NAME] = {0};   // buffer for subkey name
    //DWORD    cbName = _countof(achKey);                   // size of name string 
    TCHAR    achClass[MAX_PATH] = {0};  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    //TCHAR  achValue[MAX_VALUE_NAME]; 
    //DWORD cchValue = MAX_VALUE_NAME; 

    // Get the class name and the value count. 
    REG_VERIFY(RegQueryInfoKey(
        regLocalMachine.m_hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime));       // last write time 



    CRegKey regCurrentUser;
    REG_VERIFY(regCurrentUser.Open(HKEY_CURRENT_USER, strSubKey, KEY_QUERY_VALUE));

    REG_VERIFY(RegQueryInfoKey(
        regCurrentUser.m_hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime));       // last write time 

    //FTLASSERT(regLocalMachine.m_hKey == regCurrentUser.m_hKey);

}


BOOL VirtualPCTest()
{
    BOOL gInVirtualPC = TRUE;
    __try
    {
        __asm
        {
            pushad      
                mov ebx, 0 // Flag
                mov eax, 1 // VPC function number
                __emit 0Fh
                __emit 3Fh
                __emit 07h
                __emit 0Bh
                test ebx, ebx
                sete al
                movzx eax, al
                mov gInVirtualPC , eax;
            popad
        }

    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        gInVirtualPC=FALSE;
    }
    return gInVirtualPC;
}

void CSystemPage::OnBnClickedBtnCheckRunningOnVMWare()
{
    BOOL isVmWare = FTL::CFSystemUtil::IsInsideVMWare();
    CAtlString strInfo = isVmWare ? TEXT("On VmWare") : TEXT("Not On VmWare");
    AfxMessageBox(strInfo);
}

void CSystemPage::OnBnClickedBtnCheckRunningOnVirtualPC()
{
    BOOL isVPC = FTL::CFSystemUtil::IsInsideVPC();
    CAtlString strInfo = isVPC ? TEXT("On VPC") : TEXT("Not On VPC");
    AfxMessageBox(strInfo);
}

BOOL DoIdentify (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                         PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                         PDWORD lpcbBytesReturned)
{
    // Set up data structures for IDENTIFY command.
    pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
    pSCIP -> irDriveRegs.bFeaturesReg = 0;
    pSCIP -> irDriveRegs.bSectorCountReg = 1;
    pSCIP -> irDriveRegs.bSectorNumberReg = 1;
    pSCIP -> irDriveRegs.bCylLowReg = 0;
    pSCIP -> irDriveRegs.bCylHighReg = 0;

    // Compute the drive number.
    pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

    // The command can either be IDE identify or ATAPI identify.
    pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
    pSCIP -> bDriveNumber = bDriveNum;
    pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

    return( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
        (LPVOID) pSCIP,
        sizeof(SENDCMDINPARAMS) - 1,
        (LPVOID) pSCOP,
        sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
        lpcbBytesReturned, NULL) );
}

char *ConvertToString(DWORD diskdata [256], char* pOutBuf, int nSize, int firstIndex, int lastIndex)
{
    ZeroMemory(pOutBuf, nSize);

    FTLASSERT(lastIndex - firstIndex < nSize);
    if (lastIndex - firstIndex < nSize)
    {
        int index = 0;
        int position = 0;

        //  each integer has two characters stored in it backwards
        for (index = firstIndex; index <= lastIndex; index++)
        {
            //  get high byte for 1st character
            pOutBuf [position] = (char) (diskdata [index] / 256);
            position++;

            //  get low byte for 2nd character
            pOutBuf [position] = (char) (diskdata [index] % 256);
            position++;
        }
        //  end the string 
        pOutBuf [position] = '\0';

        //  cut off the trailing blanks
        for (index = position - 1; index > 0 && ' ' == pOutBuf [index]; index--)
            pOutBuf [index] = '\0';
    }

    return pOutBuf;
}

BOOL GetDiskName(INT nDiskIndex, CAtlString& strDiskName)
{
    BOOL bRet = FALSE;

    CAtlString strPhysicalDriverName;
    strPhysicalDriverName.Format(TEXT("\\\\.\\PhysicalDrive%d"), nDiskIndex);
    
    HANDLE hPhysicalDriveIOCTL = CreateFile (strPhysicalDriverName,
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL);
    API_VERIFY( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE );

    if (bRet)
    {
        GETVERSIONOUTPARAMS VersionParams = {0};
        DWORD               cbBytesReturned = 0;

        // Get the version, etc of PhysicalDrive IOCTL
        memset ((void*) &VersionParams, 0, sizeof(VersionParams));
        API_VERIFY(DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
            NULL,  0, &VersionParams, sizeof(VersionParams), &cbBytesReturned, NULL));

        // If there is a IDE device at number "i" issue commands
        // to the device
        if (bRet && VersionParams.bIDEDeviceMap > 0)
        {
            BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
            SENDCMDINPARAMS  scip;
            //SENDCMDOUTPARAMS OutCmd;
            BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
            // Now, get the ID sector for all IDE devices in the system.
            // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
            // otherwise use the IDE_ATA_IDENTIFY command
            bIDCmd = (VersionParams.bIDEDeviceMap >> nDiskIndex & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

            memset (&scip, 0, sizeof(scip));
            memset (IdOutCmd, 0, sizeof(IdOutCmd));

            API_VERIFY(DoIdentify(hPhysicalDriveIOCTL, 
                &scip, 
                (PSENDCMDOUTPARAMS)&IdOutCmd, 
                (BYTE) bIDCmd,
                (BYTE) nDiskIndex,
                &cbBytesReturned));
            if (bRet)
            {
                DWORD diskdata [256];
                USHORT *pIdSector = (USHORT *)((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;
                for (int i = 0; i < 256; i++)
                {
                    diskdata [i] = pIdSector [i];
                }
                char diskName[128] = {0};
                strDiskName = CAtlString(ConvertToString(diskdata, diskName, _countof(diskName), 27, 46));
            }
        }
        CloseHandle(hPhysicalDriveIOCTL);
    }

    return bRet;
}

void CSystemPage::OnBnClickedBtnCheckRunningOnVirtualBox()
{
    //BOOL isVirtualBox = FTL::CFSystemUtil::IsInsideVirtualBox();
    //CAtlString strInfo = isVirtualBox ? TEXT("On VirtualBox") : TEXT("Not On VirtualBox");
    //AfxMessageBox(strInfo);
    BOOL bRet = FALSE;
    CAtlString strDiskName;
    API_VERIFY(GetDiskName(0, strDiskName));
    if (bRet)
    {
        AfxMessageBox(strDiskName);
    }
    
}

void CSystemPage::OnBnClickedBtnCheckRunningMachineType()
{
    CString strResult = TEXT("Unknown Result");
    FTL::VirtualMachineType vmType = FTL::CFSystemUtil::CheckRunningMachineType();
    switch (vmType)
    {
    case vmtError:
        strResult = TEXT("vmtError");
        break;
    case vmtUnknown:
        strResult = TEXT("vmtUnknown");
        break;
    case vmtReal:
        strResult = TEXT("vmtReal");
        break;
    case vmtVmWare:
        strResult = TEXT("vmtVmWare");
        break;
    case vmtVirtualPC:
        strResult = TEXT("vmtVirtualPC");
        break;
    case vmtVirtualBox:
        strResult = TEXT("vmtVirtualBox");
        break;
    default:
        FTLASSERT(FALSE);
        break;
    }
    AfxMessageBox(strResult);
}