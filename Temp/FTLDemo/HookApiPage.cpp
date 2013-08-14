// HookApiPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "HookApiPage.h"
#include "ftlHookAPI.h"

#ifndef FNT_VERIFY(x)
#  define FNT_VERIFY(x)	\
	status = (x);\
	if(!NT_SUCCESS(status)) \
	{\
		FTLTRACEEX(FTL::tlError, TEXT("%s :\t Error!!! Reason = 0x%x,Code:\"%s\" \n"),\
			__FILE__LINE__, status, TEXT(#x));\
	}
#endif 

#ifdef HOOK_API_DETOURS
#  include "detours.h"
#  pragma comment(lib, "detours.lib")
#endif 

#ifdef HOOK_API_EASYHOOK
#  include "easyhook.h"
#  if defined(_M_IX86)
#    pragma comment(lib, "EasyHook32.lib")
#  elif defined(_M_X64)
#    pragma comment(lib, "EasyHook64.lib")
#  endif
#endif //HOOK_API_EASYHOOK

#ifdef HOOK_API_MINHOOK
#  include "MinHook.h"
#  if defined _M_IX86
#    pragma comment(lib, "MinHook.x86.lib")
//#    pragma comment(lib, "libMinHook.x86.lib")
#  elif defined(_M_X64)
#    pragma comment(lib, "MinHook.x64.lib")
//#    pragma comment(lib, "libMinHook.x64.lib")
#  endif

#  define MH_VERIFY(x) \
	status = (x);\
	if(MH_OK != status) \
	{\
		FTLTRACEEX(FTL::tlError, TEXT("%s :\t Error!!! Reason = 0x%x,Code:\"%s\" \n"),\
		   __FILE__LINE__, status, TEXT(#x));\
	}
#   
#endif //HOOK_API_MINHOOK

IMPLEMENT_DYNAMIC(CHookApiPage, CPropertyPage)
static CString s_strHookSource = TEXT("");

static MESSAGEBOXW	s_TrueMessageBoxW = &::MessageBoxW;
MESSAGEBOXW			g_pOrigMessageBoxW = NULL;

CHookApiPage::CHookApiPage()
	: CPropertyPage(CHookApiPage::IDD)
{
	//m_pOrigMessageBoxWProc = NULL;
	m_bDetourIsHooked = FALSE;
	m_bEasyHookIsHooked = FALSE;
	m_bMinHookIsHooked = FALSE;

#ifdef HOOK_API_EASYHOOK
	m_pHookMessageBoxW = NULL;
	ZeroMemory(m_HookCreateFileW_ACLEntries, sizeof(m_HookCreateFileW_ACLEntries));
#endif 
}

CHookApiPage::~CHookApiPage()
{
}

void CHookApiPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CHookApiPage::OnInitDialog()
{
	BOOL bRet = FALSE;

	//HMODULE hUser32 = NULL;
	//API_VERIFY((hUser32 = LoadLibrary(TEXT("User32.dll"))) != NULL);
	//if (bRet)
	//{
	//	//API_VERIFY((m_pOrigMessageBoxW = (MESSAGEBOXW)GetProcAddress(hUser32, "MessageBoxW")) != NULL);
	//	//FTLASSERT(m_pOrigMessageBoxW == s_TrueMessageBoxW);
	//	FreeLibrary(hUser32);
	//}

#if defined(HOOK_API_DETOURS)
	GetDlgItem(IDC_BTN_HOOK_API_DETOURS_INSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_HOOK_API_DETOURS_UNINSTALL)->EnableWindow(TRUE);
#endif //HOOK_API_DETOURS

#if defined(HOOK_API_EASYHOOK)
	GetDlgItem(IDC_BTN_HOOK_API_EASYHOOK_INSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_HOOK_API_EASYHOOK_UNINSTALL)->EnableWindow(TRUE);

#endif //HOOK_API_EASYHOOK

#if defined(HOOK_API_MINHOOK)
	GetDlgItem(IDC_BTN_HOOK_API_MINHOOK_INSTALL)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_HOOK_API_MINHOOK_UNINSTALL)->EnableWindow(TRUE);
#endif //HOOK_API_MINHOOK
	return bRet;
}


BEGIN_MESSAGE_MAP(CHookApiPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_HOOK_API_CALL_MESSAGEBOXW, &CHookApiPage::OnBnClickedBtnCallMessageBoxW)

	ON_BN_CLICKED(IDC_BTN_HOOK_API_DETOURS_INSTALL, &CHookApiPage::OnBnClickedBtnHookApiDetoursInstall)
	ON_BN_CLICKED(IDC_BTN_HOOK_API_DETOURS_UNINSTALL, &CHookApiPage::OnBnClickedBtnHookApiDetoursUnInstall)

	ON_BN_CLICKED(IDC_BTN_HOOK_API_EASYHOOK_INSTALL, &CHookApiPage::OnBnClickedBtnHookApiEasyHookInstall)
	ON_BN_CLICKED(IDC_BTN_HOOK_API_EASYHOOK_UNINSTALL, &CHookApiPage::OnBnClickedBtnHookApiEasyHookUnInstall)

	ON_BN_CLICKED(IDC_BTN_HOOK_API_MINHOOK_INSTALL, &CHookApiPage::OnBnClickedBtnHookApiMinHookInstall)
	ON_BN_CLICKED(IDC_BTN_HOOK_API_MINHOOK_UNINSTALL, &CHookApiPage::OnBnClickedBtnHookApiMinHookUnInstall)

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_HOOK_API_TEST_JMP, &CHookApiPage::OnBnClickedBtnHookApiTestJmp)
END_MESSAGE_MAP()


// CHookApiPage message handlers

int WINAPI Hooked_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	FTLASSERT(g_pOrigMessageBoxW);

	int nRet = 0;
    CString strHookedText;
    strHookedText.Format(TEXT("Hooked \"%s\""), lpText);

	if (g_pOrigMessageBoxW)
	{
		nRet = g_pOrigMessageBoxW(hWnd, strHookedText, s_strHookSource, uType);
	}
    return nRet;
}

void CHookApiPage::OnBnClickedBtnCallMessageBoxW()
{
	::MessageBoxW(m_hWnd, TEXT("Message Box Text"), TEXT("Caption"), MB_OK);
}

void CHookApiPage::OnBnClickedBtnHookApiDetoursInstall()
{
#ifdef HOOK_API_DETOURS
    DWORD dwDetourSecionHeaderSize = sizeof(DETOUR_SECTION_HEADER);
    FTLASSERT(dwDetourSecionHeaderSize == 4 * 16);  //16个DWORD

    LONG nRet = 0;
    DETOURS_VERIFY(DetourTransactionBegin());
    DETOURS_VERIFY(DetourUpdateThread(GetCurrentThread()));
	DETOURS_VERIFY(DetourAttach(&(PVOID&)s_TrueMessageBoxW, &::MessageBoxW));
    DETOURS_VERIFY(DetourTransactionCommit());

	m_bDetourIsHooked = TRUE;
	s_strHookSource = TEXT("Detours");
	
    //CString strDetourSecionHeaderSize;
    //strDetourSecionHeaderSize.Format(TEXT("sizeof(DETOUR_SECTION_HEADER) is %d"), dwDetourSecionHeaderSize);
    //MessageBox(strDetourSecionHeaderSize, TEXT("HookApi Detours"), MB_OK);
#endif //HOOK_API_DETOURS
}

void CHookApiPage::OnBnClickedBtnHookApiDetoursUnInstall()
{
#ifdef HOOK_API_DETOURS
	if (m_bDetourIsHooked)
	{
		m_bDetourIsHooked = FALSE;

		s_strHookSource = TEXT("");
		LONG nRet = 0;
		DETOURS_VERIFY(DetourTransactionBegin());
		DETOURS_VERIFY(DetourUpdateThread(GetCurrentThread()));
		DETOURS_VERIFY(DetourDetach(&(PVOID&)s_TrueMessageBoxW, &::MessageBoxW));
		DETOURS_VERIFY(DetourTransactionCommit());
	}
#endif 
}

void CHookApiPage::OnBnClickedBtnHookApiEasyHookInstall()
{
	BOOL bRet = FALSE;
#ifdef HOOK_API_EASYHOOK
	NTSTATUS status = 0;
	FTLASSERT(m_pHookMessageBoxW == NULL);
	if (!m_pHookMessageBoxW)
	{
		m_pHookMessageBoxW = new HOOK_TRACE_INFO();
	}
	
	FNT_VERIFY(LhInstallHook(s_TrueMessageBoxW, Hooked_MessageBoxW, 
		/*(PVOID)0x12345678*/NULL,  
		(TRACED_HOOK_HANDLE)m_pHookMessageBoxW));
	if (SUCCEEDED(status))
	{
		//一定要调用这个函数，否则注入的钩子无法正常运行。  
		FNT_VERIFY(LhSetExclusiveACL(m_HookCreateFileW_ACLEntries, 1, 
			(TRACED_HOOK_HANDLE)m_pHookMessageBoxW));
	}
	m_bEasyHookIsHooked = TRUE;
#endif //HOOK_API_EASYHOOK
}

void CHookApiPage::OnBnClickedBtnHookApiEasyHookUnInstall()
{
#ifdef HOOK_API_EASYHOOK
	NTSTATUS status = 0;
	
	if (m_bEasyHookIsHooked)
	{
		m_bEasyHookIsHooked = FALSE;

		if (m_pHookMessageBoxW)
		{
			// this will also invalidate "hHook", because it is a traced handle...  
			FNT_VERIFY(LhUninstallAllHooks());

			// this will do nothing because the hook is already removed...  

			FNT_VERIFY(LhUninstallHook((TRACED_HOOK_HANDLE)m_pHookMessageBoxW));  
			delete m_pHookMessageBoxW;
			m_pHookMessageBoxW = NULL;

			// even if the hook is removed, we need to wait for memory release  
			FNT_VERIFY(LhWaitForPendingRemovals());  
		}
	}
#endif //HOOK_API_EASYHOOK
}

void CHookApiPage::OnBnClickedBtnHookApiMinHookInstall()
{
	MH_STATUS status = MH_OK;

	MH_VERIFY(MH_Initialize());
	MH_VERIFY(MH_CreateHook(&::MessageBoxW, &Hooked_MessageBoxW, reinterpret_cast<void**>(&g_pOrigMessageBoxW)));
	MH_VERIFY(MH_EnableHook(&::MessageBoxW));

	m_bMinHookIsHooked = TRUE;
	s_strHookSource = TEXT("MinHook");
}

void CHookApiPage::OnBnClickedBtnHookApiMinHookUnInstall()
{
	if (m_bMinHookIsHooked)
	{
		m_bMinHookIsHooked = FALSE;
		s_strHookSource = TEXT("");

		MH_STATUS status = MH_OK;
		MH_VERIFY(MH_DisableHook(&::MessageBoxW));
		MH_VERIFY(MH_Uninitialize());
	}
}

void CHookApiPage::OnDestroy()
{
	OnBnClickedBtnHookApiDetoursUnInstall();
	OnBnClickedBtnHookApiEasyHookUnInstall();
	OnBnClickedBtnHookApiMinHookUnInstall();

	CPropertyPage::OnDestroy();
}

void replaceCodeBuf(BYTE* code, DWORD_PTR len, DWORD old, void* ptr)
{
	MessageBox(0,L"1",0,0);
	for (DWORD_PTR i = 0; i < len - 4; ++i)
	{
		if (*(reinterpret_cast<DWORD*>(&code[i])) == old)
		{
			MessageBox(0,L"2222", 0,0);
			*(reinterpret_cast<DWORD_PTR*>(&code[i])) = reinterpret_cast<DWORD_PTR>(ptr);
			break;
		}
	}
	MessageBox(0,L"2",0,0);
}

void CHookApiPage::OnBnClickedBtnHookApiTestJmp()
{
	//0x90 -- nop
#if defined _M_IX86
	UCHAR			            Jumper[12] = {0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif defined _M_IA64
	UCHAR			            Jumper[12] = {0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0};
#elif defined _M_X64
	UCHAR			            Jumper[12] = {0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe0};
#else
    error Not Support OS
#endif

	//init to nop
	for(int i = 0; i < _countof(Jumper); i++)
	{
		if (Jumper[i] == 0x00)
		{
			Jumper[i] = 0x90;	//nop
		}
	}

	//Init Jump

	//Will not Execute if jump enabled
	::MessageBox(m_hWnd, TEXT("You Should NOT see me!"), TEXT("JMP NOT affect!"), MB_OK);

	::MessageBox(m_hWnd, TEXT("After JMP Message"), TEXT("Text"), MB_OK);
}
