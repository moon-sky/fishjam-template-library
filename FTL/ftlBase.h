///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlbase.h
/// @brief  Fishjam Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlbase ftl base function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_BASE_H
#define FTL_BASE_H
#pragma once

#ifndef __cplusplus
#  error FTL requires C++ compilation (use a .cpp suffix)
#endif

#include "ftlDefine.h"

//#if defined USE_THREAD_SAFE_TRACE
#define FTLTRACE    FAST_TRACE
#define FTLTRACEEX  FAST_TRACE_EX
//#endif //USE_THREAD_SAFE_TRACE

//调用方可以重新指定日志输出和断言的机制，如果没有指定，使用默认
#if (!defined FTLASSERT)
#  if defined ATLTRACE
#    define FTLASSERT          ATLASSERT
#  elif defined TRACE
#    define FTLASSERT          ASSERT
#else
#   include <Windows.h>
#   include <tchar.h>
#   include <atlconv.h>
#   include <cassert>
#   define FTLASSERT          assert
#  endif
#endif 

// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after the FTL include files
//#  pragma warning(disable: 4995)  //'%s': name was marked as #pragma deprecated
//#  pragma warning(disable: 4996)  //'%s': This function or variable may be unsafe.

#define STRSAFE_NO_DEPRECATE
#  include <strsafe.h>
//#pragma warning(default: 4995)
//#pragma warning(default: 4996)

#include <set>
#include <vector>
#include <map>

namespace FTL
{
    //考虑到字节对其的问题，定义为 8 的倍数
    #define MAX_BUFFER_LENGTH       240

    //禁止拷贝构造和赋值操作符
    #define DISABLE_COPY_AND_ASSIGNMENT(className)  \
    private:\
        className(const className& ref);\
        className& operator = (const className& ref)

    //在每个类中使用该宏，可以使得测试类定义为产品类的友元类 -- 支持访问私有成员变量
    #define ENABLE_UNIT_TEST(cls) friend class cls##Tester; 

    #ifdef _DEBUG
    #  define DBG_REPORT    _CrtDbgReport
    #  define DBG_BREAK     _CrtDbgBreak
    #else
    #  define DBG_REPORT    __noop
    #  define DBG_BREAK     __noop  
    #endif

    #ifdef FTL_DEBUG
    #  define REPORT_ERROR_INFO(c, e, x) \
         do{ \
             c _err(e);\
             USES_CONVERSION;\
             FAST_TRACE_EX(FTL::tlError, TEXT("%s(%d) :\t Error!!! Reason = 0x%08x(%d,%s),Code:\"%s\" \n"),\
               TEXT(__FILE__),__LINE__, e, e, _err.GetConvertedInfo(),TEXT(#x));\
             (1 != DBG_REPORT(_CRT_ASSERT, __FILE__, __LINE__, NULL, T2CA(_err.GetConvertedInfo()))) || \
               (DBG_BREAK(), 0);\
         }while(0)
    #else //Not Define FTL_DEBUG
    #  define REPORT_ERROR_INFO(c, e, x) __noop
    #endif //FTL_DEBUG


    //自己构造 HRESULT 错误值 -- MAKE_HRESULT(sev,fac,code)
    //  SEVERITY_XXX -- 严重程度(0:成功；1:错误)
    //  FACILITY_XXX -- 设备信息(错误的类型，如 RPC、SECURITY、WIN32 等)
    //可以通过 HRESULT_SEVERITY、HRESULT_FACILITY、HRESULT_CODE 分别获取对应的值

    #ifdef FTL_DEBUG  //调试版本时，获取并判断 LastError，为了防止 REPORT_ERROR_INFO 对LastError产生影响，需要重新设置
    # define API_ASSERT(x)\
        if(FALSE == (x))\
        {\
            DWORD dwLastError = GetLastError();\
            REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, dwLastError,x);\
            SetLastError(dwLastError);\
        }

    # define API_VERIFY(x)   \
        bRet = (x);\
        if(FALSE == bRet)\
        {\
            DWORD dwLastError = GetLastError();\
            REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, dwLastError,x);\
            SetLastError(dwLastError);\
        }
    # define API_VERIFY_EXCEPT1(x,e1)\
        bRet = (x);\
        if(FALSE == bRet)\
        {\
            DWORD dwLastError = GetLastError();\
            if(dwLastError != e1)\
            {\
                REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, dwLastError,x);\
            }\
            SetLastError(dwLastError);\
        }
    #else //没有定义 FTL_DEBUG 的时候 -- 不进行 GetLastError/SetLastError 的调用
    # define API_ASSERT(x)  
    # define API_VERIFY(x)   \
        bRet = (x);
    # define API_VERIFY_EXCEPT1(x,e1)\
        bRet = (x);
    #endif //FTL_DEBUG

    //如果返回 E_FAIL，并且支持 ISupportErrorInfo 的话，需要取得Rich Error 错误信息
    # define COM_VERIFY(x)   \
        hr = (x);\
        if(S_OK != hr)\
        {\
            REPORT_ERROR_INFO(FTL::CFComErrorInfo, hr,x);\
        }

    # define COM_VERIFY_EXCEPT1(x,h1) \
        hr = (x);\
        if(S_OK != hr && (h1)!= hr)\
        {\
            REPORT_ERROR_INFO(FTL::CFComErrorInfo, hr,x);\
        }

    # define COM_VERIFY_EXCEPT2(x,h1,h2) \
        hr = (x);\
        if(S_OK != hr && (h1)!= hr && (h2) != hr)\
        {\
            REPORT_ERROR_INFO(FTL::CFComErrorInfo, hr,x);\
        }

    # define REG_VERIFY(x)   \
        lRet = (x);\
        if(ERROR_SUCCESS != lRet)\
        {\
            REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, lRet,x);\
        }

    # define REG_VERIFY_EXCEPT1(x,ret1)   \
        lRet = (x);\
        if(ERROR_SUCCESS != lRet && (ret1) != lRet )\
        {\
            REPORT_ERROR_INFO(FTL::CFAPIErrorInfo, lRet,x);\
        }

    ////////////////////////////////////////////////////////////////////////////////////////

    #ifndef _countof
    # define _countof(arr) (sizeof(arr) / sizeof(arr[0]))
    #endif

    #ifndef tstring
    #  if defined UNICODE 
    #     define tstring wstring
    #  else
    #     define tstring string
    #  endif 
    #endif


    //! @code SAFE_CLOSE_HANDLE(m_hFile,INVALID_HANDLE_VALUE); 注意 NULL 和 INVALID_HANDLE_VALUE
    #ifndef SAFE_CLOSE_HANDLE
    #  ifdef FTL_DEBUG
    #    define SAFE_CLOSE_HANDLE(h,v) if((v) != (h)) { BOOL oldbRet = bRet; API_VERIFY(::CloseHandle((h))); (h) = (v); bRet = oldbRet; }
    #  else
    #    define SAFE_CLOSE_HANDLE(h,v) if((v) != (h)) { ::CloseHandle((h)); (h) = (v); bRet = bRet; }
    #  endif
    #endif

    #ifndef SAFE_CLOSE_REG
    #  ifdef FTL_DEBUG
    #    define SAFE_CLOSE_REG(h) if(NULL != (h)) { BOOL oldbRet = bRet; API_VERIFY(ERROR_SUCCESS == ::RegCloseKey((h))); (h) = NULL; bRet = oldbRet; }
    #  else
    #    define SAFE_CLOSE_REG(h) if(NULL != (h)) { ::RegCloseKey((h)); (h) = NULL; }
    #  endif
    #endif

    #ifndef SAFE_DELETE
    #  define SAFE_DELETE(p) if( NULL != (p) ){ delete (p); (p) = NULL; }
    #endif

    #ifndef SAFE_DELETE_ARRAY
    #  define SAFE_DELETE_ARRAY(p) if( NULL != (p) ){ delete [] (p); (p) = NULL; }
    #endif

    #ifndef SAFE_FREE
    #  define SAFE_FREE(p)   if(NULL != (p)) { free((p)); (p) = NULL;}
    #endif

    #ifndef SAFE_LOCAL_FREE
    #  define SAFE_LOCAL_FREE(p) if(NULL != (p)) { ::LocalFree((p)); (p) = NULL; }
    #endif

    #ifndef SAFE_HEAP_FREE
    #  define SAFE_HEAP_FREE(p) if(NULL != (p)) { ::HeapFree(GetProcessHeap(),0,(p)); (p) = NULL; }
    #endif

    #ifndef SAFE_FREE_BSTR
    #  define SAFE_FREE_BSTR(s) if(NULL != (s)){ ::SysFreeString((s)); (s) = NULL; }
    #endif

    #ifndef SAFE_RELEASE
    #  define SAFE_RELEASE(p)  if( NULL != ((p)) ){ (p)->Release(); (p) = NULL; }
    #endif 

    #ifndef SAFE_COTASKMEMFREE
    #  define SAFE_COTASKMEMFREE(p) if(NULL != (p)){ ::CoTaskMemFree((p)); (p) = NULL; }
    #endif

    #ifndef SAFE_FREE_LIBRARY
    #  define SAFE_FREE_LIBRARY(p) if(NULL != (p)){ ::FreeLibrary(static_cast<HMODULE>(p)); (p) = NULL; }
    #endif


    ////////////////////////////////////////////////////////////////////////////////////////
    #ifndef HANDLE_CASE_TO_STRING
    # define HANDLE_CASE_TO_STRING(buf,len,c)\
        case (c):\
        StringCchCopy(buf,len,TEXT(#c));\
        break;
    #endif

    #ifndef HANDLE_CASE_RETURN_STRING
    # define HANDLE_CASE_RETURN_STRING(c) \
        case (c):\
        return TEXT(#c);
    #endif 

    #define FTL_MIN(a,b)                (((a) < (b)) ? (a) : (b))
    #define FTL_MAX(a,b)                (((a) > (b)) ? (a) : (b))
    #define FTL_ABS(a)		            (((a) < 0) ? -(a) : (a))
    #define FTL_SIGN(a)		            (((a) > 0) ? 1 : (((a) < 0) ? -1 : 0))
    #define FTL_INRANGE(low, Num, High) (((low) <= (Num)) && ((Num) <= (High)))


    #define CHECK_POINTER_RETURN_IF_FAIL(p)    \
        if(NULL == (p))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            return;\
        }

    #define CHECK_POINTER_RETURN_VALUE_IF_FAIL(p,r)    \
        if(NULL == p)\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            return r;\
        }

    #define CHECK_POINTER_WRITABLE_RETURN_IF_FAIL(p)    \
        if(NULL == p || ::IsBadWritePtr(p, sizeof(DWORD_PTR)))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid writable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadWritePtr(p, sizeof(DWORD_PTR)));\
            return;\
        }

    #define CHECK_POINTER_WRITABLE_RETURN_VALUE_IF_FAIL(p,r)    \
        if(NULL == p || ::IsBadWritePtr(p, sizeof(DWORD_PTR)))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid writable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadWritePtr(p, sizeof(DWORD_PTR)));\
            return r;\
        }

    #define CHECK_POINTER_READABLE_RETURN_IF_FAIL(p)    \
        if(NULL == p || ::IsBadReadPtr(p, sizeof(DWORD_PTR)))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid readable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadReadPtr(p, sizeof(DWORD_PTR)));\
            return;\
        }

    #define CHECK_POINTER_READABLE_RETURN_VALUE_IF_FAIL(p,r)    \
        if(NULL == p || ::IsBadReadPtr(p, sizeof(DWORD_PTR)))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid readable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadReadPtr(p, sizeof(DWORD_PTR)));\
            return r;\
        }

    #define CHECK_POINTER_WRITABLE_DATA_RETURN_IF_FAIL(p,len)    \
        if(NULL == p || ::IsBadWritePtr(p, len))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid %d len writable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p,len);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadWritePtr(p, len));\
            return;\
        }

    #define CHECK_POINTER_WRITABLE_DATA_RETURN_VALUE_IF_FAIL(p,len,r)    \
        if(NULL == p || ::IsBadWritePtr(p, len))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid %d len writable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p,len);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadWritePtr(p, len));\
            return r;\
        }

    #define CHECK_POINTER_READABLE_DATA_RETURN_IF_FAIL(p,len)    \
        if(NULL == p || ::IsBadReadPtr(p, len))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid %d len readable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p,len);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadReadPtr(p, len));\
            return;\
        }

    #define CHECK_POINTER_READABLE_DATA_RETURN_VALUE_IF_FAIL(p,len,r)    \
        if(NULL == p || ::IsBadReadPtr(p, len))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid %d len readable pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p,len);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadReadPtr(p, len));\
            return r;\
        }

    #define CHECK_POINTER_READ_WRIATE_ABLE_DATA_RETURN_IF_FAIL(p,len)    \
        if(NULL == p || ::IsBadReadPtr(p, len) || ::IsBadWritePtr(p,len))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid %d len read/write able pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p,len);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadReadPtr(p, len));\
            FTLASSERT(FALSE ==::IsBadWritePtr(p, len));\
            return;\
        }

    #define CHECK_POINTER_READ_WRIATE_ABLE_DATA_RETURN_VALUE_IF_FAIL(p,len,r)    \
        if(NULL == p || ::IsBadReadPtr(p, len))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid %d len read/write able pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p,len);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadReadPtr(p, len));\
            FTLASSERT(FALSE ==::IsBadWritePtr(p, len));\
            return r;\
        }

    #define CHECK_POINTER_ISSTRING_PTR_RETURN_IF_FAIL(p,r)    \
        if(NULL == p || ::IsBadStringPtr(p, INFINITE))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid string pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadStringPtr(p, INFINITE));\
            return;\
        }

    #define CHECK_POINTER_ISSTRING_PTR_RETURN_VALUE_IF_FAIL(p,r)    \
        if(NULL == p || ::IsBadStringPtr(p, INFINITE))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid string pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadStringPtr(p, INFINITE));\
            return r;\
        }

    #define CHECK_POINTER_CODE_RETURN_IF_FAIL(p)    \
        if(NULL == p || ::IsBadCodePtr(p))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid code pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadCodePtr(p));\
            return;\
        }

    #define CHECK_POINTER_CODE_RETURN_VALUE_IF_FAIL(p,r)    \
        if(NULL == p || ::IsBadCodePtr(p))\
        {\
            FTLTRACEEX(FTL::tlWarning, TEXT("%s(%d) : Warning!!! %s(0x%p) is not a valid code pointer\n"),TEXT(__FILE__),__LINE__, TEXT(#p),p);\
            FTLASSERT(NULL != p);\
            FTLASSERT(FALSE ==::IsBadCodePtr(p));\
            return r;\
        }
}

namespace FTL
{
    template<typename T>
    BOOL IsSameNumber(const T& expected, const T& actual, const T& delta);

    FTLEXPORT template<typename TBase, typename INFO_TYPE,LONG bufLen = MAX_BUFFER_LENGTH>
    class CFConvertInfoT
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFConvertInfoT);
    public:
        FTLINLINE explicit CFConvertInfoT(INFO_TYPE info);
        FTLINLINE virtual ~CFConvertInfoT();
        FTLINLINE LPCTSTR GetConvertedInfo();
        FTLINLINE const INFO_TYPE GetInfo() const;
        FTLINLINE void SetInfo(INFO_TYPE info);
    public:
        FTLINLINE virtual LPCTSTR ConvertInfo() = 0;
    protected:
        INFO_TYPE   m_Info;
        TCHAR       m_bufInfo[bufLen];
    };

    FTLEXPORT class CFAPIErrorInfo : public CFConvertInfoT<CFAPIErrorInfo,DWORD>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFAPIErrorInfo);
    public:
        FTLINLINE explicit CFAPIErrorInfo(DWORD dwError);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    FTLEXPORT class CFComErrorInfo : public CFConvertInfoT<CFComErrorInfo,HRESULT>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFComErrorInfo);
    public:
        FTLINLINE explicit CFComErrorInfo(HRESULT hr);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    protected:
        FTLINLINE LPCTSTR GetErrorFacility(HRESULT hr, LPTSTR pszFacility,DWORD dwLength);
    };

    //! 字符串格式化，可以根据传入的格式化字符长度自动调整，析构时释放分配的内存
    class CFStringFormater
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFStringFormater);
    public:
        FTLINLINE CFStringFormater(DWORD dwInitAllocLength = MAX_BUFFER_LENGTH);
        FTLINLINE virtual ~CFStringFormater();
        FTLINLINE HRESULT __cdecl Format(LPCTSTR lpszFormat, ...);
        FTLINLINE HRESULT __cdecl FormatV(LPCTSTR lpszFormat, va_list argList);
        FTLINLINE HRESULT __cdecl AppendFormat(LPCTSTR lpszFormat, ...);
        FTLINLINE HRESULT __cdecl AppendFormatV(LPCTSTR lpszFormat, va_list argList);
        FTLINLINE operator LPCTSTR() const
        {
            return m_pBuf;
        }
        FTLINLINE LPCTSTR GetString() const;
        FTLINLINE LONG  GetStringLength() const;
        FTLINLINE LPTSTR Detach();
    protected:
        LPTSTR  m_pBuf;
        DWORD   m_dwTotalSpaceSize;
        const DWORD m_dwInitAllocLength;
    };

    // The typedef for the debugging output function.  Note that this matches OutputDebugString.
    typedef VOID (WINAPI *PFNDEBUGOUT)(LPCTSTR lpOutputString) ;

    enum TraceLevel
    {
        tlDetail,
        tlInfo,
        tlTrace ,
        tlWarning,
        tlError,
        tlEnd,              //结束标记，用来计算枚举个数，不要直接使用
    };

    typedef struct FAST_TRACE_OPTIONS
    {
        FAST_TRACE_OPTIONS()
        {
            bWriteThrough   = FALSE;
            bWriteToFile    = FALSE;
            bDoTimings      = TRUE;
            traceThreshold  = tlTrace;
            pDebugOut       = OutputDebugString;
        }
        BOOL        bWriteToFile;           //是否写入文件，默认情况下关闭
        BOOL        bWriteThrough;          //如果写入文件的话，是否直接写出(效率低，但不会丢掉信息)
        BOOL        bDoTimings;             //是否写入当时的时间
        TraceLevel  traceThreshold;         //控制输出的阈值 -- 只有大于等于该阈值的才能输出，默认为 tlTrace
        PFNDEBUGOUT pDebugOut;              //扩展的输出方式，默认为 OutputDebugString;
    } FAST_TRACE_OPTIONS , * LPFAST_TRACE_OPTIONS ;
    

    //默认以 HMODULE_CUSTOM 和 tlTrace 进行输出
#ifdef FTL_DEBUG
#   define FAST_TRACE      FTL::CFFastTrace::GetInstance().WriteLogInfo
#   define FAST_TRACE_EX   FTL::CFFastTrace::GetInstance().WriteLogInfoEx
#else
#   define FAST_TRACE      __noop
#   define FAST_TRACE_EX   __noop
#endif

    #define FTFILESIG 'TSAF'
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 高性能的日志跟踪类，将各个线程的日志高性能的输出到单独的目的地(文件)中，查看时再进行统一
    /// 由于日志会大量使用，因此如果使用模版并且 inline 的话，是否会对文件大小、速度造成很大的影响？ 
    /// 也可以使用 #if(DEBUGLEVEL & BASIC_DEBUG)  #endif 的方式控制日志等级
    /// -- 实际代码中最好提取到单独的 DLL、Lib 模块中
    /// TODO
    ///   1.对于已经退出的线程需要进一步的处理 -- 从 m_AllFileWriters 中移除，否则可能越来越大
    ///     CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD...);
    ///   2.增加使用单独的写线程
    class CFFastTrace
    {
    public:
        typedef struct tagFTFILEHEADER
        {
            DWORD dwSig ;// The signature.  See the FTFILESIG define above.
            BOOL  bIsUnicode;  //表明当前是否是使用Unicode方式写的
            LONG  lItemCount ;// The total number of traces written to the file.
            LONG  lPID ;// The process ID for this file.  (Used to ensure a file matches it's siblings).
            LONG  lTID ;// The thread ID for this file.
        } FTFILEHEADER , * LPFTFILEHEADER ;

        typedef struct tagFTDATA
        {
            ULONG       lSeqNum ;// The sequence number for this trace.
            FILETIME    stTime ;
            HMODULE     module;
            TraceLevel  level;
            LONG        nTraceInfoLen; //pszTraceInfo 的长度，目前必须是 pszTraceInfo 字符串长度+1(包括结尾的NULL,不浪费空间)，考虑TCHAR
            LPCTSTR     pszTraceInfo;
        } FTDATA , * LPFTDATA ;
    public:
        FTLINLINE static CFFastTrace& GetInstance();
        
        //设置可以进行日志输出的类型(组合项)和等级(大于该等级的输出)
        FTLINLINE BOOL CheckLevel(TraceLevel level);
        FTLINLINE BOOL SetTraceOptions(LPFAST_TRACE_OPTIONS pOptions);
        FTLINLINE BOOL GetTraceOptions(LPFAST_TRACE_OPTIONS pOptions);
        //FTLINLINE BOOL FlushAllFiles();
        //FTLINLINE BOOL SnapAllFiles();
        //默认使用 tlTrace 进行输出
        FTLINLINE void /*__cdecl*/ WriteLogInfo(const LPCTSTR lpszFormat,...);
        FTLINLINE void /*__cdecl*/ WriteLogInfoEx(TraceLevel level,const LPCTSTR lpszFormat,...);
    protected:
        class CFTFileWriter// : public CFTargetWriter<HANDLE>
        {
        public:
            FTLINLINE CFTFileWriter();
            FTLINLINE ~CFTFileWriter();
            FTLINLINE BOOL Initialize ( HANDLE hFile , DWORD dwTID );
            FTLINLINE BOOL WriteData ( LPFTDATA pData ) ;
            FTLINLINE BOOL Flush ();
            FTLINLINE BOOL SnapFile ( );
            FTLINLINE BOOL Close();
        protected:
            HANDLE m_hFile ;    // The actual file handle.
            LONG   m_lWrites ;  // The total number of writes to the file.
            DWORD  m_dwTID ;    // The thread this file wraps.
            FTLINLINE BOOL WriteHeader () ;
        };
    protected:
        DWORD                   m_dwTLSSlot;            //保存线程局部储存的Slot值
        ULONG                   m_SequenceNumber;       //序列号
        FAST_TRACE_OPTIONS      m_Options;
        CRITICAL_SECTION        m_CsLock;               //在更改Option等时进行互斥
        typedef std::set<CFTFileWriter*>            AllFileWriterArrayType;
        typedef AllFileWriterArrayType::iterator    AllFileWriterArrayIterator;
        typedef std::pair<AllFileWriterArrayIterator , bool > AllFileWriterArrayPair;
        AllFileWriterArrayType  m_AllFileWriters;

        //FAST_TRACE_MODULE_INFO  m_AllTraceModuleInfos[MAX_MODULE_INFO_COUNT];
        //LONG                    m_CurrentModuleCount;
        //typedef std::map<HMODULE,FAST_TRACE_MODULE_INFO>    AllTraceModuleInfoType;
        //typedef std::pair<AllTraceModuleInfoType::iterator , bool > AllTraceModuleInfoPair;
        //AllTraceModuleInfoType   m_AllTraceModuleInfos;

        FTLINLINE CFTFileWriter* GetCurrentThreadFile();
        FTLINLINE HANDLE CreateRawThreadFile( DWORD dwTID );
        FTLINLINE BOOL BuildThreadFileName( LPTSTR szFileNameBuff , size_t iBuffLen , DWORD dwTID );
        FTLINLINE BOOL AddFileToArray(CFTFileWriter* pFileWriter);
        FTLINLINE BOOL RemoveFileFromArray(CFTFileWriter* pFileWriter);
        FTLINLINE BOOL CloseAllFileWriters();
        FTLINLINE void InternalWriteLogData(TraceLevel level, LPCTSTR pStrInfo);
    private:
        FTLINLINE CFFastTrace(LPCTSTR pszKey, HMODULE hModule);
        FTLINLINE ~CFFastTrace();
    };

    //高性能的计时器，能用于计算花费的时间(如计算拷贝时的速度估计) -- 支持暂停/继续
    #define NANOSECOND_PER_MILLISECOND  (1000 * 1000)
    #define MILLISECOND_PER_SECOND      (1000)
    #define NANOSECOND_PER_SECOND       (NANOSECOND_PER_MILLISECOND * MILLISECOND_PER_SECOND)

    enum RunningStatus
    {
        rsStopped,
        rsRunning,
        rsPaused
    };

    class CFElapseCounter
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFElapseCounter);
    public:
        FTLINLINE CFElapseCounter();
        FTLINLINE ~CFElapseCounter();
        FTLINLINE BOOL Start();
        FTLINLINE BOOL Pause();
        FTLINLINE BOOL Resume();
        FTLINLINE BOOL Stop();
        FTLINLINE RunningStatus GetStatus() const;

        //! 运行状态，获取当前时间到开始的时间
        //! 暂停状态，获取暂停结束时间到开始的时间
        //! 停止状态，获取结束到开始的时间
        FTLINLINE LONGLONG GetElapseTime(); //返回单位是纳秒(NS)
    private:
        LARGE_INTEGER   m_Frequency;
        LARGE_INTEGER   m_StartTime;
        LARGE_INTEGER   m_PauseTime;
        LARGE_INTEGER   m_StopTime;
        RunningStatus   m_Status;
    };

#ifdef __cplusplus
    extern "C"
#endif
    void * _ReturnAddress(void);
#   pragma intrinsic(_ReturnAddress)

#ifndef DEFAULT_BLOCK_TRACE_THRESHOLD    //默认的追踪阈值为100毫秒
#  define DEFAULT_BLOCK_TRACE_THRESHOLD  (100)
#endif 

#ifndef MAX_TRACE_INDICATE_LEVEL        //默认最多追踪50层
#  define MAX_TRACE_INDICATE_LEVEL    (50)
#endif 

#pragma message( "  MAX_TRACE_INDICATE_LEVEL = " QQUOTE(MAX_TRACE_INDICATE_LEVEL) )

    //CFBlockElapse JOIN_TWO(elapse,__LINE__) (TEXT(__FILE__),__LINE__,TEXT(__FUNCTION__),FTL::_ReturnAddress(),(minElapse))
    // #pragma TODO(此处的写法有问题，无法根据行号生成唯一变量 -- "JOIN_TWO" 不支持带参数的构造)
#ifdef FTL_DEBUG
#  define FUNCTION_BLOCK_INIT()     FTL::CFBlockElapse::Init()
    

#  define FUNCTION_BLOCK_TRACE(minElapse) \
    FTL::CFBlockElapse elapse ## __LINE__ (TEXT(__FILE__),__LINE__,TEXT(__FUNCTION__),FTL::_ReturnAddress(),(minElapse))
#  define FUNCTION_BLOCK_NAME_TRACE(blockName,minElapse) \
    FTL::CFBlockElapse elapse ## __LINE__(TEXT(__FILE__),__LINE__,blockName,FTL::_ReturnAddress(),minElapse)

#  define FUNCTION_BLOCK_UNINIT()   FTL::CFBlockElapse::UnInit()
#else
#  define  FUNCTION_BLOCK_INIT();                           __noop
#  define  FUNCTION_BLOCK_TRACE(minElapse)                  __noop
#  define  FUNCTION_BLOCK_NAME_TRACE(blockName,minElapse)   __noop
#  define  FUNCTION_BLOCK_UNINIT();                         __noop
#endif


#ifdef FTL_DEBUG        //由于BlockElapse会消耗大量的时间，因此，只在Debug状态下启用
    //! 跟踪程序的运行逻辑和效率
    //! @code FUNCTION_BLOCK_TRACE(1000) -- 指定的代码块需要在1000毫秒中执行完，否则打印日志
    //! 由于每个线程都有自己的 s_Indent/s_bufIndicate(静态全局线程局部变量)，因此其操作方法是线程安全的
    //! Bug：
    //!   1.如果是 EXE + DLL 的方式，则线程同时调用Exe + Dll 时，会认为是不同的线程。
    //!   2.如果 DLL 是使用__declspec(thread) 分配线程本地存储区，客户端应用程序必须隐式链接到 DLL。
    //!     如果客户端应用程序显式链接到 DLL，对 LoadLibrary 的调用将不会成功加载此 DLL，出现access violation (AV)错误。 
    //!     http://support.microsoft.com/kb/118816/en-us
    //!     通过 Dumpbin.exe 查看“Thread Storage Directory”是否为0，可以知道DLL是否有 静态全局线程局部变量。
    //!   怎么解决？用TLS+FileMap？
    //!   解决方式：使用TLS先解决KB118816的问题，由于在第一次时分配，最后一次时释放，因此需要在线程
    //!     开始的时候调用一次 FUNCTION_BLOCK_TRACE(0)，保证在线程结束前indent都大于0。
    class CFBlockElapse
    {
    public:
        static FTLINLINE BOOL Init();
        static FTLINLINE VOID UnInit();
        //使用毫秒作为判断是否超时
        FTLINLINE CFBlockElapse(LPCTSTR pszFileName,DWORD line, 
            LPCTSTR pBlockName, LPVOID pReturnAddr, DWORD MinElapse = 0);
        FTLINLINE ~CFBlockElapse(void);
    private:
        struct BlockElapseInfo
        {
            LONG   indent;
            TCHAR  bufIndicate[MAX_TRACE_INDICATE_LEVEL + 1];//增加最后的NULL所占的空间
        };
        static DWORD  s_dwTLSIndex;
        const TCHAR* m_pszFileName;
        const TCHAR* m_pszBlkName;
        const LPVOID m_pReturnAdr;
        const DWORD  m_Line;
        const DWORD m_MinElapse;
        DWORD m_StartTime;
    };
#endif //FTL_DEBUG

} //namespace FTL

#endif// FTL_BASE_H

#ifndef USE_EXPORT
#  include "ftlbase.hpp"
#endif
