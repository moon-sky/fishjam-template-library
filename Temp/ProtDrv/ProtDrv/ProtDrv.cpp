#include "stdafx.h"
#include "ASM/AsmHelperFun.h"
#include "KernelHookAPI.h"

//PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;

//读取MSR，然后特征码匹配的方法

//http://blog.chinaunix.net/uid-27471192-id-3314623.html


NTSTATUS SimpleDriverDispatchDefault(
								  IN PDEVICE_OBJECT InDeviceObject,
								  IN PIRP InIrp)
{
	InIrp->IoStatus.Information = 0;
	InIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(InIrp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

VOID SimpleDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
    /*
		Delete the device object
    */
	KdPrint(("SimpleDriverUnload, pDriverObject=0x%p, Device=0x%p\n", pDriverObject, pDriverObject->DeviceObject));

    IoDeleteDevice(pDriverObject->DeviceObject);
}


extern "C" NTSTATUS DriverEntry(
					 IN PDRIVER_OBJECT		pDriverObject,
					 IN PUNICODE_STRING		pRegistryPath)
{
	KdPrint(("Simple DriverEntry, pDriverObject=0x%p, pRegistryPath=%wZ\n", pDriverObject, pRegistryPath));

	UNICODE_STRING			SymbolName;
	RtlInitUnicodeString(&SymbolName, L"KiSystemCall64");
	PVOID pAddress = MmGetSystemRoutineAddress(&SymbolName);
	KdPrint(("Address of %wZ is 0x%p, \n" , &SymbolName, pAddress));

	pAddress = GetKiSystemCall64Address();
	KdPrint(("GetKiSystemCall64Address is 0x%p\n" , pAddress));


	pAddress = (PVOID)GetKeServiceDescriptorTableShadow64();
	KdPrint(("GetKeServiceDescriptorTableShadow64=0x%p\n", pAddress));

	NTSTATUS						status;    
	PDEVICE_OBJECT					DeviceObject = NULL;
	status = IoCreateDevice(
		pDriverObject,
		0,								// DeviceExtensionSize
		NULL,
		0x893D,							// DeviceType
		0,								// DeviceCharacteristics
		TRUE,							// Exclusive
		&DeviceObject					// [OUT]
		);
	if(NT_SUCCESS(status))
	{
		KdPrint(("Address of new DeviceObject is 0x%p\n" , DeviceObject));

		pDriverObject->MajorFunction[IRP_MJ_CREATE] = SimpleDriverDispatchDefault;
		pDriverObject->MajorFunction[IRP_MJ_CLOSE] = SimpleDriverDispatchDefault;
		pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SimpleDriverDispatchDefault;
		pDriverObject->DriverUnload = SimpleDriverUnload;
	}
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
