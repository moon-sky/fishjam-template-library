#pragma once

LPCTSTR ConvertRegRootKeyToString(HKEY hKeyRoot);
HKEY ConvertStringToRegRootKey(LPCTSTR pszKeyRoot);

class CPathConvert
{
public:
    CPathConvert();
    ~CPathConvert();
    //BOOL GetCsidlPath(LPCTSTR pszPath, CAtlString& strResult);
    BOOL GetEnvPath(const CAtlString& strFullPath, CAtlString& strResult, const CAtlString& strFormat = _T(""));
private:

    struct PathConvertInfo
    {
        //int             nConvertType;   //0-Env, 1-Csidl
        int             nFolder;
        CAtlString      strName;
        CAtlString      strPath;
        bool operator <(const PathConvertInfo& other) const
        {
            COMPARE_MEM_LESS(strPath, other);
            COMPARE_MEM_LESS(nFolder, other);
            COMPARE_MEM_LESS(strName, other);

            return false;
        }
    };

    typedef std::set<PathConvertInfo> PathConvertInfoContainer;
    PathConvertInfoContainer    m_pathConvertInfos;

    BOOL _AddPathConvertMap(int nFolder, LPCTSTR pszName, LPTSTR lpszPath);
};