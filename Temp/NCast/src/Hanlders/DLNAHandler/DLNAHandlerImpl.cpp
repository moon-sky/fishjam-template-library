#include "DLNAHandlerImpl.h"
#include "NDLNADevice.h"
#include "MediaItemPlayTask.h"

NPT_SET_LOCAL_LOGGER("Handler.DLNA.Impl")

CDLNAHandlerImpl::CDLNAHandlerImpl()
:m_pCallback(NULL)
,m_connection_id(-1)
{
    return;
}

CDLNAHandlerImpl::~CDLNAHandlerImpl()
{

}

int CDLNAHandlerImpl::Init(INCastingEventCallback* pCallback)
{
    m_pCallback = pCallback;

    m_pCtrlPoint = new PLT_CtrlPoint();
    m_pSyncMediaBrowser = new PLT_SyncMediaBrowser(m_pCtrlPoint, false, this);
    m_pSyncMediaBrowser->SetDelegate(this);
    m_pMediaController = new PLT_MediaController(m_pCtrlPoint, this);
    m_pMediaServerHost = new PLT_FileMediaServer("E:\\Video", "NaverMediaCasting", true, NULL, 1966, true);

    NPT_CHECK_SEVERE(m_upnp.AddCtrlPoint(m_pCtrlPoint));
    NPT_CHECK_SEVERE(m_upnp.AddDevice(m_pMediaServerHost));

    //m_pMediaController->GetCurrentConnectionIDs()
    m_upnp.Start();
    return 0;
}

int CDLNAHandlerImpl::Release()
{
    NPT_CHECK_SEVERE(m_upnp.Stop());

    delete this;
    return 0;
}

int CDLNAHandlerImpl::RefreshDevice()
{
    return 0;
}

int CDLNAHandlerImpl::PlayMediaObjectOnDevice(INMediaObject* pMediaObject, INDevice* pDevice)
{
    NPT_CHECK_SEVERE(m_pMediaController->PrepareForConnection(m_curMediaRender, "http-get:*:video/mpeg:*", 
        "", -1, true, NULL));
    

    //m_pMediaController->GetCurrentConnectionIDs()
    //MediaItemPlayTask* pTask = new MediaItemPlayTask(*m_curMediaRender, *m_pMediaServerHost, pMediaObject);
    //pTask->Start();
    //pDevice->GetFriendlyName();
    return 0;
}

bool CDLNAHandlerImpl::OnMSAdded(PLT_DeviceDataReference& device)
{
    NPT_LOG_FINE_1("OnMSAdded:%s", device->GetFriendlyName());
    if (m_pCallback)
    {
        m_pCallback->OnDeviceAdd(new CNDLNADevice(device.AsPointer()));
    }
    
    return false;
}

void CDLNAHandlerImpl::OnMSRemoved(PLT_DeviceDataReference& device)
{
    NPT_LOG_FINE_1("OnMSRemoved:%s", device->GetFriendlyName());
}

void CDLNAHandlerImpl::OnPrepareForConnectionResult(NPT_Result res, PLT_DeviceDataReference& device, 
                                          PLT_ConnectionInfo*  info, void* userdata)
{
    if (NPT_SUCCEEDED(res))
    {
        m_connection_id = info->peer_connection_id;
        NPT_LOG_FINE_2("OnPrepareForConnectionResult, res=%d, connectino_id=%d", res, info->peer_connection_id);
        m_pMediaController->SetAVTransportURI(m_curMediaRender, info->peer_connection_id, 
            "http://192.168.172.1:1966/%25/butterfly.mpg", "", NULL);
    }
}

void CDLNAHandlerImpl::OnSetAVTransportURIResult(NPT_Result res, PLT_DeviceDataReference& device, void* userdata)
{
    NPT_LOG_FINE_1("OnSetAVTransportURIResult, res=%d", res);
    m_pMediaController->Play(m_curMediaRender, m_connection_id, "1", NULL);
}
void CDLNAHandlerImpl::OnPlayResult(NPT_Result res, PLT_DeviceDataReference& device,void* userdata)
{
    NPT_LOG_FINE_1("OnPlayResult, res=%d", res);
}

bool CDLNAHandlerImpl::OnMRAdded(PLT_DeviceDataReference& device)
{
    m_curMediaRender = device;
    NPT_LOG_FINE_1("OnMRAdded:%s", device->GetFriendlyName());
    if (m_pCallback)
    {
        m_pCallback->OnDeviceAdd(new CNDLNADevice(device.AsPointer()));
    }
    return true;
}

void CDLNAHandlerImpl::OnMRRemoved(PLT_DeviceDataReference& device)
{
    m_curMediaRender = NULL;

    NPT_LOG_FINE_1("OnMRRemoved:%s", device->GetFriendlyName());
    if (m_pCallback)
    {
        //m_pCallback->OnDeviceRemove(new CNDLNADevice(device.AsPointer()));
    }

}
void CDLNAHandlerImpl::OnMRStateVariablesChanged(PLT_Service* service, NPT_List<PLT_StateVariable*>*  vars )
{
    NPT_LOG_FINE("OnMRStateVariablesChanged");
}
void CDLNAHandlerImpl::OnContainerChanged(PLT_DeviceDataReference& device, 
                                          const char* item_id, const char* update_id)
{
    NPT_LOG_FINE("OnContainerChanged");
}
