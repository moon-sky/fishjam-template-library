#include "StdAfx.h"
#include "SetupInfoMgr.h"
#include "SetupMakerHelper.h"

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

BOOL CSetupInfoMgr::SetSetupFileInfo(/*HookedSetupInfoType infoType,*/ LPCTSTR pszOldInfo, LPCTSTR pszNewInfo)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    
    HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
    pNewSetupInfo->infoType = hsit_NewFile;
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

BOOL CSetupInfoMgr::DeleteSetupFileInfo(LPCTSTR pszFilePath)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);

    HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
    pNewSetupInfo->infoType = hsit_NewFile;
    pNewSetupInfo->nInfoLength = lstrlen(pszFilePath);
    pNewSetupInfo->pszInfo = new TCHAR[pNewSetupInfo->nInfoLength + 1];
    lstrcpy(pNewSetupInfo->pszInfo, pszFilePath);
    
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
    CAtlString strFullPath = _GetRegRootKeyString(hKeyRoot) + pszPath;
    RegKeyPathPairContainer::iterator iterReg = m_RegKeyPathPairs.find(hKey);
    if (iterReg == m_RegKeyPathPairs.end())
    {
        m_RegKeyPathPairs[hKey] = strFullPath;
    }
    else
    {
        FTLASSERT(iterReg->second == strFullPath);
    }
    return bRet;
}

BOOL CSetupInfoMgr::SetSetupRegInfo(HKEY hKey, LPCTSTR pszPath)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    RegKeyPathPairContainer::iterator iterReg = m_RegKeyPathPairs.find(hKey);
    FTLASSERT(iterReg != m_RegKeyPathPairs.end());
    if (iterReg != m_RegKeyPathPairs.end())
    {
        const CString& strParentPath = iterReg->second;

        HookedSetupInfoPtr pNewSetupInfo(new HookedSetupInfo());
        pNewSetupInfo->infoType = hsit_NewReg;
        pNewSetupInfo->nInfoLength = strParentPath.GetLength() + 1 + lstrlen(pszPath);  // 1 is "\\"
        pNewSetupInfo->pszInfo = new TCHAR[pNewSetupInfo->nInfoLength + 1];
        StringCchPrintf(pNewSetupInfo->pszInfo, pNewSetupInfo->nInfoLength + 1, TEXT("%s\\%s"), 
            strParentPath, pszPath);

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
        m_RegKeyPathPairs.erase(iterReg);
    }
    
    return bRet;
}

BOOL CSetupInfoMgr::GetAllSetupInfo(HWND hWndGetResult)
{
    BOOL bRet = TRUE;
    FTL::CFAutoLock<FTL::CFLockObject> lockObj(&m_csLock);
    FTLTRACE(TEXT("GetAllSetupInfo: size=%d, hWndGetResult=0x%x\n"), m_HookedSetupInfos.size(), hWndGetResult);

    int nIndex = 0;
    for(HookedSetupInfoContainer::iterator iter = m_HookedSetupInfos.begin();
        iter != m_HookedSetupInfos.end();
        ++iter)
    {
        const HookedSetupInfoPtr& pHookedSetupInfo = *iter;
        if (pHookedSetupInfo)
        {
            SETUP_MONITOR_INFO info;
            info.dwInfoType = (DWORD)pHookedSetupInfo->infoType;
            StringCchCopy(info.szPath, _countof(info.szPath), pHookedSetupInfo->pszInfo);

            COPYDATASTRUCT copyData;
            copyData.cbData = sizeof(info);
            copyData.dwData = 1;
            copyData.lpData = &info;

            API_VERIFY(TRUE == (BOOLEAN)SendMessage(hWndGetResult,  WM_COPYDATA, NULL, (LPARAM) (LPVOID)&copyData));
            //FTLTRACE(TEXT("\t[%d]%d - %s\n"), ++nIndex, pHookedSetupInfo->infoType, pHookedSetupInfo->pszInfo);
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

CAtlString CSetupInfoMgr::_GetRegRootKeyString(HKEY hKeyRoot)
{
    switch ((ULONG_PTR)((LONG)hKeyRoot))
    {
        HANDLE_CASE_RETURN_STRING_EX(HKEY_CLASSES_ROOT, TEXT("HKEY_CLASSES_ROOT\\"));
        HANDLE_CASE_RETURN_STRING_EX(HKEY_CURRENT_USER, TEXT("HKEY_CURRENT_USER\\"));
        HANDLE_CASE_RETURN_STRING_EX(HKEY_LOCAL_MACHINE, TEXT("HKEY_LOCAL_MACHINE\\"));
        HANDLE_CASE_RETURN_STRING_EX(HKEY_USERS, TEXT("HKEY_USERS\\"));
        HANDLE_CASE_RETURN_STRING_EX(HKEY_CURRENT_CONFIG, TEXT("HKEY_CURRENT_CONFIG\\"));
    default:
        FTLTRACEEX(tlError, TEXT("Unknown RootKey:0x%x\n"), hKeyRoot);
        FTLASSERT(FALSE);
        break;
    }
    return TEXT("Unknown");
}