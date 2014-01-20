#pragma once
//#include "atlwin.h"
#include "atlctrlx.h"

class CLayeredButton : public CBitmapButtonImpl<CLayeredButton>
    //CWindowImpl<CLayeredButton, CButton, CControlWinTraits>
{
    typedef CBitmapButtonImpl<CLayeredButton> thisClass;
public:
    DECLARE_WND_SUPERCLASS( _T("LayeredButton"), GetWndClassName() )
	DISABLE_COPY_AND_ASSIGNMENT(CLayeredButton);
public:
    CLayeredButton(void){};
    ~CLayeredButton(void){};
    
    void DoPaint(CDCHandle dc);

    //BOOL SubclassWindow(HWND hWnd);

	//BEGIN_MSG_MAP(CLayeredButton)
 //       DUMP_WINDOWS_MSG(__FILE__LINE__, NULL, 0, uMsg, wParam, lParam)
	//	//MESSAGE_HANDLER(WM_CREATE, OnCreate)
	//	//MSG_WM_MOUSEMOVE(OnMouseMove)
	//	//MSG_WM_LBUTTONDOWN(OnLButtonDown)
	//	//MSG_WM_LBUTTONUP(OnLButtonUp)
	//	//MSG_WM_CAPTURECHANGED(OnCaptureChanged)
	//	//MSG_WM_MOUSELEAVE(OnMouseLeave);
 //       //REFLECTED_COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnClicked)
 //       MSG_OCM_DRAWITEM(DrawItem)
 //       //MSG_OCM_CTLCOLORBTN(OnCtlColor)
	//END_MSG_MAP()

	// static CWndClassInfo &GetWndClassInfo();

	//LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//void OnMouseMove(UINT nFlags, CPoint point);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	//void OnLButtonUp(UINT nFlags, CPoint point);
	//void OnMouseLeave(void);

	//void OnCaptureChanged(HWND hWnd);

	//void SetImages(Gdiplus::Image* pImage, int nGrow, int nNormal, int nPushed = -1, 
	//	int nFocusOrHover = -1, int nDisabled = -1);
	//int GetWidth(void);
	//int GetHeight(void);
	//
 //   void DrawItem(UINT ctrlID, LPDRAWITEMSTRUCT lpDIS);
    //LRESULT OnCtlColor(CDCHandle pDC, HWND button);
private:
	//void _DoDraw(void);

	//enum
	//{
	//	_nImageNormal = 0,
	//	_nImagePushed,
	//	_nImageFocusOrHover,
	//	_nImageDisabled,

	//	_nImageCount = 4,
	//};

	//Gdiplus::Image* m_pImage;

	//int m_nImage[_nImageCount];
	//int m_nImageCX;
	//int m_nImageCY;
	//int m_nGrow;

 //   
	//BOOL m_bHover;
	//BOOL m_bPressed;

	//UINT m_nID;
	////HWND m_hWndOwner;
};
