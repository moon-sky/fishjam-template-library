#include "NDLNADevice.h"
#include "Platinum.h"

CNDLNADevice::CNDLNADevice(PLT_DeviceData* pDeviceData)
{
    m_strFriendlyName = pDeviceData->GetFriendlyName();
}

CNDLNADevice::~CNDLNADevice(void)
{
}
