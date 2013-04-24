#ifndef FTL_MEM_HPP
#define FTL_MEM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlMem.h"
#endif

#include <TlHelp32.h>
#include "ftlSystem.h"

namespace FTL
{
    CFMemLeakChecker::CFMemLeakChecker(LPCTSTR pszFileName,DWORD line, LPCTSTR pBlockName, BOOL bPauseOtherThread)
        :m_pszFileName(pszFileName)
        ,m_Line(line)
        ,m_pszBlkName(pBlockName)
        ,m_bPauseOtherThread(bPauseOtherThread)
        ,m_sOld()
        ,m_sNew()
        ,m_sDiff()
    {
        if (m_bPauseOtherThread)
        {
            CFSystemUtil::SuspendProcess(GetCurrentProcessId(),TRUE);
        }
        _CrtMemCheckpoint(&m_sOld);
    }

    CFMemLeakChecker::~CFMemLeakChecker()
    {
        _CrtMemCheckpoint(&m_sNew);
        if (m_bPauseOtherThread)
        {
            CFSystemUtil::SuspendProcess(GetCurrentProcessId(),FALSE);
        }
        if (_CrtMemDifference(&m_sDiff, &m_sOld, &m_sNew))
        {
            FAST_TRACE_EX(tlWarning, TEXT("%s(%d) :\t TID(0x%04x) \"%s\" Maybe Have Memory Leak\n"),
                m_pszFileName,m_Line,GetCurrentThreadId(), m_pszBlkName);
            _CrtMemDumpStatistics(&m_sDiff);
        }
    }


    template <typename T>
    CFMemCheckBase<T>::CFMemCheckBase()
    {
        InterlockedIncrement(&s_Count);
        if (s_Trace)
        {
            FTLTRACE(TEXT("Construct %d %s \n"), s_Count, CA2T(typeid(T).name()));
        }
    }

    template <typename T>
    CFMemCheckBase<T>::CFMemCheckBase(const CFMemCheckBase& other)
    {
        *this = other;
        InterlockedIncrement(&s_Count);
    }

    template <typename T>
    CFMemCheckBase<T>::~CFMemCheckBase()
    {
        if (s_Trace)
        {
            FTLTRACE(TEXT("Destruct %d %s \n"), s_Count, CA2T(typeid(T).name()));
        }
        InterlockedDecrement(&s_Count);
    }

    template <typename T>
    void CFMemCheckBase<T>::SetTrace(BOOL bTrace)
    {
        s_Trace = bTrace;
    }

    template <typename T>
    BOOL CFMemCheckBase<T>::GetTrace()
    {
        return s_Trace;
    }

    template <typename T>
    LONG CFMemCheckBase<T>::s_Count = 0;

    template <typename T>
    BOOL CFMemCheckBase<T>::s_Trace = TRUE;
}

#endif //FTL_MEM_HPP