#pragma once


#include "DrawTools/DrawTypeDefine.h"

#include "DrawDataInfo.h"

class CCapImageObj : public ATL::CImage   //now just inherited from CImage
{
public:
	CCapImageObj(LPCTSTR pszObjectName, HBITMAP hbmp);
	~CCapImageObj();
	CPoint GetPosition();
	void SetPosition( INT32 x, INT32 y );
	float GetZoom();
	void SetZoom( float dZoom );
	INT GetZoomMode();
	void SetZoomMode( INT nZoomMode );
	HANDLE	CopyToHandle(const CRect& rcSrc);

	BOOL SaveImageFile(LPCTSTR pszFilePath, BOOL bChanageState = TRUE);
	BOOL SaveImageFile();
	BOOL IsNeedSaveImage() { return m_bImageChanaged; }
	BOOL SetCapImageObjActive(BOOL bActive = TRUE);

	LPCTSTR GetCaptureImageObjectName();
	void    SetCaptureImageObjectName(LPCTSTR strObjectName);

	CString GetFileName();
	void    SetFileName(LPCTSTR strFileName);

	BOOL    CropImage(CRect& rcSrc);
	void    SetEditTmpFile(LPCTSTR strEditTmpFile);

	void    CaptureImageObjectChanaged();

	BOOL    PushDrawObjectInfo(const DrawObjectList& arDrawObject, const DrawObjectList& arSelectObject, LPCTSTR strName);

	BOOL    GetPrevDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject);
	BOOL    GetNextDrawObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject);
	BOOL    GetCurrentObjectInfo(DrawObjectList& arDrawObject, DrawObjectList& arSelectObject, BOOL bCopy);

	BOOL    IsFirstDrawObjectInfo();
	BOOL    IsFinalDrawObjectInfo();

	void    SycDrawObjects();
private:
	BOOL _SaveImageToCache(BOOL bNewFile = FALSE);
	void _DrawCurrentEditObject(HDC hDC);

	float			m_flZoom;
	INT				m_nZoomMode;
	CPoint			m_ptPosition;

	BOOL            m_bImageChanaged;
	BOOL            m_bActive;
	CString         m_strCacheFileName;
	CString         m_strObjectName;
	CString         m_strEditTmpFile;;
	CString         m_strFileName;

	//DrawObjectList  m_DrawObjectList;
	//DrawObjectList	m_SelectObjectList;
	size_t          m_uDrawDataIndex;
	CDrawDataArray  m_arDrawData;

	CString         m_strLastSaveName;
};
