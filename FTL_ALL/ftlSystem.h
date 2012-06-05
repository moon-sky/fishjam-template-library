
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
* ����ϵͳ
*   4�����ԣ������ԡ������ԡ������� �� ��ȷ���ԡ�
*   5���������ܣ����̹������ļ��������洢�������豸���� �� ��ҵ������
* 
* S1˯��ģʽ -- ���봦������ͣ���ڴ���ˢ�¡�����ϵͳ���ڵ͵���ģʽ
* S3/S4˯��ģʽ -- δ���������봦�������ڴ��ӻ���������Դ���ڵ͵���ģʽ
* 
* ����ϵͳ�ṩ��IPC����һ���Ϊ���ࣺ
*   1.����IPC -- �����ڴ桢�ܵ���Unix����socket��door(?)���źŵȣ�
*   2.Զ��IPC -- Internet����socket��Win32�����ܵ�(named pipe)��
*
* ͨ�����ܼ������ӳ������ܺ�й©��
*   ������� -> �������� -> ���� -> ������־�;��� -> ��������־ -> �Ҽ� -> �½���־���� -> ����
*   �����桱�С����Ӽ������� -> "���ܶ���" ѡ�� "Process",ͨ����˵Ϊ�˼���ڴ桢��Դй¶����Ҫ�������¼��֣�
*     Handle Count -- �����
*     Working Set -- �ڴ�ʹ��
*     Virtual Bytes -- �����ڴ��С
*   ���û���ڴ�й¶��Ӧ�������ض������ڲ�����������������������ƣ�ͨ��˵����й©��
*
* �滻�����������
*   HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\taskmgr.exe
*   ���� Debugger ���ֵ���� "H:\TOOLS\SYSTEM\PROCESSEXPLORER\PROCEXP.EXE"
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
    //��ʵ��ģʽ��ģ��
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
        //RegNotifyChangeKeyValue -- ���ע����ı仯
        //RegCreateKeyEx -- ����ע����������ڵĻ����
        //RegQueryValueEx -- ��ȡ �ַ���(REG_SZ)��DWORD(REG_DWORD) ע����� ���� ��ֵ
        //RegSetValueEx -- ����ֵ�����lpSubKeyΪNULL��""��������Ĭ��ֵ
        //RegOpenKeyEx -- ��ע���
        //RegEnumKeyEx -- ö���Ӽ���������ɾ���������Ӽ��ȣ�û�и���ʱ���� ERROR_NO_MORE_ITEMS
        //RegDeleteKey -- ɾ��û���¼��Ӽ����Ӽ�,���Ҫ�ݹ�ɾ��������ʹ�� CRegKey::RecurseDeleteKey 
    };


    FTLEXPORT class CFSystemUtil
    {
    public:
		FTLINLINE static LPCTSTR GetClipboardFormatString(UINT uFormat, LPTSTR lpszFormatName, int cchMaxCount);

        FTLINLINE static BOOL Is64BitWindows();

        //static BOOL GetLocalizedDateString();
        //static BOOL GetLocalizedTimeString();
        FTLINLINE static BOOL SuspendProcess(DWORD dwProcessID,BOOL fSuspend,DWORD skipedThreadId = ::GetCurrentThreadId());
        
        //����ǿ���λͼ�Ļ��������� CF_BITMAP��CF_DIB ��
        FTLINLINE static BOOL CopyTextToClipboard ( LPCTSTR szMem , HWND hWndOwner = GetClipboardOwner());
        
        //�жϵ�ǰϵͳ�Ǵ��ϵͳ����С��ϵͳ(����ϵͳ��û�з�ʽ) -- intel��X86��С�ˣ�Apple��RISC�Ǵ��
        //���ģʽ -- ���ݵĵ�λ�������ڴ�ĸߵ�ַ�У������ݵĸ�λ�������ڴ�ĵ͵�ַ��
        //  0x12345678 ����Ϊ {�� 0x12, 0x34, 0x56, 0x78 ��}
        //С��ģʽ -- ���ݵĵ�λ�������ڴ�ĵ͵�ַ�У������ݵĸ�λ�������ڴ�ĸߵ�ַ��
        //  0x12345678 ����Ϊ {�� 0x78, 0x56, 0x34, 0x12 ��}
        //
        //ʵ������ short �б��� 0x0102 �Ϳ�����
        //union 
        //{
        //    short s;
        //    char c[sizeof(short)];
        //}un;
        //un.s = 0x0102; //if(1==un.c[0] && 2==un.c[1]){big;}
        FTLINLINE static BOOL IsLittleSystem();

        //��Dos�Ľ�����ת��ΪUnix�Ľ�����(ȥ�����е� "\r" )
        FTLINLINE static int DosLineToUnixLine(const char *src, char *dest, int maxlen);
        //��Unix�Ľ�����ת��ΪDos�Ľ�����(�� "\r" �ĳ� "\r\n" )
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
        //����һ���κ��˶�����ʹ�õ�Mutex -- ATL�����к�����

        //SECURITY_DESCRIPTOR sd;
        //BOOL ret = InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
        //ret = SetSecurityDescriptorDacl(&sd, TRUE, (PACL)NULL, FALSE);
        //SECURITY_ATTRIBUTES sa;
        //sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        //sa.lpSecurityDescriptor = &sd;
        //sa.bInheritHandle = FALSE;
        //::CreateMutex(&sa, FALSE, NULL);
    };

    class CFTempFpuRcReset  //FPU�����������趨 -- �������Чλ����
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
        //��ֹ��������͸�ֵ����
        CFTempFpuRcReset& operator= (const CFTempFpuRcReset&);
        CFTempFpuRcReset(CFTempFpuRcReset&);
        //! ������ǰ��ֵ
        const unsigned int m_oldVal;
    };

    //���캯�����޲����ģ��в�������ΰ죿
    //��������:
    //  һ���ǲ����ָ�����ͣ�һ���ǲ��ָ����ͷŷ�ʽ������� delete �����Ӧ�� Release ��
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