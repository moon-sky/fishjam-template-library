#include "stdafx.h"
#include "NPVPhotoCalcRect.h"

namespace NDGraphics
{
	void DrawGdiplusImage(HDC hdc, CGDIPImage* pImage, int l, int t, int w, int h)
	{
		pImage->Draw(hdc, l, t, w, h);
	}
}

namespace SilverlightCpp
{
	ZipManager* ZipManager::get_Current()
	{
		static ZipManager	s_ZipManager;
		return &s_ZipManager;
	}
	ZipManager::ZipManager()
	{
		m_Image.LoadFromResource(_Module.GetResourceInstance(), IDR_MAINFRAME);
	}
	CImage* ZipManager::LoadCImage(LPCTSTR pszPath)
	{
		return &m_Image;
	}

}

CRect CNPVPhotoCalcRect::GetPhotoRect(CRect rc, SIZE* pSize, float zoom, BOOL b1, BOOL b2)
{
	return rc;
}

CNPVPhotoCalcRect* CNPVPhotoCalcRect::GetPhotoCalcRectObject(ZoomType, CRect* pRect)
{
	return new CNPVPhotoCalcRect();
}

CStringResouceMgr* CStringResouceMgr::Instance()
{
	static CStringResouceMgr	s_StringResourceMgr;
	return &s_StringResourceMgr;
}

tstring CStringResouceMgr::GetStringData(LPCTSTR pszName)
{
	return tstring(pszName);
}

CNCaptureDoc::CNCaptureDoc()
{
	m_pCapImageObj = NULL;
}

CCapImageObj* CNCaptureDoc::GetCurCaptureImage()
{
	return m_pCapImageObj;
}

void CNCaptureDoc::SetCurCaptureImage(CCapImageObj* pCapImageObj)
{
	SAFE_DELETE(m_pCapImageObj);
	m_pCapImageObj = pCapImageObj;
	__raise OnCaptureImageChanged();
}
int CNCaptureDoc::GetCaptureCount()
{
	return !m_pCapImageObj ? 1 : 0;
}

CNCaptureApplication* CNCaptureApplication::Instance()
{
	static CNCaptureApplication s_CaptureApp;
	return &s_CaptureApp;
}
CNCaptureDoc* CNCaptureApplication::GetDocument()
{
	return &m_Doc;
}
CMainFrame*	CNCaptureApplication::GetMainFrame()
{
	return m_pMainFrame;
}


CFMMTextDCGuard::CFMMTextDCGuard(HDC hdc) 
  : FTL::CFMMTextDCGuard(hdc)
{

}

HDC CCanvas::GetMemoryDC()
{
	return GetCanvasDC();
}
