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

    //CFRectFDumpInfo::CFRectFDumpInfo(const Gdiplus::RectF& rect) 
    //    : CFConvertInfoT<CFRectFDumpInfo, const Gdiplus::RectF&, 64>(rect)
    //{
    //}

    //LPCTSTR CFRectFDumpInfo::ConvertInfo()
    //{
    //    if (NULL == m_bufInfo[0])
    //    {
    //        StringCchPrintf(m_bufInfo,_countof(m_bufInfo),TEXT("(%f,%f)-(%f,%f), %dx%d"),
    //            m_Info.X, m_Info.Y, m_Info.GetRight(), m_Info.GetHeight(),
    //            m_Info.Width, m_Info.Height);
    //    }
    //    return m_bufInfo;
    //}

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

    LPCTSTR CFGdiPlusUtil::GetFontFamilyInfo(FTL::CFStringFormater& formater, Gdiplus::FontFamily* pFamily, 
        INT nStyle /* = Gdiplus::FontStyleBoldItalic | Gdiplus::FontStyleUnderline | Gdiplus::FontStyleStrikeout */ )
    {
        FTLASSERT(pFamily);
        if (!pFamily)
        {
            return NULL;
        }

        Gdiplus::Status sts = Gdiplus::Ok;

        WCHAR szFamilyName[LF_FACESIZE] = {0};
        GDIPLUS_VERIFY(pFamily->GetFamilyName(szFamilyName));
        formater.Format(TEXT("FontFamily=%s: "), szFamilyName);

        typedef std::pair<Gdiplus::FontStyle, LPCTSTR> FontStyleInfoPair;
        FontStyleInfoPair checkStyles[] = {
            std::make_pair(Gdiplus::FontStyleRegular, TEXT("Regular")),
            std::make_pair(Gdiplus::FontStyleBold, TEXT("Bold")),
            std::make_pair(Gdiplus::FontStyleItalic, TEXT("Italic")),
            std::make_pair(Gdiplus::FontStyleUnderline, TEXT("Underline")),
            std::make_pair(Gdiplus::FontStyleStrikeout, TEXT("Strikeout"))
        };
        
        for (int i = 0; i < _countof(checkStyles); i++)
        {
            if ((nStyle & checkStyles[i].first) && pFamily->IsStyleAvailable(checkStyles[i].first))
            {
                UINT16 nEmHeight = pFamily->GetEmHeight(checkStyles[i].first);
                GDIPLUS_VERIFY(pFamily->GetLastStatus());

                UINT16 nCellAscent = pFamily->GetCellAscent(checkStyles[i].first);
                GDIPLUS_VERIFY(pFamily->GetLastStatus());

                UINT16 nCellDescent = pFamily->GetCellDescent(checkStyles[i].first);
                GDIPLUS_VERIFY(pFamily->GetLastStatus());

                UINT16 nLineSpacing = pFamily->GetLineSpacing(checkStyles[i].first);
                GDIPLUS_VERIFY(pFamily->GetLastStatus());

                formater.AppendFormat(TEXT("%s{nEmHeight=%d, nCellAscent=%d, nCellDescent=%d, nLineSpacing=%d}, "),
                    checkStyles[i].second, nEmHeight, nCellAscent, nCellDescent, nLineSpacing);
            }
            
        }
        return formater.GetString();
    }
}

#endif //FTL_GDIPLUS_HPP