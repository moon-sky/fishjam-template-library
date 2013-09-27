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
    BOOL SetSetupInfo(HookedSetupInfoType infoType, LPCTSTR pszOldInfo, LPCTSTR pszNewInfo);
    BOOL    DumpAllSetupInfo();
private:
    typedef std::set<HookedSetupInfoPtr, FTL::UnreferenceLess<HookedSetupInfoPtr>> HookedSetupInfoContainer;
    HookedSetupInfoContainer    m_HookedSetupInfos;
    FTL::CFCriticalSection      m_csLock;
private:
};

extern CSetupInfoMgr* g_pSetupInfoMgr;