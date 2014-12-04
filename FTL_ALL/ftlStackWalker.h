#ifndef FTL_STACK_WALKER_H
#define FTL_STACK_WALKER_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlStackWalker.h requires ftlbase.h to be included first
#endif

#include <TlHelp32.h>
#include <DbgHelp.h>
#include <Psapi.h>
#pragma comment( lib, "DbgHelp.lib" )
#pragma comment( lib, "Psapi.lib" )

/*************************************************************************************************************************
*************************************************************************************************************************/

namespace FTL
{
    // The "ugly" assembler-implementation is needed for systems before XP
    // If you have a new PSDK and you only compile for XP and later, then you can use 
    // the "RtlCaptureContext"
    // Currently there is no define which determines the PSDK-Version... 
    // So we just use the compiler-version (and assumes that the PSDK is 
    // the one which was installed by the VS-IDE)

    // INFO: If you want, you can use the RtlCaptureContext if you only target XP and later...
    //       But I currently use it in x64/IA64 environments...
    //#if defined(_M_IX86) && (_WIN32_WINNT <= 0x0500) && (_MSC_VER < 1400)

#if defined(_M_IX86)
#ifdef CURRENT_THREAD_VIA_EXCEPTION
    // TODO: The following is not a "good" implementation, 
    // because the call stack is only valid in the "__except" block...
#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    EXCEPTION_POINTERS *pExp = NULL; \
    __try { \
    throw 0; \
} __except( ( (pExp = GetExceptionInformation()) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_EXECUTE_HANDLER)) {} \
    if (pExp != NULL) \
    memcpy(&c, pExp->ContextRecord, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    } while(0);
#else
    // The following should be enough for walking the call stack...
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
#endif
#else
    // The following is defined for x86 (XP and higher), x64 and IA64:
#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    memset(&c, 0, sizeof(CONTEXT)); \
    c.ContextFlags = contextFlags; \
    RtlCaptureContext(&c); \
    } while(0);
#endif

    // The IMAGEHLP_MODULE wrapper class
    struct CImageHlp_Module : public IMAGEHLP_MODULE64
    {
        CImageHlp_Module ( )
        {
            memset ( this , NULL , sizeof ( IMAGEHLP_MODULE64 ) ) ;
            SizeOfStruct = sizeof ( IMAGEHLP_MODULE64 ) ;
        }
    } ;

    // The IMAGEHLP_LINE wrapper class
    struct CImageHlp_Line : public IMAGEHLP_LINE64
    {
        CImageHlp_Line ( )
        {
            memset ( this , NULL , sizeof ( IMAGEHLP_LINE64 ) ) ;
            SizeOfStruct = sizeof ( IMAGEHLP_LINE64 ) ;
        }
    } ;

    class CFStackWalker
    {
        //注意：这个类不是线程安全的，需要的话，必须自己进行同步
    public:           
        typedef enum StackWalkOptions
        {
            RetrieveNone = 0,           //! No addition info will be retrieved,(only the address is available)
            RetrieveSymbol = 1,         //! Try to get the symbol-name
            RetrieveLine = 2,           //! Try to get the line for this symbol
            RetrieveModuleInfo = 4,     //! Try to retrieve the module-infos
            RetrieveParams = 8,         //! Try to retrieve the params info
            RetrieveFileVersion = 0x10, //! Also retrieve the version for the DLL/EXE
            RetrieveVerbose = 0x1F,     //! Contains all the above
            SymBuildPath = 0x100,       //! Generate a "good" symbol-search-path
            SymUseSymSrv = 0x200,       //! Also use the public Microsoft-Symbol-Server
            SymAll = 0x300,             //! Contains all the above "Sym"-options
            OptionsAll = 0xFFFF,        //! Contains all options
            OptionDefualt = OptionsAll  //! Default Option
        } StackWalkOptions;

        enum { STACKWALK_MAX_NAMELEN = 512 }; // max name length for found symbols


        // Entry for each CallStack-Entry
        typedef struct CallStackEntry
        {
            DWORD64 offset;  // if 0, we have no valid entry
            DWORD64 offsetFromSmybol;
            DWORD64 baseOfImage;
            DWORD64 Params[4];

            DWORD offsetFromLine;
            DWORD lineNumber;
            DWORD symType;
            DWORD SegCs;

            TCHAR name[STACKWALK_MAX_NAMELEN];
            TCHAR undName[STACKWALK_MAX_NAMELEN];
            TCHAR undFullName[STACKWALK_MAX_NAMELEN];
            TCHAR lineFileName[STACKWALK_MAX_NAMELEN];
            TCHAR moduleName[STACKWALK_MAX_NAMELEN];
            TCHAR loadedImageName[STACKWALK_MAX_NAMELEN];
            TCHAR symTypeString[STACKWALK_MAX_NAMELEN];
        } CallStackEntry;
        enum CallStackEntryType {firstEntry, nextEntry, lastEntry};

        //typedef BOOL (__stdcall *PReadProcessMemoryRoutine)(
        //    HANDLE      hProcess,
        //    DWORD64     qwBaseAddress,
        //    PVOID       lpBuffer,
        //    DWORD       nSize,
        //    LPDWORD     lpNumberOfBytesRead,
        //    LPVOID      pUserData  // optional data, which was passed in "ShowCallstack"
        //    );

        FTLINLINE CFStackWalker(int Options = OptionDefualt, LPCTSTR pszSymPath = NULL,
            DWORD dwProcessId = GetCurrentProcessId(),
            HANDLE hProcess = GetCurrentProcess());
        FTLINLINE virtual ~CFStackWalker();
        FTLINLINE void SetMaxCallStacksNum(INT nMaxNumber) { m_nMaxCallStacksNumber = nMaxNumber; }
        FTLINLINE BOOL GetCallStackArray(HANDLE hThread, const CONTEXT *pContent);
        FTLINLINE INT GetStackTraceNum() const;
        FTLINLINE LPCTSTR GetStackTraceStringByIndex(INT index) const;
    protected:
        FTLINLINE BOOL InitSymEng();
        FTLINLINE BOOL CleanSymEng();
        FTLINLINE BOOL GetModuleListByToolHelp32(HANDLE hProcess, DWORD pid);
        FTLINLINE BOOL GetModuleListByPSAPI(HANDLE hProcess);
        FTLINLINE BOOL LoadModuleInfo(HANDLE hProcess, LPCTSTR img, LPCTSTR mod, DWORD baseAddr, DWORD size);
        FTLINLINE static BOOL __stdcall myReadProcessMemoryProc(
            HANDLE      hProcess,
            DWORD64     qwBaseAddress,
            PVOID       lpBuffer,
            DWORD       nSize,
            LPDWORD     lpNumberOfBytesRead
            );
        FTLINLINE void AddCallStackEntry(CallStackEntryType eType, CallStackEntry &entry);
        FTLINLINE void ClearCallStack();
    protected:
        CONTEXT m_Context;
        BOOL    m_bModulesLoaded;
        BOOL    m_bSymEngInit;
        INT     m_nMaxCallStacksNumber;
        INT     m_Options;
        LPTSTR  m_pszSymPath;
        HANDLE  m_hProcess;
        DWORD   m_dwProcessId;
        typedef std::vector<LPTSTR>  CallStackArray;
        typedef CallStackArray::iterator    CallStackIterator;
        CallStackArray  m_CallStatcks;
    };

}
#endif //FTL_STACK_WALKER_H

#ifndef USE_EXPORT
#  include "ftlStackWalker.hpp"
#endif 