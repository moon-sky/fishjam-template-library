
#ifndef FTL_GDI_H
#define FTL_GDI_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlGdi.h requires ftlbase.h to be included first
#endif


namespace FTL
{

#define RGBA(r,g,b,a)       ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24)))
#define GetAValue(rgba)     (LOBYTE((rgba)>>24))

#ifdef FTL_DEBUG

    //Check Bitmap Can select into DC
	//  1.isValidate
	//  2.have not select into other DC
#define CHECK_BITMAP_SELECTABLE(h)   \
    {\
		FTL::CFGdiObjectChecker checker;\
        FTLASSERT(checker.IsValidate(h));\
        FTLASSERT(!checker.IsSelected(h));\
    }

#define CHECK_GDIOBJECT_VALIDATE(h) \
    {\
		FTL::CFGdiObjectChecker checker;\
        FTLASSERT(checker.IsValidate(h));\
    }
#else
    #define CHECK_BITMAP_SELECTABLE(h)      __noop
    #define CHECK_GDIOBJECT_VALIDATE(h)     __noop
#endif 


    class CFGdiObjectInfoDump
    {
    public:
        FTLINLINE CFGdiObjectInfoDump();
        FTLINLINE ~CFGdiObjectInfoDump();
        FTLINLINE BOOL GetGdiObjectInfo(HGDIOBJ hgdiObj);
        FTLINLINE LPCTSTR GetGdiObjectInfoString();
    private:
        LPCTSTR m_pszGdiObjectInfo;
    };

    class CFGdiObjectChecker
    {
    public:
        FTLINLINE CFGdiObjectChecker(HDC hDCTarget = NULL);
        FTLINLINE ~CFGdiObjectChecker();
        FTLINLINE BOOL IsValidate(HGDIOBJ hGdiObj);
        FTLINLINE BOOL IsSelected(HBITMAP hGdiObj);
    private:
        HDC     m_hDCCompatible;
    };

	FTLEXPORT class CFGdiUtil
	{
	public:
		FTLINLINE static BOOL   DrawCoordinate(HDC hdc, const RECT& rcClient, BOOL bDrawClipbox = FALSE);
	};

    template <typename T>
    FTLEXPORT class CFGdiResourceGuardT
    {
    public:
        FTLINLINE CFGdiResourceGuardT(HDC hdc, T hNewObject);
        FTLINLINE ~CFGdiResourceGuardT();
        BOOL      IsValide();
    private:
        HDC m_hDC;
        T   m_hOldObject;
    };

    template <typename T>
    CFGdiResourceGuardT<T>::CFGdiResourceGuardT(HDC hdc, T hNewObject)
        :m_hDC(hdc)
    {
        m_hOldObject = (T)::SelectObject(hdc, (T)hNewObject);
    }

    template <typename T>
    CFGdiResourceGuardT<T>::~CFGdiResourceGuardT()
    {
        ::SelectObject(m_hDC, m_hOldObject);
    }

    template <typename T>
    BOOL CFGdiResourceGuardT<T>::IsValide()
    {
        return (NULL != m_hOldObject && HGDI_ERROR != m_hOldObject);
    }

	class CFMMTextDCGuard
	{
	public:
		CFMMTextDCGuard(HDC hdc)
			:m_hDC(hdc)
		{
			BOOL bRet = FALSE;
			ATLASSERT(m_hDC);
			API_VERIFY(::SetWindowOrgEx(m_hDC, 0,0, &m_ptOldWindowOrg));
			API_VERIFY(::SetViewportOrgEx(m_hDC, 0,0, &m_ptOldViewPortOrg));
			API_VERIFY(::SetViewportExtEx(m_hDC, 1,1, &m_szOldViewPortExt));
			API_VERIFY(::SetWindowExtEx(m_hDC, 1,1,&m_szOldWindowExt));
			m_nOldMapMode = ::SetMapMode(m_hDC, MM_TEXT);

			//FTLTRACE(TEXT("CFMMTextDCGuard, MapMode=%d,")
			//	TEXT("szOldWindowExt={%d,%d}, szOldViewPortExt={%d,%d},")
			//	TEXT("ptOldWindowOrg=[%d,%d], ptOldViewPortOrg=[%d,%d]\"),
			//	m_nOldMapMode,
			//	m_szOldWindowExt.cx, m_szOldWindowExt.cy, m_szOldViewPortExt.cx, m_szOldViewPortExt.cy,
			//	m_ptOldWindowOrg.x, m_ptOldWindowOrg.y,m_ptOldViewPortOrg.x, m_ptOldViewPortOrg.y);

		}
		~CFMMTextDCGuard()
		{
			BOOL bRet = FALSE;
			::SetMapMode(m_hDC, m_nOldMapMode);
			API_VERIFY(::SetWindowExtEx(m_hDC, m_szOldWindowExt.cx,m_szOldWindowExt.cy,NULL));
			API_VERIFY(::SetViewportExtEx(m_hDC, m_szOldViewPortExt.cx,m_szOldViewPortExt.cy, NULL));
			API_VERIFY(::SetViewportOrgEx(m_hDC, m_ptOldViewPortOrg.x, m_ptOldViewPortOrg.y, NULL));
			API_VERIFY(::SetWindowOrgEx(m_hDC, m_ptOldWindowOrg.x,m_ptOldWindowOrg.y, NULL));
		}
	private:
		HDC		m_hDC;
		int		m_nOldMapMode;
		SIZE	m_szOldWindowExt;
		SIZE    m_szOldViewPortExt;
		POINT   m_ptOldViewPortOrg;
		POINT   m_ptOldWindowOrg;
	};
}

#endif //FTL_GDI_H

#ifndef USE_EXPORT
#  include "ftlGdi.hpp"
#endif