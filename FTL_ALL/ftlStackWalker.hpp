#ifndef FTL_STACK_WALKER_HPP
#define FTL_STACK_WALKER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlStackWalker.h"
#endif

namespace FTL
{
    CFStackWalker::CFStackWalker(int Options/* = OptionDefualt*/,
        LPCTSTR pszSymPath/* = NULL*/,
        DWORD dwProcessId/* = GetCurrentProcessId()*/,
        HANDLE hProcess/* = GetCurrentProcess()*/)
    {
        m_bModulesLoaded = FALSE;
        m_bSymEngInit = FALSE;
        m_nMaxCallStacksNumber = 20;    //缺省
        m_Options = Options;
        m_dwProcessId = dwProcessId;
        m_hProcess = hProcess; //OpenProcess (PROCESS_ALL_ACCESS,FALSE,dwProcessId);// 

        if (NULL == pszSymPath)
        {
            //m_pszSymPath = new TCHAR[1];
            //m_pszSymPath[0] = NULL;
            m_pszSymPath = NULL;
        }
        else
        {
            size_t len = _tcslen(pszSymPath);
            m_pszSymPath = new TCHAR[len + 1];
            StringCchCopy(m_pszSymPath,len + 1,pszSymPath);
        }
    }

    CFStackWalker::~CFStackWalker()
    {
        ClearCallStack();
        CleanSymEng();
        SAFE_DELETE_ARRAY(m_pszSymPath);
    }

    BOOL CFStackWalker::InitSymEng()
    {
        BOOL bRet = TRUE;
        if (FALSE == m_bSymEngInit)
        {
            CFStringFormater strFormater(1024);
            if (m_pszSymPath != NULL && m_pszSymPath[0] != 0)
            {
                strFormater.AppendFormat(TEXT("%s;"),m_pszSymPath);
            }

            if ( (m_Options & SymBuildPath) != 0)
            {
                strFormater.AppendFormat(_T(".;"));

                // Now first add the (optional) provided sympath:
                TCHAR szTemp[1024] = {0};
                // Now add the current directory:
                if (GetCurrentDirectory(_countof(szTemp), szTemp) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(_T("%s;"),szTemp);
                }

                // Now add the path for the main-module:
                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetModuleFileName(NULL, szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    for (LPTSTR p = (szTemp+_tcslen(szTemp)-1); p >= szTemp; --p)
                    {
                        // locate the rightmost path separator
                        if ( (*p == _T('\\')) || (*p == _T('/')) || (*p == _T(':')) )
                        {
                            *p = 0;
                            break;
                        }
                    }  // for (search for path separator...)
                    if (_tcslen(szTemp) > 0)
                    {
                        strFormater.AppendFormat(TEXT("%s;"),szTemp);
                    }
                }

                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetEnvironmentVariable(_T("_NT_SYMBOL_PATH"), szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(TEXT("%s;"),szTemp);
                }

                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetEnvironmentVariable(_T("_NT_ALTERNATE_SYMBOL_PATH"), szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(TEXT("%s;"),szTemp);
                }

                ZeroMemory(szTemp,_countof(szTemp) * sizeof(TCHAR));
                if (GetEnvironmentVariable(_T("SYSTEMROOT"), szTemp, _countof(szTemp)) > 0)
                {
                    szTemp[_countof(szTemp)-1] = 0;
                    strFormater.AppendFormat(TEXT("%s;%s\\System32;"),szTemp,szTemp);
                }
            }

            USES_CONVERSION;

            DWORD symOptions = SymGetOptions();
            symOptions |= SYMOPT_LOAD_LINES;
            //symOptions |= SYMOPT_FAIL_CRITICAL_ERRORS;
            //symOptions |= SYMOPT_NO_PROMPTS;
            symOptions = SymSetOptions(symOptions);

            API_VERIFY(SymInitialize(m_hProcess,T2A((LPTSTR)(LPCTSTR)strFormater),TRUE));
            m_bSymEngInit = bRet;
        }
        return bRet;
    }

    BOOL CFStackWalker::CleanSymEng()
    {
        BOOL bRet = TRUE;
        if (m_bSymEngInit)
        {
            API_VERIFY(SymCleanup(m_hProcess));
            m_bSymEngInit = FALSE;
        }
        return bRet;
    }

    BOOL CFStackWalker::GetModuleListByToolHelp32(HANDLE hProcess, DWORD pid)
    {
        BOOL bRet = FALSE;
        HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pid);
        API_VERIFY(INVALID_HANDLE_VALUE != hSnap);
        if (INVALID_HANDLE_VALUE == hSnap )
        {
            return FALSE;
        }

        MODULEENTRY32 me32 = {0};
        me32.dwSize = sizeof(me32);
        API_VERIFY(Module32First( hSnap, &me32));
        int cnt = 0;
        while (bRet)
        {
            LoadModuleInfo(hProcess, me32.szExePath, me32.szModule, (DWORD)(DWORD_PTR) me32.modBaseAddr, me32.modBaseSize);
            cnt++;
            API_VERIFY_EXCEPT1(Module32Next(hSnap, &me32),ERROR_NO_MORE_FILES);
        }
        SAFE_CLOSE_HANDLE(hSnap,NULL);
        return cnt > 0;
    }

    BOOL CFStackWalker::GetModuleListByPSAPI(HANDLE hProcess)
    {
        BOOL bRet = FALSE;
        HMODULE *pModules = NULL;
        DWORD cbNeeded = 0;
        MODULEINFO mi = {0};
        DWORD ModuleCount = 0; 
        API_VERIFY(EnumProcessModules(hProcess, NULL, 0, &cbNeeded));
        if (bRet)
        {
            DWORD cnt = cbNeeded / sizeof(HMODULE);
            pModules = new HMODULE[cnt];
            API_VERIFY(EnumProcessModules(hProcess, pModules, cbNeeded, &cbNeeded));
            if (bRet)
            {
                for (DWORD i = 0; i < cnt; i++ )
                {
                    API_VERIFY(GetModuleInformation(hProcess, pModules[i], &mi, sizeof(mi)));
                    TCHAR moduleFileName[MAX_PATH] = {0};
                    API_VERIFY(GetModuleFileNameEx(hProcess, pModules[i],moduleFileName , _countof(moduleFileName)) > 0);
                    TCHAR moduleBaseName[MAX_PATH] = {0};
                    API_VERIFY(GetModuleBaseName(hProcess, pModules[i], moduleBaseName, _countof(moduleBaseName)) > 0 );

                    API_VERIFY(LoadModuleInfo(hProcess,moduleFileName , moduleBaseName, (DWORD)(DWORD_PTR) mi.lpBaseOfDll, mi.SizeOfImage));
                    ModuleCount++;
                }
            }
            SAFE_DELETE_ARRAY(pModules);
        }
        return ModuleCount != 0;
    }

    BOOL CFStackWalker::LoadModuleInfo(HANDLE hProcess, LPCTSTR img, LPCTSTR mod, DWORD baseAddr, DWORD size)
    {
        BOOL bRet = FALSE;
        USES_CONVERSION;
        DWORD64 dwResult = SymLoadModule(hProcess, 0, T2A((LPTSTR)img), T2A((LPTSTR)mod), baseAddr, size);
#pragma TODO(这里为什么会失败 ？ Error=2(ERROR_FILE_NOT_FOUND)
        UNREFERENCED_PARAMETER(dwResult);
        //API_VERIFY(dwResult != 0);
#if 0
        ULONGLONG fileVersion = 0;
        if ( (m_Options & RetrieveFileVersion) != 0)
        {
            VS_FIXEDFILEINFO *fInfo = NULL;
            DWORD dwHandle = NULL;
            DWORD dwSize = GetFileVersionInfoSize(img, &dwHandle);
            if (dwSize > 0)
            {
                LPBYTE vData = new BYTE[dwSize];
                if (vData != NULL)
                {
                    if (GetFileVersionInfo(img, dwHandle, dwSize, (LPVOID)vData) != 0)
                    {
                        UINT len;
                        TCHAR szSubBlock[] = _T("\\");
                        if (VerQueryValue(vData, szSubBlock, (LPVOID*) &fInfo, &len) == 0)
                        {
                            fInfo = NULL;
                        }
                        else
                        {
                            fileVersion = ((ULONGLONG)fInfo->dwFileVersionLS) + ((ULONGLONG)fInfo->dwFileVersionMS << 32);
                        }
                    }
                    SAFE_DELETE_ARRAY(vData);
                }
            }
        }
#endif 

        // Retrive some additional-infos about the module
        IMAGEHLP_MODULE  Module = {0};
        Module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);
        TCHAR szSymType[20] = {0};
        API_VERIFY(SymGetModuleInfo(hProcess, baseAddr, &Module));
        if(bRet)
        {
            switch(Module.SymType)
            {
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymNone);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymCoff);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymCv);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymPdb);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymExport);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymDeferred);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymSym);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymDia);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), SymVirtual);
                HANDLE_CASE_TO_STRING(szSymType, _countof(szSymType), NumSymTypes);
            default:
                _ASSERT(FALSE);
                break;
            }
        }
        return bRet;
    }

    BOOL __stdcall CFStackWalker::myReadProcessMemoryProc(
        HANDLE      hProcess,
        DWORD64     qwBaseAddress,
        PVOID       lpBuffer,
        DWORD       nSize,
        LPDWORD     lpNumberOfBytesRead
        )
    {
        SIZE_T st = 0;
        BOOL bRet = ReadProcessMemory(hProcess, (LPCVOID) qwBaseAddress, lpBuffer, nSize, &st);
        *lpNumberOfBytesRead = (DWORD) st;
        return bRet;
    }


    BOOL CFStackWalker::GetCallStackArray(HANDLE hThread, const CONTEXT *pContent)
    {
        FTLASSERT(pContent);
        BOOL bRet = FALSE;
        m_Context = *pContent;

        IMAGEHLP_SYMBOL64 *pSym = NULL;
        IMAGEHLP_MODULE64 Module = {0};
        IMAGEHLP_LINE64 Line = {0};
        int frameNum = 0;

        API_VERIFY(InitSymEng());
        if (FALSE == GetModuleListByToolHelp32(m_hProcess,m_dwProcessId))
        {   
            API_VERIFY(GetModuleListByPSAPI(m_hProcess));
        }

        // init STACKFRAME for first call
        STACKFRAME64 sf = {0}; // in/out stack frame
        DWORD imageType = 0;
#ifdef _M_IX86  //_X86_
        // normally, call ImageNtHeader() and use machine info from PE header
        imageType = IMAGE_FILE_MACHINE_I386;
        sf.AddrPC.Offset = m_Context.Eip;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = m_Context.Ebp;
        sf.AddrFrame.Mode = AddrModeFlat;
        sf.AddrStack.Offset = m_Context.Esp;
        sf.AddrStack.Mode = AddrModeFlat;
#elif _M_X64  //_AMD64_
        imageType = IMAGE_FILE_MACHINE_AMD64;
        sf.AddrPC.Offset = m_Context.Rip;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = m_Context.Rbp;
        sf.AddrFrame.Mode = AddrModeFlat;
        sf.AddrStack.Offset = m_Context.Rsp;
        sf.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64  //_IA64_
        imageType = IMAGE_FILE_MACHINE_IA64;
        sf.AddrPC.Offset = m_Context.StIIP;
        sf.AddrPC.Mode = AddrModeFlat;
        sf.AddrFrame.Offset = m_Context.IntSp;
        sf.AddrFrame.Mode = AddrModeFlat;
        sf.AddrBStore.Offset = m_Context.RsBSP;
        sf.AddrBStore.Mode = AddrModeFlat;
        sf.AddrStack.Offset = m_Context.IntSp;
        sf.AddrStack.Mode = AddrModeFlat;
#else
#  error "Platform not supported!"
#endif
        pSym = (IMAGEHLP_SYMBOL64 *) new BYTE[sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN];
        _ASSERT(pSym);
        if (pSym)
        {
            ZeroMemory(pSym,sizeof(IMAGEHLP_SYMBOL64) + STACKWALK_MAX_NAMELEN);
            pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            pSym->MaxNameLength = STACKWALK_MAX_NAMELEN;

            ZeroMemory(&Line, sizeof(Line));
            Line.SizeOfStruct = sizeof(Line);

            ZeroMemory(&Module, sizeof(Module));
            Module.SizeOfStruct = sizeof(Module);

            for (frameNum = 0; ; ++frameNum )
            {
                // get next stack frame (StackWalk64(), SymFunctionTableAccess64(), SymGetModuleBase64())
                // if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
                // assume that either you are done, or that the stack is so hosed that the next
                // deeper frame could not be found.
                // CONTEXT need not to be supplied if imageTyp is IMAGE_FILE_MACHINE_I386!
                API_VERIFY(StackWalk64(imageType, m_hProcess, hThread, &sf, &m_Context, 
                    &CFStackWalker::myReadProcessMemoryProc, SymFunctionTableAccess64, SymGetModuleBase64, NULL));

                //DWORD64 dwModBase = SymGetModuleBase64 ( m_hProcess ,
                //	sf.AddrPC.Offset  ) ;
                //if ( 0 == dwModBase )
                //{
                //	OnDbgHelpErr(_T("SymGetModuleBase64"), GetLastError(), sf.AddrPC.Offset);
                //	break;
                //}

                if (sf.AddrPC.Offset == sf.AddrReturn.Offset)
                {
                    FTLASSERT(FALSE);
                    //OnDbgHelpErr(_T("StackWalk64-Endless-Callstack!"), 0, sf.AddrPC.Offset);
                    break;
                }

                CallStackEntry csEntry = {0};
                csEntry.offset = sf.AddrPC.Offset;
                csEntry.SegCs = m_Context.SegCs;

                if (sf.AddrPC.Offset != 0)
                {
                    // we seem to have a valid PC
                    // show procedure info (SymGetSymFromAddr64())
                    pSym->Address = sf.AddrPC.Offset;
                    if(SymGetSymFromAddr64(m_hProcess, sf.AddrPC.Offset, &(csEntry.offsetFromSmybol), pSym))
                    {
                        // TODO: Mache dies sicher...!
                        USES_CONVERSION;
                        StringCchCopy(csEntry.name,_countof(csEntry.name),A2T(pSym->Name));
                        CHAR undName[STACKWALK_MAX_NAMELEN] = {0};
                        CHAR undFullName[STACKWALK_MAX_NAMELEN] = {0};
                        // UnDecorateSymbolName()
                        UnDecorateSymbolName( pSym->Name, undName, STACKWALK_MAX_NAMELEN, UNDNAME_NAME_ONLY );
                        UnDecorateSymbolName( pSym->Name, undFullName, STACKWALK_MAX_NAMELEN, UNDNAME_COMPLETE );
                        StringCchCopy(csEntry.undName,_countof(csEntry.undName), A2T(undName));
                        StringCchCopy(csEntry.undFullName,_countof(csEntry.undFullName),A2T(undFullName));
                    }

                    // show line number info, NT5.0-method (SymGetLineFromAddr64())
                    if (RetrieveLine == (RetrieveLine & m_Options))
                    { // yes, we have SymGetLineFromAddr64()
                        if(SymGetLineFromAddr64(m_hProcess, sf.AddrPC.Offset, &(csEntry.offsetFromLine), &Line))
                        {   //ERROR_INVALID_ADDRESS
                            csEntry.lineNumber = Line.LineNumber;
                            USES_CONVERSION;
                            StringCchCopy(csEntry.lineFileName,_countof(csEntry.lineFileName), A2T(Line.FileName));
                        }
                    } // yes, we have SymGetLineFromAddr64()


                    // show module info (SymGetModuleInfo64())

                    ZeroMemory(&Module,sizeof(IMAGEHLP_MODULE64));
                    Module.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
                    if(SymGetModuleInfo64(m_hProcess,sf.AddrPC.Offset,&Module))
                    { // got module info OK
                        csEntry.symType = Module.SymType;
                        switch ( Module.SymType )
                        {
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymNone);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymCoff);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymCv);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymPdb);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymExport);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymDeferred);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymSym);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymDia);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),SymVirtual);
                            HANDLE_CASE_TO_STRING(csEntry.symTypeString,_countof(csEntry.symTypeString),NumSymTypes);
                        default:
                            _ASSERT(FALSE);
                            break;
                        }

                        USES_CONVERSION;
                        StringCchCopy(csEntry.moduleName,_countof(csEntry.moduleName), A2T(Module.ModuleName));
                        csEntry.baseOfImage = Module.BaseOfImage;
                        StringCchCopy(csEntry.loadedImageName,_countof(csEntry.loadedImageName), A2T(Module.LoadedImageName));
                    } // got module info OK
                    //else
                    //{
                    //    OnDbgHelpErr(_T("SymGetModuleInfo64"), GetLastError(), sf.AddrPC.Offset);
                    //}

                    csEntry.Params[0] = sf.Params[0];
                    csEntry.Params[1] = sf.Params[1];
                    csEntry.Params[2] = sf.Params[2];
                    csEntry.Params[3] = sf.Params[3];

                } // we seem to have a valid PC
                CallStackEntryType et = nextEntry;
                if (frameNum == 0)
                    et = firstEntry;
                AddCallStackEntry(et,csEntry);

                if (sf.AddrReturn.Offset == 0)
                {
                    AddCallStackEntry(lastEntry,csEntry);
                    SetLastError(ERROR_SUCCESS);
                    break;
                }

                if (GetStackTraceNum() >= m_nMaxCallStacksNumber)
                {
                    break;
                }
            } // for ( frameNum )
            SAFE_DELETE_ARRAY(pSym);
            //CString strResultFormater;

            //CFStringFormater strResultFormater(1024);
            //for (size_t i = 0; i < m_CallStatcks.size(); i++)
            //{
            //    strResultFormater.AppendFormat(TEXT("%s\n"),m_CallStatcks[i]);
            //}
            //MessageBox(NULL,strResultFormater,NULL,MB_OK);
        }
        return TRUE;
    }

    INT CFStackWalker::GetStackTraceNum() const
    {
        return static_cast<INT>(m_CallStatcks.size());
    }

    LPCTSTR CFStackWalker::GetStackTraceStringByIndex(INT index) const
    {
        return m_CallStatcks[index];
    }


    void CFStackWalker::AddCallStackEntry(CallStackEntryType eType, CallStackEntry &entry)
    {
        if (firstEntry == eType)
        {
            ClearCallStack();
        }
        HRESULT hr = E_FAIL;
        CFStringFormater    strFormater(STACKWALK_MAX_NAMELEN);
        if ((eType != lastEntry) && (entry.offset != 0) )
        {
            if (entry.name[0] == 0)
            {
                COM_VERIFY(StringCchCopy(entry.name,_countof(entry.name), _T("(function-name not available)")));
            }
            if (entry.undName[0] != 0)
            {
                COM_VERIFY(StringCchCopy(entry.name, _countof(entry.name), entry.undName));
            }
            if (entry.undFullName[0] != 0)
            {
                COM_VERIFY(StringCchCopy(entry.name,_countof(entry.name),entry.undFullName));
            }
            if (entry.lineFileName[0] == 0)
            {
                COM_VERIFY(StringCchCopy(entry.lineFileName,_countof(entry.lineFileName), _T("(filename not available)")));
                if (entry.moduleName[0] == 0)
                {
                    COM_VERIFY(StringCchCopy(entry.moduleName,_countof(entry.moduleName),_T("(module-name not available)")));
                }
                strFormater.Format(_T("%s (Addr:%p): %s!%s"),
                    entry.lineFileName,(LPVOID) entry.offset, entry.loadedImageName, entry.name);
            }
            else
            {
                strFormater.Format(_T("%s (%d): %s!%s"), 
                    entry.lineFileName, entry.lineNumber,entry.loadedImageName, entry.name);
            }
            m_CallStatcks.push_back(strFormater.Detach());
        }
    }

    void CFStackWalker::ClearCallStack()
    {
        for (CallStackIterator iter = m_CallStatcks.begin();
            iter != m_CallStatcks.end(); ++iter)
        {
            delete [](*iter);
        }
        m_CallStatcks.clear();
    }

}
#endif //FTL_STACK_WALKER_HPP