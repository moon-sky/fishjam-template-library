
#ifndef FTL_SYSTEM_H
#define FTL_SYSTEM_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlsystem.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"

#include <Shlwapi.h>    //IShellFolder
#include <float.h>      //_control87
#include <TlHelp32.h>   //CreateToolhelp32Snapshot
#include <list>
#include <functional>

/*****************************************************************************************************
* 操作系统
*   4个特性：并发性、共享性、虚拟性 和 不确定性。
*   5个管理功能：进程管理、文件管理、存储管理、设备管理 和 作业管理。
* 
* S1睡眠模式 -- 中央处理器暂停、内存已刷新、整个系统处于低电力模式
* S3/S4睡眠模式 -- 未供电至中央处理器、内存延缓升级、电源处于低电力模式
* 
* 操作系统提供的IPC机制一般分为两类：
*   1.本地IPC -- 共享内存、管道、Unix领域socket、door(?)、信号等；
*   2.远程IPC -- Internet领域socket、Win32命名管道(named pipe)等
*
* 通过性能计数监视程序性能和泄漏：
*   控制面板 -> 管理工具 -> 性能 -> 性能日志和警报 -> 计数器日志 -> 右键 -> 新建日志设置 -> 起名
*   “常规”中“添加计数器” -> "性能对象" 选择 "Process",通常来说为了检查内存、资源泄露，需要增加以下几种：
*     Handle Count -- 句柄数
*     Working Set -- 内存使用
*     Virtual Bytes -- 虚拟内存大小
*   如果没有内存泄露，应该是在特定区间内波动，如果呈现线性增长趋势，通常说明有泄漏。
*
* 替换任务管理器：
*   HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\taskmgr.exe
*   设置 Debugger 项的值，如 "H:\TOOLS\SYSTEM\PROCESSEXPLORER\PROCEXP.EXE"
*****************************************************************************************************/
namespace FTL
{
//Class   
    template <typename T> class CFSingletonT;

    class CFRegUtil;
    class CFSystemUtil;
}

namespace FTL
{
    //单实例模式的模版
    template <typename T>
    class CFSingletonT
    {
    private:
        class SelfPtr
        {
        private:
            T*    m_pInstance;
        public:
            SelfPtr() : m_pInstance(NULL) {}
            ~SelfPtr() { delete m_pInstance; m_pInstance = NULL; }
            T* GetPtr() { return m_pInstance; }
            void SetPtr(T* ptr) { delete m_pInstance; m_pInstance = ptr; };
        };
        static SelfPtr s_pSelf;

    protected:
        CFSingletonT(){}

    public:
        virtual ~CFSingletonT(){}

        static T* Instance() 
        {
            if( !s_pSelf.GetPtr() )
                s_pSelf.SetPtr(new T());
            return s_pSelf.GetPtr();
        }
        static void Release()
        {
            s_pSelf.SetPtr(NULL);
        }
    }; 

    template <typename T>
    typename CFSingletonT<T>::SelfPtr CFSingletonT<T>::s_pSelf = CFSingletonT<T>::SelfPtr();

    class CFOSInfo
    {
    public:
        enum OSType
        {
            ostLowUnknown,
            //ostWin32,
            ostWin95,
            ostWin98,	//MSDN
            ostWin98_SE,
            ostWinMe,	//MSDN
            ostWinNT3,
            ostWinNT4,
            ostWin2000, 
            ostWinXP,	//MSDN
            ostWin2003,	//MSDN
            ostVista,	//OK
            ostWindows7,//OK
            ostHighUnknown
        };//OSType;
        //_osver,_winmajor,_winminor
        FTLINLINE CFOSInfo();
        FTLINLINE BOOL IsGreaterWinNT() const;
        FTLINLINE OSType GetOSType() const;
        // if NT without ServicePack, return TRUE but csCSDVersion is ""
        FTLINLINE BOOL GetWinNTServicePackName(LPTSTR pszNTVersion,DWORD nSize) const;
        FTLINLINE DWORD GetNumberOfProcessors() const;
        FTLINLINE BOOL GetPhysicalBytes(DWORDLONG* pAvailablePhysicalBytes, DWORDLONG *pTotalPhysicalBytes) const;
        FTLINLINE BOOL GetVolumeVisibleName(LPCTSTR pszVolume, LPTSTR pszBuf, DWORD bufSize) const;
    private:
        OSVERSIONINFO m_OsInfo;
    };

    class CFRegUtil
    {
        //RegNotifyChangeKeyValue -- 检测注册表的变化
        //RegCreateKeyEx -- 创建注册表项，如果存在的话则打开
        //RegQueryValueEx -- 获取 字符串(REG_SZ)、DWORD(REG_DWORD) 注册表的 类型 和值
        //RegSetValueEx -- 设置值，如果lpSubKey为NULL或""，则设置默认值
        //RegOpenKeyEx -- 打开注册表
        //RegEnumKeyEx -- 枚举子键，可用于删除、查找子键等，没有更多时返回 ERROR_NO_MORE_ITEMS
        //RegDeleteKey -- 删除没有下级子键的子键,如果要递归删除，可以使用 CRegKey::RecurseDeleteKey 
    };

	//SystemParametersInfo
	class SystemParamProperty
	{
	public:
		FTLINLINE SystemParamProperty();
		FTLINLINE ~SystemParamProperty();
		FTLINLINE LPCTSTR GetPropertyString();
	public:
		int		m_nKeyboardSpeed;
	private:
		CFStringFormater	m_strFormater;
		FTLINLINE BOOL Init();
	};

	struct VERSIONINFO
	{
		VERSIONINFO()
		{
			dwVer0 = 0;
			dwVer1 = 0;
			dwVer2 = 0;
			dwVer3 = 0;
		}
		bool operator < (const VERSIONINFO & other) const
		{
			COMPARE_MEM_LESS(dwVer0, other);
			COMPARE_MEM_LESS(dwVer1, other);
			COMPARE_MEM_LESS(dwVer2, other);
			COMPARE_MEM_LESS(dwVer3, other);
			return false;
		}
		DWORD dwVer0;
		DWORD dwVer1;
		DWORD dwVer2;
		DWORD dwVer3;
	};

    FTLEXPORT class CFSystemUtil
    {
    public:
		//从版本信息字符串里获取对应的版本号
		FTLINLINE static BOOL GetVersionFromString(VERSIONINFO& ver, LPCTSTR pszVersion);

		//获取当前登录的用户名
		FTLINLINE static BOOL GetCurrentUserID( LPTSTR pszUserName, int iSize );

		FTLINLINE static LPCTSTR GetClipboardFormatString(UINT uFormat, LPTSTR lpszFormatName, int cchMaxCount);

        FTLINLINE static BOOL Is64BitWindows();

        //static BOOL GetLocalizedDateString();
        //static BOOL GetLocalizedTimeString();
        FTLINLINE static BOOL SuspendProcess(DWORD dwProcessID,BOOL fSuspend,DWORD skipedThreadId = ::GetCurrentThreadId());
        
        //如果是拷贝位图的话，可以用 CF_BITMAP、CF_DIB 等
        FTLINLINE static BOOL CopyTextToClipboard ( LPCTSTR szMem , HWND hWndOwner = GetClipboardOwner());
        
        //判断当前系统是大端系统还是小端系统(操作系统有没有方式) -- intel的X86是小端，Apple的RISC是大端
        //大端模式 -- 数据的低位保存在内存的高地址中，而数据的高位保存在内存的低地址中
        //  0x12345678 保存为 {低 0x12, 0x34, 0x56, 0x78 高}
        //小端模式 -- 数据的低位保存在内存的低地址中，而数据的高位保存在内存的高地址中
        //  0x12345678 保存为 {低 0x78, 0x56, 0x34, 0x12 高}
        //
        //实际上在 short 中保存 0x0102 就可以了
        //union 
        //{
        //    short s;
        //    char c[sizeof(short)];
        //}un;
        //un.s = 0x0102; //if(1==un.c[0] && 2==un.c[1]){big;}
        FTLINLINE static BOOL IsLittleSystem();

        //把Dos的结束符转换为Unix的结束符(去掉其中的 "\r" )
        FTLINLINE static int DosLineToUnixLine(const char *src, char *dest, int maxlen);
        //把Unix的结束符转换为Dos的结束符(将 "\r" 改成 "\r\n" )
        FTLINLINE static int UnixLineToDosLine(const char *src, char *dest, int maxlen);

    };

    class CFSystemMetricsProperty
    {
    public:
        #define SYSTEM_METRICS_PROPERTY_GET_DEFAULT     0x0000FFFF
    public:
        FTLINLINE CFSystemMetricsProperty();
        FTLINLINE ~CFSystemMetricsProperty();
        FTLINLINE LPCTSTR GetPropertyString(DWORD dwPropertyGet = SYSTEM_METRICS_PROPERTY_GET_DEFAULT);
    public:
        int     m_cxScreen;
        int     m_cyScreen;
        int     m_cxVScroll;
        int     m_cyHScroll;
        int     m_cyCaption;
        int     m_cxBorder;
        int     m_cyBorder;
        int     m_cxDlgFrame;
        int     m_cyDlgFrame;
        int     m_cyVThumb;
        int     m_cxHThumb;
        int     m_cxIcon;
        int     m_cyIcon;
        int     m_cxCursor;
        int     m_cyCursor;
        int     m_cyMenu;
        int     m_cxFullScreen;
        int     m_cyFullScreen;
        int     m_cyKanjiWindow;
        int     m_MousePresent;
        int     m_cyVscroll;
        int     m_cxHscroll;
        int     m_Debug;
        int     m_SwapButton;
        int     m_Reserved1;
        int     m_Reserved2;
        int     m_Reserved3;
        int     m_Reserved4;
        int     m_cxMin;
        int     m_cyMin;
        int     m_cxSize;
        int     m_cySize;
        int     m_cxFrame;
        int     m_cyFrame;
        int     m_cxMinTrack;
        int     m_cyMinTrack;
        int     m_cxDoubleClk;
        int     m_cyDoubleClk;
        int     m_cxIconSpacing;
        int     m_cyIconSpacing;
        int     m_MenuDropAlignment;
        int     m_PenWindows;
        int     m_DBCSEnabled;
        int     m_CMouseButtons;

#if(WINVER >= 0x0400)
        int     m_cxFixedFrame;//           SM_CXDLGFRAME  /* ;win40 name change */
        int     m_cyFixedframe;//           SM_CYDLGFRAME  /* ;win40 name change */
        int     m_cxSizeFrame;//            SM_CXFRAME     /* ;win40 name change */
        int     m_cySizeFrame;//            SM_CYFRAME     /* ;win40 name change */

        int     m_Secure;
        int     m_cxEdge;
        int     m_cyEdge;
        int     m_cxMinSpacing;
        int     m_cyMinSpacing;
        int     m_cxSMIcon;
        int     m_cySMIcon;
        int     m_cySMCaption;
        int     m_cxSMSize;
        int     m_cySMSize;
        int     m_cxMenuSize;
        int     m_cyMenuSize;
        int     m_Arrange;
        int     m_cxMinimized;
        int     m_cyMinimized;
        int     m_cxMaxTrack;
        int     m_cyMaxTrack;
        int     m_cxMaximized;
        int     m_cyMaximized;
        int     m_Network;
        int     m_CleanBoot;            //how the system is started
        int     m_cxDrag;
        int     m_cyDrag;

#endif /* WINVER >= 0x0400 */

        int     m_ShowSounds;

#if(WINVER >= 0x0400)
        int     m_cxMenuCheck;
        int     m_cyMenuCheck;
        int     m_SlowMachine;          //low-end (slow) processor
        int     m_MideaStenabled;       //enabled for Hebrew and Arabic languages
#endif /* WINVER >= 0x0400 */

#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
        int     m_MouseWheelPresent;
#endif

#if(WINVER >= 0x0500)
        int     m_XVirtualScreen;
        int     m_YVirtualScreen;
        int     m_cxVirtualScreen;
        int     m_cyVirtualScreen;
        int     m_CMonitors;
        int     m_SameDisplayFormat;
#endif /* WINVER >= 0x0500 */

#if(_WIN32_WINNT >= 0x0500)
        int     m_IMMEnabled;
#endif /* _WIN32_WINNT >= 0x0500 */

#if(_WIN32_WINNT >= 0x0501)
        int     m_cxFocusBorder;
        int     m_cyFocusBorder;
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0501)
        int     m_TabletPc;             //Windows XP Tablet PC edition
        int     m_MediaCenter;          //Windows XP Media Center Edition
        int     m_Starter;              //Windows XP Starter Edition
        int     m_Serverr2;             //Windows Server 2003 R2
#endif /* _WIN32_WINNT >= 0x0501 */

#if(_WIN32_WINNT >= 0x0600)
        int     m_MouseHorizontalWheelPresent;
        int     m_cxPaddedBorder;
#endif /* _WIN32_WINNT >= 0x0600 */

        int     m_CMetrics;

#if(WINVER >= 0x0500)
        int     m_RemoteSession;
#  if(_WIN32_WINNT >= 0x0501)
        int     m_ShuttingDown;
#  endif /* _WIN32_WINNT >= 0x0501 */
#  if(WINVER >= 0x0501)
        int     m_RemoteControl;
#  endif /* WINVER >= 0x0501 */
#  if(WINVER >= 0x0501)
        int     m_CaretBlinkingEnabled;
#  endif /* WINVER >= 0x0501 */
#endif /* WINVER >= 0x0500 */

    private:
        CFStringFormater	m_strFormater;
        DWORD   m_dwOldGetProperty;
    };

    class CFSecurityUtil
    {
        //创建一个任何人都可以使用的Mutex -- ATL中已有函数？

        //SECURITY_DESCRIPTOR sd;
        //BOOL ret = InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
        //ret = SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE);
        //SECURITY_ATTRIBUTES sa;
        //sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        //sa.lpSecurityDescriptor = &sd;
        //sa.bInheritHandle = FALSE;
        //::CreateMutex(&sa, FALSE, NULL);
    };

    class CFTempFpuRcReset  //FPU的四舍五入设定 -- 能提高有效位数？
    {
    public:
        CFTempFpuRcReset()
            :m_oldVal(_control87(0, 0))
        {
            //_control87(_EM_DENORMAL | _EM_ZERODIVIDE | _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT,  _MCW_EM);
            //_control87(_PC_53, _MCW_PC);

            //_control87(_RC_CHOP, _MCW_RC);
            _control87(_RC_NEAR, _MCW_RC);

        }
        ~CFTempFpuRcReset()
        {
            _control87(m_oldVal, _MCW_RC);
        }
    private:
        //禁止拷贝构造和赋值函数
        CFTempFpuRcReset& operator= (const CFTempFpuRcReset&);
        CFTempFpuRcReset(CFTempFpuRcReset&);
        //! 保存以前的值
        const unsigned int m_oldVal;
    };

    //构造函数是无参数的，有参数的如何办？
    //两个参数:
    //  一个是插件的指针类型，一个是插件指针的释放方式，如采用 delete 或其对应的 Release 等
    template <typename T, typename F = FTL::ObjecteDeleter<T*> >
    class CFPluginMgrT
    {
    public:
        FTLINLINE BOOL Init(LPCTSTR pszPluginPath, LPCSTR pszProcName, LPCTSTR pszExtName= TEXT("*.dll"));
        FTLINLINE void UnInit();
        FTLINLINE BOOL LoadPlugin(LPCTSTR pszePlugPath, LPCSTR pszProcName);
        FTLINLINE ~CFPluginMgrT();
    public:
        typedef std::list<T*>       PluginContainer;
        typedef std::list<HMODULE>  ModuleContainer;
        PluginContainer         m_plugins;
        ModuleContainer         m_modules;
    private:
        typedef T* (* FunGetInstance )();
    };

//    class CFStringUtil
//    {
//    public:
//        static HRESULT ConvertString2LONG(LPCTSTR pszValue, BOOL bIsHex, LONG* pValue)
//        {
//            CHECK_POINTER_WRITABLE_DATA_RETURN_VALUE_IF_FAIL(pValue,sizeof(LONG), E_POINTER);
//            UNREFERENCED_PARAMETER(pszValue);
//            UNREFERENCED_PARAMETER(bIsHex);
//            UNREFERENCED_PARAMETER(pValue);
//#if 0
//            int nIndex;
//            int nPower = 0;
//            int nPowerDec = 1;
//            int nDigit = 0;
//            *pValue = 0;
//
//            StringCchLength(pszValue, MAX_PATH, (size_t *)&nIndex);
//            nIndex--;
//
//            // skip whitespace
//            while( nIndex >= 0 )
//            {
//                if( pszValue[nIndex] != TEXT(' ') && 
//                    ( pszValue[nIndex] != TEXT('L') || pszValue[nIndex] != TEXT('l')))
//                    break;
//                nIndex--;
//            }
//
//            bool bFoundDigit = false;
//            while( nIndex >= 0 )
//            {
//                // Convert to uppercase
//                if( pszValue[nIndex] >= 'a' && pszValue[nIndex] <= 'z' )
//                    pszValue[nIndex] += 'A' - 'a';
//
//                if( bIsHex && pszValue[nIndex] >= 'A' && pszValue[nIndex] <= 'F' )
//                    nDigit = pszValue[nIndex] - 'A' + 10;
//                else if( pszValue[nIndex] >= '0' && pszValue[nIndex] <= '9' )
//                    nDigit = pszValue[nIndex] - '0';
//                else if( pszValue[nIndex] == '-' )
//                {
//                    *pValue = -*pValue;
//                    break;
//                }
//                else
//                {
//                    // break if we've found a number, but don't break otherwise
//                    // This will skip any random letters in the front & end of the string
//                    if( bFoundDigit )
//                    {
//                        break;
//                    }
//                    else
//                    {
//                        nIndex--;
//                        continue;
//                    }
//                }
//
//                bFoundDigit = true;
//                if( bIsHex )
//                    *pValue += ( nDigit << (nPower*4) );
//                else
//                    *pValue += ( nDigit * nPowerDec );
//
//                nPowerDec *= 10;
//                nIndex--;
//                nPower++;
//            }
//#endif 
//            return E_NOTIMPL;
//        }
//    };

}

#endif //FTL_SYSTEM_H

#ifndef USE_EXPORT
#  include "ftlSystem.hpp"
#endif