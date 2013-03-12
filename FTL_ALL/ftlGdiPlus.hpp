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


	BOOL CFGdiPlusUtil::DrawNineBlockImage(Graphics pGraphics, Image* pImages[9], const Gdiplus::Rect* pRect)
	{
		pGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
		pGraphics->SetPixelOffsetMode(PixelOffsetModeHalf);

		//if (pRect->Width < MINIMUM_BUBBLE_WIDTH)
		//{
		//	width = MINIMUM_BUBBLE_WIDTH;
		//}

		INT vx = pRect->Width - pImages[0]->GetWidth() - pImages[2]->GetWidth();
		INT vy = pRect->Height - pImages[0]->GetHeight() - pImages[6]->GetHeight();

		INT sx = pRect->X;
		INT sy = pRect->Y;

		pGraphics->DrawImage(pImages[0], sx, sy, pImages[0]->GetWidth(), pImages[0]->GetHeight());
		sx += pImages[0]->GetWidth();
		pGraphics->DrawImage(pImages[1], sx, sy, vx + 1, pImages[1]->GetHeight());
		sx += vx;
		pGraphics->DrawImage(pImages[2], sx, sy, pImages[2]->GetWidth(), pImages[2]->GetHeight());

		sx = pRect->X;
		sy += pImages[2]->GetHeight();
		pGraphics->DrawImage(pImages[3], sx, sy, pImages[3]->GetWidth(), vy);
		sx += pImages[3]->GetWidth();
		pGraphics->DrawImage(pImages[4], sx, sy, vx + 1, vy);
		sx += vx;
		pGraphics->DrawImage(pImages[5], sx, sy, pImages[5]->GetWidth(), vy);

		sx = pRect->X;
		sy += vy;
		pGraphics->DrawImage(pImages[6], sx, sy, pImages[6]->GetWidth(), pImages[6]->GetHeight());
		sx += pImages[6]->GetWidth();
		pGraphics->DrawImage(pImages[7], sx, sy, vx + 1, pImages[7]->GetHeight());
		sx += vx;
		pGraphics->DrawImage(pImages[8], sx, sy, pImages[8]->GetWidth(), pImages[8]->GetHeight());
	}
}

#endif //FTL_GDIPLUS_HPP