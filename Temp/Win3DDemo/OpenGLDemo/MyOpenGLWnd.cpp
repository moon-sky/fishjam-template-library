#include "StdAfx.h"
#include "MyOpenGLWnd.h"

CMyOpenGLWnd::CMyOpenGLWnd(void)
{
}

CMyOpenGLWnd::~CMyOpenGLWnd(void)
{
}

LRESULT CMyOpenGLWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    //pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    bHandled = FALSE;
    return 0;
}

//virtual BOOL CMyOpenGLWnd::PreTranslateMessage(MSG* pMsg)
//{
//    return CWindowImpl<CMyOpenGLWnd>::PreTranslateMessage(pMsg);
//    //return FALSE;
//}
BOOL CMyOpenGLWnd::OnIdle()
{
    m_fAngle+= 1; // Add some rotation to the cube
    _UpdateFPS();
    RedrawWindow();

    return TRUE;
}
BOOL CMyOpenGLWnd::OnInit(void) 
{
    BOOL bRet = TRUE;
    GLenum glErr = GL_NO_ERROR;

    m_fAngle = 0.0f;
    OPENGL_VERIFY(glShadeModel(GL_SMOOTH));

    OPENGL_VERIFY(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); //Background color

    // Activate lighting and a light source
    OPENGL_VERIFY(glEnable(GL_LIGHT0));
    OPENGL_VERIFY(glEnable(GL_LIGHTING));
    OPENGL_VERIFY(glEnable(GL_DEPTH_TEST));

    // Define material parameters
    static GLfloat glfMatAmbient[] = {0.000f, 0.450f, 1.000f, 1.0f};
    static GLfloat glfMatDiffuse[] = {0.000f, 0.000f, 0.580f, 1.0f};
    static GLfloat glfMatSpecular[]= {1.000f, 1.000f, 1.000f, 1.0f};
    static GLfloat glfMatEmission[]= {0.000f, 0.000f, 0.000f, 1.0f};
    static GLfloat fShininess = 128.000f;

    // Set material parameters
    OPENGL_VERIFY(glMaterialfv(GL_FRONT, GL_AMBIENT,  glfMatAmbient));
    OPENGL_VERIFY(glMaterialfv(GL_FRONT, GL_DIFFUSE,  glfMatDiffuse));
    OPENGL_VERIFY(glMaterialfv(GL_FRONT, GL_SPECULAR, glfMatSpecular));
    OPENGL_VERIFY(glMaterialfv(GL_FRONT, GL_EMISSION, glfMatEmission));
    OPENGL_VERIFY(glMaterialf(GL_FRONT, GL_SHININESS, fShininess));

    return bRet;
}

BOOL CMyOpenGLWnd::OnRender(void)
{
    BOOL bRet = TRUE;
    GLenum glErr = GL_NO_ERROR;
    OPENGL_VERIFY(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // Clear buffers
    OPENGL_VERIFY(glLoadIdentity()); // Load identity matrix

    // Add a light source
    GLfloat glfLight[] = {-4.0f, 4.0f, 4.0f, 0.0f};
    OPENGL_VERIFY(glLightfv(GL_LIGHT0, GL_POSITION, glfLight));

    // Position and rotate the camera
    OPENGL_VERIFY(glTranslatef(0.0f, 0.0f, -5.0f));	
    OPENGL_VERIFY(glRotatef(30.0f, 1.0f, 0.0f, 0.0f));
    OPENGL_VERIFY(glRotatef(m_fAngle, 0.0f, 1.0f, 0.0f));

    // Draw a cube
    static GLfloat r = .7f;
    _GLCube(-r, -r, -r, r, r, r);

    OPENGL_VERIFY(glFlush());

    return bRet;
}

void CMyOpenGLWnd::_UpdateFPS()
{
    const int FRAMEINTERVAL = 1000;            // Show FPS every 1000th milliseconds
    static DWORD nFrames = 0;                  // Number of frames since last update
    static DWORD nLastTick = GetTickCount();   // Time of last update
    DWORD nTick = GetTickCount();              // Current time
    if(nTick-nLastTick>=FRAMEINTERVAL)
    {	
        float fFPS = 1000.0f*(GLfloat)nFrames/(GLfloat)(nTick-nLastTick);
        nLastTick = nTick;
        nFrames = 0;
        TCHAR szFPS[256];
        StringCchPrintf(szFPS, _countof(szFPS), TEXT("%.1f FPS"), fFPS);
        SetWindowText(szFPS);
    }
    nFrames++;
}

void CMyOpenGLWnd::_GLCube(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);
    glEnd();
}