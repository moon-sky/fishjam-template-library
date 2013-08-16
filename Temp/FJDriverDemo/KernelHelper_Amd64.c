#include "stdafx.h"
#include "FDriverUtil.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"

#define _M_AMD64

#if defined(_M_AMD64)


PSYSTEM_SERVICE_TABLE	g_pSystemServiceTable= NULL;

KIRQL WPOFFx64()
{
	KIRQL irql = KeRaiseIrqlToDpcLevel();  //KeGetCurrentIrql();
	UINT64 cr0;
	_disable();
	cr0 = __readcr0();
	cr0 &= ~0x10000; //0xfffffffffffeffff;
	__writecr0(cr0);
	//_disable(); //需要注释?
	return irql;
}

void WPONx64(KIRQL irql)
{
	UINT64 cr0=__readcr0();
	cr0 |= 0x10000;
	//_enable();
	__writecr0(cr0);
	_enable();
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

	if (g_pSystemServiceTable)
	{
		return g_pSystemServiceTable;
	}

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
				break;
			}
		}
	}
	g_pSystemServiceTable = addr;

	if (g_pSystemServiceTable)
	{
		KdPrint(("g_pSystemServiceTable : %#x, SSDT EntryCount=%d, Shadow EntryCount=%d\n", 
			g_pSystemServiceTable, g_pSystemServiceTable[0].NumberOfServices, 
			g_pSystemServiceTable[1].NumberOfServices));
	}
	else
	{
		KdPrint(("!!! Find Shadow SSDT Failed\n"));
	}

	return g_pSystemServiceTable; 
}

PVOID GetSSDTFuncAddr(LONG nServiceIndex)
{
	//使用 7 个字节保存相对于 g_pSystemServiceTable[1].ServiceTableBase 的地址偏移

	PBYTE				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE pServiceTable = nServiceIndex < 0x1000 ? &g_pSystemServiceTable[0] : &g_pSystemServiceTable[1];

	LONG nIndex = nServiceIndex;
	if (nIndex > 0x1000)
	{
		nIndex -= 0x1000;
	}

	W32pServiceTable=(PBYTE)pServiceTable->ServiceTableBase;

	qwTemp = W32pServiceTable + 4 * nIndex;	//这里是获得偏移地址的位置，要HOOK的话修改这里即可
	dwTemp = *(PLONG)qwTemp;
	dwTemp = dwTemp >> 4;
	qwTemp = W32pServiceTable + (LONG64)dwTemp;
	return qwTemp;
}

//ULONG GetSSDTOffsetAddress(ULONGLONG FuncAddr)
//{
//	ULONG dwtmp=0;
//	PULONG ServiceTableBase=NULL;
//	ServiceTableBase=(PULONG)KeServiceDescriptorTable->ServiceTableBase;
//	dwtmp=(ULONG)(FuncAddr-(ULONGLONG)ServiceTableBase);
//	return dwtmp<<4;
//}


//传入：待HOOK函数地址，代理函数地址，接收原始函数地址的指针，接收补丁长度的指针；返回：原来头N字节的数据
#if 0
PVOID HookKernelApi(IN PVOID ApiAddress, IN PVOID Proxy_ApiAddress, OUT PVOID *Original_ApiAddress, OUT ULONG *PatchSize)
{
	KIRQL irql;
	UINT64 tmpv;
	PVOID head_n_byte,ori_func;
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	UCHAR jmp_code_orifunc[]="\xFF\x25\x00\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
	//How many bytes shoule be patch
	*PatchSize= 14; //GetPatchSize((PUCHAR)ApiAddress);
	//step 1: Read current data
	head_n_byte=kmalloc(*PatchSize);
	irql=WPOFFx64();
	memcpy(head_n_byte,ApiAddress,*PatchSize);
	WPONx64(irql);
	//step 2: Create ori function
	ori_func=kmalloc(*PatchSize+14);	//原始机器码+跳转机器码
	RtlFillMemory(ori_func,*PatchSize+14,0x90);
	tmpv=(ULONG64)ApiAddress+*PatchSize;	//跳转到没被打补丁的那个字节
	memcpy(jmp_code_orifunc+6,&tmpv,8);
	memcpy((PUCHAR)ori_func,head_n_byte,*PatchSize);
	memcpy((PUCHAR)ori_func+*PatchSize,jmp_code_orifunc,14);
	*Original_ApiAddress=ori_func;
	//step 3: fill jmp code
	tmpv=(UINT64)Proxy_ApiAddress;
	memcpy(jmp_code+6,&tmpv,8);
	//step 4: Fill NOP and hook
	irql=WPOFFx64();
	RtlFillMemory(ApiAddress,*PatchSize,0x90);
	memcpy(ApiAddress,jmp_code,14);
	WPONx64(irql);
	//return ori code
	return head_n_byte;
}
#endif

/*
 -- 如果分配trampoline的时候，通过VirtualAlloc限定分配出的内存和old_func在同一个4G，可将限制降低为 5个字节(2+4)
1，找到需要hook的函数地址
2，解析从函数起始地址开始，至少6+8=14个字节的代码。代码不能断开。以上2个过程和detourx86一样，不同的是，detoursx86之需要e9 xxxxxxxx，也就是说只需要5个字节，而我们必须用ff15 [xxxxxxxx]。如果函数体小于14个字节，这意味着该函书无法detours。
不过函数体小于14字节多半是因为里面执行了一个call或者jmp，那么解析该代码，把函数起始地址设置为jmp之后的地址，重新进行2过程。
3，把这14或者15，16...个字节拷贝到预先分配的一块内存中，我们叫它trampoline。
4，把前6个字节改为ff15 [0]，也即ff15 00000000
5，在随后的8个字节中保存new_function的起始地址
6，修正trampoline中的14字节的代码，如果里面有jmp，call等跳转语句，修改偏移量，这时候通常又需要跨4G的跳转，那么按照上面的方法修改之，trampoline的字节数可能会增加。
7，在trampoline的代码之后，插入ff15 [0]，并且在随后的8个字节中填充old_function+14。
*/

NTSTATUS HookSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql = 0;

	pHookApiInfo->pTargetAddress = GetSSDTFuncAddr(pHookApiInfo->nIndexInSSDT);
	NT_ASSERT(pHookApiInfo->pTargetAddress);

	FNT_VERIFY(CreateInlineHook(pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, &pHookApiInfo->pOrigApiAddress,
		&pHookApiInfo->pInlineHookInfo));

	KdPrint(("Inline Hook SSDT func %ws at [0x%x], TargetAddress=%p, newAddress=%p, OrigiApiAddress=%p, nParamCount=%d\n", 
		pHookApiInfo->pwzApiName, pHookApiInfo->nIndexInSSDT, 
		pHookApiInfo->pTargetAddress, pHookApiInfo->pNewApiAddress, 
		pHookApiInfo->pOrigApiAddress, pHookApiInfo->nParamCount));

	return status;
}	

NTSTATUS RestoreSSDTFunc(PHOOK_API_INFO pHookApiInfo)
{
	pHookApiInfo->pOrigApiAddress = NULL;
	RestoreInlineHook(pHookApiInfo->pInlineHookInfo);
	pHookApiInfo->pInlineHookInfo = NULL;
	return STATUS_SUCCESS;
}

#if 0
NTSTATUS HookSSDTFunc(PSYSTEM_SERVICE_TABLE pServiceTable, int nIndex, PVOID newAddress)
{
	NTSTATUS status = STATUS_SUCCESS;
	KIRQL irql;
	//eb93 -- jmp
	UCHAR jmp_code[]="\xFF\x25\x00\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90";	//需要14字节+4字节（xor rax,rax + ret）
	
	UCHAR fuckcode[]="\x48\x33\xC0\xC3\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90";

	ULONG64 OldAddress = NULL;
	//代理函数地址
	ULONG64 myfun = (ULONG64)newAddress;
	ULONGLONG				W32pServiceTable=0, qwTemp=0;
	LONG 					dwTemp=0;
	PSYSTEM_SERVICE_TABLE	pWin32k = &pServiceTable[1];

	OldAddress = GetSSDTFuncAddr(pServiceTable, nIndex);

	//填充shellcode
	memcpy(jmp_code+6,&myfun,8);
	//写入shellcode
	
	irql=WPOFFx64();

	memcpy((PVOID)OldAddress,fuckcode,23);	//覆盖23个字节【保持指令完整性】
	memcpy((PVOID)(OldAddress+4),jmp_code,14);
	//修改记录原始地址的地方，[AddressNtUserWindowFromPhysicalPoint+4]开始是一条long jmp，跳转到ProxyNtUserPostMessage
	//ModifySSSDT(IndexOfNtUserPostMessage, AddressNtUserWindowFromPhysicalPoint+4);

	//pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)pServiceTable + sizeof(SYSTEM_SERVICE_TABLE));	//4*8
	W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
	qwTemp = W32pServiceTable + 4 * nIndex;
	dwTemp = (LONG)((ULONG64)(OldAddress + 4) - W32pServiceTable);
	dwTemp = dwTemp << 4;	
	DbgPrint("*(PLONG)qwTemp: %x, dwTemp: %x",*(PLONG)qwTemp,dwTemp);

	*(PLONG)qwTemp = dwTemp;

	WPONx64(irql);

	return status;
}

#endif //0

#endif //_M_AMD64