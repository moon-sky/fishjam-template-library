// MemPage.cpp : implementation file
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "MemPage.h"

// CMemPage dialog

IMPLEMENT_DYNAMIC(CMemPage, CPropertyPage)

CMemPage::CMemPage()
	: CPropertyPage(CMemPage::IDD)
{

}

CMemPage::~CMemPage()
{
}

void CMemPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMemPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_MEM_CHECK_MEM_LEAK, &CMemPage::OnBnClickedBtnMemCheckMemLeak)
    ON_BN_CLICKED(IDC_BTN_CHECK_MULTI_THREAD_MEM_LEAK, &CMemPage::OnBnClickedBtnCheckMultiThreadMemLeak)
    ON_BN_CLICKED(IDC_BTN_CHECK_ERROR_FREE_MEMORY, &CMemPage::OnBnClickedBtnCheckErrorFreeMemory)
END_MESSAGE_MAP()



//通过CRT函数能成功检查出内存泄露的
//注意所检查内存的生存期范围，提示有内存泄露，只是说明这个范围内有内存分配，而没有释放。不代表该内存真的没有释放。
void CMemPage::SucceedCheckAllocAndFree()
{
    CHECK_MEM_LEAK(FALSE);

    BYTE * pNew = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check New"),FALSE);
         pNew = new BYTE;
    }
    SAFE_DELETE(pNew);

    BYTE * pNewArray = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check New Array"),FALSE);
        pNewArray = new BYTE[3];
    }
    SAFE_DELETE_ARRAY(pNewArray);

    BYTE * pMalloc = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check Malloc"),FALSE);
        pMalloc = (BYTE*)malloc(5);
    }
    SAFE_FREE(pMalloc);

    CButton* pNewButton = NULL;
    {
        //将在 "Client Blocks" 中输出
        CHECK_NAME_MEM_LEAK(TEXT("Check New MFC Object"),FALSE);
        pNewButton = new CButton();
        //pNewButton->Create(TEXT("CreateButton"),WS_CHILD|WS_VISIBLE, CRect(0,0,100,100), this, 100);
    }
    SAFE_DELETE(pNewButton);
}

//通过CRT函数不能检查出内存泄露的
//注意：以下的内存分配方式，用CRT内存函数无法检测出
void CMemPage::FaildCheckAllocAndFree()
{
    CHECK_MEM_LEAK(FALSE);
    BOOL bRet = FALSE;

    BYTE * pCoTaskMemAlloc = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check CoTaskMemAlloc"),FALSE);
        pCoTaskMemAlloc = (BYTE*)::CoTaskMemAlloc(7);
    }
    SAFE_COTASKMEMFREE(pCoTaskMemAlloc);

    BYTE * pHeapAlloc = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check HeapAlloc"),FALSE);
        pHeapAlloc = (BYTE*)HeapAlloc(GetProcessHeap(), 0, 9);
    }
    SAFE_HEAP_FREE(pHeapAlloc);

    BYTE * pLocalAlloc = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check LocalAlloc"),FALSE);
        pLocalAlloc = (BYTE*)LocalAlloc(0,11);
    }
    SAFE_LOCAL_FREE(pLocalAlloc);

    BSTR bstr = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check SysAllocString"),FALSE);
        bstr = ::SysAllocString(L"Test SysAllocString");
    }
    SAFE_FREE_BSTR(bstr);

    HANDLE hTestHandle = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check CloseHandle"),FALSE);
        hTestHandle = CreateEvent(NULL,TRUE,FALSE,NULL);
    }
    SAFE_CLOSE_HANDLE(hTestHandle, NULL);
}


void CMemPage::OnBnClickedBtnMemCheckMemLeak()
{
    SucceedCheckAllocAndFree();
    FaildCheckAllocAndFree();
}


DWORD  __stdcall AllockAndFreeMemoryThread(LPVOID lpThreadParameter)
{
    DWORD dwSleepTime = DWORD(lpThreadParameter);
    BYTE* pByte = new BYTE[20];
    Sleep(dwSleepTime);
    SAFE_DELETE_ARRAY(pByte);
    return 0;
}

//由于内存快照是整个进程的，因此多线程中可能会把别的线程分配了但尚未释放的内存检测为泄露
//加入 Pause 其他线程的功能来解决这个问题
//例子中根据 子线程是否在检测范围内结束 和 是否暂停其他线程，有 2 x 2 的检测结果(是否检测出)：
//            子线程检测范围内结束   是   否
//  是否暂停
//     是                            否   否         
//     否                            否   是 


void CMemPage::OnBnClickedBtnCheckMultiThreadMemLeak()
{
    //子线程的睡眠时间如果是100ms，则先结束；如果是1000毫秒，则父线程先退出检查范围
    HANDLE hThread = (HANDLE)::CreateThread(NULL,0,AllockAndFreeMemoryThread, LPVOID(1000), CREATE_SUSPENDED, NULL);
    {
        CHECK_MEM_LEAK(FALSE);
        ResumeThread(hThread);
        Sleep(500);
    }
    CloseHandle(hThread);
}

//检查错误的内存释放方式 -- 目前都不支持
void CMemPage::CheckErrorMemoryFree()
{
    CHECK_MEM_LEAK(TRUE);

    BYTE* pNewArrayButDelete = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check new array but delete"), FALSE);
        pNewArrayButDelete = new BYTE[10];
        delete pNewArrayButDelete;
        pNewArrayButDelete = NULL;
    }

    //BYTE* pNewButDeleteArray = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check new but delete array"), FALSE);
        pNewArrayButDelete = new BYTE;
        delete [] pNewArrayButDelete;
        pNewArrayButDelete = NULL;
    }

    BYTE* pNewButFree = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check new but free"), FALSE);
        pNewButFree = new BYTE;
        free(pNewButFree);
        pNewButFree = NULL;
    }

    BYTE* pMallocButDelete = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check malloc but delete"), FALSE);
        pMallocButDelete = (BYTE*)malloc(10);
        delete pMallocButDelete;
        pMallocButDelete = NULL;
    }

    BYTE* pMallocButDeleteArray = NULL;
    {
        CHECK_NAME_MEM_LEAK(TEXT("Check malloc but deleteArray"), FALSE);
        pMallocButDeleteArray = (BYTE*)malloc(10);
        delete [] pMallocButDeleteArray;
        pMallocButDeleteArray = NULL;
    }

}

void CMemPage::OnBnClickedBtnCheckErrorFreeMemory()
{
    CheckErrorMemoryFree();
}
