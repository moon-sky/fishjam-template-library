#include "DLNAHandlerImpl.h"
#include "NDLNADevice.h"
#include "MediaItemPlayTask.h"

NPT_SET_LOCAL_LOGGER("Handler.DLNA.Impl")

CDLNAHandlerImpl::CDLNAHandlerImpl()
:m_pCallback(NULL)
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
    m_pMediaServerHost = new PLT_FileMediaServer("E:\\Video", "NaverMediaCasting", true, NULL, 0, true);

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
    //m_pMediaController->GetCurrentConnectionIDs()
    MediaItemPlayTask* pTask = new MediaItemPlayTask(*m_curMediaRender, *m_pMediaServerHost, pMediaObject);
    pTask->Start();
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
