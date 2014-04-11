#include "MediaItemPlayTask.h"
NPT_SET_LOCAL_LOGGER("Handler.DLNA.MediaItemPlayTask")


MediaItemPlayTask::MediaItemPlayTask(PLT_CtrlPointReference& ctrlPoint, PLT_DeviceData& deviceMR, INMediaObject* pMediaObject)
:m_ctrlPoint(ctrlPoint)
,m_deviceMR(deviceMR)
//,m_deviceMS(deviceMS)
,m_pMediaObject(pMediaObject)
{
}

MediaItemPlayTask::~MediaItemPlayTask(void)
{
}


void MediaItemPlayTask::DoRun(){
    
    _InnerPlayerProc();

    //int n = 10;
    //while(n > 0){
    //    NPT_LOG_FINE_1("MediaItemPlayTask::DoRun, n=%d", n);
    //    NPT_System::Sleep(1.0);
    //    n--;
    //}
    //delete this;
}

NPT_Result MediaItemPlayTask::_InnerPlayerProc()
{
    //PLT_ActionDesc* pActionDesc = NULL;
    //NPT_CHECK_FATAL(m_ctrlPoint->FindActionDesc(m_deviceMR, 
    //    "urn:schemas-upnp-org:device:MediaRenderer:1", "GetProtocolInfo", pActionDesc));
    //if (pActionDesc)
    //{
    //    pActionDesc->SetArgumentValue
    //    m_ctrlPoint->InvokeAction(pActionDesc);
    //}

    //PLT_Service* pService = NULL;
    //NPT_CHECK_FATAL(m_deviceMR.FindServiceByName("urn:schemas-upnp-org:device:MediaRenderer:1", pService));
    //
    //PLT_ActionDesc* pActionDesc = pService->FindActionDesc("GetProtocolInfo");
    //if (pActionDesc)
    //{
    //    pActionDesc->
    //}
    return NPT_SUCCESS;
}