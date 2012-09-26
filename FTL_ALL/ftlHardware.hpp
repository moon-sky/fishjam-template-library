#ifndef FTL_HARDWARE_HPP
#define FTL_HARDWARE_HPP

#pragma once

#ifndef FTL_BASE_H
#  error ftlHardware.h requires ftlbase.h to be included first
#endif

#ifdef USE_EXPORT
#  include "ftlHardware.h"
#endif

namespace FTL
{
	CFDeviceNotification::CFDeviceNotification()
    {
        m_hDevNotify = NULL;
    }
    CFDeviceNotification::~CFDeviceNotification()
    {
        if (m_hDevNotify)
        {
            UnRegister();
        }
    }

    BOOL CFDeviceNotification::Register(HANDLE hRecipient,LPVOID NotificationFilter,DWORD Flags)
    {
        FTLASSERT(NULL == m_hDevNotify);
        BOOL bRet = TRUE;
        if (NULL == m_hDevNotify)
        {
            m_hDevNotify = RegisterDeviceNotification(hRecipient,NotificationFilter,
                Flags);
            API_VERIFY(NULL != m_hDevNotify);
        }
        return bRet;
    }

    BOOL CFDeviceNotification::UnRegister()
    {
        FTLASSERT(NULL != m_hDevNotify);
        BOOL bRet = TRUE;
        if (m_hDevNotify)
        {
            API_VERIFY(UnregisterDeviceNotification(m_hDevNotify));
            m_hDevNotify = NULL;
        }
        return bRet;
    }
}

#endif //FTL_HARDWARE_H