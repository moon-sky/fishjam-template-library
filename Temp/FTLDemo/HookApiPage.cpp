// HookApiPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "HookApiPage.h"

#define HOOK_API_DETOURS

#ifdef HOOK_API_DETOURS
#  include "detours.h"
#  pragma comment(lib, "detours.lib")
#endif 

// CHookApiPage dialog

IMPLEMENT_DYNAMIC(CHookApiPage, CPropertyPage)

CHookApiPage::CHookApiPage()
	: CPropertyPage(CHookApiPage::IDD)
{

}

CHookApiPage::~CHookApiPage()
{
}

void CHookApiPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHookApiPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_HOOK_API_DETOURS, &CHookApiPage::OnBnClickedBtnHookApiDetours)
END_MESSAGE_MAP()


// CHookApiPage message handlers

static int (WINAPI* TrueMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = ::MessageBoxW;

int Hooked_MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    CString strHookedText;
    strHookedText.Format(TEXT("Hooked %s"), lpText);
    int nRet = TrueMessageBoxW(hWnd, strHookedText, lpCaption, uType);
    return nRet;
}

void CHookApiPage::OnBnClickedBtnHookApiDetours()
{
    BOOL bRet = FALSE;

    DWORD dwDetourSecionHeaderSize = sizeof(DETOUR_SECTION_HEADER);
    FTLASSERT(dwDetourSecionHeaderSize == 4 * 16);  //16¸öDWORD

    LONG nRet = 0;
    API_VERIFY(DetourTransactionBegin() == NO_ERROR);
    API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
    API_VERIFY(DetourAttach(&(PVOID&)TrueMessageBoxW, ::MessageBox) == NO_ERROR);
    API_VERIFY(DetourTransactionCommit() == NO_ERROR);


    ::MessageBoxW(m_hWnd, TEXT("Message Box Text"), TEXT("Caption"), MB_OK);
    

    API_VERIFY(DetourTransactionBegin() == NO_ERROR);
    API_VERIFY(DetourUpdateThread(GetCurrentThread()) == NO_ERROR);
    API_VERIFY(DetourDetach(&(PVOID&)TrueMessageBoxW, ::MessageBox) == NO_ERROR);
    API_VERIFY(DetourTransactionCommit() == NO_ERROR);

    //CString strDetourSecionHeaderSize;
    //strDetourSecionHeaderSize.Format(TEXT("sizeof(DETOUR_SECTION_HEADER) is %d"), dwDetourSecionHeaderSize);
    //MessageBox(strDetourSecionHeaderSize, TEXT("HookApi Detours"), MB_OK);
}
