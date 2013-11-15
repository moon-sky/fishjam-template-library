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
    template < typename T >
    CFMemoryPoolT<T>::CFMemoryPoolT( INT nInitSize /* = 0  */, INT nMaxSize /* = INT_MAX */ )
    {
        FTLASSERT( 0 <= nInitSize );
        FTLASSERT( nInitSize <= nMaxSize );

        m_nCurCount = 0;
        m_nMaxSize = nMaxSize;
        m_nNextIndex = 0;

        for (int i = 0; i < nInitSize; i++) 
        {
            CFMemoryObject *pObj = new CFMemoryObject();
            FTLASSERT(NULL != pObj);
            if (NULL != pObj) 
            {
                m_nCurCount++;
                pObj->index = _GetNextIndex();
                m_freeBlocks.insert( pObj );
                m_allBlocks.insert( pObj );
            }
        }
    }

    template < typename T >
    CFMemoryPoolT<T>::~CFMemoryPoolT()
    {
        CFAutoLock<CFLockObject> lock(&m_lockObject);

        FTLTRACE(TEXT("~CFMemoryPoolT, allBlocksSize=%d, freeBlocksSize=%d\n"),
            m_allBlocks.size(), m_freeBlocks.size());
        FTLASSERT(m_allBlocks.size() == m_freeBlocks.size() 
            && TEXT("Must Release all object back to pool before destructor"));
        FTLASSERT(m_nCurCount == m_allBlocks.size());

        for ( ObjectContainer::iterator iter = m_allBlocks.begin();
            iter != m_allBlocks.end(); ++iter) 
        {
            CFMemoryObject* pObj = *iter;
            delete pObj;
            m_nCurCount--;
        }
        
        m_allBlocks.clear();
        m_freeBlocks.clear();
    }

    template < typename T >
    T* CFMemoryPoolT<T>::Get()
    {
        CFMemoryObject* pObj = NULL;
        CFAutoLock<CFLockObject> lock(&m_lockObject);
        if (!m_freeBlocks.empty())
        {   
            pObj = *m_freeBlocks.begin();
            m_freeBlocks.erase(m_freeBlocks.begin());
        }

        if (NULL == pObj && (m_nCurCount < m_nMaxSize))
        {
            pObj = new CFMemoryObject;
            FTLASSERT(NULL != pObj);
            if (NULL != pObj)
            {
                pObj->index = _GetNextIndex();
                m_nCurCount++;
                m_allBlocks.insert(pObj);
            }
        }
        if (pObj)
        {
            return &(pObj->obj);
        }
        return NULL;
        
    }

    template < typename T >
    void CFMemoryPoolT<T>::Retrun( T* t )
    {
        FTLASSERT(NULL != t);
        if (NULL != t)
        {
            CFAutoLock<CFLockObject> lock(&m_lockObject);
            CFMemoryObject* pObj = CONTAINING_RECORD(t, CFMemoryObject, obj);
            FTLASSERT(pObj);
            if (pObj)
            {
                m_freeBlocks.insert(pObj);
            }
        }
    }
    template < typename T >
    INT CFMemoryPoolT<T>::Free(INT nCount /* = -1 */)
    {
        INT nReleaseCount = 0;

        CFAutoLock<CFLockObject> lock(&m_lockObject);
        if (-1 == nCount )
        {
            nCount = (INT)m_freeBlocks.size();
        }
        else
        {
            nCount = FTL_MIN(nCount, m_freeBlocks.size());
        }
        //BOOL isFreeAndAllCountSame = (m_freeBlocks.size() == m_allBlocks.size());

        ObjectContainer::reverse_iterator iter = m_freeBlocks.rbegin();
        while(iter != m_freeBlocks.rend())
        {
            if (nReleaseCount >= nCount)
            {
                //nCount 可以是0, 所以进入循环即判断
                break;
            }

            CFMemoryObject* pObj = *iter;
            ObjectContainer::iterator iterInAll = m_allBlocks.find(pObj);
            FTLASSERT(iterInAll != m_allBlocks.end());
            if (iterInAll != m_allBlocks.end())
            {
                m_allBlocks.erase(iterInAll);
            }

            //m_freeBlocks.erase(iter++);
            m_freeBlocks.erase(--iter.base());
            delete pObj;

            m_nCurCount--;
            nReleaseCount++;
        }
        return nReleaseCount;
    }

    template<typename T>
    CFMemoryPoolObjectHelper<T>::CFMemoryPoolObjectHelper(MemoryPoolType* pMemPool)
    {
        m_pMemPool = pMemPool;
        pObject = pMemPool->Get();
    }
    
    template<typename T>
    CFMemoryPoolObjectHelper<T>::~CFMemoryPoolObjectHelper()
    {
        if (pObject)
        {
            m_pMemPool->Retrun(pObject);
        }
    }

    //////////////////////////////////////////////////////////////////////////

    //100%，全部清除
#define MAX_CLEAR_PERCENT       (100)

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    CFItemCacheT<KEY,VALUE,CHILDCLASS>::CFItemCacheT(DWORD nMaxCacheSize,DWORD clearPercent /* = 30 */)
    {
        Init(nMaxCacheSize,clearPercent);
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    void CFItemCacheT<KEY,VALUE,CHILDCLASS>::Init(DWORD nMaxCacheSize,DWORD clearPercent)
    {
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        m_MaxCacheSize = nMaxCacheSize;
        m_ClearPercent = clearPercent;
        m_ClearMode = clearByLRU;			
        m_CurrentIndex = 0;
    }

    //template<typename KEY, typename VALUE,typename CHILDCLASS>
    //void CFItemCacheT<KEY,VALUE,CHILDCLASS>::SetMaxCacheSize(DWORD nMaxCacheSize)
    //{
    //    CFAutoLock<CFLockObject> locker(&m_LockObject);
    //    if (m_MaxCacheSize != nMaxCacheSize)
    //    {
    //        if (m_MaxCacheSize > nMaxCacheSize)  //smaller(will clear)
    //        {
    //            m_MaxCacheSize = nMaxCacheSize;
    //            //clear some out of range data
    //        }
    //        else
    //        {
    //            m_MaxCacheSize = nMaxCacheSize;
    //        }

    //    }
    //    m_MaxCacheSize = nMaxCacheSize;
    //    m_CurrentIndex = 0;
    //    m_ClearMode = clearByLRU;			
    //}


    template<typename KEY, typename VALUE,typename CHILDCLASS>
    CFItemCacheT<KEY,VALUE,CHILDCLASS>::~CFItemCacheT()
    {
        ClearCache(MAX_CLEAR_PERCENT); //// 析构时必须调用 ClearCache 函数，否则可能内存泄漏等问题。
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    BOOL CFItemCacheT<KEY,VALUE,CHILDCLASS>::GetItemByKey(const KEY key,VALUE& value)
    {
        BOOL bRet = FALSE;
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        CACHE_MAP::iterator iter = m_ItemCacheMaps.find(key);
        if (iter != m_ItemCacheMaps.end()) // find
        {
            value =  iter->second->value;
            API_VERIFY(::QueryPerformanceCounter(&iter->second->lastAccess));
            return TRUE;
        }

        // not find ,will get RealItem and insert to Item cache
        if (m_ItemCacheMaps.size() >= m_MaxCacheSize)
        {
            ClearCache(m_ClearPercent);
        }

        BOOL bGot = FALSE;
        {
            CHILDCLASS* pThis = static_cast<CHILDCLASS*>(this);
            KEY lookupKey = key;
            iter = m_ItemCacheMaps.find(lookupKey);
            if (m_ItemCacheMaps.end() == iter) //if not found, then GetRealItem
            {
                bGot = pThis->GetRealItem(lookupKey,value);
                if (bGot)
                {
                    CItemData *pNewItem = new CItemData;
                    pNewItem->key = lookupKey;
                    pNewItem->value = value;
                    API_VERIFY(::QueryPerformanceCounter(&pNewItem->lastAccess));
                    pNewItem->index = m_CurrentIndex++;
                    m_ItemCacheMaps.insert(CACHE_MAP::value_type(lookupKey,pNewItem));
                }
            }
        }
        return bGot;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    void CFItemCacheT<KEY,VALUE,CHILDCLASS>::SetClearMode(ClearMode mode)
    {
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        m_ClearMode = mode;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::LastAccessGreater(CItemData* p, CItemData* q)
    {
        return p->lastAccess.QuadPart > q->lastAccess.QuadPart;
        //return p->lastAccess > q->lastAccess;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::LastAccessSmaller(CItemData* p, CItemData* q)
    {
        return p->lastAccess.QuadPart < q->lastAccess.QuadPart;
        //return p->lastAccess < q->lastAccess;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::IndexGreater(CItemData* p, CItemData* q)
    {
        return p->index > q->index;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::IndexSmaller(CItemData* p, CItemData* q)
    {
        return p->index < q->index;
    }


    template<typename KEY, typename VALUE,typename CHILDCLASS>
    void CFItemCacheT<KEY,VALUE,CHILDCLASS>::ClearCache(DWORD percent)
    {
        //TODO: 使用 Effective STL 中看到的方法优化
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        CHILDCLASS* pThis = static_cast<CHILDCLASS*>(this);
        CACHE_MAP::iterator iterMap;
        if (MAX_CLEAR_PERCENT == percent)  //如果是全部清除的话(通常是析构中)
        {
            for (iterMap = m_ItemCacheMaps.begin();iterMap != m_ItemCacheMaps.end();iterMap++)
            {
                CItemData* pTempItemData = iterMap->second;
                _ASSERT(pTempItemData!= NULL);
                pThis->FreeItem(pTempItemData->key,pTempItemData->value);
                delete pTempItemData;
                pTempItemData = NULL;
            }
            m_ItemCacheMaps.clear();
            return;
        }
        DWORD dwClearCount = m_MaxCacheSize * percent / MAX_CLEAR_PERCENT;

        if(m_ClearMode != clearByRandom)
        {
            std::vector<CItemData*> valVector;
            for (iterMap = m_ItemCacheMaps.begin();iterMap != m_ItemCacheMaps.end(); iterMap++)
            {
                valVector.push_back(iterMap->second);
            }

            switch(m_ClearMode)
            {
            case clearByLRU:
                sort(valVector.begin(),valVector.end(),LastAccessSmaller); // time up
                break;
            case clearByFIFO:
                sort(valVector.begin(),valVector.end(),IndexSmaller); //index up
                break;
                //case clearByMRU:
                //	break;
            default:
                _ASSERT(FALSE);
                break;
            }

            m_ItemCacheMaps.clear();

            for (DWORD dwIndex = 0; dwIndex < dwClearCount; dwIndex++)
            {
                CItemData* pTempItemData = valVector.at(dwIndex);
                _ASSERT(pTempItemData!= NULL);
                pThis->FreeItem(pTempItemData->key,pTempItemData->value);
                delete pTempItemData;
                pTempItemData = NULL;
            }
            for (DWORD dwIndex = dwClearCount; dwIndex < valVector.size(); dwIndex++)
            {
                CItemData *pTempItemData = valVector.at(dwIndex);
                KEY key = pTempItemData->key;
                m_ItemCacheMaps.insert(CACHE_MAP::value_type(key,pTempItemData));
            }
            valVector.clear();
            //   partial_sort_copy(m_ItemCacheMaps.begin(),m_ItemCacheMaps.end(),testVector.begin(),testVector.end());
        }
        else  //随机清除，不排序
        {
            DWORD dwCount = 0;
            iterMap = m_ItemCacheMaps.begin();
            while (dwCount < dwClearCount)
            {
                CItemData* pTempItemData = iterMap->second;
                _ASSERT(pTempItemData!= NULL);
                pThis->FreeItem(pTempItemData->key,pTempItemData->value);
                delete pTempItemData;
                pTempItemData = NULL;
                iterMap = m_ItemCacheMaps.erase(iterMap);
                dwCount++;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
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

	CFMemCheckBase& CFMemCheckBase::operator =( const CFMemCheckBase& /*ref*/ )
	{
		//没有成员变量，do nothing
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