#pragma once

/*******************************************************************************************
* _U_STRINGorID是一个WTL的工具类，它可以自动转换LPCTSTR和资源ID
* 
* #define _WTL_NEW_PAGE_NOTIFY_HANDLERS -- 使用WTL 7的消息处理方式(更好)
* 
* WTL属性页和向导
*   由 CWindow 子类(窗口接口类) + CWindowImplBaseT 子类(消息映射链和窗口的完整实现) 共同实现
*
* 属性表Sheet:
*   1.CPropertySheetWindow -- 封装对PSM_消息的处理
*     PROPSHEETHEADER m_psh;
*     ATL::CSimpleArray<HPROPSHEETPAGE> m_arrPages;
*   2.CPropertySheetImpl -- 提供方法,如 AddPage/RemovePage/SetActivePage/SetWizardMode(向导模式) 等
*     常见的可重载的(编译时虚)函数
*        OnSheetInitialized() -- 初始化
*        PropSheetCallback() -- PropSheetProc 函数的静态回调函数(注意：未实测)：
*          static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);
* 属性页Page
*   1.CPropertyPageWindow 
*   2.CPropertyPageImpl/CAxPropertyPageImpl(支持ActiveX控件) -- 从CDialogImplBaseT继承，
*     需要指定对话框资源: Style(Child)、Border(Thin)、Title Bar(True)、Disabled(True),
*     通过OnNotify消息映射处理通知码为PSN_的WM_NOTIFY消息。
*     PROPSHEETPAGE m_psp;
*     常见的可重载的(编译时虚)函数(具体参见 WTL.CPropertyPageImpl.OnNotify )
*       OnSetActive() -- 允许页面成为激活状态，向导模式时需要调用SetWizardButtons()设置相应的按钮
*       OnKillActive() - 允许页面成为非激活状态，返回TRUE表示禁止失去焦点
*       OnQueryCancel() -- 
*       OnApply() -- 返回 PSNRET_NOERROR 表正确，否则可返回 PSNRET_INVALID 等
*       OnWizardBack/OnWizardNext/OnWizardFinish -- 返回前一个/进行到下一个/允许向导结束。返回TRUE禁止
*
* WTL已经提供的基础类：
*   CPropertySheet/CPropertyPage/CAxPropertyPage -- 不需要用户定制的简单特例
*   示例代码：
*     CPropertySheet sheet(_T("About PSheets"));
*     CPropertyPage<IDD_ABOUTBOX> pgAbout;
*     sheet.AddPage(pgAbout);
*     sheet.DoModal();
*******************************************************************************************/

#include "MakerWelcomePage.h"
#include "MakerChooseSetupFilePage.h"
#include "MakerSetupProgressPage.h"
#include "MakerCompletePage.h"

class CMakerWizardSheet :
    public CWizard97SheetImpl<CMakerWizardSheet>,
    public CMakerWizardInfoRef
{
protected:
    // Typedefs
    typedef CMakerWizardSheet thisClass;
    typedef CWizard97SheetImpl<CMakerWizardSheet> baseClass;
    typedef CMakerWizardInfoRef infoRefClass;

    // Data members
    CMakerWelcomePage           m_pageWelcome;
    CMakerChooseSetupFilePage   m_pageChooseSetupFile;
    CMakerSetupProgressPage     m_pageSetupProgress;
    CMakerCompletePage          m_pageCompletion;

public:
    // Constructors
    CMakerWizardSheet(CMakerWizardInfo* pMakerWizardInfo, UINT uStartPage = 0, HWND hWndParent = NULL);

    // Message Handlers
    BEGIN_MSG_MAP(thisClass)
        MESSAGE_HANDLER(WM_HELP, OnHelp)
        CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

    LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
