#pragma once

#include "../CastingHandler.h"
#include "Platinum.h"
#include "PltCtrlPoint.h"
#include "PltUPnP.h"
#include "PltMediaController.h"

class CASTING_HANDLER_API CDLNAHandlerImpl: 
    public ICastingHandler,
    public PLT_MediaControllerDelegate,
    public PLT_MediaContainerChangesListener,
    public PLT_MediaBrowserDelegate
{
public:
    CDLNAHandlerImpl();
    ~CDLNAHandlerImpl();

    //ICastingHandler:
    virtual NPT_Int32   Init();
    virtual NPT_Int32   Release();
    virtual NPT_Int32   RefreshDevice();

    // PLT_MediaBrowserDelegate methods
    virtual bool OnMSAdded(PLT_DeviceDataReference& device);
    virtual void OnMSRemoved(PLT_DeviceDataReference& device);

    //PLT_MediaControllerDelegate
    virtual bool OnMRAdded(PLT_DeviceDataReference& device);
    virtual void OnMRRemoved(PLT_DeviceDataReference& device);
    virtual void OnMRStateVariablesChanged(PLT_Service* service, NPT_List<PLT_StateVariable*>*  vars );

    //PLT_MediaContainerChangesListener
    virtual void OnContainerChanged(PLT_DeviceDataReference& device, const char* item_id, const char* update_id);

private:
    PLT_SyncMediaBrowserReference   m_pSyncMediaBrowser;
    PLT_CtrlPointReference          m_pCtrlPoint;
    PLT_MediaControllerReference    m_pMediaController;
    PLT_UPnP m_upnp;
};