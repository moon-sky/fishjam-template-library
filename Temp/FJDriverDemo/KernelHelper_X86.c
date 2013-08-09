#include "stdafx.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"

#if defined(_M_IX86)

PSYSTEM_SERVICE_TABLE	g_pSystemServiceTable= NULL;

KIRQL  WPOFFx64(VOID)
{
	KIRQL irql = 0; //KeGetCurrentIrql();

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

//KeServiceDescriptorTable -- 包含了 SSDT,(其 Shadow SSDT 处都是NULL)
//KeServiceDescriptorTableShadow -- 包含了 SSDT + Shadow SSDT，因此GUI线程不用转换也能调用 SSDT 中的函数
SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ()
{ 
	//32位系统中导出了KeServiceDescriptorTable变量，
	//其后紧接着有未导出的 KeServiceDescriptorTableShadow, 其前部分的内容和 KeServiceDescriptorTable 内容一样
	//通过从 KeAddSystemServiceTable 导出函数开始搜索，并比较内存中的内充，从而找到 KeServiceDescriptorTableShadow，
	//通过搜索 操作SSDT的函数实现中的有效内存地址的办法 来查找 Shadow SSDT
	//TODO: 能取到 KeServiceDescriptorTable 导出变量的地址，返回 KeServiceDescriptorTable[1] 不就是 KeServiceDescriptorTableShadow 的地址了？

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


PVOID GetSSDTFuncAddr(LONG nServiceIndex)
{
	PSYSTEM_SERVICE_TABLE pServiceTable = nServiceIndex < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];
	LONG nIndex = nServiceIndex;
	if (nIndex > 0x1000)
	{
		nIndex -= 0x1000;
	}

	return pServiceTable->ServiceTableBase[nIndex];
}

NTSTATUS HookSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql = 0;
	
	PSYSTEM_SERVICE_TABLE pServiceTable = pHookApiInfo->nIndexInSSDT < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];
	LONG nIndex = pHookApiInfo->nIndexInSSDT;
	if (nIndex > 0x1000)
	{
		nIndex -= 0x1000;
	}

	NT_ASSERT(NULL == pHookApiInfo->pOrigApiAddress);
	pHookApiInfo->pOrigApiAddress = pServiceTable->ServiceTableBase[nIndex];

	KdPrint(("Hook SSDT func %ws at [%d], oldAddress=%p, newAddress=%p\n", 
		pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, 
		pHookApiInfo->pOrigApiAddress, pHookApiInfo->pNewApiAddress));

	irql = WPOFFx64();
	InterlockedExchangePointer(&(pServiceTable->ServiceTableBase[nIndex]), pHookApiInfo->pNewApiAddress);
	WPONx64(irql);

	return status;
}

NTSTATUS RestoreSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql = 0;

	PSYSTEM_SERVICE_TABLE pServiceTable = pHookApiInfo->nIndexInSSDT < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];
	LONG nIndex = pHookApiInfo->nIndexInSSDT;
	if (nIndex > 0x1000)
	{
		nIndex -= 0x1000;
	}

	KdPrint(("Restore SSDT func %ws at [%d], origAddress=%p\n", 
		pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, pHookApiInfo->pOrigApiAddress));

	NT_ASSERT(NULL != pHookApiInfo->pOrigApiAddress);

	irql = WPOFFx64();
	InterlockedExchangePointer(&(pServiceTable->ServiceTableBase[nIndex]), pHookApiInfo->pOrigApiAddress);
	pHookApiInfo->pOrigApiAddress = NULL;
	WPONx64(irql);

	return status;
}

#endif //_M_IX86