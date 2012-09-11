#ifndef FTL_GDIPLUS_HPP
#define FTL_GDIPLUS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlGdiPlus.h"
#endif

namespace FTL
{
	CFAutoGdiplus::CFAutoGdiplus()
	{
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	}

	CFAutoGdiplus::~CFAutoGdiplus()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

}

#endif //FTL_GDIPLUS_HPP