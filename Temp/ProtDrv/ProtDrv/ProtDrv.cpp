#include "stdafx.h"
//#include "ASM/AsmHelperFun.h"
#include "KernelHookAPI.h"
#include "FDriverUtil.h"

//PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;

//读取MSR，然后特征码匹配的方法

//http://blog.chinaunix.net/uid-27471192-id-3314623.html


NTSTATUS SimpleDriverDispatchDefault(
								  IN PDEVICE_OBJECT InDeviceObject,
								  IN PIRP pIrp)
{
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS ProtectDriverDispatchDeviceControl(
                                     IN PDEVICE_OBJECT InDeviceObject,
                                     IN PIRP pIrp)
{
    NTSTATUS status	= STATUS_INVALID_DEVICE_REQUEST;

    PIO_STACK_LOCATION pIoStackLoc = IoGetCurrentIrpStackLocation(pIrp);
    NT_ASSERT(pIoStackLoc->MajorFunction == IRP_MJ_DEVICE_CONTROL);

    PVOID inputBuffer = pIrp->AssociatedIrp.SystemBuffer;
    PVOID outputBuffer = pIrp->AssociatedIrp.SystemBuffer;

    ULONG inputBufferLength = pIoStackLoc->Parameters.DeviceIoControl.InputBufferLength;
    ULONG outputBufferLength = pIoStackLoc->Parameters.DeviceIoControl.OutputBufferLength;
    ULONG ioControlCode = pIoStackLoc->Parameters.DeviceIoControl.IoControlCode;

    switch (ioControlCode)
    {
    case IOCTL_PROTDRV_INSTALL_HOOK:
		{
			if (inputBufferLength == sizeof(HANDLE))
			{
				FNT_VERIFY(InstallHook(HANDLE(inputBuffer)));
			}
		}
        break;

    case IOCTL_PROTDRV_UNINSTALL_HOOK:
		{
			FNT_VERIFY(UnInstallHook());
		}
        break;

    case IOCTL_PROTDRV_SET_INFO:
        {
            if (inputBufferLength == sizeof(PROTECT_WND_INFO))
            {
                FNT_VERIFY(SetProtectWndInfo((PPROTECT_WND_INFO)inputBuffer));
            }
            else
            {
                status = STATUS_INVALID_PARAMETER;
            }
        }
        break;
    case IOCTL_PROTDRV_CLEAR_INFO:
        {
            FNT_VERIFY(SetProtectWndInfo(NULL));
        }
        break;
    default:
        break;
    }

    //pIrp->IoStatus.Information = 0;
    //pIrp->IoStatus.Status = STATUS_SUCCESS;

    //IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    return status;
}


VOID ProtectDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
    /*
		Delete the device object
    */
	KdPrint(("ProtectDriverUnload, pDriverObject=0x%p, Device=0x%p\n", pDriverObject, pDriverObject->DeviceObject));

	
	NTSTATUS status = STATUS_SUCCESS;
	FNT_VERIFY(UnInstallHook());

	UNICODE_STRING Win32NameString;
	RtlInitUnicodeString (&Win32NameString , PROTECT_DOS_DEVICE_NAME);	

	FNT_VERIFY(IoDeleteSymbolicLink (&Win32NameString));
	NT_ASSERT(NULL != pDriverObject->DeviceObject);

    IoDeleteDevice(pDriverObject->DeviceObject);
}


extern "C" NTSTATUS DriverEntry(
					 IN PDRIVER_OBJECT		pDriverObject,
					 IN PUNICODE_STRING		pRegistryPath)
{
	KdPrint(("[%d] ProtectDrv DriverEntry, pDriverObject=0x%p, pRegistryPath=%wZ\n", 
        PsGetCurrentProcessId(), pDriverObject, pRegistryPath));

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT					DeviceObject = NULL;
    UNICODE_STRING ntName;
    UNICODE_STRING win32Name;

    RtlInitUnicodeString(&ntName, PROTECT_DEVICE_NAME);
	FNT_VERIFY(IoCreateDevice(
		pDriverObject,
		0,								// DeviceExtensionSize
		&ntName,
		PROTDRV_DEVICE_TYPE,							// DeviceType
		0,								// DeviceCharacteristics
		TRUE,							// Exclusive
		&DeviceObject					// [OUT]
		));
	if(!NT_SUCCESS(status))
	{
        KdPrint( ("%s >> IoCreateDevice fail", __FUNCTION__) );
        return status;
    }

	KdPrint(("Address of new DeviceObject is 0x%p\n" , DeviceObject));

    RtlInitUnicodeString (&win32Name, PROTECT_DOS_DEVICE_NAME);	
    FNT_VERIFY(IoCreateSymbolicLink ( 
        &win32Name, 
        &ntName));
    if (FALSE == NT_SUCCESS(status))
    {
        KdPrint( ("%s >> IoCreateSymbolicLink() failed", __FUNCTION__) );
        IoDeleteDevice( DeviceObject );
        return status;
    }
	KdPrint(("Success Create Symbolic Link %wZ => %wZ\n", &win32Name, &ntName));

    NT_ASSERT(pDriverObject->DeviceObject == NULL || pDriverObject->DeviceObject == DeviceObject);

    pDriverObject->MajorFunction[IRP_MJ_CREATE] = SimpleDriverDispatchDefault;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SimpleDriverDispatchDefault;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ProtectDriverDispatchDeviceControl;
    pDriverObject->DriverUnload = ProtectDriverUnload;

	//UNICODE_STRING			SymbolName;
	//RtlInitUnicodeString(&SymbolName, L"KiSystemCall64");
	//PVOID pAddress = MmGetSystemRoutineAddress(&SymbolName);
	//KdPrint(("Address of %wZ is 0x%p, \n" , &SymbolName, pAddress));

	//pAddress = GetKiSystemCall64Address();
	//KdPrint(("GetKiSystemCall64Address is 0x%p\n" , pAddress));

	//pAddress = (PVOID)GetKeServiceDescriptorTableShadow64();
	//KdPrint(("GetKeServiceDescriptorTableShadow64=0x%p\n", pAddress));

	//if (pAddress)
	//{
	//	ULONG64 IndexOfNtGdiBitBlt = 0x1008;
	//	ULONGLONG AddrOfGdiBitBlt = GetSSSDTFuncCurAddr64((ULONGLONG)pAddress, IndexOfNtGdiBitBlt);
	//	KdPrint(("AddrOfGdiBitBlt=0x%p\n", AddrOfGdiBitBlt));	
	//}

	return status;
}


//#ifdef ALLOC_PRAGMA
//
//#pragma alloc_text(INIT, DriverEntry)
//#pragma alloc_text(PAGE, SimpleDriverDispatchDefault)
//#pragma alloc_text(PAGE, SimpleDriverUnload)
//
//#endif
//
