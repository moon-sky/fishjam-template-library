#include "stdafx.h"
#include "FDriverMemory.h"
#include "FDriverDemoDefine.h"
#include "FDriverUtil.h"
#include "FDriverHookAPI.h"
#include "KernelHookAPI.h"

#ifdef __cplusplus
//#pragma INITCODE
extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

//提供一个Unload函数只是为了让这个程序能动态卸载，方便调试。否则一个内核模块一旦加载就不能再卸载了
void FJDriverDemoUnload(IN PDRIVER_OBJECT DriverObject)
{
	KdPrint(("Enter FJDriverDemoUnload, PID=%d\n", PsGetCurrentProcessId()));
	
    NTSTATUS status = STATUS_SUCCESS;
	FNT_VERIFY(UnInstallCopyProtectHook());

	UNICODE_STRING Win32NameString;

	RtlInitUnicodeString (&Win32NameString , FDRIVER_DEMO_DOS_DEVICE_NAME);	
	FNT_VERIFY(IoDeleteSymbolicLink (&Win32NameString));
	NT_ASSERT(NULL != DriverObject->DeviceObject);

	IoDeleteDevice(DriverObject->DeviceObject);

	KdPrint(("Leave FJDriverDemoUnload, PID=%d\n", PsGetCurrentProcessId()));
}

#pragma PAGEDCODE
NTSTATUS FJDriverDemoDefaultHandler(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    //PAGED_CODE();

	PIO_STACK_LOCATION pIoStackLoc = IoGetCurrentIrpStackLocation(pIrp);
	KdPrint(("DefaultHandler %s\n", GetIrpMajorCodeString(pIoStackLoc->MajorFunction)));

	NTSTATUS status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;	//设置IRP完成状态
	pIrp->IoStatus.Information = 0;	//设置操作的字节数为0
	IoCompleteRequest( pIrp, IO_NO_INCREMENT);	//指示完成此IRP，不再继续处理

	return status;
	//跳过当前的设备，由设备栈中的下层设备继续处理
	//IoSkipCurrentIrpStackLocation(pIrp);
	//deviceExtension = (PFJDriverDemo_DEVICE_EXTENSION) DeviceObject->DeviceExtension;
	//return IoCallDriver(deviceExtension->TargetDeviceObject, pIrp);
}

//typedef BOOL (*MYORIGINALCODE)(INT nNumber1, INT nNumber2, WCHAR* pChar3);
//MYORIGINALCODE		g_pOldMyCode = NULL;
//PINLINE_HOOK_INFO	g_pInlineHookInfo = NULL;

//BOOL MyOriginalCode(INT nNumber1, INT nNumber2, WCHAR* pChar3)
//{
//	KdPrint(("Enter MyOriginalCode, nNumber1=%d, nNunmber2=%d, pChar3=%wS\n", nNumber1, nNumber2, pChar3));
//
//	INT nResult = nNumber1 + nNumber2;
//
//	KdPrint(("Leave MyOriginalCode, nResult=%d\n", nResult));
//
//	return TRUE;
//}
//
//BOOL MyOriginalCode_Hook(INT nNumber1, INT nNumber2, WCHAR* pChar3)
//{
//	BOOL					bRet = FALSE;
//
//	KdPrint(("Enter MyOriginalCode_Hook\n"));
//	if (g_pOldMyCode)
//	{
//		bRet =  (g_pOldMyCode)(nNumber1, nNumber2, pChar3);
//	}
//	
//	KdPrint(("Leave MyOriginalCode_Hook\n"));
//	return bRet;
//}

#pragma PAGEDCODE
NTSTATUS FJDriverDemoDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    NTSTATUS status	= STATUS_INVALID_PARAMETER;;

	PIO_STACK_LOCATION pIoStackLoc = IoGetCurrentIrpStackLocation(pIrp);
	NT_ASSERT(pIoStackLoc->MajorFunction == IRP_MJ_DEVICE_CONTROL);

    //因为是缓冲方式，所以输入和输出都是使用 AssociatedIrp.SystemBuffer，操作系统会自动在 R3/R0 间复制
	PVOID inputBuffer = pIrp->AssociatedIrp.SystemBuffer;
	PVOID outputBuffer = pIrp->AssociatedIrp.SystemBuffer;

	ULONG inputBufferLength = pIoStackLoc->Parameters.DeviceIoControl.InputBufferLength;
	ULONG outputBufferLength = pIoStackLoc->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG ioControlCode = pIoStackLoc->Parameters.DeviceIoControl.IoControlCode;

	switch (ioControlCode)
	{
	case IOCTL_FDRIVER_INSTALL_HOOK:
		{
            KdPrint(("Enter IOCTL_FDRIVER_INSTALL_HOOK\n"));
            HANDLE hTargetProcess = NULL;
#if defined(_M_IX86)
            PINSTALL_COPY_PROTECT_HOOK  pHook = PINSTALL_COPY_PROTECT_HOOK(inputBuffer);
            hTargetProcess = pHook->hTargetProcess;
#elif defined(_M_AMD64)
            if (IoIs32bitProcess(pIrp))
            {
                PINSTALL_COPY_PROTECT_HOOK_32 pHook32 = PINSTALL_COPY_PROTECT_HOOK_32(inputBuffer);
                hTargetProcess = pHook32->hTargetProcess;
            }
            else
            {
                PINSTALL_COPY_PROTECT_HOOK  pHook = PINSTALL_COPY_PROTECT_HOOK(inputBuffer);
                hTargetProcess = pHook->hTargetProcess;
            }
#endif 
            FNT_VERIFY(InstallCopyProtectHook(hTargetProcess));
			break;
		}
    case IOCTL_FDRIVER_UPDATE_PROTECT_WND:
        {
            BOOL bSameBitProcess = TRUE;
            KdPrint(("Enter IOCTL_FDRIVER_UPDATE_PROTECT_WND, inputBufSize=%d, inputBuf=%p, sizeof(WND_INFO) =%d\n",
                inputBufferLength, inputBuffer, sizeof(PROTECT_WND_INFO)));

#if defined(_M_AMD64)
            //AMD64
            if (IoIs32bitProcess(pIrp))
            {
                bSameBitProcess = FALSE;
                NT_ASSERT(sizeof(PPROTECT_WND_INFO_32) == inputBufferLength || 0 == inputBufferLength);

                if (sizeof(PPROTECT_WND_INFO_32) == inputBufferLength || 0 == inputBufferLength)
                {
                    //If Caller is x32 Program
                    PPROTECT_WND_INFO_32 pProtectWndInfo32 = (PPROTECT_WND_INFO_32)inputBuffer;
                    if (pProtectWndInfo32)
                    {
                        PROTECT_WND_INFO protectWndInfo64;
                        protectWndInfo64.hWndDeskTop = (HWND)pProtectWndInfo32->hWndDeskTop;
                        //protectWndInfo64.hTargetProcess = pProtectWndInfo32->hTargetProcess;
                        protectWndInfo64.rcProtectWindow = pProtectWndInfo32->rcProtectWindow;
                        protectWndInfo64.hDCWndProtect = (HDC)pProtectWndInfo32->hDCWndProtect;
                        FNT_VERIFY(UpdateCopyProtectWndInfo(&protectWndInfo64));
                    }
                    else
                    {
                        FNT_VERIFY(UpdateCopyProtectWndInfo(NULL));
                    }
                }
            }
#endif 
            if (bSameBitProcess)  //32 or 64, Same
            {
                NT_ASSERT(sizeof(PROTECT_WND_INFO) == inputBufferLength || 0 == inputBufferLength);
                if (sizeof(PROTECT_WND_INFO) == inputBufferLength || 0 == inputBufferLength)
                {
                    FNT_VERIFY(UpdateCopyProtectWndInfo((PPROTECT_WND_INFO)inputBuffer));
                }
            }
            break;
        }
	case IOCTL_FDRIVER_UNINSTALL_HOOK:
		{
			KdPrint(("Enter IOCTL_FDRIVER_UNINSTALL_HOOK\n"));
			FNT_VERIFY(UnInstallCopyProtectHook());
			break;
		}
    default:
		NT_ASSERT(FALSE);
        break;
	}
	pIrp->IoStatus.Status = status;	//设置IRP完成状态
	pIrp->IoStatus.Information = 0;	//设置操作的字节数为0
	IoCompleteRequest( pIrp, IO_NO_INCREMENT);	//指示完成此IRP，不再继续处理
	return status;
	//return FJDriverDemoDefaultHandler(DeviceObject, pIrp);
}	

//每个内核模块的入口函数，在加载该模块时被系统进程System调用一次
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING  RegistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Enter FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));
	
#if DBG
	//汇编指令，相当于手工设置一个断点，这样可以调试
	//_asm int 3
#endif 
	PDEVICE_OBJECT deviceObject = NULL;
	UNICODE_STRING ntName;
	UNICODE_STRING win32Name;

	//UNICODE_STRING					SymbolName;
	//PVOID pAddress = NULL;
	//RtlInitUnicodeString(&SymbolName, L"KeCancelTimer");
	//pAddress = MmGetSystemRoutineAddress(&SymbolName);

	//KdPrint(("%wZ pAddress=0x%p\n"), &SymbolName, pAddress);


	RtlInitUnicodeString(&ntName, FDRIVER_DEMO_NT_DEVICE_NAME);
	FNT_VERIFY(IoCreateDevice(pDriverObject, 
		0, 
		&ntName, 
		FDRIVER_DEV_DRV,	//FILE_DEVICE_UNKNOWN
		0,	//FILE_DEVICE_SECURE_OPEN
		TRUE, 
		&deviceObject));
	if (FALSE == NT_SUCCESS(status))
	{
		KdPrint( ("%s >> IoCreateDevice fail", __FUNCTION__) );
		return status;
	}

	RtlInitUnicodeString (&win32Name, FDRIVER_DEMO_DOS_DEVICE_NAME);	
	FNT_VERIFY(IoCreateSymbolicLink ( 
		&win32Name, 
		&ntName));
	if (FALSE == NT_SUCCESS(status))
	{
		KdPrint( ("%s >> IoCreateSymbolicLink() failed", __FUNCTION__) );
		IoDeleteDevice( deviceObject );
		return status;
	}
    
    //deviceObject->Flags |= DO_BUFFERED_IO;  -- 设置为缓冲读写方式

	NT_ASSERT(pDriverObject->DeviceObject == NULL || pDriverObject->DeviceObject == deviceObject);
	//if (pDriverObject->DeviceObject == NULL)
	//{
	//  是否需要设置过去？没有看到哪个驱动程序的代码中设置过，系统会自动设置过去？Unload中直接 Delete 的
	//	//WinXP
	//	pDriverObject->DeviceObject = deviceObject;
	//}

	for (unsigned int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = FJDriverDemoDefaultHandler;
	}

	//pDriverObject->MajorFunction[IRP_MJ_CREATE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_CLOSE] = FJDriverDemoCreateClose;
	//pDriverObject->MajorFunction[IRP_MJ_PNP] = FJDriverDemoPnP;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = FJDriverDemoDeviceControl;
	pDriverObject->DriverUnload = FJDriverDemoUnload;

	KdPrint(("Leave FJDriverDemo DriverEntry,PID=%d\n", PsGetCurrentProcessId()));

	return status;
}