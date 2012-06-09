#pragma once

#include "DrawTools/DrawTypeDefine.h"

#include "DrawDataInfo.h"

class CCapImageObj : public ATL::CImage   //now just inherited from CImage
{
public:
	CCapImageObj(LPCTSTR pszObjectName);
	~CCapImageObj();
	CPoint GetPosition();
	void SetPosition( INT32 x, INT32 y );
	float GetZoom();
	void SetZoom( float dZoom );
	INT GetZoomMode();
	void SetZoomMode( INT nZoomMode );
	HANDLE	CopyToHandle(const CRect& rcSrc);

	BOOL SaveImageFile(LPCTSTR pszFilePath, BOOL bChanageState = TRUE);
	BOOL IsNeedSaveImage() { return m_bImageSaved; }
	BOOL SetCapImageObjActive(BOOL bActive = TRUE);

	LPCTSTR GetCaptureImageObjectName();
	void    SetCaptureImageObjectName(LPCTSTR strObjectName);

	LPCTSTR GetFileName();
	void    SetFileName(LPCTSTR strFileName);

	BOOL    CropImage(CRect& rcSrc);
	void    SetEditTmpFile(LPCTSTR strEditTmpFile);

	//void    SetDrawObjectInfo(const DrawObjectList& arDrawObject, const DrawObjectList& arSelectObject);
	//void    GetDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject);

	BOOL    PushDrawObjectInfo(const DrawObjectList& arDrawObject, const DrawObjectList& arSelectObject, LPCTSTR strName);

	BOOL    GetPrevDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject);
	BOOL    GetNextDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject);
	BOOL    GetCurrentObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject);

	BOOL    IsFirstDrawObjectInfo();
	BOOL    IsFinalDrawObjectInfo();
private:
	BOOL _SaveImageToCache();

	float			m_flZoom;
	INT				m_nZoomMode;
	CPoint			m_ptPosition;

	BOOL            m_bImageSaved;
	BOOL            m_bActive;
	CString         m_strCacheFileName;
	CString         m_strObjectName;
	CString         m_strEditTmpFile;
	CString         m_strFileName;

	//DrawObjectList  m_DrawObjectList;
	//DrawObjectList	m_SelectObjectList;
	size_t          m_uDrawDataIndex;
	CDrawDataArray  m_arDrawData;
};
