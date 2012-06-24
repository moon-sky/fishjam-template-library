#pragma once

#include "CapImageObj.h"
#include "ftlgdi.h"

class CMainFrame;

struct STATUSBARINFO 
{
	CRect rcSelect;
	POINT ptMouse;
	CSize csImageSize;
	BOOL  bEnable;
};

namespace NDGraphics
{
	class CGDIPImage : public CImage
	{
	public:
	};

	void DrawGdiplusImage(HDC hdc, CGDIPImage* pImage, int l, int t, int w, int h);
};

namespace SilverlightCpp
{
	class ZipManager
	{
	public:
		static ZipManager* get_Current();
		ZipManager();
		CImage* LoadCImage(LPCTSTR pszPath);
	private:
		CImage m_Image;
	};
}

class CNPVPhotoCalcRect
{
public:
	enum ZoomType
	{
		E_ZOOM_ORIGINAL,
	};
	CNPVPhotoCalcRect(const CRect& rect);
	CRect GetPhotoRect(CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst, BOOL bFirstView);
	static CNPVPhotoCalcRect* GetPhotoCalcRectObject(ZoomType, CRect* pRect);
private:
	CRect m_rcView;
	BOOL		m_bZoomFirstView;
	DOUBLE	m_dOldFactor;
};

class CStringResouceMgr
{
public:
	static CStringResouceMgr* Instance();
	tstring GetStringData(LPCTSTR pszName);
};

class CUtil
{
public:
	static LPCTSTR GetSaveImageExtByGuid(const GUID& guid)
	{
		return TEXT("PNG");
	}
	static CString GetPhotoViewerInstallDir()
	{
		return TEXT("");
	}
};
class SettingInfo
{
public:
	SettingInfo()
	{
		guidDefaultSaveFileFormat = Gdiplus::ImageFormatPNG;
	}
	GUID	guidDefaultSaveFileFormat;

};

class CNCaptureDoc
{
public:
	__event void OnCaptureImageChanged();

	CNCaptureDoc();
	CCapImageObj* GetCurCaptureImage();
	void SetCurCaptureImage(CCapImageObj* pCapImageObj);
	int GetCaptureCount();
	SettingInfo* GetSettingInfo() { return &m_SettingInfo; }
	void SetCaptureImage(CCapImageObj* pImageObj)
	{

	}
private:
	CCapImageObj*	m_pCapImageObj;
	SettingInfo		m_SettingInfo;
};

class CNCaptureApplication
{
public:
	static CNCaptureApplication* Instance();
	CNCaptureDoc* GetDocument();
	CMainFrame*	GetMainFrame();
	CMainFrame* m_pMainFrame;
	CNCaptureDoc	m_Doc;
};


class CFMMTextDCGuard : public FTL::CFMMTextDCGuard
{
public:
	explicit CFMMTextDCGuard(HDC hdc);
};

class CCanvas : public FTL::CFCanvas
{
public:
	HDC GetMemoryDC();
};

class CaptureUtil
{
public:
	void static BmpSetAlpha(HBITMAP hBmp, INT nValue);
};