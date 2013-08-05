#include "stdafx.h"
#include "KernelHelper_Amd64.h"

KIRQL ClearWriteProtect()
{
	KIRQL irql= KeRaiseIrqlToDpcLevel();  //KeGetCurrentIrql();
	UINT64 cr0=__readcr0();
	cr0 &= ~0x10000; //0xfffffffffffeffff;
	__writecr0(cr0);
	_disable();
	return irql;
}

void SetWriteProtect(KIRQL irql)
{
	UINT64 cr0=__readcr0();
	cr0 |= 0x10000;
	_enable();
	__writecr0(cr0);
	KeLowerIrql(irql);
}


SYSTEM_SERVICE_TABLE* GetKeServiceDescriptorTableShadowAddress()  //GetKeServiceDescriptorTableShadow64()
{
	PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
	PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
	PUCHAR i = NULL;
	UCHAR b1=0,b2=0,b3=0;
	ULONG templong=0;
	ULONGLONG addr=0;
	for(i=StartSearchAddress;i<EndSearchAddress;i++)
	{
		if( MmIsAddressValid(i) && MmIsAddressValid(i+1) && MmIsAddressValid(i+2) )
		{
			b1=*i;
			b2=*(i+1);
			b3=*(i+2);
			if( b1==0x4c && b2==0x8d && b3==0x1d ) //4c8d1d
			{
				memcpy(&templong,i+3,4);
				addr = (ULONGLONG)templong + (ULONGLONG)i + 7;
				return (SYSTEM_SERVICE_TABLE*)addr;
			}
		}
	}
	return NULL;
}

PVOID GetShadowSSDTFuncAddr(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex)
{
	PBYTE				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k = &pServiceTable[1];
	W32pServiceTable=pWin32k->ServiceTableBase;
	//ul64W32pServiceTable = W32pServiceTable;
	qwTemp = W32pServiceTable + 4 * nIndex;	//这里是获得偏移地址的位置，要HOOK的话修改这里即可
	dwTemp = *(PLONG)qwTemp;
	dwTemp = dwTemp >> 4;
	qwTemp = W32pServiceTable + (LONG64)dwTemp;
	return qwTemp;
}

NTSTATUS ModifyShadowSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql;
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90";	//需要14字节+4字节（xor rax,rax + ret）
	UCHAR fuckcode[]="\x48\x33\xC0\xC3\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";
	ULONG64 pOldAddress = NULL;
	ULONG64 myfun = 0;
	ULONGLONG				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k = &pServiceTable[1];

	pOldAddress = GetShadowSSDTFuncAddr(pServiceTable, nIndex);

	//代理函数地址
	myfun =(ULONG64)newAddress;
	//填充shellcode
	memcpy(jmp_code+6,&myfun,8);
	//写入shellcode
	
	irql=ClearWriteProtect();

	memcpy((PVOID)pOldAddress,fuckcode,23);	//覆盖23个字节【保持指令完整性】
	memcpy((PVOID)(pOldAddress+4),jmp_code,14);
	//修改记录原始地址的地方，[AddressNtUserWindowFromPhysicalPoint+4]开始是一条long jmp，跳转到ProxyNtUserPostMessage
	//ModifySSSDT(IndexOfNtUserPostMessage, AddressNtUserWindowFromPhysicalPoint+4);

	pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)pServiceTable + sizeof(SYSTEM_SERVICE_TABLE));	//4*8
	W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
	qwTemp = W32pServiceTable + 4 * nIndex;
	dwTemp = (LONG)((ULONG64)(pOldAddress + 4) - W32pServiceTable);
	dwTemp = dwTemp << 4;	//DbgPrint("*(PLONG)qwTemp: %x, dwTemp: %x",*(PLONG)qwTemp,dwTemp);

	*(PLONG)qwTemp = dwTemp;

	SetWriteProtect(irql);

	return status;
}
