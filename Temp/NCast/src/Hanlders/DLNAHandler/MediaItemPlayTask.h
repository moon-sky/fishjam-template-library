#pragma once

#include "PltThreadTask.h"
#include "PltDeviceData.h"
#include "../CastingHandler.h"

class MediaItemPlayTask : public PLT_ThreadTask
{
public:
    MediaItemPlayTask(PLT_CtrlPointReference& ctrlPoint, PLT_DeviceData& deviceMR, INMediaObject* pMediaObject);
    //PLT_DeviceData& deviceMR, PLT_DeviceData& deviceMS, INMediaObject* pMediaObject);
    ~MediaItemPlayTask(void);

public:
    //PLT_ThreadTask
    virtual void DoInit()    {}
    virtual void DoAbort()   {}
    virtual void DoRun();
protected:
    NPT_Result   _InnerPlayerProc();
private:
    PLT_CtrlPointReference m_ctrlPoint;
    PLT_DeviceData& m_deviceMR;
    //PLT_DeviceData& m_deviceMS;
    INMediaObject*          m_pMediaObject;
};
