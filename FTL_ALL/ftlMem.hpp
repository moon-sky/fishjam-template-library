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


    CFMemCheckBase::CFMemCheckBase()
    {
		CFMemCheckManager::GetInstance().AddObject((DWORD_PTR)this, __FILE__LINE__, CA2T(typeid(*this).name()));
    }

    CFMemCheckBase::CFMemCheckBase(const CFMemCheckBase& other)
    {
        *this = other;
		CFMemCheckManager::GetInstance().AddObject((DWORD_PTR)this, __FILE__LINE__, CA2T(typeid(*this).name()));
    }

	CFMemCheckBase& CFMemCheckBase::operator =( const CFMemCheckBase &ref )
	{
		//do nothing
		return *this;
	}

    CFMemCheckBase::~CFMemCheckBase()
    {
		CFMemCheckManager::GetInstance().RemoveObject((DWORD_PTR)this);
    }

    __declspec(selectany) CFMemCheckManager* CFMemCheckManager::s_pMemCheckMgr = NULL;
	CFMemCheckManager& CFMemCheckManager::GetInstance()
	{
		if (NULL == s_pMemCheckMgr)
		{
			s_pMemCheckMgr = new CFMemCheckManager();
			FTLASSERT(s_pMemCheckMgr);
		}
		return *s_pMemCheckMgr;
	}

	VOID CFMemCheckManager::ReleaseInstance()
	{
		if (s_pMemCheckMgr)
		{
			delete s_pMemCheckMgr;
			s_pMemCheckMgr = NULL;
		}
	}
	
	CFMemCheckManager::CFMemCheckManager()
	{
		m_bTrace = TRUE;
	}

	CFMemCheckManager::~CFMemCheckManager()
	{
		FTLASSERT(m_allObjects.empty()); //Memory leak
		DumpLeakInfo();
	}

	BOOL CFMemCheckManager::AddObject(DWORD_PTR pObject, LPCTSTR pszPosition, LPCTSTR pszName /* = NULL */)
	{
		BOOL bRet = FALSE;
		CFAutoLock<CFLockObject>	locker(&m_LockObj);
		ObjectPtrInfoContainer::iterator iter = m_allObjects.find(pObject);
		if (iter != m_allObjects.end())
		{
			//找到相同的地址，原因是:
			FTLASSERT(FALSE);
		}
		else
		{
			ObjectInfo* pInfo = new ObjectInfo;
			pInfo->m_strInfo.Format(TEXT("%s:%s"), pszPosition, pszName);
			m_allObjects[pObject] = pInfo;
			bRet = TRUE;
		}
		return bRet;
	}

	BOOL CFMemCheckManager::RemoveObject(DWORD_PTR pObject)
	{
		CFAutoLock<CFLockObject>	locker(&m_LockObj);
		BOOL bRet = FALSE;
		ObjectPtrInfoContainer::iterator iter = m_allObjects.find(pObject);
		if (iter != m_allObjects.end())
		{
			delete iter->second;
			m_allObjects.erase(iter);
			bRet = TRUE;
		}
		else
		{
			FTLASSERT(FALSE);
		}
		return bRet;
	}

	VOID CFMemCheckManager::DumpLeakInfo()
	{
		CFAutoLock<CFLockObject>	locker(&m_LockObj);
		FTLTRACEEX(tlWarning, TEXT("Memory Leak\n"));
		INT nIndex = 0;
		for (ObjectPtrInfoContainer::iterator iter = m_allObjects.begin();
			iter != m_allObjects.end(); 
			++iter)
		{
			FTLTRACEEX(tlWarning, TEXT("%d: %s\n"), ++nIndex, iter->second->m_strInfo.GetString());
		}
	}

    void CFMemCheckManager::SetTrace(BOOL bTrace)
    {
        m_bTrace = bTrace;
    }

    BOOL CFMemCheckManager::GetTrace()
    {
        return m_bTrace;
    }

	
}

#endif //FTL_MEM_HPP