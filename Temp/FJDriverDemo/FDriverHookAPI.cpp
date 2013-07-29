#include "stdafx.h"
#include "FDriverHookAPI.h"
#include "FDriverUtil.h"
#include "FDriverMemory.h"
#include "FDriverAPI.h"
#include "FDriverDemoDefine.h"

#define ObjectNameInformation  1
#define SystemHandleInformation 0x10
#define HOOK_API_INFO_TAG     (ULONG)0x12345678

SYSTEM_SERVICE_TABLE *g_pShadowTable = NULL;

typedef NTSTATUS (*NTBITBLT)(HDC hDCDest,int  XDest,int  YDest, int  Width,int  Height,
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


typedef NTSTATUS (*NTGDIGETDCDWORD)(HDC hdc,
                                    INT n, 
                                    PDWORD result );

typedef NTSTATUS (*NTUSERCALLONEPARAM)(IN ULONG Param, IN ULONG Routine);

typedef struct _DRIVER_HOOK_API_INFOS
{
public:
	HWND					m_hWndDesktop;
    HWND					m_hWndProtect;
    unsigned long			m_OldCr0;
    BOOL                    m_ValidCallNumber;

public:
    int						IndexOfNtGdiBitBlt;
	//int						IndexOfNtGdiStretchBlt;
	//int                     IndexOfNtGdiGetDCDword;
    int                     IndexOfNtGdiExtTextOutW;
    int                     IndexOfNtUserCallOneParam;

	int						ONEPARAM_ROUTINE_WINDOWFROMDC;

	NTBITBLT				pOrigNtBitBlt;
	NTGDIEXTTEXTOUTW		pOrigNtGdiExtTextOutW;
    NTGDIGETDCDWORD         pOrigNtGdiGetDcDWord;
    NTUSERCALLONEPARAM      pOrigNtUserCallOneParam;

	VOID InitValue()
	{
		m_hWndDesktop = (HWND)0;

        m_hWndProtect = (HWND)0;
        m_OldCr0 = 0;
        m_ValidCallNumber = FALSE;

		IndexOfNtGdiBitBlt = -1;
		//IndexOfNtGdiStretchBlt = -1;
        //IndexOfNtGdiGetDCDword = -1;
        IndexOfNtGdiExtTextOutW = -1;
        IndexOfNtUserCallOneParam = -1;


		ONEPARAM_ROUTINE_WINDOWFROMDC = -1;

		pOrigNtBitBlt = NULL;
		pOrigNtGdiExtTextOutW = NULL;
        pOrigNtGdiGetDcDWord = NULL;
        pOrigNtUserCallOneParam = NULL;
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
                IndexOfNtGdiExtTextOutW = 0x92;
				//IndexOfNtGdiStretchBlt = 0x124; 		//292, 111A 0008:A003022B params=0C NtGdiStretchBlt 
                //IndexOfNtGdiGetDCDword = 167;
                IndexOfNtUserCallOneParam = 0x143;      //323;

				ONEPARAM_ROUTINE_WINDOWFROMDC = 0x1f;	//31
				break;
			}
		case 601:	//Win7
			{
				KdPrint(("Running on Windows 7\n"));
				IndexOfNtGdiBitBlt = 0xE;
				IndexOfNtGdiExtTextOutW = 0x95;
				//IndexOfNtGdiStretchBlt = 0x12e;
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
			NT_ASSERT(nPointSize == 4);
			NT_ASSERT(g_pShadowTable[1].ArgumentsTable[IndexOfNtGdiBitBlt] == (0xB * nPointSize));
			//NT_ASSERT(g_pShadowTable[1].ArgumentsTable[IndexOfNtGdiStretchBlt] == (0xC * nPointSize));
            NT_ASSERT(g_pShadowTable[1].ArgumentsTable[IndexOfNtUserCallOneParam] == (0x2 * nPointSize));

            m_ValidCallNumber = TRUE;
		}

		return status;
	}
}DRIVER_HOOK_API_INFOS, *PDRIVER_HOOK_API_INFOS;




#pragma LOCKEDCODE	
DRIVER_HOOK_API_INFOS* g_pDriverHookApiInfos = NULL;



//int g_NtGdiStretchBltIndex = (-1);

HANDLE g_hProcess = 0;

SCROLL_DATA g_ScrollData = {0};
SCROLL_HOOK_TARGET g_ScrollHookTarget = {0};

//Win32k.sys file, scroll to the target, the target-specific API calls to the process even if one has a value of TRUE.
ULONG g_IsScrolled = FALSE;

// If the current one hooking SSDT API function calls and function call in progress, if this value has a value greater than 0.
ULONG g_SSDTAPILockCount = 0;

//typedef ULONG (*SCROLLWINDOWEX)(HWND hWnd, int dx, int dy, const RECT *prcScroll, const RECT *prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags);
//ULONG (*OrigScrollWindowEx)(HWND hWnd, int dx, int dy, const RECT *prcScroll, const RECT *prcClip, HRGN hrgnUpdate, LPRECT prcUpdate, UINT flags);

//typedef ULONG (*SCROLLDC)(HDC hDC,int dx, int dy, const RECT *lprcScroll, const RECT *lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate);
//ULONG (*OrigScrollDC)(HDC hDC,int dx, int dy, const RECT *lprcScroll, const RECT *lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate);



//typedef ULONG (*NTGDISTRETCHBLT)(HDC hDCDest);
//ULONG (*OrigNtGdiStretchBlt)(HDC hDCDest);


typedef struct _SERVICE_DESCRIPTOR_TABLE *PSERVICE_DESCRIPTOR_TABLE;

extern "C" PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;
extern "C" __declspec(dllimport) KeAddSystemServiceTable(ULONG, ULONG, ULONG, ULONG, ULONG); 

//根据相同版本下与SSDT地址存在的偏移获取的SSDT SHADOW的地址
// WinDbg 下 ?KeServiceDescriptorTable-
// 会打印出：Evaluate expression: 64 = 00000040(XP), -0xE0(2K)
//卡巴斯基有个类似的函数：FindSystemServiceDescriptionTableShadow
SYSTEM_SERVICE_TABLE *GetKeServiceDescriptorTableShadowAddress ()
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

// It is not part of a fairly clean.
// MDL to approach needs to be changed.
VOID  ClearWriteProtect(VOID)
{
	//cli + sti 类似 push eax + pop eax
	__asm
	{
		push  eax;
		mov   eax, cr0;
		and   eax, 0x0FFFEFFFF;		//and     eax, not 10000h
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
		or    eax, not 0x0FFFEFFFF;  //or eax, 10000h
		mov   cr0, eax;
		pop   eax;
	}
}


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

int GetNtGdiStretchBltIndex(SYSTEM_SERVICE_TABLE *p)
{
	/*
	1116 0008:A006FBB5 params=05 NtGdiSetVirtualResolution 
	1117 0008:A006F8BE params=03 NtGdiSetSizeDevice 
	1118 0008:A0071AA4 params=04 NtGdiStartDoc 
	1119 0008:A0070474 params=01 NtGdiStartPage 
	111A 0008:A003022B params=0C NtGdiStretchBlt 
	111B 0008:A0059D6F params=10 NtGdiStretchDIBitsInternal 
	111C 0008:A00BF555 params=01 NtGdiStrokeAndFillPath 
	111D 0008:A0121E76 params=01 NtGdiStrokePath 
	111E 0008:A01367B4 params=01 NtGdiSwapBuffers
	*/

	const int WIN2K_NtGdiSetVirtualResolution = 0x116;
	for (int i = WIN2K_NtGdiSetVirtualResolution; i <= WIN2K_NtGdiSetVirtualResolution + 20; i++)
	{
		KdPrint(("%d", p[1].ArgumentsTable[i] / 4));
		if (
			((p[1].ArgumentsTable[i + 0]) == 20)		//0x116, NtGdiSetVirtualResolution
			&& ((p[1].ArgumentsTable[i + 1]) == 12)		//0x117, NtGdiSetSizeDevice
			&& ((p[1].ArgumentsTable[i + 2]) == 16)		//0x118, NtGdiStartDoc
			&& ((p[1].ArgumentsTable[i + 3]) == 4)		//0x119, NtGdiStartPage
			&& ((p[1].ArgumentsTable[i + 4]) == 48)		//0x11A, NtGdiStretchBlt
			&& ((p[1].ArgumentsTable[i + 5]) == 64)		//0x11B, NtGdiStretchDIBitsInternal
			&& ((p[1].ArgumentsTable[i + 6]) == 4)		//0x11C, NtGdiStrokeAndFillPath
			&& ((p[1].ArgumentsTable[i + 7]) == 4)		//0x11D, NtGdiStrokePath
			&& ((p[1].ArgumentsTable[i + 8]) == 4)		//0x11E, NtGdiSwapBuffers
			)
		{
			KdPrint(("Find NtGdiStretchBlt in Windows XP, 0x%x!!\n", (i+4)));
			return (i + 4);
		}
	}

	return (-1);
}

int GetNtGdiBitBltIndex(SYSTEM_SERVICE_TABLE *p)
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
	return (-1);

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

PSYSTEM_HANDLE_INFORMATION_EX GetInfoTable(ULONG ATableType)
{
	ULONG mSize = 0x4000;
	PSYSTEM_HANDLE_INFORMATION_EX mPtr = NULL;
	NTSTATUS St;
	do
	{
		mPtr = (PSYSTEM_HANDLE_INFORMATION_EX)ExAllocatePool(PagedPool, mSize);
		memset(mPtr, 0, mSize);
		if (mPtr)
		{
			St = ZwQuerySystemInformation(ATableType, mPtr, mSize, NULL);
		} else return NULL;
		if (St == STATUS_INFO_LENGTH_MISMATCH)
		{
			ExFreePool(mPtr);
			mSize = mSize * 2;
		}
	} while (St == STATUS_INFO_LENGTH_MISMATCH);
	if (St == STATUS_SUCCESS) return mPtr;
	ExFreePool(mPtr);
	return NULL;
}

HANDLE GetCsrPid()
{
	HANDLE Process, hObject;
	HANDLE CsrId = (HANDLE)0;
	OBJECT_ATTRIBUTES obj;
	CLIENT_ID cid;
	UCHAR Buff[0x100];
	POBJECT_NAME_INFORMATION ObjName = (POBJECT_NAME_INFORMATION)&Buff;
	PSYSTEM_HANDLE_INFORMATION_EX Handles;
	ULONG r;

	Handles = GetInfoTable(SystemHandleInformation);

	if (!Handles) return CsrId;

	for (r = 0; r < Handles->NumberOfHandles; r++)
	{
		if (Handles->Information[r].ObjectTypeNumber == 21) //Port object
		{
			InitializeObjectAttributes(&obj, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

			cid.UniqueProcess = (HANDLE)Handles->Information[r].ProcessID;
			cid.UniqueThread = 0;

			if (NT_SUCCESS(NtOpenProcess(&Process, PROCESS_DUP_HANDLE, &obj, &cid)))
			{
				if (NT_SUCCESS(ZwDuplicateObject(Process, (HANDLE)Handles->Information[r].Handle,NtCurrentProcess(), &hObject, 0, 0, DUPLICATE_SAME_ACCESS)))
				{
					if (NT_SUCCESS(ZwQueryObject(hObject, (OBJECT_INFORMATION_CLASS)ObjectNameInformation, 
						ObjName, 0x100, NULL)))
					{
						if (ObjName->Name.Buffer && !wcsncmp(L"\\Windows\\ApiPort", ObjName->Name.Buffer, 20))
						{
							CsrId = (HANDLE)Handles->Information[r].ProcessID;
						} 
					}

					ZwClose(hObject);
				}

				ZwClose(Process);
			}
		}
	}

	ExFreePool(Handles);
	return CsrId;
}

//ULONG MyScrollDC(HDC hDC, int dx, int dy, const RECT *lprcScroll, const RECT *lprcClip, HRGN hrgnUpdate, LPRECT lprcUpdate)
//{
//	ULONG uResult = OrigScrollDC(hDC, dx, dy, lprcScroll, lprcClip, hrgnUpdate, lprcUpdate);
//	KdPrint(("MyScrollDC hDC=0x%x \n", hDC));
//
//	HANDLE PID = PsGetCurrentProcessId();
//	if (PID == g_ScrollHookTarget.hTargetProcess)
//	{
//	}
//	return uResult;
//}

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

ULONG MyNtGdiBitBlt(
					 HDC hDCDest,int  XDest,int  YDest,int  Width,int  Height,
					 HDC hDCSrc, int  XSrc, int  YSrc,	
					 ULONG  ROP, ULONG crBackColor,ULONG fl)
{
	NTSTATUS status = STATUS_SUCCESS;
	static LONG nCount = 0;
	ULONG nResult = 0;

    HWND hWndDestFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCDest, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);
    HWND hWndSrcFromDC = (HWND)g_pDriverHookApiInfos->pOrigNtUserCallOneParam((ULONG)hDCSrc, g_pDriverHookApiInfos->ONEPARAM_ROUTINE_WINDOWFROMDC);
	
	if (hWndDestFromDC == g_pDriverHookApiInfos->m_hWndDesktop
		|| hWndSrcFromDC == g_pDriverHookApiInfos->m_hWndDesktop)
	{
		//Skip 

		//KdPrint(("[%d], Bitblt for Desktop Wnd\n"));
		KdPrint(("[%d], !!!! In MyNtGdiBitBlt, hWndDesktop=0x%x, hWndDestFromDC=0x%x, hDCDest=0x%x, hWndSrcFromDC=0x%x, hDCSrc=0x%x, nResult=%d\n", 
			PsGetCurrentProcessId(),
			g_pDriverHookApiInfos->m_hWndDesktop, hWndDestFromDC, hDCDest, 
			hWndSrcFromDC, hDCSrc, nResult));
	}
	else
	{
		ULONG nResult = g_pDriverHookApiInfos->pOrigNtBitBlt(hDCDest, XDest, YDest, Width, Height, hDCSrc, XSrc, YSrc, ROP, crBackColor, fl);
	}
	if (g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW)
	{
		RECT rcClient = {0, 0, 400, 400};
		FNT_VERIFY(g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW(hDCDest, 0, 0, ETO_OPAQUE, &rcClient, L"CopyProtect", 11, NULL, 0x0000000B));
		//KdPrint(("OrigNtGdiExtTextOutW, return 0x%x\n", status));
	}

    if (g_pDriverHookApiInfos)
    {
        typedef NTSTATUS (*NTGDIGETDCDWORD)( HDC, INT n, PDWORD result );

    }

	//EngBitBlt(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0);
	
	InterlockedIncrement(&nCount);
	if (nCount % 20 == 0)
	{
	}
	return nResult;
}

BOOL MyNtGdiExtTextOutW(IN HDC 	hDC,
                         IN INT 	XStart,
                         IN INT 	YStart,
                         IN UINT 	fuOptions,
                         IN OPTIONAL LPRECT 	UnsafeRect,
                         IN LPWSTR 	UnsafeString,
                         IN INT 	Count,
                         IN OPTIONAL LPINT 	UnsafeDx,
                         IN DWORD 	dwCodePage)
{
    PAGED_CODE();

    KdPrint(("[%d], In MyNtGdiExtTextOutW hDC=0x%x, StartXY=(%d, %d), fuOptions=%d, String=%S, Count=%d, codePage=%d\n", 
        PsGetCurrentProcessId(), hDC, XStart, YStart, fuOptions, UnsafeString, Count, dwCodePage));
    //if (UnsafeRect)
    //{
    //    KdPrint(("  UnsafeRect = (%d, %d - %d, %d)\n", UnsafeRect->left, UnsafeRect->top, UnsafeRect->right, UnsafeRect->bottom));
    //}
    //if (UnsafeDx)
    //{
    //    KdPrint(("  *UnsafeDx=%d\n", *UnsafeDx));
    //}

    BOOL bRet = g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW(hDC, XStart, YStart,fuOptions, UnsafeRect, UnsafeString,
        Count, UnsafeDx, dwCodePage);

    return bRet;
}

//ULONG MyNtGdiStretchBlt(HDC hDCDest)
//{
//	ULONG nResult = OrigNtGdiStretchBlt(hDCDest);
//
//	return nResult;
//}


BOOLEAN Sleep(ULONG MillionSecond)
{
	NTSTATUS st;
	LARGE_INTEGER DelayTime;
	DelayTime = RtlConvertLongToLargeInteger(-10000 * MillionSecond);
	st=KeDelayExecutionThread( KernelMode, FALSE, &DelayTime );
	return (NT_SUCCESS(st));
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

void InstallCopyProtectHook(HANDLE hProcess, HWND hWndDesktop)
{
	//PAGED_CODE();

#if DBG
    //_asm int 3
#else 
    abc
#endif

    if (!g_pDriverHookApiInfos)
    {
        g_pDriverHookApiInfos = (PDRIVER_HOOK_API_INFOS)ExAllocatePoolWithTag(NonPagedPoolCacheAlignedMustS, sizeof(DRIVER_HOOK_API_INFOS), HOOK_API_INFO_TAG);
        NT_ASSERT(g_pDriverHookApiInfos);
        if (g_pDriverHookApiInfos)
        {
            g_pDriverHookApiInfos->InitValue();
			g_pDriverHookApiInfos->m_hWndDesktop = hWndDesktop;

            NTSTATUS status = STATUS_SUCCESS;
            ClearWriteProtect();

            //KdPrint(("IRQL : %#x", KeGetCurrentIrql()));

            g_pShadowTable = GetKeServiceDescriptorTableShadowAddress();
            if (g_pShadowTable == NULL)
            {
                KdPrint(("Failed GetShadowTable"));
            }
            else
            {
                PEPROCESS pEProcess;

                ULONG hCsrPid = (ULONG)GetCsrPid();

                KdPrint(("ServiceDescriptorShadowTableAddress : %#x, SSDT EntryCount=%d, Shadow EntryCount=%d, PID=%d, hCsrPid=%d\n", 
                    g_pShadowTable, g_pShadowTable[0].NumberOfServices, g_pShadowTable[1].NumberOfServices, hProcess, hCsrPid));

                PsLookupProcessByProcessId(hProcess, &pEProcess);
                if (pEProcess == NULL)
                {
                    SetWriteProtect();

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

                    g_pDriverHookApiInfos->pOrigNtBitBlt = (NTBITBLT)(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtGdiBitBlt]);// g_NtGdiBitBltIndex]);
                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtGdiBitBlt]), MyNtGdiBitBlt);

                    g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW = (NTGDIEXTTEXTOUTW)(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtGdiExtTextOutW]);
                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtGdiExtTextOutW]), MyNtGdiExtTextOutW);

                    g_pDriverHookApiInfos->pOrigNtUserCallOneParam = (NTUSERCALLONEPARAM)(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtUserCallOneParam]);
                }
                KeUnstackDetachProcess(&KApcState);
                //KeDetachProcess();
            }
            SetWriteProtect();
        }
    }


}

void UnInstallScrollHook(void)
{
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
                    PEPROCESS pEProcess;

                    PsLookupProcessByProcessId(g_hProcess, &pEProcess);

                    KAPC_STATE KApcState = {0};
                    KeStackAttachProcess(pEProcess, &KApcState);

                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtGdiBitBlt]), g_pDriverHookApiInfos->pOrigNtBitBlt);
                    InterlockedExchangePointer(&(g_pShadowTable[1].ServiceTable[g_pDriverHookApiInfos->IndexOfNtGdiExtTextOutW]), g_pDriverHookApiInfos->pOrigNtGdiExtTextOutW);
                    
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
	DbgPrint("OpenCapture ScrollHook Driver UnLoaded\n");
}