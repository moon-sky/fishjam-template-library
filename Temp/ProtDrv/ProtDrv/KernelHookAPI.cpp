#include "stdafx.h"
#include "ASM/AsmHelperFun.h"
#include "KernelDefine.h"
#include "KernelHookAPIHelper.h"
#include "KernelHookAPI.h"
#include "FDriverUtil.h"

#define ObjectNameInformation  1
#define SystemHandleInformation 0x10
#define HOOK_API_INFO_TAG     (ULONG)0x12345678

SYSTEM_SERVICE_TABLE *g_pShadowTable = NULL;
// If the current one hooking SSDT API function calls and function call in progress, if this value has a value greater than 0.
ULONG g_SSDTAPILockCount = 0;


typedef NTSTATUS (*NTGDIBITBLT)(HDC hDCDest,int  XDest,int  YDest, int  Width,int  Height,
						  HDC  hDCSrc,int  XSrc,int  YSrc,	
						  ULONG  ROP,ULONG crBackColor,ULONG fl);

typedef HDC (*NTGDIOPENDCW)( PUNICODE_STRING Device,
					 DEVMODEW *InitData,
					 PUNICODE_STRING pustrLogAddr,
					 ULONG iType,
					 HANDLE hspool,
					 VOID *pDriverInfo2,
					 VOID *pUMdhpdev );

typedef BOOL (*NTGDIDELETEOBJECTAPP)(HDC  DCHandle);


typedef NTSTATUS (*NTUSERCALLONEPARAM)(IN ULONG Param, IN ULONG Routine);

#define MAX_TRACE_CREATE_DISPLAY_DC_COUNT	1

typedef struct _DRIVER_HOOK_API_INFOS
{
public:
	HWND					m_hWndDesktop;
    HWND					m_hWndProtect;
    unsigned long			m_OldCr0;
    BOOL                    m_ValidCallNumber;
	HDC						m_hCreatedDisplayDC[MAX_TRACE_CREATE_DISPLAY_DC_COUNT];
	//UNICODE_STRING			m_UnicodeString_DISPLAY;
    PROTECT_WND_INFO        m_ProtectWndInfo;
public:
    int						IndexOfNtGdiBitBlt;
	int						IndexOfNtGdiOpenDCW;
    int                     IndexOfNtUserCallOneParam;
	int						IndexOfNtGdiDeleteObjectApp;

	int						ONEPARAM_ROUTINE_WINDOWFROMDC;

	NTGDIBITBLT				pOrigNtGdiBitBlt;
	NTGDIOPENDCW			pOrigNtGdiOpenDCW;
	NTGDIDELETEOBJECTAPP	pOrigNtGdiDeleteObjectApp;


    NTUSERCALLONEPARAM      pOrigNtUserCallOneParam;


	VOID InitValue()
	{
		m_hWndDesktop = (HWND)0;

        m_hWndProtect = (HWND)0;
        m_OldCr0 = 0;
        m_ValidCallNumber = FALSE;
		for (int i = 0; i < MAX_TRACE_CREATE_DISPLAY_DC_COUNT; i++ )
		{
			m_hCreatedDisplayDC[i] = NULL;
		}

		IndexOfNtGdiBitBlt = -1;
		IndexOfNtGdiOpenDCW = -1;
        IndexOfNtUserCallOneParam = -1;
		IndexOfNtGdiDeleteObjectApp = -1;

		ONEPARAM_ROUTINE_WINDOWFROMDC = -1;

		pOrigNtGdiBitBlt = NULL;
		pOrigNtGdiOpenDCW = NULL;
		pOrigNtGdiDeleteObjectApp = NULL;

        pOrigNtUserCallOneParam = NULL;

		//RtlInitUnicodeString(&m_UnicodeString_DISPLAY, L"\\\\.\\DISPLAY1");// L"DISPLAY");
	}
	BOOL AddCreatedDisplayDC(HDC hDC)
	{
		BOOL bRet = FALSE;
		if (hDC)
		{
			for (int i = 0; i < MAX_TRACE_CREATE_DISPLAY_DC_COUNT; i++)
			{
				if (NULL == m_hCreatedDisplayDC[i])
				{
					m_hCreatedDisplayDC[i] = hDC;
					bRet = TRUE;
					break;
				}
			}
		}
		if (!bRet)
		{
			KdPrint(("AddCreatedDisplayDC for HDC=0x%x FAIL\n", hDC));
		}
		return bRet;
	}
	BOOL IsCreatedDisplayDC(HDC hDC)
	{
		BOOL bRet = FALSE;
		if (hDC)
		{
			for (int i = 0; i < MAX_TRACE_CREATE_DISPLAY_DC_COUNT; i++)
			{
				if (hDC == m_hCreatedDisplayDC[i])
				{
					bRet = TRUE;
					break;
				}
			}
		}
		return bRet;
	}
	BOOL RemoveCreatedDisplayDC(HDC hDC)
	{
		BOOL bRet = FALSE;
		if (hDC)
		{
			for (int i = 0; i < MAX_TRACE_CREATE_DISPLAY_DC_COUNT; i++)
			{
				if (hDC == m_hCreatedDisplayDC[i])
				{
					m_hCreatedDisplayDC[i] = NULL;
					bRet = TRUE;
					break;
				}
			}
		}
		return bRet;
	}

	VOID FinalizeValue()
	{
		
	}

    BOOL HasValidCallNumber()
    {
        return m_ValidCallNumber;
    }

	//根据操作系统来确定具体函数的服务号
	NTSTATUS InitCallNumber()
	{
		NT_ASSERT(g_pShadowTable);

		NTSTATUS status = STATUS_SUCCESS;
		ULONG majorVersion = 0;
		ULONG minorVersion = 0;
		PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );

		ULONG versionCode = majorVersion * 100 + minorVersion;
		KdPrint(("PsGetVersion, version is %d\n", versionCode));
		switch (versionCode)
		{
		case 501:	//WinXp
			{
                //ShadowSSDT_array_xp3 -- http://bbs.pediy.com/showthread.php?t=116044&highlight=NtGdiBitBlt
				KdPrint(("Running on Windows XP\n"));
				IndexOfNtGdiBitBlt = 0xD;				//13,  100D 0008:A001B344 params=0B NtGdiBitBlt 
				IndexOfNtGdiOpenDCW = 0xe9;				//0x10e9 	 0x10e9 	 0x10e9 	
				IndexOfNtGdiDeleteObjectApp = 0x7a;		//0x107a
                IndexOfNtUserCallOneParam = 0x143;      //323;

				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f;	//31
				break;
			}
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7\n"));
				IndexOfNtGdiBitBlt = 0xE;
				IndexOfNtUserCallOneParam = 0x14e;

				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x24;	//Win7
				break;
			}
        case 0xFFF: //win8 x86 fre 8319 -- http://bbs.pediy.com/showthread.php?t=149861&highlight=NtGdiBitBlt
            {
                IndexOfNtGdiBitBlt = 0x12e;  //0x12e    NtGdiBitBlt
            }
		default:
			status = STATUS_NOT_SUPPORTED;
			break;
		}

		if (NT_SUCCESS(status))
		{
			int nPointSize = sizeof(void *);
            m_ValidCallNumber = TRUE;

#if defined(_M_IX86)
			NT_ASSERT(nPointSize == 4);
#elif defined(_M_AMD64) || defined(_M_IA64)
            NT_ASSERT(nPointSize == 8);
#endif 
            m_ValidCallNumber &= (g_pShadowTable[1].ArgumentsTable[IndexOfNtGdiBitBlt] == (0xB * nPointSize));
			NT_ASSERT(m_ValidCallNumber);

            
            m_ValidCallNumber &= (g_pShadowTable[1].ArgumentsTable[IndexOfNtUserCallOneParam] == (0x2 * nPointSize));
            NT_ASSERT(m_ValidCallNumber);

			m_ValidCallNumber &= (g_pShadowTable[1].ArgumentsTable[IndexOfNtGdiOpenDCW] == (0x7 * nPointSize));
            NT_ASSERT(m_ValidCallNumber);

			m_ValidCallNumber &= (g_pShadowTable[1].ArgumentsTable[IndexOfNtGdiDeleteObjectApp] == (0x1 * nPointSize));
            NT_ASSERT(m_ValidCallNumber);
		}

		return status;
	}
}DRIVER_HOOK_API_INFOS, *PDRIVER_HOOK_API_INFOS;


//#pragma LOCKEDCODE	
DRIVER_HOOK_API_INFOS* g_pDriverHookApiInfos = NULL;
HANDLE g_hProcess = 0;


#if defined(_M_IX86)
// It is not part of a fairly clean.
// MDL to approach needs to be changed.

VOID  ClearWriteProtect(VOID)
{
    //cli + sti 类似 push eax + pop eax
    __asm
    {
        push  eax;
        mov   eax, cr0;
        and   eax, not 10000h;		//and     eax, not 10000h
        mov   cr0, eax;
        pop   eax;
    }
}

VOID  SetWriteProtect(VOID)
{
	__asm
	{
		push  eax;
		mov   eax, cr0;
		or    eax, 10000h;  //or eax, 10000h
		mov   cr0, eax;
		pop   eax;
	}
}
#else //_M_AMD64, _M_IA64
  extern "C" void __stdcall ClearWriteProtect();
  extern "C" void __stdcall SetWriteProtect();
#endif //

ULONG MyNtGdiBitBlt(
					 HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
					 HDC hDCSrc, int  XSrc, int  YSrc,	
					 ULONG  ROP, ULONG crBackColor,ULONG fl)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG nResult = TRUE;

    HWND hWndDestFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCDest, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);
    HWND hWndSrcFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCSrc, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);


	BOOL bWillPrevent = FALSE;
	if (hWndDestFromDC == g_pDriverHookApiInfos->m_hWndDesktop
		|| hWndSrcFromDC == g_pDriverHookApiInfos->m_hWndDesktop
		|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hDCSrc))
	{
		//Skip 
		//KdPrint(("[%d], Bitblt for Desktop Wnd\n"));
		KdPrint(("[%d], !!!! In MyNtGdiBitBlt, hWndDesktop=0x%x, hWndDestFromDC=0x%x, hDCDest=0x%x, hWndSrcFromDC=0x%x, hDCSrc=0x%x, nResult=%d\n", 
			PsGetCurrentProcessId(),
			g_pDriverHookApiInfos->m_hWndDesktop, hWndDestFromDC, hDCDest, 
			hWndSrcFromDC, hDCSrc, nResult));
		bWillPrevent = TRUE;
	}
	else
	{
		nResult = g_pDriverHookApiInfos->pOrigNtGdiBitBlt(hDCDest, XDest, YDest, Width, Height, hDCSrc, XSrc, YSrc, ROP, crBackColor, fl);
	}
	if (bWillPrevent)
	{
		RECT rcClient = {XDest, YDest, XDest + Width, YDest + Height};
	}
	return nResult;
}

HDC MyNtGdiOpenDCW( PUNICODE_STRING Device,
							DEVMODEW *InitData,
							PUNICODE_STRING pustrLogAddr,
							ULONG iType,
							HANDLE hspool,
							VOID *pDriverInfo2,
							VOID *pUMdhpdev )
{
	HDC hDC = g_pDriverHookApiInfos->pOrigNtGdiOpenDCW(Device, InitData, pustrLogAddr, iType, hspool, pDriverInfo2, pUMdhpdev);

	KdPrint(("[%d], In MyNtGdiOpenDCW Device=%wZ, hDC=0x%x\n", 
		PsGetCurrentProcessId(), Device, hDC));

	if (!Device )
		//|| (RtlEqualUnicodeString(Device, &g_pDriverHookApiInfos->m_UnicodeString_DISPLAY, TRUE)))
	{
		g_pDriverHookApiInfos->AddCreatedDisplayDC(hDC);
	}
	return hDC;
}

BOOL MyNtGdiDeleteObjectApp(HDC  DCHandle)
{

	BOOL bRet = g_pDriverHookApiInfos->RemoveCreatedDisplayDC(DCHandle);
	if (bRet)
	{
		KdPrint(("[%d], In MyNtGdiDeleteObjectApp hDC=0x%x\n", 
			PsGetCurrentProcessId(), DCHandle));
	}
	bRet = g_pDriverHookApiInfos->pOrigNtGdiDeleteObjectApp(DCHandle);
	return bRet;
}


NTSTATUS SetProtectWndInfo(PPROTECT_WND_INFO pProtectWndInfo)
{
    NTSTATUS status = STATUS_SUCCESS;
	//PAGED_CODE();

#if DBG
    //_asm int 3
#endif

    if (pProtectWndInfo)
    {

    }
    return status;
}


NTSTATUS InstallHook(HANDLE hProcessId)
{
    NTSTATUS status = STATUS_SUCCESS;
	KdPrint(("Enter InstallHook, hProcessId=%d\n"));

    if (!g_pDriverHookApiInfos)
    {
        g_pDriverHookApiInfos = (PDRIVER_HOOK_API_INFOS)ExAllocatePoolWithTag(NonPagedPoolCacheAlignedMustS, sizeof(DRIVER_HOOK_API_INFOS), HOOK_API_INFO_TAG);
        NT_ASSERT(g_pDriverHookApiInfos);
        if (g_pDriverHookApiInfos)
        {
            RtlZeroMemory(g_pDriverHookApiInfos, sizeof(DRIVER_HOOK_API_INFOS));
            g_pDriverHookApiInfos->InitValue();
            //g_pDriverHookApiInfos->m_hWndDesktop = pProtectWndInfo->hWndDesktop;

            ClearWriteProtect();

            //KdPrint(("IRQL : %#x", KeGetCurrentIrql()));

            g_pShadowTable = GetKeServiceDescriptorTableShadowX86();
            if (g_pShadowTable == NULL)
            {
                KdPrint(("Failed GetShadowTable\n"));
            }
            else
            {
                KdPrint(("KeServiceDescriptorTableShadow : %#x, SSDT EntryCount=%d, Shadow EntryCount=%d, PID=%d\n",   //%#x 也是地址?
                    g_pShadowTable, g_pShadowTable[0].NumberOfServices, g_pShadowTable[1].NumberOfServices, hProcessId));

                PEPROCESS pEProcess = NULL;

                FNT_VERIFY(PsLookupProcessByProcessId(hProcessId, &pEProcess));
                if (pEProcess == NULL)
                {
                    //SetWriteProtect();
                    g_pDriverHookApiInfos->FinalizeValue();
                    ExFreePoolWithTag(g_pDriverHookApiInfos, HOOK_API_INFO_TAG);
                    g_pDriverHookApiInfos = NULL;
                    return status;
                }
				//ObDereferenceObject(pEProcess);

                KAPC_STATE KApcState = {0};
                KeStackAttachProcess(pEProcess, &KApcState);
                //KeAttachProcess(pEProcess);

                FNT_VERIFY(g_pDriverHookApiInfos->InitCallNumber());
                if (NT_SUCCESS(status))
                {
                    g_hProcess = hProcessId;
                    
                    g_pDriverHookApiInfos->pOrigNtGdiBitBlt = (NTGDIBITBLT)(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiBitBlt]);// g_NtGdiBitBltIndex]);
                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiBitBlt]), MyNtGdiBitBlt);

                    g_pDriverHookApiInfos->pOrigNtGdiOpenDCW = (NTGDIOPENDCW)(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiOpenDCW]);
                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiOpenDCW]), MyNtGdiOpenDCW);

                    g_pDriverHookApiInfos->pOrigNtGdiDeleteObjectApp = (NTGDIDELETEOBJECTAPP)(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiDeleteObjectApp]);
                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiDeleteObjectApp]), MyNtGdiDeleteObjectApp);

                    g_pDriverHookApiInfos->pOrigNtUserCallOneParam = (NTUSERCALLONEPARAM)(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtUserCallOneParam]);
                }
                KeUnstackDetachProcess(&KApcState);
            }
            SetWriteProtect();
        }
    }
	KdPrint(("Leave InstallHook, status=0x%x\n", status));

    return status;
}
NTSTATUS UnInstallHook()
{
    NTSTATUS status = STATUS_SUCCESS;

    LARGE_INTEGER WaitTime;
    KdPrint(("Enter UnInstallScrollHook\n"));

    if (g_pDriverHookApiInfos)
    {
        ClearWriteProtect();

        if (g_pShadowTable != NULL)
        {
            KdPrint(("ServiceDescriptorShadowTableAddress : %#x", g_pShadowTable));

            if (g_hProcess != 0)
            {
                if (g_pDriverHookApiInfos->HasValidCallNumber())// g_pDriverHookApiInfos-> g_NtGdiBitBltIndex != (-1))
                {
                    PEPROCESS pEProcess = NULL;

                    FNT_VERIFY(PsLookupProcessByProcessId(g_hProcess, &pEProcess));
					KAPC_STATE KApcState = {0};
					KeStackAttachProcess(pEProcess, &KApcState);

					//ObDereferenceObject(pEProcess);

					InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiBitBlt]), g_pDriverHookApiInfos->pOrigNtGdiBitBlt);
					InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiOpenDCW]), g_pDriverHookApiInfos->pOrigNtGdiOpenDCW);
					InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTableBase[g_pDriverHookApiInfos->IndexOfNtGdiDeleteObjectApp]), g_pDriverHookApiInfos->pOrigNtGdiDeleteObjectApp);

					g_pDriverHookApiInfos->pOrigNtUserCallOneParam = NULL;

                    KeUnstackDetachProcess(&KApcState);
                }
            }
        }
        SetWriteProtect();

        WaitTime.QuadPart = -1 * 10 * 1000 * 1000;
        while (g_SSDTAPILockCount > 0)
        {
            // This is hooking the SSDT API during execution.
            // To release hooking InterlockedExchangePointer function, but the hooking off
            // That the function is called just before the MyScrollDC same API is called, the call back then if InterlockedExchangePointer
            // The state of the call is already MyScrollDC.
            // Therefore, when the driver is unloaded DRIVER_UNLOADED_WITHOUT_CANCELLING_PENDING_OPERATIONS might cause a blue screen to prevent it.
            KdPrint(("g_SSDTAPILockCount : %u\n", g_SSDTAPILockCount));

            // After waiting for 1 second to re-check Lock the pool ryeotneunji.
            WaitTime.QuadPart = -1 * 10 * 1000 * 1000;
            KeDelayExecutionThread(KernelMode ,FALSE, &WaitTime);	

            // NOTE! Lock Lock is loose again after that check may take a while.
            // Increment the counter because the Lock function call that part is because just to increase.
            // As a function of the fact nacked make parts, but also affects the stability awake, so it is not recommended.
        }
        // After waiting for 1 second again to allow the driver to unload.
        WaitTime.QuadPart = -1 * 10 * 1000 * 1000;
        KeDelayExecutionThread(KernelMode ,FALSE, &WaitTime);	


        ExFreePoolWithTag(g_pDriverHookApiInfos, HOOK_API_INFO_TAG);
        g_pDriverHookApiInfos = NULL;
    }
    DbgPrint("Protected Driver UnLoaded\n");

    return status;

}
