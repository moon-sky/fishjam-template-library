#pragma once

#include "../CastingHandler.h"
#include "Platinum.h"
#include "PltCtrlPoint.h"
#include "PltUPnP.h"
#include "PltMediaController.h"

class CDLNAHandlerImpl: 
    public INCastingHandler,
    public PLT_MediaControllerDelegate,
    public PLT_MediaContainerChangesListener,
    public PLT_MediaBrowserDelegate
{
public:
    CDLNAHandlerImpl();
    ~CDLNAHandlerImpl();

    //ICastingHandler:
    virtual int   Init(INCastingEventCallback* pCallback);
    virtual int   Release();
    virtual int   RefreshDevice();
    virtual int   PlayMediaObjectOnDevice(INMediaObject* pMediaObject, INDevice* pDevice);

    // PLT_MediaBrowserDelegate methods
    virtual bool OnMSAdded(PLT_DeviceDataReference& device);
    virtual void OnMSRemoved(PLT_DeviceDataReference& device);

    //PLT_MediaControllerDelegate
    virtual bool OnMRAdded(PLT_DeviceDataReference& device);
    virtual void OnMRRemoved(PLT_DeviceDataReference& device);
    virtual void OnMRStateVariablesChanged(PLT_Service* service, NPT_List<PLT_StateVariable*>*  vars );
    virtual void OnPrepareForConnectionResult(NPT_Result res, PLT_DeviceDataReference& device, 
        PLT_ConnectionInfo*  info, void* userdata);
    virtual void OnSetAVTransportURIResult(NPT_Result res, PLT_DeviceDataReference& device,
        void* userdata);
    virtual void OnPlayResult(NPT_Result res, PLT_DeviceDataReference& device,void* userdata);

    //PLT_MediaContainerChangesListener
    virtual void OnContainerChanged(PLT_DeviceDataReference& device, const char* item_id, const char* update_id);

private:
    INCastingEventCallback*         m_pCallback;

    PLT_SyncMediaBrowserReference   m_pSyncMediaBrowser;
    PLT_CtrlPointReference          m_pCtrlPoint;
    PLT_MediaControllerReference    m_pMediaController;
    PLT_DeviceHostReference         m_pMediaServerHost;

    PLT_DeviceDataReference         m_curMediaRender;
    PLT_UPnP m_upnp;
    int m_connection_id;
};