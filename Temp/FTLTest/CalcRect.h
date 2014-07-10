#pragma once


class CNPVPhotoCalcRect
{
public:
	enum ZoomMode
	{
		E_ZOOM_AUTO_FIT_IF_BIGSIZE,
		E_ZOOM_AUTO_FIT,
		E_ZOOM_WIDTH_FIT,
		E_ZOOM_HEIGHT_FIT,
		E_ZOOM_ORIGINAL,
		E_ZOOM_NORMAL
	};

public:
	CNPVPhotoCalcRect( void );
	virtual ~CNPVPhotoCalcRect( void );

	static CNPVPhotoCalcRect*	GetPhotoCalcRectObject( INT nZoomMode, LPRECT lpRect );
	virtual CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE ) = 0; // NS
	void SetCurrentRect( LPRECT lpRect );
	DOUBLE GetCurrentZoomFactor()
	{
		return m_dOldFactor;
	}
	INT GetCurrentZoomMode()
	{
		return m_nZoomMode;
	}
	void SetCurrentZoomMode( INT nZoomMode )
	{
		m_nZoomMode = nZoomMode;
	}

	static BOOL		m_bZoomFirstView;
	static DOUBLE	m_dOldFactor;

protected:
	CRect				m_rcView;
	CPoint				m_ptOffset;
	INT					m_nZoomMode;
};

class CAutoFitIfBigSizePhotoRect : public CNPVPhotoCalcRect
{
public:
	CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE );
};

class CAutoFitPhotoRect : public CNPVPhotoCalcRect
{
public:
	CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE );
};

class CWidthFitPhotoRect : public CNPVPhotoCalcRect
{
public:
	CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE );
};

class CHeightFitPhotoRect : public CNPVPhotoCalcRect
{
public:
	CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE );
};

class COriginalPhotoRect : public CNPVPhotoCalcRect
{
public:
	CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE );
};

class CZoomPhotoRect : public CNPVPhotoCalcRect
{
	CRect GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst = TRUE, BOOL bFirstView = FALSE );
};