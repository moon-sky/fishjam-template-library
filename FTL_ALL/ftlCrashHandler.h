#ifndef FTL_CRASH_HANDLER_H
#define FTL_CRASH_HANDLER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlcrashhandler requires ftlbase.h to be included first
#endif

#include <atlwin.h>
//#include <ftlWindow.h>
// 无资源对话框需要ATL

namespace FTL
{
	/*********************************************************************************************
	* CFileDialog dlg(TRUE); dlg.DoModal(); 在Win7下崩溃
	*   http://social.msdn.microsoft.com/Forums/en-US/2de2670e-03df-495b-9a84-1049eddff9b6/cfiledialog-on-vindows-7-exception
	*********************************************************************************************/

    /*********************************************************************************************
	* 符号服务器(Symbol Server)
    *   由 WinDbg.exe/devenv.exe + DbgEng.dll + DbgHelp.dll(OS的符号库模块，负责读取和解析调试符号) + SymSvr.Dll(从符号服务器查找、下载和管理符号文件) 等组成：
    *   语法为：symsrv*ServerDLL*[DownstreamStore*]ServerPath
    *     如果使用系统提供的SymSvr.Dll，可以简化成：srv*[DownstreamStore*]ServerPath
	*   环境变量：%_NT_SYMBOL_PATH%=SRV*E:\OSSymbols*http://msdl.microsoft.com/download/symbols
	*   问题：VS中可以在调试符号路径中指明 http://msdl.microsoft.com/download/symbols 不？
    *
	* CrashReport: 程序出现异常的时候显示发送错误的对话框, 并把Dump文件发送到指定的地址. http://code.google.com/p/crashrpt/
	* XCrashReport: 与上面的类似的一个开源项目. http://www.codeproject.com/KB/debug/XCrashReportPt1.aspx
	* procdump: 可以使得指定进程生成 dmp 文件(http://technet.microsoft.com/en-us/sysinternals/dd996900)
	*   procdump -ma <PID>  -- 使得指定进程生成完整的dump文件，可在程序崩溃弹出错误对话框时使用
	*
	* 读取Dump信息 -- http://support.microsoft.com/kb/315263
	* WINDBG分析DMP方法： http://bbs.icafe8.com/forum.php?mod=viewthread&tid=400104&fromuid=30123
    *
    * 二次开发：符号服务器API(Symbol Server API, 如 SymbolServer 等函数，具体参见 dbghelp.chm 文件),
	*
	* 常见问题：
	*   1.post mortem debugging -- 调试 dmp 文件时无法设置断点
	*   1.加载 .dmp 文件调试时，在 Output 中搜索 “.dmp': Loaded '*” 可以查看在客户端加载但在调试机器上没有的文件
	*     (典型情况是各种版本的 msvcrp* 或 mfc*)，然后需要下载对应文件，否则无法加载调试符号
	* 无法通过符号服务器下载 mfc90u.pdb
	*   1.有可能是不同的OS?
	*
	* 
	*********************************************************************************************/

	/*********************************************************************************************
	* 每个异常，Windows 会最多给于两轮处理机会 -- 
	*   异常处理器(VEH、SEH 等)
    * SetUnhandledExceptionFilter -- 在发生未处理异常时，由系统调用进行处理
    *   其异常处理函数的返回值：
    *     EXCEPTION_EXECUTE_HANDLER -- 
    *     EXCEPTION_CONTINUE_EXECUTION -- 代码会试图对发生异常的代码再执行一遍，通常需要先修改相关逻辑，否则会死循环
    *     EXCEPTION_CONTINUE_SEARCH -- 进行系统通常的异常处理（错误消息对话框） 
    * Vista系统下可以使用 RegisterApplicationRecoveryCallback 注册恢复回调函数，可以在
    * 未知错误或者超过Windows响应时间(默认5秒)后被系统调用，可以保存用户未保存的数据并恢复（有Windows提供的UI）
    * 在 TIB 中保存SEH的链表，CONTEXT里寄存器的值
    * 通过VirtualQuery查询EIP后，使用GetModuleName可获知异常模块名
    * StackWalk的回调可用dbghelp实现好的函数
    * SymFromAddr获取栈位置的函数名
    * SymGetLineFromAddr -- 获取源代码文件名和行
	*
	* RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);  -- 主动抛出异常
	* 
	* __try { ...} 
	* __except( 
	*           code=GetExceptionCode(), 
	*           MyMiniDump(hFile, GetExceptionInformation() ),
	*           EXCEPTION_EXECUTE_HANDLER 
	*          ) { 异常处理代码 } 
	*
	* 
	* DbgHelp中的DumpAPI例子: http://www.debuginfo.com/examples/src/effminidumps/MiniDump.cpp 
    *
    * TODO(未考虑调用约定，需要确认下面的内容是否正确)
    *   EBP-C -- 第一个局部变量
    *   EBP-8 -- 是Cookie屏障(一般是 0xCCCCCCC, 只有调试版才有?)
    *   EBP-4 -- 安全Cookie
    *   EBP -- 父函数的EBP 值
    *   EBP+4 -- 函数的返回地址
    *   EBP+8 -- 参数N(第一个还是最后一个?)
    **********************************************************************************************/
}

//extern CServerAppModule _Module;
//extern CComModule _Module;
//extern CAppModule _Module;  //CFResourcelessDlg需要使用



#include <vector>
//#include <atlbase.h>
//#include <atlwin.h>
//#include <WindowsX.h>

#include <ftlsystem.h>  //SuspendProcess


namespace FTL
{
    FTLEXPORT template<typename T>
    class CFResourcelessDlg: public ATL::CDialogImplBase  //无资源对话框，用于显示CrashHandler的基类
    {
    public:
        // Message map for IDOK & IDCANCEL
        BEGIN_MSG_MAP(CFResourcelessDlg)
            COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
            COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
        END_MSG_MAP()

        // Dailog creation functions
        FTLINLINE HWND Create(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL);
        FTLINLINE virtual BOOL	DestroyWindow();
        FTLINLINE INT_PTR DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL);
        // Accessor functions
        FTLINLINE bool IsModal() const;
        FTLINLINE UINT NumberOfControls()	const;
    protected:
        // Misc functions
        FTLINLINE void EndDialog(INT_PTR iResult);
        FTLINLINE virtual	LRESULT	OnCloseCmd(UINT, int iId, HWND, BOOL&);
        // Template create functions
        FTLINLINE void CreateDlgTemplate(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short sFontSize = 0, 
            ATL::_U_STRINGorID pszFontName = (UINT)0, 
            ATL::_U_STRINGorID pszMenu = (UINT)0, 
            ATL::_U_STRINGorID pszWndClass = (UINT)0);
        FTLINLINE void AddDlgItem(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short id, 
            ATL::_U_STRINGorID pszWndClass = (UINT)0, 
            short sCreateDataSize = 0, 
            void* pCreateData = NULL);
        FTLINLINE void AddButton(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddEditBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddStatic(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddListBox(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddScrollBar(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
        FTLINLINE void AddCombo(ATL::_U_STRINGorID pszTitle, DWORD dwStyle, DWORD dwExStyle, 
            short x, short y, short cx, short cy, short wId);
    private:
        FTLINLINE int WriteString(WORD* &dest, LPCTSTR pszString, bool bWriteResource = false);
        enum	COMMONCONTROLS{
            DLG_BUTTON		= 0x80,
            DLG_EDIT		= 0x81,
            DLG_STATIC		= 0x82,
            DLG_LIST		= 0x83,
            DLG_SCROLLBAR	= 0x84,
            DLG_COMBO		= 0x85
        };
        WORD	m_uiNumCtrls;	// Number of controls on in the template
        int		m_iSize;		// Total size of the template
        int		m_iOffset;		// Offset
        int*	m_pOffset;		// Pointer to the offset
        BYTE*	m_pTemplate;	// Pointer to the template data
        bool	m_bModal;
    };

    class CFCrashHandlerDialog;
    class CFCrashHandler;
}


namespace FTL
{
    class CFCrashHandlerDialog : public CFResourcelessDlg<CFCrashHandlerDialog>
    {
    public:
        BEGIN_MSG_MAP(CFCrashHandlerDialog)
            //DUMP_WINDOWS_MSG(__FILE__LINE__, DEAFULT_DUMP_FILTER_MESSAGES, _countof(DEAFULT_DUMP_FILTER_MESSAGES), uMsg, wParam, lParam)

            COMMAND_HANDLER(IDC_BTN_CREATE_MINIDUMP, BN_CLICKED, OnCreateMiniDumpClick)
			COMMAND_HANDLER(IDC_BTN_SAVE_STACK, BN_CLICKED, OnSaveStackClick)
            MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
            MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
			CHAIN_MSG_MAP(CFResourcelessDlg<CFCrashHandlerDialog>)
            //MESSAGE_HANDLER(WM_SIZE, OnSize)
        END_MSG_MAP()
        enum 
        {
            IDC_STATIC_ADDRESS  = 1000,
            IDC_STATIC_REASON,
			IDC_LIST_STACK,
            IDC_BTN_DEBUG,
            IDC_BTN_CREATE_MINIDUMP,
			IDC_BTN_SAVE_STACK,
        };
        FTLINLINE CFCrashHandlerDialog(PEXCEPTION_POINTERS pExcption);
        FTLINLINE virtual ~CFCrashHandlerDialog();
        FTLINLINE LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        FTLINLINE LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        FTLINLINE LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        //FTLINLINE LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        FTLINLINE LPTSTR GetFaultReason(DWORD ExceptionCode);
        FTLINLINE void CreateDlg();
		FTLINLINE LRESULT OnCreateMiniDumpClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
		FTLINLINE LRESULT OnSaveStackClick(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    protected:
		FTLINLINE BOOL _GetCrashFilePrefix(LPTSTR pszBuffer, DWORD dwSize);		
        PEXCEPTION_POINTERS m_pException;
        TCHAR m_FaultReason[MAX_BUFFER_LENGTH];
    };

    //enum MiniDumpType{
    //    mdtNormal         = 0x0000,
    //    mdtWithDataSegs   = 0x0001,
    //    mdtWithFullMemory = 0x0002,
    //    mdtWithHandleData = 0x0004,
    //};

    //注意：由于 SetUnhandledExceptionFilter 无法传入额外的参数，因此使用类方法不好控制
    //typedef long ( __stdcall *PFNCHFILTFN ) ( PEXCEPTION_POINTERS pExPtrs ) ;
	//TODO:需要使用 AdjustTokenPrivileges 来提升 SE_DEBUG_NAME 权限 来调用 MiniDumpWriteDump ?
	//  https://www.evernote.com/shard/s90/sh/81c75ef2-416b-4b72-8ed2-f997c12da163/2eb0ee2d8fa5dd2ad832351371216788
	//  
    class CFCrashHandler
    {
    public:
        FTLINLINE CFCrashHandler();
        FTLINLINE ~CFCrashHandler();
        FTLINLINE BOOL SetDefaultCrashHandlerFilter();
        FTLINLINE BOOL RestoreCrashHandlerFilter();
        //BOOL SetCrashHandlerFilter ( PFNCHFILTFN pFn ) ;
        FTLINLINE static BOOL SnapProcessMiniDump(HANDLE hProcess, MINIDUMP_TYPE type, LPCTSTR pszDumpFileName);
        FTLINLINE static BOOL CreateProcessCrashDump(HANDLE hProcess, MINIDUMP_TYPE type, LPCTSTR pszDumpFileName,
            DWORD dwThread, EXCEPTION_POINTERS * pExceptInfo);
    private:
        //PFNCHFILTFN m_pfnCallBack;
        static CFCrashHandler* s_pSingleCrashHandler;
		static CRITICAL_SECTION	*s_pCriticalSection;

        LPTOP_LEVEL_EXCEPTION_FILTER m_pfnOrigFilt;
        FTLINLINE static LONG __stdcall DefaultCrashHandlerFilter( PEXCEPTION_POINTERS pExPtrs);
    };
}

#endif //FTL_CRASH_HANDLER_H

#ifndef USE_EXPORT
#  include "ftlCrashHandler.hpp"
#endif
