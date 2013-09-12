#ifndef FTL_GDIPLUS_HPP
#define FTL_GDIPLUS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlGdiPlus.h"
#endif
#include "ftlConversion.h"
#include "ftlGdi.h"

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

	CFAutoGdiplus::CFAutoGdiplus(BOOL bEnableDebug /* = FALSE */)
	{
#if (GDIPVER >= 0x0110) 
		m_gdiplusStartupInput.GdiplusVersion = 2;
#endif //GDIPVER
		if (bEnableDebug)
		{
			m_gdiplusStartupInput.DebugEventCallback = FtlDebugEventProc;
		}
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	}

	CFAutoGdiplus::~CFAutoGdiplus()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	VOID CFAutoGdiplus::FtlDebugEventProc(Gdiplus::DebugEventLevel level, CHAR *message)
	{
		switch (level)
		{
		case Gdiplus::DebugEventLevelWarning:
			FTLTRACEEX(tlWarning, TEXT("Gdi+:%s\n"), CFConversion(CP_ACP).MBCS_TO_TCHAR(message));
			break;
		case Gdiplus::DebugEventLevelFatal:
			FTLTRACEEX(tlError, TEXT("Gdi+:%s\n"), CFConversion(CP_ACP).MBCS_TO_TCHAR(message));
			break;
		}
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

	LPCTSTR CFGdiPlusUtil::GetFontInfo(CFStringFormater& formater, Gdiplus::Font* pFont, Gdiplus::Graphics* pGraphics)
	{
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pFont, NULL);
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pGraphics, NULL);

		Gdiplus::Status sts = Gdiplus::Ok;

		LOGFONT logFont = {0};

		//TODO: Font::GetLogFontX 为什么需要 Graphics 参数?
#ifdef _UNICODE
		GDIPLUS_VERIFY(pFont->GetLogFontW(pGraphics, &logFont));
#else
		GDIPLUS_VERIFY(pFont->GetLogFontA(pGraphics, &logFont));
#endif 	

		CFStringFormater formaterLogFont;
		CFGdiUtil::GetLogFontInfo(formaterLogFont, &logFont);
		
		//获取FontFamily信息
		Gdiplus::FontFamily	fontFamily;
		GDIPLUS_VERIFY(pFont->GetFamily(&fontFamily));
		CFStringFormater formaterFamily;
		GetFontFamilyInfo(formaterFamily, &fontFamily);

		HDC hDCDesktop = ::GetWindowDC(NULL);
		INT nDpiY = ::GetDeviceCaps(hDCDesktop, LOGPIXELSY);
		::ReleaseDC(NULL, hDCDesktop);

		//获取其他信息 -- 理论上说 LOGFONT 已经有全部信息了
		CFStringFormater formaterOtherInfo;
		formaterOtherInfo.Format(
			TEXT("IsAvailable=%d, Style=%d, Size=%f, Unit=%d,")
			TEXT("Height(Graphi)=%f, Height(Dpi)=%f, DPI=%d"),
			pFont->IsAvailable(), pFont->GetStyle(), pFont->GetSize(), pFont->GetUnit(),
            pFont->GetHeight(pGraphics), pFont->GetHeight((Gdiplus::REAL)nDpiY), nDpiY
			);
		
		formater.Format(TEXT("%s, %s%s"), 
			formaterLogFont.GetString(), formaterFamily.GetString(), formaterOtherInfo.GetString());

		return formater.GetString();
	}


	LPCTSTR CFGdiPlusUtil::GetImagePropertyIdString(PROPID id)
	{
		switch (id)
		{
			// Image property ID tags
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifIFD, TEXT("ExifIFD"));					//0x8769
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsIFD, TEXT("GpsIFD"));					//0x8825
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagNewSubfileType, TEXT("NewSubfileType"));	//0x00FE
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSubfileType, TEXT("SubfileType"));			//0x00FF
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagImageWidth, TEXT("ImageWidth"));			//0x0100
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagImageHeight, TEXT("ImageHeight"));			//0x0101
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagBitsPerSample, TEXT("BitsPerSample"));		//0x0102
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagCompression, TEXT("Compression"));			//0x0103
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPhotometricInterp, TEXT("PhotometricInterp"));//0x0106
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThreshHolding, TEXT("ThreshHolding"));		//0x0107
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagCellWidth, TEXT("CellWidth"));				//0x0108
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagCellHeight, TEXT("CellHeight"));			//0x0109
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagFillOrder, TEXT("FillOrder"));				//0x010A
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagDocumentName, TEXT("DocumentName"));		//0x010D
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagImageDescription, TEXT("ImageDescription"));//0x010E
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagEquipMake, TEXT("EquipMake"));				//照相机制造商
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagEquipModel, TEXT("EquipModel"));			//照相机型号
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagStripOffsets, TEXT("StripOffsets"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagOrientation, TEXT("Orientation"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSamplesPerPixel, TEXT("SamplesPerPixel"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagRowsPerStrip, TEXT("RowsPerStrip"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagStripBytesCount, TEXT("StripBytesCount"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagMinSampleValue, TEXT("MinSampleValue"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagMaxSampleValue, TEXT("MaxSampleValue"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagXResolution, TEXT("XResolution"));			//水平分辨率
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagYResolution, TEXT("YResolution"));			//垂直分辨率
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPlanarConfig, TEXT("PlanarConfig"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPageName, TEXT("PageName"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagXPosition, TEXT("XPosition"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagYPosition, TEXT("YPosition"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagFreeOffset, TEXT("FreeOffset"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagFreeByteCounts, TEXT("FreeByteCounts"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGrayResponseUnit, TEXT("GrayResponseUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGrayResponseCurve, TEXT("GrayResponseCurve"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagT4Option, TEXT("T4Option"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagT6Option, TEXT("T6Option"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagResolutionUnit, TEXT("ResolutionUnit"));			//分辨率单位(2 -- ?)
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPageNumber, TEXT("PageNumber"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTransferFuncition, TEXT("TransferFuncition"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSoftwareUsed, TEXT("SoftwareUsed"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagDateTime, TEXT("DateTime"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagArtist, TEXT("Artist"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHostComputer, TEXT("HostComputer"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPredictor, TEXT("Predictor"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagWhitePoint, TEXT("WhitePoint"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPrimaryChromaticities, TEXT("PrimaryChromaticities"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagColorMap, TEXT("ColorMap"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneHints, TEXT("HalftoneHints"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTileWidth, TEXT("TileWidth"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTileLength, TEXT("TileLength"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTileOffset, TEXT("TileOffset"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTileByteCounts, TEXT("TileByteCounts"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagInkSet, TEXT("InkSet"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagInkNames, TEXT("InkNames"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagNumberOfInks, TEXT("NumberOfInks"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagDotRange, TEXT("DotRange"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTargetPrinter, TEXT("TargetPrinter"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExtraSamples, TEXT("ExtraSamples"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSampleFormat, TEXT("SampleFormat"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSMinSampleValue, TEXT("SMinSampleValue"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSMaxSampleValue, TEXT("SMaxSampleValue"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTransferRange, TEXT("TransferRange"));

			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGProc, TEXT("JPEGProc"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGInterFormat, TEXT("JPEGInterFormat"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGInterLength, TEXT("JPEGInterLength"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGRestartInterval, TEXT("JPEGRestartInterval"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGLosslessPredictors, TEXT("JPEGLosslessPredictors"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGPointTransforms, TEXT("JPEGPointTransforms"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGQTables, TEXT("JPEGQTables"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGDCTables, TEXT("JPEGDCTables"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGACTables, TEXT("JPEGACTables"));

			HANDLE_CASE_RETURN_STRING_EX(PropertyTagYCbCrCoefficients, TEXT("YCbCrCoefficients"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagYCbCrSubsampling, TEXT("YCbCrSubsampling"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagYCbCrPositioning, TEXT("YCbCrPositioning"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagREFBlackWhite, TEXT("REFBlackWhite"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagICCProfile, TEXT("ICCProfile"));
			// for embedded ICC in TIFF
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGamma, TEXT("Gamma"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagICCProfileDescriptor, TEXT("ICCProfileDescriptor"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagSRGBRenderingIntent, TEXT("SRGBRenderingIntent"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagImageTitle, TEXT("ImageTitle"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagCopyright, TEXT("Copyright"));

			// Extra TAGs (Like Adobe Image Information tags etc.)
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagResolutionXUnit, TEXT("ResolutionXUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagResolutionYUnit, TEXT("ResolutionYUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagResolutionXLengthUnit, TEXT("ResolutionXLengthUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagResolutionYLengthUnit, TEXT("ResolutionYLengthUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPrintFlags, TEXT("PrintFlags"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPrintFlagsVersion, TEXT("PrintFlagsVersion"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPrintFlagsCrop, TEXT("PrintFlagsCrop"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPrintFlagsBleedWidth, TEXT("PrintFlagsBleedWidth"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPrintFlagsBleedWidthScale, TEXT("PrintFlagsBleedWidthScale"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneLPI, TEXT("HalftoneLPI"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneLPIUnit, TEXT("HalftoneLPIUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneDegree, TEXT("HalftoneDegree"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneShape, TEXT("HalftoneShape"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneMisc, TEXT("HalftoneMisc"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagHalftoneScreen, TEXT("HalftoneScreen"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagJPEGQuality, TEXT("JPEGQuality"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGridSize, TEXT("GridSize"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailFormat, TEXT("ThumbnailFormat"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailWidth, TEXT("ThumbnailWidth"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailHeight, TEXT("ThumbnailHeight"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailColorDepth, TEXT("ThumbnailColorDepth"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailPlanes, TEXT("ThumbnailPlanes"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailRawBytes, TEXT("ThumbnailRawBytes"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailSize, TEXT("ThumbnailSize"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailCompressedSize, TEXT("ThumbnailCompressedSize"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagColorTransferFunction, TEXT("ColorTransferFunction"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailData, TEXT("ThumbnailData"));	// RAW thumbnail bits in

			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailImageWidth, TEXT("ThumbnailImageWidth"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailImageHeight, TEXT("ThumbnailImageHeight"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailBitsPerSample, TEXT("ThumbnailBitsPerSample"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailCompression, TEXT("ThumbnailCompression"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailPhotometricInterp, TEXT("ThumbnailPhotometricInterp"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailImageDescription, TEXT("ThumbnailImageDescription"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailEquipMake, TEXT("ThumbnailEquipMake"));
			// Input equipment
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailEquipModel, TEXT("ThumbnailEquipModel"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailStripOffsets, TEXT("ThumbnailStripOffsets"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailOrientation, TEXT("ThumbnailOrientation"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailSamplesPerPixel, TEXT("ThumbnailSamplesPerPixel"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailRowsPerStrip, TEXT("ThumbnailRowsPerStrip"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailStripBytesCount, TEXT("ThumbnailStripBytesCount"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailResolutionX, TEXT("ThumbnailResolutionX"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailResolutionY, TEXT("ThumbnailResolutionY"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailPlanarConfig, TEXT("ThumbnailPlanarConfig"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailResolutionUnit, TEXT("ThumbnailResolutionUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailTransferFunction, TEXT("ThumbnailTransferFunction"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailSoftwareUsed, TEXT("ThumbnailSoftwareUsed"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailDateTime, TEXT("ThumbnailDateTime"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailArtist, TEXT("ThumbnailArtist"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailWhitePoint, TEXT("ThumbnailWhitePoint"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailPrimaryChromaticities, TEXT("ThumbnailPrimaryChromaticities"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailYCbCrCoefficients, TEXT("ThumbnailYCbCrCoefficients"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailYCbCrSubsampling, TEXT("ThumbnailYCbCrSubsampling"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailYCbCrPositioning, TEXT("ThumbnailYCbCrPositioning"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailRefBlackWhite, TEXT("ThumbnailRefBlackWhite"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagThumbnailCopyRight, TEXT("ThumbnailCopyRight"));

			HANDLE_CASE_RETURN_STRING_EX(PropertyTagLuminanceTable, TEXT("LuminanceTable"));				//亮度表
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagChrominanceTable, TEXT("ChrominanceTable"));			//色度表
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagFrameDelay, TEXT("FrameDelay"));						
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagLoopCount, TEXT("LoopCount"));
#if (GDIPVER >= 0x0110)
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGlobalPalette, TEXT("GlobalPalette"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagIndexBackground, TEXT("IndexBackground"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagIndexTransparent, TEXT("IndexTransparent"));
#endif //(GDIPVER >= 0x0110)
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPixelUnit, TEXT("PixelUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPixelPerUnitX, TEXT("PixelPerUnitX"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPixelPerUnitY, TEXT("PixelPerUnitY"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagPaletteHistogram, TEXT("PaletteHistogram"));

			//EXIF specific tag
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifExposureTime, TEXT("ExifExposureTime"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFNumber, TEXT("ExifFNumber"));					//光圈值(如 0012/0005 表示 f/2.4)
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifExposureProg, TEXT("ExifExposureProg"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSpectralSense, TEXT("ExifSpectralSense"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifISOSpeed, TEXT("ExifISOSpeed"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifOECF, TEXT("ExifOECF"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifVer, TEXT("ExifVer"));							//类型是 Undefined(0220)
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDTOrig, TEXT("ExifDTOrig"));					//拍摄时间
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDTDigitized, TEXT("ExifDTDigitized"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifCompConfig, TEXT("ExifCompConfig"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifCompBPP, TEXT("ExifCompBPP"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifShutterSpeed, TEXT("ExifShutterSpeed"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifAperture, TEXT("ExifAperture"));				//? -- 3640/1441
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifBrightness, TEXT("ExifBrightness"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifExposureBias, TEXT("ExifExposureBias"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifMaxAperture, TEXT("ExifMaxAperture"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSubjectDist, TEXT("ExifSubjectDist"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifMeteringMode, TEXT("ExifMeteringMode"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifLightSource, TEXT("ExifLightSource"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFlash, TEXT("ExifFlash"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFocalLength, TEXT("ExifFocalLength"));			//? -- 0431/0100
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSubjectArea, TEXT("ExifSubjectArea"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifMakerNote, TEXT("ExifMakerNote"));				//Undeined
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifUserComment, TEXT("ExifUserComment"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDTSubsec, TEXT("ExifDTSubsec"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDTOrigSS, TEXT("ExifDTOrigSS"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDTDigSS, TEXT("ExifDTDigSS"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFPXVer, TEXT("ExifFPXVer"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifColorSpace, TEXT("ExifColorSpace"));			//
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifPixXDim, TEXT("ExifPixXDim"));					//宽度, 如 3200 像素
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifPixYDim, TEXT("ExifPixYDim"));					//高度, 如 2400 像素
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifRelatedWav, TEXT("ExifRelatedWav"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifInterop, TEXT("ExifInterop"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFlashEnergy, TEXT("ExifFlashEnergy"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSpatialFR, TEXT("ExifSpatialFR"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFocalXRes, TEXT("ExifFocalXRes"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFocalYRes, TEXT("ExifFocalYRes"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFocalResUnit, TEXT("ExifFocalResUnit"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSubjectLoc, TEXT("ExifSubjectLoc"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifExposureIndex, TEXT("ExifExposureIndex"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSensingMethod, TEXT("ExifSensingMethod"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFileSource, TEXT("ExifFileSource"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSceneType, TEXT("ExifSceneType"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifCfaPattern, TEXT("ExifCfaPattern"));

			// New EXIF 2.2 properties
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifCustomRendered, TEXT("ExifCustomRendered"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifExposureMode, TEXT("ExifExposureMode"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifWhiteBalance, TEXT("ExifWhiteBalance"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDigitalZoomRatio, TEXT("ExifDigitalZoomRatio"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifFocalLengthIn35mmFilm, TEXT("ExifFocalLengthIn35mmFilm"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSceneCaptureType, TEXT("ExifSceneCaptureType"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifGainControl, TEXT("ExifGainControl"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifContrast, TEXT("ExifContrast"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSaturation, TEXT("ExifSaturation"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSharpness, TEXT("ExifSharpness"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifDeviceSettingDesc, TEXT("ExifDeviceSettingDesc"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifSubjectDistanceRange, TEXT("ExifSubjectDistanceRange"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifUniqueImageID, TEXT("ExifUniqueImageID"));

			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsVer, TEXT("GpsVer"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsLatitudeRef, TEXT("GpsLatitudeRef"));				//ASCII -- N
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsLatitude, TEXT("GpsLatitude"));						//维度，如 30/1,34/1,23104/1000 表示"30; 34; 23.104"
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsLongitudeRef, TEXT("GpsLongitudeRef"));				//ASCII -- E
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsLongitude, TEXT("GpsLongitude"));					//经度，如 104/1, 3/1, 56143/1000 表示 "104; 3; 56.143"
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsAltitudeRef, TEXT("GpsAltitudeRef"));				//高度标识?
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsAltitude, TEXT("GpsAltitude"));						//高度，如 454299/1000 表示 "454.299"
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsGpsTime, TEXT("GpsGpsTime"));						//时间(似乎是GMT)？如 11/1,36/1,39/1，Windows的属性中未显示
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsGpsSatellites, TEXT("GpsGpsSatellites"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsGpsStatus, TEXT("GpsGpsStatus"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsGpsMeasureMode, TEXT("GpsGpsMeasureMode"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsGpsDop, TEXT("GpsGpsDop"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsSpeedRef, TEXT("GpsSpeedRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsSpeed, TEXT("GpsSpeed"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsTrackRef, TEXT("GpsTrackRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsTrack, TEXT("GpsTrack"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsImgDirRef, TEXT("GpsImgDirRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsImgDir, TEXT("GpsImgDir"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsMapDatum, TEXT("GpsMapDatum"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestLatRef, TEXT("GpsDestLatRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestLat, TEXT("GpsDestLat"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestLongRef, TEXT("GpsDestLongRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestLong, TEXT("GpsDestLong"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestBearRef, TEXT("GpsDestBearRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestBear, TEXT("GpsDestBear"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestDistRef, TEXT("GpsDestDistRef"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDestDist, TEXT("GpsDestDist"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsProcessingMethod, TEXT("GpsProcessingMethod"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsAreaInformation, TEXT("GpsAreaInformation"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDate, TEXT("GpsDate"));							//ASCII, 如 2013:05:09
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagGpsDifferential, TEXT("GpsDifferential"));
			//HANDLE_CASE_RETURN_STRING_EX(xxxxx, TEXT("xxxxx"));


			//Gdiplus 中没有定义的Tag -- http://www.exiv2.org/tags.html
#ifndef PropertyTagRating							//(Short)Rating tag used by Windows
#  define PropertyTagRating	0x4746					
#endif 
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagRating, TEXT("Rating"));

#ifndef PropertyTagRatingPercent					//(Short)Rating tag used by Windows, value in percent
#  define PropertyTagRatingPercent	0x4749			
#endif 
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagRatingPercent, TEXT("RatingPercent"));

#ifndef PropertyTagExifInteroperabilityVersion	
#  define PropertyTagExifInteroperabilityVersion	0x5042
#endif 
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagExifInteroperabilityVersion, TEXT("ExifInteroperabilityVersion"));

#ifndef PropertyTagXPTitle							//(BYTE)Title tag used by Windows, encoded in UCS2
#  define PropertyTagXPTitle	0x9c9b
#endif 
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagXPTitle, TEXT("XPTitle"));



			default:
				FTLTRACEEX(tlWarning, TEXT("Warning: Unknown Property Tag Id for 0x%x\n"), id);
				FTLASSERT(FALSE && TEXT("Unknown Property Tag Id"));
				break;
		}
		return TEXT("Unknown");
	}

	LPCTSTR CFGdiPlusUtil::GetImagePropertyTagTypeString(WORD type)
	{
		switch (type)
		{
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeByte, TEXT("Byte"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeASCII, TEXT("ASCII"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeShort, TEXT("Short"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeLong, TEXT("Long"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeRational, TEXT("Rational"));
			//缺少6
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeUndefined, TEXT("Undefined"));
			//缺少8
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeSLONG, TEXT("SLONG"));
			HANDLE_CASE_RETURN_STRING_EX(PropertyTagTypeSRational, TEXT("SRational"));
		default:
			FTLASSERT(FALSE && TEXT("Not Found PropertyItem::type"));
			break;
		}
		return TEXT("Unknown");
	}

	LPCTSTR CFGdiPlusUtil::GetGdiplusPropertyItemString(const Gdiplus::PropertyItem* pItem, CFStringFormater& formater)
	{
		FTLASSERT(pItem);
		if (pItem)
		{
			formater.Format(TEXT("id=0x%x(%s), length=%d, type=%d(%s),"),
				pItem->id, GetImagePropertyIdString(pItem->id),
				pItem->length, pItem->type, 
				GetImagePropertyTagTypeString(pItem->type));

			//根据类型获取具体的值
			switch (pItem->type)
			{
			case PropertyTagTypeByte:
				{
					//最多只打印100个字节的数据
					ULONG nPrintLen = FTL_MIN( pItem->length, 100);
					formater.AppendFormat(TEXT("value[0-%d]="), nPrintLen);
					for(ULONG i = 0; i < nPrintLen; ++i)
					{
						formater.AppendFormat(TEXT("0x%x,"), ((byte*)pItem->value)[i]);		
					}
				}
				break;
			case PropertyTagTypeASCII:
				{
					//ASCII时长度是包含结束时的NULL，因此可以直接转换
					formater.AppendFormat(TEXT("value=%s"), CFConversion(CP_UTF8).UTF8_TO_TCHAR((const char*)pItem->value));
				}
				break;
			case PropertyTagTypeShort:
				{
					formater.AppendFormat(TEXT("value="));
					for( ULONG i=0; i<pItem->length / 2; ++i )
					{
						formater.AppendFormat(TEXT("%d,"), ((unsigned short*)pItem->value)[i]);
					}
				}
				break;
			case PropertyTagTypeLong:
				{
					formater.AppendFormat(TEXT("value="));
					for( ULONG i=0; i<pItem->length / 4; ++i )
					{
						formater.AppendFormat(TEXT("%d,"), ((unsigned long*)pItem->value)[i]);
					}
				}
				break;
			case PropertyTagTypeRational:
				{
					formater.AppendFormat(TEXT("value="));
					for( ULONG i=0; i <pItem->length / 8; ++i )
					{
						formater.AppendFormat(TEXT("%d/%d,"), 
							((unsigned long*)pItem->value)[2*i + 0], 
							((unsigned long*)pItem->value)[2*i + 1]);
					}
				}
				break;
			case PropertyTagTypeUndefined:
				{
					//最多只打印100个字节的数据
					ULONG nPrintLen = FTL_MIN( pItem->length, 100);
					formater.AppendFormat(TEXT("value[0-%d]="), nPrintLen);
					for(ULONG i = 0; i < nPrintLen; ++i)
					{
						formater.AppendFormat(TEXT("0x%x,"), ((byte*)pItem->value)[i]);		
					}
				}
				break;
			case PropertyTagTypeSLONG:
				{
					formater.AppendFormat(TEXT("value="));
					for( ULONG i=0; i<pItem->length / 4; ++i )
					{
						formater.AppendFormat(TEXT("%d,"), ((signed long*)pItem->value)[i]);
					}
				}
				break;
			case PropertyTagTypeSRational:
				{
					formater.AppendFormat(TEXT("value="));
					for( ULONG i=0; i <pItem->length / 8; ++i )
					{
						formater.AppendFormat(TEXT("%d/%d,"), 
							((signed long*)pItem->value)[2*i + 0], 
							((signed long*)pItem->value)[2*i + 1]);
					}
				}
				break;
			default:
				FTLASSERT(FALSE && TEXT("Not Found PropertyItem::type"));
				break;
			}
			return formater.GetString();
		}
		return NULL;
	}

	Gdiplus::Status CFGdiPlusUtil::GetImageProperty(LPCTSTR pszImagePath, CFStringFormater& formater)
	{
		Gdiplus::Status sts = Gdiplus::InvalidParameter;
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(CT2W(pszImagePath));
		if (pImage)
		{
			GDIPLUS_VERIFY(pImage->GetLastStatus());

			UINT nTotalBufferSize = 0;
			UINT nNumProperties = 0;
			GDIPLUS_VERIFY(pImage->GetPropertySize(&nTotalBufferSize, &nNumProperties));
			Gdiplus::PropertyItem* pAllItems = (Gdiplus::PropertyItem*)new BYTE[nTotalBufferSize];// (Gdiplus::PropertyItem*)operator new( nTotalBufferSize ); 
			if (pAllItems)
			{
				ZeroMemory(pAllItems, nTotalBufferSize);
				GDIPLUS_VERIFY(pImage->GetAllPropertyItems(nTotalBufferSize, nNumProperties, pAllItems));
				for (UINT i=0; i< nNumProperties; ++i )
				{
					const Gdiplus::PropertyItem& item = pAllItems[i];
					CFStringFormater itemFormater;
					formater.AppendFormat(TEXT("index=%d, item=[%s]\n"),
						i, GetGdiplusPropertyItemString(&item, itemFormater));
				}
				//operator delete(pAllItems);
				delete [] pAllItems;
			}
			delete pImage;
		}

		return sts;
	}

	Gdiplus::Status  CFGdiPlusUtil::GetImageSaveEncoder(LPCWSTR pszFileExt, OUT CLSID* pClsidEncoder)
	{
		FTLASSERT(pszFileExt);
		FTLASSERT(pClsidEncoder);
		if (!pszFileExt || !pClsidEncoder)
		{
			return Gdiplus::InvalidParameter;
		}

		*pClsidEncoder = CLSID_NULL;

		Gdiplus::Status sts = Gdiplus::InvalidParameter;
		UINT nEncoders = 0;
		UINT nBytes  = 0;
		GDIPLUS_VERIFY(Gdiplus::GetImageEncodersSize( &nEncoders, &nBytes ));
		if (Gdiplus::Ok == sts)
		{
			Gdiplus::ImageCodecInfo* pEncoders = (Gdiplus::ImageCodecInfo*)(new BYTE[nBytes]);
			if (pEncoders)
			{
				GDIPLUS_VERIFY(Gdiplus::GetImageEncoders( nEncoders, nBytes, pEncoders ));

				BOOL bFound = FALSE;
				for( UINT iCodec = 0; iCodec < nEncoders && !bFound; iCodec++ )
				{
					CStringW strExtensions( pEncoders[iCodec].FilenameExtension );

					int iStart = 0;
					do
					{
						CStringW strExtension = ::PathFindExtensionW( strExtensions.Tokenize( L";", iStart ) );
						if( iStart != -1 )
						{
							if( strExtension.CompareNoCase( pszFileExt ) == 0 )
							{
								*pClsidEncoder = pEncoders[iCodec].Clsid;
								bFound = TRUE;
								sts = Gdiplus::Ok;
								break;
							}
						}
					} while( iStart != -1 );
				}

				delete [] pEncoders;
			}
			else
			{
				sts = Gdiplus::OutOfMemory;
			}
		}
		return sts;
	}

	Gdiplus::Color CFGdiPlusUtil::ConvertColor(COLORREF clrGdi, BOOL bSupportAlpha /* = FALSE */)
	{
		BYTE bAlpha = GetAValue(clrGdi);
		BYTE bRed = GetRValue(clrGdi);
		BYTE bGreen = GetGValue(clrGdi);
		BYTE bBlue = GetBValue(clrGdi);
		if (!bSupportAlpha)
		{
			bAlpha = 0xFF;
		}
		return Gdiplus::Color::MakeARGB(bAlpha, bRed, bGreen, bBlue);	
		
	}

	COLORREF CFGdiPlusUtil::ConvertColor(Gdiplus::Color& clrGdiPlus, BOOL bSupportAlpha /* = FALSE */)
	{
		BYTE bAlpha = clrGdiPlus.GetAlpha();
		BYTE bRed = clrGdiPlus.GetRed();
		BYTE bGreen = clrGdiPlus.GetGreen();
		BYTE bBlue = clrGdiPlus.GetBlue();
        if (!bSupportAlpha)
        {
            bAlpha = 0xFF;
        }
		return MAKE_RGBA(bRed, bGreen, bBlue, bAlpha);
	}

	struct UnitConvertMatrix
	{
		Gdiplus::Unit	uniFrom;
		Gdiplus::Unit	unitTo;

		Gdiplus::REAL	mulValue1;
		Gdiplus::REAL   mulValue2;
		Gdiplus::REAL   divValue1;
		Gdiplus::REAL   divValue2;
	};

	BOOL CFGdiPlusUtil::ConvertUnitCoordinate(IN Gdiplus::REAL dpi, IN Gdiplus::Unit unitFrom, IN Gdiplus::REAL valueFrom, 
		IN Gdiplus::Unit unitTo, OUT Gdiplus::REAL& valueTo)
	{
		
		FTLASSERT(unitFrom >= Gdiplus::UnitPixel);
		FTLASSERT(unitTo >= Gdiplus::UnitPixel);

		if (unitFrom == unitTo)
		{
			valueTo = valueFrom;
			return TRUE;
		}

		BOOL bRet = FALSE;

		const Gdiplus::REAL mm_per_inch = 25.4f;
		//http://en.wikipedia.org/wiki/Pixels_per_inch
		const Gdiplus::REAL inch_per_point = 1.0f/72.0f; //TODO: PPI 这个值可能不是定值
		const Gdiplus::REAL doc_per_inch = 300.0f;
		UnitConvertMatrix convertMatrix[] = 
		{
			//wine的源码中找出来的 -- http://www.winehq.org/download/

			//http://www.winehq.org/pipermail/wine-patches/2012-July/115857.html
			{ Gdiplus::UnitPoint, Gdiplus::UnitPixel, dpi, inch_per_point, 1.0f, 1.0f },			//units * dpi * inch_per_point;
			{ Gdiplus::UnitInch, Gdiplus::UnitPixel, dpi, 1.0f, 1.0f, 1.0f },						//units * dpi;
			{ Gdiplus::UnitDocument, Gdiplus::UnitPixel, dpi, 1.0f, doc_per_inch, 1.0f },			//units * dpi / 300.0;
			{ Gdiplus::UnitMillimeter, Gdiplus::UnitPixel, dpi, 1.0f, mm_per_inch, 1.0f },			//units * dpi / mm_per_inch;
		};

		for (int i = 0; i < _countof(convertMatrix); i++)
		{
			if (convertMatrix[i].uniFrom == unitFrom
				&& convertMatrix[i].unitTo == unitTo)
			{
				valueTo = valueFrom * convertMatrix[i].mulValue1 * convertMatrix[i].mulValue2 / convertMatrix[i].divValue2 / convertMatrix[i].divValue2;
				bRet = TRUE;
				break;
			}
		}

		return bRet;
	}
}

#endif //FTL_GDIPLUS_HPP