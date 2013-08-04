#include "stdafx.h"
#include "KernelDefine.h"
#include "KernelHookAPIHelper.h"


#if defined(_M_IX86)
//typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;
//extern "C" PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;

//extern "C" 
PSYSTEM_DESCRIPTOR_TABLE KeServiceDescriptorTable; 
//extern "C" 
__declspec(dllimport) KeAddSystemServiceTable(ULONG, ULONG, ULONG, ULONG, ULONG); 

//根据相同版本下与SSDT地址存在的偏移获取的SSDT SHADOW的地址
// WinDbg 下 ?KeServiceDescriptorTable-
// 会打印出：Evaluate expression: 64 = 00000040(XP), -0xE0(2K)
//卡巴斯基有个类似的函数：FindSystemServiceDescriptionTableShadow
SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowX86()
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

#endif //_M_IX86


#if defined(_M_AMD64)

ULONGLONG GetKeServiceDescriptorTableShadow64()
{
#if 1
    PUCHAR StartSearchAddress = (PUCHAR)__readmsr(0xC0000082);
    PUCHAR EndSearchAddress = StartSearchAddress + 0x500;
    PUCHAR i = NULL;
    UCHAR b1=0,b2=0,b3=0;
    ULONG templong=0;
    ULONGLONG addr=0;
#if DBG
    //SetSoftBreakPoint();
#endif 

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

ULONGLONG GetSSSDTFuncCurAddr64(ULONGLONG pShadowSSDT,  ULONG64 Index)
{
    ULONGLONG				W32pServiceTable=0, qwTemp=0;
    LONG 					dwTemp=0;
    PSYSTEM_SERVICE_TABLE	pWin32k;
    pWin32k = (PSYSTEM_SERVICE_TABLE)((ULONG64)pShadowSSDT + sizeof(SYSTEM_SERVICE_TABLE));
    W32pServiceTable=(ULONGLONG)(pWin32k->ServiceTableBase);
    //ul64W32pServiceTable = W32pServiceTable;
    qwTemp = W32pServiceTable + 4 * (Index-0x1000);	//这里是获得偏移地址的位置，要HOOK的话修改这里即可
    dwTemp = *(PLONG)qwTemp;
    dwTemp = dwTemp >> 4;
    qwTemp = W32pServiceTable + (LONG64)dwTemp;
    return qwTemp;
}

PVOID GetKeServiceDescriptorTable64()
{
#if DBG
    //SetSoftBreakPoint();
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

#if 0
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

#endif //_M_AMD64