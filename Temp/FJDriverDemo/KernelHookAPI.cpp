#include "stdafx.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"
#include "FDriverUtil.h"
#include "FDriverDemoDefine.h"
#include <ddrawint.h>

#define ObjectNameInformation  1
#define SystemHandleInformation 0x10
#define HOOK_API_INFO_TAG     (ULONG)0x12345678

SYSTEM_SERVICE_TABLE *g_pShadowTable = NULL;

//函数原型 -- http://doxygen.reactos.org/
//Windows WIN32K.SYS System Call Table (NT/2000/XP/2003/Vista/2008/7)
//  http://j00ru.vexillium.org/win32k_syscalls/
//  http://j00ru.vexillium.org/win32k_x64/

typedef HDC (*NTGDICREATECOMPATIBLEDC)(HDC hdc);
typedef HBITMAP (*NTGDICREATECOMPATIBLEBITMAP)(HDC hDC, INT Width, INT Height);
typedef HBITMAP (*NTGDISELECTBITMAP)(HDC hdc, HBITMAP hbmp)	;

typedef DWORD_PTR (*NTUSERCALLONEPARAM)(DWORD_PTR Param, DWORD Routine);

typedef BOOL (*NTGDIBITBLT)(HDC hDCDest, int XDest, int YDest, int Width, int Height,
						    HDC hDCSrc, int XSrc, int YSrc,	
						    ULONG  ROP,ULONG crBackColor,ULONG fl);

typedef BOOL (*NTGDISTRETCHBLT)(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
								HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
								DWORD dwRop, DWORD dwBackColor);

typedef BOOL (*NTGDIPLGBLT)(HDC hdcTrg, LPPOINT pptlTrg, 
							HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
							HBITMAP hbmMask, INT xMask, INT yMask, DWORD crBackColor);

typedef BOOL (*NTGDIMASKBLT)(HDC hdc, INT xDst, INT yDst, INT cx, INT cy,
							 HDC hdcSrc, INT xSrc, INT ySrc, 
							 HBITMAP hbmMask, INT xMask, INT yMask, 
							 DWORD dwRop4, DWORD crBackColor);

typedef BOOL (*NTGDITRANSPARENTBLT)(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst,
									HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
									COLORREF TransColor);


typedef BOOL (*NTGDIEXTTEXTOUTW)(HDC hDC, INT XStart, INT YStart, UINT fuOptions,
								 LPRECT UnsafeRect, LPWSTR UnsafeString, INT Count, 
								 LPINT UnsafeDx, DWORD dwCodePage);

typedef HDC (*NTGDIOPENDCW)( PUNICODE_STRING Device,
					 DEVMODEW *InitData,
					 PUNICODE_STRING pustrLogAddr,
					 ULONG iType,
					 HANDLE hspool,
					 VOID *pDriverInfo2,
					 VOID *pUMdhpdev );
typedef BOOL (*NTGDIDELETEOBJECTAPP)(HDC  DCHandle);

typedef BOOL (*NTUSERPRINTWINDOW)(HWND 	hwnd, HDC hdcBlt, UINT nFlags);


typedef DWORD (*NTGDIDDLOCK)(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip);


#define MAX_TRACE_CREATE_DISPLAY_DC_COUNT	1

BOOL Hooked_NtGdiBitBlt(
						HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
						HDC hDCSrc, int  XSrc, int  YSrc,	
						ULONG  ROP, ULONG crBackColor,ULONG fl);

BOOL Hooked_NtGdiStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
									 HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
									 DWORD dwRop, DWORD dwBackColor);

BOOL Hooked_NtGdiPlgBlt(HDC hdcTrg, LPPOINT pptlTrg, 
							HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
							HBITMAP hbmMask, INT xMask, INT yMask, DWORD crBackColor);

BOOL Hooked_NtGdiMaskBlt(HDC hdc, INT xDst, INT yDst, INT cx, INT cy,
						HDC hdcSrc, INT xSrc, INT ySrc, 
						HBITMAP hbmMask, INT xMask, INT yMask, 
						DWORD dwRop4, DWORD crBackColor);

BOOL Hooked_NtGdiTransparentBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst,
								   HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
								   COLORREF TransColor);

BOOL Hooked_NtGdiExtTextOutW(HDC hDC, INT XStart, INT YStart, UINT fuOptions,
								 LPRECT UnsafeRect, LPWSTR UnsafeString, INT Count, 
								 LPINT UnsafeDx, DWORD dwCodePage);


HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING Device,
						DEVMODEW *InitData,
						PUNICODE_STRING pustrLogAddr,
						ULONG iType,
						HANDLE hspool,
						VOID *pDriverInfo2,
						VOID *pUMdhpdev );

BOOL Hooked_NtGdiDeleteObjectApp(HDC  DCHandle);

BOOL Hooked_NtUserPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

DWORD Hooked_NtGdiDdLock(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip);

BOOL IsFilterHDC(HDC hDC, HWND* phWndFromDC);

enum HookFuncType
{
	hft_NtGdiBitBlt = 0,
	hft_NtGdiStretchBlt,
	hft_NtGdiPlgBlt,
	hft_NtGdiMaskBlt,
	hft_NtGdiTransparentBlt,
	hft_NtGdiExtTextOutW,

	hft_NtGdiOpenDCW,
	hft_NtGdiDeleteObjectApp,

	hft_NtUserPrintWindow,
	hft_NtGdiDdLock,

	hft_FunctionCount,
};

typedef struct _DRIVER_HOOK_API_INFOS
{
public:
	PROTECT_WND_INFO		m_ProtectWndInfo;
#if 0
	HDC						m_hMemoryDC;
	HBITMAP					m_hBitmap;
	//PEPROCESS				m_pEProcess;
	//HWND					m_hWndDesktop;
    //HWND					m_hWndProtect;
    unsigned long			m_OldCr0;
#endif 
    BOOL                    m_ValidCallNumber;
	HDC						m_hCreatedDisplayDC[MAX_TRACE_CREATE_DISPLAY_DC_COUNT];
	//UNICODE_STRING			m_UnicodeString_DISPLAY;
public:
	HOOK_API_INFO			m_HookFuns[hft_FunctionCount];

	LONG					IndexOfNtGdiSelectBitmap;
	LONG					IndexOfNtGdiCreateCompatibleBitmap;
	LONG					IndexOfNtGdiCreateCompatibleDC;
	LONG                    IndexOfNtUserCallOneParam;

	LONG					ONEPARAM_ROUTINE_WINDOWFROMDC;

	ULONG					nPatchSizeGdiBltBlt;
	
	NTGDISELECTBITMAP		pOrigNtGdiSelectBitmap;
	NTGDICREATECOMPATIBLEBITMAP pOrigNtGdiCreateCompatibleBitmap;
	NTGDICREATECOMPATIBLEDC pOrigNtGdiCreateCompatibleDC;

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
				m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0xD;				//13,  100D 0008:A001B344 params=0B NtGdiBitBlt 
				m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x124;
				m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0xed;
				m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0xe3;
				m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x12a;
				m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x92;

				m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0xe9;
				m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x7a;

				m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x1dd;
				m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x53;

				IndexOfNtGdiCreateCompatibleDC = 0x1e;
				IndexOfNtGdiCreateCompatibleBitmap = 0x1d;
				IndexOfNtGdiSelectBitmap = 0x101;
				IndexOfNtUserCallOneParam = 0x143;      //323;
				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f;	//31
				break;
			}
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7\n"));
				m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0xE;
				m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x12e;
				m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0xf7;
				m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0xed;
				m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x134;
				m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x95;

				m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0xf3;
				m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x7d;

				m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x1fe;
				m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x56;

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

				m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x8;
				m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x31;
				m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x24a;
				m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x69;
				m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x275;
				m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x38;

				m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0xda;
				m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x23;

				m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x2eb;
				m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1ab;

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

		m_HookFuns[hft_NtGdiBitBlt].pwzApiName = L"NtGdiBitBlt";
		m_HookFuns[hft_NtGdiBitBlt].pNewApiAddress = Hooked_NtGdiBitBlt;
		m_HookFuns[hft_NtGdiBitBlt].nParamCount = 11;
		m_HookFuns[hft_NtGdiBitBlt].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiStretchBlt].pwzApiName = L"NtGdiStretchBlt";
		m_HookFuns[hft_NtGdiStretchBlt].pNewApiAddress = Hooked_NtGdiStretchBlt;
		m_HookFuns[hft_NtGdiStretchBlt].nParamCount = 12;
		m_HookFuns[hft_NtGdiStretchBlt].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiPlgBlt].pwzApiName = L"NtGdiPlgBlt";
		m_HookFuns[hft_NtGdiPlgBlt].pNewApiAddress = Hooked_NtGdiPlgBlt;
		m_HookFuns[hft_NtGdiPlgBlt].nParamCount = 11;
		m_HookFuns[hft_NtGdiPlgBlt].bEnableHook = FALSE;

		m_HookFuns[hft_NtGdiMaskBlt].pwzApiName = L"NtGdiMaskBlt";
		m_HookFuns[hft_NtGdiMaskBlt].pNewApiAddress = Hooked_NtGdiMaskBlt;
		m_HookFuns[hft_NtGdiMaskBlt].nParamCount = 13;
		m_HookFuns[hft_NtGdiMaskBlt].bEnableHook = FALSE;

		m_HookFuns[hft_NtGdiTransparentBlt].pwzApiName = L"NtGdiTransparentBlt";
		m_HookFuns[hft_NtGdiTransparentBlt].pNewApiAddress = Hooked_NtGdiTransparentBlt;
		m_HookFuns[hft_NtGdiTransparentBlt].nParamCount = 11;
		m_HookFuns[hft_NtGdiTransparentBlt].bEnableHook = FALSE;

		m_HookFuns[hft_NtGdiExtTextOutW].pwzApiName = L"NtGdiExtTextOutW";
		m_HookFuns[hft_NtGdiExtTextOutW].pNewApiAddress = Hooked_NtGdiExtTextOutW;
		m_HookFuns[hft_NtGdiExtTextOutW].nParamCount = 9;
		m_HookFuns[hft_NtGdiExtTextOutW].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiOpenDCW].pwzApiName = L"NtGdiOpenDCW";
		m_HookFuns[hft_NtGdiOpenDCW].pNewApiAddress = Hooked_NtGdiOpenDCW;
		m_HookFuns[hft_NtGdiOpenDCW].nParamCount = 7;
		m_HookFuns[hft_NtGdiOpenDCW].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiDeleteObjectApp].pwzApiName = L"NtGdiDeleteObjectApp";
		m_HookFuns[hft_NtGdiDeleteObjectApp].pNewApiAddress = Hooked_NtGdiDeleteObjectApp;
		m_HookFuns[hft_NtGdiDeleteObjectApp].nParamCount = 1;
		m_HookFuns[hft_NtGdiDeleteObjectApp].bEnableHook = TRUE;

		m_HookFuns[hft_NtUserPrintWindow].pwzApiName = L"NtUserPrintWindow";
		m_HookFuns[hft_NtUserPrintWindow].pNewApiAddress = Hooked_NtUserPrintWindow;
		m_HookFuns[hft_NtUserPrintWindow].nParamCount = 3;
		m_HookFuns[hft_NtUserPrintWindow].bEnableHook = FALSE;

		m_HookFuns[hft_NtGdiDdLock].pwzApiName = L"NtGdiDdLock";
		m_HookFuns[hft_NtGdiDdLock].pNewApiAddress = Hooked_NtGdiDdLock;
		m_HookFuns[hft_NtGdiDdLock].nParamCount = 0x3;
		m_HookFuns[hft_NtGdiDdLock].bEnableHook = FALSE;

#if defined(_M_IX86)
		FNT_VERIFY(_InitCallNumberX86(versionCode));
#elif defined(_M_AMD64)
		FNT_VERIFY(_InitCallNumberX64(versionCode));
#else
		error unsupport OS
#endif 
		return status;
	}

#if 0
	NTSTATUS RefreshMemoryDC()
	{
		//__asm int 3
		//NOTICE: there is no DeleteDC or DeleteObject for bitmap

		NTSTATUS status = STATUS_INVALID_PARAMETER;
		int cx = m_ProtectWndInfo.rcProtectWindow.right - m_ProtectWndInfo.rcProtectWindow.left;
		int cy = m_ProtectWndInfo.rcProtectWindow.bottom - m_ProtectWndInfo.rcProtectWindow.top;

		PEPROCESS pEProcess = NULL;
		KAPC_STATE KApcState = {0};

		FNT_VERIFY(PsLookupProcessByProcessId(m_ProtectWndInfo.hSelfProcess, &pEProcess));
		if (pEProcess)
		{
			KeStackAttachProcess(pEProcess, &KApcState);
		}

		m_hMemoryDC = pOrigNtGdiCreateCompatibleDC(m_ProtectWndInfo.hDCDesktop);
		m_hBitmap = pOrigNtGdiCreateCompatibleBitmap(m_ProtectWndInfo.hDCDesktop, cx, cy);
		HBITMAP hOldBmp = pOrigNtGdiSelectBitmap(m_hMemoryDC, m_hBitmap);

		BOOL bRet = ((NTGDIBITBLT)(m_HookFuns[hft_NtGdiBitBlt].pOrigApiAddress))
			(m_hMemoryDC, 0, 0, cx, cy, m_ProtectWndInfo.hDCWndProtect, 0, 0, SRCCOPY, ULONG(-1) , 0);

		if (pEProcess)
		{
			KeUnstackDetachProcess(&KApcState);
			ObDereferenceObject(pEProcess);
		}
		if (bRet)
		{
			status = STATUS_SUCCESS;
		}
		return bRet;
	}
#endif 

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
					 ULONG  ROP, ULONG crBackColor, ULONG fl)
{
	BOOL bRet = TRUE;
	NTSTATUS status = STATUS_SUCCESS;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		HWND hWndFromDest = NULL;
		HWND hWndFromSrc = NULL;
		BOOL bIsFilterDCDest = IsFilterHDC(hDCDest, &hWndFromDest);
		BOOL bIsFilterDCSrc = IsFilterHDC(hDCSrc, &hWndFromSrc);
		BOOL bIsCreateDisplayDC = g_pDriverHookApiInfos->IsCreatedDisplayDC(hDCSrc);

		if (bIsFilterDCSrc || bIsFilterDCDest
			|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hDCSrc))
		{
			//Skip 
			RECT rcWnd = g_pDriverHookApiInfos->m_ProtectWndInfo.rcProtectWindow;
			HDC hdcMemory = g_pDriverHookApiInfos->m_ProtectWndInfo.hDCWndProtect;

			//PEPROCESS pEProcess = NULL;
			//KAPC_STATE KApcState = {0};

			////FNT_VERIFY(PsLookupProcessByProcessId(PsGetCurrentProcessId(), &pEProcess));
			//if (pEProcess)
			//{
			//	KeStackAttachProcess(pEProcess, &KApcState);
			//}

			//drawProtectResult  -- FALSE , 而 正常的 Draw 返回 TRUE
			//BOOL drawProtectResult = ((NTGDIBITBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiBitBlt].pOrigApiAddress))
			//		(hDCDest, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
			//		hdcMemory, 0, 0, SRCCOPY, ULONG(-1) , 0);
			KdPrint(("[%d]!!! in Hooked_NtGdiBitBlt,isDest=%d(0x%x), isSrc=%d(0x%x), isCreateDC=%d, hDCDest=0x%x, hDCSrc=0x%x, hdcMemory =0x%x, rcProtect={%d,%d -- %d,%d}\n", 
				PsGetCurrentProcessId(), bIsFilterDCDest, hWndFromDest, bIsFilterDCSrc, hWndFromSrc, bIsCreateDisplayDC, hDCDest, hDCSrc,
				hdcMemory, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom));

			//if (pEProcess)
			//{
			//	KeUnstackDetachProcess(&KApcState);
			//	ObDereferenceObject(pEProcess);
			//}
		}
		else
		{
			bRet = ((NTGDIBITBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiBitBlt].pOrigApiAddress))
				(hDCDest, XDest, YDest, Width, Height, hDCSrc, XSrc, YSrc, ROP, crBackColor, fl);
		}
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
		//KdPrint(("[%d] In Hooked_NtGdiStretchBlt, hdcDst=0x%x, hdcSrc=0x%x\n", 
		//	PsGetCurrentProcessId(), hdcDst, hdcSrc));

		if (IsFilterHDC(hdcDst, NULL) 
			|| IsFilterHDC(hdcSrc, NULL)
			|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
		{
			//RECT rcWnd = g_pDriverHookApiInfos->m_ProtectWndInfo.rcProtectWindow;
			//HDC hdcMemory = g_pDriverHookApiInfos->m_ProtectWndInfo.hDCWndProtect;
			//LONG cx = rcWnd.right - rcWnd.left;
			//LONG cy = rcWnd.bottom - rcWnd.top;

			//BOOL drawProtectResult = ((NTGDISTRETCHBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiBitBlt].pOrigApiAddress))
			//	(hdcDst, rcWnd.left, rcWnd.top, cx, cy,
			//	hdcMemory, 0, 0, cx, cy, SRCCOPY, dwBackColor);

			//KdPrint(("!!! in Hooked_NtGdiStretchBlt, hdcMemory =0x%x, rcProtect={%d,%d -- %d,%d}, BitBltMemory=%d, Reason=%d\n", 
			//	hdcMemory, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, drawProtectResult, 0));

		}
		else
		{
			bRet = ((NTGDISTRETCHBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiStretchBlt].pOrigApiAddress))
				(hdcDst, xDst, yDst, cxDst, cyDst, hdcSrc, xSrc, ySrc, cxSrc, cySrc, dwRop, dwBackColor);
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return bRet;
}

BOOL Hooked_NtGdiPlgBlt(HDC hdcTrg, LPPOINT pptlTrg, 
						HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, 
						HBITMAP hbmMask, INT xMask, INT yMask, DWORD crBackColor)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("Enter Hooked_NtGdiPlgBlt, hdcTrg=0x%x, hdcSrc=0x%x\n", hdcTrg, hdcSrc));

		bRet = ((NTGDIPLGBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiPlgBlt].pOrigApiAddress))
			(hdcTrg, pptlTrg, hdcSrc, xSrc, ySrc, cxSrc, cySrc,
			hbmMask, xMask, yMask, crBackColor);

		//KdPrint(("Leave Hooked_NtGdiPlgBlt, hdcTrg=0x%x, hdcSrc=0x%x\n", hdcTrg, hdcSrc));
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

BOOL Hooked_NtGdiMaskBlt(HDC hdc, INT xDst, INT yDst, INT cx, INT cy, 
						 HDC hdcSrc, INT xSrc, INT ySrc, 
						 HBITMAP hbmMask, INT xMask, INT yMask, 
						 DWORD dwRop4, DWORD crBackColor)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("Enter Hooked_NtGdiMaskBlt\n"));
		
		bRet = ((NTGDIMASKBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiMaskBlt].pOrigApiAddress))
			(hdc, xDst, yDst, cx, cy, hdcSrc, xSrc, ySrc,
			hbmMask, xMask, yMask, dwRop4, crBackColor);

		//KdPrint(("Leave Hooked_NtGdiMaskBlt\n"));
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

BOOL Hooked_NtGdiTransparentBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
								HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, 
								COLORREF TransColor)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("Enter Hooked_NtGdiTransparentBlt\n"));

		bRet = ((NTGDITRANSPARENTBLT)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiTransparentBlt].pOrigApiAddress))
			(hdcDst, xDst, yDst, cxDst, cyDst, 
			hdcSrc, xSrc, ySrc, cxSrc, cySrc,
			TransColor);

		//KdPrint(("Leave Hooked_NtGdiTransparentBlt\n"));
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;

}

BOOL Hooked_NtGdiExtTextOutW(HDC hDC, INT XStart, INT YStart, UINT fuOptions,
							 LPRECT UnsafeRect, LPWSTR UnsafeString, INT Count, 
							 LPINT UnsafeDx, DWORD dwCodePage)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		bRet = (NTGDIEXTTEXTOUTW(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiExtTextOutW].pOrigApiAddress))
			(hDC, XStart, YStart, fuOptions, UnsafeRect, UnsafeString, Count, UnsafeDx, dwCodePage);

		//KdPrint(("[%d], In Hooked_NtGdiExtTextOutW hDC=0x%x, Start[%d, %d], codePage=%d, nCount=%d, String=%ws\n", 
		//	PsGetCurrentProcessId(), hDC, XStart, YStart, dwCodePage, Count,
		//	UnsafeString ? UnsafeString : L"None"));
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
		hDC = (NTGDIOPENDCW(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiOpenDCW].pOrigApiAddress))(Device, InitData, pustrLogAddr, iType, hspool, pDriverInfo2, pUMdhpdev);

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
		//KdPrint(("[%d], In Hooked_NtGdiDeleteObjectApp hDC=0x%x\n", 
		//	PsGetCurrentProcessId(), DCHandle));

		bRet = g_pDriverHookApiInfos->RemoveCreatedDisplayDC(DCHandle);
		bRet = ((NTGDIDELETEOBJECTAPP)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiDeleteObjectApp].pOrigApiAddress))(DCHandle);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

//使用 PrintWindow 进行 后台截图
//PrintWindow(NtUserPrintWindow)->xxxPrintWindow->(SetRedirectedWindow->CreateRedirectionBitmap)
// ->xxxUpdateWindow->_GetDCEx->NtGdiBitBlt
//http://bbs.csdn.net/topics/380255549?page=1#post-393923087
BOOL Hooked_NtUserPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("[%d], In Hooked_NtUserPrintWindow, hWnd=0x%x, hdcBlt=0x%x, nFlags=%d\n",
			PsGetCurrentProcessId(), hwnd, hdcBlt, nFlags));

		bRet = ((NTUSERPRINTWINDOW)(g_pDriverHookApiInfos->m_HookFuns[hft_NtUserPrintWindow].pOrigApiAddress))(hwnd, hdcBlt, nFlags);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return bRet;
}

DWORD Hooked_NtGdiDdLock(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip)
{
	DWORD dwResult = 0;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("[%d] In Hooked_NtGdiDdLock, hSurface=0x%x, hdcClip=0x%x\n",
			PsGetCurrentProcessId(), hSurface, hdcClip));

		dwResult = ((NTGDIDDLOCK)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiDdLock].pOrigApiAddress))(hSurface, puLockData, hdcClip);
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return dwResult;
}


NTSTATUS InstallCopyProtectHook(PPROTECT_WND_INFO pProtectWndInfo)
{
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
		KdPrint(("Enter InstallCopyProtectHook, hTargetProcess=%d\n", //, hSelfProcess=%d, WndDesk=0x%x\n",
			g_pDriverHookApiInfos->m_ProtectWndInfo.hTargetProcess
			//g_pDriverHookApiInfos->m_ProtectWndInfo.hSelfProcess,
			//g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop
			));

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
			//__asm int 3
			g_pDriverHookApiInfos->pOrigNtUserCallOneParam = (NTUSERCALLONEPARAM)(GetShadowSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtUserCallOneParam));
			g_pDriverHookApiInfos->pOrigNtGdiCreateCompatibleDC = (NTGDICREATECOMPATIBLEDC)(GetShadowSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiCreateCompatibleDC));
			g_pDriverHookApiInfos->pOrigNtGdiCreateCompatibleBitmap = (NTGDICREATECOMPATIBLEBITMAP)(GetShadowSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiCreateCompatibleBitmap));
			g_pDriverHookApiInfos->pOrigNtGdiSelectBitmap = (NTGDISELECTBITMAP)(GetShadowSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiSelectBitmap));

			for (INT i = 0; i < hft_FunctionCount; i++)
			{
				if (g_pDriverHookApiInfos->m_HookFuns[i].bEnableHook)
				{
					FNT_VERIFY(HookShadowSSDTFunc(&g_pDriverHookApiInfos->m_HookFuns[i]));
				}
			}

			//FNT_VERIFY(g_pDriverHookApiInfos->RefreshMemoryDC());
		}
		KeUnstackDetachProcess(&KApcState);
		ObDereferenceObject(pEProcess);
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

				for (INT i = 0; i < hft_FunctionCount; i++)
				{
					if (g_pDriverHookApiInfos->m_HookFuns[i].bEnableHook)
					{
						FNT_VERIFY(RestoreShadowSSDTFunc(&g_pDriverHookApiInfos->m_HookFuns[i]));
					}
				}
				g_pDriverHookApiInfos->pOrigNtUserCallOneParam = NULL;

				KeUnstackDetachProcess(&KApcState);
				ObDereferenceObject(pEProcess);
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

BOOL IsFilterHDC(HDC hDC, HWND* phWndFromDC)
{
	BOOL bRet = FALSE;
	HWND hWndFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDC, 
		g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);

	if (phWndFromDC)
	{
		*phWndFromDC = hWndFromDC;
	}

	if (hWndFromDC == g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop)
		//|| hWndFromDC == g_pDriverHookApiInfos->m_ProtectWndInfo.hProtectWindow)
	{
		bRet = TRUE;
	}

	return bRet;
}
