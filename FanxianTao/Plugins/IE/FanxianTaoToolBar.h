#pragma once

#include "FanxiantaoDefine.h"

class CFanxianTaoToolBar: public CWindowImpl<CFanxianTaoToolBar> , public IFToolbar
{
    //DECLARE_WND_SUPERCLASS(TEXT("FANXIANTOOLBAR"), TOOLBARCLASSNAME)
public:
	CFanxianTaoToolBar(void);
	virtual ~CFanxianTaoToolBar(void);

	//IFToolbar
	virtual HWND GetToolbarWnd();

	BOOL CreateToolbar(HWND hParentWnd, CFanxianBand* pSrchBand);
	STDMETHOD(TranslateAcceleratorIO)(LPMSG pMsg);

	BEGIN_MSG_MAP_EX(CFanxianTaoToolBar)
		//MSG_WM_DESTROY(OnDestroy)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MSG_WM_PAINT(OnPaint)
	END_MSG_MAP()

protected:
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	void OnPaint(CDCHandle dc);

	CFanxianBand* m_pSrchBand;
};
