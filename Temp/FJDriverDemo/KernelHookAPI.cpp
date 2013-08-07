#include "stdafx.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"
#include "FDriverUtil.h"
#include "FDriverDemoDefine.h"

#define ObjectNameInformation  1
#define SystemHandleInformation 0x10
#define HOOK_API_INFO_TAG     (ULONG)0x12345678

SYSTEM_SERVICE_TABLE *g_pShadowTable = NULL;

typedef BOOL (*NTGDIBITBLT)(HDC hDCDest,int  XDest,int  YDest, int  Width,int  Height,
						  HDC  hDCSrc,int  XSrc,int  YSrc,	
						  ULONG  ROP,ULONG crBackColor,ULONG fl);

typedef BOOL (*NTGDIEXTTEXTOUTW)(IN HDC 	hDC,
                                     IN INT 	XStart,
                                     IN INT 	YStart,
                                     IN UINT 	fuOptions,
                                     IN OPTIONAL LPRECT 	UnsafeRect,
                                     IN LPWSTR 	UnsafeString,
                                     IN INT 	Count,
                                     IN OPTIONAL LPINT 	UnsafeDx,
                                     IN DWORD 	dwCodePage);

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

BOOL Hooked_NtGdiBitBlt(
						HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
						HDC hDCSrc, int  XSrc, int  YSrc,	
						ULONG  ROP, ULONG crBackColor,ULONG fl);
HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING Device,
						DEVMODEW *InitData,
						PUNICODE_STRING pustrLogAddr,
						ULONG iType,
						HANDLE hspool,
						VOID *pDriverInfo2,
						VOID *pUMdhpdev );
BOOL Hooked_NtGdiDeleteObjectApp(HDC  DCHandle);


typedef struct _DRIVER_HOOK_API_INFOS
{
public:
	HWND					m_hWndDesktop;
    HWND					m_hWndProtect;
    unsigned long			m_OldCr0;
    BOOL                    m_ValidCallNumber;
	HDC						m_hCreatedDisplayDC[MAX_TRACE_CREATE_DISPLAY_DC_COUNT];
	//UNICODE_STRING			m_UnicodeString_DISPLAY;
public:
	HOOK_API_INFO			hookNtGdiBitBlt;
	HOOK_API_INFO			hookNtGdiOpenDCW;
	HOOK_API_INFO			hookNtGdiDeleteObjectApp;

    int                     IndexOfNtUserCallOneParam;
	int						ONEPARAM_ROUTINE_WINDOWFROMDC;

	ULONG					nPatchSizeGdiBltBlt;
	
    NTUSERCALLONEPARAM      pOrigNtUserCallOneParam;

	VOID InitValue()
	{
        m_ValidCallNumber = FALSE;
		for (int i = 0; i < MAX_TRACE_CREATE_DISPLAY_DC_COUNT; i++ )
		{
			m_hCreatedDisplayDC[i] = NULL;
		}
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

	NTSTATUS _InitCallNumberX86(ULONG versionCode)
	{
		NTSTATUS status = STATUS_SUCCESS;
		switch (versionCode)
		{
		case 501:	//WinXp
			{
				//ShadowSSDT_array_xp3 -- http://bbs.pediy.com/showthread.php?t=116044&highlight=NtGdiBitBlt
				KdPrint(("Running on Windows XP\n"));
				hookNtGdiBitBlt.nIndexInSSDT = 0xD;				//13,  100D 0008:A001B344 params=0B NtGdiBitBlt 
				hookNtGdiOpenDCW.nIndexInSSDT = 0xe9;
				hookNtGdiDeleteObjectApp.nIndexInSSDT = 0x7a;

				IndexOfNtUserCallOneParam = 0x143;      //323;
				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f;	//31
				break;
			}
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7\n"));
				hookNtGdiBitBlt.nIndexInSSDT = 0xE;
				hookNtGdiOpenDCW.nIndexInSSDT = 0xf3;
				hookNtGdiDeleteObjectApp.nIndexInSSDT = 0x7d;

				IndexOfNtUserCallOneParam = 0x14e;
				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x24;	//Win7
				break;
			}
		default:
			status = STATUS_NOT_SUPPORTED;
			break;
		}

		if (NT_SUCCESS(status))
		{
			int nPointSize = sizeof(void *);
			KdPrint(("Pointer Size is %d\n", nPointSize));
			m_ValidCallNumber = TRUE;
		}
		return status;
	}

	NTSTATUS _InitCallNumberX64(ULONG versionCode)
	{
		NTSTATUS status = STATUS_SUCCESS;
		switch (versionCode)
		{
		case 501:	//WinXp
			{
				KdPrint(("Running on Windows XP 64\n"));

				NT_ASSERT(FALSE);
				break;
			}
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7 64\n"));

				hookNtGdiBitBlt.nIndexInSSDT = 0x8;
				hookNtGdiOpenDCW.nIndexInSSDT = 0xda;
				hookNtGdiDeleteObjectApp.nIndexInSSDT = 0x23;

				IndexOfNtUserCallOneParam = 0x2;
				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x24;	//Win7
				break;
			}
		default:
			status = STATUS_NOT_SUPPORTED;
			break;
		}

		if (NT_SUCCESS(status))
		{
			int nPointSize = sizeof(void *);
			KdPrint(("Pointer Size is %d\n", nPointSize));
			m_ValidCallNumber = TRUE;
		}
		return status;
	}

	//根据操作系统来确定具体函数的服务号
	NTSTATUS InitCallNumber()
	{
		NTSTATUS status = STATUS_SUCCESS;
		ULONG majorVersion = 0;
		ULONG minorVersion = 0;
		PsGetVersion( &majorVersion, &minorVersion, NULL, NULL );

		ULONG versionCode = majorVersion * 100 + minorVersion;
		KdPrint(("PsGetVersion, version is %d\n", versionCode));

		hookNtGdiBitBlt.pwzApiName = L"NtGdiBitBlt";
		hookNtGdiBitBlt.pNewApiAddress = Hooked_NtGdiBitBlt;
		hookNtGdiBitBlt.nParamCount = 0xB;

		hookNtGdiOpenDCW.pwzApiName = L"NtGdiOpenDCW";
		hookNtGdiOpenDCW.pNewApiAddress = Hooked_NtGdiOpenDCW;
		hookNtGdiOpenDCW.nParamCount = 0x7;

		hookNtGdiDeleteObjectApp.pwzApiName = L"NtGdiDeleteObjectApp";
		hookNtGdiDeleteObjectApp.pNewApiAddress = Hooked_NtGdiDeleteObjectApp;
		hookNtGdiDeleteObjectApp.nParamCount = 0x1;

#if defined(_M_IX86)
		FNT_VERIFY(_InitCallNumberX86(versionCode));
#elif defined(_M_AMD64)
		FNT_VERIFY(_InitCallNumberX64(versionCode));
#else
		error unsupport OS
#endif 
		return status;
	}
}DRIVER_HOOK_API_INFOS, *PDRIVER_HOOK_API_INFOS;




#pragma LOCKEDCODE	
DRIVER_HOOK_API_INFOS* g_pDriverHookApiInfos = NULL;



//int g_NtGdiStretchBltIndex = (-1);

HANDLE g_hProcess = 0;

//SCROLL_DATA g_ScrollData = {0};
//SCROLL_HOOK_TARGET g_ScrollHookTarget = {0};
PROTECT_WND_INFO   g_ProtectWndInfo = {0};
//Win32k.sys file, scroll to the target, the target-specific API calls to the process even if one has a value of TRUE.

// If the current one hooking SSDT API function calls and function call in progress, if this value has a value greater than 0.
LONG g_SSDTAPILockCount = 0;


int GetGdiExTextOutWIndex(SYSTEM_SERVICE_TABLE *p)
{
	/*
	1089 0008:A006F5AD params=03 NtGdiExtCreateRegion 
	108A 0008:A00B2E12 params=08 NtGdiExtEscape 
	108B 0008:A0133BFF params=05 NtGdiExtFloodFill 
	108C 0008:A002873A params=03 NtGdiExtGetObjectW 
	108D 0008:A0021B1A params=03 NtGdiExtSelectClipRgn 
	108E 0008:A0067926 params=09 NtGdiExtTextOutW
	108F 0008:A00948AF params=01 NtGdiFillPath 
	1090 0008:A00B617B params=03 NtGdiFillRgn 
	1091 0008:A0121AFA params=01 NtGdiFlattenPath 
	1092 0008:A001A344 params=00 NtGdiFlushUserBatch 
	1093 0008:A0017DB4 params=00 GreFlush 
	*/

	const int WIN2K_NtGdiExtCreateRegion = 0x89;
	for (int i = WIN2K_NtGdiExtCreateRegion; i <= WIN2K_NtGdiExtCreateRegion + 20; i++)
	{
		KdPrint(("%d", p[1].ArgumentsTable[i] / 4));
		if (
			((p[1].ArgumentsTable[i + 0]) == 12)		//0x89, NtGdiExtCreateRegion
			&& ((p[1].ArgumentsTable[i + 1]) == 32)		//0x8A, NtGdiExtEscape
			&& ((p[1].ArgumentsTable[i + 2]) == 20)		//0x8B, NtGdiExtFloodFill
			&& ((p[1].ArgumentsTable[i + 3]) == 12)		//0x8C, NtGdiExtGetObjectW
			&& ((p[1].ArgumentsTable[i + 4]) == 12)		//0x8D, NtGdiExtSelectClipRgn
			&& ((p[1].ArgumentsTable[i + 5]) == 36)		//0x8E, NtGdiExtTextOutW
			&& ((p[1].ArgumentsTable[i + 6]) == 4)		//0x8F, NtGdiFillPath
			&& ((p[1].ArgumentsTable[i + 7]) == 12)		//0x90, NtGdiFillRgn
			&& ((p[1].ArgumentsTable[i + 8]) == 4)		//0x91, NtGdiFlattenPath
			&& ((p[1].ArgumentsTable[i + 9]) == 0)		//0x92, NtGdiFlushUserBatch
			)
		{
			KdPrint(("Find GdiExTextOutW in Windows XP, 0x%x!!\n", (i+5)));
			return (i + 5);
		}
	}
	KdPrint(("ERROR CAN NOT FIND GdiExTextOutW!!\n"));
	return -1;
}

//http://www.reactos.org/wiki/Techwiki:Win32k/apfnSimpleCall

//enum SimpleCallRoutines
//{
//NOPARAM_ROUTINE_CREATEMENU,
//NOPARAM_ROUTINE_CREATEMENUPOPUP,
//NOPARAM_ROUTINE_ENABLEPROCWNDGHSTING,
//NOPARAM_ROUTINE_MSQCLEARWAKEMASK,
//NOPARAM_ROUTINE_ALLOWFOREGNDACTIVATION,
//NOPARAM_ROUTINE_DESTROY_CARET,
//NOPARAM_ROUTINE_GETDEVICECHANGEINFO,
//NOPARAM_ROUTINE_GETIMESHOWSTATUS,
//NOPARAM_ROUTINE_GETINPUTDESKTOP,
//NOPARAM_ROUTINE_GETMSESSAGEPOS,
//NOPARAM_ROUTINE_GETREMOTEPROCID,
//
//NOPARAM_ROUTINE_HIDECURSORNOCAPTURE,
//NOPARAM_ROUTINE_LOADCURSANDICOS,
//NOPARAM_ROUTINE_RELEASECAPTURE,
//NOPARAM_ROUTINE_RESETDBLCLICK,
//NOPARAM_ROUTINE_ZAPACTIVEANDFOUS,
//NOPARAM_ROUTINE_REMOTECONSHDWSTOP,
//NOPARAM_ROUTINE_REMOTEDISCONNECT,
//NOPARAM_ROUTINE_REMOTELOGOFF,
//NOPARAM_ROUTINE_REMOTENTSECURITY,
//NOPARAM_ROUTINE_REMOTESHDWSETUP,
//NOPARAM_ROUTINE_REMOTESHDWSTOP,
//NOPARAM_ROUTINE_REMOTEPASSTHRUENABLE,
//NOPARAM_ROUTINE_REMOTEPASSTHRUDISABLE,
//NOPARAM_ROUTINE_REMOTECONNECTSTATE,
//NOPARAM_ROUTINE_UPDATEPERUSERIMMENABLING,
//NOPARAM_ROUTINE_USERPWRCALLOUTWORKER,
//NOPARAM_ROUTINE_INIT_MESSAGE_PUMP,
//NOPARAM_ROUTINE_UNINIT_MESSAGE_PUMP,
//NOPARAM_ROUTINE_LOADUSERAPIHOOK,
//ONEPARAM_ROUTINE_BEGINDEFERWNDPOS,
//ONEPARAM_ROUTINE_GETSENDMSGRECVR,
//ONEPARAM_ROUTINE_WINDOWFROMDC,      //32 in WinXP
//ONEPARAM_ROUTINE_ALLOWSETFOREGND,
//ONEPARAM_ROUTINE_CREATEEMPTYCUROBJECT,
//ONEPARAM_ROUTINE_CREATESYSTEMTHREADS,
//ONEPARAM_ROUTINE_CSDDEUNINITIALIZE,
//ONEPARAM_ROUTINE_DIRECTEDYIELD,
//ONEPARAM_ROUTINE_ENUMCLIPBOARDFORMATS,
//ONEPARAM_ROUTINE_GETCURSORPOS,
//ONEPARAM_ROUTINE_GETINPUTEVENT,
//ONEPARAM_ROUTINE_GETKEYBOARDLAYOUT,
//ONEPARAM_ROUTINE_GETKEYBOARDTYPE,
//ONEPARAM_ROUTINE_GETPROCDEFLAYOUT,
//ONEPARAM_ROUTINE_GETQUEUESTATUS,
//ONEPARAM_ROUTINE_GETWINSTAINFO,
//ONEPARAM_ROUTINE_HANDLESYSTHRDCREATFAIL,
//};

//#define ONEPARAM_ROUTINE_WINDOWFROMDC_WINXP 31		//0x1f

//USER32!NtUserCallOneParam (77d184ae)
//mov     edx,offset SharedUserData!SystemCallStub (7ffe0300)

BOOL Hooked_NtGdiBitBlt(
					 HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
					 HDC hDCSrc, int  XSrc, int  YSrc,	
					 ULONG  ROP, ULONG crBackColor,ULONG fl)
{
	NTSTATUS status = STATUS_SUCCESS;
	BOOL nResult = TRUE;

	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		HWND hWndDestFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCDest, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);
		HWND hWndSrcFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCSrc, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);


		BOOL bWillPrevent = FALSE;
		if (hWndDestFromDC == g_pDriverHookApiInfos->m_hWndDesktop
			|| hWndSrcFromDC == g_pDriverHookApiInfos->m_hWndDesktop
			|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hDCSrc))
		{
			//Skip 
			//KdPrint(("[%d], Bitblt for Desktop Wnd\n"));
			//RECT rcWnd = g_ScrollHookTarget.rcProtectWindow;

			//drawProtectResult  -- FALSE , 而 正常的 Draw 返回 TRUE
			//[1972], !!!! In Hooked_NtGdiBitBlt, hWndDesktop=0x10014, hWndDestFromDC=0x0, hDCDest=0x92010acd, hWndSrcFromDC=0x10014, 
			//hDCSrc=0x1010056, nResult=1, drawProtecResult=0, rcWnd={373, 313, 907 ,708}, hDCProtect=0x8801097d
			//BOOL drawProtectResult = ((NTGDIBITBLT)(g_pDriverHookApiInfos->hookNtGdiBitBlt.pOrigApiAddress))
			//	(hDCDest, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
			//	g_ScrollHookTarget.hDCProtect, 0, 0, SRCCOPY, ULONG(-1) , 0);

			//KdPrint(("[%d], !!!! In Hooked_NtGdiBitBlt, hWndDesktop=0x%x, hWndDestFromDC=0x%x, "
			//	"hDCDest=0x%x, hWndSrcFromDC=0x%x, hDCSrc=0x%x, nResult=%d, drawProtecResult=%d,"
			//	"rcWnd={%d, %d, %d ,%d}, hDCProtect=0x%x\n"
			//	, 
			//	PsGetCurrentProcessId(),
			//	g_pDriverHookApiInfos->m_hWndDesktop, hWndDestFromDC, hDCDest, 
			//	hWndSrcFromDC, hDCSrc, nResult, drawProtectResult,
			//	rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, g_ScrollHookTarget.hDCProtect
			//	));


			bWillPrevent = TRUE;
		}
		else
		{
			nResult = ((NTGDIBITBLT)(g_pDriverHookApiInfos->hookNtGdiBitBlt.pOrigApiAddress))
				(hDCDest, XDest, YDest, Width, Height, hDCSrc, XSrc, YSrc, ROP, crBackColor, fl);
		}
		if (bWillPrevent)
		{
			//RECT rcClient = {XDest, YDest, XDest + Width, YDest + Height};
			//FNT_VERIFY(g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW(hDCDest, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, NULL, 0));
			//KdPrint(("OrigNtGdiExtTextOutW, return 0x%x\n", status));
		}

		//EngBitBlt(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}


	return nResult;
}

HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING Device,
							DEVMODEW *InitData,
							PUNICODE_STRING pustrLogAddr,
							ULONG iType,
							HANDLE hspool,
							VOID *pDriverInfo2,
							VOID *pUMdhpdev )
{
	HDC hDC = (NTGDIOPENDCW(g_pDriverHookApiInfos->hookNtGdiOpenDCW.pOrigApiAddress))(Device, InitData, pustrLogAddr, iType, hspool, pDriverInfo2, pUMdhpdev);

	//KdPrint(("[%d], In Hooked_NtGdiOpenDCW Device=%wZ, hDC=0x%x\n", 
	//	PsGetCurrentProcessId(), Device, hDC));

	if (!Device)
		//|| (RtlEqualUnicodeString(Device, &g_pDriverHookApiInfos->m_UnicodeString_DISPLAY, TRUE)))
	{
		g_pDriverHookApiInfos->AddCreatedDisplayDC(hDC);
	}
	return hDC;
}

BOOL Hooked_NtGdiDeleteObjectApp(HDC  DCHandle)
{

	BOOL bRet = g_pDriverHookApiInfos->RemoveCreatedDisplayDC(DCHandle);
	if (bRet)
	{
		KdPrint(("[%d], In Hooked_NtGdiDeleteObjectApp hDC=0x%x\n", 
			PsGetCurrentProcessId(), DCHandle));
	}
	bRet = ((NTGDIDELETEOBJECTAPP)(g_pDriverHookApiInfos->hookNtGdiDeleteObjectApp.pOrigApiAddress))(DCHandle);
	return bRet;
}


void InstallApiHook(HANDLE hProcess, HWND hWndDesktop)
{
		//PAGED_CODE();

#if DBG
		//_asm int 3
#endif

		KdPrint(("InstallApiHook for hProcess=0x%x, hWndDesktop=0x%x, g_pDriverHookApiInfos=0x%p\n", hProcess, hWndDesktop, g_pDriverHookApiInfos));

		if (!g_pDriverHookApiInfos)
		{
			g_pDriverHookApiInfos = (PDRIVER_HOOK_API_INFOS)ExAllocatePoolWithTag(NonPagedPoolCacheAlignedMustS, sizeof(DRIVER_HOOK_API_INFOS), HOOK_API_INFO_TAG);
			NT_ASSERT(g_pDriverHookApiInfos);
			if (g_pDriverHookApiInfos)
			{
				RtlZeroMemory(g_pDriverHookApiInfos, sizeof(DRIVER_HOOK_API_INFOS));

				g_pDriverHookApiInfos->InitValue();
				g_pDriverHookApiInfos->m_hWndDesktop = hWndDesktop;

				NTSTATUS status = STATUS_SUCCESS;
				//KIRQL irql = WPOFFx64();

				//KdPrint(("IRQL : %#x", KeGetCurrentIrql()));


				if (NULL == GetKeServiceDescriptorTableShadowAddress())
				{
					KdPrint(("Failed GetShadowTable"));
				}
				else
				{
					PEPROCESS pEProcess = NULL;

					PsLookupProcessByProcessId(hProcess, &pEProcess);
					if (pEProcess == NULL)
					{
						//WPONx64(irql);

						g_pDriverHookApiInfos->FinalizeValue();
						ExFreePoolWithTag(g_pDriverHookApiInfos, HOOK_API_INFO_TAG);
						g_pDriverHookApiInfos = NULL;
						return ;
					}

					KAPC_STATE KApcState = {0};
					KeStackAttachProcess(pEProcess, &KApcState);
					//KeAttachProcess(pEProcess);

					FNT_VERIFY(g_pDriverHookApiInfos->InitCallNumber());
					if (NT_SUCCESS(status))
					{
						g_hProcess = hProcess;

						HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiBitBlt);
						HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiOpenDCW);
						HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiDeleteObjectApp);


						g_pDriverHookApiInfos->pOrigNtUserCallOneParam = (NTUSERCALLONEPARAM)(GetShadowSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtUserCallOneParam));
					}
					KeUnstackDetachProcess(&KApcState);
				}
			}
		}
}

void UnInstallAPIHook(void)
{
	LARGE_INTEGER WaitTime;
	KdPrint(("Enter UnInstallAPIHook\n"));

	if (g_pDriverHookApiInfos)
	{
		KdPrint(("ServiceDescriptorShadowTableAddress : %#x", g_pShadowTable));

		if (g_hProcess != 0)
		{
			if (g_pDriverHookApiInfos->HasValidCallNumber())// g_pDriverHookApiInfos-> g_NtGdiBitBltIndex != (-1))
			{
				PEPROCESS pEProcess;

				PsLookupProcessByProcessId(g_hProcess, &pEProcess);

				KAPC_STATE KApcState = {0};
				KeStackAttachProcess(pEProcess, &KApcState);

				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiBitBlt);
				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiOpenDCW);
				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiDeleteObjectApp);

				g_pDriverHookApiInfos->pOrigNtUserCallOneParam = NULL;

				KeUnstackDetachProcess(&KApcState);
			}
		}

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
	DbgPrint("Leave UnInstallAPIHook\n");
}
