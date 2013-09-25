#pragma once

#include <ftlOpenGL.h>

class CMyOpenGLWnd : public CWindowImpl<CMyOpenGLWnd, CWindow, CFrameWinTraits>
    , public FTL::CFOpenGLWindowImpl<CMyOpenGLWnd>
    , public CUpdateUI<CMyOpenGLWnd>
    //, public CMessageFilter
    , public CIdleHandler
{
public:
    CMyOpenGLWnd();
    ~CMyOpenGLWnd();
public:
    DECLARE_WND_CLASS_EX(TEXT("FTLOpenGLWindow"), CS_VREDRAW | CS_HREDRAW | CS_OWNDC, -1)
    //DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    BEGIN_MSG_MAP(CMyOpenGLWnd)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        CHAIN_MSG_MAP(CUpdateUI<CMyOpenGLWnd>)
        //CHAIN_MSG_MAP(CWindowImpl<CMyOpenGLWnd>)
        CHAIN_MSG_MAP(CFOpenGLWindowImpl<CMyOpenGLWnd>)
    END_MSG_MAP()

    BEGIN_UPDATE_UI_MAP(CMyOpenGLWnd)
    END_UPDATE_UI_MAP()
public:
    LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

    //virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BOOL OnInit(void);
    BOOL OnRender(void);
private:
    void _UpdateFPS();
    void _GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
private:
    GLfloat m_fAngle;		// Rotation angle of the cube
};