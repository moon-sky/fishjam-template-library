#include "StdAfx.h"
#include "SetupInfoMgr.h"

HookedSetupInfo::HookedSetupInfo()
{
    infoType = hsit_Invalid;
    pszInfo = NULL;
    nInfoLength = 0;
}
HookedSetupInfo::~HookedSetupInfo()
{
    if (nInfoLength)
    {
        SAFE_DELETE_ARRAY(pszInfo);
    }
}

bool HookedSetupInfo::operator < (const HookedSetupInfo & other) const
{
    FTLASSERT(hsit_Invalid != infoType && hsit_Invalid != other.infoType);
    FTLASSERT(pszInfo && other.pszInfo);

    COMPARE_MEM_LESS(infoType, other);
    if (pszInfo && other.pszInfo)
    {
        return ( lstrcmpi(pszInfo, other.pszInfo) < 0);
    }
    return false;
}

CSetupInfoMgr::CSetupInfoMgr(void)
{
}

CSetupInfoMgr::~CSetupInfoMgr(void)
{
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    m_HookedSetupInfos.clear();
}

BOOL CSetupInfoMgr::SetSetupInfo(HookedSetupInfoType infoType, LPCTSTR pszOldInfo, LPCTSTR pszNewInfo)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    
    HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
    pNewSetupInfo->infoType = infoType;
    pNewSetupInfo->nInfoLength = lstrlen(pszOldInfo);
    pNewSetupInfo->pszInfo = new TCHAR[pNewSetupInfo->nInfoLength + 1];
    lstrcpy(pNewSetupInfo->pszInfo, pszOldInfo);

    HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.find(pNewSetupInfo);
    if (iter == m_HookedSetupInfos.end())
    {
        //not found
        m_HookedSetupInfos.insert(pNewSetupInfo);
    }
    else
    {
        pNewSetupInfo = *iter;
        if (pszNewInfo && lstrcmpi(pszNewInfo, pNewSetupInfo->pszInfo) != 0)
        {
            SAFE_DELETE_ARRAY(pNewSetupInfo->pszInfo);
            pNewSetupInfo->nInfoLength = lstrlen(pszNewInfo);
            pNewSetupInfo->pszInfo = new TCHAR[pNewSetupInfo->nInfoLength + 1];
            lstrcpy(pNewSetupInfo->pszInfo, pszNewInfo);
        }
    }
    return bRet;
}

BOOL CSetupInfoMgr::DumpAllSetupInfo()
{
    BOOL bRet = TRUE;

    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    FTLTRACE(TEXT("DumpAllSetupInfo: %d\n"), m_HookedSetupInfos.size());

    int nIndex = 0;
    for(HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.begin();
        iter != m_HookedSetupInfos.end();
        ++iter)
    {
        const HookedSetupInfoPtr& pHookedSetupInfo = *iter;
        if (pHookedSetupInfo)
        {
            FTLTRACE(TEXT("\t[%d]%d - %s\n"), ++nIndex, pHookedSetupInfo->infoType, pHookedSetupInfo->pszInfo);
        }
    }
    return bRet;
}