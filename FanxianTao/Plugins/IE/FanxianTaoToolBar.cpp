#include "StdAfx.h"
#include "FanxianTaoToolBar.h"

#include "dllmain.h"

CFanxianTaoToolBar::CFanxianTaoToolBar(void)
{
}

CFanxianTaoToolBar::~CFanxianTaoToolBar(void)
{
}


HWND CFanxianTaoToolBar::GetToolbarWnd()
{
	return m_hWnd;
}

BOOL CFanxianTaoToolBar::CreateToolbar(HWND hParentWnd, CFanxianBand* pSrchBand)
{
	BOOL bRet = FALSE;

	m_pSrchBand = pSrchBand;

	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		CCS_TOP | CCS_NODIVIDER | CCS_NORESIZE | CCS_NOPARENTALIGN | TBSTYLE_FLAT | TBSTYLE_LIST ;
	HWND hWnd = NULL ;
	HINSTANCE hInst = _AtlModule.GetModuleInstance() ;
	hWnd = ::CreateWindowEx(0, TOOLBARCLASSNAME, NULL, dwStyle, 0, 0, 100, 25, hParentWnd, (HMENU)7777, hInst, NULL) ;

	API_VERIFY(hWnd != NULL);
	if ( hWnd == NULL )
	{
		return FALSE;
	}
	//m_eActionType = kNotUseAP;
	//API_VERIFY(SubclassWindow(hWnd));
	API_VERIFY(SetWindowText(TEXT("FanxianToolbar")));

	//PostMessage( UM_POSTINIT );
	return bRet;
}

STDMETHODIMP CFanxianTaoToolBar::TranslateAcceleratorIO(LPMSG pMsg)
{
	return S_FALSE;
}

LRESULT	CFanxianTaoToolBar::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	return 0;
}

LRESULT CFanxianTaoToolBar::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	int cx = LOWORD(lParam);
	int cy = HIWORD(lParam);

	if ( cx == 0 && cy == 0 )
	{
		return 0;
	}
	//ResizeCombo( cx, m_nSeparatorPos );
	return 0;
}

void CFanxianTaoToolBar::OnPaint(CDCHandle /*dc*/)
{
	BOOL bRet = FALSE;
	CPaintDC dc(m_hWnd);

	CRect rcClient;
	API_VERIFY(GetClientRect(&rcClient));

	dc.FillSolidRect(rcClient, RGB(255, 0, 0));
}