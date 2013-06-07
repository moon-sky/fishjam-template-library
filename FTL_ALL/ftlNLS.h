#ifndef FTL_NLS_H
#define FTL_NLS_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlNLS.h requires ftlbase.h to be included first
#endif

/****************************************************************************************************************
* TODO: eMule中的DataIO 加载了 WideCharLowerMap.bin 文件用于对宽字节的大小写映射(没有使用系统提供的)
*    -- 是因为 LCMapStringW 不能在Win9X 下工作还是为了保证网络中所有的客户端，不论版本、OS、语言都能使用相同的转换？
*       参见 s_awcLowerMap 静态变量
*       但因为有大小的判断(if (SizeofResource(hInst, hResInfo) == sizeof s_awcLowerMap))，需要调试看是否真正的使用了
*
* 
* cmd命令行中
*   chcp [nnn] -- 显示或设置活动代码页编号，如中文是(936)
* 
* CodePage -- 国际化中 UNICODE 和 ANSI 的本地语言转换时指定的转换码?
*   GetACP() -- 获取当前ANSI的 code page 的具体值，使用上 等价于 CP_ACP(常量 0)
* 
* http://blog.csdn.net/w8u/article/details/5773449  -- 不同codepage文件转换操作实录
* NLS -- National Language Support，国际化支持
* MUI --
* 
* 可以利用 GetLocaleInfo 获取很多用户相关的多语言信息
*   如 GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SABBREVLANGNAME ...) 可以获取当前用户语言的缩写(CHS)
*   http://www.flounder.com/localeexplorer.htm 网站上有 localeexplorer.zip 源码包，详细列出了多种国际化相关的代码
*
* CheckList
*   1.使用 UNICODE/_UNICODE
*   2.源代码中无 fixed text
*   3.源代码中无 graphic element(带文字的图片?)
*   4.源代码中无UI元素固定位置和大小
*   5.统一使用 _T 和 TCHAR
*   6.将资源和XML Encode 为UTF-8
*   7.Unicode文件需要 BOM(Byte Order Marker)
*   8.处理XML时将 UTF-8 转换为 Unicode
*   9.显示 date,time,currency,number,calendar 等时需要考虑本地化设置
*     GetLocalInfo, GetUserDefaultLangID,
*     GetNumberFormat, GetDateFormat, GetCurrencyFormat, SetThreadLocale
*   10.不要在运行时修改本地化 string(?)
*   11.给字符串显示留下足够的空间
*   12.do not put UI controls on string (?)
*   13.do not allow UI controls to overlay each other.
*   14.最好不要用CRT中的 _tcsxxx 等函数，而应用Win32 API的 lstrncpy 等，这样可使用 system locale of windows)
*   15.字符串比较或转换用 ?
*      CompareString(LOCALE_USER_DEFAULT,
*      LCID locale = MAKELCID(MAKELANGID(), SORT_DEFAULT); 然后用 LCMapString(locale, LCMAP_LOWERCASE, pwsz, -1, pwsz, iLen + 1);
*   16.多参数字符串格式化： CString::FormatMessage("%1,%2,%3")
*   17.在一个资源(如res[codepage].dll)中，可以保存多个语言的资源，通过 SetThreadLocale 切换，
*      然后通过 LoadString 等函数获取不同语言的值
*
*
* 统一定义版本等信息
*   生成 version.h 文件，然后在 .rc 文件中包含，并设置 CompanyName 等的值为定义的相关宏
*     QQUOTE(__MAJOR_VERSION)"."QQUOTE(__MINOR_VERSION)"."QQUOTE("__BUILD_TYPE")"."QQUOTE(BUILD_NUMBER)
*   问题：似乎编辑过以后会自动变成原样？
*   
*
* 输入法编程
*   原理：监控用户的按键，来预料用户可能需要的字符，并提供一张备择字符列表以供用户从中选出所需字符
*   定义
*     IME(Input Method Editor) -- 使用标准键盘输入复杂字母与符号（如中日文等）
*     IMM(Input Method Manager) -- 输入法管理器，通过 GetSystemMetrics(SM_IMMENABLED) 判断当前IMM是否激活
*   应用程序类型：
*     1.IME-unaware(默认处理) -- 系统(DefWindowProc)自动处理IME相关的消息、内容
*     2.IME-aware -- 应用程序通过传递、截取和处理送往默认窗体的消息，来控制窗体的操作、位置以及外观。
*   输入上下文(ImmGetContext/ImmReleaseContext) -- 
*     默认时，系统为每个线程创建和指派一个输入上下文，被连接到该线程内的每个新建窗体,但可创建和关联窗体特定的上下文。
*     输入上下文与窗体连接后(ITextHost::TxImmGetContext)，系统自动选择何时窗体被激活和被输入焦点的上下文，
*     1.ImmCreateContext/ImmDestroyContext -- 创建/销毁 输入上下文
*     2.ImmAssociateContext -- 关联或取消关联指定窗体和输入上下文
*   相关窗体：
*     1.状态窗体(StatusWindow) -- 指示IME被打开，并向用户提供设置转换模式的方法
*     2.录入窗体(CompositionWindow) -- 用户输入文本时，依赖转换模式显示输入文本或转换后的文本
*       录入字符串--录入窗体的当前文本(即IME转换到最终字符的文本)，Imm Get/Set CompositionString -- 获取和设置录入字符串
*       录入字符串由一或多个Clauses(IME能转换到最终字符的字符最小组合)组成
*     3.备选窗体(CandidateWindow) -- 用户输入文本时，包含待选的备择字符或输入窗体中的字符，用户可以滚动备择列表来选择所需的字符
*       备选列表(CANDIDATELIST) -- 由指定字符串的数组成或用户可以从中选择的字符串组成
*         ImmGetCandidateListCount、ImmGetCandidateList
*   相关消息 -- 如果应用程序不处理相关消息，DefWindowProc 会进行默认处理（如 WM_IME_CHAR 转义为 一个或多个WM_CHAR） 
*   
*  TF_FloatingLangBar_WndTitle(CiceroUIWndFrame)
****************************************************************************************************************/

//////////////////////////////////////////////////////////////////////////
#include <map>

namespace FTL
{
    //! 使用这种方式可以支持动态更改语言，问题是需要客户端做很多工作 -- 直接使用 Win32 API
    //  通常在English的DLL中，直接使用Main的资源(Main资源为英文)，这样可以保证当Satellite不存在时，可以使用英文显示
    class CFSatelliteDllManager
    {
    public:
        CFSatelliteDllManager();
        ~CFSatelliteDllManager();
        BOOL AddSatelliteDll(LANGID language, LPCTSTR pszDllPath);
        BOOL RemoveSatelliteDll(LANGID language);
        BOOL SetDefaultModule(HMODULE hDefaultModule);
        BOOL ChangeLanguage(LANGID DesiredLanguage); //实现时需要注意旧的Module的释放
    public:
        //各种会涉及到 Satellite Dll 的 WinUser 函数包装 -- 不区分 A/W，直接使用 T
        int LoadString(__in UINT uID, LPTSTR lpBuffer,int cchBufferMax);
        HACCEL LoadAccelerators(LPCTSTR lpTableName);
        HICON LoadIcon(LPCTSTR lpIconName);
        INT_PTR DialogBoxIndirectParam(LPCDLGTEMPLATE hDialogTemplate,HWND hWndParent,DLGPROC lpDialogFunc,LPARAM dwInitParam);
        HMENU LoadMenu(LPCTSTR lpMenuName);
        HCURSOR LoadCursor(LPCTSTR lpCursorName); //注意：如果是系统的Cursor(如 IDC_ARROW)，则不能使用该Satellite函数
    private:
        typedef std::map<LANGID,LPCTSTR> SATELLITE_MAP;
        SATELLITE_MAP   m_AllSatellites;
        HMODULE         m_DefaultModule;    //用于设置的Satellite Dll找不到的情况
        HMODULE         m_CurrentSatelliteDll;
        LANGID          m_CurrentLanguage;
    };

    class CFNLS
    {
    public:
        //detects a correct initial UI language for all platforms (Win9x, ME, NT4, Windows 2000, Windows XP)
        //英语为1033，德语为1031，日语为1041 -- GetLocaleInfo(AvailableLangID,LOCALE_SNATIVELANGNAME,AvailableLangName,MAX_LANGNAME)
        FTLINLINE LANGID DetectUILanguage();
    protected:
        // The following functions contain code to detect the language in which the initial user interface should be displayed
        FTLINLINE static BOOL CALLBACK EnumLangProc(HANDLE, LPCTSTR, LPCTSTR,WORD wIDLanguage, LONG_PTR lParam);
        // Detects the language of ntdll.dll with some specific processing for the Hongkong SAR version
        FTLINLINE LANGID GetNTDLLNativeLangID();
        // Checks if NT4 system is Hongkong SAR version
        FTLINLINE BOOL IsHongKongVersion();
    };

	class CFIMEUtil
	{
	public:

		//获取 WM_IME_NOTIFY 消息对应的字符串信息
		FTLINLINE static LPCTSTR GetIMENotifyInfoString(CFStringFormater& formater, WPARAM wParam, LPARAM lParam);

		//IME Composition String Values
		FTLINLINE static LPCTSTR GetCompositionValueString(CFStringFormater& formater, UINT nGCSValue, LPCTSTR pszDivide = TEXT("|"));
	};
}

#endif //FTL_NLS_H

#ifndef USE_EXPORT
# include "ftlNLS.hpp"
#endif //USE_EXPORT