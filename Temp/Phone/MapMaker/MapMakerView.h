// MapMakerView.h : interface of the CMapMakerView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMapMakerView : public CScrollWindowImpl<CMapMakerView>
{
public:
	DECLARE_WND_CLASS(TEXT("CMapMakerView"))

	BOOL PreTranslateMessage(MSG* pMsg);

	void DoPaint(CDCHandle dc);

	BEGIN_MSG_MAP(CMapMakerView)
        DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, uMsg, wParam, lParam)
		CHAIN_MSG_MAP(CScrollWindowImpl<CMapMakerView>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};
