#include "StdAfx.h"
#include "SetupInfoMgr.h"
#include "InfoConvUtil.h"

HookedSetupInfo::HookedSetupInfo()
{
    infoType = hsit_Invalid;
    pszPath = NULL;
    pszValue = NULL;
    //nInfoLength = 0;
}
HookedSetupInfo::~HookedSetupInfo()
{
    //if (nInfoLength)
    {
        SAFE_DELETE_ARRAY(pszPath);
        SAFE_DELETE_ARRAY(pszValue);
    }
}

bool HookedSetupInfo::operator < (const HookedSetupInfo & other) const
{
    FTLASSERT(hsit_Invalid != infoType && hsit_Invalid != other.infoType);
    FTLASSERT(pszPath && other.pszPath);

    COMPARE_MEM_LESS(infoType, other);
    if (pszPath && other.pszPath)
    {
        int nPathCmp = lstrcmpi(pszPath, other.pszPath);
        if (nPathCmp != 0)
        {
            return ( nPathCmp < 0);
        }
    }
    if (pszValue && other.pszValue)
    {
        int nValueCmp = lstrcmpi(pszValue, other.pszValue);
        if (nValueCmp != 0)
        {
            return (nValueCmp < 0);
        }
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

BOOL CSetupInfoMgr::SetSetupFileInfo(LPCTSTR pszOldInfo, LPCTSTR pszNewInfo)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    
    HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
    pNewSetupInfo->infoType = hsit_NewFile;
    int nInfoLength = lstrlen(pszOldInfo);
    pNewSetupInfo->pszPath = new TCHAR[nInfoLength + 1];
    lstrcpyn(pNewSetupInfo->pszPath, pszOldInfo, nInfoLength + 1);

    HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.find(pNewSetupInfo);
    if (iter == m_HookedSetupInfos.end())
    {
        //not found
        m_HookedSetupInfos.insert(pNewSetupInfo);
    }
    else
    {
        pNewSetupInfo = *iter;
        if (pszNewInfo && lstrcmpi(pszNewInfo, pNewSetupInfo->pszPath) != 0)
        {
            SAFE_DELETE_ARRAY(pNewSetupInfo->pszPath);
            int nInfoLength = lstrlen(pszNewInfo);
            pNewSetupInfo->pszPath = new TCHAR[nInfoLength + 1];
            lstrcpyn(pNewSetupInfo->pszPath, pszNewInfo, nInfoLength + 1);
        }
    }
    return bRet;
}

BOOL CSetupInfoMgr::DeleteSetupFileInfo(LPCTSTR pszFilePath)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);

    HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
    pNewSetupInfo->infoType = hsit_NewFile;
    int nInfoLength = lstrlen(pszFilePath);
    pNewSetupInfo->pszPath = new TCHAR[nInfoLength + 1];
    lstrcpy(pNewSetupInfo->pszPath, pszFilePath);
    
    HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.find(pNewSetupInfo);
    if (iter != m_HookedSetupInfos.end())
    {
        m_HookedSetupInfos.erase(iter);
    }
    return bRet;
}

BOOL CSetupInfoMgr::OpenSetupReg(HKEY hKeyRoot, HKEY hKey, LPCTSTR pszPath)
{
    //FTLASSERT(pszPath);

    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    CAtlString strFullPath;
    LPCTSTR pszRegRootKey = ConvertRegRootKeyToString(hKeyRoot);
    if (pszRegRootKey)
    {
        strFullPath = pszRegRootKey;
    }
    else
    {
        //Reopen a subReg
        RegKeyPathPairContainer::iterator iterReg = m_RegKeyPathPairs.find(hKeyRoot);
        if (iterReg != m_RegKeyPathPairs.end())
        {
            strFullPath = iterReg->second;
        }
        else
        {
            FTLTRACEEX(FTL::tlWarning, TEXT("Can not Find hKeyRoot 0x%x, hKey=0x%x, pszPath=%s\n"),
                hKeyRoot, hKey, pszPath);
            //FTLASSERT(FALSE);
        }
    }
    if (!strFullPath.IsEmpty() && strFullPath.GetAt(strFullPath.GetLength() - 1) != _T('\\'))
    {
        strFullPath.AppendFormat(TEXT("\\%s"), pszPath);
    }
    else
    {
        strFullPath.AppendFormat(TEXT("%s"), pszPath);
    }
    
    RegKeyPathPairContainer::iterator iterReg = m_RegKeyPathPairs.find(hKey);
    if (iterReg == m_RegKeyPathPairs.end())
    {
        m_RegKeyPathPairs[hKey] = strFullPath;
    }
    else
    {
        //maybe there are other close way ?
        FTLTRACEEX(tlWarning, TEXT("Warning in OpenSetupReg(hKeyRoot=0x%x), hKey(0x%x) will replace from \"%s\" to \"%s\"\n"),
            hKeyRoot, hKey, iterReg->second, strFullPath);

        iterReg->second = strFullPath;
        //FTLASSERT(iterReg->second == strFullPath);
    }
    return bRet;
}

BOOL CSetupInfoMgr::SetSetupRegInfo(HKEY hKey, LPCTSTR pszPath)
{
    BOOL bRet = TRUE;
    static TCHAR szEmpty[2] = {0};
    if (NULL == pszPath)
    {
        pszPath = szEmpty;  //tmp code for pszPath is NULL, other wise will copy rubbish
    }
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    RegKeyPathPairContainer::iterator iterReg = m_RegKeyPathPairs.find(hKey);
    //FTLASSERT(iterReg != m_RegKeyPathPairs.end());
    if (iterReg != m_RegKeyPathPairs.end())
    {
        const CString& strParentPath = iterReg->second;

        HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
        pNewSetupInfo->infoType = hsit_NewReg;

        int nPathLength = strParentPath.GetLength();// + 1 + lstrlen(pszPath);  // 1 is "\\"
        pNewSetupInfo->pszPath = new TCHAR[nPathLength + 1];
        lstrcpyn(pNewSetupInfo->pszPath, strParentPath, nPathLength + 1);

        int nValueLength = lstrlen(pszPath);
        pNewSetupInfo->pszValue = new TCHAR[nValueLength + 1];
        lstrcpyn(pNewSetupInfo->pszValue, pszPath, nValueLength + 1);

        //StringCchPrintf(pNewSetupInfo->pszPath, nPathLength + 1, TEXT("%s\\%s"), 
        //    strParentPath, pszPath);

        HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.find(pNewSetupInfo);
        if (iter == m_HookedSetupInfos.end())
        {
            //not found
            m_HookedSetupInfos.insert(pNewSetupInfo);
        }
    }
    return bRet;
}

BOOL CSetupInfoMgr::CloseSetupReg(HKEY hKey)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    RegKeyPathPairContainer::iterator iterReg = m_RegKeyPathPairs.find(hKey);
    //FTLASSERT(iterReg != m_RegKeyPathPairs.end());
    if (iterReg != m_RegKeyPathPairs.end())
    {
        FTLTRACE(TEXT("!!! CloseSetupReg, hKey=0x%x, path=%s\n"), hKey, iterReg->second);

        m_RegKeyPathPairs.erase(iterReg);
    }
    else
    {
        FTLTRACEEX(tlWarning, TEXT("Warning, Close not saved key for 0x%x\n"), hKey);
    }
    
    return bRet;
}

BOOL CSetupInfoMgr::GetAllSetupInfo(HWND hWndGetResult)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    FTLTRACE(TEXT("GetAllSetupInfo: size=%d, hWndGetResult=0x%x\n"), m_HookedSetupInfos.size(), hWndGetResult);
#if 0
    int nIndex = 0;
    for(HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.begin();
        iter != m_HookedSetupInfos.end();
        ++iter)
    {
        const HookedSetupInfoPtr& pHookedSetupInfo = *iter;
        if (pHookedSetupInfo)
        {
            SETUP_MONITOR_INFO info;
            info.type = (MonitorInfoType)pHookedSetupInfo->infoType;
            info.szPath[0] = NULL;
            info.szKeyName[0] = NULL;
            StringCchCopy(info.szPath, _countof(info.szPath), pHookedSetupInfo->pszPath);
            if (pHookedSetupInfo->pszValue)
            {
                StringCchCopy(info.szKeyName, _countof(info.szKeyName), pHookedSetupInfo->pszValue);
            }
            else
            {
                info.szKeyName[0] = _T('\0');
            }
            COPYDATASTRUCT copyData;
            copyData.cbData = sizeof(info);
            copyData.dwData = 1;
            copyData.lpData = &info;

            bRet  = (BOOLEAN)SendMessage(hWndGetResult,  WM_COPYDATA, NULL, (LPARAM) (LPVOID)&copyData);
            FTLTRACE(TEXT("\t[%d]%d - %s -> %s\n"), ++nIndex, pHookedSetupInfo->infoType, pHookedSetupInfo->pszPath, 
                pHookedSetupInfo->pszValue ? pHookedSetupInfo->pszValue : _T("<None>"));
        }
    }
#endif
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
            FTLTRACE(TEXT("\t[%d]%d - %s -> %s\n"), ++nIndex, pHookedSetupInfo->infoType, pHookedSetupInfo->pszPath,
                pHookedSetupInfo->pszValue ? pHookedSetupInfo->pszValue : _T("<None>"));
        }
    }
    return bRet;
}

