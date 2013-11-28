#include "stdafx.h"
#include "InfoConvUtil.h"
#include <shlobj.h>
#include <ftlSystem.h>

typedef std::pair<HKEY, LPCTSTR> HKeyRoot2StringPair;
static HKeyRoot2StringPair s_RegRootStringPairs[] = {
    std::make_pair(HKEY_CLASSES_ROOT, TEXT("HKEY_CLASSES_ROOT")),
    std::make_pair(HKEY_CURRENT_USER, TEXT("HKEY_CURRENT_USER")),
    std::make_pair(HKEY_LOCAL_MACHINE, TEXT("HKEY_LOCAL_MACHINE")),
    std::make_pair(HKEY_USERS, TEXT("HKEY_USERS")),
    std::make_pair(HKEY_CURRENT_CONFIG, TEXT("HKEY_CURRENT_CONFIG")),
};

LPCTSTR ConvertRegRootKeyToString(HKEY hKeyRoot)
{
    for (int i = 0; i < _countof(s_RegRootStringPairs); i++)
    {
        if (hKeyRoot == s_RegRootStringPairs[i].first)
        {
            return s_RegRootStringPairs[i].second;
        }
    }
    //FTLTRACEEX(FTL::tlError, TEXT("Unknown RootKey:0x%x\n"), hKeyRoot);
    return NULL;
}

HKEY ConvertStringToRegRootKey(LPCTSTR pszKeyRoot)
{
    for (int i = 0; i < _countof(s_RegRootStringPairs); i++)
    {
        if (lstrcmpi(pszKeyRoot, s_RegRootStringPairs[i].second) == 0)
        {
            return s_RegRootStringPairs[i].first;
        }
    }
    FTLTRACEEX(FTL::tlError, TEXT("Unknown Root String:%s\n"), pszKeyRoot);
    FTLASSERT(FALSE);
    return NULL;
}

CPathConvert::CPathConvert()
{
    BOOL bRet = FALSE;

    TCHAR szPath[MAX_PATH] = {0};

    _AddPathConvertMap(-1, TEXT("AllUsersProfile"), szPath);
    _AddPathConvertMap(-1, TEXT("AppData"), szPath);
    _AddPathConvertMap(-1, TEXT("CommonProgramFiles"), szPath);
    _AddPathConvertMap(-1, TEXT("ProgramFiles(x86)"), szPath);
    _AddPathConvertMap(-1, TEXT("ProgramFiles"), szPath);
    _AddPathConvertMap(-1, TEXT("UserProfile"), szPath);
    _AddPathConvertMap(-1, TEXT("PUBLIC"), szPath);
    if (FTL::CFSystemUtil::Is64BitWindows())
    {
        _AddPathConvertMap(-1, TEXT("ProgramFiles(x86)"), szPath);
    }
    
    //_AddConvertPathMap(CSIDL_DESKTOP, _T("CSIDL_DESKTOP"), szPath);
}

CPathConvert::~CPathConvert()
{

}

BOOL CPathConvert::_AddPathConvertMap(int nFolder, LPCTSTR pszName, LPTSTR lpszPath)
{
    BOOL bRet = FALSE;
    lpszPath[0] = NULL;
    //API_VERIFY(SHGetSpecialFolderPath(NULL, lpszPath, nFolder, FALSE));
    API_VERIFY_EXCEPT1((GetEnvironmentVariable(pszName, lpszPath, MAX_PATH) > 0), ERROR_ENVVAR_NOT_FOUND);
    if (bRet)
    {
        PathConvertInfo info;
        info.nFolder = nFolder;
        info.strName = pszName;
        info.strPath = lpszPath;
        m_pathConvertInfos.insert(info);
    }
    return bRet;
}

BOOL CPathConvert::GetEnvPath(const CAtlString& strFullPath, CAtlString& strResult, const CAtlString& strFormat)
{
    BOOL bRet = FALSE;
    for (PathConvertInfoContainer::iterator iter = m_pathConvertInfos.begin();
        iter != m_pathConvertInfos.end();
        ++iter)
    {
        const PathConvertInfo& pathInfo = *iter;
        
        if (StrStrI(strFullPath, pathInfo.strPath) != NULL)
        {
            strResult = strFullPath;
            CAtlString strReplaceValue;
            if (!strFormat.IsEmpty())
            {
                strReplaceValue.Format(strFormat, pathInfo.strName);
            }
            else
            {
                strReplaceValue = pathInfo.strName;
            }
            strResult.Replace(pathInfo.strPath,  strReplaceValue);
            bRet = TRUE;
            break;
        }
    }
    return bRet;
}