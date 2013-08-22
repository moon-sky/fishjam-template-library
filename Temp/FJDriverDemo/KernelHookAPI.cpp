#include "stdafx.h"
#include "KernelHookAPI.h"
#include "KernelHelper.h"

#ifdef FDRIVER_DEMO_PROJECT
#  include "FDriverUtil.h"
#  include "FDriverDemoDefine.h"
#else
#  include "nComicDrvDefine.h"
#endif 
#include <ddrawint.h>

#define ObjectNameInformation  1
#define SystemHandleInformation 0x10
#define HOOK_API_INFO_TAG     (ULONG)0x12345678

//SYSTEM_SERVICE_TABLE *g_pShadowTable = NULL;

//function define -- http://doxygen.reactos.org/
//Windows WIN32K.SYS System Call Table (NT/2000/XP/2003/Vista/2008/7)
//  http://j00ru.vexillium.org/win32k_syscalls/
//  http://j00ru.vexillium.org/win32k_x64/

typedef HDC (*NtGdiCreateCompatibleDC)(HDC hdc);
typedef HBITMAP (*NtGdiCreateCompatibleBitmap)(HDC hDC, INT Width, INT Height);
typedef HBITMAP (*NtGdiSelectBitmap)(HDC hdc, HBITMAP hbmp)	;
typedef HRGN  (*NtGdiCreateRectRgn)(INT LeftRect, INT TopRect, INT RightRect, INT BottomRect);
typedef BOOL (*NtGdiFillRgn)(HDC hDC, HRGN hRgn, HBRUSH hBrush);
typedef HBRUSH (*NtGdiCreateSolidBrush) ( COLORREF 	Color, IN OPTIONAL HBRUSH 	hbr);

typedef DWORD_PTR (*NtUserCallOneParam)(DWORD_PTR Param, DWORD Routine);

typedef BOOL (*NtGdiBitBlt)(HDC hDCDest, int XDest, int YDest, int Width, int Height,
						    HDC hDCSrc, int XSrc, int YSrc,	
						    ULONG  ROP,ULONG crBackColor,ULONG fl);

typedef BOOL (*NtGdiStretchBlt)(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
								HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
								DWORD dwRop, DWORD dwBackColor);

typedef BOOL (*NtGdiPlgBlt)(HDC hdcDst, LPPOINT pptlTrg, 
							HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
							HBITMAP hbmMask, INT xMask, INT yMask, DWORD crBackColor);

typedef BOOL (*NtGdiMaskBlt)(HDC hdcDst, INT xDst, INT yDst, INT cx, INT cy,
							 HDC hdcSrc, INT xSrc, INT ySrc, 
							 HBITMAP hbmMask, INT xMask, INT yMask, 
							 DWORD dwRop4, DWORD crBackColor);

typedef BOOL (*NtGdiTransparentBlt)(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst,
									HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
									COLORREF TransColor);


typedef BOOL (*NtGdiExtTextOutW)(HDC hDC, INT XStart, INT YStart, UINT fuOptions,
								 LPRECT UnsafeRect, LPWSTR UnsafeString, INT Count, 
								 LPINT UnsafeDx, DWORD dwCodePage);

typedef HDC (*NtGdiOpenDCW)( PUNICODE_STRING pustrDevice, DEVMODEW * pdm, PUNICODE_STRING pustrLogAddr, ULONG 	iType,
                 BOOL bDisplay, HANDLE hspool, VOID *pDriverInfo2, VOID *pUMdhpdev);

typedef HDC (*NtGdiOpenDCW_Win8)( PUNICODE_STRING pustrDevice, DEVMODEW * pdm, PUNICODE_STRING pustrLogAddr, ULONG 	iType,
                            BOOL bDisplay, BOOL someParam, HANDLE hspool, VOID *pDriverInfo2, VOID *pUMdhpdev);

typedef BOOL (*NtGdiDeleteObjectApp)(HANDLE hobj);

typedef BOOL (*NtUserPrintWindow)(HWND 	hwnd, HDC hdcBlt, UINT nFlags);


typedef DWORD (*NtGdiDdLock)(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip);
typedef DWORD (*NtGdiDdUnlock)(HANDLE hSurface, PDD_UNLOCKDATA puUnlockData);

#define MAX_TRACE_CREATE_DISPLAY_DC_COUNT	1

BOOL DrawFilterWnd(HDC hDCDest);

BOOL Hooked_NtGdiBitBlt(
						HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
						HDC hDCSrc, int  XSrc, int  YSrc,	
						ULONG  ROP, ULONG crBackColor,ULONG fl);

BOOL Hooked_NtGdiStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst, 
									 HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
									 DWORD dwRop, DWORD dwBackColor);

BOOL Hooked_NtGdiPlgBlt(HDC hdcDst, LPPOINT pptlTrg, 
							HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
							HBITMAP hbmMask, INT xMask, INT yMask, DWORD crBackColor);

BOOL Hooked_NtGdiMaskBlt(HDC hdcDst, INT xDst, INT yDst, INT cx, INT cy,
						HDC hdcSrc, INT xSrc, INT ySrc, 
						HBITMAP hbmMask, INT xMask, INT yMask, 
						DWORD dwRop4, DWORD crBackColor);

BOOL Hooked_NtGdiTransparentBlt(HDC hdcDst, INT xDst, INT yDst, INT cxDst, INT cyDst,
								   HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
								   COLORREF TransColor);

BOOL Hooked_NtGdiExtTextOutW(HDC hDC, INT XStart, INT YStart, UINT fuOptions,
								 LPRECT UnsafeRect, LPWSTR UnsafeString, INT Count, 
								 LPINT UnsafeDx, DWORD dwCodePage);

HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING pustrDevice, DEVMODEW * pdm, PUNICODE_STRING pustrLogAddr, ULONG 	iType,
                            BOOL bDisplay, HANDLE hspool, VOID *pDriverInfo2, VOID *pUMdhpdev);

//TODO: someParam
HDC Hooked_NtGdiOpenDCW_Win8( PUNICODE_STRING pustrDevice, DEVMODEW * pdm, PUNICODE_STRING pustrLogAddr, ULONG 	iType,
                        BOOL bDisplay, BOOL someParam, HANDLE hspool, VOID *pDriverInfo2, VOID *pUMdhpdev); //TODO: someParam

BOOL Hooked_NtGdiDeleteObjectApp(HDC  DCHandle);

BOOL Hooked_NtUserPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags);

DWORD Hooked_NtGdiDdLock(HANDLE hSurface, PDD_LOCKDATA puLockData, HDC hdcClip);
DWORD Hooked_NtGdiDdUnlock(HANDLE hSurface, PDD_UNLOCKDATA puUnlockData);

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
	hft_NtGdiDdUnlock,

	hft_FunctionCount,
};

typedef struct _DRIVER_HOOK_API_INFOS
{
public:
    HANDLE                  m_hTargetProcess;
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

	LONG					IndexOfNtGdiCreateRectRgn;
	LONG					IndexOfNtGdiFillRgn;
	LONG					IndexOfNtGdiCreateSolidBrush;
	LONG					IndexOfNtGdiDeleteObjectApp;

    LONG					ONEPARAM_ROUTINE_WINDOWFROMDC;  //http://www.reactos.org/wiki/Techwiki:Win32k/apfnSimpleCall

	ULONG					nPatchSizeGdiBltBlt;
	
	NtGdiSelectBitmap		pOrigNtGdiSelectBitmap;
	NtGdiCreateCompatibleBitmap pOrigNtGdiCreateCompatibleBitmap;
	NtGdiCreateCompatibleDC pOrigNtGdiCreateCompatibleDC;
	NtGdiCreateRectRgn		pOrigNtGdiCreateRectRgn;
	NtGdiFillRgn			pOrigNtGdiFillRgn;
	NtGdiCreateSolidBrush	pOrigNtGdiCreateSolidBrush;
	NtGdiDeleteObjectApp	pOrigNtGdiDeleteObjectApp;


    NtUserCallOneParam      pOrigNtUserCallOneParam;

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
				KdPrint(("Running on Windows XP 32\n"));
				m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x100D;				//13,  100D 0008:A001B344 params=0B NtGdiBitBlt 
				m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x1124;
				m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x10ED;
				m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x10E3;
				m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x112A;
				m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1092;

				m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10E9;
				m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x107A;

				m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x11dd;
				m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1053;
				m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x1061;

				IndexOfNtGdiCreateCompatibleDC = 0x101E;
				IndexOfNtGdiCreateCompatibleBitmap = 0x101D;
				IndexOfNtGdiSelectBitmap = 0x1101;
				IndexOfNtUserCallOneParam = 0x1143;      //323;
				
				IndexOfNtGdiCreateRectRgn = 0x1029;
				IndexOfNtGdiFillRgn = 0x1094;
				IndexOfNtGdiCreateSolidBrush = 0x102c;
				IndexOfNtGdiDeleteObjectApp =  0x107a;

				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f; //不要加 0x1000
				break;
			}
        case 502:   //Win2003
            {
                KdPrint(("Running on Win2003 32\n"));

                m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x100d;
                m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x1123;
                m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x10ec;
                m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x10e2;
                m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1129;
                m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1092;

                m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10e8;
                m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x107a;

                m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x11dc;
                m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1053;
				m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x1061;

                IndexOfNtGdiCreateCompatibleDC = 0x101e;
                IndexOfNtGdiCreateCompatibleBitmap = 0x101d;
                IndexOfNtGdiSelectBitmap = 0x1100;
                IndexOfNtUserCallOneParam = 0x1142;

				IndexOfNtGdiCreateRectRgn = 0x1029;
				IndexOfNtGdiFillRgn = 0x1094;
				IndexOfNtGdiCreateSolidBrush = 0x102c;
				IndexOfNtGdiDeleteObjectApp =  0x107a;

                ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f; //不要加 0x1000

                break;
            }
        case 600:   //Vista
            {
                KdPrint(("Running on Vista 32\n"));

                m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x100d;
                m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x112d;
                m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x10f5;
                m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x10eb;
                m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1133;
                m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1095;

                m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10f1;
                m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x107c;

                m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x11f3;
                m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1055;
				m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x1063;

                IndexOfNtGdiCreateCompatibleDC = 0x101f;
                IndexOfNtGdiCreateCompatibleBitmap = 0x101e;
                IndexOfNtGdiSelectBitmap = 0x1109;
                IndexOfNtUserCallOneParam = 0x114d;

				IndexOfNtGdiCreateRectRgn = 0x102b;
				IndexOfNtGdiFillRgn = 0x1097;
				IndexOfNtGdiCreateSolidBrush = 0x102e;
				IndexOfNtGdiDeleteObjectApp =  0x107c;

                ONEPARAM_ROUTINE_WINDOWFROMDC = 0x23; //不要加 0x1000

                break;
            }
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7 32\n"));
				m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x100E;
				m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x112e;
				m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x10F7;
				m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x10ED;
				m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1134;
				m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1095;

				m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10F3;
				m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x107D;

				m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x11FE;
				m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1056;
				m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x1064;

                IndexOfNtGdiCreateCompatibleDC = 0x1020;
                IndexOfNtGdiCreateCompatibleBitmap = 0x101f;
                IndexOfNtGdiSelectBitmap = 0x110b;
				IndexOfNtUserCallOneParam = 0x114e;

				IndexOfNtGdiCreateRectRgn = 0x102c;
				IndexOfNtGdiFillRgn = 0x1097;
				IndexOfNtGdiCreateSolidBrush = 0x102f;
				IndexOfNtGdiDeleteObjectApp =  0x107d;

				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x24; ////不要加 0x1000
				break;
			}
        //case xxxx:    //Windows 2008 Server
        //    {
        //        KdPrint(("Running on Windows 2008 Server 32\n"));
        //        break;
        //    }
        case 602:   //Win8
            {
                KdPrint(("Running on Windows 8 32\n"));

                m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x112E;
                m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x100C;
                m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x1043;
                m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x104D;
                m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1006;
                m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x10A5;

                m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x1047;
                m_HookFuns[hft_NtGdiOpenDCW].nParamCount = 9;
                m_HookFuns[hft_NtGdiOpenDCW].pNewApiAddress = Hooked_NtGdiOpenDCW_Win8;
                m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x10BD;

                m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x11E7;
                m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x10C3;
                m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x10CB;

                IndexOfNtGdiCreateCompatibleDC = 0x111B;
                IndexOfNtGdiCreateCompatibleBitmap = 0x111C;
                IndexOfNtGdiSelectBitmap = 0x102F;
                IndexOfNtUserCallOneParam = 0x115D;

                IndexOfNtGdiCreateRectRgn = 0x110E;
                IndexOfNtGdiFillRgn = 0x10A3;
                IndexOfNtGdiCreateSolidBrush = 0x110B;
                IndexOfNtGdiDeleteObjectApp =  0x10BD;

                ONEPARAM_ROUTINE_WINDOWFROMDC = 0x26; ////不要加 0x1000
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

                m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x1008;
                m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x1030;
                m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x11f0;
                m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x1068;
                m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1219;
                m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1037;

                m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10de;
                m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x1022;

                m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x1265;
                m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1164;
                m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x116f;

                IndexOfNtGdiCreateCompatibleDC = 0x1054;
                IndexOfNtGdiCreateCompatibleBitmap = 0x104a;
                IndexOfNtGdiSelectBitmap = 0x100b;
                IndexOfNtUserCallOneParam = 0x1002;      //323;

                IndexOfNtGdiCreateRectRgn = 0x1085;
                IndexOfNtGdiFillRgn = 0x10d8;
                IndexOfNtGdiCreateSolidBrush = 0x10bc;
                IndexOfNtGdiDeleteObjectApp =  0x1022;

                ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f;   //不要加 0x1000

                break;
			}
        case 502:   //Win2003
            {
                KdPrint(("Running on Win2003 64\n"));

                m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x1008;
                m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x1030;
                m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x11f0;
                m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x1068;
                m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1219;
                m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1037;

                m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10de;
                m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x1022;

                m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x1265;
                m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x1164;
                m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x116f;

                IndexOfNtGdiCreateCompatibleDC = 0x1054;
                IndexOfNtGdiCreateCompatibleBitmap = 0x104a;
                IndexOfNtGdiSelectBitmap = 0x100b;
                IndexOfNtUserCallOneParam = 0x1002;

                IndexOfNtGdiCreateRectRgn = 0x1085;
                IndexOfNtGdiFillRgn = 0x10d8;
                IndexOfNtGdiCreateSolidBrush = 0x10bc;
                IndexOfNtGdiDeleteObjectApp =  0x1022;

                ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f; //不要加 0x1000

                break;
            }
        case 600:   //Vista
            {
                KdPrint(("Running on Vista 64\n"));

                m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x1008;
                m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x1031;
                m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x123e;
                m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x1069;
                m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1268;
                m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1038;

                m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10df;
                m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x1023;

                m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x12c7;
                m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x11a5;
                m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x11b0;

                IndexOfNtGdiCreateCompatibleDC = 0x1055;
                IndexOfNtGdiCreateCompatibleBitmap = 0x104b;
                IndexOfNtGdiSelectBitmap = 0x100b;
                IndexOfNtUserCallOneParam = 0x1002;

                IndexOfNtGdiCreateRectRgn = 0x1086;
                IndexOfNtGdiFillRgn = 0x10d9;
                IndexOfNtGdiCreateSolidBrush = 0x10bd;
                IndexOfNtGdiDeleteObjectApp =  0x1023;

                ONEPARAM_ROUTINE_WINDOWFROMDC = 0x23; //不要加 0x1000

                break;
            }
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7 64\n"));

				m_HookFuns[hft_NtGdiBitBlt].nIndexInSSDT = 0x1008;
				m_HookFuns[hft_NtGdiStretchBlt].nIndexInSSDT = 0x1031;
				m_HookFuns[hft_NtGdiPlgBlt].nIndexInSSDT = 0x124a;
				m_HookFuns[hft_NtGdiMaskBlt].nIndexInSSDT = 0x1069;
				m_HookFuns[hft_NtGdiTransparentBlt].nIndexInSSDT = 0x1275;
				m_HookFuns[hft_NtGdiExtTextOutW].nIndexInSSDT = 0x1038;

				m_HookFuns[hft_NtGdiOpenDCW].nIndexInSSDT = 0x10da;
				m_HookFuns[hft_NtGdiDeleteObjectApp].nIndexInSSDT = 0x1023;

				m_HookFuns[hft_NtUserPrintWindow].nIndexInSSDT = 0x12eb;
				m_HookFuns[hft_NtGdiDdLock].nIndexInSSDT = 0x11ab;
				m_HookFuns[hft_NtGdiDdUnlock].nIndexInSSDT = 0x11b8;

                IndexOfNtGdiCreateCompatibleDC = 0x1055;
                IndexOfNtGdiCreateCompatibleBitmap = 0x104b;
                IndexOfNtGdiSelectBitmap = 0x100b;
                IndexOfNtUserCallOneParam = 0x1002;

                IndexOfNtGdiCreateRectRgn = 0x1084;
                IndexOfNtGdiFillRgn = 0x10d4;
                IndexOfNtGdiCreateSolidBrush = 0x10b8;
                IndexOfNtGdiDeleteObjectApp =  0x1023;

				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x24;	//不要加 0x1000
				break;
			}
        //case 602:   //Win8
        //    {
        //        KdPrint(("Running on Windows 8 64\n"));
        //        break;
        //    }
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
		m_HookFuns[hft_NtGdiPlgBlt].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiMaskBlt].pwzApiName = L"NtGdiMaskBlt";
		m_HookFuns[hft_NtGdiMaskBlt].pNewApiAddress = Hooked_NtGdiMaskBlt;
		m_HookFuns[hft_NtGdiMaskBlt].nParamCount = 13;
		m_HookFuns[hft_NtGdiMaskBlt].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiTransparentBlt].pwzApiName = L"NtGdiTransparentBlt";
		m_HookFuns[hft_NtGdiTransparentBlt].pNewApiAddress = Hooked_NtGdiTransparentBlt;
		m_HookFuns[hft_NtGdiTransparentBlt].nParamCount = 11;
		m_HookFuns[hft_NtGdiTransparentBlt].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiExtTextOutW].pwzApiName = L"NtGdiExtTextOutW";
		m_HookFuns[hft_NtGdiExtTextOutW].pNewApiAddress = Hooked_NtGdiExtTextOutW;
		m_HookFuns[hft_NtGdiExtTextOutW].nParamCount = 9;
		m_HookFuns[hft_NtGdiExtTextOutW].bEnableHook = FALSE;

		m_HookFuns[hft_NtGdiOpenDCW].pwzApiName = L"NtGdiOpenDCW";
		m_HookFuns[hft_NtGdiOpenDCW].pNewApiAddress = Hooked_NtGdiOpenDCW;
		m_HookFuns[hft_NtGdiOpenDCW].nParamCount = 8;
		m_HookFuns[hft_NtGdiOpenDCW].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiDeleteObjectApp].pwzApiName = L"NtGdiDeleteObjectApp";
		m_HookFuns[hft_NtGdiDeleteObjectApp].pNewApiAddress = Hooked_NtGdiDeleteObjectApp;
		m_HookFuns[hft_NtGdiDeleteObjectApp].nParamCount = 1;
		m_HookFuns[hft_NtGdiDeleteObjectApp].bEnableHook = TRUE;

		m_HookFuns[hft_NtUserPrintWindow].pwzApiName = L"NtUserPrintWindow";
		m_HookFuns[hft_NtUserPrintWindow].pNewApiAddress = Hooked_NtUserPrintWindow;
		m_HookFuns[hft_NtUserPrintWindow].nParamCount = 3;
		m_HookFuns[hft_NtUserPrintWindow].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiDdLock].pwzApiName = L"NtGdiDdLock";
		m_HookFuns[hft_NtGdiDdLock].pNewApiAddress = Hooked_NtGdiDdLock;
		m_HookFuns[hft_NtGdiDdLock].nParamCount = 3;
		m_HookFuns[hft_NtGdiDdLock].bEnableHook = TRUE;

		m_HookFuns[hft_NtGdiDdUnlock].pwzApiName = L"NtGdiDdUnlock";
		m_HookFuns[hft_NtGdiDdUnlock].pNewApiAddress = Hooked_NtGdiDdUnlock;
		m_HookFuns[hft_NtGdiDdUnlock].nParamCount = 2;
		m_HookFuns[hft_NtGdiDdUnlock].bEnableHook = TRUE;

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


BOOL DrawFilterWnd(HDC hDCDest)
{
    BOOL bRet = FALSE;
    NT_ASSERT(g_pDriverHookApiInfos->pOrigNtGdiFillRgn);

    //Skip 
    RECT rcWnd = g_pDriverHookApiInfos->m_ProtectWndInfo.rcProtectWindow;
    HDC hdcMemory = g_pDriverHookApiInfos->m_ProtectWndInfo.hDCWndProtect;

    HRGN hRgn = g_pDriverHookApiInfos->pOrigNtGdiCreateRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);
    HBRUSH hBrush = g_pDriverHookApiInfos->pOrigNtGdiCreateSolidBrush(RGB(111, 111, 111), NULL);
    bRet = g_pDriverHookApiInfos->pOrigNtGdiFillRgn(hDCDest,hRgn, hBrush);  
    KdPrint(("NtGdiFillRgn hRgn in DrawFilterWnd [%d]\n", bRet));

    bRet = g_pDriverHookApiInfos->pOrigNtGdiDeleteObjectApp(hRgn);
    KdPrint(("DeleteObjectapp hRgn [%d]\n", bRet));
    bRet = g_pDriverHookApiInfos->pOrigNtGdiDeleteObjectApp(hBrush);//todo: it is success to delete hRgn, but failed to delete hbrush
    KdPrint(("DeleteObjectapp hBrush [%d]\n", bRet));

    return bRet;
}


BOOL Hooked_NtGdiBitBlt(
					 HDC hdcDst,int  XDest,int  YDest,int  Width,int  Height,
					 HDC hdcSrc, int  XSrc, int  YSrc,	
					 ULONG  ROP, ULONG crBackColor, ULONG fl)
{
	BOOL bRet = TRUE;
	NTSTATUS status = STATUS_SUCCESS;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		HWND hWndFromDest = NULL;
		HWND hWndFromSrc = NULL;
		BOOL bIsFilterDCDest = IsFilterHDC(hdcDst, &hWndFromDest);
		BOOL bIsFilterDCSrc = IsFilterHDC(hdcSrc, &hWndFromSrc);
		BOOL bIsCreateDisplayDC = g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc);
		NtGdiBitBlt pOrigNtGdiBitBlt = (NtGdiBitBlt)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiBitBlt].pOrigApiAddress);
		if (pOrigNtGdiBitBlt)
		{
            if (bIsFilterDCSrc || bIsFilterDCDest
				|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
			{
                KdPrint(("!!! In Hooked_NtGdiBitBlt, hdcDst=0x%x, hdcSrc=0x%x\n", hdcDst, hdcSrc));
                //DrawFilterWnd(hdcDst);
			}
			else
			{
                bRet = (pOrigNtGdiBitBlt)
                    (hdcDst, XDest, YDest, Width, Height, hdcSrc, XSrc, YSrc, ROP, crBackColor, fl);
			}
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
		NtGdiStretchBlt pOrigNtGdiStretchBlt= (NtGdiStretchBlt)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiStretchBlt].pOrigApiAddress);
		if (pOrigNtGdiStretchBlt)
		{
			//KdPrint(("[%d] In Hooked_NtGdiStretchBlt, hdcDst=0x%x, hdcSrc=0x%x\n", 
			//	PsGetCurrentProcessId(), hdcDst, hdcSrc));
			if (IsFilterHDC(hdcDst, NULL) 
				|| IsFilterHDC(hdcSrc, NULL)
				|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
			{
                KdPrint(("!!! In Hooked_NtGdiStretchBlt, hdcDst=0x%x, hdcSrc=0x%x\n", hdcDst, hdcSrc));
                //DrawFilterWnd(hdcDst);
			}
            else
            {
                bRet = (pOrigNtGdiStretchBlt)
                    (hdcDst, xDst, yDst, cxDst, cyDst, hdcSrc, xSrc, ySrc, cxSrc, cySrc, dwRop, dwBackColor);
            }
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return bRet;
}

BOOL Hooked_NtGdiPlgBlt(HDC hdcDst, LPPOINT pptlTrg, 
						HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc, 
						HBITMAP hbmMask, INT xMask, INT yMask, DWORD crBackColor)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		NtGdiPlgBlt pOrigNtGdiPlgBlt = ((NtGdiPlgBlt)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiPlgBlt].pOrigApiAddress));
		if (pOrigNtGdiPlgBlt)
		{
			if (IsFilterHDC(hdcDst, NULL) 
				|| IsFilterHDC(hdcSrc, NULL)
				|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
			{
                KdPrint(("!!! In Hooked_NtGdiPlgBlt, hdcDst=0x%x, hdcSrc=0x%x\n", hdcDst, hdcSrc));
                //DrawFilterWnd(hdcDst);
			}
            else
            {
                bRet = (pOrigNtGdiPlgBlt)
                    (hdcDst, pptlTrg, hdcSrc, xSrc, ySrc, cxSrc, cySrc,
                    hbmMask, xMask, yMask, crBackColor);
            }
		}
		//KdPrint(("Leave Hooked_NtGdiPlgBlt, hdcTrg=0x%x, hdcSrc=0x%x\n", hdcTrg, hdcSrc));
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

BOOL Hooked_NtGdiMaskBlt(HDC hdcDst, INT xDst, INT yDst, INT cx, INT cy, 
						 HDC hdcSrc, INT xSrc, INT ySrc, 
						 HBITMAP hbmMask, INT xMask, INT yMask, 
						 DWORD dwRop4, DWORD crBackColor)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		NtGdiMaskBlt pOrigNtGdiMaskBlt = ((NtGdiMaskBlt)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiMaskBlt].pOrigApiAddress));
		if (pOrigNtGdiMaskBlt)
		{
			if (IsFilterHDC(hdcDst, NULL) 
				|| IsFilterHDC(hdcSrc, NULL)
				|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
			{
                KdPrint(("!!! In Hooked_NtGdiMaskBlt, hdcDst=0x%x, hdcSrc=0x%x\n", hdcDst, hdcSrc));
                //DrawFilterWnd(hdcDst);
			}
            else
            {
                bRet = (pOrigNtGdiMaskBlt)
                    (hdcDst, xDst, yDst, cx, cy, hdcSrc, xSrc, ySrc,
                    hbmMask, xMask, yMask, dwRop4, crBackColor);
            }
		}
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
		NtGdiTransparentBlt pOrigNtGdiTransparentBlt = (NtGdiTransparentBlt)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiTransparentBlt].pOrigApiAddress);
		if (pOrigNtGdiTransparentBlt)
		{
			if (IsFilterHDC(hdcDst, NULL) 
				|| IsFilterHDC(hdcSrc, NULL)
				|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcSrc))
			{
                KdPrint(("!!! In Hooked_NtGdiTransparentBlt, hdcDst=0x%x, hdcSrc=0x%x\n", hdcDst, hdcSrc));
                //DrawFilterWnd(hdcDst);
			}
            else
            {
                bRet = (pOrigNtGdiTransparentBlt)
                    (hdcDst, xDst, yDst, cxDst, cyDst, 
                    hdcSrc, xSrc, ySrc, cxSrc, cySrc,
                    TransColor);
            }
		}
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
		NtGdiExtTextOutW pOrigNtGdiExtTextOutW = (NtGdiExtTextOutW)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiExtTextOutW].pOrigApiAddress);
		if (pOrigNtGdiExtTextOutW)
		{
			bRet = (pOrigNtGdiExtTextOutW)
				(hDC, XStart, YStart, fuOptions, UnsafeRect, UnsafeString, Count, UnsafeDx, dwCodePage);
		}
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

HDC Hooked_NtGdiOpenDCW( PUNICODE_STRING pustrDevice, DEVMODEW * pdm, PUNICODE_STRING pustrLogAddr, ULONG 	iType,
                        BOOL bDisplay, HANDLE hspool, VOID *pDriverInfo2, VOID *pUMdhpdev)
{
	HDC hDC = NULL;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		NtGdiOpenDCW pOrigNtGdiOpenDCW = (NtGdiOpenDCW)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiOpenDCW].pOrigApiAddress);

		if (pOrigNtGdiOpenDCW)
		{
			hDC = (pOrigNtGdiOpenDCW)(pustrDevice, pdm, pustrLogAddr, iType, bDisplay, hspool, pDriverInfo2, pUMdhpdev);
		}
		//KdPrint(("[%d], In Hooked_NtGdiOpenDCW Device=%wZ, hDC=0x%x\n", 
		//	PsGetCurrentProcessId(), Device, hDC));
		if (!pustrDevice)
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

HDC Hooked_NtGdiOpenDCW_Win8( PUNICODE_STRING pustrDevice, DEVMODEW * pdm, PUNICODE_STRING pustrLogAddr, ULONG 	iType,
                             BOOL bDisplay, BOOL someParam, HANDLE hspool, VOID *pDriverInfo2, VOID *pUMdhpdev)
{
    HDC hDC = NULL;
    SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
    __try
    {
        NtGdiOpenDCW_Win8 pOrigNtGdiOpenDCW = (NtGdiOpenDCW_Win8)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiOpenDCW].pOrigApiAddress);

        if (pOrigNtGdiOpenDCW)
        {
            hDC = (pOrigNtGdiOpenDCW)(pustrDevice, pdm, pustrLogAddr, iType, bDisplay, someParam, hspool, pDriverInfo2, pUMdhpdev);
        }
        //KdPrint(("[%d], In Hooked_NtGdiOpenDCW Device=%wZ, hDC=0x%x\n", 
        //	PsGetCurrentProcessId(), Device, hDC));
        if (!pustrDevice)
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

		NtGdiDeleteObjectApp pOrigNtGdiDeleteObjectApp = (NtGdiDeleteObjectApp)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiDeleteObjectApp].pOrigApiAddress);
		if (pOrigNtGdiDeleteObjectApp)
		{
			bRet = (pOrigNtGdiDeleteObjectApp)(DCHandle);
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}

	return bRet;
}

//使用 PrintWindow 进行 后台截图 -- Snagit 的滚动截图会用到
//PrintWindow(NtUserPrintWindow)->xxxPrintWindow->(SetRedirectedWindow->CreateRedirectionBitmap)
// ->xxxUpdateWindow->_GetDCEx->NtGdiBitBlt
//http://bbs.csdn.net/topics/380255549?page=1#post-393923087
BOOL Hooked_NtUserPrintWindow(HWND hwnd, HDC hdcBlt, UINT nFlags)
{
	BOOL bRet = FALSE;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("[%d], In Hooked_NtUserPrintWindow, hWnd=0x%x, hdcBlt=0x%x, nFlags=0x%x\n",
			PsGetCurrentProcessId(), hwnd, hdcBlt, nFlags));

		NtUserPrintWindow pOrigNtUserPrintWindow = (NtUserPrintWindow)(g_pDriverHookApiInfos->m_HookFuns[hft_NtUserPrintWindow].pOrigApiAddress);
		if (pOrigNtUserPrintWindow)
		{
			if (IsFilterHDC(hdcBlt, NULL) 
				|| IsFilterHDC(hdcBlt, NULL)
				|| g_pDriverHookApiInfos->IsCreatedDisplayDC(hdcBlt))
			{
                KdPrint(("!!! In Hooked_NtUserPrintWindow, hdcBlt=0x%x\n", hdcBlt));
                DrawFilterWnd(hdcBlt);
			}
            else
            {
                bRet = (pOrigNtUserPrintWindow)(hwnd, hdcBlt, nFlags);
            }
		}
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
		NtGdiDdLock pOrigNtGdiDdLock = (NtGdiDdLock)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiDdLock].pOrigApiAddress);
		if (pOrigNtGdiDdLock)
		{
			dwResult = (pOrigNtGdiDdLock)(hSurface, puLockData, hdcClip);
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return dwResult;
}

DWORD Hooked_NtGdiDdUnlock(HANDLE hSurface, PDD_UNLOCKDATA puUnlockData)
{
	DWORD dwResult = 0;
	SSDT_API_CALL_ENTER(g_SSDTAPILockCount);
	__try
	{
		KdPrint(("[%d] In Hooked_NtGdiDdUnlock, hSurface=0x%x, puUnlockData=0x%p\n",
			PsGetCurrentProcessId(), hSurface, puUnlockData));

		NtGdiDdUnlock pOrigNtGdiDdUnlock = (NtGdiDdUnlock)(g_pDriverHookApiInfos->m_HookFuns[hft_NtGdiDdUnlock].pOrigApiAddress);
		if (pOrigNtGdiDdUnlock)
		{
			dwResult = (pOrigNtGdiDdUnlock)(hSurface, puUnlockData);
		}
	}
	__finally
	{
		SSDT_API_CALL_LEAVE(g_SSDTAPILockCount);
	}
	return dwResult;
}

NTSTATUS InstallCopyProtectHook(HANDLE hTargetProcess)
{
    NTSTATUS status = STATUS_SUCCESS;
    KdPrint(("Enter InstallCopyProtectHook\n"));

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
        KdPrint(("InstallCopyProtectHook Will Allocate %d size Buffer\n", sizeof(DRIVER_HOOK_API_INFOS)));
        g_pDriverHookApiInfos = (PDRIVER_HOOK_API_INFOS)ExAllocatePoolWithTag(NonPagedPoolCacheAlignedMustS, sizeof(DRIVER_HOOK_API_INFOS), HOOK_API_INFO_TAG);
        NT_ASSERT(g_pDriverHookApiInfos);
        if (g_pDriverHookApiInfos)
        {
            RtlZeroMemory(g_pDriverHookApiInfos, sizeof(DRIVER_HOOK_API_INFOS));
            g_pDriverHookApiInfos->InitValue();
            g_pDriverHookApiInfos->m_hTargetProcess = hTargetProcess;
        }

        PEPROCESS pEProcess = NULL;
        FNT_VERIFY(PsLookupProcessByProcessId(hTargetProcess, &pEProcess));
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
            g_pDriverHookApiInfos->pOrigNtUserCallOneParam = (NtUserCallOneParam)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtUserCallOneParam));
            g_pDriverHookApiInfos->pOrigNtGdiCreateCompatibleDC = (NtGdiCreateCompatibleDC)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiCreateCompatibleDC));
            g_pDriverHookApiInfos->pOrigNtGdiCreateCompatibleBitmap = (NtGdiCreateCompatibleBitmap)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiCreateCompatibleBitmap));
            g_pDriverHookApiInfos->pOrigNtGdiSelectBitmap = (NtGdiSelectBitmap)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiSelectBitmap));

			g_pDriverHookApiInfos->pOrigNtGdiCreateRectRgn = (NtGdiCreateRectRgn)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiCreateRectRgn));
			g_pDriverHookApiInfos->pOrigNtGdiFillRgn = (NtGdiFillRgn)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiFillRgn));
			g_pDriverHookApiInfos->pOrigNtGdiCreateSolidBrush = (NtGdiCreateSolidBrush)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiCreateSolidBrush));
			g_pDriverHookApiInfos->pOrigNtGdiDeleteObjectApp = (NtGdiDeleteObjectApp)(GetSSDTFuncAddr(g_pDriverHookApiInfos->IndexOfNtGdiDeleteObjectApp));

			for (INT i = 0; i < hft_FunctionCount; i++)
			{
				if (g_pDriverHookApiInfos->m_HookFuns[i].bEnableHook)
				{
					FNT_VERIFY(HookSSDTFunc(&g_pDriverHookApiInfos->m_HookFuns[i]));
				}
                //else
                //{
                //    g_pDriverHookApiInfos->m_HookFuns[i].pOrigApiAddress = GetSSDTFuncAddr(g_pDriverHookApiInfos->m_HookFuns[i].nIndexInSSDT);
                //}
			}

            //FNT_VERIFY(g_pDriverHookApiInfos->RefreshMemoryDC());
        }
        KeUnstackDetachProcess(&KApcState);
        ObDereferenceObject(pEProcess);
    }

    KdPrint(("Leave InstallCopyProtectHook, status=0x%x\n"));

    return status;
}

NTSTATUS UpdateCopyProtectWndInfo(PPROTECT_WND_INFO pProtectWndInfo)
{
    NTSTATUS status = STATUS_SUCCESS;
    KdPrint(("Enter InstallCopyProtectHook, pProtectWndInfo=%p\n", pProtectWndInfo));

	if (g_pDriverHookApiInfos)
	{
        if (pProtectWndInfo)
        {
            RtlCopyMemory(&g_pDriverHookApiInfos->m_ProtectWndInfo, pProtectWndInfo, sizeof(PROTECT_WND_INFO));
        }
        else
        {
            RtlZeroMemory(&g_pDriverHookApiInfos->m_ProtectWndInfo, sizeof(PROTECT_WND_INFO));
            //, hTargetProcess=%d\n", //, hSelfProcess=%d, WndDesk=0x%x\n",
            //g_pDriverHookApiInfos->m_ProtectWndInfo.hTargetProcess
            //g_pDriverHookApiInfos->m_ProtectWndInfo.hSelfProcess,
            //g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop
            //));
        }
	}
	return status;
}

NTSTATUS UnInstallCopyProtectHook(void)
{
	NTSTATUS status = STATUS_SUCCESS;

	KdPrint(("Enter UnInstallCopyProtectHook\n"));
	if (g_pDriverHookApiInfos)
	{
		//KdPrint(("ServiceDescriptorShadowTableAddress : %#x\n", g_pDriverHookApiInfos));

		if (g_pDriverHookApiInfos->HasValidCallNumber())// g_pDriverHookApiInfos-> g_NtGdiBitBltIndex != (-1))
		{
			PEPROCESS pEProcess = NULL;
			FNT_VERIFY(PsLookupProcessByProcessId(g_pDriverHookApiInfos->m_hTargetProcess, &pEProcess));
			if (pEProcess)
			{
				KAPC_STATE KApcState = {0};
				KeStackAttachProcess(pEProcess, &KApcState);

				for (INT i = 0; i < hft_FunctionCount; i++)
				{
					if (g_pDriverHookApiInfos->m_HookFuns[i].bEnableHook)
					{
						FNT_VERIFY(RestoreSSDTFunc(&g_pDriverHookApiInfos->m_HookFuns[i]));
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
	DbgPrint("Leave UnInstallCopyProtectHook\n");
	return status;
}

BOOL IsFilterHDC(HDC hDC, HWND* phWndFromDC)
{
	BOOL bRet = FALSE;
    if (g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop != NULL)
    {
        HWND hWndFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDC, 
            g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);

        if (phWndFromDC)
        {
            *phWndFromDC = hWndFromDC;
        }

        if (NULL != hWndFromDC
            && hWndFromDC == g_pDriverHookApiInfos->m_ProtectWndInfo.hWndDeskTop)
            //|| hWndFromDC == g_pDriverHookApiInfos->m_ProtectWndInfo.hProtectWindow)
        {
            bRet = TRUE;
        }
    }

	return bRet;
}
