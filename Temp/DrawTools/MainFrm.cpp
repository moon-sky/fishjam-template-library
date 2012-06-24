// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "nCaptureView.h"
#include "MainFrm.h"
#include <atldlgs.h>
#include <ftlgdi.h>
#include "NPVPhotoCalcRect.h"
#include "DrawTools/TextObject.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CNCaptureApplication::Instance()->m_pMainFrame = this;

	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_view.Initialize();
	
#ifdef FTL_DEBUG
	HRESULT hr = E_FAIL;
	CWindowDC dcScreen(NULL);
	m_bitmap.CreateCompatibleBitmap(dcScreen, 100, 100);
	CCapImageObj* pImageObj = new CCapImageObj(TEXT("G:\\TestImage.PNG"), NULL);
	COM_VERIFY(pImageObj->Load(TEXT("G:\\TestImage.PNG")));
	if (SUCCEEDED(hr))
	{
		CNCaptureApplication::Instance()->GetDocument()->SetCurCaptureImage(pImageObj);
	}
#endif

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		HRESULT hr = E_FAIL;
		CCapImageObj* pImageObj = new CCapImageObj(dlg.m_szFileName, m_bitmap);
		COM_VERIFY(pImageObj->Load(dlg.m_szFileName));
		if (SUCCEEDED(hr))
		{
			CNCaptureApplication::Instance()->GetDocument()->SetCurCaptureImage(pImageObj);
		}
	}
	return 0;
}


LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

BOOL CALLBACK EnumFamCallBack(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, DWORD FontType, LPVOID aFontCount) 
{ 
	int far * aiFontCount = (int far *) aFontCount; 

	// Record the number of raster, TrueType, and vector 
	// fonts in the font-count array. 

	FTLTRACE(TEXT("Font -- %s\n"), lplf->lfFaceName);

	if (FontType & RASTER_FONTTYPE) 
		aiFontCount[0]++; 
	else if (FontType & TRUETYPE_FONTTYPE) 
		aiFontCount[2]++; 
	else 
		aiFontCount[1]++; 

	if (aiFontCount[0] || aiFontCount[1] || aiFontCount[2]) 
		return TRUE; 
	else 
		return FALSE; 

	UNREFERENCED_PARAMETER( lplf ); 
	UNREFERENCED_PARAMETER( lpntm ); 
} 
LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//std::list<LOGFONT> lstFont;

	//HDC hdc = GetDC();
	//UINT uAlignPrev; 
	//int aFontCount[] = { 0, 0, 0 }; 
	//char szCount[8];
	//HRESULT hr;
	//size_t * pcch; 

	//EnumFontFamilies(hdc, (LPCTSTR) NULL, 
	//	(FONTENUMPROC) EnumFamCallBack, (LPARAM) aFontCount); 

	//uAlignPrev = SetTextAlign(hdc, TA_UPDATECP); 

	//FTLTRACE(TEXT("Number of raster fonts =%d\n"), aFontCount[0]);
	//FTLTRACE(TEXT("Number of vector fonts =%d\n"), aFontCount[1]);
	//FTLTRACE(TEXT("Number of TrueType fonts =%d\n"), aFontCount[2]);

	//ReleaseDC(hdc);

	CAboutDlg dlg;
	dlg.DoModal();
	//HRESULT hr = E_FAIL;
	//TCHAR buf[5];
	//COM_VERIFY(StringCchCopy(buf, _countof(buf), TEXT("fishjam")));
	return 0;
}

LRESULT CMainFrame::OnToolSelect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//m_view.SetCurrentToolType(ttSelection);
	CPoint ptMouse;
	GetCursorPos(&ptMouse);
	m_view.SelectToolTypeByMenu(ptMouse);

	return 0;
}

LRESULT CMainFrame::OnToolRect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.SetCurrentToolType(ttRect);
	return 0;
}

LRESULT CMainFrame::OnToolText(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_view.SetCurrentToolType(ttText);
	return 0;
}

LRESULT CMainFrame::OnEditFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HRESULT hr = E_FAIL;
	if (m_view.GetSelection().size() > 0)
	{
		CTextObject* pTextObject = dynamic_cast<CTextObject*>(m_view.GetSelection().front());
		if (pTextObject)
		{
			//LOGFONT		defaultFont = {0};
			//StringCchCopy(defaultFont.lfFaceName, _countof(defaultFont.lfFaceName), TEXT("Times New Roman"));
			//defaultFont.lfHeight = 18;

			//HFONT hDefaultFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
			CFontDialog dlg;
			dlg.DoModal();
			LONG nFontHeight = -MulDiv(dlg.m_cf.lpLogFont->lfHeight, 72, 96);
			pTextObject->GetRichEditPanel()->SetTextFontName(0, 0, dlg.m_cf.lpLogFont->lfFaceName);
			pTextObject->GetRichEditPanel()->SetTextFontSize(0, 0, nFontHeight);
			pTextObject->GetRichEditPanel()->SetTextForeColor(0, 0, dlg.m_cf.rgbColors);
			//COM_VERIFY(pTextObject->GetRichEditPanel()->SetTextFont(0, 0, dlg.m_cf.lpLogFont , 0xFFFFFFFF));

			//COM_VERIFY(pTextObject->GetRichEditPanel()->SetTextForeColor(0, 0, dlg.m_cf.rgbColors));
		}
	}
	return 0;
}

LRESULT CMainFrame::OnStreamIn(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_spStream)
	{
		m_spStream.Release();
	}

	HRESULT hr = E_FAIL;
	if (m_view.GetSelection().size() > 0)
	{
#ifndef DRAW_TOOL_TEST
		CTextObject* pTextObject = dynamic_cast<CTextObject*>(m_view.GetSelection().front());
		if (pTextObject)
		{
			COM_VERIFY(pTextObject->GetRichEditPanel()->GetTextStream(0, 0, &m_spStream));
			if (m_spStream)
			{
				ULARGE_INTEGER nLength;
				nLength.QuadPart = 0L;

				LARGE_INTEGER nStart;
				nStart.LowPart = 0;
				nStart.HighPart = 0;
				COM_VERIFY(m_spStream->Seek(nStart, STREAM_SEEK_END, &nLength));
				COM_VERIFY(m_spStream->Seek(nStart, STREAM_SEEK_SET, NULL));

				std::vector<CHAR> vStreamInfo(nLength.QuadPart);
				COM_VERIFY(m_spStream->Read(&vStreamInfo[0], nLength.QuadPart, NULL));
				CStringA strInfo(&vStreamInfo[0]);

				ATLTRACE(TEXT("Read: %s\n"), CA2T(strInfo));
			}
		}
#endif
	}

	return 0;
}

LRESULT CMainFrame::OnStreamOut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_spStream)
	{
		HRESULT hr = E_FAIL;
		if (m_view.GetSelection().size() > 0)
		{
#ifndef DRAW_TOOL_TEST
			CTextObject* pTextObject = dynamic_cast<CTextObject*>(m_view.GetSelection().front());
			if (pTextObject)
			{
				LARGE_INTEGER nStart;
				nStart.QuadPart = 0;
				COM_VERIFY(m_spStream->Seek(nStart,STREAM_SEEK_SET, NULL ));
				pTextObject->GetRichEditPanel()->SetTextStream(0, 0, m_spStream);
			}
#endif 
		}
	}

	return 0;
}