#include "stdafx.h"
#include "FDriverHookAPI.h"

SYS_SERVICE_TABLE *g_pShadowTable = NULL;

int g_ScrollDCIndex = (-1);
HANDLE g_hProcess = 0;

SCROLL_DATA g_ScrollData = {0};
SCROLL_HOOK_TARGET g_ScrollHookTarget = {0};
KAPC_STATE g_KApcState = {0};

//Win32k.sys file, scroll to the target, the target-specific API calls to the process even if one has a value of TRUE.
ULONG g_IsScrolled = FALSE;

// If the current one hooking SSDT API function calls and function call in progress, if this value has a value greater than 0.
ULONG g_SSDTAPILockCount = 0;

//typedef ULONG (*SCROLLWINDOWEX)(HWND hWnd, int dx, int dy, const RECT *prcScroll, const RECT *prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags);
//ULONG (*OrigScrollWindowEx)(HWND hWnd, int dx, int dy, const RECT *prcScroll, const RECT *prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags);

typedef ULONG (*SCROLLDC)(HDC hDC,int dx, int dy, const RECT *lprcScroll, const RECT *lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate);
ULONG (*OrigScrollDC)(HDC hDC,int dx, int dy, const RECT *lprcScroll, const RECT *lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate);

typedef ULONG (*NTBITBLT)(HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
					HDC  hDCSrc,int  XSrc,int  YSrc,	
					ULONG  ROP,ULONG crBackColor,ULONG fl);
ULONG (*OrigNtBitBlt)(HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
					  HDC  hDCSrc,int  XSrc,int  YSrc,	
					  ULONG  ROP,ULONG crBackColor,ULONG fl);

typedef ULONG (*NTGDIEXTTEXTOUTW)(HDC hDC, INT XStart, INT YStart,UINT fuOptions, LPRECT UnsafeRect, LPWSTR UnsafeString, 
								  INT Count, INT* UnsafeDx, ULONG dwCodePage);
ULONG (*OrigNtGdiExtTextOutW)(HDC hDC, INT XStart, INT YStart,UINT fuOptions, LPRECT UnsafeRect, LPWSTR UnsafeString, 
							  INT Count, INT* UnsafeDx, ULONG dwCodePage);


typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;

extern "C" PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
extern "C" __declspec(dllimport) KeAddSystemServiceTable(ULONG, ULONG, ULONG, ULONG, ULONG); 

SYS_SERVICE_TABLE *GetServiceDescriptorShadowTableAddress ()
{ 
	// ShadowTable of obtaining the source code to address what I do not know who the original author is.
	// Almost floating around on the Internet can be seen as the source.

	// First, obtain a pointer to KeAddSystemServiceTable
	unsigned char *check = (unsigned char*)KeAddSystemServiceTable; 
	int i;
	//Initialize an instance of System Service Table, will be used to
	//obtain an address from KeAddSystemServiceTable
	SYS_SERVICE_TABLE *rc=0; 
	// Make 100 attempts to match a valid address with that of KeServiceDescriptorTable 
	for (i=0; i<4096; i++) { 
		__try { 
			// try to obtain an address from  KeAddSystemServiceTable 
			rc = *(SYS_SERVICE_TABLE **)check; 
			// if this address is NOT valid OR it itself is the address of 
			//KeServiceDescriptorTable OR its first entry is NOT equal 
			//to the first entry of KeServiceDescriptorTable 
			if (!MmIsAddressValid (rc) || (rc == (SYS_SERVICE_TABLE *)KeServiceDescriptorTable) 
				|| (memcmp (rc, KeServiceDescriptorTable, sizeof (*rc)) != 0)) { 
					// Proceed with the next address 
					check++; 
					// don't forget to reset the old address 
					rc = 0; 
			} 
		} __except (EXCEPTION_EXECUTE_HANDLER) { rc = 0; } 
		// when the loop is completed, check if it produced a valid address 
		if (rc) 
			// because if it didn't, we failed to find the address of KeServiceDescriptorTableShadow 
			break; 
	} 
	// otherwise, there is a valid address! So return it! 
	return rc; 
}

// It is not part of a fairly clean.
// MDL to approach needs to be changed.
VOID  ClearWriteProtect(VOID)
{
	__asm
	{
		push  eax;
		mov   eax, cr0;
		and   eax, CR0_WP_MASK;
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
		or    eax, not CR0_WP_MASK;
		mov   cr0, eax;
		pop   eax;
	}
}

int GetScrollDCIndex(SYS_SERVICE_TABLE *p)
{
	/*
	1009 0008:A00AA5B4 params=00 NtGdiAnyLinkedFonts 
	100A 0008:A0123725 params=01 NtGdiFontIsLinked 
	100B 0008:A00BB83F params=0A NtGdiArcInternal 
	100C 0008:A0082128 params=01 NtGdiBeginPath 
	100D 0008:A001B344 params=0B NtGdiBitBlt 
	100E 0008:A01234E0 params=01 NtGdiCancelDC 
	100F 0008:A0125354 params=08 NtGdiCheckBitmaPBits 
	1010 0008:A008209C params=01 NtGdiCloseFigure 
	*/
	const int WIN2K_NtGdiAnyLinkedFonts = 0x9;
	for (int i = WIN2K_NtGdiAnyLinkedFonts; i <= WIN2K_NtGdiAnyLinkedFonts + 20; i++)
	{
		KdPrint(("%d", p[1].ArgumentsTable[i] / 4));
		if (
			((p[1].ArgumentsTable[i + 0]) == 0)		//0x9, NtGdiAnyLinkedFonts
			&& ((p[1].ArgumentsTable[i + 1]) == 4)		//0xA, NtGdiFontIsLinked
			&& ((p[1].ArgumentsTable[i + 2]) == 40)		//0xB, NtGdiArcInternal
			&& ((p[1].ArgumentsTable[i + 3]) == 4)		//0xC, NtGdiBeginPath
			&& ((p[1].ArgumentsTable[i + 4]) == 44)		//0xD, NtGdiBitBlt
			&& ((p[1].ArgumentsTable[i + 5]) == 4)		//0xE, NtGdiCancelDC
			&& ((p[1].ArgumentsTable[i + 6]) == 32)		//0xF, NtGdiCheckBitmaPBits
			&& ((p[1].ArgumentsTable[i + 7]) == 4)		//0x10, NtGdiCloseFigure
			)	
		{
			KdPrint(("Find NtGdiBitBlt in Windows XP!!\n"));
			return (i + 4);
		}
	}

#if 0
	int WIN2K_NtUserRemoveMenu = 0;
	int WINDOWS7_RC_NtUserRemoveMenu = 0;
	/*
	//Test 
	[494] BF999A38: BF89C5CC (win32k!NtUserRemoveMenu (bf89c5cc))

	Call Number
	Windows 2000 >> 0x1D9(473) NtUserRemoveMenu
	Windows Vista >> 0x20A(522)	NtUserScrollDC
	*/

	/*
	Windows 7 RC version Function Index
	NtUserResolveDesktop a closer look you can see that the function has disappeared.
	
	NtUserRemoveMenu ( 0x1211 )	4625
	NtUserRemoveProp
	NtUserResolveDesktopForWOW
	NtUserSBGetParms
	NtUserScrollDC
	NtUserScrollWindowEx
	NtUserSelectPalette
	NtUserSendInput
	*/

	// Shadow Table If the operating system on every API is different from the index.
	// n this case, therefore the number of parameters for the selected operating system and that is the basis
	// Create the parameters of the specific array pattern, a pattern scanning through all the operating system is to find the API.
	// This method is how I came up with. Fortunately, there are still problems.
	// Windows 2000 from Windows 2008 Server to have been tested.
	WINDOWS7_RC_NtUserRemoveMenu = 0x211;  //TODO:0x1211? 4625
	int i;

	if (((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu]) == 12) // NtUserRemoveMenu
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 1]) == 8) // NtUserRemoveProp
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 2]) == 4) // NtUserResolveDesktopForWOW
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 3]) == 16) // NtUserSBGetParms
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 4]) == 28) // NtUserScrollDC
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 5]) == 32) // NtUserScrollWindowEx
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 6]) == 12) // NtUserSelectPalette
			&& ((p[1].ArgumentsTable[WINDOWS7_RC_NtUserRemoveMenu + 7]) == 12) // NtUserSendInput
		)
	{
		KdPrint(("Windows 7 RC!!\n"));
		return (WINDOWS7_RC_NtUserRemoveMenu + 4);
	}

	/*
	Windows 2000 Order parameters and call
	A0069191  	03  	NtUserRemoveMenu
	A0063B65  	02  	NtUserRemoveProp
	A009A4D9  	04  	NtUserResolveDesktop
	A00E2A8C  	01  	NtUserResolveDesktopForWOW
	A00B76FB  	04  	NtUserSBGetParms
	A0040899  	07  	NtUserScrollDC
	A00ABB92  	08  	NtUserScrollWindowEx
	A002895B  	03  	NtUserSelectPalette
	A0088DAD  	03  	NtUserSendInput
	*/
	WIN2K_NtUserRemoveMenu = 0x1D9; //473
	for (i = WIN2K_NtUserRemoveMenu; i <= 0x20a; i++)  //522
	{
		KdPrint(("%d", p[1].ArgumentsTable[i] / 4));
		if (
			   ((p[1].ArgumentsTable[i + 0]) == 12)		//0x1D9, NtUserRemoveMenu
			&& ((p[1].ArgumentsTable[i + 1]) == 8)		//0x1DA, NtUserRemoveProp
			&& ((p[1].ArgumentsTable[i + 2]) == 16)		//0x1DB, NtUserResolveDesktop
			&& ((p[1].ArgumentsTable[i + 3]) == 4)		//0x1DC, NtUserResolveDesktopForWOW
			&& ((p[1].ArgumentsTable[i + 4]) == 16)		//0x1DD, NtUserSBGetParms
			&& ((p[1].ArgumentsTable[i + 5]) == 28)		//0x1DE, NtUserScrollDC
			&& ((p[1].ArgumentsTable[i + 6]) == 32)
			&& ((p[1].ArgumentsTable[i + 7]) == 12)
			&& ((p[1].ArgumentsTable[i + 8]) == 12)
			)
		{
			KdPrint(("Windows XP!!\n"));
			return (i + 5);
		}
	}
#endif 
	return (-1);
}

ULONG MyScrollDC(HDC hDC, int dx, int dy, const RECT *lprcScroll, const RECT *lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate)
{
	ULONG uResult = OrigScrollDC(hDC, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate);
	KdPrint(("MyScrollDC hDC=0x%x \n", hDC));

	HANDLE PID = PsGetCurrentProcessId();
	if (PID == g_ScrollHookTarget.hTargetProcess)
	{
	}
	return uResult;
}

ULONG MyNtGdiBitBlt(
					 HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
					 HDC  hDCSrc,int  XSrc,int  YSrc,	
					 ULONG  ROP,ULONG crBackColor,ULONG fl)
{
	static LONG nCount = 0;
	ULONG uResult = OrigNtBitBlt(hDCDest, XDest, YDest, Width, Height, hDCSrc, XSrc, YSrc, ROP, crBackColor, fl);

	
	InterlockedIncrement(&nCount);
	if (nCount % 20 == 0)
	{
		KdPrint(("In MyNtGdiBitBlt hDCDest=0x%x, hDCDest=0x%x\n", hDCDest, hDCSrc));
	}
	return uResult;
}

//ULONG MyScrollWindowEx(HWND hWnd, int dx, int dy, const RECT *prcScroll, const RECT *prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags)
//{
//	KdPrint(("MyScrollWindowEx hWnd=0x%x,\n", hWnd));
//
//	RECT rcScroll;
//	ULONG uResult = (OrigScrollWindowEx(hWnd, dx, dy, prcScroll, prcClip, hrgnUpdate, prcUpdate, flags));
//	HANDLE PID = PsGetCurrentProcessId();
//	if ((PID == g_ScrollHookTarget.hTargetProcess)
//		&& (hWnd == g_ScrollHookTarget.hTargetWindow))
//	{
//	}
//	return uResult;
//}

void InstallCopyProtectHook(HANDLE hProcess)
{
	PAGED_CODE();

	ClearWriteProtect();

	KdPrint(("IRQL : %#x", KeGetCurrentIrql()));

	g_pShadowTable = GetServiceDescriptorShadowTableAddress();
	if (g_pShadowTable == NULL)
	{
		KdPrint(("Failed GetShadowTable"));
	}
	else
	{
		PEPROCESS pEProcess;

		KdPrint(("ServiceDescriptorShadowTableAddress : %#x\n", g_pShadowTable));
		KdPrint(("PID %u\n", hProcess));

		PsLookupProcessByProcessId(hProcess, &pEProcess);
		if (pEProcess == NULL)
		{
			SetWriteProtect();

			return ;
		}
		KeStackAttachProcess(pEProcess, &g_KApcState);
		//KeAttachProcess(pEProcess);

		g_ScrollDCIndex = GetScrollDCIndex(g_pShadowTable);
		KdPrint(("g_ScrollDCIndex: %#x", g_ScrollDCIndex));

		g_hProcess = hProcess;
		if (g_ScrollDCIndex != (-1))
		{
			//
			// This method is not accessible.
			// This is very extremely low chance of a blue screen may occur. (A very unlikely possibility, but ...)
			// OrigScrollDC = InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]), MyScrollDC);
			//
			OrigScrollDC = (SCROLLDC)(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]);
			//InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]), MyScrollDC);

			OrigNtBitBlt = (NTBITBLT)(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]);
			InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]), MyNtGdiBitBlt);

			//OrigScrollWindowEx = (SCROLLWINDOWEX)(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex + 1]);
			//InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex + 1]), MyScrollWindowEx);
		}

		KeDetachProcess();
	}
	SetWriteProtect();
}

void UnInstallScrollHook(void)
{
	LARGE_INTEGER WaitTime;
	KdPrint(("Enter UnInstallScrollHook\n"));

	ClearWriteProtect();

	if (g_pShadowTable != NULL)
	{
		KdPrint(("ServiceDescriptorShadowTableAddress : %#x", g_pShadowTable));

		if (g_hProcess != 0)
		{
			if (g_ScrollDCIndex != (-1))
			{
				PEPROCESS pEProcess;

				PsLookupProcessByProcessId(g_hProcess, &pEProcess);

				KeUnstackDetachProcess(&g_KApcState);
				//KeAttachProcess(pEProcess);

				//g_pShadowTable[1].ServiceTable[g_ScrollDCIndex] = OrigScrollDC;
				//InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]), OrigScrollDC);
				//g_pShadowTable[1].ServiceTable[g_ScrollDCIndex + 1] = OrigScrollWindowEx;
				//InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex + 1]), OrigScrollWindowEx);

				InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_ScrollDCIndex]), OrigNtBitBlt);

				KeDetachProcess();

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

	DbgPrint("OpenCapture ScrollHook Driver UnLoaded\n");
}