#ifndef FTL_GDI_HPP
#define FTL_GDI_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlGdi.h"
#endif

namespace FTL
{
    CFGdiObjectInfoDump::CFGdiObjectInfoDump()
    {
        m_pszGdiObjectInfo = NULL;
    }

    CFGdiObjectInfoDump::~CFGdiObjectInfoDump()
    {
        SAFE_DELETE_ARRAY(m_pszGdiObjectInfo);
    }

    LPCTSTR CFGdiObjectInfoDump::GetGdiObjectInfoString()
    {
        return m_pszGdiObjectInfo;
    }

    BOOL CFGdiObjectInfoDump::GetGdiObjectInfo(HGDIOBJ hgdiObj)
    {
        BOOL bRet = FALSE;
        SAFE_DELETE_ARRAY(m_pszGdiObjectInfo);

        CFStringFormater formater;
        DWORD objType = ::GetObjectType(hgdiObj);
        switch(objType)
        {
        case 0:
            FTLTRACEEX(FTL::tlWarning, TEXT("GetGdiObjectInfo Call GetObjectType(0x%p) Failed, Maybe Already Call DeletedObject!!!\n"));
            FTLASSERT(ERROR_INSUFFICIENT_BUFFER == GetLastError());
            break;
        case OBJ_PEN:
            {
                LOGPEN penInfo = {0};
                API_VERIFY(::GetObject(hgdiObj, sizeof(penInfo), &penInfo));
                formater.Format(TEXT("Pen::style=%d,width=[%d,%d], color=0x%08x"), 
                    penInfo.lopnStyle, penInfo.lopnWidth.x, penInfo.lopnWidth.y,penInfo.lopnColor);
                break;
            }
        case OBJ_BRUSH:
            {
                LOGBRUSH brushInfo = {0};
                API_VERIFY(::GetObject(hgdiObj, sizeof(brushInfo), &brushInfo));
                formater.Format(TEXT("Brush::style=%d,color=0x%08x, hatch=0x%p"), 
                    brushInfo.lbStyle, brushInfo.lbColor, brushInfo.lbHatch);
                break;
            }
        case OBJ_DC:
            {
                formater.Format(TEXT("%s"), TEXT("DC"));
                break;
            }
        case OBJ_METADC:
            {
                formater.Format(TEXT("%s"), TEXT("MetaDC"));
                break;
            }
        case OBJ_PAL:
            {
                WORD PalentrieCount = 0;
                ::GetObject(hgdiObj, sizeof(PalentrieCount), &PalentrieCount);
                formater.Format(TEXT("Palette::Entries Count=%d"), PalentrieCount);
                break;
            }
        case OBJ_FONT:
            {
                LOGFONT fontInfo = {0};
                API_VERIFY(::GetObject(hgdiObj, sizeof(fontInfo), &fontInfo));
                formater.Format(TEXT("Font::Height=%d,Width=%d, Escapement=%d,")
                    TEXT("Orientation=%d,Weight=%d, Italic=%d,") 
                    TEXT("Underline=%d,StrikeOut=%d, CharSet=%d,") 
                    TEXT("OutPrecision=%d,ClipPrecision=%d,") 
                    TEXT("Quality=%d,PitchAndFamily=%d,")
                    TEXT("FaceName=%s"),
                    fontInfo.lfHeight, fontInfo.lfWidth,fontInfo.lfEscapement,
                    fontInfo.lfOrientation, fontInfo.lfWeight,fontInfo.lfItalic,
                    fontInfo.lfUnderline,fontInfo.lfStrikeOut,fontInfo.lfCharSet,
                    fontInfo.lfOutPrecision, fontInfo.lfClipPrecision,
                    fontInfo.lfQuality, fontInfo.lfPitchAndFamily,
                    fontInfo.lfFaceName);
                break;
            }
        case OBJ_BITMAP:
            {
                DIBSECTION dibInfo = {0};
                API_VERIFY(::GetObject(hgdiObj, sizeof(dibInfo), &dibInfo));

                formater.Format(
                    TEXT("%s::BITMAP[Type=%d,Width=%d,Height=%d,WidthBytes=%d,")
                    TEXT("Planes=%d,BitsPixel=%d]"),
                    dibInfo.dsBmih.biSize > 0 ? TEXT("DIB") : TEXT("Bitmap"),
                    dibInfo.dsBm.bmType,dibInfo.dsBm.bmWidth,dibInfo.dsBm.bmHeight,dibInfo.dsBm.bmWidthBytes,
                    dibInfo.dsBm.bmPlanes, dibInfo.dsBm.bmBitsPixel
                    );
                if (0 < dibInfo.dsBmih.biSize)
                {
                    formater.AppendFormat(TEXT(",BITMAPINFOHEADER[biSize=%d,biWidth=%ld,biHeight=%ld,")
                        TEXT("biPlanes=%d,biBitCount=%d,biCompression=%d,")
                        TEXT("biSizeImage=%d,biXPelsPerMeter=%ld,biYPelsPerMeter=%ld,")
                        TEXT("biClrUsed=%d,biClrImportant=%d,")
                        TEXT("dsBitfields=[%d,%d,%d],")
                        TEXT("dshSection=0x%p,dsOffset=%d"),
                        dibInfo.dsBmih.biSize,dibInfo.dsBmih.biWidth, dibInfo.dsBmih.biHeight,
                        dibInfo.dsBmih.biPlanes, dibInfo.dsBmih.biBitCount,dibInfo.dsBmih.biCompression,
                        dibInfo.dsBmih.biSizeImage, dibInfo.dsBmih.biXPelsPerMeter, dibInfo.dsBmih.biYPelsPerMeter,
                        dibInfo.dsBmih.biClrUsed, dibInfo.dsBmih.biClrImportant,
                        dibInfo.dsBitfields[0], dibInfo.dsBitfields[1], dibInfo.dsBitfields[2],
                        dibInfo.dshSection, dibInfo.dsOffset);
                }
                else
                {
                    const DWORD OtherInfoSize = sizeof(DIBSECTION) - sizeof(BITMAP);
                    BYTE dibSecInfo[OtherInfoSize] = {0};
                    FTLASSERT(memcmp(&dibInfo.dsBmih, dibSecInfo, OtherInfoSize) == 0);
                }
                break;
            }
        case OBJ_REGION:
            {
                FTLASSERT(FALSE);
                break;
            }
        case OBJ_METAFILE:
            {
                FTLASSERT(FALSE);
                break;
            }
        case OBJ_MEMDC:
            {
                FTLASSERT(FALSE);
                break;
            }
        case OBJ_EXTPEN:
            {
                EXTLOGPEN extPenInfo = {0};
                API_VERIFY(::GetObject(hgdiObj, sizeof(extPenInfo), &extPenInfo));
                formater.Format(TEXT("ExtPen::PenStyle=%d,Width=%d, BrushStyle=%d")
                    TEXT("Color=0x%08x,Hatch=%d, NumEntries=%d"),
                    extPenInfo.elpPenStyle,extPenInfo.elpWidth,extPenInfo.elpBrushStyle,
                    extPenInfo.elpColor, extPenInfo.elpHatch,extPenInfo.elpNumEntries);
                break;
            }
        case OBJ_ENHMETADC:
            {
                FTLASSERT(FALSE);
                break;
            }
        case OBJ_ENHMETAFILE:
            {
                FTLASSERT(FALSE);
                break;
            }
        case OBJ_COLORSPACE:
            {
                FTLASSERT(FALSE);
                break;
            }
        default:
            FTLASSERT(FALSE);
            break;
        }

        if (formater.GetStringLength() > 0)
        {
            m_pszGdiObjectInfo = formater.Detach();
            return TRUE;
        }
        return FALSE;
    }

    CFGdiObjectChecker::CFGdiObjectChecker(HDC hDCTarget /* = NULL */)
    {
        m_hDCCompatible = ::CreateCompatibleDC(hDCTarget);
        API_ASSERT(NULL != m_hDCCompatible);
    }

    CFGdiObjectChecker::~CFGdiObjectChecker()
    {
        BOOL bRet = FALSE;
        API_VERIFY(::DeleteDC(m_hDCCompatible));
    }

    BOOL CFGdiObjectChecker::IsValidate(HGDIOBJ hGdiObj)
    {
        BOOL bRet = FALSE;
        CFGdiObjectInfoDump objInfoDumper;
        bRet = objInfoDumper.GetGdiObjectInfo(hGdiObj);
        return bRet;
    }

    BOOL CFGdiObjectChecker::IsSelected(HBITMAP hBmp)
    {
        BOOL bRet = FALSE;
        FTLASSERT(IsValidate(hBmp));
        bRet = IsValidate(hBmp);
        if (bRet)
        {
            HBITMAP hOldBmp = (HBITMAP)::SelectObject(m_hDCCompatible, (HGDIOBJ)hBmp);

            bRet = (NULL == hOldBmp);
            if (hOldBmp)
            {
                HBITMAP hOriginalBmp = (HBITMAP)::SelectObject(m_hDCCompatible, (HGDIOBJ)hOldBmp);
                API_ASSERT(hOriginalBmp == hBmp);
            }
        }
        return bRet;
    }

	BOOL CFGdiUtil::DrawCoordinate(HDC hdc, const RECT& rcClient, BOOL bDrawClipbox)
	{
		BOOL bRet = FALSE;

		HBRUSH hOldBrush  = (HBRUSH)::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
		FTLASSERT(HGDI_ERROR != hOldBrush);
		HPEN hOldPen = (HPEN)::SelectObject(hdc, ::GetStockObject(BLACK_PEN));
		FTLASSERT(HGDI_ERROR != hOldPen);
		POINT ptOldPos = {0};
		API_VERIFY(::GetCurrentPositionEx(hdc, &ptOldPos));
		UINT nOldTextAlign = ::GetTextAlign(hdc);
		API_ASSERT(GDI_ERROR != nOldTextAlign);
		int nOldBkMode = ::SetBkMode(hdc, TRANSPARENT);
		API_ASSERT(0 != nOldBkMode);

		CFStringFormater formater;


		POINT ptWindowOrg = { 0 };
		SIZE szWindowExt = { 0 };
		API_VERIFY(::GetWindowOrgEx(hdc, &ptWindowOrg));
		API_VERIFY(::GetWindowExtEx(hdc, &szWindowExt));

		POINT ptViewportOrg = { 0 };
		SIZE szViewportExt = { 0 };
		API_VERIFY(::GetViewportOrgEx(hdc, &ptViewportOrg));
		API_VERIFY(::GetViewportExtEx(hdc, &szViewportExt));

		int nMapMode = 0;
		API_VERIFY( 0 != (nMapMode = ::GetMapMode(hdc)));
		switch(nMapMode)
		{
		case MM_TEXT:

			break;
		}

		RECT rcLPClient = rcClient;
		//API_VERIFY(::DPtoLP(hdc,(PPOINT)&rcLPClient, 2));
		FTLASSERT(rcLPClient.left <= rcLPClient.right);
		//FTLASSERT(rcLPClient.top <= rcLPClient.bottom);

		int xCenter = (rcLPClient.left + rcLPClient.right) / 2;
		int yCenter = (rcLPClient.top + rcLPClient.bottom) / 2;

		API_VERIFY(::Rectangle(hdc, rcLPClient.left, rcLPClient.top, rcLPClient.right, rcLPClient.bottom));
		API_VERIFY(::MoveToEx(hdc, rcLPClient.left , yCenter, NULL));
		API_VERIFY(::LineTo(hdc, rcLPClient.right, yCenter));
		API_VERIFY(::MoveToEx(hdc, xCenter, rcLPClient.top, NULL));
		API_VERIFY(::LineTo(hdc, xCenter, rcLPClient.bottom));


		//(0,0) and center
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_CENTER|TA_BASELINE));
		formater.Format(TEXT("O[0,0]"));
		API_VERIFY(::TextOut(hdc, 0, 0, formater.GetString(), formater.GetStringLength()));


		//LeftTop
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_LEFT|TA_TOP));
		formater.Format(TEXT("LT[%d,%d]"), rcLPClient.left, rcLPClient.top);
		API_VERIFY(::TextOut(hdc, rcLPClient.left, rcLPClient.top, formater.GetString() , formater.GetStringLength()));

		//CenterTop
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_CENTER|TA_TOP));
		formater.Format(TEXT("T[%d,%d]"), xCenter, rcLPClient.top);
		API_VERIFY(::TextOut(hdc, xCenter, rcLPClient.top, formater.GetString() , formater.GetStringLength()));

		//RightTop
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_RIGHT|TA_TOP));
		formater.Format(TEXT("RT[%d,%d]"), rcLPClient.right, rcLPClient.top);
		API_VERIFY(::TextOut(hdc, rcLPClient.right, rcLPClient.top, formater.GetString() , formater.GetStringLength()));

		//LeftCenter
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_LEFT|TA_BASELINE));
		formater.Format(TEXT("LC[%d,%d]"), rcLPClient.left, yCenter);
		API_VERIFY(::TextOut(hdc, rcLPClient.left, yCenter, formater.GetString(), formater.GetStringLength()));

		//Center
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_CENTER|TA_BASELINE));
		formater.Format(TEXT("C[%d,%d], %d"), xCenter,yCenter, nMapMode);
		API_VERIFY(::TextOut(hdc, xCenter, yCenter, formater.GetString(), formater.GetStringLength()));

		//RightCenter
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_RIGHT|TA_BASELINE));
		formater.Format(TEXT("RC[%d,%d]"), rcLPClient.right, yCenter);
		API_VERIFY(::TextOut(hdc, rcLPClient.right, yCenter, formater.GetString() , formater.GetStringLength()));

		//LeftBottom
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_LEFT|TA_BOTTOM ));
		formater.Format(TEXT("LB(%d,%d)"), rcLPClient.left, rcLPClient.bottom);
		API_VERIFY(::TextOut(hdc, rcLPClient.left, rcLPClient.bottom, formater.GetString() , formater.GetStringLength()));

		//CenterBottom
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_CENTER|TA_BOTTOM ));
		formater.Format(TEXT("B(%d,%d)"), xCenter, rcLPClient.bottom);
		API_VERIFY(::TextOut(hdc, xCenter, rcLPClient.bottom, formater.GetString() , formater.GetStringLength()));

		//RightBottom
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_RIGHT|TA_BOTTOM ));
		formater.Format(TEXT("RB(%d,%d),%d"), rcLPClient.right, rcLPClient.bottom, nMapMode);
		API_VERIFY(::TextOut(hdc, rcLPClient.right, rcLPClient.bottom, formater.GetString() , formater.GetStringLength()));


		if (bDrawClipbox)
		{
			RECT rcClipBox = {0};
			API_VERIFY(ERROR != ::GetClipBox(hdc, &rcClipBox));
			//API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, (HBRUSH)GetStockObject(LTGRAY_BRUSH)));
			//::InflateRect(&rcClipBox, -4, -4);

			HPEN hRedPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0));
			API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, hRedPen));

			API_VERIFY(::Rectangle(hdc, rcClipBox.left, rcClipBox.top, rcClipBox.right, rcClipBox.bottom));
			formater.Format(TEXT("Clip:(%d,%d)] X (%d,%d)"), rcClipBox.left, rcClipBox.top, rcClipBox.right, rcClipBox.bottom);
			API_VERIFY(GDI_ERROR != SetTextAlign(hdc, TA_CENTER|TA_BOTTOM ));
			API_VERIFY(::TextOut(hdc, (rcClipBox.left + rcClipBox.right) / 2, 
				(rcClipBox.top + rcClipBox.bottom) /2,
				formater.GetString() , formater.GetStringLength()));

			API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, (HPEN)GetStockObject(BLACK_PEN)));
			API_VERIFY(DeleteObject(hRedPen));
		}

		API_VERIFY(SetBkMode(hdc, nOldBkMode));
		API_VERIFY(GDI_ERROR != SetTextAlign(hdc, nOldTextAlign));
		API_VERIFY(::MoveToEx(hdc, ptOldPos.x, ptOldPos.y, NULL));
		API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, hOldBrush));
		API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, hOldPen));

		return bRet;
	}

}

#endif //FTL_GDI_HPP