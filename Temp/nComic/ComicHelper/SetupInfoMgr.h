#pragma once
#include <set>
#include <map>
#include <ftlThread.h>
#include <ftlSharePtr.h>
#include <ftlFunctional.h>

enum HookedSetupInfoType
{
    hsit_Invalid,

    hsit_NewFile,

    hsit_NewReg,
};
struct HookedSetupInfo
{
    HookedSetupInfoType infoType;
    LPTSTR              pszInfo;
    LONG                nInfoLength;
    //HookedSetupInfo*    pNextInfo;

    bool operator < (const HookedSetupInfo & other) const;
    HookedSetupInfo();
    ~HookedSetupInfo();
};

typedef CFSharePtr<HookedSetupInfo> HookedSetupInfoPtr;

class CSetupInfoMgr
{
public:
    CSetupInfoMgr(void);
    ~CSetupInfoMgr(void);

public:
    BOOL SetSetupFileInfo(/*HookedSetupInfoType infoType, */LPCTSTR pszOldInfo, LPCTSTR pszNewInfo);
    BOOL DeleteSetupFileInfo(LPCTSTR pszFilePath);

    BOOL OpenSetupReg(HKEY hKeyRoot, HKEY hKey, LPCTSTR pszPath);
    BOOL SetSetupRegInfo(HKEY hKey, LPCTSTR pszPath);
    BOOL CloseSetupReg(HKEY hKey);

    BOOL    DumpAllSetupInfo();
private:
    FTL::CFCriticalSection      m_csLock;

    //TODO: will new every time
    typedef std::set<HookedSetupInfoPtr, FTL::UnreferenceLess<HookedSetupInfoPtr>> HookedSetupInfoContainer;
    HookedSetupInfoContainer    m_HookedSetupInfos;

    typedef std::map<HKEY, CAtlString> RegKeyPathPairContainer;
    RegKeyPathPairContainer     m_RegKeyPathPairs;
    
private:
    CAtlString _GetRegRootKeyString(HKEY hKeyRoot);
};

extern CSetupInfoMgr* g_pSetupInfoMgr;