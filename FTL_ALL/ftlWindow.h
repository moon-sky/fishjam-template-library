#ifndef FTL_WINDOW_H
#define FTL_WINDOW_H
#pragma once


#ifndef FTL_BASE_H
#  error ftlwindow.h requires ftlbase.h to be included first
#endif

/******************************************************************************************************
* WinProc 返回值：
*   0 -- 用户已经处理好消息；如 WM_CLOSE 中返回0则不会关闭，返回 DefaultWindowProc 才关闭(WM_DESTROY)
*
* SendMessage 后执行体是UI线程(后台线程会等待，直到UI线程执行完毕)
*   如果收到Send消息时，UI线程还在消息处理体中,则★不会立即强制抢断执行★,需要等待执行体结束后，
*   再通过GetMessage★优先★调用执行。
*   WTL的例子程序分析:
*     Send时:_GetMessage -> _DispatchClientMessage -> _InternalCallWinProc
*     Post时:_DispatchMessage -> DispatchMessageWorker -> _InternalCallWinProc
* 
* DefaultWindowProc -- 默认消息处理函数
*
* 注意：改变窗体大小时，会连续发送 WM_SIZE + WM_PAINT 消息，擦除背景，容易造成闪烁
* 
* 自绘控件：OWNERDRAW，在创建时处理 WM_MEASUREITEM 消息，改变外观时处理 WM_DRAWITEM 消息
* 
* 创建全屏窗体( WS_POPUP属性, 0,0 ~ CxScreen, CyScreen )：
*   hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP | WS_VISIBLE, 0, 0, 
*     GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);
*
* AdjustWindowRectEx -- 根据客户区的大小和窗口样式，计算并调整窗口的完整尺寸,然后MoveWindow进行调整即可
*   RECT rcClient = { 0,0,800,600 };
*   AdjustWindowRectEx( &rcClient, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));
* MapWindowPoints -- 把相对于一个窗口的坐标空间的一组点映射成相对于另一窗口的坐标空间的一组点
*
* 滚动窗体 -- 依靠更改MapMode、ViewportExt/WindowExt, ViewportOrg/WindowOrg 等实现
*   CScrollView 中有 m_totalDev, m_pageDev 等参数
*   CScrollView 的 SetScrollSize 是逻辑坐标
*
*
* ShowWindow 
*   SW_HIDE -- 隐藏，失活
*   SW_MINIMIZE -- 最小化，失活
*   SW_RESTORE -- 激活并恢复原显示
*   SW_SHOW -- 激活并显示
*   SW_SHOWMAXIMIZED -- 激活并最大化
*   SW_SHOWMINIMIZED -- 激活并图标化
*   SW_SHOWMINNOACTIVE -- 图标化但是不激活
*   SW_SHOWNA -- 显示并保持原激活状态
*   SW_SHOWNOACTIVATE -- 用最近的大小和位置显示并保持原激活状态
*   SW_SHOWNORMAL -- 激活并显示，必要时从最大最小化中恢复
*
******************************************************************************************************/

/******************************************************************************************************
* RichEdit -- CWindowImple<CEditView, CRichEditCtrl>
*   BitBlt 到 HDC 中，称为图片
*   StreamOutRtf -- 到Buffer中持久化
*
* SliderCtrl -- 滑块控件,滑块以创建时所指定的增量移动
*   常见属性、方法:
*     Buddy -- 关联窗口
*     LineSize -- 对应键盘的方向键
*     PageSize -- 对应PageUp、PageDown键
*     Selection-- 选择范围，高亮显示
*     ChannelRect -- 在刻度之上，供Thumb移动的区域
*     ThumbRect -- “拇指”(通过鼠标拖动的那个小方块)的范围
*     Tic/TicFreq -- 获取或设置显示刻度的位置/间隔
*   风格:
*     TBS_AUTOTICKS -- 滑动条具有刻度(?)
*     TBS_NOTICKS -- 滑动条不具有刻度(?)
*     TBS_ENABLESELRANGE -- 允许选择范围
*   要处理滑动条变化的事件，需要父窗体处理 WM_HSCROLL/WM_VSCROLL 消息(因为和水平滚动条公用同一个OnXScroll函数,
*     所以参数中的指针变量被定义为CScrollBar*类型，需要转换为CSliderCtrl*)
*     TRBN_THUMBPOSCHANGING Notify 事件不可以
*     
******************************************************************************************************/

/******************************************************************************************************
* 资源文件： RC -> RES，定义方式通常都是  ID 类型 文件名或资源名    （注意：只能有一个STRINGTABLE，没有名字）
*   CURSOR -- 光标
*   DIALOGEX --
*   ICON --
*   MENU -- DISCARDABLE指示什么？尽量丢弃？
*   STRINGTABLE -- 每个RC文件中只能有一个字符串表，每行字符串不能超过255个字符
*   WAVE -- 声音，使用 PlaySound 播放，SND_PURGE(停止播放)
******************************************************************************************************/

namespace FTL
{
#define DUMP_FILTER_MOUSE_MOVE              1
#define DUMP_FILTER_NCHITTEST               2
#define DUMP_FILTER_SETCURSOR               4

#define DEFAULT_DUMP_FILTER_MESSAGE         (DUMP_FILTER_MOUSE_MOVE\
    |DUMP_FILTER_NCHITTEST\
    |DUMP_FILTER_SETCURSOR)

    //在Output中Dump出当前接受到的消息
#ifdef FTL_DEBUG
#  define DUMP_WINDOWS_MSG(pszName, filters, uMsg, wParam, lParam) \
    {\
        BOOL bFilterd = FALSE;\
        if( (filters) & DUMP_FILTER_MOUSE_MESSAGE)\
        {\
			bFilterd = (WM_MOUSEMOVE == uMsg) ? TRUE : bFilterd; \
        }\
        if( (filters) & DUMP_FILTER_NCHITTEST )\
        {\
            bFilterd = (WM_NCHITTEST == uMsg) ? TRUE : bFilterd;\
        }\
        if( (filters) & DUMP_FILTER_SETCURSOR )\
        {\
            bFilterd = (WM_SETCURSOR == uMsg) ? TRUE : bFilterd;\
        }\
        if(!bFilterd)\
        {\
            FTLTRACE(TEXT("%s:%s, wParam=0x%x, lParam=0x%x\n"),\
            pszName, FTL::CFMessageInfo(uMsg).ConvertInfo(), wParam, lParam);\
        }\
    }
#else
#  define DUMP_WINDOWS_MSG(pszName, uMsg, filters) __noop
#endif 

    //! 将消息( WM_XXX )转换为易读的格式，类似于 ",wm"
    FTLEXPORT class CFMessageInfo : public CFConvertInfoT<CFMessageInfo,UINT>
    {
    public:
        FTLINLINE explicit CFMessageInfo(UINT msg);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    #define BEGIN_WINDOW_RESIZE_MAP(thisClass) \
        static const _WindowResizeMap* GetWindowResizeMap() \
        { \
            static const _WindowResizeMap theMap[] = \
            {

    #define END_WINDOW_RESIZE_MAP() \
                { (DWORD)(-1), 0 }, \
            }; \
            return theMap; \
        }

    #define WINDOW_RESIZE_CONTROL(id, flags) \
                { id, flags },

    //开始定义一个组，之后定义的Control的位置将以Group前的一个Control的位置为标准
    #define BEGIN_WINDOW_RESIZE_GROUP() \
                { -1, _WINSZ_BEGIN_GROUP },

    #define END_WINDOW_RESIZE_GROUP() \
                { -1, _WINSZ_END_GROUP },


    FTLEXPORT template <typename T>
    class CFMFCDlgAutoSizeTraits
    {
    public:
        static HWND GetWinHwndProxy(T* pThis)
        {
            return pThis->GetSafeHwnd();
        }
        static DWORD GetStatusBarCtrlID()
        {
            return AFX_IDW_STATUS_BAR;
        }
        //static LRESULT OnSizeProxy(T* pThis,UINT nType, int cx, int cy);
    };

    FTLEXPORT template <typename T>
    class CFWTLDlgAutoSizeTraits
    {
    public:
        static HWND GetWinHwndProxy(T* pThis)
        {
            return pThis->m_hWnd;
        }
        static DWORD GetStatusBarCtrlID()
        {
            return ATL_IDW_STATUS_BAR;
        }
        //static void OnSizeProxy(UINT nType, CSize size);
    };

    //! 从WTL的 CDialogResize 拷贝并改造的自动调整窗体控件位置、大小的类，支持MFC、WTL的Dialog
    //! TODO:支持普通的Windows，如 View；增加窗体位置保存、恢复等功能（扩展WTL）
    //! Bug:是否有 TWindowAutoSizeTraits 的必要？
    //! 使用方法：继承列表中增加 public FTL::CFWindowAutoSize<CMyDlg,FTL::CFWTLDlgAutoSizeTraits<CMyDlg> >
    //!   然后在 OnInitDialog 中调用 InitAutoSizeInfo，在 WM_SIZE 处理函数中调用 AutoResizeUpdateLayout，处理 WM_GETMINMAX...
    //! 窗体如果具有 WS_CLIPCHILDREN 属性，可以防止出现闪烁？但实际测试时发现Child时无法正确绘制
    FTLEXPORT template <typename T, typename TWindowAutoSizeTraits >
    class CFWindowAutoSize
    {
    public:
        // Data declarations and members
        enum
        {
            WINSZ_SIZE_X        = 0x00000001,   //自动横向扩展、伸缩，保证左、右边界不变
            WINSZ_SIZE_Y        = 0x00000002,   //自动纵向扩展、伸缩，保证上、下边界不变
            WINSZ_MOVE_X        = 0x00000004,   //自动横向移动，保证右边界不变
            WINSZ_MOVE_Y        = 0x00000008,   //自动纵向移动，保证下边界不变
            WINSZ_CENTER_X      = 0x00000010,   //自动横向移动，保证左、右边界不变
            WINSZ_CENTER_Y      = 0x00000020,   //自动纵向移动，保证上、下边界不变
            WINSZ_REPAINT       = 0x00000040,   //Resize后强制重绘，通常没有必要 

            // internal use only
            _WINSZ_BEGIN_GROUP  = 0x00001000,
            _WINSZ_END_GROUP    = 0x00002000,
            _WINSZ_GRIPPER      = 0x00004000    //内部使用，表明是拖拽的Gripper，只能有一个(内部设置)
        };
        struct _WindowResizeMap
        {
            DWORD   m_nCtlID;
            DWORD   m_dwResizeFlags;
        };
        struct _WindowResizeData
        {
            int     m_nCtlID;
            //! 0x   7   6   |    5   4   |    3   2   1   0    |
            //!      未用(0) |   Group数  |      WINSZ 标志     |
            DWORD   m_dwResizeFlags; 
            RECT    m_rect;

            int GetGroupCount() const
            {
                return (int)LOBYTE(HIWORD(m_dwResizeFlags));
            }

            void SetGroupCount(int nCount)
            {
                FTLASSERT(nCount > 0 && nCount < 256);
                FTLASSERT(( m_dwResizeFlags & 0x00FFFFFF ) == m_dwResizeFlags); //目前最高的一个字节没有内容
                DWORD dwCount = (DWORD)MAKELONG(0, MAKEWORD(nCount, 0));
                m_dwResizeFlags &= 0xFF00FFFF;
                m_dwResizeFlags |= dwCount;
            }

            bool operator ==(const _WindowResizeData& r) const
            { 
                return (m_nCtlID == r.m_nCtlID && m_dwResizeFlags == r.m_dwResizeFlags); 
            }
        };
        typedef std::vector<_WindowResizeData> WindowResizeDataArray;
        WindowResizeDataArray   m_allResizeData;
        SIZE    m_sizeWindow;
        POINT   m_ptMinTrackSize;
        BOOL    m_bGripper;
    public:
        FTLINLINE CFWindowAutoSize();
        FTLINLINE BOOL InitAutoSizeInfo(BOOL bAddGripper = TRUE, BOOL bUseMinTrackSize = TRUE);
        FTLINLINE BOOL AutoResizeUpdateLayout(int cxWidth, int cyHeight);
    protected:
        FTLINLINE BOOL AutoPositionControl(int cxWidth, int cyHeight, RECT& rectGroup, _WindowResizeData& data, bool bGroup, 
            _WindowResizeData* pDataPrev = NULL);
    };
    //管理显示器的信息(如双显)
    class CFMonitorManager
    {
    public:
    private:
        //static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
    };

    FTLEXPORT class CFWinUtil
    {
    public:
        //激活并将指定窗体放在最前方 -- 解决在状态栏闪烁但不到前台的Bug
        //常用于二重启动时将窗体显示在前台
        FTLINLINE static BOOL ActiveAndForegroundWindow(HWND hWnd);

        //全屏窗口：WS_POPUP|WS_VISIBLE 属性(不能是 WS_OVERLAPPEDWINDOW ), 0,0 ~ CxScreen, CyScreen
        FTLINLINE static BOOL SetWindowFullScreen(HWND hWnd,BOOL isFullScreen, BOOL &oldZoomedState);
        FTLINLINE static LPCDLGTEMPLATE LoadDialog(HMODULE hModuleRes, LPCTSTR szResource, HINSTANCE * phInstFoundIn);
        
        //WM_HSCROLL 或 WM_VSCROLL 的通知码
        FTLINLINE static LPCTSTR GetScrollBarCodeString(UINT  nSBCode);

        //获取 WM_NOTIFY 消息 Code 对应的字符串信息
        FTLINLINE static LPCTSTR GetNotifyCodeString(UINT nCode);

        //获取 Windows 窗体属性对应的字符串信息 
        FTLINLINE static LPCTSTR GetWindowStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));
        FTLINLINE static LPCTSTR GetWindowExStyleString(FTL::CFStringFormater& formater, HWND hWnd, LPCTSTR pszDivide = TEXT("|"));

#if 0
        //窗体居中放置(ATL有源码？)
        FTLINLINE static BOOL CenterWindow(HWND hWndCenter , HWND hWndParent, BOOL bCurrMonitor);
#endif
    };//CFWinUtil

    //DI项目中使用来适配Notification的基类
    template <typename T>
    class CFUIAdapterForWin
    {
    public:
        //typedef T     parent_class;

        //virtual bool PostNotification(XfNotificationPtr pNotify) = 0;
        virtual void UpdateUI() = 0;

        //static bool PostNotificationProxy(XfNotificationPtr pNotify, void* data)
        //{
        //    T* pThis = static_cast<T*>(data);
        //    return pThis->PostNotification(pNotify);
        //}
        static void UpdateUIProxy(void* data)
        {
            T* pThis = static_cast<T*>(data);
            pThis->UpdateUI();
        }
    };

    //对MessageBox进行Hook，可以更改按钮的文本，目前没有更改按钮的大小
    //! 用法：CFMessageBoxHook(m_hWnd, TEXT("MyClose"));
    //        ::MessageBox(...);
    #define PREV_WND_PROC_NAME  TEXT("PrevWndProc")
    class CFMessageBoxHook
    {
    public:
         //更改从线程弹出的对话框上的OK按钮文字
        FTLINLINE CFMessageBoxHook(DWORD dwThreadId, LPCTSTR pszOKString);
        FTLINLINE ~CFMessageBoxHook(void);
    private:
        FTLINLINE static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam);
        FTLINLINE static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    private:
        static HHOOK    s_hHook;
        static HWND     s_ProphWnd;
        static LPCTSTR  s_pszOKString;
    };

    class CFLockUpdate     
    { 
    public: 
        CFLockUpdate(HWND hwnd, BOOL bHighPrivilege = FALSE)
            :m_bHighPrivilege(bHighPrivilege)
            ,m_hwnd(hwnd) 
          { 
              if(m_hwnd==NULL)return; 
              if(m_bHighPrivilege) 
                  LockWindowUpdate(m_hwnd); 
              else 
                  SendMessage(m_hwnd,WM_SETREDRAW,FALSE,0); 
          } 
          ~CFLockUpdate() 
          { 
              if(m_hwnd==NULL)
              {
                  return; 
              }
              if(m_bHighPrivilege) 
                  LockWindowUpdate(NULL); 
              else 
                  SendMessage(m_hwnd,WM_SETREDRAW,TRUE,0); 
          } 
    private: 
        BOOL   m_bHighPrivilege; 
        HWND   m_hwnd; 
    };

#if 0
    template <typename ControlT , typename ConverterFun>
    class CControlPropertyHandleT
    {
    public:
        CControlPropertyHandleT(ControlT& control);//, ConverterFun& fun);
        FTLINLINE INT AddProperty(INT value);
    private:
        ControlT&       m_control;
        //ConverterFun&   m_fun;
    };
#endif

}//namespace FTL

#endif //FTL_WINDOW_H

#ifndef USE_EXPORT
#  include "ftlwindow.hpp"
#endif 