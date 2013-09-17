#ifndef FTL_OPENGL_H
#define FTL_OPENGL_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlOpenGL.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
    * OpenGL的开发SDK下载(VC6版本的)：http://download.csdn.net/detail/jinxiu/274113
    * OpenGL(NeHe)中文学习资料：http://www.yakergong.net/nehe/
    *
    * 窗体风格必须包括：WS_CLIPSIBLINGS 和 WS_CLIPCHILDREN
    *****************************************************************************************************/

    /*****************************************************************************************************
    * glMatrixMode(GL_PROJECTION 等)
    *   Modelview Matrix(模型观察矩阵) -- 存放了我们的物体讯息
    *   Projection Matrix(投影矩阵) -- 负责为我们的场景增加透视
    *****************************************************************************************************/

    /*****************************************************************************************************
    * OpenGL 是平台独立的2D/3D 图形API，是由各家显示厂商定义的共通函数库，所以它是绘图的工业标准，因为其精确度和
    *   性能都较高，主要应用在CAD/CAM 建模、动画制作、模拟仿真和科学显像领域。
    * 
    * 根据嵌入式系统的特点挑选了OpenGL 中的一些命令(Command)和状态(State)并增加了一些命令(Profile Extension)
    *   得到了OpenGL ES(Open Graphics Library for Embedded System)，嵌入式系统（包括移动手持设备、家电、车辆等）
    *   上用于绘制2D/3D 图形的一个免版税、跨平台的图形库。
    *   OpenGL ES 规范描述了OpenGL ES API 的两个剖面(Profile)，Common 剖面和 Common-Lite 剖面。Common-Lite 剖面
    *   中的API 是为不支持高性能浮点运算的绘图系统而设计的，所以两个剖面中的命令(Command，相当于函数)主要区别
    *   是CM Profile 支持浮点运算，CL Profile 只支持定点运算，
    *
    * OpenGL的库函数采用C语言风格，所有OpenGL函数采用了以下格式进行命名：
    *   <库前缀><根命令><可选的参数个数><可选的参数类型>
    *   库前缀有gl、glu、aux、glut、wgl、glx、agl 等等，分别表示该函数属于OpenGL那个开发库
    * OpenGL函数库相关的API有核心库(gl)、实用库(glu)、辅助库(aux)、实用工具库(glut)、
    *   针对不同窗口系统的窗口库(glx、agl、wgl)和扩展函数库等。其中 核心库(gl)和实用库(glu)可以在所有的平台上使用。
    * 
    *   A.核心库(gl) -- 用于常规的、核心的图形处理，主要分为：
    *     1.绘制基本几何图元的函数 -- glBegin、glEnd、glNormal*、glVertex* 等；
    *     2.矩阵操作、几何变换和投影变换的函数 
    *         矩阵操作：glPushMatrix、glPopMatrix、glLoadMatrix、glMultMatrix、
    *         几何变换：glTranslate*、glRotate*、glScale*、
    *         投影变换：glOrtho、glFrustum、
    *         视口变换：glViewport
    *     3.颜色、光照和材质的函数
    *         设置颜色模式：glColor*、glIndex*、
    *         设置光照效果：glLight*、glLightModel*
    *         设置材质效果：glMaterial、
    *     4.显示列表函数 -- 创建(glNewList)、结束(glEndList)、生成(glGenLists)、删除(glDeleteLists)和调用(glCallList)显示列表
    *     5.纹理映射函数
    *         一二维的纹理函数：glTexImage*
    *         设置纹理参数(glTexParameter*)、纹理环境（glTexEnv*）和纹理坐标(glTetCoord*);
    *     6.特殊效果函数
    *         融合函数glBlendFunc、反走样函数glHint、雾化效果glFog*
    *     7.光栅化、象素操作函数
    *         象素位置glRasterPos*、线型宽度glLineWidth、多边形绘制模式glPolygonMode、读取象素glReadPixel、复制象素glCopyPixel
    *     8.选择与反馈函数
    *         渲染模式glRenderMode、选择缓冲区glSelectBuffer、反馈缓冲区glFeedbackBuffer
    *     9.曲线与曲面的绘制函数
    *         生成曲线或曲面的函数 glMap*、glMapGrid*，
    *         求值器 glEvalCoord* glEvalMesh*；
    *     10.状态设置与查询函数
    *        glGet*()、glEnable()、glGetError()
    *  
    *   B.OpenGL实用库(The OpenGL Utility Library -- GLU),所有较复杂的绘图都必须从点、线、面开始，通过glu封装一些较为复杂的操作。
    *     1.辅助纹理贴图函数
    *         gluScaleImage() 、gluBuild1Dmipmaps()、gluBuild2Dmipmaps()
    *     2.坐标转换和投影变换函数
    *         定义投影方式函数 gluPerspective()、gluOrtho2D() 、gluLookAt()
    *         拾取投影视景体函数 gluPickMatrix()，
    *         投影矩阵计算 gluProject() 和 gluUnProject()
    *     3.多边形镶嵌工具
    *         gluNewTess()、gluDeleteTess()、gluTessCallback()、gluBeginPolygon()、gluTessVertex()、gluNextContour()、gluEndPolygon()；
    *     4.二次曲面绘制工具，主要有绘制球面、锥面、柱面、圆环面 
    *         gluNewQuadric()、gluSphere()、gluCylinder()、gluDisk()、gluPartialDisk()、gluDeleteQuadric()
    *     5.非均匀有理B样条绘制工具
    *         主要用来定义和绘制Nurbs曲线和曲面，包括 gluNewNurbsRenderer、gluNurbsCurve、gluBeginSurface、gluEndSurface、
    *         gluBeginCurve、gluNurbsProperty
    *     6.错误反馈工具
    *         获取出错信息的字符串 gluErrorString
    *   
    *   C.OpenGL辅助库(aux)，注意：Windows实现有很大问题，很大程度上已经被 glut 所取代
    *     1.窗口初始化和退出函数：auxInitDisplayMode()和auxInitPosition()
    *     2.窗口处理和时间输入函数：auxReshapeFunc()、auxKeyFunc()和auxMouseFunc()； 
    *     3.颜色索引装入函数：auxSetOneColor()； 
    *     4.三维物体绘制函数：包括了 网状体和实心体 两种形式。
    *         如：立方体auxWireCube()和 auxSolidCube()。
    *     5.背景过程管理函数：auxIdleFunc()；
    *     6.程序运行函数：auxMainLoop()。
    * 
    *   D.OpenGL工具库(glut) OpenGL Utility Toolkit -- 可以在X-Window, Windows NT, OS/2等系统下运行
    *   　　不依赖于窗口平台的OpenGL工具包，目的是隐藏不同窗口平台API的复杂度。 作为aux库功能更强的替代品，提供更为复杂的绘制功能，
    *     1.窗口操作函数：窗口初始化、窗口大小、窗口位置函数等[glutInit()、glutInitDisplayMode()、glutInitWindowSize()、glutInitWindowPosition()]；
    *     2.回调函数：响应刷新消息、键盘消息、鼠标消息、定时器函数[GlutDisplayFunc()、glutPostRedisplay()、glutReshapeFunc()、glutTimerFunc()、glutKeyboardFunc()、glutMouseFunc()]；
    *     3.创建复杂的三维物体：这些和aux库的函数功能相同；
    *     4.菜单函数：创建添加菜单的函数GlutCreateMenu()、glutSetMenu()、glutAddMenuEntry()、glutAddSubMenu()和glutAttachMenu()；
    *     5.程序运行函数：glutMainLoop()。
    *  
    *   E.Windows专用库(wgl) -- 
    *     用于连接OpenGL和Windows ，以弥补OpenGL在文本方面的不足
    *     1.绘图上下文相关函数：wglCreateContext、wglDeleteContext、wglGetCurrentContent、wglGetCurrentDC、wglDeleteContent
    *     2.文字和文本处理函数：wglUseFontBitmaps、wglUseFontOutlines
    *     3.覆盖层、地层和主平面层处理函数：wglCopyContext、wglCreateLayerPlane、wglDescribeLayerPlane、wglReakizeLayerPlatte
    *     4.其他函数：wglShareLists()、wglGetProcAddress()。
    *   
    *   F.Win32 API函数库 -- 函数名无专用前缀，是 Win32 扩展函数。主要用于处理像素存储格式和双帧缓存。
    *   
    *   G.X窗口专用库(glX) -- 是针对Unix和Linux的扩展函数
    *     1.初始化：glXQueryExtension
    *     2.渲染上下文函数：glXCreateContext、glXDestroyContext、glXCopyContext、glXMakeCurrent、glXCreateGLXPixmap
    *     3.执行：glXWaitGL、glXWaitX
    *     4.缓冲区和字体：glXSwapBuffers、glXUseXFont
    *
    *   H.其他扩展库 -- 可能是新的OpenGL函数，并没有在标准OpenGL库中实现，用来扩展已存在的OpenGL函数的功能。
    *     通常是硬件厂商为实现硬件更新利用OpenGL的扩展机制开发的函数。OpenGL扩展(OpenGL Extention)包含了大量的扩展API函数。
    *     放在 glext.h、glxext.h和wglext.h 等头文件中
    *****************************************************************************************************/

    #define OPENGL_VERIFY(x) \
        {\
            (x);\
            if(GL_NO_ERROR != (glErr = glGetError()))\
            {\
                REPORT_ERROR_INFO(FTL::CFOpenGLErrorInfo, glErr, x);\
            }\
        }

    FTLEXPORT class CFOpenGLErrorInfo : public CFConvertInfoT<CFOpenGLErrorInfo, GLenum>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFOpenGLErrorInfo);
    public:
        FTLINLINE explicit CFOpenGLErrorInfo(GLenum glErr);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };


    FTLEXPORT class CFOpenGLApp
    {
    public:
        FTLINLINE BOOL Initialize();
    };

    template <class T>
    FTLEXPORT class /*ATL_NO_VTABLE*/ CFOpenGLWndT : public ATL::CWindowImpl<T, CWindow, CFrameWinTraits>
    {
    public:
        DECLARE_WND_CLASS_EX(TEXT("FTLOpenGLWindow"), CS_VREDRAW | CS_HREDRAW | CS_OWNDC, -1)

        BEGIN_MSG_MAP(CFOpenGLWndT)
            MESSAGE_HANDLER(WM_CREATE, OnCreate)
            MESSAGE_HANDLER(WM_PAINT, OnPaint)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
            MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        END_MSG_MAP()

    public:
        CFOpenGLWndT();
        ~CFOpenGLWndT();
        BOOL CreateGLWindow(char* title, INT width, INT height, INT bits, BOOL fullscreenflag);
        BOOL SetFullScreenMode(BOOL bFullScreen, BOOL *pOldFullScreenMode);
        BOOL DrawGLScene(GLvoid);
        GLenum ReSizeGLScene(GLsizei width, GLsizei height);

    protected:
        HGLRC   m_hRC;          //窗口着色描述表(Rendering Context)句柄，将所有的OpenGL调用命令连接到Device Context(设备描述表)上
        HDC     m_hDC;    //OpenGL渲染设备描述表句柄
        BOOL    m_bFullScreen;  //全屏标志
        int     m_iPixelFormat;

        BOOL    OnInitScene();
        BOOL    OnFiniScene();
    private:
        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    };
}

#endif //FTL_OPENGL_H

#ifndef USE_EXPORT
#  include "ftlOpenGL.hpp"
#endif