#include "DLNAHandlerImpl.h"

NPT_SET_LOCAL_LOGGER("Handler.DLNA.Impl")

CDLNAHandlerImpl::CDLNAHandlerImpl()
{
    return;
}

CDLNAHandlerImpl::~CDLNAHandlerImpl()
{

}

NPT_Int32 CDLNAHandlerImpl::Init()
{
    m_pCtrlPoint = new PLT_CtrlPoint();
    m_pSyncMediaBrowser = new PLT_SyncMediaBrowser(m_pCtrlPoint, false, this);
    m_pSyncMediaBrowser->SetDelegate(this);
    m_pMediaController = new PLT_MediaController(m_pCtrlPoint, this);
    
    return 0;
}

NPT_Int32 CDLNAHandlerImpl::Release()
{
    delete this;
    return 0;
}

NPT_Int32 CDLNAHandlerImpl::RefreshDevice()
{
    m_upnp.AddCtrlPoint(m_pCtrlPoint);
    m_upnp.Start();
    return 0;
}

bool CDLNAHandlerImpl::OnMSAdded(PLT_DeviceDataReference& device)
{
    NPT_LOG_FINE_1("OnMSAdded:%s", device->GetFriendlyName());
    return false;
}

void CDLNAHandlerImpl::OnMSRemoved(PLT_DeviceDataReference& device)
{
    NPT_LOG_FINE_1("OnMSRemoved:%s", device->GetFriendlyName());
}


bool CDLNAHandlerImpl::OnMRAdded(PLT_DeviceDataReference& device)
{
    NPT_LOG_FINE_1("OnMRAdded:%s", device->GetFriendlyName());
    return true;
}

void CDLNAHandlerImpl::OnMRRemoved(PLT_DeviceDataReference& device)
{
    NPT_LOG_FINE_1("OnMRRemoved:%s", device->GetFriendlyName());
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
