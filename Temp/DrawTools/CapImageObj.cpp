#include "StdAfx.h"
#include "CapImageObj.h"
#include "NPVPhotoCalcRect.h"
#ifdef DRAW_TOOL_TEST
#else
#include "../Capture/AlphaBitmapUI/Canvas.h"
#endif
#include "ftlBase.h"

#include "ftlFunctional.h"
#include <algorithm>

#include <atltime.h>

CCapImageObj::CCapImageObj(LPCTSTR pszObjectName)
{
	m_flZoom	= 1.00;
	m_nZoomMode = CNPVPhotoCalcRect::E_ZOOM_ORIGINAL;
	m_ptPosition.SetPoint( 0, 0 );

	m_bImageSaved = TRUE;
	m_bActive     = TRUE;
	m_strCacheFileName = _T("");
	m_strObjectName = pszObjectName;
	m_strEditTmpFile = _T("");
	m_strFileName    = _T("");
	m_uDrawDataIndex = 1;
	m_arDrawData.clear();
	CDrawDataInfo* pDataInfo = new CDrawDataInfo();
	m_arDrawData.push_back(pDataInfo);
}

CCapImageObj::~CCapImageObj(void)
{
	if (!m_bActive)
	{
		DeleteFile(m_strCacheFileName);
	}
	else
	{
		Destroy();
	}

	if (!m_strEditTmpFile.IsEmpty())
	{
		DeleteFile(m_strEditTmpFile);
	}

	for (size_t i = 0; i < m_arDrawData.size(); i++)
	{
		CDrawDataInfo* pDataInfo = m_arDrawData[i];
		if (pDataInfo)
		{
			delete pDataInfo;
			pDataInfo = NULL;
		}
	}
	m_arDrawData.clear();
}


CPoint CCapImageObj::GetPosition()
{
	return m_ptPosition;
}

void CCapImageObj::SetPosition( INT32 x, INT32 y )
{
	m_ptPosition.x = x;
	m_ptPosition.y = y;
}

float CCapImageObj::GetZoom()
{
	return m_flZoom;
}
void CCapImageObj::SetZoom( float flZoom )
{
	m_flZoom = flZoom;
}
INT CCapImageObj::GetZoomMode()
{
	return m_nZoomMode;
}
void CCapImageObj::SetZoomMode( INT nZoomMode )
{
	m_nZoomMode = nZoomMode;
}

HANDLE CCapImageObj::CopyToHandle(const CRect& rcSrc)
{
	BOOL bRet = FALSE;
	HANDLE hMem = NULL;
	CRect rcImage(0, 0, GetWidth(), GetHeight());
	CRect rcRealSrc;
	rcRealSrc.IntersectRect(&rcSrc, rcImage);
	if (!rcRealSrc.IsRectEmpty())
	{
		 CCanvas canvas;
		 bRet = canvas.Create(NULL, rcRealSrc.Width(), rcRealSrc.Height());
		if (bRet)
		{
			BitBlt(canvas.GetMemoryDC(), CRect(CPoint(0), rcRealSrc.Size()), rcRealSrc.TopLeft());
			hMem = canvas.CopyToHandle();
		}
	}
	return hMem;
}

BOOL CCapImageObj::SaveImageFile(LPCTSTR pszFilePath, BOOL bChanageState/* = TRUE*/)
{
	BOOL bRet = FALSE;

	if (!m_bActive)
	{
		if(m_strCacheFileName.IsEmpty() || Load(m_strCacheFileName) != S_OK)
		{
			return FALSE;
		}
	}
	if (Save(pszFilePath, GUID_NULL) == S_OK)
	{
		bRet = TRUE;
	}
	//CCanvas canvas24;
	//CRect rectImage(0, 0, GetWidth(), GetHeight());
	//API_VERIFY(canvas24.Create(NULL, rectImage.Width(), rectImage.Height(), 24));
	//if (bRet)
	//{
	//	API_VERIFY(BitBlt(canvas24.GetMemoryDC(), 0, 0));
	//	CImage image;
	//	image.Attach(canvas24.GetBitmap());
	//	HRESULT hr = E_FAIL;
	//	COM_VERIFY(image.Save(pszFilePath, GUID_NULL));
	//	if (SUCCEEDED(hr))
	//	{
	//		bRet = TRUE;
	//	}
	//	image.Detach();
	//}
	if (bChanageState)
	{
		m_bImageSaved = !bRet;
	}
	
	//SetCapImageObjActive(m_bActive);
	return bRet;
}

BOOL CCapImageObj::_SaveImageToCache()
{
	BOOL bRet = FALSE;
	TCHAR appDataPath[MAX_PATH] = {0};
	API_VERIFY(SHGetSpecialFolderPath(NULL, appDataPath, CSIDL_APPDATA , TRUE));
	CString strFilePath(appDataPath);
	strFilePath += TEXT("\\nCapture\\");
	if (bRet)
	{
		SHCreateDirectoryEx(NULL, strFilePath, NULL);
		CTime CurTime = CTime::GetCurrentTime();
		CString strTime = CurTime.Format(_T("%Y-%m-%d-%H-%M-%S"));
		strFilePath.AppendFormat(TEXT("%s_%s_%d%s"), TEXT("TmpFile"), strTime, GetTickCount(), TEXT(".PNG"));	

		while(!_taccess(strFilePath, 0))
		{
			strFilePath = appDataPath;
			strFilePath += TEXT("\\nCapture\\");
			SHCreateDirectoryEx(NULL, strFilePath, NULL);
			strFilePath.AppendFormat(TEXT("%s_%s_%d%s"), TEXT("TmpFile"), strTime, GetTickCount(), TEXT(".PNG"));		
			Sleep(100);
		}
		// if (SaveImageFile(strFilePath, FALSE))
		if (SUCCEEDED(Save(strFilePath, Gdiplus::ImageFormatPNG)))
		{
			m_bActive = FALSE;
			m_strCacheFileName = strFilePath;
			bRet = TRUE;
			Destroy();
		}
		else
		{
			m_bActive = TRUE;
			//m_strCacheFileName = strFilePath;
			bRet = FALSE;
		}
	}
	return bRet;
}


BOOL CCapImageObj::SetCapImageObjActive(BOOL bActive/* = TRUE*/)
{
	if (bActive)
	{
		if(!m_strCacheFileName.IsEmpty() && Load(m_strCacheFileName) == S_OK)
		{
			m_bActive = TRUE;
			DeleteFile(m_strCacheFileName);
			m_strCacheFileName = _T("");
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return _SaveImageToCache();
	}
}

LPCTSTR CCapImageObj::GetCaptureImageObjectName()
{
	return m_strObjectName;
}

void CCapImageObj::SetCaptureImageObjectName(LPCTSTR strObjectName)
{
	m_strObjectName = strObjectName;
}

LPCTSTR CCapImageObj::GetFileName()
{
	if (m_strFileName.IsEmpty())
	{
		m_strFileName = m_strObjectName + _T(".png");
	}
	return m_strFileName;
}

void CCapImageObj::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

BOOL CCapImageObj::CropImage(CRect& rcSrc)
{
	if (rcSrc.IsRectNull() && rcSrc.Width() > GetWidth() && rcSrc.Height() > GetHeight())
	{
		return FALSE;
	}
	CImage tmpImage;
	tmpImage.Create(GetWidth(), GetHeight(), 32, CImage::createAlphaChannel);
	Draw(CImageDC(tmpImage), 0, 0);
	Destroy();
	Create(rcSrc.Width(), rcSrc.Height(), 32, CImage::createAlphaChannel);
	int xSrc = rcSrc.left;
	tmpImage.Draw(CImageDC(*this), 0, 0, GetWidth(), GetHeight(), rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height());
	m_bImageSaved = TRUE;
	return TRUE;
}

void CCapImageObj::SetEditTmpFile(LPCTSTR strEditTmpFile)
{
	if (!m_strEditTmpFile.IsEmpty() && m_strEditTmpFile.CompareNoCase(strEditTmpFile) != 0)
	{
		DeleteFile(m_strEditTmpFile);
	}
	m_strEditTmpFile = strEditTmpFile;
}

//void CCapImageObj::SetDrawObjectInfo(const DrawObjectList& arDrawObject, const DrawObjectList& arSelectObject)
//{
//	m_DrawObjectList.clear();
//	std::copy(arDrawObject.begin(), arDrawObject.end(), std::back_inserter(m_DrawObjectList));
//
//	m_SelectObjectList.clear();
//	std::copy(arSelectObject.begin(), arSelectObject.end(), std::back_inserter(m_SelectObjectList));
//}
//
//void CCapImageObj::GetDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject)
//{
//	arDrawObject.clear();
//	std::copy(m_DrawObjectList.begin(), m_DrawObjectList.end(), std::back_inserter(arDrawObject));
//
//	arSelectObject.clear();
//	std::copy(m_SelectObjectList.begin(), m_SelectObjectList.end(), std::back_inserter(arSelectObject));
//}

BOOL CCapImageObj::PushDrawObjectInfo(const DrawObjectList& arDrawObject, const DrawObjectList& arSelectObject, LPCTSTR strName)
{
	if (m_arDrawData.size() > 0 && m_uDrawDataIndex != m_arDrawData.size())
	{
		if (m_uDrawDataIndex < m_arDrawData.size())
		{
			CDrawDataArray::iterator it = m_arDrawData.begin();
			std::advance(it, m_uDrawDataIndex);
			//while(it != m_arDrawData.end())
			//{
			//	CDrawDataInfo* pDataInfo = *it;
			//	m_arDrawData.erase(it);
			//	delete pDataInfo;
			//	pDataInfo = NULL;
			//	it++;
			//}
			std::for_each(it, m_arDrawData.end(), FTL::ObjecteDeleter<CDrawDataInfo*>());
			m_arDrawData.erase(it, m_arDrawData.end());
			CDrawDataInfo* pDataInfo = new CDrawDataInfo(arDrawObject, arSelectObject, strName);
			m_arDrawData.push_back(pDataInfo);
			m_uDrawDataIndex = m_arDrawData.size();
		}
	}
	else
	{
		CDrawDataInfo* pDataInfo = new CDrawDataInfo(arDrawObject, arSelectObject, strName);
		m_arDrawData.push_back(pDataInfo);
		m_uDrawDataIndex = m_arDrawData.size();
	}

	return FALSE;
}

BOOL CCapImageObj::GetPrevDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject)
{
	if (!IsFirstDrawObjectInfo())
	{
		m_uDrawDataIndex --;
		CDrawDataInfo* pDataInfo = m_arDrawData[m_uDrawDataIndex - 1];
		if (pDataInfo)
		{
			return pDataInfo->CopyDataInfo(arDrawObject, arSelectObject);
		}
	}
	return FALSE;
}

BOOL CCapImageObj::GetNextDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject)
{
	if (!IsFinalDrawObjectInfo())
	{
		m_uDrawDataIndex ++;
		CDrawDataInfo* pDataInfo = m_arDrawData[m_uDrawDataIndex - 1];
		if (pDataInfo)
		{
			return pDataInfo->CopyDataInfo(arDrawObject, arSelectObject);
		}
	}
	return FALSE;
}

BOOL CCapImageObj::GetCurrentObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject)
{
	arSelectObject.clear();

	for_each(arDrawObject.begin(), arDrawObject.end(), FTL::ObjecteDeleter<CDrawObject*>());
	arDrawObject.clear();

	if (m_arDrawData.size() > 0 && m_uDrawDataIndex <= m_arDrawData.size())
	{
		CDrawDataInfo* pDataInfo = m_arDrawData[m_uDrawDataIndex - 1];
		if (pDataInfo)
		{
			return pDataInfo->CopyDataInfo(arDrawObject, arSelectObject);
		}
	}
	return FALSE;
}

BOOL CCapImageObj::IsFirstDrawObjectInfo()
{
	if (m_arDrawData.size() > 0 && m_uDrawDataIndex == 1)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CCapImageObj::IsFinalDrawObjectInfo()
{
	if (m_arDrawData.size() > 0 && m_uDrawDataIndex == m_arDrawData.size())
	{
		return TRUE;
	}
	return FALSE;
}