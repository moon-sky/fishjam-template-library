#include "stdafx.h"
#include "KernelHelper_X86.h"

#if defined(_M_IX86)

KIRQL  ClearWriteProtect(VOID)
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

VOID  SetWriteProtect(KIRQL irql)
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

PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
__declspec(dllimport) KeAddSystemServiceTable(ULONG, ULONG, ULONG, ULONG, ULONG); 

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
	return rc; 

	//方法2
#if 0
	PUCHAR cPtr, pOpcode;
	ULONG Length = 0;

	for (cPtr = (PUCHAR)KeAddSystemServiceTable;
		cPtr < (PUCHAR)KeAddSystemServiceTable + PAGE_SIZE;
		cPtr += Length)
	{
		if (!MmIsAddressValid(cPtr)) break;

		Length = SizeOfCode(cPtr, &pOpcode);

		if (!Length || (Length == 1 && *pOpcode == 0xC3)) break;

		//找到以下汇编对应的位置： 
		//805ba5a3 8d8840a65580    lea    ecx,nt!KeServiceDescriptorTableShadow (8055a640)[eax]
		if (*(PUSHORT)pOpcode == 0x888D)
		{
			KeServiceDescriptorTableShadow = *(PVOID *)(pOpcode + 2);
			break;
		}
	}
#endif 
}

PVOID GetShadowSSDTFuncAddr(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex)
{
	return pServiceTable[1].ServiceTableBase[nIndex];
}

NTSTATUS ModifyShadowSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress)
{
	NTSTATUS status = STATUS_SUCCESS;
	InterlockedExchangePointer(&(pServiceTable[1].ServiceTableBase[nIndex]), newAddress);

	return status;
}


#endif //_M_IX86