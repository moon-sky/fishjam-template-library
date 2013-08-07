#include "stdafx.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"

#if defined(_M_IX86)

PSYSTEM_SERVICE_TABLE	g_pSystemServiceTable= NULL;

KIRQL  WPOFFx64(VOID)
{
	KIRQL irql = KeGetCurrentIrql();

	//cli + sti 类似 push eax + pop eax
	__asm
	{
		push  eax;
		mov   eax, cr0;
		and   eax, 0x0FFFEFFFF;		//and     eax, not 10000h
		mov   cr0, eax;
		pop   eax;
	}

	return irql;
}

VOID  WPONx64(KIRQL irql)
{
	__asm
	{
		push  eax;
		mov   eax, cr0;
		or    eax, not 0x0FFFEFFFF;  //or eax, 10000h
		mov   cr0, eax;
		pop   eax;
	}
}


typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;

extern PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
extern __declspec(dllimport) KeAddSystemServiceTable(ULONG, ULONG, ULONG, ULONG, ULONG); 

//根据相同版本下与SSDT地址存在的偏移获取的SSDT SHADOW的地址
// WinDbg 下 ?KeServiceDescriptorTable-
// 会打印出：Evaluate expression: 64 = 00000040(XP), -0xE0(2K)
//卡巴斯基有个类似的函数：FindSystemServiceDescriptionTableShadow
SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ()
{ 
	//通过搜索 操作SSDT的函数实现中的有效内存地址的办法 来查找 Shadow SSDT

	// First, obtain a pointer to KeAddSystemServiceTable
	unsigned char *check = (unsigned char*)KeAddSystemServiceTable; 
	int i;
	//Initialize an instance of System Service Table, will be used to
	//obtain an address from KeAddSystemServiceTable
	SYSTEM_SERVICE_TABLE *rc=0; 

	if (g_pSystemServiceTable)
	{
		return g_pSystemServiceTable;
	}

	// Make 100 attempts to match a valid address with that of KeServiceDescriptorTable 
	for (i=0; i<4096; i++) {  //PAGE_SIZE
		__try { 
			// try to obtain an address from  KeAddSystemServiceTable 
			rc = *(SYSTEM_SERVICE_TABLE **)check; 
			// if this address is NOT valid OR it itself is the address of 
			//KeServiceDescriptorTable OR its first entry is NOT equal 
			//to the first entry of KeServiceDescriptorTable 
			if (!MmIsAddressValid (rc) 
				|| (rc == (SYSTEM_SERVICE_TABLE *)KeServiceDescriptorTable) 
				|| (memcmp (rc, KeServiceDescriptorTable, sizeof (*rc)) != 0)) { 
					// Proceed with the next address 
					check++; 
					// don't forget to reset the old address 
					rc = 0; 
			} 
		} __except (EXCEPTION_EXECUTE_HANDLER) 
		{
			rc = 0; 
		} 
		// when the loop is completed, check if it produced a valid address 
		if (rc) 
		{
			// because if it didn't, we failed to find the address of KeServiceDescriptorTableShadow 
			break; 
		}
	} 
	// otherwise, there is a valid address! So return it! 

	g_pSystemServiceTable = rc;

	if (g_pSystemServiceTable)
	{
		KdPrint(("g_pSystemServiceTable : %#x, SSDT EntryCount=%d, Shadow EntryCount=%d\n", 
			g_pSystemServiceTable, g_pSystemServiceTable[0].NumberOfServices, 
			g_pSystemServiceTable[1].NumberOfServices));
	}
	else
	{
		KdPrint(("!!! Find Shadow SSDT Failed from KeAddSystemServiceTable=%p\n", KeAddSystemServiceTable));
	}

	return g_pSystemServiceTable; 
}


PVOID GetShadowSSDTFuncAddr(int nIndex)
{
	return g_pSystemServiceTable[1].ServiceTableBase[nIndex];
}

NTSTATUS HookShadowSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql = 0;

	KdPrint(("Hook Shadow func %ws at [%d], newAddress=%p\n", 
		pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, pHookApiInfo->pNewApiAddress));
	NT_ASSERT(NULL == pHookApiInfo->pOrigApiAddress);

	irql = WPOFFx64();
	pHookApiInfo->pOrigApiAddress = g_pSystemServiceTable[1].ServiceTableBase[pHookApiInfo->nIndexInSSDT];
	InterlockedExchangePointer(&(g_pSystemServiceTable[1].ServiceTableBase[pHookApiInfo->nIndexInSSDT]), pHookApiInfo->pNewApiAddress);
	WPONx64(irql);

	return status;
}

NTSTATUS RestoreShadowSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql = 0;

	KdPrint(("Restore Shadow func %ws at [%d], origAddress=%p\n", 
		pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, pHookApiInfo->pOrigApiAddress));

	NT_ASSERT(NULL != pHookApiInfo->pOrigApiAddress);

	irql = WPOFFx64();
	InterlockedExchangePointer(&(g_pSystemServiceTable[1].ServiceTableBase[pHookApiInfo->nIndexInSSDT]), pHookApiInfo->pOrigApiAddress);
	pHookApiInfo->pOrigApiAddress = NULL;
	WPONx64(irql);

	return status;
}

#endif //_M_IX86