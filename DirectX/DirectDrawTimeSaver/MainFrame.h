#pragma once

#define FULLSCREEN_RUNNING  //在全屏模式与窗口模式下切换

#include <ddraw.h>

class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame)
public:
    CMainFrame(void);
    ~CMainFrame(void);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
    UINT_PTR                m_TimerID;
    IDirectDraw7*           m_pDDraw;
    IDirectDrawSurface7*    m_pDDSPrimary;  //主页面
#ifdef FULLSCREEN_RUNNING
    IDirectDrawSurface7*    m_pDDSBack;     //后台缓冲页面
#endif
    IDirectDrawSurface7*    m_pDDSBackground; //屏外页面

public:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
