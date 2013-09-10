#pragma once

#include "ComicService_i.h"
class CComicServiceModule : public CAtlServiceModuleT< CComicServiceModule, IDS_SERVICENAME >
{
public :
    DECLARE_LIBID(LIBID_ComicServiceLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMICSERVICE, "{91F71D7D-DBA9-4403-B629-940731E8AA4F}")

    CComicServiceModule();
    HRESULT InitializeSecurity() throw();
    HRESULT RegisterAppId(bool bService = false) throw();

    HRESULT PreMessageLoop(int nShowCmd) throw();
    void RunMessageLoop() throw();
    HRESULT PostMessageLoop() throw();
    void OnPause() throw();
    void OnContinue() throw();
    void OnStop() throw();
    //void Handler(DWORD dwOpcode) throw()
    //{
    //    switch(dwOpcode)
    //    {
    //    case SERVICE_CONTROL_PAUSE:
    //        OnPause();
    //        break;
    //    case SERVICE_CONTROL_CONTINUE:
    //        OnContinue();
    //        break;
    //    default:
    //        __super::Handler(dwOpcode);
    //    }
    //}
};
