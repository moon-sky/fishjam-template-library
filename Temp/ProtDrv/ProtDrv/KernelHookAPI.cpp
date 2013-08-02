#include "stdafx.h"
#include "ASM/AsmHelperFun.h"
#include "KernelHookAPI.h"


ULONGLONG GetKeServiceDescriptorTableShadow64()
{
#if 0
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
				return addr;
			}
		}
	}
#endif 
	return 0;
}


PVOID GetKeServiceDescriptorTable64()
{
#if DBG
	SetSoftBreakPoint();
#endif

#if 0
	//fffff800`0288dff2 4c8d1547782300  lea     r10,[nt!KeServiceDescriptorTable (fffff800`02ac5840)]
	//fffff800`0288dff9 4c8d1d80782300  lea     r11,[nt!KeServiceDescriptorTableShadow (fffff800`02ac5880)]
	char KiSystemServiceStart_pattern[15] = "\x4c\x8d\x15\x47\x78\x23\x00\x4c\x8d\x1d\x80\x78\x23\x00";

	PBYTE pRet = NULL;
	PBYTE pFind = NULL;

	PBYTE CodeScanStart =  (PBYTE)GetKiSystemCall64Address();
	if (CodeScanStart)
	{
		for (int i = 0; i < 2048; i++)
		{
			__try { 
				pFind = CodeScanStart + i;
				if (MmIsAddressValid(pFind) 
					&& 0 == memcmp(pFind, (char*)KiSystemServiceStart_pattern, 15))
				{
					KdPrint(("Find KiSystemServiceStart_pattern,i=%d, pFind=0x%p\n", 
						i, pFind));

					pRet = *( (UINT32*)( pFind + 3 ) )  +  ( pFind + 7 );
					break;
				}

			}__except (EXCEPTION_EXECUTE_HANDLER) 
			{
				KdPrint(("Except in GetKeServiceDescriptorTable64, i = %d\n", i));
				break;
			}
		}
	}

	KdPrint(("GetKeServiceDescriptorTable64 return pRet=0x%p\n", pRet));

	return pRet;
#endif 

#if 1
	char KiSystemServiceStart_pattern[14] = "\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x25\xFF\x0F\x00\x00";

	ULONGLONG CodeScanStart = (ULONGLONG)&_strnicmp;
	ULONGLONG CodeScanEnd = (ULONGLONG)&KdDebuggerNotPresent;

	UNICODE_STRING Symbol;
	ULONGLONG i, tbl_address, b;

	__try
	{
		for (i = 0; i < CodeScanEnd - CodeScanStart; i++)
		{
			if (MmIsAddressValid((char*)(ULONGLONG)CodeScanStart +i))
			{
				if (!memcmp((char*)(ULONGLONG)CodeScanStart +i, (char*)KiSystemServiceStart_pattern,14))
				{ 
					for (b = 0; b < 50; b++)
					{
						tbl_address = ((ULONGLONG)CodeScanStart+i+b);

						if (*(USHORT*) ((ULONGLONG)tbl_address ) == (USHORT)0x8d4c)
							return (PVOID)(((LONGLONG)tbl_address +7) + *(LONG*)(tbl_address +3));
					}
				}
			}
		}
	}__except (EXCEPTION_EXECUTE_HANDLER) 
	{
		KdPrint(("Except in GetKeServiceDescriptorTable64, i = %d\n", i));
	}

	return NULL;
#endif


#if 0
	//实测时 Win7 64 上范围不对，会core
	//http://www.rohitab.com/discuss/topic/36432-x64-ssdt-hook/

	//WINDOWS 64位SSDT定位思路 --  http://blog.csdn.net/lziog/article/details/6059409

	//Pattern
	char KiSystemServiceStart_pattern[14] = "\x8B\xF8\xC1\xEF\x07\x83\xE7\x20\x25\xFF\x0F\x00\x00";
	8BF8C1EF0783E72025FF0F0000

		//Scan boundaries
		ULONGLONG CodeScanStart = (ULONGLONG)&KeNumberProcessors;
	ULONGLONG CodeScanEnd = (ULONGLONG)&KdDebuggerNotPresent;
	//Another needed variables
	UNICODE_STRING Symbol;
	ULONGLONG i, tbl_address, b;
	//Loop - to find the KiSystemServiceStart function
	for (i = 0; i < CodeScanEnd - CodeScanStart; i++)
	{
		//Check if those bytes are equal to our pattern-bytes
		if (!memcmp((char*)(ULONGLONG)CodeScanStart +i, (char*)KiSystemServiceStart_pattern,14))
		{
			//Search lea rdx, * - by opcodes: 4c 8d
			for (b = 0; b < 50; b++)
			{
				tbl_address = ((ULONGLONG)CodeScanStart+i+b);
				//Check for lea rdx, * and calculate base address from relative address
				if (*(USHORT*) ((ULONGLONG)tbl_address ) == (USHORT)0x8d4c)
					return ((LONGLONG)tbl_address +7) + *(LONG*)(tbl_address +3);
			}
		}
	}
#endif 

	return 0;
}