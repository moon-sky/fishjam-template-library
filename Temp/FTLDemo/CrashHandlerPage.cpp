// CrackHandlerPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "CrashHandlerPage.h"
#include <ftlcrashhandler.h>

// CCrashHandlerPage 对话框

IMPLEMENT_DYNAMIC(CCrashHandlerPage, CPropertyPage)

CCrashHandlerPage::CCrashHandlerPage()
	: CPropertyPage(CCrashHandlerPage::IDD)
{

}

CCrashHandlerPage::~CCrashHandlerPage()
{
}

void CCrashHandlerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCrashHandlerPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BTN_CRASH, &CCrashHandlerPage::OnBnClickedBtnCrash)
    ON_BN_CLICKED(IDC_BTN_DIS_RESOURCELESS_DLG, &CCrashHandlerPage::OnBnClickedBtnDisResourcelessDlg)
END_MESSAGE_MAP()


// CCrashHandlerPage 消息处理程序

#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    __asm    call x \
    __asm x: pop eax \
    __asm    mov c.Eip, eax \
    __asm    mov c.Ebp, ebp \
    __asm    mov c.Esp, esp \
    } while(0);

#define USED_CONTEXT_FLAGS CONTEXT_FULL

void CCrashHandlerPage::OnBnClickedBtnCrash()
{
    LPTSTR pBuf = NULL;
    *pBuf = 100;

    //CFCrashHandlerDialog dlg(NULL);
    //dlg.DoModal();

    //CONTEXT c = {0};
    //GET_CURRENT_CONTEXT(c, USED_CONTEXT_FLAGS);
    //CFStackWalker stackWalker;
    //stackWalker.GetCallStackArray(GetCurrentThread(),&c);
    //stackWalker.GetModuleListPSAPI(GetCurrentProcess());
    //stackWalker.GetModuleListTH32(GetCurrentProcess(),GetCurrentProcessId());
}

class CRsDlgDemo : public CFResourcelessDlg<CRsDlgDemo>
{
public:
    BEGIN_MSG_MAP(CRsDlgDemo)
        CHAIN_MSG_MAP(CFResourcelessDlg<CRsDlgDemo>)
    END_MSG_MAP()
    void CreateDlg()
    {
        // Add dailog creation code here
        // Start generated dialog
        CreateDlgTemplate(TEXT("Demo"), DS_SETFONT | DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU,
            0, 0, 0, 166, 137, 8, TEXT("Comic Sans MS"), TEXT(""), TEXT(""));
        AddButton(TEXT("OK"), 0|BS_DEFPUSHBUTTON, 0, 7, 116, 50, 14, IDOK);
        AddButton(TEXT("Cancel"), 0, 0, 109, 116, 50, 14, IDCANCEL);
        AddStatic(TEXT("Congratulation!"), 0, 0, 52, 7, 51, 8, IDC_STATIC);
        AddStatic(TEXT("You have finished the tutorial, I hope you liked it.")\
            TEXT("The following controls do nothing because you did not add any code for them, ")\
            TEXT("but now you know how a resource dialog works you could add code for them yourself."),
            0, 0, 14, 25, 137, 41,  IDC_STATIC);
        AddButton(TEXT(""), 0 | BS_GROUPBOX, 0, 7, 19, 151, 50, IDC_STATIC);
        AddEditBox(TEXT(""), ES_AUTOHSCROLL | WS_BORDER, 0, 30, 83, 128, 12, 1000);
        AddStatic(TEXT("Name:"), 0, 0, 7, 85, 22, 8, IDC_STATIC);
        AddScrollBar(TEXT(""), 0, 0, 7, 103, 151, 9, 1001);
        // End generated dialog
    }
};

void CCrashHandlerPage::OnBnClickedBtnDisResourcelessDlg()
{
    CRsDlgDemo dlg;
    dlg.DoModal();
}
