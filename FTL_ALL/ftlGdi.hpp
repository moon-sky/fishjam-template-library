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
			{
				DWORD dwLastError = GetLastError();
				FTLTRACEEX(FTL::tlWarning, TEXT("GetGdiObjectInfo Call GetObjectType(0x%p) Failed(Err=%d), Maybe Already Call DeletedObject!!!\n"), 
					hgdiObj, dwLastError);
				FTLASSERT(ERROR_INSUFFICIENT_BUFFER == dwLastError || ERROR_NOT_ENOUGH_MEMORY == dwLastError);  //目前测试了 HBitmap，返回值是这个
				break;
			}
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
                    
                    fontInfo.lfHeight,			//参见头文件中的"字体大小单位"
                    fontInfo.lfWidth,           //字体平均宽度的逻辑值,为0时,会自动选择一个与高度最匹配的值
                    fontInfo.lfEscapement,      //字符串底线与水平线的夹角(单位为0.1角度)
                    fontInfo.lfOrientation,     //每个字符的底线与水平线的夹角（单位为0.1度）
                    fontInfo.lfWeight,          //字体的粗细,单位为千分比，取值范围为0(DONTCARE)~1000(全黑),
												//如果为 FW_BOLD(700), 就是默认的粗体字, 系统默认的是 FW_NORMAL(400)
                    fontInfo.lfItalic,          //TRUE则字体为斜体字
                    fontInfo.lfUnderline,       //TRUE则字体带下划线
                    fontInfo.lfStrikeOut,       //TRUE则字体带删除线
                    fontInfo.lfCharSet,         //字符集，有 ANSI_CHARSET(0)、DEFAULT_CHARSET(1)、GB2312_CHARSET(134) 等
                    fontInfo.lfOutPrecision,    //输出字体的精度,有 OUT_DEFAULT_PRECIS(0) 等
                    fontInfo.lfClipPrecision,   //输出时字体被裁剪的精度,有 CLIP_DEFAULT_PRECIS(0) 等
                    fontInfo.lfQuality,         //输出质量,有 CLIP_DEFAULT_PRECIS(0) 等
                    fontInfo.lfPitchAndFamily,  //字符间距与字体家族,字符间距(pitch)占字节的低2位,如 DEFAULT_PITCH(0),
                                                //字体家族(family) 占字节的4~7位，如 FF_DONTCARE(0<<4).
                                                //可以用位或“|”将间距常量与家族常量组合使用(宏中已经定义，不用再移位了)
                    fontInfo.lfFaceName);       //字体名称
                break;
            }
        case OBJ_BITMAP:
            {
                //只要是BITMAP都可用获取到 DIBSECTION, 但普通的Bitmap获取到的 BITMAPINFOHEADER 等值都为0
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
                    //CreateDIBSection 创建的DIB
                    formater.AppendFormat(TEXT(",BITMAPINFOHEADER[biSize=%d,biWidth=%ld,biHeight=%ld,")
                        TEXT("biPlanes=%d,biBitCount=%d,biCompression=%d,")
                        TEXT("biSizeImage=%d,biXPelsPerMeter=%ld,biYPelsPerMeter=%ld,")
                        TEXT("biClrUsed=%d,biClrImportant=%d,")
                        TEXT("dsBitfields=[%d,%d,%d],")
                        TEXT("dshSection=0x%p,dsOffset=%d"),
                        dibInfo.dsBmih.biSize,dibInfo.dsBmih.biWidth, dibInfo.dsBmih.biHeight,
                        dibInfo.dsBmih.biPlanes, dibInfo.dsBmih.biBitCount,
						dibInfo.dsBmih.biCompression,  //未压缩时为0，压缩时可以是压缩格式的字符串，如 "FMP4"?
                        dibInfo.dsBmih.biSizeImage, dibInfo.dsBmih.biXPelsPerMeter, dibInfo.dsBmih.biYPelsPerMeter,
                        dibInfo.dsBmih.biClrUsed, dibInfo.dsBmih.biClrImportant,
                        dibInfo.dsBitfields[0], dibInfo.dsBitfields[1], dibInfo.dsBitfields[2],
                        dibInfo.dshSection, dibInfo.dsOffset);
                }
                else
                {
                    //DIBSECTION 中除了 BITMAP 以外其他结构体的大小 -- 此时其值应该全为 0
                    const DWORD OtherInfoSize = sizeof(DIBSECTION) - sizeof(BITMAP);
                    BYTE dibSecInfo[OtherInfoSize] = {0};
                    FTLASSERT(memcmp(&dibInfo.dsBmih, dibSecInfo, OtherInfoSize) == 0);
                }
                break;
            }
        case OBJ_REGION:
            {
                RECT rcClipBox = {0};
                int nRgnType = ::GetRgnBox((HRGN)hgdiObj, &rcClipBox);
                API_ASSERT(ERROR != nRgnType);
                formater.Format(TEXT("Region:Type=%s,(%d,%d)-(%d,%d), %dx%d"), 
                    CFGdiUtil::GetRegionType(nRgnType), 
                    rcClipBox.left, rcClipBox.top, rcClipBox.right, rcClipBox.bottom, 
                    rcClipBox.right - rcClipBox.left, rcClipBox.bottom - rcClipBox.top);
                break;
            }
        case OBJ_METAFILE:
            {
                FTLASSERT(FALSE);
                break;
            }
        case OBJ_MEMDC:
            {
                HDCProperty propHDC;
                CFGdiUtil::GetHDCProperty((HDC)hgdiObj,&propHDC);
                formater.Format(TEXT("%s"), propHDC.GetPropertyString(HDC_PROPERTY_GET_DEFAULT));
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
                    //#pragma TODO(Need Dump elpStyleEntry)
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
                //如果成功选入，则再Select，恢复原状
                HBITMAP hOriginalBmp = (HBITMAP)::SelectObject(m_hDCCompatible, (HGDIOBJ)hOldBmp);
                API_ASSERT(hOriginalBmp == hBmp);
            }
        }
        return bRet;
    }

    HDCProperty::HDCProperty()
    {
        m_hSafeHdc = NULL;

        m_nGraphicsMode = -1;
        m_nMapMode = -1;
        m_nArcDirection = -1;

        m_ptBrushOrg.x = -1;
        m_ptBrushOrg.y = -1;

        m_ptWindowOrg.x = -1;
        m_ptWindowOrg.y = -1;
        m_szWindowExt.cx = -1;
        m_szWindowExt.cy = -1;

        m_ptViewportOrg.x = -1;
        m_ptViewportOrg.y = -1;
        m_szViewportExt.cx = -1;
        m_szViewportExt.cy = -1;

        m_nDeviceCapsDriverVersion = -1;
        m_nDeviceCapsTechnology = -1;
        m_nDeviceCapsHorzsize = -1;
        m_nDeviceCapsVertsize = -1;
        m_nDeviceCapsHorzres = -1;
        m_nDeviceCapsVertres = -1;
        m_nDeviceCapsLogPixelsX = -1;
        m_nDeviceCapsLogPixelsY = -1;
        m_nDeviceCapsBitsPixel = -1;
        m_nDeviceCapsPlanes = -1;
        m_nDeviceCapsNumBrushes = -1;
        m_nDeviceCapsNumPens = -1;
        m_nDeviceCapsNumFonts = -1;
        m_nDeviceCapsNumColors = -1;
        m_nDeviceCapsAspectX = -1;
        m_nDeviceCapsAspectY = -1;
        m_nDeviceCapsAspectXY = -1;
        m_nDeviceCapsPDeviceSizeReserved = -1;
        m_nDeviceCapsClipCaps = -1;
        m_nDeviceCapsSizePalette = -1;
        m_nDeviceCapsNumReserved = -1;
        m_nDeviceCapsColorRes = -1;
        m_nDeviceCapsPhysicalWidth = -1;
        m_nDeviceCapsPhysicalHeight = -1;
        m_nDeviceCapsPhysicalOffsetX = -1;
        m_nDeviceCapsPhysicalOffsetY = -1;
        m_nDeviceCapsVRefresh = -1;
        m_nDeviceCapsScalingFactorX = -1;
        m_nDeviceCapsScalingFactorY = -1;
        m_nDeviceCapsBltAlignment = -1;
        m_nDeviceCapsShadeBlendCaps = -1;
        m_nDeviceCapsRasterCaps = -1;
        m_nDeviceCapsCurveCaps = -1;
        m_nDeviceCapsLineCaps = -1;
        m_nDeviceCapsPolygonalCaps = -1;
        m_nDeviceCapsTextCaps = -1;
        m_nDeviceCapsColorMgmtCaps = -1;

        m_BkColor = CLR_INVALID;
        m_BkMode = -1;
        m_nROP2 = -1;
        m_nStretchBltMode = -1;
        m_nPolyFillMode = -1;

        ::ZeroMemory(&m_xForm, sizeof(m_xForm));

		m_TextAlign  = -1;
        m_rcClipBox.left = -1;
        m_rcClipBox.top = -1;
        m_rcClipBox.top = -1;
        m_rcClipBox.bottom = -1;

        m_ptCurrentPosition.x = -1;
        m_ptCurrentPosition.y = -1;

        m_pszProperty = NULL;
        m_dwOldGetProperty = 0;

        //m_hCurBitmap = NULL;
        ZeroMemory(&m_curBitmapInfo, sizeof(m_curBitmapInfo));
    }
    HDCProperty::~HDCProperty()
    {
        if (NULL != m_pszProperty)
        {
            SAFE_DELETE_ARRAY(m_pszProperty);
        }
    }
    
    LPCTSTR HDCProperty::GetPropertyString(DWORD dwPropertyGet/* = HDC_PROPERTY_GET_DEFAULT*/)
    {
        if (m_dwOldGetProperty != dwPropertyGet)
        {
            SAFE_DELETE_ARRAY(m_pszProperty);
        }

        if (NULL == m_pszProperty)
        {
            CFStringFormater formater(1024);

            if (HDC_PROPERTY_GET_MAPMODE == (HDC_PROPERTY_GET_MAPMODE & dwPropertyGet))
            {
                formater.AppendFormat(
                    TEXT("MapMode=%s,"),
                    CFGdiUtil::GetMapModeString(m_nMapMode));

            }
            formater.AppendFormat(TEXT("ArcDirection=%d, ptBrushOrg=[%d,%d],"), 
                m_nArcDirection,
                m_ptBrushOrg.x,m_ptBrushOrg.y);

            if (HDC_PROPERTY_GET_WINDOW_INFO == (HDC_PROPERTY_GET_WINDOW_INFO & dwPropertyGet))
            {
                formater.AppendFormat(
                    TEXT("\tWindow=(%d,%d)-(%d,%d), %dx%d\n"),
                    m_ptWindowOrg.x, m_ptWindowOrg.y,
                    m_ptWindowOrg.x + m_szWindowExt.cx, m_ptWindowOrg.y + m_szWindowExt.cy,
                    m_szWindowExt.cx, m_szWindowExt.cy
                    );
            }

            if (HDC_PROPERTY_GET_VIEWPORT_INFO == (HDC_PROPERTY_GET_VIEWPORT_INFO & dwPropertyGet))
            {
                formater.AppendFormat(
                    TEXT("\tViewport=(%d,%d)-(%d,%d), %dx%d\n"),
                    m_ptViewportOrg.x, m_ptViewportOrg.y,
                    m_ptViewportOrg.x + m_szViewportExt.cx, m_ptViewportOrg.y + m_szViewportExt.cy,
                    m_szViewportExt.cx, m_szViewportExt.cy
                    );

            }

            if (HDC_PROPERTY_GET_DEVICE_CAPS == (HDC_PROPERTY_GET_DEVICE_CAPS & dwPropertyGet))
            {
                //DeviceCaps
                formater.AppendFormat(
                    TEXT("\tDriverVersion=0x%x,")
                    TEXT("Technology=%d,")
                    TEXT("Horzsize=%d,")
                    TEXT("Vertsize=%d,")
                    TEXT("Horzres=%d,")
                    TEXT("Vertres=%d,")
                    TEXT("LogPixelsX=%d,")
                    TEXT("LogPixelsY=%d,")
                    TEXT("BitsPixel=%d,")
                    TEXT("Planes=%d\n")

                    TEXT("\tNumBrushes=%d,")
                    TEXT("NumPens=%d,")
                    TEXT("NumFonts=%d,")
                    TEXT("NumColors=%d,")
                    TEXT("AspectX=%d,")
                    TEXT("AspectY=%d,")
                    TEXT("AspectXY=%d,")
                    TEXT("PDeviceSizeReserved=%d,")
                    TEXT("ClipCaps=%d,")
                    TEXT("SizePalette=%d,")
                    TEXT("NumReserved=%d,")
                    TEXT("ColorRes=%d\n")

                    TEXT("\tPhysicalWidth=%d,")
                    TEXT("PhysicalHeight=%d,")
                    TEXT("PhysicalOffsetX=%d,")
                    TEXT("PhysicalOffsetY=%d,")
                    TEXT("VRefresh=%d,")
                    TEXT("ScalingFactorX=%d,")
                    TEXT("ScalingFactorY=%d,")
                    TEXT("BltAlignment=%d\n")

                    TEXT("\tShadeBlendCaps=0x%x,")
                    TEXT("RasterCaps=0x%x,")
                    TEXT("CurveCaps=0x%x,")
                    TEXT("LineCaps=0x%x,")
                    TEXT("PolygonalCaps=0x%x,")
                    TEXT("TextCaps=0x%x,")
                    TEXT("ColorMgmtCaps=0x%x\n")
                    ,

                    //DeviceCaps
                    m_nDeviceCapsDriverVersion,
                    m_nDeviceCapsTechnology,
                    m_nDeviceCapsHorzsize,
                    m_nDeviceCapsVertsize,
                    m_nDeviceCapsHorzres,
                    m_nDeviceCapsVertres,
                    m_nDeviceCapsLogPixelsX,
                    m_nDeviceCapsLogPixelsY,
                    m_nDeviceCapsBitsPixel,
                    m_nDeviceCapsPlanes,
                    m_nDeviceCapsNumBrushes,
                    m_nDeviceCapsNumPens,
                    m_nDeviceCapsNumFonts,
                    m_nDeviceCapsNumColors,
                    m_nDeviceCapsAspectX,
                    m_nDeviceCapsAspectY,
                    m_nDeviceCapsAspectXY,
                    m_nDeviceCapsPDeviceSizeReserved,
                    m_nDeviceCapsClipCaps,
                    m_nDeviceCapsSizePalette,
                    m_nDeviceCapsNumReserved,
                    m_nDeviceCapsColorRes,
                    m_nDeviceCapsPhysicalWidth,
                    m_nDeviceCapsPhysicalHeight,
                    m_nDeviceCapsPhysicalOffsetX,
                    m_nDeviceCapsPhysicalOffsetY,
                    m_nDeviceCapsVRefresh,
                    m_nDeviceCapsScalingFactorX,
                    m_nDeviceCapsScalingFactorY,
                    m_nDeviceCapsBltAlignment,
                    m_nDeviceCapsShadeBlendCaps,
                    m_nDeviceCapsRasterCaps,
                    m_nDeviceCapsCurveCaps,
                    m_nDeviceCapsLineCaps,
                    m_nDeviceCapsPolygonalCaps,
                    m_nDeviceCapsTextCaps,
                    m_nDeviceCapsColorMgmtCaps
                    );
            }


            if (HDC_PROPERTY_GET_MODE == (HDC_PROPERTY_GET_MODE & dwPropertyGet))
            {
                formater.AppendFormat(
                    TEXT("\tGraphicsMode=%s,\n")

                    TEXT("\tBkColor=0x%08x,")
                    TEXT("BkMode=%s,")
                    TEXT("ROP2=%s,")
                    TEXT("nStretchBltMode=%s,")
                    TEXT("nPolyFillMode=%s,\n")

                    TEXT("\txForm is [eM11=%f,eM12=%f,eM21=%f,eM22=%f,eDx=%f,eDy=%f]\n")
                    TEXT("\trcClipBox = [(%d,%d)X(%d,%d)], ptCurrentPosition=(%d X %d)\n"),

                    CFGdiUtil::GetGraphicsModeString(m_nGraphicsMode),
                    m_BkColor,
                    CFGdiUtil::GetBkModeString(m_BkMode),
                    CFGdiUtil::GetROP2String(m_nROP2),
                    CFGdiUtil::GetStretchBltModeString(m_nStretchBltMode),
                    CFGdiUtil::GetPolyFillModeString(m_nPolyFillMode),

                    m_xForm.eM11,m_xForm.eM12, m_xForm.eM21, m_xForm.eM22, m_xForm.eDx, m_xForm.eDy,
                    m_rcClipBox.left, m_rcClipBox.top, m_rcClipBox.right, m_rcClipBox.bottom,
                    m_ptCurrentPosition.x, m_ptCurrentPosition.y
                    );
            }

            if (HDC_PROPERTY_GET_BITMAP == (HDC_PROPERTY_GET_BITMAP & dwPropertyGet))
            {
                formater.AppendFormat(
                    TEXT("\n%s"), BitmapProperty(m_curBitmapInfo).GetPropertyString()
                    );
            }
            m_pszProperty = formater.Detach();
        }

        m_dwOldGetProperty = dwPropertyGet;
        return m_pszProperty;
    }

	//////////////////////////////////////////////////////////////////////////
	
	PointPosQuadrant	CFGdiUtil::CalcPointPosQuadrant(const RECT& rect, const POINT& point)
	{
		//Bit Array?
		//static CDrawBalloon::PointPosQuadrant posTypees[2][2][2] = 
		//{
		//	posTopRight, posRightTop, posRightBottom, posBottomRight,
		//	posBottomLeft, posLeftBottom, posLeftTop, posTopLeft
		//};
		PointPosQuadrant posQuadrant = posTopRight;

		RECT rectCheck = rect;
		//Normalize
		if (rectCheck.left < rectCheck.right)
		{
			SwapValue(rectCheck.left, rectCheck.right);
		}
		if (rectCheck.top < rectCheck.right)
		{
			SwapValue(rectCheck.left, rectCheck.right);
		}
		//rectCheck.NormalizeRect();
		POINT ptCenter = { (rectCheck.left + rectCheck.right) / 2, (rectCheck.top + rectCheck.bottom) / 2 };
		::OffsetRect(&rectCheck, -ptCenter.x, -ptCenter.y);
		POINT ptCheck = point;
		ptCheck.x += -ptCenter.x;
		ptCheck.y += -ptCenter.y;

		if (ptCheck.x > 0)
		{
			if (ptCheck.y > 0)
			{
				POINT ptCorner = {rectCheck.right, rectCheck.bottom};
				//if (abs(ptCheck.x) < abs(ptCheck.y))
				if (abs(ptCheck.y * ptCorner.x) > abs(ptCheck.x * ptCorner.y))
				{
					posQuadrant = posBottomRight;
				}
				else
				{
					posQuadrant = posRightBottom;
				}
			}
			else
			{
				POINT ptCorner = {rectCheck.right, rectCheck.top};
				if (abs(ptCheck.y * ptCorner.x) > abs(ptCheck.x * ptCorner.y))
					//if (abs(ptCheck.x) < abs(ptCheck.y))
				{
					posQuadrant = posTopRight;
				}
				else
				{
					posQuadrant = posRightTop;
				}
			}
		}
		else
		{
			if (ptCheck.y > 0)
			{
				POINT ptCorner = {rectCheck.left, rectCheck.bottom};
				if (abs(ptCheck.y * ptCorner.x) > abs(ptCheck.x * ptCorner.y))
					//if (abs(ptCheck.x) < abs(ptCheck.y))
				{
					posQuadrant = posBottomLeft;
				}
				else
				{
					posQuadrant = posLeftBottom;
				}
			}
			else
			{
				POINT ptCorner = {rectCheck.left, rectCheck.top};
				if (abs(ptCheck.y * ptCorner.x) > abs(ptCheck.x * ptCorner.y))
					//if (abs(ptCheck.x) < abs(ptCheck.y))
				{
					posQuadrant = posTopLeft;
				}
				else
				{
					posQuadrant = posLeftTop;
				}
			}
		}
		//ATLTRACE(TEXT("posType=%d(%s)\n"), posQuadrant, GetPosString(posQuadrant));
		return posQuadrant;
	}

	BOOL CFGdiUtil::LoadPNGFromResource(CImage& image, HMODULE hModule, UINT nIDResource, LPCTSTR pszType)
	{
		BOOL bRet = FALSE;
		HRSRC hrSrc = ::FindResource(hModule, MAKEINTRESOURCE(nIDResource), pszType);
		API_ASSERT(NULL != hrSrc);
		if (hrSrc)
		{
#if 0 
			//或者 -- 没有测试
			HGLOBAL hGlobal = LoadResource( hModule, hrSrc );
			DWORD dwSize = SizeofResource( hModule, hrSrc );
			BYTE* pResData = (BYTE*)LockResource( hGlobal );
			//使用数据， 比如 CopyMemory 到buffer中
			UnlockResource( hGlobal );
			FreeResource( hrSrc );
#endif 

			DWORD dwResSize = SizeofResource(hModule, hrSrc);
			BYTE* lpRsrc = (BYTE*)LoadResource(hModule, hrSrc);
			API_ASSERT(NULL != lpRsrc);
			if (lpRsrc)
			{
				HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, dwResSize);
				BYTE* pmem = (BYTE*)GlobalLock(hMem);
				CopyMemory(pmem,lpRsrc,dwResSize);

				HRESULT hr = E_FAIL;
				CComPtr<IStream> pStream;
				COM_VERIFY(CreateStreamOnHGlobal(hMem, TRUE, &pStream));
				if (SUCCEEDED(hr) && pStream)
				{
					COM_VERIFY(image.Load(pStream));
					if (SUCCEEDED(hr))
					{
						bRet = TRUE;
					}
				}
				GlobalUnlock(hMem);
			}
			UnlockResource(hrSrc);
			FreeResource(hrSrc);
		}
		return bRet;
	}

    LPCTSTR CFGdiUtil::GetBkModeString(int nBkMode)
    {
        switch(nBkMode)
        {
            HANDLE_CASE_RETURN_STRING(OPAQUE);          //[不透明] -- 空隙用背景色填充
            HANDLE_CASE_RETURN_STRING(TRANSPARENT);     //透明 -- 空隙处保持原背景图不变
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetGraphicsModeString(int nGraphicsMode)
    {
        switch(nGraphicsMode)
        {
            HANDLE_CASE_RETURN_STRING(GM_COMPATIBLE);
            HANDLE_CASE_RETURN_STRING(GM_ADVANCED);
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetMapModeString(int nMapMode)
    {
        switch(nMapMode)
        {
            HANDLE_CASE_RETURN_STRING(MM_TEXT);
            HANDLE_CASE_RETURN_STRING(MM_LOMETRIC);
            HANDLE_CASE_RETURN_STRING(MM_HIMETRIC);
            HANDLE_CASE_RETURN_STRING(MM_LOENGLISH);
            HANDLE_CASE_RETURN_STRING(MM_HIENGLISH);
            HANDLE_CASE_RETURN_STRING(MM_TWIPS);
            HANDLE_CASE_RETURN_STRING(MM_ISOTROPIC);
            HANDLE_CASE_RETURN_STRING(MM_ANISOTROPIC);
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetMapModeDescString(int nMapMode)
    {
        switch(nMapMode)
        {
        case MM_TEXT: return TEXT("pixel,↓");
        case MM_LOMETRIC: return TEXT("0.01 inch,↑");
        case MM_HIMETRIC: return TEXT("0.001 inch,↑");
        case MM_LOENGLISH: return TEXT("0.1 mm,↑");
        case MM_HIENGLISH: return TEXT("0.01 mm,↑");
        case MM_TWIPS: return TEXT("1/1440 inch,↑");
        case MM_ISOTROPIC: return TEXT("xxxx");
        case MM_ANISOTROPIC: return TEXT("xxxx");
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetROP2String(int fnDrawMode)
    {
        switch(fnDrawMode)
        {
            HANDLE_CASE_RETURN_STRING(R2_BLACK);        //黑色, pixel = black
            HANDLE_CASE_RETURN_STRING(R2_NOTMERGEPEN);  //或非, pixel = ~(scCol | pbCol)
            HANDLE_CASE_RETURN_STRING(R2_MASKNOTPEN);   //与反色, pixel = scCol & ~pbCol
            HANDLE_CASE_RETURN_STRING(R2_NOTCOPYPEN);   //反色覆盖, pixel = ~pbCol
            HANDLE_CASE_RETURN_STRING(R2_MASKPENNOT);   //反色与, pixel = ~scCol & pbCol
            HANDLE_CASE_RETURN_STRING(R2_NOT);          //反色, pixel = ~scCol
            HANDLE_CASE_RETURN_STRING(R2_XORPEN);       //异或, pixel = scCol ^ pbCol,常用于两次绘制恢复成原样
            HANDLE_CASE_RETURN_STRING(R2_NOTMASKPEN);   //与非, pixel = ~(scCol & pbCol)
            HANDLE_CASE_RETURN_STRING(R2_MASKPEN);      //与, pixel = scCol & pbCol
            HANDLE_CASE_RETURN_STRING(R2_NOTXORPEN);    //异或非, pixel = ~(scCol ^ pbCol) -- 也用于两次绘制恢复成原样?
            HANDLE_CASE_RETURN_STRING(R2_NOP);          //不变, pixel = pbCol
            HANDLE_CASE_RETURN_STRING(R2_MERGENOTPEN);  //或反色, pixel = scCol | ~pbCol
            HANDLE_CASE_RETURN_STRING(R2_COPYPEN);      //[覆盖], pixel = pbCol
            HANDLE_CASE_RETURN_STRING(R2_MERGEPENNOT);  //反色或, pixel = ~scCol | pbCol
            HANDLE_CASE_RETURN_STRING(R2_MERGEPEN);     //或, pixel = scCol | pbCol
            HANDLE_CASE_RETURN_STRING(R2_WHITE);        //白色, pixel = white
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetStretchBltModeString(int nStretchMode)
    {
		//产生像素堆积问题?
        switch(nStretchMode)
        {
            //[清除的像素与保存的像素的颜色相与，对黑白图会保存黑色，去掉白色，常用于单色位图]
            //效果:越来越黑 -- 重叠时执行 "与" 操作
            HANDLE_CASE_RETURN_STRING(BLACKONWHITE);
            
            //清除的像素与保存的像素的颜色相或，对黑白图会保存白色，去掉黑色，用于单色位图
            //效果:越来越白
            HANDLE_CASE_RETURN_STRING(WHITEONBLACK);
            
            //删除清除的扫描线，常用于彩色位图和灰度图
            //效果:变粗糙 ?
            HANDLE_CASE_RETURN_STRING(COLORONCOLOR);    //STRETCH_DELETESCANS
            
            //计算平均颜色，质量高，但速度慢。
			//为了使刷能够对齐，在调用SetStretchBltMode (HALFTONE) 后，必须再调用::SetBrushOrgEx 校正画刷
            HANDLE_CASE_RETURN_STRING(HALFTONE);        //STRETCH_HALFTONE
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetBitBltRopString( DWORD dwRop )
    {
        switch(dwRop)
        {
            HANDLE_CASE_RETURN_STRING(SRCCOPY);     //dest = source
            HANDLE_CASE_RETURN_STRING(SRCPAINT);    //dest = source OR dest
            HANDLE_CASE_RETURN_STRING(SRCAND);      //dest = source AND dest
            HANDLE_CASE_RETURN_STRING(SRCINVERT);   //dest = source XOR dest
            HANDLE_CASE_RETURN_STRING(SRCERASE);    //dest = source AND (NOT dest )
            HANDLE_CASE_RETURN_STRING(NOTSRCCOPY);  //dest = (NOT source)
            HANDLE_CASE_RETURN_STRING(NOTSRCERASE); //dest = (NOT src) AND (NOT dest)
            HANDLE_CASE_RETURN_STRING(MERGECOPY);   //dest = (source AND pattern)
            HANDLE_CASE_RETURN_STRING(MERGEPAINT);  //dest = (NOT source) OR dest
            HANDLE_CASE_RETURN_STRING(PATCOPY);     //dest = pattern
            HANDLE_CASE_RETURN_STRING(PATPAINT);    //dest = DPSnoo -- ?((NOT 源位图) OR 图案) OR 目标位图
            HANDLE_CASE_RETURN_STRING(PATINVERT);   //dest = pattern XOR dest
            HANDLE_CASE_RETURN_STRING(DSTINVERT);   //dest = (NOT dest)
            HANDLE_CASE_RETURN_STRING(BLACKNESS);   //dest = BLACK
            HANDLE_CASE_RETURN_STRING(WHITENESS);   //dest = WHITE
#if(WINVER >= 0x0500)
            HANDLE_CASE_RETURN_STRING(NOMIRRORBITMAP);//Do not Mirror the bitmap in this call
            HANDLE_CASE_RETURN_STRING(CAPTUREBLT);  //Include layered windows
#endif /* WINVER >= 0x0500 */
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }


    LPCTSTR CFGdiUtil::GetPolyFillModeString(int nPolyFillMode)
    {
        switch(nPolyFillMode)
        {
            //[交替]--填充奇数边和偶数边之间的区域，为了判断那些像素需要高亮，执行下面的测试：
            //1.在区域内部选择一个像素
            //2.从该点沿正X轴方向画一条虚设的光线，从像素开始到无穷远
            //3.每次光线和边界线相交的，增加一个记数值，系统高亮那些记数值是奇数的像素。
            //  五角星(★)的话填充五个角，空出中间的五边形
            HANDLE_CASE_RETURN_STRING(ALTERNATE);

            //缠绕--根据多边形边的走向来确定是否填充一区域，为了判断缠绕模式下那些像素需要高亮，需要执行下面测试：
            //1.在画每个边界线的时候判断方向
            //2.在区域的内部选择一个像素
            //3.画一条光线，沿正X方向，从像素点到无穷远
            //4.每次光线与边界线在正Y部分正交的时候，增加记数值，如果在Y负方向，减记数值。
            //5.所有非0的像素被高亮。
            //  五角星(★)的话，填充全部
            HANDLE_CASE_RETURN_STRING(WINDING);
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetArcDirectionString(int nArcDirection)
    {
        switch(nArcDirection)
        {
            HANDLE_CASE_RETURN_STRING(AD_COUNTERCLOCKWISE); //[逆时针]
            HANDLE_CASE_RETURN_STRING(AD_CLOCKWISE);        //顺时针
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }
    LPCTSTR CFGdiUtil::GetRegionType(int nType)
    {
        switch(nType)
        {
            HANDLE_CASE_RETURN_STRING(RGN_ERROR);
            HANDLE_CASE_RETURN_STRING(NULLREGION);
            HANDLE_CASE_RETURN_STRING(SIMPLEREGION);
            HANDLE_CASE_RETURN_STRING(COMPLEXREGION);
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

    LPCTSTR CFGdiUtil::GetTextAlignString(int nFlags)
    {
        switch(nFlags)
        {
            HANDLE_CASE_RETURN_STRING(TA_LEFT | TA_TOP | TA_NOUPDATECP);//[居左显示文本,顶对齐,不更新当前位置]
            HANDLE_CASE_RETURN_STRING(TA_UPDATECP);     //更新当前位置
            HANDLE_CASE_RETURN_STRING(TA_RIGHT);        //居右显示文本
            HANDLE_CASE_RETURN_STRING(TA_CENTER);       //居中显示文本
            HANDLE_CASE_RETURN_STRING(TA_BOTTOM);       //底对齐
            HANDLE_CASE_RETURN_STRING(TA_BASELINE);     //基线对齐
        default:
            FTLASSERT(FALSE);
            return TEXT("");
        }
    }

	LPCTSTR CFGdiUtil::GetTextMetricsInfo(HDC hdc, CFStringFormater& strFormater , HFONT hFont /*= NULL*/)
	{
		BOOL bRet = FALSE;
		HFONT hOldFont = NULL;
		if (hFont)
		{
			hOldFont = (HFONT)::SelectObject(hdc, hFont);
			FTLASSERT(NULL != hOldFont);
		}

		TEXTMETRIC	tm = {0};
		API_VERIFY(::GetTextMetrics(hdc, &tm));
		if (bRet)
		{
			//未确认!
			//字体的高度 = tmHeight + mExternalLeading
			//一行的高度 = tmInternalLeading + tmHeight + mExternalLeading
			//             tmAscent + tmDescent 等价于正的 LOGFONT::lfHeight?
			HRESULT hr = E_FAIL;
			COM_VERIFY(strFormater.Format(TEXT("tmHeight=%d, tmAscent=%d, tmDescent=%d, tmInternalLeading=%d, ")
				TEXT("tmExternalLeading=%d, tmAveCharWidth=%d, tmMaxCharWidth=%d, tmWeight=%d, ")
				TEXT("tmOverhang=%d, tmDigitizedAspectX=%d, tmDigitizedAspectY=%d, ")
				TEXT("tmFirstChar=%d, tmLastChar=%d, tmDefaultChar=%d, tmBreakChar=%d, ")
				TEXT("tmItalic=%d, tmUnderlined=%d, tmStruckOut=%d, tmPitchAndFamily=%d, tmCharSet=%d"),
				tm.tmHeight,
				tm.tmAscent,
				tm.tmDescent,
				tm.tmInternalLeading,
				tm.tmExternalLeading,	//两行之间的额外高度,通常在输出多行文本时使用
				tm.tmAveCharWidth,
				tm.tmMaxCharWidth,
				tm.tmWeight,
				tm.tmOverhang,			//斜体时的倾斜宽度?
				tm.tmDigitizedAspectX,
				tm.tmDigitizedAspectY,
				tm.tmFirstChar,
				tm.tmLastChar,
				tm.tmDefaultChar,
				tm.tmBreakChar,
				tm.tmItalic,
				tm.tmUnderlined,
				tm.tmStruckOut,
				tm.tmPitchAndFamily,
				tm.tmCharSet));
		}
		if (hFont)
		{
			//restore
			::SelectObject(hdc, hOldFont);
		}
		return strFormater.GetString();
	}

    LPCTSTR CFGdiUtil::GetFontNumberByPointSize(int nPointSize) 
    {
        //注意：nPointSize / 10 就是磅数
        switch(nPointSize)
        {
		//计算对应的像素值： nPointSize * dipY / 72 = nPointSize*96/72
		//对应的毫米数？？：     nPointSize * 25.4 /72 -- 好像对应高度刚好错了一行？
        case 420: return TEXT("初号");  //像素值：56, 毫米数：? 12.7
        case 360: return TEXT("小初");	//像素值：48, 毫米数：? 11.1
        case 260: return TEXT("一号");	//像素值：34, 毫米数：? 9.66
        case 240: return TEXT("小一");	//像素值：32, 毫米数：? 8.42
        case 220: return TEXT("二号");	//像素值：29, 毫米数：? 7.80
        case 180: return TEXT("小二");	//像素值：24, 毫米数：? 6.39
        case 160: return TEXT("三号");	//像素值：21, 毫米数：? 5.55
        case 150: return TEXT("小三");	//像素值：20, 毫米数：? 5.23
        case 140: return TEXT("四号");	//像素值：18, 毫米数：? 4.93
        case 120: return TEXT("小四");	//像素值：16, 毫米数：? 4.25
        case 105: return TEXT("五号");	//像素值：14, 毫米数：? 3.70
        case  90: return TEXT("小五");	//像素值：12, 毫米数：? 3.15
        case  75: return TEXT("六号");	//像素值：10, 毫米数：? 2.81
        case  65: return TEXT("小六");	//像素值：8,  毫米数：? 2.45
        case  55: return TEXT("七号");	//像素值：7,  毫米数：? 2.12
        case  50: return TEXT("八号");	//像素值：6,  毫米数：? 1.74
        default:
            return TEXT("Unknown");
        }
    }

    BOOL CFGdiUtil::GetHDCProperty(HDC hdc, HDCProperty* pProperty)
    {
        BOOL bRet = FALSE;
        CHECK_POINTER_READABLE_RETURN_VALUE_IF_FAIL(pProperty, FALSE);
        API_VERIFY(0 != (pProperty->m_nMapMode = ::GetMapMode(hdc)));
        API_VERIFY(0 != (pProperty->m_nArcDirection = ::GetArcDirection(hdc)));
        API_VERIFY(::GetBrushOrgEx(hdc, &pProperty->m_ptBrushOrg));

        API_VERIFY(::GetWindowOrgEx(hdc, &pProperty->m_ptWindowOrg));
        API_VERIFY(::GetWindowExtEx(hdc, &pProperty->m_szWindowExt));

        API_VERIFY(::GetViewportOrgEx(hdc, &pProperty->m_ptViewportOrg));
        API_VERIFY(::GetViewportExtEx(hdc, &pProperty->m_szViewportExt));
        
        API_VERIFY(0 != (pProperty->m_nGraphicsMode = ::GetGraphicsMode(hdc)));

        pProperty->m_nDeviceCapsDriverVersion = ::GetDeviceCaps(hdc, DRIVERVERSION);
        pProperty->m_nDeviceCapsTechnology = ::GetDeviceCaps(hdc, TECHNOLOGY);
        pProperty->m_nDeviceCapsHorzsize = ::GetDeviceCaps(hdc, HORZSIZE);
        pProperty->m_nDeviceCapsVertsize = ::GetDeviceCaps(hdc, VERTSIZE);
        pProperty->m_nDeviceCapsHorzres = ::GetDeviceCaps(hdc, HORZRES);
        pProperty->m_nDeviceCapsVertres = ::GetDeviceCaps(hdc, VERTRES);
        pProperty->m_nDeviceCapsLogPixelsX = ::GetDeviceCaps(hdc, LOGPIXELSX);
        pProperty->m_nDeviceCapsLogPixelsY = ::GetDeviceCaps(hdc, LOGPIXELSY);
        pProperty->m_nDeviceCapsBitsPixel = ::GetDeviceCaps(hdc, BITSPIXEL);
        pProperty->m_nDeviceCapsPlanes = ::GetDeviceCaps(hdc, PLANES);
        pProperty->m_nDeviceCapsNumBrushes = ::GetDeviceCaps(hdc, NUMBRUSHES);
        pProperty->m_nDeviceCapsNumPens = ::GetDeviceCaps(hdc, NUMPENS);
        pProperty->m_nDeviceCapsNumFonts = ::GetDeviceCaps(hdc, NUMFONTS);
        pProperty->m_nDeviceCapsNumColors = ::GetDeviceCaps(hdc, NUMCOLORS);
        pProperty->m_nDeviceCapsAspectX = ::GetDeviceCaps(hdc, ASPECTX);
        pProperty->m_nDeviceCapsAspectY = ::GetDeviceCaps(hdc, ASPECTY);
        pProperty->m_nDeviceCapsAspectXY = ::GetDeviceCaps(hdc, ASPECTXY);
        pProperty->m_nDeviceCapsPDeviceSizeReserved = ::GetDeviceCaps(hdc, PDEVICESIZE);
        pProperty->m_nDeviceCapsClipCaps = ::GetDeviceCaps(hdc, CLIPCAPS);
        pProperty->m_nDeviceCapsSizePalette = ::GetDeviceCaps(hdc, SIZEPALETTE);
        pProperty->m_nDeviceCapsNumReserved = ::GetDeviceCaps(hdc, NUMRESERVED);
        pProperty->m_nDeviceCapsColorRes = ::GetDeviceCaps(hdc, COLORRES);
        pProperty->m_nDeviceCapsPhysicalWidth = ::GetDeviceCaps(hdc, PHYSICALWIDTH);
        pProperty->m_nDeviceCapsPhysicalHeight = ::GetDeviceCaps(hdc, PHYSICALHEIGHT);
        pProperty->m_nDeviceCapsPhysicalOffsetX = ::GetDeviceCaps(hdc, PHYSICALOFFSETX);
        pProperty->m_nDeviceCapsPhysicalOffsetY = ::GetDeviceCaps(hdc, PHYSICALOFFSETY);
        pProperty->m_nDeviceCapsVRefresh = ::GetDeviceCaps(hdc, VREFRESH);
        pProperty->m_nDeviceCapsScalingFactorX = ::GetDeviceCaps(hdc, SCALINGFACTORX);
        pProperty->m_nDeviceCapsScalingFactorY = ::GetDeviceCaps(hdc, SCALINGFACTORY);
        pProperty->m_nDeviceCapsBltAlignment = ::GetDeviceCaps(hdc, BLTALIGNMENT);
        pProperty->m_nDeviceCapsShadeBlendCaps = ::GetDeviceCaps(hdc, SHADEBLENDCAPS);
        pProperty->m_nDeviceCapsRasterCaps = ::GetDeviceCaps(hdc, RASTERCAPS);
        pProperty->m_nDeviceCapsCurveCaps = ::GetDeviceCaps(hdc, CURVECAPS);
        pProperty->m_nDeviceCapsLineCaps = ::GetDeviceCaps(hdc, LINECAPS);
        pProperty->m_nDeviceCapsPolygonalCaps = ::GetDeviceCaps(hdc, POLYGONALCAPS);
        pProperty->m_nDeviceCapsTextCaps = ::GetDeviceCaps(hdc, TEXTCAPS);
        pProperty->m_nDeviceCapsColorMgmtCaps = ::GetDeviceCaps(hdc, COLORMGMTCAPS);

        API_VERIFY(CLR_INVALID != (pProperty->m_BkColor = ::GetBkColor(hdc)));
        API_VERIFY(0 != (pProperty->m_BkMode = ::GetBkMode(hdc)));
        API_VERIFY(0 != (pProperty->m_nROP2 = ::GetROP2(hdc)));
        API_VERIFY(0 != (pProperty->m_nStretchBltMode = ::GetStretchBltMode(hdc)));
        API_VERIFY(0 != (pProperty->m_nPolyFillMode = ::GetPolyFillMode(hdc)));

        API_VERIFY(::GetWorldTransform(hdc, &pProperty->m_xForm));
        API_VERIFY(ERROR != ::GetClipBox(hdc, &pProperty->m_rcClipBox));

        API_VERIFY(GetCurrentPositionEx(hdc,&pProperty->m_ptCurrentPosition));

        HBITMAP hCurBitmap = (HBITMAP)::GetCurrentObject(hdc, OBJ_BITMAP);
        API_ASSERT(NULL != hCurBitmap);
        if (NULL != hCurBitmap)
        {
            int needSize = ::GetObject(hCurBitmap, 0, NULL);
            API_ASSERT(needSize >= sizeof(BITMAP));
            ::GetObject(hCurBitmap, sizeof(BITMAP), &pProperty->m_curBitmapInfo);
        }
        return bRet;
    }

    BOOL CFGdiUtil::DrawCoordinate(HDC hdc, const RECT& rcClient, BOOL bDrawText ,BOOL bDrawClipbox)
    {
        BOOL bRet = FALSE;

        //备份原来的HDC信息
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


        //获取 Window/Viewport 的位置信息
        POINT ptWindowOrg = { 0 };
        SIZE szWindowExt = { 0 };
        API_VERIFY(::GetWindowOrgEx(hdc, &ptWindowOrg));
        API_VERIFY(::GetWindowExtEx(hdc, &szWindowExt));

        POINT ptViewportOrg = { 0 };
        SIZE szViewportExt = { 0 };
        API_VERIFY(::GetViewportOrgEx(hdc, &ptViewportOrg));
        API_VERIFY(::GetViewportExtEx(hdc, &szViewportExt));

        //获取映射模式,并根据映射模式进行特殊处理
        int nMapMode = 0;
        API_VERIFY( 0 != (nMapMode = ::GetMapMode(hdc)));
        switch(nMapMode)
        {
        case MM_TEXT:

            break;
        }

        //画坐标
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

        //写坐标的位置信息
        //TCHAR buf[MAX_PATH] = {0};

        //两个特殊点: (0,0) 和 中点
        if (bDrawText)
        {
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
            formater.Format(TEXT("C[%d,%d], %s(%s)"), xCenter,yCenter, GetMapModeString(nMapMode), GetMapModeDescString(nMapMode));
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
            formater.Format(TEXT("RB(%d,%d),%s"), rcLPClient.right, rcLPClient.bottom, GetMapModeDescString(nMapMode));
            API_VERIFY(::TextOut(hdc, rcLPClient.right, rcLPClient.bottom, formater.GetString() , formater.GetStringLength()));
        }


        if (bDrawClipbox)
        {
            //使用红色画笔绘制裁减区域(为了方便查看，向内缩进了4个单位)
            RECT rcClipBox = {0};
            API_VERIFY(ERROR != ::GetClipBox(hdc, &rcClipBox));
            //API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, (HBRUSH)GetStockObject(LTGRAY_BRUSH)));
            ::InflateRect(&rcClipBox, -4, -4);

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

        //清理工作，恢复成原来的状态，避免影响代码逻辑
        API_VERIFY(SetBkMode(hdc, nOldBkMode));
        API_VERIFY(GDI_ERROR != SetTextAlign(hdc, nOldTextAlign));
        API_VERIFY(::MoveToEx(hdc, ptOldPos.x, ptOldPos.y, NULL));
        API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, hOldBrush));
        API_VERIFY(HGDI_ERROR != ::SelectObject(hdc, hOldPen));

        return bRet;
    }

    BOOL CFGdiUtil::SetLogicalMapMode(HDC hdc, LogicalMapMode logicalMapmode)
    {
        int xWinExt = 0;
        int yWinExt = 0;

        switch(logicalMapmode)
        {
        case lmmLoEnglish:
            xWinExt = 100;
            yWinExt = 100;
            break;
        case lmmHiEnglish:
            xWinExt = 1000;
            yWinExt = 1000;
            break;
        case lmmLoMetric:
            xWinExt = 254;
            yWinExt = 254;
            break;
        case lmmHiMetric:
            xWinExt = 2540;
            yWinExt = 2540;
            break;
        case lmmTwips:
            xWinExt = 1440;
            yWinExt = 1440;
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }

        BOOL bRet =FALSE;
        API_VERIFY(0 != ::SetMapMode(hdc, MM_ANISOTROPIC));
        API_VERIFY(::SetWindowExtEx(hdc, xWinExt,yWinExt, NULL));
        API_VERIFY(::SetViewportExtEx(hdc, 
            ::GetDeviceCaps(hdc, LOGPIXELSX), 
            ::GetDeviceCaps(hdc, LOGPIXELSY), NULL));

        return bRet;
    }

	BOOL CFGdiUtil::SaveBitmapToFile(HBITMAP hBmp, LPCTSTR pszFilePath)
	{
		//BOOL bRet = FALSE;
		//HRESULT hr = E_FAIL;
		//
		//CImage img;
		//img.Attach(hBmp);//, DIBOR_DEFAULT);
		//COM_VERIFY(img.Save(pszFilePath, GUID_NULL));
		//if (SUCCEEDED(hr))
		//{
		//	bRet = TRUE;
		//}
		//img.Detach();
		//return bRet;

		BOOL bRet = FALSE;
		HDC            hDC;        //设备描述表
		int            iBits;      //当前显示分辨率下每个像素所占字节数
		WORD            wBitCount; //位图中每个像素所占字节数
			
		
		DWORD           dwPaletteSize=0,	//定义调色板大小
			dwBmBitsSize,					//位图中像素字节大小
			dwDIBSize, dwWritten;			//位图文件大小 ， 写入文件字节数
		BITMAP          Bitmap = {0};
		//位图属性结构
		BITMAPFILEHEADER   bmfHdr = {0};
		//位图文件头结构
		BITMAPINFOHEADER   bi = {0};            
		//位图信息头结构 
		LPBITMAPINFOHEADER lpbi = NULL;       
		//指向位图信息头结构
		HANDLE          fh = NULL, hDib = NULL;
		HPALETTE        hPal,hOldPal=NULL;
		//定义文件，分配内存句柄，调色板句柄

		//计算位图文件每个像素所占字节数
		hDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
		iBits = GetDeviceCaps(hDC, BITSPIXEL) * 
			GetDeviceCaps(hDC, PLANES);
		API_VERIFY(DeleteDC(hDC));
		if (iBits <= 1)
			wBitCount = 1;
		else if (iBits <= 4)
			wBitCount = 4;
		else if (iBits <= 8)
			wBitCount = 8;
		else if (iBits <= 24)
			wBitCount = 24;
		else {
			wBitCount = 32;
		}
		//计算调色板大小
		if (wBitCount <= 8)
		{
			dwPaletteSize = (1 <<  wBitCount) *	sizeof(RGBQUAD);
		}
		//设置位图信息头结构
		GetObject(hBmp, sizeof(BITMAP), (LPSTR)&Bitmap);
		bi.biSize            = sizeof(BITMAPINFOHEADER);
		bi.biWidth           = Bitmap.bmWidth;
		bi.biHeight          = Bitmap.bmHeight;
		bi.biPlanes          = 1;
		bi.biBitCount         = wBitCount;
		bi.biCompression      = BI_RGB;
		bi.biSizeImage        = 0;
		bi.biXPelsPerMeter     = 0;
		bi.biYPelsPerMeter     = 0;
		bi.biClrUsed         = 0;
		bi.biClrImportant      = 0;

		dwBmBitsSize = ((Bitmap.bmWidth *
			wBitCount+31)/32)* 4
			*Bitmap.bmHeight ;
		//为位图内容分配内存
		hDib  = GlobalAlloc(GHND,dwBmBitsSize+
			dwPaletteSize+sizeof(BITMAPINFOHEADER));
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
		*lpbi = bi;
		// 处理调色板   
		hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
		if (hPal)
		{
			hDC  = GetDC(NULL);
			hOldPal = SelectPalette(hDC, hPal, FALSE);
			RealizePalette(hDC);
		}
		// 获取该调色板下新的像素值
		GetDIBits(hDC, hBmp, 0, (UINT) Bitmap.bmHeight,
			(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
			+dwPaletteSize,(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
		//恢复调色板   
		if (hOldPal)
		{
			SelectPalette(hDC, hOldPal, TRUE);
			RealizePalette(hDC);
			ReleaseDC(NULL, hDC);
		}
		//创建位图文件    
		fh = CreateFile(pszFilePath, GENERIC_WRITE, 
			0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (fh == INVALID_HANDLE_VALUE)
			return FALSE;
		// 设置位图文件头
		bmfHdr.bfType = 0x4D42;  // "BM"
		dwDIBSize    = sizeof(BITMAPFILEHEADER) 
			+ sizeof(BITMAPINFOHEADER)
			+ dwPaletteSize + dwBmBitsSize;  
		bmfHdr.bfSize = dwDIBSize;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = (DWORD)sizeof
			(BITMAPFILEHEADER) 
			+ (DWORD)sizeof(BITMAPINFOHEADER)
			+ dwPaletteSize;
		// 写入位图文件头
		API_VERIFY(WriteFile(fh, (LPSTR)&bmfHdr, sizeof
			(BITMAPFILEHEADER), &dwWritten, NULL));
		// 写入位图文件其余内容
		API_VERIFY(WriteFile(fh, (LPSTR)lpbi, dwDIBSize, 
			&dwWritten, NULL));
		//清除   
		GlobalUnlock(hDib);
		GlobalFree(hDib);
		CloseHandle(fh);
		return bRet;
	}
	BOOL CFGdiUtil::SaveDCImageToFile(HDC hdc, LPCTSTR pszFilePath)
	{
		BOOL bRet = FALSE;
		HBITMAP hBmp = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
		if (hBmp)
		{
			bRet = SaveBitmapToFile(hBmp, pszFilePath);
		}
		return bRet;
	}

    BitmapProperty::BitmapProperty(const BITMAP& bitmap)
    {
        m_bitmap = bitmap;
        m_pszProperty = NULL;
    }
    BitmapProperty::~BitmapProperty()
    {
        SAFE_DELETE_ARRAY(m_pszProperty);
    }
    LPCTSTR BitmapProperty::GetPropertyString()
    {
        FTLASSERT(m_bitmap.bmWidth != 0 && m_bitmap.bmHeight != 0
            && TEXT("width or height is zero check choose bitmap"));
        //FTLASSERT(m_bitmap.bmBitsPixel >)
        if(!m_pszProperty)
        {
            CFStringFormater formater;
            formater.Format(
                TEXT("Type=%d,Width=%d,Height=%d,WidthBytes=%d,")
                TEXT("Planes=%d,BitsPixel=%d"),
                m_bitmap.bmType,m_bitmap.bmWidth,m_bitmap.bmHeight,m_bitmap.bmWidthBytes,
                m_bitmap.bmPlanes, m_bitmap.bmBitsPixel
                );

            m_pszProperty = formater.Detach();
        }
        return m_pszProperty;
    }

	//////////////////////////////////////////////////////////////////////////
	CFCalcRect* CFCalcRect::GetCalcRectObject( ZoomMode nZoomMode )
	{
		CFCalcRect* pCalcRectObject = NULL;
		switch (nZoomMode)
		{
		case modeAutoFitIfBigSize:
			pCalcRectObject = new CFAutoFitIfBigSizeCalcRect();
			break;
		case modeAutoFit:
			pCalcRectObject = new CFAutoFitCalcRect();
			break;
		default:
			FTLASSERT(FALSE);
			break;
		}
		return pCalcRectObject;
	}

	CRect CFAutoFitCalcRect::GetFitRect( const CRect& rcMargin, const CSize& szContent )
	{
		CRect rcResult = rcMargin;

		INT nWidthAllowed = rcMargin.Width();
		INT nHeightAllowed = rcMargin.Height();
		INT nWidthFactor = szContent.cx;
		INT nHeightFactor = szContent.cy;

		if ( MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor ) < nHeightAllowed )
		{
			INT nHeight = MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor );
			INT nDiff = ( nHeightAllowed - nHeight ) / 2;
			rcResult.top = rcResult.top + nDiff;
			rcResult.bottom = rcResult.bottom - nDiff;
		}
		else
		{
			INT nWidth = MulDiv( nHeightAllowed, nWidthFactor, nHeightFactor );
			INT nDiff = ( nWidthAllowed - nWidth ) / 2;
			rcResult.left = rcResult.left + nDiff;
			rcResult.right = rcResult.right - nDiff;
		}
		return rcResult;
	}

	CRect CFAutoFitIfBigSizeCalcRect::GetFitRect( const CRect& rcMargin, const CSize& szContent )
	{
		CRect rcResult = rcMargin;
		//INT nWidthAllowed, nHeightAllowed, nWidthFactor, nHeightFactor;
		//INT nWidth, nHeight, nDiff;

		INT nWidthAllowed = rcMargin.Width();
		INT nHeightAllowed = rcMargin.Height();
		INT nWidthFactor = szContent.cx;
		INT nHeightFactor = szContent.cy;

		if ( ( nWidthAllowed >= nWidthFactor ) && ( nHeightAllowed >= nHeightFactor ) )
		{
			rcResult.left = rcMargin.left + ( rcMargin.Width() - szContent.cx ) / 2;
			rcResult.top = rcMargin.top + (rcMargin.Height() - szContent.cy ) / 2;
			rcResult.right = rcResult.left + szContent.cx;
			rcResult.bottom = rcResult.top + szContent.cy;
		}
		else
		{
			if ( MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor ) < nHeightAllowed )
			{
				INT nHeight = MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor );
				INT nDiff = ( nHeightAllowed - nHeight ) / 2;
				rcResult.top = rcResult.top + nDiff;
				rcResult.bottom = rcResult.bottom - nDiff;
			}
			else
			{
				INT nWidth = MulDiv( nHeightAllowed, nWidthFactor, nHeightFactor );
				INT nDiff = ( nWidthAllowed - nWidth ) / 2;
				rcResult.left = rcResult.left + nDiff;
				rcResult.right = rcResult.right - nDiff;
			}
		}
		return rcResult;
	}
	//////////////////////////////////////////////////////////////////////////

    CFCanvas::CFCanvas()
    {
        m_hCanvasDC = NULL;
        m_hMemBitmap = NULL;
        m_hOldBitmap = NULL;
        m_pBuffer = NULL;
        m_width = 0;
        m_height = 0;
        m_bpp = 0;
        ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
    }

    CFCanvas::~CFCanvas()
    {
        Release();
    }

    BOOL CFCanvas::Create(HWND hWnd, int width, int heigth, int bpp /* = 32 */)
    {
		UNREFERENCED_PARAMETER(hWnd);
        FTLASSERT( NULL == m_hCanvasDC );
        FTLASSERT( 32 == bpp && TEXT("Now just support 32 bit"));
        
        BOOL bRet = FALSE;
        m_height = FTL_ABS(heigth);
        m_width = FTL_ABS(width);
        m_bpp = bpp;

        ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
        m_bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFO);
        m_bmpInfo.bmiHeader.biWidth = m_width;
        m_bmpInfo.bmiHeader.biHeight = m_height;
        m_bmpInfo.bmiHeader.biPlanes = 1;
        m_bmpInfo.bmiHeader.biBitCount = (WORD)bpp;
        m_bmpInfo.bmiHeader.biCompression = BI_RGB;
        m_bmpInfo.bmiHeader.biSizeImage = ((m_height * m_width * bpp + 31) >> 3) & ~3;
										  //(m_height * m_width * bpp + 31) / 32 * 4 ; 

		//bmpInfo.bmiHeader.biXPelsPerMeter = ::GetDeviceCaps(hdc, LOGPIXELSX);
        //bmpInfo.bmiHeader.biYPelsPerMeter = ::GetDeviceCaps(hdc, LOGPIXELSY);
        //bmpInfo.bmiHeader.biClrUsed = 0;
        //bmpInfo.bmiHeader.biClrImportant = 0;
		
        m_hCanvasDC = ::CreateCompatibleDC(NULL);

        API_VERIFY(NULL != (m_hMemBitmap = ::CreateDIBSection(
			NULL, //使用 DIB_RGB_COLORS 时忽略 HDC 参数，只有使用 DIB_PAL_COLORS 时才使用该参数
			&m_bmpInfo, 
			DIB_RGB_COLORS,
            (VOID**)&m_pBuffer, NULL, 0)));

        ZeroMemory(m_pBuffer, m_bmpInfo.bmiHeader.biSizeImage);

        m_hOldBitmap = (HBITMAP)::SelectObject(m_hCanvasDC, (HGDIOBJ)m_hMemBitmap);
        return bRet;
    }

    VOID CFCanvas::Release()
    {
        if(m_hCanvasDC)
        {
            BOOL bRet = FALSE;
            ::SelectObject(m_hCanvasDC, m_hOldBitmap);
            API_VERIFY(::DeleteObject(m_hMemBitmap));
            API_VERIFY(::DeleteDC(m_hCanvasDC));

            m_hCanvasDC = NULL;
            m_hMemBitmap = NULL;
            m_hOldBitmap = NULL;
            m_pBuffer = NULL;
            m_width = 0;
            m_height = 0;
            m_bpp = 0;
            ZeroMemory(&m_bmpInfo, sizeof(m_bmpInfo));
        }
    }

    BOOL CFCanvas::IsCanvasChanged( const RECT& rc , int bpp /* = 32*/ )
    {
        FTLASSERT(rc.right >= rc.left);
        FTLASSERT(rc.bottom >= rc.top);
        BOOL bRet = FALSE;
        if (rc.right - rc.left != m_width 
            || rc.bottom - rc.top != m_height
            || bpp != m_bpp )
        {
            bRet = TRUE;
        }
        return bRet;
    }

    HANDLE CFCanvas::CopyToHandle()
    {
        HANDLE hDib = NULL;
        if (m_pBuffer)
        {
            hDib = GlobalAlloc(GHND, GetSize());
            BYTE* pDst = (BYTE*)GlobalLock(hDib);
            if (pDst)
            {
                memcpy(pDst, &m_bmpInfo, m_bmpInfo.bmiHeader.biSize);
                pDst += m_bmpInfo.bmiHeader.biSize;
                memcpy(pDst, m_pBuffer, m_bmpInfo.bmiHeader.biSizeImage);
                GlobalUnlock(hDib);
            }
        }
        return hDib;
    }

    DWORD CFCanvas::GetSize()
    {
        DWORD dwSize = m_bmpInfo.bmiHeader.biSize + m_bmpInfo.bmiHeader.biSizeImage;
        return dwSize;
    }

#ifdef __ATLGDI_H__

    CFScrollZoomMemoryDC::CFScrollZoomMemoryDC(HDC hDC, RECT* pRectPaint)  //rcClip
        :m_hDCOriginal(hDC)
        ,m_hBmpOld(NULL)
    {
        BOOL bRet = FALSE;
        if (NULL == pRectPaint)
        {
            API_VERIFY(::GetClipBox(m_hDCOriginal, &m_rcPaint));
        }
        else
        {
            m_rcPaint = *pRectPaint;
        }
		FTLASSERT(m_rcPaint.right > m_rcPaint.left);
		FTLASSERT(m_rcPaint.bottom > m_rcPaint.top);

        m_hDC = ::CreateCompatibleDC(m_hDCOriginal);
        API_ASSERT(NULL != m_hDC);

		//change to device and create bitmap
        API_VERIFY(::LPtoDP(m_hDCOriginal, (LPPOINT)&m_rcPaint, 2));
        m_hBmp = ::CreateCompatibleBitmap(m_hDCOriginal, 
            m_rcPaint.right - m_rcPaint.left, 
            m_rcPaint.bottom - m_rcPaint.top);
        API_VERIFY(m_hBmp != NULL);

        m_hBmpOld = (HBITMAP)::SelectObject(m_hDC, m_hBmp);

        API_VERIFY(0 !=::SetMapMode(m_hDC, ::GetMapMode(m_hDCOriginal)));

#if 0
#pragma TODO(是否需要这部分的代码--Capture中没有)
        SIZE szWindowExt = {0,0};
        API_VERIFY(::GetWindowExtEx(m_hDCOriginal, &szWindowExt));
        API_VERIFY(::SetWindowExtEx(m_hDC, szWindowExt.cx, szWindowExt.cy, NULL));

        SIZE szViewportExt = {0,0};
        API_VERIFY(::GetViewportExtEx(m_hDCOriginal, &szViewportExt));
        API_VERIFY(::SetViewportExtEx(m_hDC, szViewportExt.cx, szViewportExt.cy, NULL));
#endif

        API_VERIFY(::DPtoLP(m_hDCOriginal, (LPPOINT)&m_rcPaint, 2));


        //也不需要? CMemDC Use this -- Maybe better, because WTL::CScrollZoomImpl will Call SetViewportOrg
        //SetWindowOrg(m_rcPaint.left, m_rcPaint.top);
        ////CMemoryDC Use this
        //SetViewportOrg(-m_rcPaint.left, -m_rcPaint.top);
        //WTL.CZoomScrollImpl.PrepareDC will Use this
        //dc.SetViewportOrg(-m_ptOffset.x, -m_ptOffset.y);
    }
    CFScrollZoomMemoryDC::~CFScrollZoomMemoryDC()
    {
        ::BitBlt(m_hDCOriginal, m_rcPaint.left, m_rcPaint.top, 
            m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top, 
            m_hDC, m_rcPaint.left, m_rcPaint.top, 
            SRCCOPY);
        ::SelectObject(m_hDC, m_hBmpOld);
        ::DeleteObject(m_hBmp);
    }
    //operator CFScrollZoomMemoryDC::CFScrollZoomMemoryDC*()
    //{
    //    return this;
    //}
    //CFScrollZoomMemoryDC* CFScrollZoomMemoryDC::operator->()
    //{
    //    return this;
    //}
#endif //__ATLGDI_H__
}

#endif //FTL_GDI_HPP