#include "StdAfx.h"
#include "CalcRect.h"
#include <math.h>

BOOL CNPVPhotoCalcRect::m_bZoomFirstView = FALSE;
DOUBLE CNPVPhotoCalcRect::m_dOldFactor = 0.0;

CNPVPhotoCalcRect::CNPVPhotoCalcRect( void )
{
	m_rcView.SetRectEmpty();
	m_ptOffset.SetPoint( 0, 0 );
}
CNPVPhotoCalcRect::~CNPVPhotoCalcRect( void )
{
}
void CNPVPhotoCalcRect::SetCurrentRect( LPRECT lpRect )
{
	m_rcView = lpRect;
}
CNPVPhotoCalcRect* CNPVPhotoCalcRect::GetPhotoCalcRectObject( INT nZoomMode, LPRECT lpRect )
{
	CNPVPhotoCalcRect* pCalcRectObject = NULL;
	switch ( nZoomMode )
	{
		case E_ZOOM_AUTO_FIT_IF_BIGSIZE:
			pCalcRectObject = new CAutoFitIfBigSizePhotoRect;
			break;
		case E_ZOOM_AUTO_FIT:
			pCalcRectObject = new CAutoFitPhotoRect;
			break;
		case E_ZOOM_WIDTH_FIT:
			pCalcRectObject = new CWidthFitPhotoRect;
			break;
		case E_ZOOM_HEIGHT_FIT:
			pCalcRectObject = new CHeightFitPhotoRect;
			break;
		case E_ZOOM_ORIGINAL:
			pCalcRectObject = new COriginalPhotoRect;
			break;
			//case CNPVPageView::E_ZOOM_LOCK:
		case E_ZOOM_NORMAL:
			pCalcRectObject = new CZoomPhotoRect;
			break;
	}
	if ( pCalcRectObject )
	{
		pCalcRectObject->SetCurrentRect( lpRect );
		pCalcRectObject->SetCurrentZoomMode( nZoomMode );
	}
	return pCalcRectObject;
}
CRect CAutoFitIfBigSizePhotoRect::GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE /*dZoomFactor*/, BOOL /*bFirst*/, BOOL /*bFirstView*/ ) // NS
{
	m_bZoomFirstView = FALSE;

	CRect rcPhoto = rcClient;
	INT nWidthAllowed, nHeightAllowed, nWidthFactor, nHeightFactor;
	INT nWidth, nHeight, nDiff;

	nWidthAllowed = rcClient.Width();
	nHeightAllowed = rcClient.Height();
	nWidthFactor = lpSize->cx;
	nHeightFactor = lpSize->cy;

	if ( ( nWidthAllowed >= nWidthFactor ) && ( nHeightAllowed >= nHeightFactor ) )
	{
		rcPhoto.left = ( rcClient.Width() - lpSize->cx ) / 2;
		rcPhoto.top = ( rcClient.Height() - lpSize->cy ) / 2;
		rcPhoto.right = rcPhoto.left + lpSize->cx;
		rcPhoto.bottom = rcPhoto.top + lpSize->cy;
	}
	else
	{
		if ( MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor ) < nHeightAllowed )
		{
			//Width, auto fit
			nHeight = MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor );
			nDiff = ( nHeightAllowed - nHeight ) / 2;
			rcPhoto.top = rcPhoto.top + nDiff;
			rcPhoto.bottom = rcPhoto.bottom - nDiff;
		}
		else
		{
			//{kr}세로에 맞춤{/kr}
			nWidth = MulDiv( nHeightAllowed, nWidthFactor, nHeightFactor );
			nDiff = ( nWidthAllowed - nWidth ) / 2;
			rcPhoto.left = rcPhoto.left + nDiff;
			rcPhoto.right = rcPhoto.right - nDiff;
		}
	}
	m_rcView = rcPhoto;
	m_dOldFactor = ( DOUBLE )m_rcView.Width() / ( DOUBLE )lpSize->cx;
	return rcPhoto;
}
CRect CAutoFitPhotoRect::GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE /*dZoomFactor*/, BOOL /*bFirst*/, BOOL /*bFirstView*/ ) // NS
{
	m_bZoomFirstView = FALSE;

	CRect rcPhoto = rcClient;
	INT nWidthAllowed, nHeightAllowed, nWidthFactor, nHeightFactor;
	INT nWidth, nHeight, nDiff;

	nWidthAllowed = rcClient.Width();
	nHeightAllowed = rcClient.Height();
	nWidthFactor = lpSize->cx;
	nHeightFactor = lpSize->cy;

	if ( MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor ) < nHeightAllowed )
	{
		//{kr}세로에 맞춤{/kr}
		nHeight = MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor );
		nDiff = ( nHeightAllowed - nHeight ) / 2;
		rcPhoto.top = rcPhoto.top + nDiff;
		rcPhoto.bottom = rcPhoto.bottom - nDiff;
	}
	else
	{
		//{kr}가로에 맞춤{/kr}
		nWidth = MulDiv( nHeightAllowed, nWidthFactor, nHeightFactor );
		nDiff = ( nWidthAllowed - nWidth ) / 2;
		rcPhoto.left = rcPhoto.left + nDiff;
		rcPhoto.right = rcPhoto.right - nDiff;
	}
	m_rcView = rcPhoto;
	m_dOldFactor = ( DOUBLE )m_rcView.Width() / ( DOUBLE )lpSize->cx;
	return rcPhoto;
}
CRect CWidthFitPhotoRect::GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE /*dZoomFactor*/, BOOL /*bFirst*/, BOOL bFirstView ) // NS
{
	CRect rcPhoto = rcClient;

	INT nWidthAllowed, nHeightAllowed, nWidthFactor, nHeightFactor;
	INT nHeight, nDiff;

	nWidthAllowed = rcClient.Width();
	nHeightAllowed = rcClient.Height();
	nWidthFactor = lpSize->cx;
	nHeightFactor = lpSize->cy;

	//{kr}가로에 맞춤{/kr}
	nHeight = MulDiv( nWidthAllowed, nHeightFactor, nWidthFactor );
	nDiff = ( nHeightAllowed - nHeight ) / 2;
	rcPhoto.top = rcPhoto.top + nDiff;
	rcPhoto.bottom = rcPhoto.bottom - nDiff;
	if ( bFirstView )
	{
		if ( rcPhoto.top < 0 )
		{
			m_bZoomFirstView = TRUE;
			rcPhoto.top = 0;
			rcPhoto.bottom = rcPhoto.bottom - nDiff;
		}
	}
	m_rcView = rcPhoto;
	m_dOldFactor = ( DOUBLE )m_rcView.Width() / ( DOUBLE )lpSize->cx;
	return rcPhoto;
}
CRect CHeightFitPhotoRect::GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE /*dZoomFactor*/, BOOL /*bFirst*/, BOOL bFirstView ) // NS
{
	CRect rcPhoto = rcClient;

	INT nWidthAllowed, nHeightAllowed, nWidthFactor, nHeightFactor;
	INT nWidth, nDiff;

	nWidthAllowed = rcClient.Width();
	nHeightAllowed = rcClient.Height();
	nWidthFactor = lpSize->cx;
	nHeightFactor = lpSize->cy;

	//{kr}세로에 맞춤{/kr}
	nWidth = MulDiv( nHeightAllowed, nWidthFactor, nHeightFactor );
	nDiff = ( nWidthAllowed - nWidth ) / 2;
	rcPhoto.left = rcPhoto.left + nDiff;
	rcPhoto.right = rcPhoto.right - nDiff;
	if ( bFirstView )
	{
		if ( rcPhoto.left < 0 )
		{
			m_bZoomFirstView = TRUE;
			rcPhoto.left = 0;
			rcPhoto.right = rcPhoto.right - nDiff;
		}
	}
	m_rcView = rcPhoto;
	m_dOldFactor = ( DOUBLE )m_rcView.Width() / ( DOUBLE )lpSize->cx;
	return rcPhoto;
}
CRect COriginalPhotoRect::GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE /*dZoomFactor*/, BOOL bFirst, BOOL bFirstView ) // NS
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
CRect CZoomPhotoRect::GetPhotoRect( CRect rcClient, LPSIZE lpSize, DOUBLE dZoomFactor, BOOL bFirst, BOOL bFirstView )
{
#pragma warning(disable: 4244)
	// rcClient : 윈도우의 크기
	// lpSize : 이미지의 실제 크기
	// dZoomFactor : 이미지의 배율
	// bFirst : 처음 들어오는지 여부
	// m_rcView : 이미지의 크기(확대 축소가 적용된 크기)

	CRect rcPhoto;
	INT nTmp;
	if ( bFirstView )
	{
		rcPhoto.left = ( rcClient.Width() - ( lpSize->cx * dZoomFactor ) ) / 2;
		if ( rcPhoto.left < 0 )
		{
			m_bZoomFirstView = TRUE;
			rcPhoto.left = 0;
		}
		rcPhoto.top = ( rcClient.Height() - ( lpSize->cy * dZoomFactor ) ) / 2;
		if ( rcPhoto.top < 0 )
		{
			m_bZoomFirstView = TRUE;
			rcPhoto.top = 0;
		}
		rcPhoto.right = rcPhoto.left + ( lpSize->cx * dZoomFactor );
		rcPhoto.bottom = rcPhoto.top + ( lpSize->cy * dZoomFactor );
	}
	else if ( bFirst && ( m_bZoomFirstView == FALSE ) )
	{
		rcPhoto.left = ( rcClient.Width() - ( lpSize->cx * dZoomFactor ) ) / 2;
		rcPhoto.top = ( rcClient.Height() - ( lpSize->cy * dZoomFactor ) ) / 2;
		rcPhoto.right = rcPhoto.left + ( lpSize->cx * dZoomFactor );
		rcPhoto.bottom = rcPhoto.top + ( lpSize->cy * dZoomFactor );
		m_ptOffset.SetPoint( 0, 0 );
	}
	else
	{
		INT nWidth = lpSize->cx * dZoomFactor;
		INT nHeight = lpSize->cy * dZoomFactor;
		INT nOldWidth = lpSize->cx * m_dOldFactor;
		INT nOldHeight = lpSize->cy * m_dOldFactor;
		DOUBLE dWidthRatio = ( DOUBLE )( ( DOUBLE )nWidth / ( DOUBLE )nOldWidth );
		DOUBLE dHeightRatio = ( DOUBLE )( ( DOUBLE )nHeight / ( DOUBLE )nOldHeight );

		// 현재 보고 있는 곳의 중심 Center X, Y 를 구한다.
		INT iCenterX = -m_rcView.left + rcClient.Width() / 2;
		INT iCenterY = -m_rcView.top + rcClient.Height() / 2;

		// 새로 볼 영역의 중심 영역을 다시 계산한다.
		rcPhoto.left = -( ( iCenterX * dWidthRatio ) - rcClient.Width() / 2 );
		rcPhoto.top = -( ( iCenterY * dHeightRatio ) - rcClient.Height() / 2 );
		rcPhoto.right = rcPhoto.left + lpSize->cx * dZoomFactor;
		rcPhoto.bottom = rcPhoto.top + lpSize->cy * dZoomFactor;

		if ( rcPhoto.Width() > rcClient.Width() )
		{
			nTmp = rcPhoto.Width();
			if ( rcPhoto.left > rcClient.left )
			{
				rcPhoto.left = rcClient.left;
				rcPhoto.right = rcClient.left + nTmp;
			}
			else if ( rcPhoto.right < rcClient.right )
			{
				rcPhoto.right = rcClient.right;
				rcPhoto.left = rcClient.right - nTmp;
			}
		}
		else
		{
			rcPhoto.left = ( rcClient.Width() - ( lpSize->cx * dZoomFactor ) ) / 2;
			rcPhoto.right = rcPhoto.left + ( lpSize->cx * dZoomFactor );
		}
		if ( rcPhoto.Height() > rcClient.Height() )
		{
			nTmp = rcPhoto.Height();
			if ( rcPhoto.top > rcClient.top )
			{
				rcPhoto.top = rcClient.top;
				rcPhoto.bottom = rcClient.top + nTmp;
			}
			else if ( rcPhoto.bottom < rcClient.bottom )
			{
				rcPhoto.bottom = rcClient.bottom;
				rcPhoto.top = rcClient.bottom - nTmp;
			}
		}
		else
		{
			rcPhoto.top = ( rcClient.Height() - ( lpSize->cy * dZoomFactor ) ) / 2;
			rcPhoto.bottom = rcPhoto.top + ( lpSize->cy * dZoomFactor );
		}
	}
	m_rcView = rcPhoto;
	m_dOldFactor = dZoomFactor;
	return rcPhoto;
}