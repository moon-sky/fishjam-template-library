#include "stdafx.h"
#include "NPVPhotoCalcRect.h"
#include "resource.h"
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
		//m_Image.LoadFromResource(_Module.GetResourceInstance(), IDR_MAINFRAME);
	}
	CImage* ZipManager::LoadCImage(LPCTSTR pszPath)
	{
		if (m_Image.IsNull())
		{
			FTL::CFGdiUtil::LoadPNGFromResource(m_Image, _Module.GetResourceInstance(), IDB_PNG1, TEXT("PNG"));
		}
		return &m_Image;
	}

}

CNPVPhotoCalcRect::CNPVPhotoCalcRect(const CRect& rect)
{
	m_rcView = rect;
}

CRect CNPVPhotoCalcRect::GetPhotoRect(CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst, BOOL bFirstView)
{
	CRect rcPhoto;
	INT nTmp;
	if ( bFirstView )
	{
		rcPhoto.left = ( rcClient.Width() - lpSize->cx ) / 2;
		if ( rcPhoto.left < 0 )
		{
			m_bZoomFirstView = TRUE;
			rcPhoto.left = 0;
		}
		rcPhoto.top = ( rcClient.Height() - lpSize->cy ) / 2;
		if ( rcPhoto.top < 0 )
		{
			m_bZoomFirstView = TRUE;
			rcPhoto.top = 0;
		}
		rcPhoto.right = rcPhoto.left + lpSize->cx;
		rcPhoto.bottom = rcPhoto.top + lpSize->cy;
	}
	else if ( bFirst )
	{
		rcPhoto.left = ( rcClient.Width() - lpSize->cx ) / 2;
		rcPhoto.top = ( rcClient.Height() - lpSize->cy ) / 2;
		rcPhoto.right = rcPhoto.left + lpSize->cx;
		rcPhoto.bottom = rcPhoto.top + lpSize->cy;
	}
	else
	{
		rcPhoto.left = ( rcClient.Width() - lpSize->cx ) / 2;
		rcPhoto.top = ( rcClient.Height() - lpSize->cy ) / 2;
		rcPhoto.right = rcPhoto.left + lpSize->cx;
		rcPhoto.bottom = rcPhoto.top + lpSize->cy;

		if ( rcPhoto.Width() > rcClient.Width() )
		{
			nTmp = rcPhoto.Width();
			if ( m_rcView.left > rcClient.left )
			{
				rcPhoto.left = rcClient.left;
				rcPhoto.right = rcPhoto.left + nTmp;
			}
			else if ( m_rcView.right < rcClient.right )
			{
				rcPhoto.right = rcClient.right;
				rcPhoto.left = rcPhoto.right - nTmp;
			}
			else
			{
				rcPhoto.left = m_rcView.left;
				rcPhoto.right = rcPhoto.left + nTmp;
			}
		}
		if ( rcPhoto.Height() > rcClient.Height() )
		{
			nTmp = rcPhoto.Height();
			if ( m_rcView.top > rcClient.top )
			{
				rcPhoto.top = rcClient.top;
				rcPhoto.bottom = rcPhoto.top + nTmp;
			}
			else if ( m_rcView.bottom < rcClient.bottom )
			{
				rcPhoto.bottom = rcClient.bottom;
				rcPhoto.top = rcPhoto.bottom - nTmp;
			}
			else
			{
				rcPhoto.top = m_rcView.top;
				rcPhoto.bottom = rcPhoto.top + nTmp;
			}
		}
	}
	m_rcView = rcPhoto;
	m_dOldFactor = ( DOUBLE )m_rcView.Width() / ( DOUBLE )lpSize->cx;
	return rcPhoto;
}

CNPVPhotoCalcRect* CNPVPhotoCalcRect::GetPhotoCalcRectObject(ZoomType, CRect* pRect)
{
	CNPVPhotoCalcRect* pNewRect = new CNPVPhotoCalcRect(pRect);
	
	return pNewRect;
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
