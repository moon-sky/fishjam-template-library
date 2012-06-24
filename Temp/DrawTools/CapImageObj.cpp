#include "StdAfx.h"
#include "CapImageObj.h"
#include "NPVPhotoCalcRect.h"
#ifndef DRAW_TOOL_TEST
#include "../Capture/AlphaBitmapUI/Canvas.h"
#include "NCaptureApplication.h"
#include "nCaptureDoc.h"
#include "Util.h"
#else
#endif
#include "ftlBase.h"
#include "ftlFunctional.h"
#include <algorithm>
#include <atltime.h>
#include "DrawTools/DrawObject.h"
#include "nCaptureView.h"

CCapImageObj::CCapImageObj(LPCTSTR pszObjectName, HBITMAP hbmp)
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

	Attach(hbmp, CImage::DIBOR_BOTTOMUP);

	_SaveImageToCache();

	CDrawDataInfo* pDataInfo = new CDrawDataInfo(m_strCacheFileName);
	m_arDrawData.push_back(pDataInfo);
	m_strLastSaveName = _T("");
}

CCapImageObj::~CCapImageObj(void)
{
	SetCapImageObjActive(FALSE);

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

	BOOL bActive = m_bActive; 
	if (!m_bActive && !SetCapImageObjActive(TRUE))
	{
		return FALSE;
	}

	SycDrawObjects();

	CaptureUtil::BmpSetAlpha((HBITMAP)*this, 255);

	if (Save(pszFilePath, Gdiplus::ImageFormatPNG) == S_OK)
	{
		bRet = TRUE;
	}
	SetCapImageObjActive(FALSE);
	SetCapImageObjActive(bActive);

	if (bChanageState)
	{
		m_strLastSaveName = pszFilePath;
		m_bImageSaved = !bRet;
	}
	
	//SetCapImageObjActive(m_bActive);
	return bRet;
}

BOOL CCapImageObj::SaveImageFile()
{
	if (!m_strLastSaveName.IsEmpty())
	{
		return SaveImageFile(m_strLastSaveName, TRUE);
	}
	return FALSE;
}


BOOL CCapImageObj::_SaveImageToCache(BOOL bNewFile)
{
	if (!bNewFile && !m_strCacheFileName.IsEmpty() && !_taccess(m_strCacheFileName, 0))
	{
		return TRUE;
	}
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
			//m_bActive = FALSE;
			m_strCacheFileName = strFilePath;
			bRet = TRUE;
			//Destroy();
		}
		else
		{
			//m_bActive = TRUE;
			m_strCacheFileName = _T("");
			bRet = FALSE;
		}
	}
	return bRet;
}


BOOL CCapImageObj::SetCapImageObjActive(BOOL bActive/* = TRUE*/)
{
	if (bActive)
	{
		if(!m_bActive && Load(m_strCacheFileName) == S_OK)
		{
			m_bActive = TRUE;
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if (_SaveImageToCache())
		{
			m_bActive = FALSE;
			Destroy();
			return TRUE;
		}
	}
	return FALSE;
}

LPCTSTR CCapImageObj::GetCaptureImageObjectName()
{
	return m_strObjectName;
}

void CCapImageObj::SetCaptureImageObjectName(LPCTSTR strObjectName)
{
	m_strObjectName = strObjectName;
}

CString CCapImageObj::GetFileName()
{
	if (m_strFileName.IsEmpty())
	{
		const GUID& defaultSaveFormat = CNCaptureApplication::Instance()->GetDocument()->GetSettingInfo()->guidDefaultSaveFileFormat;
		CString strTempName = m_strObjectName + _T(".") +  CUtil::GetSaveImageExtByGuid(defaultSaveFormat);
		return strTempName;
	}
	return m_strFileName;
}

void CCapImageObj::SetFileName(LPCTSTR strFileName)
{
	m_strFileName = strFileName;
}

BOOL CCapImageObj::CropImage(CRect& rcSrc)
{
	if (rcSrc.IsRectEmpty() && rcSrc.Width() > GetWidth() && rcSrc.Height() > GetHeight())
	{
		return FALSE;
	}
	SycDrawObjects();

	CImage tmpImage;
	tmpImage.Create(rcSrc.Width(), rcSrc.Height(), 32, CImage::excludePNG);
	Draw(CImageDC(tmpImage), 0, 0, rcSrc.Width(), rcSrc.Height(), rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height());

	Destroy();
	Attach(tmpImage.Detach(), CImage::DIBOR_BOTTOMUP);
	_SaveImageToCache(TRUE);

	DrawObjectList arDrawObject, arSelectObject;
	CDrawDataInfo* pDataInfo = new CDrawDataInfo(arDrawObject, arSelectObject, _T("Crop"), m_strCacheFileName);
	m_arDrawData.push_back(pDataInfo);
	m_uDrawDataIndex ++;
	m_bImageSaved = TRUE;
	return TRUE;
}

void CCapImageObj::CaptureImageObjectChanaged()
{
	m_bImageSaved = FALSE;
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
			CDrawDataInfo* pDataInfo = new CDrawDataInfo(arDrawObject, arSelectObject, strName, m_strCacheFileName);
			m_arDrawData.push_back(pDataInfo);
			m_uDrawDataIndex = m_arDrawData.size();
			CaptureImageObjectChanaged();
			return TRUE;
		}
	}
	else
	{
		CDrawDataInfo* pDataInfo = new CDrawDataInfo(arDrawObject, arSelectObject, strName, m_strCacheFileName);
		m_arDrawData.push_back(pDataInfo);
		m_uDrawDataIndex = m_arDrawData.size();
		CaptureImageObjectChanaged();
		return TRUE;
	}

	return FALSE;
}

BOOL CCapImageObj::GetPrevDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject)
{
	BOOL bRet = FALSE;
	if (!IsFirstDrawObjectInfo())
	{
		m_uDrawDataIndex --;
		CDrawDataInfo* pDataInfo = m_arDrawData[m_uDrawDataIndex - 1];
		if (pDataInfo)
		{
			CString strFileName = m_strCacheFileName;
			bRet = pDataInfo->CopyDataInfo(arDrawObject, arSelectObject, strFileName);
			if (bRet && m_strCacheFileName.CompareNoCase(strFileName) != 0)
			{
				BOOL bActive = m_bActive;
				SetCapImageObjActive(FALSE);
				m_strCacheFileName = strFileName;
				SetCapImageObjActive(bActive);
				CNCaptureApplication::Instance()->GetDocument()->SetCaptureImage(this);
			}
		}
	}
	return bRet;
}

BOOL CCapImageObj::GetNextDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject)
{
	BOOL bRet = FALSE;
	if (!IsFinalDrawObjectInfo())
	{
		m_uDrawDataIndex ++;
		CDrawDataInfo* pDataInfo = m_arDrawData[m_uDrawDataIndex - 1];
		if (pDataInfo)
		{
			CString strFileName = m_strCacheFileName;
			bRet = pDataInfo->CopyDataInfo(arDrawObject, arSelectObject, strFileName);
			if (bRet && m_strCacheFileName.CompareNoCase(strFileName) != 0)
			{
				BOOL bActive = m_bActive;
				SetCapImageObjActive(FALSE);
				m_strCacheFileName = strFileName;
				SetCapImageObjActive(bActive);
				CNCaptureApplication::Instance()->GetDocument()->SetCaptureImage(this);
			}
		}
	}
	return bRet;
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
			return pDataInfo->CopyDataInfo(arDrawObject, arSelectObject, m_strCacheFileName);
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

void CCapImageObj::_DrawCurrentEditObject(HDC hDC)
{
	//CNCaptureApplication::Instance()->GetView()->DrawAllObjects(hDC);
	DrawObjectList listDrawObject, listSelectObject;
	if (GetCurrentObjectInfo(listDrawObject, listSelectObject))
	{	
		for (DrawObjectList::iterator iter = listDrawObject.begin();
			iter != listDrawObject.end();
			++iter)
		{
			CDrawObject* pObj = *iter;
			if (pObj)
			{
				pObj->Draw(hDC, FALSE);
			}
		}
	}
}

void CCapImageObj::SycDrawObjects()
{
	CImageDC dcImage(*this);
	_DrawCurrentEditObject((HDC)dcImage);
}