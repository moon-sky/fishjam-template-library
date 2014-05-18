#ifndef FTL_DUI_HPP
#define FTL_DUI_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDUI.h"
#endif

namespace FTL
{
	CFDuiNotifyDumpInfo::CFDuiNotifyDumpInfo(const TNotifyUI& msg) 
		: CFConvertInfoT<CFDuiNotifyDumpInfo, const TNotifyUI&, 128>(msg)
	{
	}

	LPCTSTR CFDuiNotifyDumpInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			CControlUI* pSender = m_Info.pSender;
			ATLASSERT(pSender);
			StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
				TEXT("sType=%s, sVirtualWnd=%s, pSender=(%s , 0x%p),wParam=0x%x, lParam=0x%x, ptMouse=(%d,%d)"),
				m_Info.sType.GetData(), m_Info.sVirtualWnd.GetData(), 
				pSender->GetName().GetData(), pSender,
				m_Info.wParam,m_Info.lParam,
				m_Info.ptMouse.x, m_Info.ptMouse.y);
		}
		return m_bufInfo;
	}

}
#endif //FTL_DUI_HPP