#ifndef FTL_DEBUG_HPP
#define FTL_DEBUG_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlDebug.h"
#endif

namespace FTL
{
	CFRectDumpInfo::CFRectDumpInfo(const RECT& rect) 
		: CFConvertInfoT<CFRectDumpInfo, const RECT&, 64>(rect)
	{
	}

	LPCTSTR CFRectDumpInfo::ConvertInfo()
	{
		if (NULL == m_bufInfo[0])
		{
			StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("(%d,%d)-(%d,%d), %dx%d"),
				m_Info.left, m_Info.top, m_Info.right, m_Info.bottom,
				m_Info.right - m_Info.left, m_Info.bottom - m_Info.top);
		}
		return m_bufInfo;
	}
}

#endif //FTL_DEBUG_HPP