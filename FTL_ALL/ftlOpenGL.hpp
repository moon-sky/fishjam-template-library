#ifndef FTL_OPENGL_HPP
#define FTL_OPENGL_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlOpenGL.h"
#endif

namespace FTL
{
    CFOpenGLErrorInfo::CFOpenGLErrorInfo(GLenum glErr) : CFConvertInfoT<CFOpenGLErrorInfo,GLenum>(glErr)
    {

    }
    LPCTSTR CFOpenGLErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_INVALID_ENUM);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_INVALID_VALUE);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_INVALID_OPERATION);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_STACK_OVERFLOW);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_STACK_UNDERFLOW);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_OUT_OF_MEMORY);
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
                    TEXT("Unknown Error:%d(0x%x)"), m_Info, m_Info);
                break;
            }
        }
        return m_bufInfo;
    }

    BOOL CFOpenGLApp::Initialize()
    {
        GLenum glErr = GL_NO_ERROR;
        OPENGL_VERIFY(glClearColor(0.0f, 1.0f, 0.0f, 0.5f));    //设置清除屏幕时所用的颜色(黑色背景)
        
        OPENGL_VERIFY(glClearDepth(1.0f));  //深度缓存--对物体进入屏幕内部有多深进行跟踪，其排序决定物体的可见性
        OPENGL_VERIFY(glEnable(GL_DEPTH_TEST)); //启用深度测试
        OPENGL_VERIFY(glDepthFunc(GL_LEQUAL));  //所作深度测试的类型

        OPENGL_VERIFY(glShadeModel(GL_SMOOTH)); //启用smooth shading(阴影平滑) -- 通过多边形精细的混合色彩，并对外部光进行平滑
        
        //告诉系统对透视进行最好的修正 -- 效果好，但影响性能
        OPENGL_VERIFY(glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST));
        //ResizeDraw(true); // 改变OpenGL窗口大小，直接调用子类的函数

        return TRUE;
    }

    template <class T>
    CFOpenGLWindowImpl<T>::CFOpenGLWindowImpl()
    {
        m_hRC = NULL;
        //m_hDC = NULL;
        m_bFullScreen = FALSE;
        m_iPixelFormat = 0;
    }

    template <class T>
    CFOpenGLWindowImpl<T>::~CFOpenGLWindowImpl()
    {

    }
    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFOpenGLWindowImpl<T>::OnCreate\n"));

        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        CClientDC dc(pT->m_hWnd);

        //设置窗口使用的像素格式
        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR); 
        pfd.nVersion   = 1;                 //版本号
        pfd.dwFlags    = PFD_DOUBLEBUFFER   //必须支持双缓冲
            | PFD_SUPPORT_OPENGL            //格式必须支持OpenGL 
            | PFD_DRAW_TO_WINDOW;           //格式支持窗口
        pfd.iPixelType = PFD_TYPE_RGBA;     //申请 RGBA 格式
        pfd.cColorBits = 24;                //选定色彩深度
        //pfd.cRedBits ~ cAlphaShift -- 等都是0，表示忽略色彩位且无Alpha缓存
        pfd.cAccumBits = 0;                 //无累加缓存
        //pfd.cAccumRedBits ~ cAccumAlphaBits = 0; //忽略聚集位
        pfd.cDepthBits = 32;  //16 ?         //Z-缓存 (深度缓存)
        pfd.cStencilBits = 0;               //无蒙板缓存
        pfd.cAuxBuffers = 0;                //无辅助缓存
        pfd.iLayerType = PFD_MAIN_PLANE ;   //主绘图层
        pfd.dwLayerMask = 0;                //忽略层遮罩
        pfd.dwVisibleMask = 0;
        pfd.dwDamageMask = 0;

        //尝试找到对应的象素格式
        m_iPixelFormat = dc.ChoosePixelFormat(&pfd);
        API_ASSERT(m_iPixelFormat != 0);

        API_VERIFY(dc.SetPixelFormat (m_iPixelFormat, &pfd));   //设置象素格式
        API_VERIFY((m_hRC = dc.wglCreateContext()) != NULL);    //取得着色描述表
        API_VERIFY(dc.wglMakeCurrent(m_hRC));                   //激活着色描述表

        API_VERIFY(pT->OnInit());

        API_VERIFY(dc.wglMakeCurrent(NULL));

        bHandled = FALSE;

        return 0;
    }

    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFOpenGLWindowImpl<T>::OnPaint\n"));

        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        CPaintDC dc(pT->m_hWnd);
        API_VERIFY(dc.wglMakeCurrent(m_hRC));
        API_VERIFY(pT->OnRender());
        API_VERIFY(dc.SwapBuffers());
        API_VERIFY(dc.wglMakeCurrent(NULL));

        //BOOL bRet = FALSE;
        //PAINTSTRUCT ps = { 0 };
        //HDC hDC = BeginPaint(&ps);
        //if(hDC)
        //{
        //    CRect rcClient;
        //    API_VERIFY(GetClientRect(rcClient));
        //    COLORREF clrOld = ::SetBkColor(hDC, RGB(255, 0, 0));
        //    ATLASSERT(clrOld != CLR_INVALID);
        //    if(clrOld != CLR_INVALID)
        //    {
        //        ::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, NULL);
        //        ::SetBkColor(hDC, clrOld);
        //    }
        //    EndPaint(&ps);
        //}
        //bHandled = FALSE;
        return 0;
    }
    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFOpenGLWindowImpl<T>::OnSize\n"));

        //ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        T* pT = static_cast<T*>(this);
        int cx = LOWORD(lParam);
        int cy = HIWORD(lParam);

        if(0 == cx || 0 == cy || NULL == pT->m_hWnd || NULL == m_hRC)
        {
            return 0;
        }
        BOOL bRet = FALSE;
        CClientDC dc(pT->m_hWnd);
        API_VERIFY(dc.wglMakeCurrent(m_hRC));
        API_VERIFY(pT->OnResize(cx, cy));
        API_VERIFY(dc.wglMakeCurrent(NULL));

        bHandled = FALSE;
        return 0;
    }

    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 1;
    }

    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        BOOL bRet = FALSE;
        bHandled = FALSE;
        FTLTRACE(TEXT("CFOpenGLWndT<T>::OnDestroy\n"));

        if (m_hRC) 
        {
            API_VERIFY(wglDeleteContext(m_hRC));
            m_hRC = NULL;
        }

        //if (m_bFullScreen)
        //{
        //    //切换回桌面--强制Windows使用注册表中的值(分辨率、色深、刷新频率等)来恢复原始桌面
        //    ChangeDisplaySettings(NULL,0);		
        //    ShowCursor(TRUE);					//显示鼠标指针
        //}
        return 0;
    }

    template <class T>
    GLenum CFOpenGLWindowImpl<T>::ReSizeGLScene(GLsizei width, GLsizei height)
    {
        GLenum glErr = GL_NO_ERROR;

        if (0 == height)  // 防止被零除
        {
            height = 1;
        }
        OPENGL_VERIFY(glViewport(0, 0, width, height));             // 重置当前的视口

        // 为透视图设置屏幕 -- 越远的东西看起来越小
        OPENGL_VERIFY(glMatrixMode(GL_PROJECTION));						        // 选择投影矩阵
        OPENGL_VERIFY(glLoadIdentity());							                // 重置投影矩阵

        //此处透视按照基于窗口宽度和高度的45度视角来计算。0.1f，100.0f是我们在场景中所能绘制深度的起点和终点
        OPENGL_VERIFY(gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f));   // 设置视口的大小
        OPENGL_VERIFY(glMatrixMode(GL_MODELVIEW));						            // 选择模型观察矩阵
        OPENGL_VERIFY(glLoadIdentity());							                // 重置模型观察矩阵

        return glErr;
    }
    
    template <class T>
    BOOL CFOpenGLWindowImpl<T>::CreateGLWindow(char* title, INT width, INT height, INT bits, BOOL fullscreenflag)
    {
        BOOL bRet = FALSE;
        GLuint		PixelFormat; // 保存查找匹配的结果

        return bRet;
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::SetFullScreenMode(BOOL bFullScreen, BOOL *pOldFullScreenMode)
    {
        BOOL bRet = FALSE;
        if (pOldFullScreenMode)
        {
            *pOldFullScreenMode = m_bFullScreen;
        }
        if (m_bFullScreen != bFullScreen)
        {
            DEVMODE dmScreenSettings = {0};						// 设备模式
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Devmode 结构的大小
            RECT WindowRect = {0};
            GetClientRect(&WindowRect);
            if (bFullScreen)
            {
                dmScreenSettings.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN);				// 所选屏幕宽度
                dmScreenSettings.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);				// 所选屏幕高度
                dmScreenSettings.dmBitsPerPel = 32;					// 每象素所选的色彩深度
            }
            else
            {

            }
            dmScreenSettings.dmFields= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
            API_VERIFY(DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN));
            if (bRet)
            {
                DWORD dwStyle = 0;
                DWORD dwExStyle = 0;
                if (bFullScreen)
                {
                    dwExStyle = WS_EX_APPWINDOW; //WS_EX_APPWINDOW，将强制我们的窗体可见时处于最前面
                    dwStyle = WS_POPUP; //窗体没有边框，使全屏模式得以完美显示
                    //ShowCursor(FALSE);						// 隐藏鼠标指针
                }
                else
                {
                    dwExStyle = WS_EX_APPWINDOW|WS_EX_WINDOWEDGE; //扩展窗体风格
                    dwStyle = WS_OVERLAPPEDWINDOW; //带标题栏、可变大小的边框、菜单和最大化/最小化按钮的窗体
                }

                //根据创建的窗体类型调整窗口
                API_VERIFY(AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle));		// 调整窗口达到真正要求的大小
                SetWindowPos(WindowRect, SWP_SHOWWINDOW);
            }

        }
        if (bRet)
        {
            m_bFullScreen = bFullScreen;
        }

        return bOldFullScreen;
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnInit()
    {
        return TRUE;
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度缓存
        glLoadIdentity();							// 重置当前的模型观察矩阵
        return TRUE;								//  一切 OK
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnResize(int cx, int cy)
    {
        return TRUE;
    }
    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnFini()
    {
        return TRUE;
    }
}

#endif //FTL_OPENGL_HPP