#pragma once


// CMyTooltipCtrl

class CMyTooltipCtrl : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CMyTooltipCtrl)

public:
	CMyTooltipCtrl();
	virtual ~CMyTooltipCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	BOOL MyAddTool(_In_ CWnd* pWnd, _In_ LPCTSTR lpszText, _In_opt_ LPCRECT lpRectTool = NULL,
		_In_ UINT_PTR nIDTool = 0);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


