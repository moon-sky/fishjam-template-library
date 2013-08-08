#include "stdafx.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"
#include "FDriverUtil.h"
#include "FDriverDemoDefine.h"

#define ObjectNameInformation  1
#define SystemHandleInformation 0x10
#define HOOK_API_INFO_TAG     (ULONG)0x12345678

SYSTEM_SERVICE_TABLE *g_pShadowTable = NULL;

typedef BOOL (*NTGDIBITBLT)(HDC hDCDest, int XDest, int YDest, int Width, int Height,
						    HDC hDCSrc, int XSrc, int YSrc,	
						    ULONG  ROP,ULONG crBackColor,ULONG fl);

typedef BOOL (*NTGDISTRETCHBLT)(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
								HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
								DWORD dwRop, DWORD dwBackColor);

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

typedef BOOL (*NTUSERPRINTWINDOW)(HWND 	hwnd, HDC hdcBlt, UINT nFlags);


//typedef DWORD (*NTGDIDDLOCK)(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip);


#define MAX_TRACE_CREATE_DISPLAY_DC_COUNT	1

BOOL Hooked_NtGdiBitBlt(
						HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
						HDC hDCSrc, int  XSrc, int  YSrc,	
						ULONG  ROP, ULONG crBackColor,ULONG fl);

BOOL Hooked_NtGdiStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
									 HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
									 DWORD dwRop, DWORD dwBackColor);


HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING Device,
						DEVMODEW *InitData,
						PUNICODE_STRING pustrLogAddr,
						ULONG iType,
						HANDLE hspool,
						VOID *pDriverInfo2,
						VOID *pUMdhpdev );

BOOL Hooked_NtGdiDeleteObjectApp(HDC  DCHandle);

BOOL Hooked_NtUserPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

//DWORD Hooked_NtGdiDdLock(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip);


typedef struct _DRIVER_HOOK_API_INFOS
{
public:
	PROTECT_WND_INFO		m_ProtectWndInfo;
	//HWND					m_hWndDesktop;
    //HWND					m_hWndProtect;
    unsigned long			m_OldCr0;
    BOOL                    m_ValidCallNumber;
	HDC						m_hCreatedDisplayDC[MAX_TRACE_CREATE_DISPLAY_DC_COUNT];
	//UNICODE_STRING			m_UnicodeString_DISPLAY;
public:
	HOOK_API_INFO			hookNtGdiBitBlt;
	HOOK_API_INFO			hookNtGdiStretchBlt;
	HOOK_API_INFO			hookNtGdiOpenDCW;
	HOOK_API_INFO			hookNtGdiDeleteObjectApp;
	HOOK_API_INFO			hookNtUserPrintWindow;
	HOOK_API_INFO			hookNtGdiDdLock;

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
				//if (NULL == m_hCreatedDisplayDC[i])
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
				hookNtGdiStretchBlt.nIndexInSSDT = 0x124;
				hookNtGdiOpenDCW.nIndexInSSDT = 0xe9;
				hookNtGdiDeleteObjectApp.nIndexInSSDT = 0x7a;
				hookNtUserPrintWindow.nIndexInSSDT = 0x1dd;
				hookNtGdiDdLock.nIndexInSSDT = 0x53;

				IndexOfNtUserCallOneParam = 0x143;      //323;
				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f;	//31
				break;
			}
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7\n"));
				hookNtGdiBitBlt.nIndexInSSDT = 0xE;
				hookNtGdiStretchBlt.nIndexInSSDT = 0x12e;
				hookNtGdiOpenDCW.nIndexInSSDT = 0xf3;
				hookNtGdiDeleteObjectApp.nIndexInSSDT = 0x7d;
				hookNtUserPrintWindow.nIndexInSSDT = 0x1fe;
				hookNtGdiDdLock.nIndexInSSDT = 0x56;

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
				hookNtGdiStretchBlt.nIndexInSSDT = 0x31;
				hookNtGdiOpenDCW.nIndexInSSDT = 0xda;
				hookNtGdiDeleteObjectApp.nIndexInSSDT = 0x23;
				hookNtUserPrintWindow.nIndexInSSDT = 0x2eb;
				hookNtGdiDdLock.nIndexInSSDT = 0x1ab;

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

		hookNtGdiStretchBlt.pNewApiAddress = L"NtGdiStretchBlt";
		hookNtGdiStretchBlt.pNewApiAddress = Hooked_NtGdiStretchBlt;
		hookNtGdiStretchBlt.nParamCount = 0xC; //12

		hookNtGdiOpenDCW.pwzApiName = L"NtGdiOpenDCW";
		hookNtGdiOpenDCW.pNewApiAddress = Hooked_NtGdiOpenDCW;
		hookNtGdiOpenDCW.nParamCount = 0x7;

		hookNtGdiDeleteObjectApp.pwzApiName = L"NtGdiDeleteObjectApp";
		hookNtGdiDeleteObjectApp.pNewApiAddress = Hooked_NtGdiDeleteObjectApp;
		hookNtGdiDeleteObjectApp.nParamCount = 0x1;

		hookNtUserPrintWindow.pwzApiName = L"NtUserPrintWindow";
		hookNtUserPrintWindow.pNewApiAddress = Hooked_NtUserPrintWindow;
		hookNtUserPrintWindow.nParamCount = 0x3;

		hookNtGdiDdLock.pwzApiName = L"NtGdiDdLock";
		//hookNtGdiDdLock.pNewApiAddress = Hooked_NtGdiDdLock;
		hookNtGdiDdLock.nParamCount = 0x3;

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

LONG g_SSDTAPILockCount = 0;


// If the current one hooking SSDT API function calls and function call in progress, if this value has a value greater than 0.

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
	BOOL bRet = TRUE;

	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		HWND hWndDestFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCDest, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);
		HWND hWndSrcFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCSrc, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);


		//BOOL bWillPrevent = FALSE;
		HWND hWndDeskTop = g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop;
		if (hWndDestFromDC == hWndDeskTop
			|| hWndSrcFromDC == hWndDeskTop
			|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hDCSrc))
		{
			//Skip 
			KdPrint(("!!! in Hooked_NtGdiBitBlt\n"));

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

			//bWillPrevent = TRUE;
		}
		else
		{
			bRet = ((NTGDIBITBLT)(g_pDriverHookApiInfos->hookNtGdiBitBlt.pOrigApiAddress))
				(hDCDest, XDest, YDest, Width, Height, hDCSrc, XSrc, YSrc, ROP, crBackColor, fl);
		}
		//if (bWillPrevent)
		//{
			//RECT rcClient = {XDest, YDest, XDest + Width, YDest + Height};
			//FNT_VERIFY(g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW(hDCDest, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, NULL, 0));
			//KdPrint(("OrigNtGdiExtTextOutW, return 0x%x\n", status));
		//}

		//EngBitBlt(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

BOOL Hooked_NtGdiStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
							HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, 
							DWORD dwRop, DWORD dwBackColor)
{
	BOOL bRet = TRUE;

	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		HWND hWndDestFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hdcDst, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);
		HWND hWndSrcFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hdcSrc, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);

		HWND hWndDeskTop = g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop;
		if (hWndDestFromDC == hWndDeskTop
			|| hWndSrcFromDC == hWndDeskTop
			|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
		{
			KdPrint(("!!! in Hooked_NtGdiStretchBlt\n"));
		}
		else
		{
			bRet = ((NTGDISTRETCHBLT)(g_pDriverHookApiInfos->hookNtGdiStretchBlt.pOrigApiAddress))
				(hdcDst, xDst, yDst, cxDst, cyDst, hdcSrc, xSrc, ySrc, cxSrc, cySrc, dwRop, dwBackColor);
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return bRet;
}

HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING Device,
							DEVMODEW *InitData,
							PUNICODE_STRING pustrLogAddr,
							ULONG iType,
							HANDLE hspool,
							VOID *pDriverInfo2,
							VOID *pUMdhpdev )
{
	HDC hDC = NULL;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		hDC = (NTGDIOPENDCW(g_pDriverHookApiInfos->hookNtGdiOpenDCW.pOrigApiAddress))(Device, InitData, pustrLogAddr, iType, hspool, pDriverInfo2, pUMdhpdev);

		//KdPrint(("[%d], In Hooked_NtGdiOpenDCW Device=%wZ, hDC=0x%x\n", 
		//	PsGetCurrentProcessId(), Device, hDC));

		if (!Device)
			//|| (RtlEqualUnicodeString(Device, &g_pDriverHookApiInfos->m_UnicodeString_DISPLAY, TRUE)))
		{
			g_pDriverHookApiInfos->AddCreatedDisplayDC(hDC);
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return hDC;
}

BOOL Hooked_NtGdiDeleteObjectApp(HDC  DCHandle)
{

	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		bRet = g_pDriverHookApiInfos->RemoveCreatedDisplayDC(DCHandle);
		if (bRet)
		{
			KdPrint(("[%d], In Hooked_NtGdiDeleteObjectApp hDC=0x%x\n", 
				PsGetCurrentProcessId(), DCHandle));
		}
		bRet = ((NTGDIDELETEOBJECTAPP)(g_pDriverHookApiInfos->hookNtGdiDeleteObjectApp.pOrigApiAddress))(DCHandle);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

BOOL Hooked_NtUserPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("Enter Hooked_NtUserPrintWindow, hWnd=0x%x, hdcBlt=0x%x, nFlags=%d\n",
			hwnd, hdcBlt, nFlags));

		bRet = ((NTUSERPRINTWINDOW)(g_pDriverHookApiInfos->hookNtUserPrintWindow.pOrigApiAddress))(hwnd, hdcBlt, nFlags);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return bRet;
}

//DWORD Hooked_NtGdiDdLock(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip)
//{
//	DWORD dwResult = 0;
//	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
//	__try
//	{
//		KdPrint(("Enter Hooked_NtGdiDdLock, hSurface=0x%x, hdcClip=0x%x\n",
//			hSurface, hdcClip));
//
//		dwResult = ((NTGDIDDLOCK)(g_pDriverHookApiInfos->hookNtGdiDdLock.pOrigApiAddress))(hSurface, puLockData, hdcClip);
//	}
//	__finally
//	{
//		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
//	}
//	return dwResult;
//}


NTSTATUS InstallCopyProtectHook(PPROTECT_WND_INFO pProtectWndInfo)
{
		//PAGED_CODE();
	NTSTATUS status = STATUS_SUCCESS;

#if DBG
		//_asm int 3
#endif
	if (NULL == GetKeServiceDescriptorTableShadowAddress())
	{
		KdPrint(("!!!Error -- Find Shadow SSDT Faild\n"));
		return STATUS_FAILED_DRIVER_ENTRY;
	}

	if (!g_pDriverHookApiInfos)
	{
		g_pDriverHookApiInfos = (PDRIVER_HOOK_API_INFOS)ExAllocatePoolWithTag(NonPagedPoolCacheAlignedMustS, sizeof(DRIVER_HOOK_API_INFOS), HOOK_API_INFO_TAG);
		NT_ASSERT(g_pDriverHookApiInfos);
		if (g_pDriverHookApiInfos)
		{
			RtlZeroMemory(g_pDriverHookApiInfos, sizeof(DRIVER_HOOK_API_INFOS));
			g_pDriverHookApiInfos->InitValue();
		}
	}
	if (g_pDriverHookApiInfos)
	{
		RtlCopyMemory(&g_pDriverHookApiInfos->m_ProtectWndInfo, pProtectWndInfo, sizeof(PROTECT_WND_INFO));
		KdPrint(("Enter InstallCopyProtectHook, hProcess=%d, WndDesk=0x%x\n",
			g_pDriverHookApiInfos->m_ProtectWndInfo.hTargetProcess, 
			g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop));

		PEPROCESS pEProcess = NULL;

		FNT_VERIFY(PsLookupProcessByProcessId(pProtectWndInfo->hTargetProcess, &pEProcess));
		if (pEProcess == NULL)
		{
			g_pDriverHookApiInfos->FinalizeValue();
			ExFreePoolWithTag(g_pDriverHookApiInfos, HOOK_API_INFO_TAG);
			g_pDriverHookApiInfos = NULL;
			return status;
		}

		KAPC_STATE KApcState = {0};
		KeStackAttachProcess(pEProcess, &KApcState);

		FNT_VERIFY(g_pDriverHookApiInfos->InitCallNumber());
		if (NT_SUCCESS(status))
		{
			FNT_VERIFY(HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiBitBlt));
			FNT_VERIFY(HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiOpenDCW));
			FNT_VERIFY(HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiDeleteObjectApp));
			FNT_VERIFY(HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtUserPrintWindow));
			//FNT_VERIFY(HookShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiDdLock));

			g_pDriverHookApiInfos->pOrigNtUserCallOneParam = (NTUSERCALLONEPARAM)(GetShadowSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtUserCallOneParam));
		}
		KeUnstackDetachProcess(&KApcState);
	}
	return status;
}

NTSTATUS UnInstallAPIHook(void)
{
	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Enter UnInstallAPIHook\n"));
	if (g_pDriverHookApiInfos)
	{
		KdPrint(("ServiceDescriptorShadowTableAddress : %#x", g_pShadowTable));

		if (g_pDriverHookApiInfos->HasValidCallNumber())// g_pDriverHookApiInfos-> g_NtGdiBitBltIndex != (-1))
		{
			PEPROCESS pEProcess = NULL;
			FNT_VERIFY(PsLookupProcessByProcessId(g_pDriverHookApiInfos->m_ProtectWndInfo.hTargetProcess, &pEProcess));
			if (pEProcess)
			{
				KAPC_STATE KApcState = {0};
				KeStackAttachProcess(pEProcess, &KApcState);

				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiBitBlt);
				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiOpenDCW);
				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiDeleteObjectApp);
				RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtUserPrintWindow);
				//RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->hookNtGdiDdLock);

				g_pDriverHookApiInfos->pOrigNtUserCallOneParam = NULL;

				KeUnstackDetachProcess(&KApcState);
			}
		}

		LARGE_INTEGER WaitTime;
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
	return status;
}
