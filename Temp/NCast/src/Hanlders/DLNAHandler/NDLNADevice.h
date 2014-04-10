#pragma once

#include "../CastingHandler.h"
#include "NptStrings.h"

class PLT_DeviceData;
class CNDLNADevice : public INDevice
{
public:
    CNDLNADevice(PLT_DeviceData* pDeviceData);
    ~CNDLNADevice(void);

    virtual const char* GetFriendlyName(){
        return m_strFriendlyName;
    }

private:
    NPT_String m_strFriendlyName;
};
