#ifndef FTL_GDIPLUS_HPP
#define FTL_GDIPLUS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlGdiPlus.h"
#endif

namespace FTL
{
    CFGdiPlusErrorInfo::CFGdiPlusErrorInfo(Gdiplus::Status status)
        : CFConvertInfoT<CFGdiPlusErrorInfo, Gdiplus::Status>(status)
    {
    }

    LPCTSTR CFGdiPlusErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            using namespace Gdiplus;
            switch (m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), Ok);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), GenericError);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), InvalidParameter);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), OutOfMemory);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), ObjectBusy);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), InsufficientBuffer);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), NotImplemented);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), Win32Error);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), WrongState);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), Aborted);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), FileNotFound);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), ValueOverflow);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), AccessDenied);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), UnknownImageFormat);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), FontFamilyNotFound);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), FontStyleNotFound);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), NotTrueTypeFont);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), UnsupportedGdiplusVersion);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), GdiplusNotInitialized);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), PropertyNotFound);
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), PropertyNotSupported);
#if (GDIPVER >= 0x0110)
                HANDLE_CASE_TO_STRING(m_bufInfo,_countof(m_bufInfo), ProfileNotFound);
#endif //(GDIPVER >= 0x0110)
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("Unknown Error:%d"), m_Info);
                break;
            }
        }
        return m_bufInfo;
    }

	CFAutoGdiplus::CFAutoGdiplus()
	{
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	}

	CFAutoGdiplus::~CFAutoGdiplus()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}


	BOOL CFGdiPlusUtil::DrawNineBlockImage(Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImages[9], const Gdiplus::Rect* pRect)
	{
		pGraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
		pGraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHalf);

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